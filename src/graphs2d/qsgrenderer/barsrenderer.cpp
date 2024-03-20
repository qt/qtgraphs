// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGraphs/qbarseries.h>
#include <QtGraphs/qbarset.h>
#include <private/barsrenderer_p.h>
#include <private/qbarseries_p.h>
#include <private/qgraphsview_p.h>
#include <private/barcomponent_p.h>

QT_BEGIN_NAMESPACE

BarsRenderer::BarsRenderer(QQuickItem *parent)
    : QQuickItem(parent)
{
    m_graph = qobject_cast<QGraphsView *>(parent);
    setFlag(QQuickItem::ItemHasContents);
}

void BarsRenderer::handlePolish(QBarSeries *series)
{
    auto seriesTheme = series->theme();
    if (!seriesTheme)
        return;

    if (series->barSets().isEmpty())
        return;

    int setCount = series->barSets().size();

    if (m_colorIndex < 0)
        m_colorIndex = seriesTheme->graphSeriesCount();
    seriesTheme->setGraphSeriesCount(m_colorIndex + setCount);

    if (!series->barComponent() && !m_barItems.isEmpty()) {
        // If we have switched from custom bar component to rectangle nodes,
        // remove the redundant items.
        for (int i = 0; i < m_barItems.size(); i++) {
            m_barItems[i]->deleteLater();
        }
        m_barItems.clear();
    }

    // Get bars values
    int valuesPerSet = series->barSets().first()->values().size();

    // Bars area width & height
    QRectF seriesRect = m_graph->seriesRect();
    float w = seriesRect.width();
    float h = seriesRect.height();
    // Margin between bars.
    float barMargin = 2.0;
    // Max width of a bar if no separation between sets.
    float maxBarWidth = w / (setCount * valuesPerSet) - barMargin;
    // Actual bar width.
    float barWidth = maxBarWidth * series->barWidth();
    // Helper to keep barsets centered when bar width is less than max width.
    float barCentering = (maxBarWidth - barWidth) * setCount * 0.5;

    // Clear the selection rects
    // These will be filled only if series is selectable
    m_rectNodesInputRects.clear();

    float seriesPos = 0;
    float posInSet = 0;
    int barIndex = 0;
    int barIndexInSet = 0;
    int barSerieIndex = 0;
    QList<QLegendData> legendDataList;
    for (auto s : series->barSets()) {
        QVariantList v = s->values();
        int valuesCount = v.size();
        if (valuesCount == 0)
            continue;
        seriesPos = 0;
        barIndexInSet = 0;
        BarSelectionRect *barSelectionRect = nullptr;
        if (series->selectable() || series->hoverable()) {
            m_rectNodesInputRects << BarSelectionRect();
            barSelectionRect = &m_rectNodesInputRects.last();
            barSelectionRect->barSet = s;
            barSelectionRect->series = series;
        }
        // Use set colors if available
        QColor color = s->color().alpha() != 0
                ? s->color()
                : seriesTheme->graphSeriesColor(m_colorIndex + barSerieIndex);
        QColor borderColor = s->borderColor().alpha() != 0
                ? s->borderColor()
                : seriesTheme->graphSeriesBorderColor(m_colorIndex + barSerieIndex);
        qreal borderWidth = s->borderWidth();
        if (qFuzzyCompare(borderWidth, -1.0))
            borderWidth = seriesTheme->borderWidth();
        // Update legendData
        legendDataList.push_back({
                color,
                borderColor,
                s->label()
        });
        // Apply series opacity
        color.setAlpha(color.alpha() * series->opacity());
        borderColor.setAlpha(borderColor.alpha() * series->opacity());
        const auto selectedBars = s->selectedBars();
        for (auto variantValue : std::as_const(v)) {
            const float realValue = variantValue.toReal();
            float value = (realValue - m_graph->m_axisRenderer->m_axisVerticalMinValue) * series->valuesMultiplier();
            const bool isSelected = selectedBars.contains(barIndexInSet);
            double delta = m_graph->m_axisRenderer->m_axisVerticalMaxValue - m_graph->m_axisRenderer->m_axisVerticalMinValue;
            double maxValues = delta > 0 ? 1.0 / delta : 100.0;
            float barHeight = h * value * maxValues;
            float barY = m_graph->m_marginTop + h - barHeight;
            float barX = m_graph->m_marginLeft + m_graph->m_axisRenderer->m_axisWidth + seriesPos + posInSet + barCentering;
            QRectF barRect(barX, barY, barWidth, barHeight);
            if (barSelectionRect)
                barSelectionRect->rects << barRect;
            if (series->barComponent()) {
                // Set custom bar components
                if (m_barItems.size() <= barIndex) {
                    // Create more custom components as needed
                    QQuickItem *item = qobject_cast<QQuickItem *>(
                            series->barComponent()->create(series->barComponent()->creationContext()));
                    if (item) {
                        item->setParent(this);
                        item->setParentItem(this);
                        m_barItems << item;
                    }
                }
                if (m_barItems.size() > barIndex) {
                    auto &barItem = m_barItems[barIndex];
                    barItem->setX(barRect.x());
                    barItem->setY(barRect.y());
                    barItem->setWidth(barRect.width());
                    barItem->setHeight(barRect.height());
                    auto barComponent = qobject_cast<BarComponent *>(barItem);
                    if (barComponent) {
                        barComponent->setBarColor(color);
                        barComponent->setBarBorderColor(borderColor);
                        barComponent->setBarBorderWidth(borderWidth);
                        barComponent->setBarSelected(isSelected);
                        barComponent->setBarValue(realValue);
                        barComponent->setBarLabel(s->label());
                    }
                    barItem->update();
                }
            } else {
                // Not using custom components.
                // Collect data for default rectangle nodes.
                BarSeriesData d;
                d.rect = barRect;
                d.color = color;
                d.borderColor = borderColor;
                d.borderWidth = borderWidth;
                d.isSelected = isSelected;
                m_seriesData[barIndex] = d;
            }

            barIndex++;
            barIndexInSet++;
            seriesPos = ((float)barIndexInSet / valuesPerSet) * w;
        }
        posInSet += barWidth + barMargin;
        barSerieIndex++;
    }
    series->d_func()->setLegendData(legendDataList);
}

