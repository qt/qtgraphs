// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGraphs/qbarseries.h>
#include <QtGraphs/qbarset.h>
#include <private/barsrenderer_p.h>
#include <private/qbarseries_p.h>
#include <private/qgraphsview_p.h>

QT_BEGIN_NAMESPACE

static const char* TAG_BAR_COLOR = "barColor";
static const char* TAG_BAR_BORDER_COLOR = "barBorderColor";
static const char* TAG_BAR_BORDER_WIDTH = "barBorderWidth";
static const char* TAG_BAR_SELECTED = "barSelected";
static const char* TAG_BAR_VALUE = "barValue";
static const char* TAG_BAR_LABEL = "barLabel";

BarsRenderer::BarsRenderer(QQuickItem *parent)
    : QQuickItem(parent)
{
    m_graph = qobject_cast<QGraphsView *>(parent);
    setFlag(QQuickItem::ItemHasContents);
}

// Returns color in this order:
// 1) QBarSet::color if that is defined (alpha > 0).
// 2) QBarSeries::seriesColors at index if that is defined.
// 3) QGraphsTheme::seriesColors at index.
// 4) Black if seriesColors is empty.
QColor BarsRenderer::getSetColor(QBarSeries *series, QBarSet *set, qsizetype barSeriesIndex)
{
    const auto &seriesColors = !series->seriesColors().isEmpty()
            ? series->seriesColors() : m_graph->theme()->seriesColors();
    if (seriesColors.isEmpty())
        return QColorConstants::Black;
    qsizetype index = m_colorIndex + barSeriesIndex;
    index = index % seriesColors.size();
    QColor color = set->color().alpha() != 0
            ? set->color()
            : seriesColors.at(index);
    return color;
}

QColor BarsRenderer::getSetSelectedColor(QBarSeries *series, QBarSet *set)
{
    Q_UNUSED(series);
    auto theme = m_graph->theme();
    QColor color = set->selectedColor().alpha() != 0
            ? set->selectedColor()
            : theme->singleHighlightColor();
    return color;
}

QColor BarsRenderer::getSetBorderColor(QBarSeries *series, QBarSet *set, qsizetype barSeriesIndex)
{
    const auto &borderColors = !series->borderColors().isEmpty()
            ? series->borderColors() : m_graph->theme()->borderColors();
    if (borderColors.isEmpty())
        return QColorConstants::Black;
    qsizetype index = m_colorIndex + barSeriesIndex;
    index = index % borderColors.size();
    QColor color = set->borderColor().alpha() != 0
            ? set->borderColor()
            : borderColors.at(index);
    return color;
}

qreal BarsRenderer::getSetBorderWidth(QBarSeries *series, QBarSet *set)
{
    Q_UNUSED(series);
    auto theme = m_graph->theme();
    qreal borderWidth = set->borderWidth();
    if (qFuzzyCompare(borderWidth, -1.0))
        borderWidth = theme->borderWidth();
    return borderWidth;
}

QString BarsRenderer::generateLabelText(QBarSeries *series, qreal value)
{
    static const QString valueTag(QLatin1String("@value"));
    QString valueString = QString::number(value, 'f', series->labelsPrecision());
    QString valueLabel;
    if (series->labelsFormat().isEmpty()) {
        valueLabel = valueString;
    } else {
        valueLabel = series->labelsFormat();
        valueLabel.replace(valueTag, valueString);
    }
    return valueLabel;
}

