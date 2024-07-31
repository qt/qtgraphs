// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGraphs/qpieseries.h>
#include <QtGraphs/qpieslice.h>
#include <private/pierenderer_p.h>
#include <private/qabstractseries_p.h>
#include <private/qgraphsview_p.h>
#include <private/qpieseries_p.h>
#include <private/qpieslice_p.h>
#include <private/qquickshape_p.h>
#include <QtQuick/private/qquicktext_p.h>

PieRenderer::PieRenderer(QGraphsView *graph)
    : QQuickItem(graph)
    , m_graph(graph)
{
    m_shape = new QQuickShape(this);
    m_shape->setParentItem(this);
    m_shape->setPreferredRendererType(QQuickShape::CurveRenderer);

    setFlag(QQuickItem::ItemHasContents);
    setClip(true);
}

PieRenderer::~PieRenderer() {}

void PieRenderer::setSize(QSizeF size)
{
    QQuickItem::setSize(size);
}

void PieRenderer::handlePolish(QPieSeries *series)
{
    for (QPieSlice *slice : series->slices()) {
        QPieSlicePrivate *d = slice->d_func();
        QQuickShapePath *shapePath = d->m_shapePath;
        auto pathElements = shapePath->pathElements();
        auto labelElements = d->m_labelPath->pathElements();
        auto labelItem = d->m_labelItem;

        QQuickShape *labelShape = d->m_labelShape;
        labelShape->setVisible(series->isVisible() && d->m_isLabelVisible);
        labelItem->setVisible(series->isVisible() && d->m_isLabelVisible);

        if (!series->isVisible()) {
            pathElements.clear(&pathElements);
            labelElements.clear(&labelElements);
            continue;
        } else if (pathElements.count(&pathElements) == 0) {
            pathElements.append(&pathElements, d->m_largeArc);
            pathElements.append(&pathElements, d->m_lineToCenter);
            pathElements.append(&pathElements, d->m_smallArc);
            pathElements.append(&pathElements, d->m_lineFromCenter);

            labelElements.append(&labelElements, d->m_labelArm);
            labelElements.append(&labelElements, d->m_labelUnderline);
        }

        if (!shapePath->parent())
            shapePath->setParent(m_shape);

        if (!d->m_labelItem->parent()) {
            d->m_labelItem->setParent(this);
            d->m_labelItem->setParentItem(this);
        }

        if (!labelShape->parent()) {
            labelShape->setParent(this);
            labelShape->setParentItem(this);
        }
    }

    if (!series->isVisible())
        return;

    QPointF center = QPointF(size().width() * series->horizontalPosition(),
                             size().height() * series->verticalPosition());
    qreal radius = size().width() > size().height() ? size().height() : size().width();
    radius *= (.5 * series->pieSize());

    QGraphsTheme *theme = m_graph->theme();

    if (!theme)
        return;

    if (m_colorIndex < 0)
        m_colorIndex = m_graph->graphSeriesCount();
    m_graph->setGraphSeriesCount(m_colorIndex + series->slices().size());

    int sliceIndex = 0;
    QList<QLegendData> legendDataList;
    for (QPieSlice *slice : series->slices()) {
        QPieSlicePrivate *d = slice->d_func();

        // update slice
        QQuickShapePath *shapePath = d->m_shapePath;

        const auto &borderColors = theme->borderColors();
        int index = sliceIndex % borderColors.size();
        QColor borderColor = borderColors.at(index);
        if (d->m_borderColor.isValid())
            borderColor = d->m_borderColor;
        qreal borderWidth = theme->borderWidth();
        if (d->m_borderWidth > 0.0)
            borderWidth = d->m_borderWidth;
        const auto &seriesColors = theme->seriesColors();
        index = sliceIndex % seriesColors.size();
        QColor color = seriesColors.at(index);
        if (d->m_color.isValid())
            color = d->m_color;
        shapePath->setStrokeWidth(borderWidth);
        shapePath->setStrokeColor(borderColor);
        shapePath->setFillColor(color);
        qreal radian = qDegreesToRadians(slice->startAngle());
        qreal startBigX = radius * qSin(radian);
        qreal startBigY = radius * qCos(radian);

        qreal startSmallX = startBigX * series->holeSize();
        qreal startSmallY = startBigY * series->holeSize();

        qreal explodeDistance = .0;
        if (slice->isExploded())
            explodeDistance = slice->explodeDistanceFactor() * radius;
        radian = qDegreesToRadians(slice->startAngle() + (slice->angleSpan() * .5));
        qreal xShift = center.x() + (explodeDistance * qSin(radian));
        qreal yShift = center.y() - (explodeDistance * qCos(radian));
        shapePath->setStartX(xShift + startBigX);
        shapePath->setStartY(yShift - startBigY);

        QQuickPathArc *pathArc = d->m_largeArc;
        radian = qDegreesToRadians(slice->angleSpan());
        qreal pointX = startBigY * qSin(radian) + startBigX * qCos(radian);
        qreal pointY = startBigY * qCos(radian) - startBigX * qSin(radian);
        pathArc->setX(xShift + pointX);
        pathArc->setY(yShift - pointY);
        pathArc->setRadiusX(radius);
        pathArc->setRadiusY(radius);
        if (slice->angleSpan() > 180)
            pathArc->setUseLargeArc(true);
        else
            pathArc->setUseLargeArc(false);

        pointX = startSmallY * qSin(radian) + startSmallX * qCos(radian);
        pointY = startSmallY * qCos(radian) - startSmallX * qSin(radian);

        QQuickPathLine *pathLine = d->m_lineToCenter;
        pathLine->setX(xShift + pointX);
        pathLine->setY(yShift - pointY);

        pathArc = d->m_smallArc;
        pathArc->setDirection(QQuickPathArc::Counterclockwise);
        pathArc->setX(xShift + startSmallX);
        pathArc->setY(yShift - startSmallY);
        pathArc->setRadiusX(radius * series->holeSize());
        pathArc->setRadiusY(radius * series->holeSize());
        if (slice->angleSpan() > 180)
            pathArc->setUseLargeArc(true);
        else
            pathArc->setUseLargeArc(false);

        pathLine = d->m_lineFromCenter;
        pathLine->setX(xShift + startBigX);
        pathLine->setY(yShift - startBigY);

        // Update label
        QQuickShapePath *labelPath = d->m_labelPath;
        radian = qDegreesToRadians(slice->startAngle() + (slice->angleSpan() * .5));
        startBigX = radius * qSin(radian);
        startBigY = radius * qCos(radian);
        labelPath->setStartX(xShift + startBigX);
        labelPath->setStartY(yShift - startBigY);

        QQuickPathLine *labelArm = d->m_labelArm;
        pointX = radius * (1.0 + d->m_labelArmLengthFactor) * qSin(radian);
        pointY = radius * (1.0 + d->m_labelArmLengthFactor) * qCos(radian);
        labelArm->setX(xShift + pointX);
        labelArm->setY(yShift - pointY);

        d->setLabelPosition(d->m_labelPosition);

        sliceIndex++;
        legendDataList.push_back({color, borderColor, d->m_labelText});
    }
    series->d_func()->setLegendData(legendDataList);
}

