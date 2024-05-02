// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGraphs/qareaseries.h>
#include <QtGraphs/qbarseries.h>
#include <QtGraphs/qlineseries.h>
#include <QtGraphs/qpieseries.h>
#include <QtGraphs/qscatterseries.h>
#include <QtGraphs/qsplineseries.h>
#include <private/qgraphsview_p.h>
#include <QTimer>

QT_BEGIN_NAMESPACE

/*!
    \qmltype GraphsView
    \instantiates QGraphsView
    \inqmlmodule QtGraphs
    \ingroup graphs_qml_2D
    \brief Base type for all Qt Graphs views.

This class collects the series and theming together and draws the graphs.
You will need to import Qt Graphs module to use this type:

\snippet doc_src_qmlgraphs.cpp 0

After that you can use GraphsView in your qml files:

\snippet doc_src_qmlgraphs.cpp 10

\image graphsview-minimal.png

See \l{Testbed} for more thorough usage examples.

\sa BarSeries, LineSeries, BarCategoryAxis, ValueAxis, GraphTheme
*/

QGraphsView::QGraphsView(QQuickItem *parent) :
    QQuickItem(parent)
{
    setFlag(QQuickItem::ItemHasContents);
    setAcceptedMouseButtons(Qt::LeftButton);
    setAcceptHoverEvents(true);
    setClip(true);
    m_defaultTheme = new QGraphsTheme();
}

QGraphsView::~QGraphsView()
{
    const auto slist = m_seriesList;
    for (const auto &s : slist)
        removeSeries(s);
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
            m_seriesList.insert(index, series);

            QObject::connect(series, &QAbstractSeries::update,
                             this, &QGraphsView::polishAndUpdate);
            if (series->theme()) {
                QObject::connect(series->theme(), &QGraphsTheme::update,
                                 this, &QGraphsView::polishAndUpdate);
            }
            QObject::connect(series, &QAbstractSeries::themeChanged,
                             [this, series] {
                if (series->theme()) {
                    QObject::connect(series->theme(), &QGraphsTheme::update,
                                     this, &QGraphsView::polishAndUpdate);
                }
            });

            QObject::connect(series, &QAbstractSeries::hoverEnter,
                             this, &QGraphsView::handleHoverEnter);
            QObject::connect(series, &QAbstractSeries::hoverExit,
                             this, &QGraphsView::handleHoverExit);
            QObject::connect(series, &QAbstractSeries::hover,
                             this, &QGraphsView::handleHover);
        }
    }
}

void QGraphsView::removeSeries(QObject *object)
{
    if (auto series = reinterpret_cast<QAbstractSeries *>(object)) {
        series->setGraph(nullptr);
        m_seriesList.removeAll(series);
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
        polishAndUpdate();
        QObject::connect(axis, &QAbstractAxis::update, this, &QGraphsView::polishAndUpdate);
    }
}

void QGraphsView::removeAxis(QAbstractAxis *axis)
{
    if (m_axis.contains(axis)) {
        m_axis.removeAll(axis);
        polishAndUpdate();
    }
}

int QGraphsView::graphSeriesCount() const
{
    return m_graphSeriesCount;
}

void QGraphsView::setGraphSeriesCount(int count)
{
    if (count > m_graphSeriesCount)
        m_graphSeriesCount = count;
}

QRectF QGraphsView::seriesRect() const
{
    // When axis are in left & bottom
    qreal x = m_marginLeft;
    if (m_axisRenderer)
        x += m_axisRenderer->m_axisWidth;
    qreal y = m_marginTop;
    qreal w = width() - x - m_marginRight;
    qreal h = height() - y - m_marginBottom;
    if (m_axisRenderer)
        h -= m_axisRenderer->m_axisHeight;
    return QRectF(x, y, w, h);
}

void QGraphsView::createBarsRenderer()
{
    if (!m_barsRenderer)
        m_barsRenderer = new BarsRenderer(this);
}

void QGraphsView::createAxisRenderer()
{
    if (!m_axisRenderer) {
        m_axisRenderer = new AxisRenderer(this);
        m_axisRenderer->setZ(-1);
    }
}

void QGraphsView::createPointRenderer()
{
    if (!m_pointRenderer)
        m_pointRenderer = new PointRenderer(this);
}

