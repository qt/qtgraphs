// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGraphs/qlineseries.h>
#include <QtGraphs/qscatterseries.h>
#include <QtGraphs/qsplineseries.h>
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

PointRenderer::~PointRenderer()
{
}

void PointRenderer::handlePolish(QScatterSeries *series)
{
    if (series->points().isEmpty())
        return;

    if (!m_groups.contains(series)) {
        PointGroup *group = new PointGroup();
        group->series = series;
        m_groups.insert(series, group);
    }

    auto scatter = m_groups.value(series);

    int pointCount = series->points().size();

    if (series->pointMarker()) {
        int markerCount = scatter->markers.size();
        if (markerCount < pointCount) {
            for (int i = markerCount; i < pointCount; ++i) {
                QQuickItem *item = qobject_cast<QQuickItem *>(series->pointMarker()->create());
                item->setParentItem(this);
                scatter->markers << item;
                scatter->rects << QRectF();
            }
        }
    } else if (scatter->markers.size() > 0) {
        for (int i = 0; i < scatter->markers.size(); i++) {
            scatter->markers[i]->deleteLater();
        }
        scatter->markers.clear();
    }

    if (!series->pointMarker()) {
        int rectCount = scatter->nodes.size();
        if (rectCount < series->points().size()) {
            for (int i = rectCount; i < series->points().size(); ++i) {
                auto rect = new QSGDefaultInternalRectangleNode();
                scatter->nodes << rect;
                scatter->rects << QRectF();
            }
        }
    }

    if (scatter->colorIndex < 0) {
        scatter->colorIndex = m_currentColorIndex;
        m_currentColorIndex++;
    }

    auto seriesTheme = series->theme();
    if (seriesTheme) {
        auto &&colors = seriesTheme->colors();
        if (colors.size() > 0)
            series->setColor(colors[scatter->colorIndex % colors.size()]);
    }

    float w = width() - m_graph->m_marginLeft - m_graph->m_marginRight - m_graph->m_axisRenderer->m_axisWidth;
    float h = height() - m_graph->m_marginTop - m_graph->m_marginBottom - m_graph->m_axisRenderer->m_axisHeight;

    auto &&points = series->points();
    if (points.count() > 0) {
        double maxVertical = m_graph->m_axisRenderer->m_axisVerticalValueRange > 0
                                 ? 1.0 / m_graph->m_axisRenderer->m_axisVerticalValueRange : 100.0;
        double maxHorizontal = m_graph->m_axisRenderer->m_axisHorizontalValueRange > 0
                                   ? 1.0 / m_graph->m_axisRenderer->m_axisHorizontalValueRange : 100.0;
        double verticalOffset = (m_graph->m_axisRenderer->m_axisVerticalMinValue
                                 / m_graph->m_axisRenderer->m_axisVerticalValueRange) * h;
        double horizontalOffset = (m_graph->m_axisRenderer->m_axisHorizontalMinValue
                                   / m_graph->m_axisRenderer->m_axisHorizontalValueRange) * w;
        for (int i = 0; i < points.count(); ++i) {
            qreal x = m_graph->m_marginLeft + m_graph->m_axisRenderer->m_axisWidth + w * points[i].x() * maxHorizontal - horizontalOffset;
            qreal y = m_graph->m_marginTop + h - h * points[i].y() * maxVertical + verticalOffset;

            if (series->pointMarker()) {
                if (scatter->markers[i]->property("selected").isValid())
                    scatter->markers[i]->setProperty("selected", series->isPointSelected(i));
                scatter->markers[i]->setX(x - scatter->markers[i]->width() / 2.0);
                scatter->markers[i]->setY(y - scatter->markers[i]->height() / 2.0);

                scatter->rects[i] = QRectF(x - scatter->markers[i]->width() / 2.0, y - scatter->markers[i]->height() / 2.0,
                                           scatter->markers[i]->width(), scatter->markers[i]->height());
            } else {
                auto &pointItem = scatter->nodes[i];

                qreal markerSize = series->markerSize();
                scatter->rects[i] = QRectF(x - markerSize / 2.0, y - markerSize / 2.0, markerSize, markerSize);

                pointItem->setRect(scatter->rects[i]);
                QColor c = series->color();
                if (series->isPointSelected(i) && series->selectedColor().isValid())
                    c = series->selectedColor();
                c.setAlpha(c.alpha() * series->opacity());

                if (series->isPointSelected(i))
                    pointItem->setColor(c);
                else
                    pointItem->setColor(QColorConstants::Transparent);

                pointItem->setPenColor(c);
                pointItem->setPenWidth(2.0);
                // TODO: Required because of QTBUG-117892
                pointItem->setTopLeftRadius(-1);
                pointItem->setTopRightRadius(-1);
                pointItem->setBottomLeftRadius(-1);
                pointItem->setBottomRightRadius(-1);
                pointItem->setRadius(180.0);
                pointItem->setAntialiasing(true);
                pointItem->update();
            }
        }
    }
}

