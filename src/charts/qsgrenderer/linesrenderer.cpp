// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <private/linesrenderer_p.h>
#include <private/qgraphsview_p.h>
#include <QtGraphs2D/qlineseries.h>

LinesRenderer::LinesRenderer(QQuickItem *parent)
    : QQuickItem(parent)
{
    m_graph = qobject_cast<QGraphsView*>(parent);
    setFlag(QQuickItem::ItemHasContents);
    m_shape.setParentItem(this);
    m_shape.setPreferredRendererType(QQuickShape::CurveRenderer);
}

LinesRenderer::~LinesRenderer()
{
    for (auto&& line : std::as_const(m_linePaths))
        delete line;
}

void LinesRenderer::handlePolish(QLineSeries *series)
{
    if (!m_linePaths.contains(series)) {
        LinePath *linePath = new LinePath();
        linePath->series = series;
        linePath->shapePath = new QQuickShapePath(&m_shape);
        m_linePaths.insert(series, linePath);

        auto data = m_shape.data();
        data.append(&data, m_linePaths.value(series)->shapePath);
    }

    auto line = m_linePaths.value(series);

    int pointCount = series->points().size();
    int currentSize = line->paths.size();
    if (currentSize < pointCount - 1) {
        auto pathElements = line->shapePath->pathElements();
        for (int i = currentSize; i < pointCount - 1; ++i) {
            auto path = new QQuickPathLine(line->shapePath);
            pathElements.append(&pathElements, path);
            line->paths << path;
        }
    }

    if (series->pointMarker()) {
        int markerCount = line->markers.size();
        if (markerCount < pointCount) {
            for (int i = markerCount; i < pointCount; ++i) {
                QQuickItem *item = qobject_cast<QQuickItem *>(series->pointMarker()->create());
                item->setParentItem(this);
                line->markers << item;
            }
        }
    } else if (line->markers.size() > 0) {
        for (int i = 0; i < line->markers.size(); i++) {
            line->markers[i]->deleteLater();
        }
        line->markers.clear();
    }

    if (!series->pointMarker()) {
        // Create markers for selected points for a line
        // without a user defined point marker

        int selectionCount = line->selections.size();
        if (selectionCount < series->selectedPoints().size()) {
            for (int i = selectionCount; i < series->selectedPoints().size(); ++i) {
                auto selection = new QSGDefaultInternalRectangleNode();
                m_graph->m_backgroundNode->appendChildNode(selection);
                line->selections << selection;
            }
        }

        auto &&selectedPoints = series->selectedPoints();
        for (int i = 0; i < line->selections.size();) {
            if (i < selectedPoints.size()) {
                int index = selectedPoints[i];
                auto &pointItem = line->selections[i];

                qreal x = line->shapePath->startX();
                qreal y = line->shapePath->startY();

                if (index > 0) {
                    x = line->paths[index - 1]->x();
                    y = line->paths[index - 1]->y();
                }

                qreal markerSize = series->markerSize();
                pointItem->setRect(QRectF(x - markerSize / 2.0, y - markerSize / 2.0,
                                          markerSize, markerSize));
                QColor c = series->color();
                if (series->selectedColor().isValid())
                    c = series->selectedColor();
                c.setAlpha(c.alpha() * series->opacity());
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

                i++;
            } else {
                m_graph->m_backgroundNode->removeChildNode(line->selections[i]);
                delete line->selections[i];
                line->selections.removeAt(i);
            }
        }
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
}

void LinesRenderer::updateLineSeries(QLineSeries *series)
{
    if (series->points().isEmpty())
        return;

    if (!m_linePaths.contains(series))
        return;

    auto line = m_linePaths.value(series);

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
        double maxVertical = m_graph->m_axisRenderer->m_axisVerticalMaxValue > 0 ? 1.0 / m_graph->m_axisRenderer->m_axisVerticalMaxValue : 100.0;
        double maxHorizontal = m_graph->m_axisRenderer->m_axisHorizontalMaxValue > 0 ? 1.0 / m_graph->m_axisRenderer->m_axisHorizontalMaxValue : 100.0;
        for (int i = 0; i < points.count(); ++i) {
            qreal x = m_graph->m_marginLeft + m_graph->m_axisRenderer->m_axisWidth + w * points[i].x() * maxHorizontal;
            qreal y = m_graph->m_marginTop + h - h * points[i].y() * maxVertical;
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
            }
        }
    }
}

