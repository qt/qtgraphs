// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <private/qabstractseries_p.h>
#include <private/qgraphsview_p.h>
#include <private/pierenderer_p.h>
#include <private/qquickshape_p.h>
#include <private/qpieslice_p.h>
#include <QtGraphs/qpieseries.h>
#include <QtGraphs/qpieslice.h>

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

    QSeriesTheme *theme = series->theme();
    if (!theme)
        return;
    theme->setGraphSeriesCount(series->slices().size());

    int sliceIndex = 0;
    QList<QLegendData> legendDataList;
    for (QPieSlice *slice : series->slices()) {
        QPieSlicePrivate *d = slice->d_func();

        // update slice
        QQuickShapePath *shapePath = d->m_shapePath;

        QColor borderColor = theme->graphSeriesBorderColor(sliceIndex);
        QColor color = theme->graphSeriesColor(sliceIndex);
        shapePath->setStrokeWidth(theme->borderWidth());
        shapePath->setStrokeColor(borderColor);
        shapePath->setFillColor(color);
        qreal radian = qDegreesToRadians(slice->startAngle());
        qreal startX = radius * qSin(radian);
        qreal startY = radius * qCos(radian);

        qreal explodeDistance = .0;
        if (slice->isExploded())
            explodeDistance = slice->explodeDistanceFactor() * radius;
        radian = qDegreesToRadians(slice->startAngle() + (slice->angleSpan() * .5));
        qreal xShift = center.x() + (explodeDistance * qSin(radian));
        qreal yShift = center.y() - (explodeDistance * qCos(radian));
        shapePath->setStartX(xShift + startX);
        shapePath->setStartY(yShift - startY);

        QQuickPathArc *pathArc = d->m_arc;
        radian = qDegreesToRadians(slice->angleSpan());
        qreal pointX = startY * qSin(radian) + startX * qCos(radian);
        qreal pointY = startY * qCos(radian) - startX * qSin(radian);
        pathArc->setX(xShift + pointX);
        pathArc->setY(yShift - pointY);
        pathArc->setRadiusX(radius);
        pathArc->setRadiusY(radius);
        if (slice->angleSpan() > 180)
            pathArc->setUseLargeArc(true);
        else
            pathArc->setUseLargeArc(false);

        QQuickPathLine *pathLine = d->m_lineToCenter;
        pathLine->setX(xShift);
        pathLine->setY(yShift);

        pathLine = d->m_lineFromCenter;
        pathLine->setX(xShift + startX);
        pathLine->setY(yShift - startY);

        // Update label
        QQuickShapePath *labelPath = d->m_labelPath;
        radian = qDegreesToRadians(slice->startAngle() + (slice->angleSpan() * .5));
        startX = radius * qSin(radian);
        startY = radius * qCos(radian);
        labelPath->setStartX(xShift + startX);
        labelPath->setStartY(yShift - startY);

        QQuickPathLine *labelArm = d->m_labelArm;
        pointX = radius * (1.0 + d->m_labelArmLengthFactor) * qSin(radian);
        pointY = radius * (1.0 + d->m_labelArmLengthFactor) * qCos(radian);
        labelArm->setX(xShift + pointX);
        labelArm->setY(yShift - pointY);

        d->setLabelPosition(d->m_labelPosition);

        sliceIndex++;
        legendDataList.push_back({color, borderColor, d->m_labelText});
    }
    series->d_ptr->setLegendData(legendDataList);
}