void PointRenderer::handlePolish(QSplineSeries *series)
{
    if (series->points().isEmpty())
        return;

    if (!m_groups.contains(series)) {
        PointGroup *group = new PointGroup();
        group->series = series;
        group->shapePath = new QQuickShapePath(&m_shape);
        m_groups.insert(series, group);

        auto data = m_shape.data();
        data.append(&data, m_groups.value(series)->shapePath);
    }

    auto line = m_groups.value(series);

    int pointCount = series->points().size();
    int currentSize = line->nodes.size();
    if (currentSize < pointCount) {
        auto pathElements = line->shapePath->pathElements();
        for (int i = currentSize; i < pointCount; ++i) {
            if (i < pointCount - 1) {
                auto path = new QQuickPathCubic(line->shapePath);
                pathElements.append(&pathElements, path);
                line->paths << path;
            }

            auto node = new QSGDefaultInternalRectangleNode();
            line->nodes << node;
            line->rects << QRectF();
        }
    }

    if (series->pointMarker()) {
        int markerCount = line->markers.size();
        if (markerCount < pointCount) {
            for (int i = markerCount; i < pointCount; ++i) {
                QQuickItem *item = qobject_cast<QQuickItem *>(series->pointMarker()->create());
                item->setParentItem(this);
                line->markers << item;
                line->rects << QRectF();
            }
        }
    } else if (line->markers.size() > 0) {
        for (int i = 0; i < line->markers.size(); i++)
            line->markers[i]->deleteLater();

        line->markers.clear();
    }

    if (line->colorIndex < 0) {
        line->colorIndex = m_currentColorIndex;
        m_currentColorIndex++;
    }

    auto seriesTheme = series->theme();
    if (seriesTheme) {
        auto &&colors = seriesTheme->colors();
        if (colors.size() > 0)
            series->setColor(colors[line->colorIndex % colors.size()]);
    }

    line->shapePath->setStrokeColor(series->color());
    line->shapePath->setStrokeWidth(series->width());
    line->shapePath->setFillColor(QColorConstants::Transparent);

    Qt::PenCapStyle capStyle = series->capStyle();
    if (capStyle == Qt::PenCapStyle::SquareCap)
        line->shapePath->setCapStyle(QQuickShapePath::CapStyle::SquareCap);
    else if (capStyle == Qt::PenCapStyle::FlatCap)
        line->shapePath->setCapStyle(QQuickShapePath::CapStyle::FlatCap);
    else if (capStyle == Qt::PenCapStyle::RoundCap)
        line->shapePath->setCapStyle(QQuickShapePath::CapStyle::RoundCap);

    // Line area width & height
    float w = width() - m_graph->m_marginLeft - m_graph->m_marginRight
              - m_graph->m_axisRenderer->m_axisWidth;
    float h = height() - m_graph->m_marginTop - m_graph->m_marginBottom
              - m_graph->m_axisRenderer->m_axisHeight;

    auto &&points = series->points();
    if (points.count() > 0) {
        auto fittedPoints = fitCubicSpline(points);

        double maxVertical = m_graph->m_axisRenderer->m_axisVerticalValueRange > 0
                                 ? 1.0 / m_graph->m_axisRenderer->m_axisVerticalValueRange
                                 : 100.0;
        double maxHorizontal = m_graph->m_axisRenderer->m_axisHorizontalValueRange > 0
                                   ? 1.0 / m_graph->m_axisRenderer->m_axisHorizontalValueRange
                                   : 100.0;
        double verticalOffset = (m_graph->m_axisRenderer->m_axisVerticalMinValue
                                 / m_graph->m_axisRenderer->m_axisVerticalValueRange)
                                * h;
        double horizontalOffset = (m_graph->m_axisRenderer->m_axisHorizontalMinValue
                                   / m_graph->m_axisRenderer->m_axisHorizontalValueRange)
                                  * w;

        for (int i = 0, j = 0; i < points.count(); ++i, ++j) {
            qreal x = m_graph->m_marginLeft + m_graph->m_axisRenderer->m_axisWidth
                      + w * points[i].x() * maxHorizontal - horizontalOffset;
            qreal y = m_graph->m_marginTop + h - h * points[i].y() * maxVertical + verticalOffset;

            if (i == 0) {
                line->shapePath->setStartX(x);
                line->shapePath->setStartY(y);
            } else {
                auto *cubicPath = qobject_cast<QQuickPathCubic *>(line->paths[i - 1]);

                qreal x1 = m_graph->m_marginLeft + m_graph->m_axisRenderer->m_axisWidth
                           + w * fittedPoints[j - 1].x() * maxHorizontal - horizontalOffset;
                qreal y1 = m_graph->m_marginTop + h - h * fittedPoints[j - 1].y() * maxVertical
                           + verticalOffset;
                qreal x2 = m_graph->m_marginLeft + m_graph->m_axisRenderer->m_axisWidth
                           + w * fittedPoints[j].x() * maxHorizontal - horizontalOffset;
                qreal y2 = m_graph->m_marginTop + h - h * fittedPoints[j].y() * maxVertical
                           + verticalOffset;

                ++j;

                cubicPath->setX(x);
                cubicPath->setY(y);

                cubicPath->setControl2X(x2);
                cubicPath->setControl1X(x1);
                cubicPath->setControl2Y(y2);
                cubicPath->setControl1Y(y1);
            }

            if (series->pointMarker()) {
                if (line->markers[i]->property("selected").isValid())
                    line->markers[i]->setProperty("selected", series->isPointSelected(i));
                line->markers[i]->setX(x - line->markers[i]->width() / 2.0);
                line->markers[i]->setY(y - line->markers[i]->height() / 2.0);

                line->rects[i] = QRectF(x - line->markers[i]->width() / 2.0,
                                        y - line->markers[i]->height() / 2.0,
                                        line->markers[i]->width(),
                                        line->markers[i]->height());
            } else if (series->selectable()) {
                auto &pointItem = line->nodes[i];

                qreal markerSize = series->markerSize();
                line->rects[i] = QRectF(x - markerSize / 2.0,
                                        y - markerSize / 2.0,
                                        markerSize,
                                        markerSize);

                pointItem->setRect(line->rects[i]);
                QColor c = series->color();
                if (series->isPointSelected(i) && series->selectedColor().isValid())
                    c = series->selectedColor();
                c.setAlpha(c.alpha() * series->opacity());

                if (series->isPointSelected(i))
                    pointItem->setColor(c);
                else
                    pointItem->setColor(QColorConstants::Transparent);

                pointItem->setPenColor(c);
                pointItem->setPenWidth(2.0);
                // TODO: Required because of QTBUG-117892
                pointItem->setTopLeftRadius(-1);
                pointItem->setTopRightRadius(-1);
                pointItem->setBottomLeftRadius(-1);
                pointItem->setBottomRightRadius(-1);
                pointItem->setRadius(180.0);
                pointItem->setAntialiasing(true);
                pointItem->update();
            }
        }
    }
}

