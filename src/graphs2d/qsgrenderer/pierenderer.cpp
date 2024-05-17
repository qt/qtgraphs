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

PieRenderer::PieRenderer(QQuickItem *parent)
    : QQuickItem(parent)
{
    m_graph = qobject_cast<QGraphsView *>(parent);

    m_shape = new QQuickShape(this);
    m_shape->setParentItem(this);
    m_shape->setPreferredRendererType(QQuickShape::CurveRenderer);

    setFlag(QQuickItem::ItemHasContents);
}

void PieRenderer::setSize(QSizeF size)
{
    QQuickItem::setSize(size);
}

void PieRenderer::handlePolish(QPieSeries *series)
{
    for (QPieSlice *slice : series->slices()) {
        QPieSlicePrivate *d = slice->d_func();
        QQuickShapePath *shapePath = d->m_shapePath;
        if (shapePath->parent())
            shapePath->setParent(m_shape);
        auto data = m_shape->data();
        data.append(&data, shapePath);

        if (!d->m_labelItem->parent()) {
            d->m_labelItem->setParent(m_graph);
            d->m_labelItem->setParentItem(m_graph);
        }

        QQuickShape *labelShape = d->m_labelShape;
        if (!labelShape->parent()) {
            labelShape->setParent(this);
            labelShape->setParentItem(this);
        }
    }
}

void PieRenderer::updateSeries(QPieSeries *series)
{
    QPointF center = QPointF(size().width() * series->horizontalPosition(),
                             size().height() * series->verticalPosition());
    qreal radius = size().width() > size().height() ? size().height() : size().width();
    radius *= (.5 * series->size());

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
