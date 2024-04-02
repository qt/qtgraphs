// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGraphs/qlineseries.h>
#include <QtGraphs/qscatterseries.h>
#include <QtGraphs/qsplineseries.h>
#include <private/pointrenderer_p.h>
#include <private/qabstractseries_p.h>
#include <private/qgraphsview_p.h>
#include <private/qxyseries_p.h>

QT_BEGIN_NAMESPACE

PointRenderer::PointRenderer(QQuickItem *parent)
    : QQuickItem(parent)
{
    m_graph = qobject_cast<QGraphsView *>(parent);
    setFlag(QQuickItem::ItemHasContents);
    m_shape.setParentItem(this);
    m_shape.setPreferredRendererType(QQuickShape::CurveRenderer);
}

PointRenderer::~PointRenderer() {}

void PointRenderer::calculateRenderCoordinates(
    AxisRenderer *axisRenderer, qreal origX, qreal origY, qreal *renderX, qreal *renderY)
{
    *renderX = m_graph->m_marginLeft + axisRenderer->m_axisWidth
               + m_areaWidth * origX * m_maxHorizontal - m_horizontalOffset;
    *renderY = m_graph->m_marginTop + m_areaHeight - m_areaHeight * origY * m_maxVertical
               + m_verticalOffset;
}

void PointRenderer::updatePointMarker(
    QXYSeries *series, PointGroup *group, int pointIndex, qreal x, qreal y)
{
    auto marker = group->markers[pointIndex];
    auto &rect = group->rects[pointIndex];

    if (marker->property("selected").isValid())
        marker->setProperty("selected", series->isPointSelected(pointIndex));
    marker->setX(x - marker->width() / 2.0);
    marker->setY(y - marker->height() / 2.0);

    rect = QRectF(x - marker->width() / 2.0,
                  y - marker->height() / 2.0,
                  marker->width(),
                  marker->height());
}

void PointRenderer::updateRenderablePoint(QXYSeries *series,
                                          PointRenderer::PointGroup *group,
                                          int pointIndex)
{
    auto &node = group->nodes[pointIndex];
    if (!node->parent() && m_graph->m_backgroundNode)
        m_graph->m_backgroundNode->appendChildNode(node);

    node->setRect(group->rects[pointIndex]);

    QColor c = series->color().alpha() != 0 ? series->color()
                                            : series->theme()->graphSeriesColor(group->colorIndex);

    if (series->isPointSelected(pointIndex) && series->selectedColor().alpha() != 0)
        c = series->selectedColor();

    c.setAlpha(c.alpha() * series->opacity());

    if (series->isPointSelected(pointIndex))
        node->setColor(c);
    else
        node->setColor(QColorConstants::Transparent);

    node->setPenColor(c);
    node->setPenWidth(2.0);
    // TODO: Required because of QTBUG-117892
    node->setTopLeftRadius(-1);
    node->setTopRightRadius(-1);
    node->setBottomLeftRadius(-1);
    node->setBottomRightRadius(-1);
    node->setRadius(180.0);
    node->setAntialiasing(true);
    node->update();
}

void PointRenderer::updateLegendData(QXYSeries *series, QLegendData &legendData)
{
    QList<QLegendData> legendDataList = {legendData};
    series->d_func()->setLegendData(legendDataList);
}

void PointRenderer::updateScatterSeries(QScatterSeries *series, QLegendData &legendData)
{
    auto group = m_groups.value(series);

    auto &&points = series->points();
    if (points.count() > 0) {
        for (int i = 0; i < points.count(); ++i) {
            qreal x, y;
            calculateRenderCoordinates(m_graph->m_axisRenderer, points[i].x(), points[i].y(), &x, &y);

            if (series->pointMarker()) {
                updatePointMarker(series, group, i, x, y);
            } else {
                qreal markerSize = series->markerSize();
                group->rects[i] = QRectF(x - markerSize / 2.0,
                                         y - markerSize / 2.0,
                                         markerSize,
                                         markerSize);
            }
        }
    }
    // TODO: When fill color is added to the scatterseries use it instead for
    // the color. QTBUG-122434
    legendData = {series->color(), series->color(), series->name()};
}

