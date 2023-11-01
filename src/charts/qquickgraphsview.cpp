// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <private/qquickgraphsview_p.h>
#include <QtGraphs2D/qbarseries.h>
#include <QtGraphs2D/qbarset.h>
#include <QtGraphs2D/QBarCategoryAxis>
#include <QtGraphs2D/qlineseries.h>
#include <private/qvalueaxis_p.h>

QT_BEGIN_NAMESPACE

QQuickGraphs2DView::QQuickGraphs2DView(QQuickItem *parent) :
    QQuickItem(parent)
{
    setFlag(QQuickItem::ItemHasContents);
    m_shape.setParentItem(this);
}

QQuickGraphs2DView::~QQuickGraphs2DView()
{
    for (auto&& line : std::as_const(m_linePaths))
        delete line;
}

void QQuickGraphs2DView::setBackgroundColor(QColor color)
{
    auto &b = m_backgroundBrush;
    if (b.style() != Qt::SolidPattern || color != b.color()) {
        b.setStyle(Qt::SolidPattern);
        b.setColor(color);
        emit backgroundColorChanged();
    }
}

QColor QQuickGraphs2DView::backgroundColor()
{
    return m_backgroundBrush.color();
}

void QQuickGraphs2DView::addSeries(QObject *series)
{
    insertSeries(m_seriesList.size(), series);
}

void QQuickGraphs2DView::insertSeries(int index, QObject *object)
{
    if (auto series = qobject_cast<QAbstractSeries *>(object)) {
        series->setChart(this);
        if (m_seriesList.contains(series)) {
            int oldIndex = m_seriesList.indexOf(series);
            if (index != oldIndex) {
                m_seriesList.removeOne(series);
                if (oldIndex < index)
                    index--;
                m_seriesList.insert(index, series);
            }
        } else {
            int oldSize = m_seriesList.size();
            m_seriesList.insert(index, series);
            QObject::connect(series, &QAbstractSeries::update, this, &QQuickItem::update);
            if (series->theme())
                QObject::connect(series->theme(), &SeriesTheme::update, this, &QQuickItem::update);
            //QObject::connect(series, &QAbstractSeries::visibilityChanged,
            //                 this, &Abstract3DController::handleSeriesVisibilityChanged);
            //series->resetToTheme(*m_themeManager->activeTheme(), oldSize, false);
        }
//        if (series->isVisible())
//            handleSeriesVisibilityChangedBySender(series);

    }
}

void QQuickGraphs2DView::removeSeries(QObject *object)
{
    if (auto series = reinterpret_cast<QAbstractSeries *>(object)) {
        m_seriesList.removeAll(series);
        for (auto a : series->attachedAxes())
            m_axis.removeAll(a);

        //QObject::disconnect(series, &QAbstractSeries::visibilityChanged,
        //                    this, &Abstract3DController::handleSeriesVisibilityChanged);
        //series->d_func()->setController(0);
        //m_isDataDirty = true;
        //m_isSeriesVisualsDirty = true;
        //emitNeedRender();
    }
}

bool QQuickGraphs2DView::hasSeries(QObject *series)
{
    return m_seriesList.contains(series);
}


void QQuickGraphs2DView::addAxis(QAbstractAxis *axis)
{
    if (!m_axis.contains(axis)) {
        m_axis << axis;
        update();
    }
}
void QQuickGraphs2DView::removeAxis(QAbstractAxis *axis)
{
    if (m_axis.contains(axis)) {
        m_axis.removeAll(axis);
        update();
    }
}

void QQuickGraphs2DView::componentComplete()
{
    if (!m_theme) {
        qDebug() << "Using default theme!";
        m_theme = new GraphTheme(this);
        QObject::connect(m_theme, &GraphTheme::update, this, &QQuickItem::update);
        m_theme->resetColorTheme();
    }
    QQuickItem::componentComplete();
    ensurePolished();
}