void QGraphsView::createPieRenderer()
{
    if (!m_pieRenderer)
        m_pieRenderer = new PieRenderer(this);
}

void QGraphsView::createAreaRenderer()
{
    if (!m_areaRenderer)
        m_areaRenderer = new AreaRenderer(this);
}

qreal QGraphsView::axisXSmoothing() const
{
    return m_axisXSmoothing;
}

void QGraphsView::setAxisXSmoothing(qreal smoothing)
{
    if (qFuzzyCompare(m_axisXSmoothing, smoothing))
        return;
    m_axisXSmoothing = smoothing;
    emit axisXSmoothingChanged();
}

qreal QGraphsView::axisYSmoothing() const
{
    return m_axisYSmoothing;
}

void QGraphsView::setAxisYSmoothing(qreal smoothing)
{
    if (qFuzzyCompare(m_axisYSmoothing, smoothing))
        return;
    m_axisYSmoothing = smoothing;
    emit axisYSmoothingChanged();
}

qreal QGraphsView::gridSmoothing() const
{
    return m_gridSmoothing;
}

void QGraphsView::setGridSmoothing(qreal smoothing)
{
    if (qFuzzyCompare(m_gridSmoothing, smoothing))
        return;
    m_gridSmoothing = smoothing;
    emit gridSmoothingChanged();
}


bool QGraphsView::shadowEnabled() const
{
    return m_shadowEnabled;
}

void QGraphsView::setShadowEnabled(bool newShadowEnabled)
{
    if (m_shadowEnabled == newShadowEnabled)
        return;
    m_shadowEnabled = newShadowEnabled;
    emit shadowEnabledChanged();
}

QColor QGraphsView::shadowColor() const
{
    return m_shadowColor;
}

void QGraphsView::setShadowColor(const QColor &newShadowColor)
{
    if (m_shadowColor == newShadowColor)
        return;
    m_shadowColor = newShadowColor;
    emit shadowColorChanged();
}

qreal QGraphsView::shadowBarWidth() const
{
    return m_shadowBarWidth;
}

void QGraphsView::setShadowBarWidth(qreal newShadowBarWidth)
{
    if (qFuzzyCompare(m_shadowBarWidth, newShadowBarWidth))
        return;
    m_shadowBarWidth = newShadowBarWidth;
    emit shadowBarWidthChanged();
}

qreal QGraphsView::shadowXOffset() const
{
    return m_shadowXOffset;
}

void QGraphsView::setShadowXOffset(qreal newShadowXOffset)
{
    if (qFuzzyCompare(m_shadowXOffset, newShadowXOffset))
        return;
    m_shadowXOffset = newShadowXOffset;
    emit shadowXOffsetChanged();
}

qreal QGraphsView::shadowYOffset() const
{
    return m_shadowYOffset;
}

void QGraphsView::setShadowYOffset(qreal newShadowYOffset)
{
    if (qFuzzyCompare(m_shadowYOffset, newShadowYOffset))
        return;
    m_shadowYOffset = newShadowYOffset;
    emit shadowYOffsetChanged();
}

qreal QGraphsView::shadowSmoothing() const
{
    return m_shadowSmoothing;
}

void QGraphsView::setShadowSmoothing(qreal smoothing)
{
    if (qFuzzyCompare(m_shadowSmoothing, smoothing))
        return;
    m_shadowSmoothing = smoothing;
    emit shadowSmoothingChanged();
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
    if (m_axisRenderer)
        m_axisRenderer->setSize(size());

    if (m_barsRenderer)
        m_barsRenderer->setSize(size());

    if (m_pointRenderer)
        m_pointRenderer->setSize(size());

    if (m_pieRenderer)
        m_pieRenderer->setSize(size());

    if (m_areaRenderer)
        m_areaRenderer->setSize(size());
}

void QGraphsView::componentComplete()
{
    if (!m_theme) {
        m_theme = m_defaultTheme;
        QObject::connect(m_theme, &QGraphsTheme::update, this, &QQuickItem::update);
        m_theme->resetColorTheme();
        m_theme->setGridMainWidth(2.0);
        m_theme->setGridSubWidth(1.0);
    }
    QQuickItem::componentComplete();

    ensurePolished();
}