void PieRenderer::afterPolish(QList<QAbstractSeries *> &cleanupSeries)
{
    for (auto series : cleanupSeries) {
        auto pieSeries = qobject_cast<QPieSeries *>(series);
        if (pieSeries) {
            for (QPieSlice *slice : pieSeries->slices()) {
                QPieSlicePrivate *d = slice->d_func();
                QQuickShapePath *shapePath = d->m_shapePath;
                auto pathElements = shapePath->pathElements();
                auto labelElements = d->m_labelPath->pathElements();

                pathElements.clear(&pathElements);
                labelElements.clear(&labelElements);

                slice->deleteLater();
                d->m_labelItem->deleteLater();

                m_activeSlices.remove(slice);
            }
        }
    }
}

void PieRenderer::updateSeries(QPieSeries *series)
{
    for (QPieSlice *slice : series->slices()) {
        QPieSlicePrivate *d = slice->d_func();
        QQuickShapePath *shapePath = d->m_shapePath;

        if (!m_activeSlices.contains(slice)) {
            auto data = m_shape->data();
            data.append(&data, shapePath);
            m_activeSlices.insert(slice);

            handlePolish(series);
        }
    }
}

void PieRenderer::afterUpdate(QList<QAbstractSeries *> &cleanupSeries)
{
    Q_UNUSED(cleanupSeries);
}
