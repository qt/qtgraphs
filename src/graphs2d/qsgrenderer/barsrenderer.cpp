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

QColor BarsRenderer::getSetColor(QBarSeries *series, QBarSet *set, int barSerieIndex)
{
    auto seriesTheme = series->theme();
    QColor color = set->color().alpha() != 0
            ? set->color()
            : seriesTheme->graphSeriesColor(m_colorIndex + barSerieIndex);
    return color;
}

QColor BarsRenderer::getSetBorderColor(QBarSeries *series, QBarSet *set, int barSerieIndex)
{
    auto seriesTheme = series->theme();
    QColor color = set->borderColor().alpha() != 0
            ? set->borderColor()
            : seriesTheme->graphSeriesBorderColor(m_colorIndex + barSerieIndex);
    return color;
}

qreal BarsRenderer::getSetBorderWidth(QBarSeries *series, QBarSet *set)
{
    auto seriesTheme = series->theme();
    qreal borderWidth = set->borderWidth();
    if (qFuzzyCompare(borderWidth, -1.0))
        borderWidth = seriesTheme->borderWidth();
    return borderWidth;
}

void BarsRenderer::updateComponents(QBarSeries *series)
{
    if (series->barComponent()) {
        // Update default rectangle nodes
        int barIndex = 0;
        for (auto i = m_seriesData.cbegin(), end = m_seriesData.cend(); i != end; ++i) {
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
                // Set custom bar components
                BarSeriesData d = *i;
                auto &barItem = m_barItems[barIndex];
                barItem->setX(d.rect.x());
                barItem->setY(d.rect.y());
                barItem->setWidth(d.rect.width());
                barItem->setHeight(d.rect.height());
                auto barComponent = qobject_cast<BarComponent *>(barItem);
                if (barComponent) {
                    barComponent->setBarColor(d.color);
                    barComponent->setBarBorderColor(d.borderColor);
                    barComponent->setBarBorderWidth(d.borderWidth);
                    barComponent->setBarSelected(d.isSelected);
                    barComponent->setBarValue(d.value);
                    barComponent->setBarLabel(d.label);
                }
                barItem->update();
            }
            barIndex++;
        }
    }
}

void calculateCategoryTotalValues(QBarSeries *series, QList<float> &totalValues, int valuesPerSet)
{
    totalValues.fill(0, valuesPerSet);
    for (auto s : series->barSets()) {
        QVariantList v = s->values();
        int setIndex = 0;
        for (auto variantValue : std::as_const(v)) {
            if (setIndex < totalValues.size())
                totalValues[setIndex] += variantValue.toReal();
            setIndex++;
        }
    }
}

void BarsRenderer::updateVerticalBars(QBarSeries *series, int setCount, int valuesPerSet)
{
    bool stacked = series->barsType() == QBarSeries::BarsType::Stacked
            || series->barsType() == QBarSeries::BarsType::StackedPercent;
    bool percent = series->barsType() == QBarSeries::BarsType::StackedPercent;
    // Bars area width & height
    QRectF seriesRect = m_graph->seriesRect();
    float w = seriesRect.width();
    float h = seriesRect.height();
    // Max width of a bar if no separation between sets.
    float maxBarWidth = w / (setCount * valuesPerSet) - m_barMargin;
    if (stacked)
        maxBarWidth = w / valuesPerSet;
    // Actual bar width.
    float barWidth = maxBarWidth * series->barWidth();
    // Helper to keep barsets centered when bar width is less than max width.
    float barCentering = (maxBarWidth - barWidth) * setCount * 0.5;
    if (stacked)
        barCentering = (maxBarWidth - barWidth) * 0.5;

    // Clear the selection rects
    // These will be filled only if series is selectable
    m_rectNodesInputRects.clear();

    float seriesPos = 0;
    float posXInSet = 0;
    QList<float> posYListInSet;
    if (stacked)
        posYListInSet.fill(0, valuesPerSet);
    QList<float> totalValuesListInSet;
    if (percent)
        calculateCategoryTotalValues(series, totalValuesListInSet, valuesPerSet);

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

        QColor color = getSetColor(series, s, barSerieIndex);
        QColor borderColor = getSetBorderColor(series, s, barSerieIndex);
        qreal borderWidth = getSetBorderWidth(series, s);
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
            if (percent) {
                if (auto totalValue = totalValuesListInSet.at(barIndexInSet))
                    value *= (100.0 / totalValue);
            }
            const bool isSelected = selectedBars.contains(barIndexInSet);
            double delta = m_graph->m_axisRenderer->m_axisVerticalMaxValue - m_graph->m_axisRenderer->m_axisVerticalMinValue;
            double maxValues = delta > 0 ? 1.0 / delta : 100.0;
            float barLength = h * value * maxValues;
            float barY = m_graph->m_marginTop + h - barLength;
            float barX = m_graph->m_marginLeft + m_graph->m_axisRenderer->m_axisWidth + seriesPos + posXInSet + barCentering;
            if (stacked) {
                barY = m_graph->m_marginTop + h - barLength - posYListInSet[barIndexInSet];
                barX = m_graph->m_marginLeft + m_graph->m_axisRenderer->m_axisWidth + seriesPos + barCentering;
            }
            QRectF barRect(barX, barY, barWidth, barLength);
            if (barSelectionRect)
                barSelectionRect->rects << barRect;

            // Collect the series data
            BarSeriesData d;
            d.rect = barRect;
            d.color = color;
            d.borderColor = borderColor;
            d.borderWidth = borderWidth;
            d.isSelected = isSelected;
            d.label = s->label();
            d.value = realValue;
            m_seriesData[barIndex] = d;

            if (stacked)
                posYListInSet[barIndexInSet] += barLength;
            barIndex++;
            barIndexInSet++;
            seriesPos = ((float)barIndexInSet / valuesPerSet) * w;
        }
        posXInSet += barWidth + m_barMargin;
        barSerieIndex++;
    }
    series->d_func()->setLegendData(legendDataList);
}

