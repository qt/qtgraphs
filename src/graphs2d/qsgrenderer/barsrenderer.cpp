// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGraphs/qbarseries.h>
#include <QtGraphs/qbarset.h>
#include <private/barsrenderer_p.h>
#include <private/qbarseries_p.h>
#include <private/qgraphsview_p.h>

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

    int setCount = series->barSets().size();

    if (m_colorIndex < 0)
        m_colorIndex = seriesTheme->graphSeriesCount();
    seriesTheme->setGraphSeriesCount(m_colorIndex + setCount);
}

void BarsRenderer::updateSeries(QBarSeries *series)
{
    if (series->barSets().isEmpty())
        return;

    // Count bar amount
    int barItems = 0;
    for (auto s : series->barSets())
        barItems += s->values().size();

    auto seriesTheme = series->theme();
    if (!seriesTheme)
        return;

    // Get bars values
    // TODO: Make these themeable
    int setCount = series->barSets().size();
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

    // See if we need more rectangle nodes
    int currentSize = m_rectNodes.size();
    if (currentSize < barItems) {
        for (int i = currentSize; i <= barItems; i++) {
            auto bi = new QSGDefaultInternalRectangleNode();
            m_graph->m_backgroundNode->appendChildNode(bi);
            m_rectNodes << bi;
        }
    }

    // Clear the selection rects
    // These will be filled only if series is selectable
    m_rectNodesInputRects.clear();

    float barX = 0;
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
            float value = (variantValue.toReal() - m_graph->m_axisRenderer->m_axisVerticalMinValue) * series->valuesMultiplier();
            if (value > 0) {
                const bool isSelected = selectedBars.contains(barIndexInSet);
                double delta = m_graph->m_axisRenderer->m_axisVerticalMaxValue - m_graph->m_axisRenderer->m_axisVerticalMinValue;
                double maxValues = delta > 0 ? 1.0 / delta : 100.0;
                float barHeight = h * value * maxValues;
                float barY = m_graph->m_marginTop + h - barHeight;
                barX = m_graph->m_marginLeft + m_graph->m_axisRenderer->m_axisWidth + seriesPos + posInSet + barCentering;
                QRectF barRect(barX, barY, barWidth, barHeight);
                if (barSelectionRect)
                    barSelectionRect->rects << barRect;
                auto &barItem = m_rectNodes[barIndex];
                barItem->setRect(barRect);
                // TODO: Theming for selection?
                if (isSelected)
                    color = QColor(0,0,0);
                barItem->setColor(color);
                qreal borderWidth = s->borderWidth();
                if (qFuzzyCompare(borderWidth, -1.0))
                    borderWidth = seriesTheme->borderWidth();
                barItem->setPenWidth(borderWidth);
                barItem->setPenColor(borderColor);
                // TODO: Required because of QTBUG-117892
                barItem->setTopLeftRadius(-1);
                barItem->setTopRightRadius(-1);
                barItem->setBottomLeftRadius(-1);
                barItem->setBottomRightRadius(-1);
                barItem->setRadius(4.0);
                barItem->setAntialiasing(true);
                barItem->update();

            } else {
                auto &barItem = m_rectNodes[barIndex];
                barItem->setRect(QRectF());
                if (barSelectionRect)
                    barSelectionRect->rects << QRectF();
                barItem->update();
            }
            barIndex++;
            barIndexInSet++;
            seriesPos = ((float)barIndexInSet / valuesPerSet) * w;
        }
        posInSet += barWidth + barMargin;
        barSerieIndex++;
    }
    series->d_ptr->setLegendData(legendDataList);
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