void PointRenderer::updateLineSeries(QLineSeries *series, QLegendData &legendData)
{
    auto group = m_groups.value(series);

    QColor color = series->color().alpha() != 0
                       ? series->color()
                       : series->theme()->graphSeriesColor(group->colorIndex);

    group->shapePath->setStrokeColor(color);
    group->shapePath->setStrokeWidth(series->width());
    group->shapePath->setFillColor(QColorConstants::Transparent);

    Qt::PenCapStyle capStyle = series->capStyle();
    if (capStyle == Qt::PenCapStyle::SquareCap)
        group->shapePath->setCapStyle(QQuickShapePath::CapStyle::SquareCap);
    else if (capStyle == Qt::PenCapStyle::FlatCap)
        group->shapePath->setCapStyle(QQuickShapePath::CapStyle::FlatCap);
    else if (capStyle == Qt::PenCapStyle::RoundCap)
        group->shapePath->setCapStyle(QQuickShapePath::CapStyle::RoundCap);

    auto &&points = series->points();
    if (points.count() > 0) {
        for (int i = 0; i < points.count(); ++i) {
            qreal x, y;
            calculateRenderCoordinates(m_graph->m_axisRenderer, points[i].x(), points[i].y(), &x, &y);

            if (i == 0) {
                group->shapePath->setStartX(x);
                group->shapePath->setStartY(y);
            } else {
                group->paths[i - 1]->setX(x);
                group->paths[i - 1]->setY(y);
            }

            if (series->pointMarker()) {
                updatePointMarker(series, group, i, x, y);
            } else if (series->selectable()) {
                qreal markerSize = series->markerSize();
                group->rects[i] = QRectF(x - markerSize / 2.0,
                                         y - markerSize / 2.0,
                                         markerSize,
                                         markerSize);
            }
        }
    }
    legendData = {color, color, series->name()};
}

void PointRenderer::updateSplineSeries(QSplineSeries *series, QLegendData &legendData)
{
    auto group = m_groups.value(series);

    QColor color = series->color().alpha() != 0
                       ? series->color()
                       : series->theme()->graphSeriesColor(group->colorIndex);

    group->shapePath->setStrokeColor(color);
    group->shapePath->setStrokeWidth(series->width());
    group->shapePath->setFillColor(QColorConstants::Transparent);

    Qt::PenCapStyle capStyle = series->capStyle();
    if (capStyle == Qt::PenCapStyle::SquareCap)
        group->shapePath->setCapStyle(QQuickShapePath::CapStyle::SquareCap);
    else if (capStyle == Qt::PenCapStyle::FlatCap)
        group->shapePath->setCapStyle(QQuickShapePath::CapStyle::FlatCap);
    else if (capStyle == Qt::PenCapStyle::RoundCap)
        group->shapePath->setCapStyle(QQuickShapePath::CapStyle::RoundCap);

    auto &&points = series->points();
    if (points.count() > 0) {
        auto fittedPoints = series->getControlPoints();

        for (int i = 0, j = 0; i < points.count(); ++i, ++j) {
            qreal x, y;
            calculateRenderCoordinates(m_graph->m_axisRenderer, points[i].x(), points[i].y(), &x, &y);

            if (i == 0) {
                group->shapePath->setStartX(x);
                group->shapePath->setStartY(y);
            } else {
                auto *cubicPath = qobject_cast<QQuickPathCubic *>(group->paths[i - 1]);

                qreal x1, y1, x2, y2;
                calculateRenderCoordinates(m_graph->m_axisRenderer,
                                           fittedPoints[j - 1].x(),
                                           fittedPoints[j - 1].y(),
                                           &x1,
                                           &y1);
                calculateRenderCoordinates(m_graph->m_axisRenderer,
                                           fittedPoints[j].x(),
                                           fittedPoints[j].y(),
                                           &x2,
                                           &y2);

                ++j;

                cubicPath->setX(x);
                cubicPath->setY(y);

                cubicPath->setControl2X(x2);
                cubicPath->setControl1X(x1);
                cubicPath->setControl2Y(y2);
                cubicPath->setControl1Y(y1);
            }

            if (series->pointMarker()) {
                updatePointMarker(series, group, i, x, y);
            } else if (series->selectable()) {
                qreal markerSize = series->markerSize();
                group->rects[i] = QRectF(x - markerSize / 2.0,
                                         y - markerSize / 2.0,
                                         markerSize,
                                         markerSize);
            }
        }
    }
    legendData = {color, color, series->name()};
}