void BarsRenderer::positionLabelItem(QBarSeries *series, QQuickText *textItem, const BarSeriesData &d)
{
    auto pos = series->labelsPosition();
    const bool vertical = m_graph->orientation() == Qt::Orientation::Vertical;
    const float w = textItem->contentWidth() + series->labelsMargin() * 2;
    const float h = textItem->contentHeight() + series->labelsMargin() * 2;
    textItem->setWidth(w);
    textItem->setHeight(h);
    textItem->setHAlign(QQuickText::HAlignment::AlignHCenter);
    textItem->setVAlign(QQuickText::VAlignment::AlignVCenter);
    if (pos == QBarSeries::LabelsPosition::Center) {
        textItem->setX(d.rect.x() + d.rect.width() * 0.5 - w * 0.5);
        textItem->setY(d.rect.y() + d.rect.height() * 0.5 - h * 0.5);
    } else if (pos == QBarSeries::LabelsPosition::InsideEnd) {
        if (vertical) {
            textItem->setX(d.rect.x() + d.rect.width() * 0.5 - w * 0.5);
            textItem->setY(d.rect.y());
        } else {
            textItem->setX(d.rect.x() + d.rect.width() - w);
            textItem->setY(d.rect.y() + d.rect.height() * 0.5 - h * 0.5);
        }
    } else if (pos == QBarSeries::LabelsPosition::InsideBase) {
        if (vertical) {
            textItem->setX(d.rect.x() + d.rect.width() * 0.5 - w * 0.5);
            textItem->setY(d.rect.y() + d.rect.height() - h);
        } else {
            textItem->setX(d.rect.x());
            textItem->setY(d.rect.y() + d.rect.height() * 0.5 - h * 0.5);
        }
    } else {
        // OutsideEnd
        if (vertical) {
            textItem->setX(d.rect.x() + d.rect.width() * 0.5 - w * 0.5);
            textItem->setY(d.rect.y() - h);
        } else {
            textItem->setX(d.rect.x() + d.rect.width());
            textItem->setY(d.rect.y() + d.rect.height() * 0.5 - h * 0.5);
        }
    }
    textItem->update();
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
                // Check for specific dynamic properties
                if (barItem->property(TAG_BAR_COLOR).isValid())
                    barItem->setProperty(TAG_BAR_COLOR, d.color);
                if (barItem->property(TAG_BAR_BORDER_COLOR).isValid())
                    barItem->setProperty(TAG_BAR_BORDER_COLOR, d.borderColor);
                if (barItem->property(TAG_BAR_BORDER_WIDTH).isValid())
                    barItem->setProperty(TAG_BAR_BORDER_WIDTH, d.borderWidth);
                if (barItem->property(TAG_BAR_SELECTED).isValid())
                    barItem->setProperty(TAG_BAR_SELECTED, d.isSelected);
                if (barItem->property(TAG_BAR_VALUE).isValid())
                    barItem->setProperty(TAG_BAR_VALUE, d.value);
                if (barItem->property(TAG_BAR_LABEL).isValid())
                    barItem->setProperty(TAG_BAR_LABEL, d.label);
            }
            barIndex++;
        }
    }
}

void BarsRenderer::updateValueLabels(QBarSeries *series)
{
    if (!series->barComponent() && series->isLabelsVisible()) {
        // Update default value labels
        int barIndex = 0;
        for (auto i = m_seriesData.cbegin(), end = m_seriesData.cend(); i != end; ++i) {
            if (m_labelTextItems.size() <= barIndex) {
                // Create more label items as needed
                auto labelItem = new QQuickText();
                labelItem->setParentItem(this);
                m_labelTextItems << labelItem;
            }
            if (m_labelTextItems.size() > barIndex) {
                // Set label item values
                auto &textItem = m_labelTextItems[barIndex];
                const auto d = *i;
                if (qFuzzyIsNull(d.value)) {
                    textItem->setVisible(false);
                } else {
                    textItem->setVisible(series->isLabelsVisible());
                    QString valueLabel = generateLabelText(series, d.value);
                    textItem->setText(valueLabel);
                    positionLabelItem(series, textItem, d);
                    QColor labelColor = d.labelColor;
                    if (labelColor.alpha() == 0) {
                        // TODO: Use graphs theme labels color.
                        labelColor = QColor(255, 255, 255);
                    }
                    textItem->setColor(labelColor);
                    textItem->setRotation(series->labelsAngle());
                }
            }
            barIndex++;
        }
    } else {
        // Hide all possibly existing label items
        for (auto textItem : m_labelTextItems)
            textItem->setVisible(false);
    }
}

void calculateCategoryTotalValues(QBarSeries *series, QList<float> &totalValues, qsizetype valuesPerSet)
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