QSGNode *QQuickGraphs2DView::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *updatePaintNodeData)
{
    if (!m_backgroundNode)
        m_backgroundNode = new QSGNode();

    // Background node
    //m_backgroundNode->setRect(boundingRect());
    //m_backgroundNode->setColor(m_backgroundBrush.color());
    //m_backgroundNode->update();
    oldNode = m_backgroundNode;

    updateAxis();

    for (auto series : std::as_const(m_seriesList)) {
        if (auto barSeries = qobject_cast<QBarSeries*>(series))
            updateBarSeries(barSeries);

        if (auto lineSeries = qobject_cast<QLineSeries*>(series))
            updateLineSeries(lineSeries);
    }

    // Now possibly dirty theme has been taken into use
    m_theme->resetThemeDirty();

    polish();

    return oldNode;
}

void QQuickGraphs2DView::updatePolish()
{
    for (auto series : std::as_const(m_seriesList)) {
        if (auto lineSeries = qobject_cast<QLineSeries*>(series)) {
            if (!m_linePaths.contains(lineSeries)) {
                LinePath *linePath = new LinePath();
                linePath->shapePath = new QQuickShapePath(&m_shape);
                m_linePaths.insert(lineSeries, linePath);

                auto data = m_shape.data();
                data.append(&data, m_linePaths.value(lineSeries)->shapePath);
            }

            auto line = m_linePaths.value(lineSeries);

            int pointCount = lineSeries->points().size();
            int currentSize = line->paths.size();
            if (currentSize < pointCount - 1) {
                auto pathElements = line->shapePath->pathElements();
                for (int i = currentSize; i < pointCount - 1; ++i) {
                    auto path = new QQuickPathLine(line->shapePath);
                    pathElements.append(&pathElements, path);
                    line->paths << path;
                }
            }
        }
    }
}

void QQuickGraphs2DView::updateAxis()
{
    // Update active tickers
    QAbstractAxis *axisVertical = nullptr;
    QAbstractAxis *axisHorizontal = nullptr;
    for (auto a : m_axis) {
        if (a->orientation() == Qt::Vertical)
            axisVertical = a;
        else
            axisHorizontal = a;
    }
    m_axisVertical = axisVertical;
    m_axisHorizontal = axisHorizontal;

    if (auto vaxis = qobject_cast<QValueAxis *>(m_axisVertical)) {
        if (vaxis->autoScale()) {
            // TODO: Count max from single seried or all or what?
            m_axisVerticalMaxValue = 20;
        } else {
            m_axisVerticalMaxValue = vaxis->max();
        }
    }
    updateAxisTickers();
    updateAxisGrid();
}