void PointRenderer::handlePolish(QXYSeries *series)
{
    auto seriesTheme = series->theme();
    if (!seriesTheme)
        return;

    if (series->points().isEmpty())
        return;

    m_areaWidth = width() - m_graph->m_marginLeft - m_graph->m_marginRight
                  - m_graph->m_axisRenderer->m_axisWidth;
    m_areaHeight = height() - m_graph->m_marginTop - m_graph->m_marginBottom
                   - m_graph->m_axisRenderer->m_axisHeight;

    m_maxVertical = m_graph->m_axisRenderer->m_axisVerticalValueRange > 0
                        ? 1.0 / m_graph->m_axisRenderer->m_axisVerticalValueRange
                        : 100.0;
    m_maxHorizontal = m_graph->m_axisRenderer->m_axisHorizontalValueRange > 0
                          ? 1.0 / m_graph->m_axisRenderer->m_axisHorizontalValueRange
                          : 100.0;
    m_verticalOffset = (m_graph->m_axisRenderer->m_axisVerticalMinValue
                        / m_graph->m_axisRenderer->m_axisVerticalValueRange)
                       * m_areaHeight;
    m_horizontalOffset = (m_graph->m_axisRenderer->m_axisHorizontalMinValue
                          / m_graph->m_axisRenderer->m_axisHorizontalValueRange)
                         * m_areaWidth;

    if (!m_groups.contains(series)) {
        PointGroup *group = new PointGroup();
        group->series = series;
        m_groups.insert(series, group);

        if (series->type() != QAbstractSeries::SeriesType::Scatter) {
            group->shapePath = new QQuickShapePath(&m_shape);
            auto data = m_shape.data();
            data.append(&data, m_groups.value(series)->shapePath);
        }
    }

    auto group = m_groups.value(series);

    int pointCount = series->points().size();
    int currentSize = group->rects.size();
    if (currentSize < pointCount) {
        for (int i = std::max(0, currentSize - 1); i < pointCount; ++i) {
            if (i < pointCount - 1) {
                QQuickCurve *path = nullptr;
                if (series->type() == QAbstractSeries::SeriesType::Line)
                    path = new QQuickPathLine(group->shapePath);
                else if (series->type() == QAbstractSeries::SeriesType::Spline)
                    path = new QQuickPathCubic(group->shapePath);

                if (path) {
                    auto pathElements = group->shapePath->pathElements();
                    pathElements.append(&pathElements, path);
                    group->paths << path;
                }
            }

            if (currentSize == 0 || (currentSize > 0 && i < pointCount - 1))
                group->rects << QRectF();
        }
    }

    if (series->pointMarker()) {
        int markerCount = group->markers.size();
        if (markerCount < pointCount) {
            for (int i = markerCount; i < pointCount; ++i) {
                QQuickItem *item = qobject_cast<QQuickItem *>(
                    series->pointMarker()->create(series->pointMarker()->creationContext()));
                item->setParentItem(this);
                group->markers << item;
            }
        }
    } else if (group->markers.size() > 0) {
        for (int i = 0; i < group->markers.size(); i++)
            group->markers[i]->deleteLater();
        group->markers.clear();
    }

    if (group->colorIndex < 0) {
        group->colorIndex = seriesTheme->graphSeriesCount();
        seriesTheme->setGraphSeriesCount(group->colorIndex + 1);
    }

    QLegendData legendData;
    if (auto scatter = qobject_cast<QScatterSeries *>(series))
        updateScatterSeries(scatter, legendData);
    else if (auto line = qobject_cast<QLineSeries *>(series))
        updateLineSeries(line, legendData);
    else if (auto spline = qobject_cast<QSplineSeries *>(series))
        updateSplineSeries(spline, legendData);

    updateLegendData(series, legendData);
}

void PointRenderer::updateSeries(QXYSeries *series)
{
    if (series->pointMarker()
        || (series->type() != QAbstractSeries::SeriesType::Scatter && !series->selectable())) {
        return;
    }

    auto group = m_groups.value(series);
    int nodeCount = group->nodes.size();
    int pointCount = series->points().size();

    for (int i = nodeCount; i < pointCount; ++i)
        group->nodes << new QSGDefaultInternalRectangleNode();

    for (int i = 0; i < pointCount; ++i)
        updateRenderablePoint(series, group, i);
}

bool PointRenderer::handleMouseMove(QMouseEvent *event)
{
    bool handled = false;
    if (m_pointPressed && m_pressedGroup->series->isPointSelected(m_pressedPointIndex)) {
        float w = width() - m_graph->m_marginLeft - m_graph->m_marginRight
                  - m_graph->m_axisRenderer->m_axisWidth;
        float h = height() - m_graph->m_marginTop - m_graph->m_marginBottom
                  - m_graph->m_axisRenderer->m_axisHeight;
        double maxVertical = m_graph->m_axisRenderer->m_axisVerticalValueRange > 0
                                 ? 1.0 / m_graph->m_axisRenderer->m_axisVerticalValueRange
                                 : 100.0;
        double maxHorizontal = m_graph->m_axisRenderer->m_axisHorizontalValueRange > 0
                                   ? 1.0 / m_graph->m_axisRenderer->m_axisHorizontalValueRange
                                   : 100.0;

        QPoint delta = m_pressStart - event->pos();

        qreal deltaX = -delta.x() / w / maxHorizontal;
        qreal deltaY = delta.y() / h / maxVertical;

        for (auto &&group : m_groups) {
            auto &&selectedPoints = group->series->selectedPoints();
            for (int index : selectedPoints) {
                QPointF point = group->series->at(index) + QPointF(deltaX, deltaY);
                group->series->replace(index, point);
                handled = true;
            }
        }

        m_pressStart = event->pos();
        m_pointDragging = true;
    }
    return handled;
}

