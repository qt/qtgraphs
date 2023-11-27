// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <private/qgraphsview_p.h>
#include <QtGraphs2D/qbarseries.h>
#include <QtGraphs2D/qlineseries.h>
#include <QtGraphs2D/qscatterseries.h>

QT_BEGIN_NAMESPACE

QGraphsView::QGraphsView(QQuickItem *parent) :
    QQuickItem(parent)
{
    setFlag(QQuickItem::ItemHasContents);
    setAcceptedMouseButtons(Qt::LeftButton);
    setAcceptHoverEvents(true);
}

QGraphsView::~QGraphsView()
{
}

void QGraphsView::setBackgroundColor(QColor color)
{
    auto &b = m_backgroundBrush;
    if (b.style() != Qt::SolidPattern || color != b.color()) {
        b.setStyle(Qt::SolidPattern);
        b.setColor(color);
        emit backgroundColorChanged();
    }
}

QColor QGraphsView::backgroundColor()
{
    return m_backgroundBrush.color();
}

void QGraphsView::addSeries(QObject *series)
{
    insertSeries(m_seriesList.size(), series);
}

void QGraphsView::insertSeries(int index, QObject *object)
{
    if (auto series = qobject_cast<QAbstractSeries *>(object)) {
        series->setGraph(this);
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
            QObject::connect(series, &QAbstractSeries::hoverEnter, this, &QGraphsView::handleHoverEnter);
            QObject::connect(series, &QAbstractSeries::hoverExit, this, &QGraphsView::handleHoverExit);
            QObject::connect(series, &QAbstractSeries::hover, this, &QGraphsView::handleHover);
            //QObject::connect(series, &QAbstractSeries::visibilityChanged,
            //                 this, &Abstract3DController::handleSeriesVisibilityChanged);
            //series->resetToTheme(*m_themeManager->activeTheme(), oldSize, false);
        }
//        if (series->isVisible())
//            handleSeriesVisibilityChangedBySender(series);

    }
}

void QGraphsView::removeSeries(QObject *object)
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

bool QGraphsView::hasSeries(QObject *series)
{
    return m_seriesList.contains(series);
}


void QGraphsView::addAxis(QAbstractAxis *axis)
{
    if (!m_axis.contains(axis)) {
        m_axis << axis;
        update();
    }
}
void QGraphsView::removeAxis(QAbstractAxis *axis)
{
    if (m_axis.contains(axis)) {
        m_axis.removeAll(axis);
        update();
    }
}

QRectF QGraphsView::seriesRect() const
{
    // When axis are in left & bottom
    qreal x = m_marginLeft + m_axisRenderer->m_axisWidth;
    qreal y = m_marginTop;
    qreal w = width() - x - m_marginRight;
    qreal h = height() - y - m_marginBottom - m_axisRenderer->m_axisHeight;
    return QRectF(x, y, w, h);
}

void QGraphsView::handleHoverEnter(QString seriesName, QPointF position, QPointF value)
{
    if (m_hoverCount == 0)
        emit hoverEnter(seriesName, position, value);
    m_hoverCount++;
}

void QGraphsView::handleHoverExit(QString seriesName, QPointF position)
{
    m_hoverCount--;
    if (m_hoverCount == 0)
        emit hoverExit(seriesName, position);
}

void QGraphsView::handleHover(QString seriesName, QPointF position, QPointF value)
{
    emit hover(seriesName, position, value);
}

void QGraphsView::updateComponentSizes()
{
    if (!m_axisRenderer || !m_barsRenderer || !m_pointRenderer)
        return;

    m_axisRenderer->setSize(size());
    m_barsRenderer->setSize(size());
    m_pointRenderer->setSize(size());
}

void QGraphsView::componentComplete()
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

void QGraphsView::geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    QQuickItem::geometryChange(newGeometry, oldGeometry);

    // TODO: Take margins into account here, so render items
    // sizes already match to their content.

    if (!m_axisRenderer) {
        m_axisRenderer = new AxisRenderer(this);
        m_axisRenderer->setZ(-1);
    }

    if (!m_barsRenderer)
        m_barsRenderer = new BarsRenderer(this);

    if (!m_pointRenderer)
        m_pointRenderer = new PointRenderer(this);

    updateComponentSizes();

    ensurePolished();
}

void QGraphsView::mouseMoveEvent(QMouseEvent *event)
{
    m_pointRenderer->handleMouseMove(event);

    polish();
}

void QGraphsView::mousePressEvent(QMouseEvent *event)
{
    m_barsRenderer->handleMousePress(event);
    m_pointRenderer->handleMousePress(event);

    polish();
}

void QGraphsView::mouseReleaseEvent(QMouseEvent *event)
{
    m_pointRenderer->handleMouseRelease(event);

    polish();
}