void QQuickGraphs2DView::updateAxisTickers()
{
    // TODO: Get data into AxisTicker from QAbstractAxis
    float axisWidth = 40;
    float axisHeight = 20;
    float axisTickersWidth = 15;

    if (m_axisVertical) {
        if (!m_axisTickerVertical) {
            m_axisTickerVertical = new AxisTicker();
            m_axisTickerVertical->setParentItem(this);
            m_axisTickerVertical->setZ(-1);
            m_axisTickerVertical->setOrigo(0);
            m_axisTickerVertical->setMinorBarsVisible(true);
            m_axisTickerVertical->setBarsMovement(0.0);
            m_axisTickerVertical->setMinorTickScale(0.2);
            // TODO: Configurable in theme or axis?
            m_axisTickerVertical->setMinorBarsLength(0.5);
            m_axisTickerVertical->setupShaders();
        }
        if (m_theme->themeDirty()) {
            m_axisTickerVertical->setMinorColor(m_theme->axisYMinorColor());
            m_axisTickerVertical->setMajorColor(m_theme->axisYMajorColor());
            m_axisTickerVertical->setMinorBarWidth(m_theme->axisYMajorBarWidth());
            m_axisTickerVertical->setMajorBarWidth(m_theme->axisYMinorBarWidth());
            m_axisTickerVertical->setSmoothing(m_theme->axisYSmoothing());
        }
        // TODO Only when changed
        m_axisTickerVertical->setX(axisWidth + m_marginLeft - axisTickersWidth);
        m_axisTickerVertical->setY(m_marginTop);
        m_axisTickerVertical->setWidth(axisTickersWidth);
        m_axisTickerVertical->setHeight(height() - m_marginTop - m_marginBottom - axisHeight);
        m_axisTickerVertical->setSpacing(m_axisTickerVertical->height() / m_axisVerticalMaxValue);
    }

    if (m_axisHorizontal) {
        if (!m_axisTickerHorizontal) {
            m_axisTickerHorizontal = new AxisTicker();
            m_axisTickerHorizontal->setParentItem(this);
            m_axisTickerHorizontal->setZ(-1);
            m_axisTickerHorizontal->setIsHorizontal(true);
            m_axisTickerHorizontal->setOrigo(0);
            m_axisTickerHorizontal->setMinorBarsVisible(true);
            m_axisTickerHorizontal->setBarsMovement(0.0);
            m_axisTickerHorizontal->setMinorTickScale(0.5);
            // TODO: Configurable in theme or axis?
            m_axisTickerHorizontal->setMinorBarsLength(0.2);
            m_axisTickerHorizontal->setupShaders();
        }
        if (m_theme->themeDirty()) {
            m_axisTickerHorizontal->setMinorColor(m_theme->axisXMinorColor());
            m_axisTickerHorizontal->setMajorColor(m_theme->axisXMajorColor());
            m_axisTickerHorizontal->setMinorBarWidth(m_theme->axisXMajorBarWidth());
            m_axisTickerHorizontal->setMajorBarWidth(m_theme->axisXMinorBarWidth());
            m_axisTickerHorizontal->setSmoothing(m_theme->axisXSmoothing());
        }
        // TODO Only when changed
        float axisTickersHeight = 15;
        m_axisTickerHorizontal->setX(axisWidth + m_marginLeft);
        m_axisTickerHorizontal->setY(height() - m_marginBottom - axisHeight);
        m_axisTickerHorizontal->setWidth(width() - m_marginLeft - m_marginRight - axisWidth);
        m_axisTickerHorizontal->setHeight(axisTickersHeight);
        m_axisTickerHorizontal->setSpacing(m_axisTickerHorizontal->width() / 6);
    }
}

void QQuickGraphs2DView::updateAxisGrid()
{
    if (!m_axisGrid) {
        m_axisGrid = new AxisGrid();
        m_axisGrid->setParentItem(this);
        m_axisGrid->setZ(-1);
        m_axisGrid->setupShaders();
        m_axisGrid->setOrigo(0);
        m_axisGrid->setBarsVisibility(QVector4D(1, 1, 0, 1));
        m_axisGrid->setGridMovement(QPointF(0, 0));
        m_axisGrid->setVerticalMinorTickScale(0.1);
        m_axisGrid->setHorizontalMinorTickScale(0.1);
    }
    if (m_theme->themeDirty()) {
        m_axisGrid->setMajorColor(m_theme->gridMajorBarsColor());
        m_axisGrid->setMinorColor(m_theme->gridMinorBarsColor());
        m_axisGrid->setMinorBarWidth(m_theme->gridMinorBarsWidth());
        m_axisGrid->setMajorBarWidth(m_theme->gridMajorBarsWidth());
        const double minimumSmoothing = 0.05;
        m_axisGrid->setSmoothing(m_theme->gridSmoothing() + minimumSmoothing);
    }
    // TODO Only when changed
    float axisWidth = 40;
    float axisHeight = 20;
    m_axisGrid->setX(axisWidth + m_marginLeft);
    m_axisGrid->setY(m_marginTop);
    m_axisGrid->setWidth(width() - m_marginLeft - m_marginRight - axisWidth);
    m_axisGrid->setHeight(height() - m_marginTop - m_marginBottom - axisHeight);
    m_axisGrid->setGridWidth(m_axisGrid->width() / 6);
    m_axisGrid->setGridHeight(m_axisGrid->height() / 2);
}