bool PointRenderer::handleMousePress(QMouseEvent *event)
{
    bool handled = false;
    for (auto &&group : m_groups) {
        if (!group->series->selectable())
            continue;

        int index = 0;
        for (auto &&rect : group->rects) {
            if (rect.contains(event->pos())) {
                m_pointPressed = true;
                m_pressStart = event->pos();
                m_pressedGroup = group;
                m_pressedPointIndex = index;
                handled = true;
            }
            index++;
        }
    }
    return handled;
}

bool PointRenderer::handleMouseRelease(QMouseEvent *event)
{
    bool handled = false;
    if (!m_pointDragging && m_pointPressed && m_pressedGroup) {
        if (m_pressedGroup->rects[m_pressedPointIndex].contains(event->pos())) {
            if (m_pressedGroup->series->isPointSelected(m_pressedPointIndex)) {
                m_pressedGroup->series->deselectPoint(m_pressedPointIndex);
            } else {
                m_pressedGroup->series->selectPoint(m_pressedPointIndex);
            }
            handled = true;
        }
    }
    m_pointPressed = false;
    m_pointDragging = false;
    return handled;
}

bool PointRenderer::handleHoverMove(QHoverEvent *event)
{
    bool handled = false;
    const QPointF &position = event->position();

    for (auto &&group : m_groups) {
        if (!group->series->hoverable())
            continue;

        if (group->series->type() == QAbstractSeries::SeriesType::Scatter) {
            const QString &name = group->series->name();

            bool hovering = false;

            int index = 0;
            for (auto &&rect : group->rects) {
                if (rect.contains(position.toPoint())) {
                    if (!group->hover) {
                        group->hover = true;
                        emit group->series->hoverEnter(name, position, group->series->at(index));
                    }
                    emit group->series->hover(name, position, group->series->at(index));
                    hovering = true;
                }
                index++;
            }

            if (!hovering && group->hover) {
                group->hover = false;
                emit group->series->hoverExit(name, position);
            }
        } else {
            const qreal x0 = event->position().x();
            const qreal y0 = event->position().y();

            const int hoverSize = group->series->markerSize() / 2;
            const QString &name = group->series->name();
            auto &&points = group->series->points();

            if (points.size() >= 2) {
                bool hovering = false;

                for (int i = 0; i < points.size() - 1; i++) {
                    qreal x1, y1, x2, y2;

                    if (i == 0) {
                        x1 = group->shapePath->startX();
                        y1 = group->shapePath->startY();
                        x2 = group->paths[0]->x();
                        y2 = group->paths[0]->y();
                    } else {
                        x1 = group->paths[i - 1]->x();
                        y1 = group->paths[i - 1]->y();
                        x2 = group->paths[i]->x();
                        y2 = group->paths[i]->y();
                    }

                    qreal denominator = (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
                    if (denominator > 0) {
                        qreal hoverDistance = qAbs((x2 - x1) * (y1 - y0) - (x1 - x0) * (y2 - y1))
                                              / qSqrt(denominator);
                        if (hoverDistance < hoverSize) {
                            qreal alpha = 0;
                            qreal extrapolation = 0;
                            if (x2 - x1 >= y2 - y1) {
                                if (x2 - x1 != 0) {
                                    alpha = ((x2 - x1) - (x0 - x1)) / qAbs(x2 - x1);
                                    extrapolation = hoverSize / qAbs(x2 - x1);
                                }
                            } else {
                                if (y2 - y1 != 0) {
                                    alpha = ((y2 - y1) - (y0 - y1)) / qAbs(y2 - y1);
                                    extrapolation = hoverSize / qAbs(y2 - y1);
                                }
                            }

                            if (alpha >= -extrapolation && alpha <= 1.0 + extrapolation) {
                                const QPointF &point1 = points[i];
                                const QPointF &point2 = points[i + 1];

                                QPointF point = (point2 * (1.0 - alpha)) + (point1 * alpha);

                                if (!group->hover) {
                                    group->hover = true;
                                    emit group->series->hoverEnter(name, position, point);
                                }

                                emit group->series->hover(name, position, point);
                                hovering = true;
                                handled = true;
                            }
                        }
                    }
                }

                if (!hovering && group->hover) {
                    group->hover = false;
                    emit group->series->hoverExit(name, position);
                    handled = true;
                }
            }
        }
    }
    return handled;
}

QT_END_NAMESPACE