void BarsRenderer::updateVerticalBars(QBarSeries *series, qsizetype setCount, qsizetype valuesPerSet)
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
    m_seriesData.clear();

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
    int barSeriesIndex = 0;
    QList<QLegendData> legendDataList;
    for (auto s : series->barSets()) {
        QVariantList v = s->values();
        qsizetype valuesCount = v.size();
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

        QColor color = getSetColor(series, s, barSeriesIndex);
        QColor borderColor = getSetBorderColor(series, s, barSeriesIndex);
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
            d.color = isSelected ? getSetSelectedColor(series, s) : color;
            d.borderColor = borderColor;
            d.borderWidth = borderWidth;
            d.isSelected = isSelected;
            d.label = s->label();
            d.labelColor = s->labelColor();
            d.value = realValue;
            m_seriesData[barIndex] = d;

            if (stacked)
                posYListInSet[barIndexInSet] += barLength;
            barIndex++;
            barIndexInSet++;
            seriesPos = ((float)barIndexInSet / valuesPerSet) * w;
        }
        posXInSet += barWidth + m_barMargin;
        barSeriesIndex++;
    }
    series->d_func()->setLegendData(legendDataList);
}

void BarsRenderer::updateHorizontalBars(QBarSeries *series, qsizetype setCount, qsizetype valuesPerSet)
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
    m_seriesData.clear();

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
        qsizetype valuesCount = v.size();
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
            d.color = isSelected ? getSetSelectedColor(series, s) : color;
            d.borderColor = borderColor;
            d.borderWidth = borderWidth;
            d.isSelected = isSelected;
            d.label = s->label();
            d.labelColor = s->labelColor();
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
    auto theme = m_graph->theme();
    if (!theme)
        return;

    qsizetype setCount = series->barSets().size();
    if (setCount == 0) {
        series->d_func()->clearLegendData();
        m_rectNodesInputRects.clear();
        m_seriesData.clear();
        return;
    }

    if (m_colorIndex < 0)
        m_colorIndex = m_graph->graphSeriesCount();
    m_graph->setGraphSeriesCount(m_colorIndex + setCount);

    if (!series->barComponent() && !m_barItems.isEmpty()) {
        // If we have switched from custom bar component to rectangle nodes,
        // remove the redundant items.
        for (int i = 0; i < m_barItems.size(); i++)
            m_barItems[i]->deleteLater();
        m_barItems.clear();
    }

    // Get bars values
    qsizetype valuesPerSet = series->barSets().first()->values().size();
    if (m_graph->orientation() == Qt::Orientation::Vertical)
        updateVerticalBars(series, setCount, valuesPerSet);
    else
        updateHorizontalBars(series, setCount, valuesPerSet);
    updateComponents(series);
    updateValueLabels(series);
}

void BarsRenderer::updateSeries(QBarSeries *series)
{
    if (series->barSets().isEmpty()) {
        auto it = m_rectNodes.begin();
        while (it != m_rectNodes.end())
            delete *it++;
        m_rectNodes.clear();
        return;
    }

    auto theme = m_graph->theme();
    if (!theme)
        return;

    qsizetype difference = m_seriesData.size() - m_rectNodes.size();
    for (qsizetype i = m_rectNodes.size() - 1; i >= m_seriesData.size(); --i)
        delete m_rectNodes[i];
    if (difference != 0)
        m_rectNodes.resize(m_seriesData.size());

    if (!series->barComponent()) {
        // Update default rectangle nodes
        int barIndex = 0;
        for (auto i = m_seriesData.cbegin(), end = m_seriesData.cend(); i != end; ++i) {
            if (m_rectNodes[barIndex] == nullptr) {
                // Create more rectangle nodes as needed
                auto bi = new QSGDefaultInternalRectangleNode();
                m_graph->m_backgroundNode->appendChildNode(bi);
                m_rectNodes[barIndex] = bi;
            }
            if (m_rectNodes.size() > barIndex) {
                auto &barItem = m_rectNodes[barIndex];
                BarSeriesData d = *i;
                barItem->setRect(d.rect);
                barItem->setColor(d.color);
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
        qsizetype indexInSet = 0;
        for (auto &rect : barSelection.rects) {
            if (rect.contains(event->pos())) {
                // TODO: Currently just toggling selection
                QList<qsizetype> indexList = {indexInSet};
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