void QQuickGraphs2DView::updateBarXAxis(QBarCategoryAxis *axis, const QRectF &rect)
{
    int categoriesCount =  axis->categories().size();
    // See if we need more text items
    int currentTextItemsSize = m_xAxisTextItems.size();
    if (currentTextItemsSize < categoriesCount) {
        for (int i = currentTextItemsSize; i <= categoriesCount; i++) {
            //auto bi = new QSGTextNode();
            auto bi = new QQuickText();
            bi->setParentItem(this);
            m_xAxisTextItems << bi;
        }
    }
    int textIndex = 0;
    for (auto category : axis->categories()) {
        auto &textItem = m_xAxisTextItems[textIndex];
        float posX = rect.x() + ((float)textIndex / categoriesCount) *  rect.width();
        textItem->setX(posX);
        float posY = rect.y();
        textItem->setY(posY);
        textItem->setHAlign(QQuickText::HAlignment::AlignHCenter);
        textItem->setVAlign(QQuickText::VAlignment::AlignVCenter);
        textItem->setWidth(rect.width() / categoriesCount);
        textItem->setHeight(rect.height());
        textItem->setFont(m_theme->axisXLabelsFont());
        textItem->setColor(m_theme->axisXLabelsColor());
        textItem->setText(category);
        textIndex++;
    }
}

void QQuickGraphs2DView::updateBarYAxis(QValueAxis *axis, const QRectF &rect)
{
    // TODO: No hard-coded item amount
    double categoriesCountDouble = m_axisVerticalMaxValue + 1;
    int categoriesCount = int(categoriesCountDouble);
    // See if we need more text items
    int currentTextItemsSize = m_yAxisTextItems.size();
    if (currentTextItemsSize < categoriesCount) {
        for (int i = currentTextItemsSize; i <= categoriesCount; i++) {
            //auto bi = new QSGTextNode();
            auto bi = new QQuickText();
            bi->setParentItem(this);
            m_yAxisTextItems << bi;
        }
    } else if (categoriesCount < currentTextItemsSize) {
        // Hide unused text items
        for (int i = categoriesCount;  i < currentTextItemsSize; i++) {
            auto &textItem = m_yAxisTextItems[i];
            textItem->setVisible(false);
        }
    }
    int textIndex = 0;
    for (int i = 0;  i < categoriesCount; i++) {
        auto &textItem = m_yAxisTextItems[textIndex];
        // TODO: Not general, fix vertical align to work in all cases
        float fontSize = m_theme->axisYLabelsFont().pixelSize() < 0 ? m_theme->axisYLabelsFont().pointSize() : m_theme->axisYLabelsFont().pixelSize();
        float posX = rect.x();
        textItem->setX(posX);
        float posY = rect.y() + rect.height() - fontSize * 1.0 - (((float)textIndex) / (categoriesCountDouble-1)) *  rect.height();
        textItem->setY(posY);
        textItem->setHAlign(QQuickText::HAlignment::AlignRight);
        textItem->setVAlign(QQuickText::VAlignment::AlignBottom);
        textItem->setWidth(rect.width());
        //textItem->setHeight(rect.height()  / categoriesCount);
        textItem->setFont(m_theme->axisYLabelsFont());
        textItem->setColor(m_theme->axisYLabelsColor());
        textItem->setText(QString::number(i));
        textItem->setVisible(true);
        textIndex++;
    }
}