void BarsRenderer::updateHorizontalBars(QBarSeries *series, int setCount, int valuesPerSet)
{
    bool stacked = series->barsType() == QBarSeries::BarsType::Stacked
            || series->barsType() == QBarSeries::BarsType::StackedPercent;
    bool percent = series->barsType() == QBarSeries::BarsType::StackedPercent;
    // Bars area width & height
    QRectF seriesRect = m_graph->seriesRect();
    float w = seriesRect.width();
    float h = seriesRect.height();
    // Max width of a bar if no separation between sets.
    float maxBarWidth = h / (setCount * valuesPerSet) - m_barMargin;
    if (stacked)
        maxBarWidth = h / valuesPerSet;
    // Actual bar width.
    float barWidth = maxBarWidth * series->barWidth();
    // Helper to keep barsets centered when bar width is less than max width.
    float barCentering = (maxBarWidth - barWidth) * setCount * 0.5;
    if (stacked)
        barCentering = (maxBarWidth - barWidth) * 0.5;

    // Clear the selection rects
    // These will be filled only if series is selectable
    m_rectNodesInputRects.clear();

    float seriesPos = 0;
    float posYInSet = 0;
    QList<float> posXListInSet;
    if (stacked)
        posXListInSet.fill(0, valuesPerSet);
    QList<float> totalValuesListInSet;
    if (percent)
        calculateCategoryTotalValues(series, totalValuesListInSet, valuesPerSet);
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

        QColor color = getSetColor(series, s, barSerieIndex);
        QColor borderColor = getSetBorderColor(series, s, barSerieIndex);
        qreal borderWidth = getSetBorderWidth(series, s);
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
            float value = (realValue - m_graph->m_axisRenderer->m_axisHorizontalMinValue) * series->valuesMultiplier();
            if (percent) {
                if (auto totalValue = totalValuesListInSet.at(barIndexInSet))
                    value *= (100.0 / totalValue);
            }
            const bool isSelected = selectedBars.contains(barIndexInSet);
            double delta = m_graph->m_axisRenderer->m_axisHorizontalMaxValue - m_graph->m_axisRenderer->m_axisHorizontalMinValue;
            double maxValues = delta > 0 ? 1.0 / delta : 100.0;
            float barLength = w * value * maxValues;
            float barY = m_graph->m_marginTop + seriesPos + posYInSet + barCentering;
            float barX = m_graph->m_marginLeft + m_graph->m_axisRenderer->m_axisWidth;
            if (stacked) {
                barY = m_graph->m_marginTop + seriesPos + barCentering;
                barX = m_graph->m_marginLeft + m_graph->m_axisRenderer->m_axisWidth + posXListInSet[barIndexInSet];
            }
            QRectF barRect(barX, barY, barLength, barWidth);
            if (barSelectionRect)
                barSelectionRect->rects << barRect;

            // Collect the series data
            BarSeriesData d;
            d.rect = barRect;
            d.color = color;
            d.borderColor = borderColor;
            d.borderWidth = borderWidth;
            d.isSelected = isSelected;
            d.label = s->label();
            d.value = realValue;
            m_seriesData[barIndex] = d;

            if (stacked)
                posXListInSet[barIndexInSet] += barLength;
            barIndex++;
            barIndexInSet++;
            seriesPos = ((float)barIndexInSet / valuesPerSet) * h;
        }
        posYInSet += barWidth + m_barMargin;
        barSerieIndex++;
    }
    series->d_func()->setLegendData(legendDataList);
}

void BarsRenderer::handlePolish(QBarSeries *series)
{
    auto seriesTheme = series->theme();
    if (!seriesTheme)
        return;

    int setCount = series->barSets().size();
    if (setCount == 0)
        return;

    if (m_colorIndex < 0)
        m_colorIndex = seriesTheme->graphSeriesCount();
    seriesTheme->setGraphSeriesCount(m_colorIndex + setCount);

    if (!series->barComponent() && !m_barItems.isEmpty()) {
        // If we have switched from custom bar component to rectangle nodes,
        // remove the redundant items.
        for (int i = 0; i < m_barItems.size(); i++)
            m_barItems[i]->deleteLater();
        m_barItems.clear();
    }

    // Get bars values
    int valuesPerSet = series->barSets().first()->values().size();
    if (series->barsOrientation() == QBarSeries::BarsOrientation::Vertical)
        updateVerticalBars(series, setCount, valuesPerSet);
    else
        updateHorizontalBars(series, setCount, valuesPerSet);
    updateComponents(series);
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