void PointRenderer::handlePolish(QLineSeries *series)
{
    if (series->points().isEmpty())
        return;

    if (!m_groups.contains(series)) {
        PointGroup *group = new PointGroup();
        group->series = series;
        group->shapePath = new QQuickShapePath(&m_shape);
        m_groups.insert(series, group);

        auto data = m_shape.data();
        data.append(&data, m_groups.value(series)->shapePath);
    }

    auto line = m_groups.value(series);

    int pointCount = series->points().size();
    int currentSize = line->nodes.size();
    if (currentSize < pointCount) {
        auto pathElements = line->shapePath->pathElements();
        for (int i = currentSize; i < pointCount; ++i) {
            if (i < pointCount - 1) {
                auto path = new QQuickPathLine(line->shapePath);
                pathElements.append(&pathElements, path);
                line->paths << path;
            }

            auto node = new QSGDefaultInternalRectangleNode();
            line->nodes << node;
            line->rects << QRectF();
        }
    }

    if (series->pointMarker()) {
        int markerCount = line->markers.size();
        if (markerCount < pointCount) {
            for (int i = markerCount; i < pointCount; ++i) {
                QQuickItem *item = qobject_cast<QQuickItem *>(series->pointMarker()->create());
                item->setParentItem(this);
                line->markers << item;
                line->rects << QRectF();
            }
        }
    } else if (line->markers.size() > 0) {
        for (int i = 0; i < line->markers.size(); i++) {
            line->markers[i]->deleteLater();
        }
        line->markers.clear();
    }

    if (line->colorIndex < 0) {
        line->colorIndex = m_currentColorIndex;
        m_currentColorIndex++;
    }

    auto seriesTheme = series->theme();
    if (seriesTheme) {
        auto &&colors = seriesTheme->colors();
        if (colors.size() > 0)
            series->setColor(colors[line->colorIndex % colors.size()]);
    }

    line->shapePath->setStrokeColor(series->color());
    line->shapePath->setStrokeWidth(series->width());
    line->shapePath->setFillColor(QColorConstants::Transparent);

    Qt::PenCapStyle capStyle = series->capStyle();
    if (capStyle == Qt::PenCapStyle::SquareCap) {
        line->shapePath->setCapStyle(QQuickShapePath::CapStyle::SquareCap);
    } else if (capStyle == Qt::PenCapStyle::FlatCap) {
        line->shapePath->setCapStyle(QQuickShapePath::CapStyle::FlatCap);
    } else if (capStyle == Qt::PenCapStyle::RoundCap) {
        line->shapePath->setCapStyle(QQuickShapePath::CapStyle::RoundCap);
    }

    // Line area width & height
    float w = width() - m_graph->m_marginLeft - m_graph->m_marginRight - m_graph->m_axisRenderer->m_axisWidth;
    float h = height() - m_graph->m_marginTop - m_graph->m_marginBottom - m_graph->m_axisRenderer->m_axisHeight;

    auto &&points = series->points();
    if (points.count() > 0) {
        double maxVertical = m_graph->m_axisRenderer->m_axisVerticalValueRange > 0
                                 ? 1.0 / m_graph->m_axisRenderer->m_axisVerticalValueRange : 100.0;
        double maxHorizontal = m_graph->m_axisRenderer->m_axisHorizontalValueRange > 0
                                   ? 1.0 / m_graph->m_axisRenderer->m_axisHorizontalValueRange : 100.0;
        double verticalOffset = (m_graph->m_axisRenderer->m_axisVerticalMinValue
                                 / m_graph->m_axisRenderer->m_axisVerticalValueRange) * h;
        double horizontalOffset = (m_graph->m_axisRenderer->m_axisHorizontalMinValue
                                   / m_graph->m_axisRenderer->m_axisHorizontalValueRange) * w;
        for (int i = 0; i < points.count(); ++i) {
            qreal x = m_graph->m_marginLeft + m_graph->m_axisRenderer->m_axisWidth + w * points[i].x() * maxHorizontal - horizontalOffset;
            qreal y = m_graph->m_marginTop + h - h * points[i].y() * maxVertical + verticalOffset;

            if (i == 0) {
                line->shapePath->setStartX(x);
                line->shapePath->setStartY(y);
            } else {
                line->paths[i - 1]->setX(x);
                line->paths[i - 1]->setY(y);
            }

            if (series->pointMarker()) {
                if (line->markers[i]->property("selected").isValid())
                    line->markers[i]->setProperty("selected", series->isPointSelected(i));
                line->markers[i]->setX(x - line->markers[i]->width() / 2.0);
                line->markers[i]->setY(y - line->markers[i]->height() / 2.0);

                line->rects[i] = QRectF(x - line->markers[i]->width() / 2.0, y - line->markers[i]->height() / 2.0,
                                        line->markers[i]->width(), line->markers[i]->height());
            } else if (series->selectable()) {
                auto &pointItem = line->nodes[i];

                qreal markerSize = series->markerSize();
                line->rects[i] = QRectF(x - markerSize / 2.0, y - markerSize / 2.0, markerSize, markerSize);

                pointItem->setRect(line->rects[i]);
                QColor c = series->color();
                if (series->isPointSelected(i) && series->selectedColor().isValid())
                    c = series->selectedColor();
                c.setAlpha(c.alpha() * series->opacity());

                if (series->isPointSelected(i))
                    pointItem->setColor(c);
                else
                    pointItem->setColor(QColorConstants::Transparent);

                pointItem->setPenColor(c);
                pointItem->setPenWidth(2.0);
                // TODO: Required because of QTBUG-117892
                pointItem->setTopLeftRadius(-1);
                pointItem->setTopRightRadius(-1);
                pointItem->setBottomLeftRadius(-1);
                pointItem->setBottomRightRadius(-1);
                pointItem->setRadius(180.0);
                pointItem->setAntialiasing(true);
                pointItem->update();
            }
        }
    }
}