void QQuickGraphs2DView::updateBarSeries(QBarSeries *series)
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
    seriesTheme->setGraphSeriesCount(setCount);

    // Sizes required of axis labels
    float axisHeight = 20;
    float axisWidth = 40;
    // Bars area width & hight
    float w = width() - m_marginLeft - m_marginRight - axisWidth;
    float h = height() - m_marginTop - m_marginBottom - axisHeight;
    // Margin between bars.
    float barMargin = 2.0;
    // Max width of a bar if no separation between sets.
    float maxBarWidth = w / (setCount * valuesPerSet) - barMargin;
    // Actual bar width.
    float barWidth = maxBarWidth * 0.5;
    // Helper to keep barsets centered when bar width is less than max width.
    float barCentering = (maxBarWidth - barWidth) * setCount * 0.5;

    if (QBarCategoryAxis *axisX = qobject_cast<QBarCategoryAxis*>(series->axisX())) {
        QRectF xAxisRect(m_marginLeft + axisWidth, m_marginTop + h, w, axisHeight);
        updateBarXAxis(axisX, xAxisRect);
    }

    if (QValueAxis *axisY = qobject_cast<QValueAxis*>(series->axisY())) {
        float rightMargin = 20;
        QRectF yAxisRect(m_marginLeft, m_marginTop, axisWidth - rightMargin, h);
        updateBarYAxis(axisY, yAxisRect);
    }

    // See if we need more rectangle nodes
    int currentSize = m_rectNodes.size();
    if (currentSize < barItems) {
        for (int i = currentSize; i <= barItems; i++) {
            auto bi = new QSGDefaultInternalRectangleNode();
            m_backgroundNode->appendChildNode(bi);
            m_rectNodes << bi;
        }
    }

    float barX = 0;
    float seriesPos = 0;
    float posInSet = 0;
    int barIndex = 0;
    int barIndexInSet = 0;
    int barSerieIndex = 0;
    for (auto s : series->barSets()) {
        QVariantList v = s->values();
        int valuesCount = v.size();
        if (valuesCount == 0)
            continue;
        seriesPos = 0;
        barIndexInSet = 0;
        for (auto variantValue : std::as_const(v)) {
            float value = variantValue.toReal() * series->valuesMultiplier();
            if (value > 0) {
                double maxValues = m_axisVerticalMaxValue > 0 ? 1.0 / m_axisVerticalMaxValue : 100.0;
                float barHeight = h * value * maxValues;
                float barY = m_marginTop + h - barHeight;
                barX = m_marginLeft + axisWidth + seriesPos + posInSet + barCentering;
                QRectF barRect(barX, barY, barWidth, barHeight);
                auto &barItem = m_rectNodes[barIndex];
                barItem->setRect(barRect);
                // Use set colors if available
                QColor c = s->color();
                if (c.alpha() == 0)
                    c = seriesTheme->graphSeriesColor(barSerieIndex);
                c.setAlpha(c.alpha() * series->opacity());
                barItem->setColor(c);
                qreal borderWidth = s->borderWidth();
                if (qFuzzyCompare(borderWidth, -1.0))
                    borderWidth = seriesTheme->borderWidth();
                barItem->setPenWidth(borderWidth);
                QColor borderColor = s->borderColor();
                if (borderColor.alpha() == 0)
                    borderColor = seriesTheme->graphSeriesBorderColor(barSerieIndex);
                borderColor.setAlpha(borderColor.alpha() * series->opacity());
                barItem->setPenColor(borderColor);
                // TODO: Required because of QTBUG-117892
                barItem->setTopLeftRadius(-1);
                barItem->setTopRightRadius(-1);
                barItem->setBottomLeftRadius(-1);
                barItem->setBottomRightRadius(-1);
                barItem->setRadius(4.0);
                barItem->setAntialiasing(true);
                barItem->update();
            }
            barIndex++;
            barIndexInSet++;
            seriesPos = ((float)barIndexInSet / valuesPerSet) * w;
        }
        posInSet += barWidth + barMargin;
        barSerieIndex++;
    }
}