void QGraphsView::geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    QQuickItem::geometryChange(newGeometry, oldGeometry);

    // TODO: Take margins into account here, so render items
    // sizes already match to their content.

    updateComponentSizes();

    ensurePolished();
}

void QGraphsView::mouseMoveEvent(QMouseEvent *event)
{
    bool handled = false;

    if (m_pointRenderer)
        handled |= m_pointRenderer->handleMouseMove(event);

    if (!handled)
        event->ignore();
    else
        polishAndUpdate();
}

void QGraphsView::mousePressEvent(QMouseEvent *event)
{
    bool handled = false;

    if (m_barsRenderer)
        handled |= m_barsRenderer->handleMousePress(event);

    if (m_pointRenderer)
        handled |= m_pointRenderer->handleMousePress(event);

    if (m_areaRenderer)
        handled |= m_areaRenderer->handleMousePress(event);

    if (!handled)
        event->ignore();
    else
        polishAndUpdate();
}

void QGraphsView::mouseReleaseEvent(QMouseEvent *event)
{
    bool handled = false;

    if (m_pointRenderer)
        handled |= m_pointRenderer->handleMouseRelease(event);

    if (!handled)
        event->ignore();
    else
        polishAndUpdate();
}

void QGraphsView::hoverMoveEvent(QHoverEvent *event)
{
    bool handled = false;

    if (m_barsRenderer)
        handled |= m_barsRenderer->handleHoverMove(event);

    if (m_pointRenderer)
        handled |= m_pointRenderer->handleHoverMove(event);

    if (m_areaRenderer)
        handled |= m_areaRenderer->handleHoverMove(event);

    if (!handled)
        event->ignore();
}

QSGNode *QGraphsView::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *updatePaintNodeData)
{
    Q_UNUSED(updatePaintNodeData);

    if (!m_backgroundNode)
        m_backgroundNode = new QSGClipNode();

    // Background node, used for clipping
    QRectF clipRect = boundingRect();
    qreal widthAdjustment = .0f;
    qreal heightAdjustment = .0f;
    if (m_axisRenderer) {
        widthAdjustment = m_axisRenderer->m_axisWidth;
        heightAdjustment = m_axisRenderer->m_axisHeight;
    }
    clipRect.adjust(m_marginLeft + widthAdjustment, m_marginTop, -m_marginRight, -m_marginBottom - heightAdjustment);
    m_backgroundNode->setClipRect(clipRect);
    m_backgroundNode->setIsRectangular(true);
    oldNode = m_backgroundNode;

    for (auto series : std::as_const(m_seriesList)) {
        if (m_barsRenderer) {
            if (auto barSeries = qobject_cast<QBarSeries *>(series))
                m_barsRenderer->updateSeries(barSeries);
        }

        if (m_pointRenderer) {
            if (auto lineSeries = qobject_cast<QLineSeries *>(series))
                m_pointRenderer->updateSeries(lineSeries);

            if (auto scatterSeries = qobject_cast<QScatterSeries *>(series))
                m_pointRenderer->updateSeries(scatterSeries);

            if (auto splineSeries = qobject_cast<QSplineSeries *>(series))
                m_pointRenderer->updateSeries(splineSeries);
        }

        if (m_pieRenderer) {
            if (auto pieSeries = qobject_cast<QPieSeries *>(series))
                m_pieRenderer->updateSeries(pieSeries);
        }

        if (m_areaRenderer) {
            if (auto areaSeries = qobject_cast<QAreaSeries *>(series))
                m_areaRenderer->updateSeries(areaSeries);
        }
    }

    // Now possibly dirty theme has been taken into use
    m_theme->resetThemeDirty();

    return oldNode;
}