void PointRenderer::updateSeries(QXYSeries *series)
{
    auto group = m_groups.value(series);
    auto &&points = series->points();

    if (group->nodes.size() != points.size())
        return;

    for (int i = 0; i < points.count(); ++i) {
        auto &pointItem = group->nodes[i];
        if (!pointItem->parent() && m_graph->m_backgroundNode)
            m_graph->m_backgroundNode->appendChildNode(pointItem);
    }
}

void PointRenderer::handleMouseMove(QMouseEvent *event)
{
    if (m_pointPressed && m_pressedGroup->series->isPointSelected(m_pressedPointIndex)) {
        float w = width() - m_graph->m_marginLeft - m_graph->m_marginRight - m_graph->m_axisRenderer->m_axisWidth;
        float h = height() - m_graph->m_marginTop - m_graph->m_marginBottom - m_graph->m_axisRenderer->m_axisHeight;
        double maxVertical = m_graph->m_axisRenderer->m_axisVerticalValueRange > 0
                                 ? 1.0 / m_graph->m_axisRenderer->m_axisVerticalValueRange : 100.0;
        double maxHorizontal = m_graph->m_axisRenderer->m_axisHorizontalValueRange > 0
                                   ? 1.0 / m_graph->m_axisRenderer->m_axisHorizontalValueRange : 100.0;

        QPoint delta = m_pressStart - event->pos();

        qreal deltaX = -delta.x() / w / maxHorizontal;
        qreal deltaY = delta.y() / h / maxVertical;

        for (auto &&group : m_groups) {
            auto &&selectedPoints = group->series->selectedPoints();
            for (int index : selectedPoints) {
                QPointF point = group->series->at(index) + QPointF(deltaX, deltaY);
                group->series->replace(index, point);
            }
        }

        m_pressStart = event->pos();
        m_pointDragging = true;
    }
}