void QQuickGraphs2DView::updateLineSeries(QLineSeries *series)
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

    // Sizes required of axis labels
    float axisHeight = 20;
    float axisWidth = 40;
    // Bars area width & hight
    float w = width() - m_marginLeft - m_marginRight - axisWidth;
    float h = height() - m_marginTop - m_marginBottom - axisHeight;

    auto &&points = series->points();
    if (points.count() > 0) {
        line->shapePath->setStartX(m_marginLeft + axisWidth + w * points[0].x() * 0.05f);
        line->shapePath->setStartY(m_marginTop + h - h * points[0].y() * 0.05f);
        for (int i = 1; i < points.count(); ++i) {
            line->paths[i - 1]->setX(m_marginLeft + axisWidth + w * points[i].x() * 0.05f);
            line->paths[i - 1]->setY(m_marginTop + h - h * points[i].y() * 0.05f);
        }
    }
}






// ***** Static QQmlListProperty methods *****

QQmlListProperty<QObject> QQuickGraphs2DView::seriesList()
{
    return QQmlListProperty<QObject>(this, this,
                                          &QQuickGraphs2DView::appendSeriesFunc,
                                          &QQuickGraphs2DView::countSeriesFunc,
                                          &QQuickGraphs2DView::atSeriesFunc,
                                          &QQuickGraphs2DView::clearSeriesFunc);
}

void QQuickGraphs2DView::appendSeriesFunc(QQmlListProperty<QObject> *list, QObject *series)
{
    reinterpret_cast<QQuickGraphs2DView *>(list->data)->addSeries(series);
}

qsizetype QQuickGraphs2DView::countSeriesFunc(QQmlListProperty<QObject> *list)
{
    return reinterpret_cast<QQuickGraphs2DView *>(list->data)->getSeriesList().size();
}

QObject *QQuickGraphs2DView::atSeriesFunc(QQmlListProperty<QObject> *list, qsizetype index)
{
    return reinterpret_cast<QQuickGraphs2DView *>(list->data)->getSeriesList().at(index);
}

void QQuickGraphs2DView::clearSeriesFunc(QQmlListProperty<QObject> *list)
{
    QQuickGraphs2DView *declItems = reinterpret_cast<QQuickGraphs2DView *>(list->data);
    QList<QObject *> realList = declItems->getSeriesList();
    int count = realList.size();
    for (int i = 0; i < count; i++)
        declItems->removeSeries(realList.at(i));
}

GraphTheme *QQuickGraphs2DView::theme() const
{
    return m_theme;
}

void QQuickGraphs2DView::setTheme(GraphTheme *newTheme)
{
    if (m_theme == newTheme)
        return;

    if (m_theme)
        QObject::disconnect(m_theme, nullptr, this, nullptr);

    m_theme = newTheme;

    if (m_theme)
        QObject::connect(m_theme, &GraphTheme::update, this, &QQuickItem::update);

    emit themeChanged();
}

qreal QQuickGraphs2DView::marginTop() const
{
    return m_marginTop;
}

void QQuickGraphs2DView::setMarginTop(qreal newMarginTop)
{
    if (qFuzzyCompare(m_marginTop, newMarginTop))
        return;
    m_marginTop = newMarginTop;
    emit marginTopChanged();
}

qreal QQuickGraphs2DView::marginBottom() const
{
    return m_marginBottom;
}

void QQuickGraphs2DView::setMarginBottom(qreal newMarginBottom)
{
    if (qFuzzyCompare(m_marginBottom, newMarginBottom))
        return;
    m_marginBottom = newMarginBottom;
    emit marginBottomChanged();
}

qreal QQuickGraphs2DView::marginLeft() const
{
    return m_marginLeft;
}

void QQuickGraphs2DView::setMarginLeft(qreal newMarginLeft)
{
    if (qFuzzyCompare(m_marginLeft, newMarginLeft))
        return;
    m_marginLeft = newMarginLeft;
    emit marginLeftChanged();
}

qreal QQuickGraphs2DView::marginRight() const
{
    return m_marginRight;
}

void QQuickGraphs2DView::setMarginRight(qreal newMarginRight)
{
    if (qFuzzyCompare(m_marginRight, newMarginRight))
        return;
    m_marginRight = newMarginRight;
    emit marginRightChanged();
}

QT_END_NAMESPACE