void LinesRenderer::handleMouseMove(QMouseEvent *event)
{
    if (m_pointPressed && m_pressedLine->series->isPointSelected(m_pressedPointIndex)) {
        float w = width() - m_graph->m_marginLeft - m_graph->m_marginRight - m_graph->m_axisRenderer->m_axisWidth;
        float h = height() - m_graph->m_marginTop - m_graph->m_marginBottom - m_graph->m_axisRenderer->m_axisHeight;
        double maxVertical = m_graph->m_axisRenderer->m_axisVerticalMaxValue > 0
                                 ? 1.0 / m_graph->m_axisRenderer->m_axisVerticalMaxValue : 100.0;
        double maxHorizontal = m_graph->m_axisRenderer->m_axisHorizontalMaxValue > 0
                                   ? 1.0 / m_graph->m_axisRenderer->m_axisHorizontalMaxValue : 100.0;

        QPoint delta = m_pressStart - event->pos();

        qreal deltaX = -delta.x() / w / maxHorizontal;
        qreal deltaY = delta.y() / h / maxVertical;

        for (auto &&line : m_linePaths) {
            auto &&selectedPoints = line->series->selectedPoints();
            for (int index : selectedPoints) {
                QPointF point = line->series->at(index) + QPointF(deltaX, deltaY);
                line->series->replace(index, point);
            }
        }

        m_pressStart = event->pos();
        m_pointDragging = true;
    }
}

void LinesRenderer::handleMousePress(QMouseEvent *event)
{
    const int selectionSize = 20;
    for (auto &&line : m_linePaths) {
        QRect startRect(line->shapePath->startX() - selectionSize / 2.0,
                        line->shapePath->startY() - selectionSize / 2.0,
                        selectionSize, selectionSize);
        if (startRect.contains(event->pos())) {
            m_pointPressed = true;
            m_pressStart = event->pos();
            m_pressedLine = line;
            m_pressedPointIndex = 0;
        }

        int index = 1;
        for (auto &&path : line->paths) {
            QRect rect(path->x() - selectionSize / 2.0,
                       path->y() - selectionSize / 2.0,
                       selectionSize, selectionSize);
            if (rect.contains(event->pos())) {
                m_pointPressed = true;
                m_pressStart = event->pos();
                m_pressedLine = line;
                m_pressedPointIndex = index;
            }
            index++;
        }
    }
}

void LinesRenderer::handleMouseRelease(QMouseEvent *event)
{
    if (!m_pointDragging && m_pointPressed && m_pressedLine) {
        const int selectionSize = 20;
        QRect rect(m_pressedLine->shapePath->startX() - selectionSize / 2.0,
                   m_pressedLine->shapePath->startY() - selectionSize / 2.0,
                   selectionSize, selectionSize);

        if (m_pressedPointIndex > 0) {
            rect = QRect(m_pressedLine->paths[m_pressedPointIndex - 1]->x() - selectionSize / 2.0,
                         m_pressedLine->paths[m_pressedPointIndex - 1]->y() - selectionSize / 2.0,
                         selectionSize, selectionSize);
        }

        if (rect.contains(event->pos())) {
            if (m_pressedLine->series->isPointSelected(m_pressedPointIndex)) {
                m_pressedLine->series->deselectPoint(m_pressedPointIndex);
            } else {
                m_pressedLine->series->selectPoint(m_pressedPointIndex);
            }
        }
    }
    m_pointPressed = false;
    m_pointDragging = false;
}

void LinesRenderer::handleHoverMove(QHoverEvent *event)
{
    const QPointF &position = event->position();

    const qreal x0 = event->position().x();
    const qreal y0 = event->position().y();

    for (auto &&line : m_linePaths) {
        const int hoverSize = line->series->markerSize() / 2;
        const QString &name = line->series->name();
        auto &&points = line->series->points();

        if (points.size() >= 2) {
            bool hovering = false;

            for (int i = 0; i < points.size() - 1; i++) {
                qreal x1, y1, x2, y2;

                if (i == 0) {
                    x1 = line->shapePath->startX();
                    y1 = line->shapePath->startY();
                    x2 = line->paths[0]->x();
                    y2 = line->paths[0]->y();
                } else {
                    x1 = line->paths[i - 1]->x();
                    y1 = line->paths[i - 1]->y();
                    x2 = line->paths[i]->x();
                    y2 = line->paths[i]->y();
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

                            if (!line->hover) {
                                line->hover = true;
                                emit line->series->hoverEnter(name, position, point);
                            }

                            emit line->series->hover(name, position, point);
                            hovering = true;
                        }
                    }
                }
            }

            if (!hovering && line->hover) {
                line->hover = false;
                emit line->series->hoverExit(name, position);
            }
        }
    }
}