void PointRenderer::handleMousePress(QMouseEvent *event)
{
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
            }
            index++;
        }
    }
}

void PointRenderer::handleMouseRelease(QMouseEvent *event)
{
    if (!m_pointDragging && m_pointPressed && m_pressedGroup) {
        if (m_pressedGroup->rects[m_pressedPointIndex].contains(event->pos())) {
            if (m_pressedGroup->series->isPointSelected(m_pressedPointIndex)) {
                m_pressedGroup->series->deselectPoint(m_pressedPointIndex);
            } else {
                m_pressedGroup->series->selectPoint(m_pressedPointIndex);
            }
        }
    }
    m_pointPressed = false;
    m_pointDragging = false;
}

void PointRenderer::handleHoverMove(QHoverEvent *event)
{
    const QPointF &position = event->position();

    for (auto &&group : m_groups) {
        if (!group->series->hoverable())
            continue;

        if (qobject_cast<QScatterSeries*>(group->series)) {
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
                        qreal hoverDistance = qAbs((x2 - x1) * (y1 - y0) - (x1 - x0) * (y2 - y1)) / qSqrt(denominator);
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
                            }
                        }
                    }
                }

                if (!hovering && group->hover) {
                    group->hover = false;
                    emit group->series->hoverExit(name, position);
                }
            }
        }
    }
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
