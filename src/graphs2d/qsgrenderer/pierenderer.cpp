// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <private/qgraphsview_p.h>
#include <private/pierenderer_p.h>
#include <private/qquickshape_p.h>
#include <private/qpieslice_p.h>
#include <QtGraphs/qpieseries.h>
#include <QtGraphs/qpieslice.h>

PieRenderer::PieRenderer(QQuickItem *parent)
    : QQuickItem(parent)
    , m_needUpdate(false)
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
        QPieSlicePrivate *d = QPieSlicePrivate::fromSlice(slice);
        if (!d->m_shapePath)
            d->m_shapePath = new QQuickShapePath(m_shape);
        QQuickShapePath *shapePath = d->m_shapePath;
        auto data = m_shape->data();
        data.append(&data, shapePath);

        auto pathElements = shapePath->pathElements();

        if (!d->m_arc) {
            d->m_arc = new QQuickPathArc(shapePath);
            pathElements.append(&pathElements, d->m_arc);
        }

        if (!d->m_lineToCenter) {
            d->m_lineToCenter = new QQuickPathLine(shapePath);
            pathElements.append(&pathElements, d->m_lineToCenter);
        }

        if (!d->m_lineFromCenter) {
            d->m_lineFromCenter = new QQuickPathLine(shapePath);
            pathElements.append(&pathElements, d->m_lineFromCenter);
        }
    }
}

void PieRenderer::updateSeries(QPieSeries *series)
{
    QPointF center = QPointF(size().width() * series->horizontalPosition(),
                             size().height() * series->verticalPosition());
    qreal radius = size().width() > size().height() ? size().height() : size().width();
    radius *= (.5 * series->size());

    qreal startX = qQNaN();
    qreal startY = qQNaN();

    QSeriesTheme *theme = series->theme();
    if (!theme)
        return;
    theme->setGraphSeriesCount(series->slices().size());

    int sliceIndex = 0;

    for (QPieSlice *slice : series->slices()) {
        QPieSlicePrivate *d = QPieSlicePrivate::fromSlice(slice);
        QQuickShapePath *shapePath = d->m_shapePath;
        auto data = m_shape->data();
        data.append(&data, shapePath);
        shapePath->setStrokeWidth(theme->borderWidth());
        shapePath->setStrokeColor(theme->graphSeriesBorderColor(sliceIndex));
        shapePath->setFillColor(theme->graphSeriesColor(sliceIndex));
        qreal radian = .0;
        if (qIsNaN(startX) || qIsNaN(startY)) {
            radian = qDegreesToRadians(slice->startAngle());
            startX = radius * qSin(radian);
            startY = radius * qCos(radian);
        }

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

        startX = pointX;
        startY = pointY;
        sliceIndex++;
    }
}