void QGraphsView::hoverMoveEvent(QHoverEvent *event)
{
    m_barsRenderer->handleHoverMove(event);
    m_pointRenderer->handleHoverMove(event);
}

QSGNode *QGraphsView::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *updatePaintNodeData)
{
    if (!m_backgroundNode)
        m_backgroundNode = new QSGClipNode();

    // Background node, used for clipping
    QRectF clipRect = boundingRect();
    clipRect.adjust(m_marginLeft + m_axisRenderer->m_axisWidth, m_marginTop, -m_marginRight, -m_marginBottom - m_axisRenderer->m_axisHeight);
    m_backgroundNode->setClipRect(clipRect);
    m_backgroundNode->setIsRectangular(true);
    oldNode = m_backgroundNode;

    for (auto series : std::as_const(m_seriesList)) {
        if (auto barSeries = qobject_cast<QBarSeries*>(series))
            m_barsRenderer->updateBarSeries(barSeries);

        if (auto lineSeries = qobject_cast<QLineSeries*>(series))
            m_pointRenderer->updateSeries(lineSeries);

        if (auto scatterSeries = qobject_cast<QScatterSeries*>(series))
            m_pointRenderer->updateSeries(scatterSeries);
    }

    // Now possibly dirty theme has been taken into use
    m_theme->resetThemeDirty();

    polish();

    return oldNode;
}

void QGraphsView::updatePolish()
{
    m_axisRenderer->handlePolish();

    // Polish for all series
    for (auto series : std::as_const(m_seriesList)) {
        if (auto barSeries = qobject_cast<QBarSeries*>(series))
            m_barsRenderer->handlePolish(barSeries);

        if (auto lineSeries = qobject_cast<QLineSeries*>(series))
            m_pointRenderer->handlePolish(lineSeries);

        if (auto scatterSeries = qobject_cast<QScatterSeries*>(series))
            m_pointRenderer->handlePolish(scatterSeries);
    }
}

// ***** Static QQmlListProperty methods *****

QQmlListProperty<QObject> QGraphsView::seriesList()
{
    return QQmlListProperty<QObject>(this, this,
                                          &QGraphsView::appendSeriesFunc,
                                          &QGraphsView::countSeriesFunc,
                                          &QGraphsView::atSeriesFunc,
                                          &QGraphsView::clearSeriesFunc);
}

void QGraphsView::appendSeriesFunc(QQmlListProperty<QObject> *list, QObject *series)
{
    reinterpret_cast<QGraphsView *>(list->data)->addSeries(series);
}

qsizetype QGraphsView::countSeriesFunc(QQmlListProperty<QObject> *list)
{
    return reinterpret_cast<QGraphsView *>(list->data)->getSeriesList().size();
}

QObject *QGraphsView::atSeriesFunc(QQmlListProperty<QObject> *list, qsizetype index)
{
    return reinterpret_cast<QGraphsView *>(list->data)->getSeriesList().at(index);
}

void QGraphsView::clearSeriesFunc(QQmlListProperty<QObject> *list)
{
    QGraphsView *declItems = reinterpret_cast<QGraphsView *>(list->data);
    QList<QObject *> realList = declItems->getSeriesList();
    int count = realList.size();
    for (int i = 0; i < count; i++)
        declItems->removeSeries(realList.at(i));
}

GraphTheme *QGraphsView::theme() const
{
    return m_theme;
}

void QGraphsView::setTheme(GraphTheme *newTheme)
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

qreal QGraphsView::marginTop() const
{
    return m_marginTop;
}

void QGraphsView::setMarginTop(qreal newMarginTop)
{
    if (qFuzzyCompare(m_marginTop, newMarginTop))
        return;
    m_marginTop = newMarginTop;
    updateComponentSizes();
    update();
    emit marginTopChanged();
}

qreal QGraphsView::marginBottom() const
{
    return m_marginBottom;
}

void QGraphsView::setMarginBottom(qreal newMarginBottom)
{
    if (qFuzzyCompare(m_marginBottom, newMarginBottom))
        return;
    m_marginBottom = newMarginBottom;
    updateComponentSizes();
    update();
    emit marginBottomChanged();
}

qreal QGraphsView::marginLeft() const
{
    return m_marginLeft;
}

void QGraphsView::setMarginLeft(qreal newMarginLeft)
{
    if (qFuzzyCompare(m_marginLeft, newMarginLeft))
        return;
    m_marginLeft = newMarginLeft;
    updateComponentSizes();
    update();
    emit marginLeftChanged();
}

qreal QGraphsView::marginRight() const
{
    return m_marginRight;
}

void QGraphsView::setMarginRight(qreal newMarginRight)
{
    if (qFuzzyCompare(m_marginRight, newMarginRight))
        return;
    m_marginRight = newMarginRight;
    updateComponentSizes();
    update();
    emit marginRightChanged();
}

QT_END_NAMESPACE
