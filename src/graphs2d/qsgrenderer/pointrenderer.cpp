// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGraphs/qlineseries.h>
#include <QtGraphs/qscatterseries.h>
#include <QtGraphs/qsplineseries.h>
#include <private/qabstractseries_p.h>
#include <private/pointrenderer_p.h>
#include <private/qgraphsview_p.h>

QT_BEGIN_NAMESPACE

PointRenderer::PointRenderer(QQuickItem *parent)
    : QQuickItem(parent)
{
    m_graph = qobject_cast<QGraphsView*>(parent);
    setFlag(QQuickItem::ItemHasContents);
    m_shape.setParentItem(this);
    m_shape.setPreferredRendererType(QQuickShape::CurveRenderer);
}

PointRenderer::~PointRenderer() {}

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
    QColor c = series->color();
    if (series->isPointSelected(pointIndex) && series->selectedColor().isValid())
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

void PointRenderer::updateLegendData(QXYSeries *series)
{
    QLegendData legendData;
    // TODO: When fill color is added to the scatterseries use it instead for the color.
    legendData.color = series->color();
    legendData.borderColor = series->color();
    legendData.label = series->name();
    QList<QLegendData> legendDataList = {legendData};
    series->d_ptr->setLegendData(legendDataList);
}

void PointRenderer::calculateRenderCoordinates(
    AxisRenderer *axisRenderer, qreal origX, qreal origY, qreal *renderX, qreal *renderY)
{
    *renderX = m_graph->m_marginLeft + axisRenderer->m_axisWidth
               + m_areaWidth * origX * m_maxHorizontal - m_horizontalOffset;
    *renderY = m_graph->m_marginTop + m_areaHeight - m_areaHeight * origY * m_maxVertical
               + m_verticalOffset;
}

void PointRenderer::updateScatterSeries(QScatterSeries *series)
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
}

void PointRenderer::updateLineSeries(QLineSeries *series)
{
    auto group = m_groups.value(series);

    group->shapePath->setStrokeColor(series->color());
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
}

void PointRenderer::updateSplineSeries(QSplineSeries *series)
{
    auto group = m_groups.value(series);

    group->shapePath->setStrokeColor(series->color());
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
        auto fittedPoints = fitCubicSpline(points);

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
}