void QGraphsView::updatePolish()
{
    if (m_axisRenderer) {
        m_axisRenderer->handlePolish();
        // Initialize shaders after system's event queue
        QTimer::singleShot(0, m_axisRenderer, &AxisRenderer::initialize);
    }
    // Polish for all series
    for (auto series : std::as_const(m_seriesList)) {
        if (m_barsRenderer) {
            if (auto barSeries = qobject_cast<QBarSeries*>(series))
                m_barsRenderer->handlePolish(barSeries);
        }

        if (m_pointRenderer) {
            if (auto lineSeries = qobject_cast<QLineSeries *>(series))
                m_pointRenderer->handlePolish(lineSeries);

            if (auto scatterSeries = qobject_cast<QScatterSeries *>(series))
                m_pointRenderer->handlePolish(scatterSeries);

            if (auto splineSeries = qobject_cast<QSplineSeries *>(series)) {
                m_pointRenderer->handlePolish(splineSeries);
            }
        }

        if (m_pieRenderer) {
            if (auto pieSeries = qobject_cast<QPieSeries *>(series))
                m_pieRenderer->handlePolish(pieSeries);
        }

        if (m_areaRenderer) {
            if (auto areaSeries = qobject_cast<QAreaSeries *>(series))
                m_areaRenderer->handlePolish(areaSeries);
        }
    }
}

void QGraphsView::polishAndUpdate()
{
    polish();
    update();
}

// ***** Static QQmlListProperty methods *****

/*!
    \qmlproperty list GraphsView::seriesList

    List of series that are rendered by the GraphsView. Filled automatically
    with the series type children of the GraphsView.

    This is the default property, so child elements are automatically added
    into the series list.
    \sa BarSeries, LineSeries, ScatterSeries
*/
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

/*!
    \qmlproperty GraphTheme GraphsView::theme
    The theme used by the graph. Determines coloring,
    axis lines, fonts etc. If theme has not been set,
    the default theme is used.
*/
QGraphsTheme *QGraphsView::theme() const
{
    return m_theme;
}

void QGraphsView::setTheme(QGraphsTheme *newTheme)
{
    if (m_theme == newTheme)
        return;

    if (m_theme)
        QObject::disconnect(m_theme, nullptr, this, nullptr);

    m_theme = newTheme;

    if (!m_theme) {
        m_theme = m_defaultTheme;
        m_theme->resetColorTheme();
    }

    QObject::connect(m_theme, &QGraphsTheme::update, this, &QQuickItem::update);
    emit themeChanged();
}

/*!
    \qmlproperty real GraphsView::marginTop
    The amount of empty space on the top of the graph.
    By default, the margin is 20.
*/
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
    polishAndUpdate();
    emit marginTopChanged();
}

/*!
    \qmlproperty real GraphsView::marginBottom
    The amount of empty space on the bottom of the graph.
    By default, the margin is 20.
*/
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
    polishAndUpdate();
    emit marginBottomChanged();
}

/*!
    \qmlproperty real GraphsView::marginLeft
    The amount of empty space on the left of the graph.
    By default, the margin is 20.
*/
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
    polishAndUpdate();
    emit marginLeftChanged();
}

/*!
    \qmlproperty real GraphsView::marginRight
    The amount of empty space on the right of the graph.
    By default, the margin is 20.
*/
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
    polishAndUpdate();
    emit marginRightChanged();
}

/*!
    \property QGraphsView::axisX
    \brief X-axis of this view.

    The x-axis used for the series inside this view.
*/
/*!
    \qmlproperty AbstractAxis GraphsView::axisX
    The x-axis used for the series inside this view.
    \sa axisY
*/

QAbstractAxis *QGraphsView::axisX() const
{
    return m_axisX;
}

void QGraphsView::setAxisX(QAbstractAxis *axis)
{
    removeAxis(m_axisX);
    m_axisX = axis;
    if (axis) {
        axis->setOrientation(Qt::Horizontal);
        addAxis(axis);
    }
    emit update();
}

/*!
    \property QGraphsView::axisY
    \brief Y-axis of this view.

    The y-axis used for the series inside this view.
*/
/*!
    \qmlproperty AbstractAxis QGraphsView::axisY
    The y-axis used for the series inside this view.
    \sa axisX
*/

QAbstractAxis *QGraphsView::axisY() const
{
    return m_axisY;
}

void QGraphsView::setAxisY(QAbstractAxis *axis)
{
    removeAxis(m_axisY);
    m_axisY = axis;
    if (axis) {
        axis->setOrientation(Qt::Vertical);
        addAxis(axis);
    }
    emit update();
}

Qt::Orientation QGraphsView::orientation() const
{
    return m_orientation;
}

void QGraphsView::setOrientation(Qt::Orientation newOrientation)
{
    if (m_orientation == newOrientation)
        return;
    m_orientation = newOrientation;
    emit orientationChanged();
    emit update();
}

QT_END_NAMESPACE