void BarsRenderer::updateSeries(QBarSeries *series)
{
    if (series->barSets().isEmpty())
        return;

    auto seriesTheme = series->theme();
    if (!seriesTheme)
        return;

    if (!series->barComponent()) {
        // Update default rectangle nodes
        int barIndex = 0;
        for (auto i = m_seriesData.cbegin(), end = m_seriesData.cend(); i != end; ++i) {
            if (m_rectNodes.size() <= barIndex) {
                // Create more rectangle nodes as needed
                auto bi = new QSGDefaultInternalRectangleNode();
                m_graph->m_backgroundNode->appendChildNode(bi);
                m_rectNodes << bi;
            }
            if (m_rectNodes.size() > barIndex) {
                auto &barItem = m_rectNodes[barIndex];
                BarSeriesData d = *i;
                barItem->setRect(d.rect);
                // TODO: Theming for selection?
                QColor barColor = d.isSelected ? QColor(0,0,0) : d.color;
                barItem->setColor(barColor);
                barItem->setPenWidth(d.borderWidth);
                barItem->setPenColor(d.borderColor);
                barItem->setRadius(4.0);
                barItem->setAntialiasing(true);
                barItem->update();
                barIndex++;
            }
        }
    }
}

bool BarsRenderer::handleMousePress(QMouseEvent *event)
{
    bool handled = false;
    for (auto &barSelection : m_rectNodesInputRects) {
        if (!barSelection.series->selectable())
            continue;
        int indexInSet = 0;
        for (auto &rect : barSelection.rects) {
            if (rect.contains(event->pos())) {
                // TODO: Currently just toggling selection
                QList<int> indexList = {indexInSet};
                barSelection.barSet->toggleSelection(indexList);
                handled = true;
            }
            indexInSet++;
        }
    }
    return handled;
}

bool BarsRenderer::handleHoverMove(QHoverEvent *event)
{
    bool handled = false;
    const QPointF &position = event->position();

    bool hovering = false;
    for (auto &barSelection : m_rectNodesInputRects) {
        int indexInSet = 0;

        for (auto &rect : barSelection.rects) {
            if (rect.contains(event->position().toPoint())) {
                const QString &name = barSelection.series->name();
                const QPointF point(indexInSet, barSelection.barSet->at(indexInSet));

                if (!m_currentHoverSeries) {
                    m_currentHoverSeries = barSelection.series;
                    emit barSelection.series->hoverEnter(name, position, point);
                }

                emit barSelection.series->hover(name, position, point);
                hovering = true;
                handled = true;
            }
            indexInSet++;
        }
    }

    if (!hovering && m_currentHoverSeries) {
        emit m_currentHoverSeries->hoverExit(m_currentHoverSeries->name(), position);
        m_currentHoverSeries = nullptr;
        handled = true;
    }
    return handled;
}

QT_END_NAMESPACE