void PointRenderer::handlePolish(QXYSeries *series)
{
    if (series->points().isEmpty())
        return;

    if (!m_groups.contains(series)) {
        PointGroup *group = new PointGroup();
        group->series = series;
        m_groups.insert(series, group);

        if (series->type() != QAbstractSeries::SeriesTypeScatter) {
            group->shapePath = new QQuickShapePath(&m_shape);
            auto data = m_shape.data();
            data.append(&data, m_groups.value(series)->shapePath);
        }
    }

    auto group = m_groups.value(series);

    int pointCount = series->points().size();
    int currentSize = group->rects.size();
    if (currentSize < pointCount) {
        for (int i = currentSize; i < pointCount; ++i) {
            if (i < pointCount - 1) {
                QQuickCurve *path = nullptr;
                if (series->type() == QAbstractSeries::SeriesTypeLine)
                    path = new QQuickPathLine(group->shapePath);
                else if (series->type() == QAbstractSeries::SeriesTypeSpline)
                    path = new QQuickPathCubic(group->shapePath);

                if (path) {
                    auto pathElements = group->shapePath->pathElements();
                    pathElements.append(&pathElements, path);
                    group->paths << path;
                }
            }

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
        group->colorIndex = m_currentColorIndex;
        m_currentColorIndex++;
    }

    auto seriesTheme = series->theme();
    if (seriesTheme) {
        auto &&colors = seriesTheme->colors();
        if (colors.size() > 0)
            series->setColor(colors[group->colorIndex % colors.size()]);
    }

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

    if (auto scatter = qobject_cast<QScatterSeries *>(series))
        updateScatterSeries(scatter);
    else if (auto line = qobject_cast<QLineSeries *>(series))
        updateLineSeries(line);
    else if (auto spline = qobject_cast<QSplineSeries *>(series))
        updateSplineSeries(spline);

    updateLegendData(series);
}

void PointRenderer::updateSeries(QXYSeries *series)
{
    if (series->pointMarker()
        || (series->type() != QAbstractSeries::SeriesTypeScatter && !series->selectable())) {
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

        if (group->series->type() == QAbstractSeries::SeriesTypeScatter) {
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

QList<QPointF> PointRenderer::fitCubicSpline(const QList<QPointF> &points)
{
    if (points.size() == 1)
        return {points[0], points[0]};

    QList<QPointF> controlPoints;
    controlPoints.resize(points.size() * 2 - 2);

    int n = points.size() - 1;

    if (n == 1) {
        //for n==1
        controlPoints[0].setX((2 * points[0].x() + points[1].x()) / 3);
        controlPoints[0].setY((2 * points[0].y() + points[1].y()) / 3);
        controlPoints[1].setX(2 * controlPoints[0].x() - points[0].x());
        controlPoints[1].setY(2 * controlPoints[0].y() - points[0].y());
        return controlPoints;
    }

    // Calculate first Bezier control points
    // Set of equations for P0 to Pn points.
    //
    //  |   2   1   0   0   ... 0   0   0   ... 0   0   0   |   |   P1_1    |   |   P1 + 2 * P0             |
    //  |   1   4   1   0   ... 0   0   0   ... 0   0   0   |   |   P1_2    |   |   4 * P1 + 2 * P2         |
    //  |   0   1   4   1   ... 0   0   0   ... 0   0   0   |   |   P1_3    |   |   4 * P2 + 2 * P3         |
    //  |   .   .   .   .   .   .   .   .   .   .   .   .   |   |   ...     |   |   ...                     |
    //  |   0   0   0   0   ... 1   4   1   ... 0   0   0   | * |   P1_i    | = |   4 * P(i-1) + 2 * Pi     |
    //  |   .   .   .   .   .   .   .   .   .   .   .   .   |   |   ...     |   |   ...                     |
    //  |   0   0   0   0   0   0   0   0   ... 1   4   1   |   |   P1_(n-1)|   |   4 * P(n-2) + 2 * P(n-1) |
    //  |   0   0   0   0   0   0   0   0   ... 0   2   7   |   |   P1_n    |   |   8 * P(n-1) + Pn         |
    //
    QList<qreal> list;
    list.resize(n);

    list[0] = points[0].x() + 2 * points[1].x();

    for (int i = 1; i < n - 1; ++i)
        list[i] = 4 * points[i].x() + 2 * points[i + 1].x();

    list[n - 1] = (8 * points[n - 1].x() + points[n].x()) / 2.0;

    const QList<qreal> xControl = firstControlPoints(list);

    list[0] = points[0].y() + 2 * points[1].y();

    for (int i = 1; i < n - 1; ++i)
        list[i] = 4 * points[i].y() + 2 * points[i + 1].y();

    list[n - 1] = (8 * points[n - 1].y() + points[n].y()) / 2.0;

    const QList<qreal> yControl = firstControlPoints(list);

    for (int i = 0, j = 0; i < n; ++i, ++j) {
        controlPoints[j].setX(xControl[i]);
        controlPoints[j].setY(yControl[i]);

        j++;

        if (i < n - 1) {
            controlPoints[j].setX(2 * points[i + 1].x() - xControl[i + 1]);
            controlPoints[j].setY(2 * points[i + 1].y() - yControl[i + 1]);
        } else {
            controlPoints[j].setX((points[n].x() + xControl[n - 1]) / 2);
            controlPoints[j].setY((points[n].y() + yControl[n - 1]) / 2);
        }
    }
    return controlPoints;
}

QList<qreal> PointRenderer::firstControlPoints(const QList<qreal> &list)
{
    QList<qreal> result;

    int count = list.size();
    result.resize(count);
    result[0] = list[0] / 2.0;

    QList<qreal> temp;
    temp.resize(count);
    temp[0] = 0;

    qreal b = 2.0;

    for (int i = 1; i < count; i++) {
        temp[i] = 1 / b;
        b = (i < count - 1 ? 4.0 : 3.5) - temp[i];
        result[i] = (list[i] - result[i - 1]) / b;
    }

    for (int i = 1; i < count; i++)
        result[count - i - 1] -= temp[count - i] * result[count - i];

    return result;
}

QT_END_NAMESPACE
