// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
// Qt-Security score:significant reason:default


#include "graphs2d/qabstractseries.h"
#include "graphs2d/qabstractseries_p.h"
#if QT_CONFIG(graphs_2d_area)
#include <QtGraphs/qareaseries.h>
#include <private/arearenderer_p.h>
#endif
#if QT_CONFIG(graphs_2d_bar)
#include <QtGraphs/qbarseries.h>
#include <private/barsrenderer_p.h>
#endif
#if QT_CONFIG(graphs_2d_donut_pie)
#include <QtGraphs/qpieseries.h>
#include <private/pierenderer_p.h>
#endif
#if QT_CONFIG(graphs_2d_line) || QT_CONFIG(graphs_2d_area)
#include <QtGraphs/qlineseries.h>
#endif
#if QT_CONFIG(graphs_2d_scatter)
#include <QtGraphs/qscatterseries.h>
#endif
#if QT_CONFIG(graphs_2d_spline)
#include <QtGraphs/qsplineseries.h>
#endif
#if QT_CONFIG(graphs_2d_area) || QT_CONFIG(graphs_2d_line) || QT_CONFIG(graphs_2d_scatter) || QT_CONFIG(graphs_2d_spline)
#include <private/pointrenderer_p.h>
#endif
#if QT_CONFIG(graphs_2d_custom)
#include <QtGraphs/qcustomseries.h>
#include <private/customrenderer_p.h>
#endif
#if QT_CONFIG(graphs_2d_high_performance_backend)
#include "qcpainteritem_p.h"
#endif
#include <QTimer>
#include <QtQuick/private/qquickpinchhandler_p.h>
#include <QtQuick/private/qquickrectangle_p.h>
#include <QtCore/qset.h>
#include <private/axisrenderer_p.h>
#include <private/qabstractaxis_p.h>
#include <private/qgraphsview_p.h>

#include <qtgraphs_tracepoints_p.h>

QT_BEGIN_NAMESPACE

Q_TRACE_PREFIX(qtgraphs,
              "QT_BEGIN_NAMESPACE" \
               "class QGraphsView;" \
              "QT_END_NAMESPACE"
          )

Q_TRACE_POINT(qtgraphs, QGraphs2DGraphsVieUpdatePolish_entry);
Q_TRACE_POINT(qtgraphs, QGraphs2DGraphsVieUpdatePolish_exit);

Q_TRACE_POINT(qtgraphs, QGraphs2DGraphsViewComponentComplete_entry);
Q_TRACE_POINT(qtgraphs, QGraphs2DGraphsViewComponentComplete_exit);

Q_TRACE_POINT(qtgraphs, QGraphs2DGraphsViewInsertSeries_entry);
Q_TRACE_POINT(qtgraphs, QGraphs2DGraphsViewInsertSeries_exit);

Q_TRACE_POINT(qtgraphs, QGraphs2DGraphsViewCreateBarsRenderer_entry);
Q_TRACE_POINT(qtgraphs, QGraphs2DGraphsViewCreateBarsRenderer_exit);

Q_TRACE_POINT(qtgraphs, QGraphs2DGraphsViewCreateAxisRenderer_entry);
Q_TRACE_POINT(qtgraphs, QGraphs2DGraphsViewCreateAxisRenderer_exit);

Q_TRACE_POINT(qtgraphs, QGraphs2DGraphsViewCreatePointRenderer_entry);
Q_TRACE_POINT(qtgraphs, QGraphs2DGraphsViewCreatePointRenderer_exit);

Q_TRACE_POINT(qtgraphs, QGraphs2DGraphsViewCreatePieRenderer_entry);
Q_TRACE_POINT(qtgraphs, QGraphs2DGraphsViewCreatePieRenderer_exit);

Q_TRACE_POINT(qtgraphs, QGraphs2DGraphsViewCreateAreaRenderer_entry);
Q_TRACE_POINT(qtgraphs, QGraphs2DGraphsViewCreateAreaRenderer_exit);

/*!
    \class QGraphsView
    \inmodule QtGraphs
    \internal
*/

/*!
    \qmltype GraphsView
    \nativetype QGraphsView
    \inqmlmodule QtGraphs
    \ingroup graphs_qml_2D
    \brief Base type for all Qt Graphs views.

This class collects the series and theming together and draws the graphs.
You will need to import Qt Graphs module to use this type:

\snippet doc_src_qmlgraphs.cpp 0

After that you can use GraphsView in your qml files:

\snippet doc_src_qmlgraphs.cpp 10

\image graphsview-minimal.png {Bar graph displaying values for 2023, 2024, and 2025}

\sa BarSeries, LineSeries, BarCategoryAxis, ValueAxis, GraphsTheme
*/

Q_LOGGING_CATEGORY(lcGraphs2D, "qt.graphs2d.general")
Q_LOGGING_CATEGORY(lcViewProperties2D, "qt.graphs2d.graphsview.properties")
Q_LOGGING_CATEGORY(lcEvents2D, "qt.graphs2d.events")
Q_LOGGING_CATEGORY(lcCritical2D, "qt.graphs2d.critical")

QGraphsView::QGraphsView(QQuickItem *parent) :
    QQuickItem(parent)
{
    setFlag(QQuickItem::ItemHasContents);
    setAcceptedMouseButtons(Qt::LeftButton);
    setAcceptHoverEvents(true);
    m_defaultTheme = new QGraphsTheme(this);
    m_pinchHandler = new QQuickPinchHandler(this);
    m_pinchHandler->setTarget(nullptr);

#if QT_CONFIG(graphs_2d_high_performance_backend)
    if (m_useCanvasPainter)
        createCanvasPainter();
#endif

    QObject::connect(m_pinchHandler,
                     &QQuickPinchHandler::scaleChanged,
                     this,
                     &QGraphsView::onPinchScaleChanged);
    QObject::connect(m_pinchHandler,
                     &QQuickPinchHandler::grabChanged,
                     this,
                     &QGraphsView::onPinchGrabChanged);
    m_initialized = true;
}

QGraphsView::~QGraphsView()
{
    const auto slist = m_seriesList;
    for (const auto &s : slist)
        removeSeries(s);
    if (m_axisX)
        m_axisX->d_func()->setGraph(nullptr);
    if (m_axisY)
        m_axisY->d_func()->setGraph(nullptr);
}

void QGraphsView::onPinchScaleChanged(qreal delta)
{
    if (m_axisRenderer)
        m_axisRenderer->handlePinchScale(delta);
}

void QGraphsView::onPinchGrabChanged(QPointingDevice::GrabTransition transition, QEventPoint point)
{
    if (m_axisRenderer)
        m_axisRenderer->handlePinchGrab(transition, point);
}

/*!
    \qmlmethod void GraphsView::addSeries(AbstractSeries series)
    Appends a \a series into GraphsView.
    If the \a series is null, it will not be added. If the \a series already
    belongs to the graph, it will be moved into the end.
*/
/*!
    Appends a \a series into GraphsView.
    If the \a series is null, it will not be added. If the \a series already
    belongs to the graph, it will be moved into the end.
*/
void QGraphsView::addSeries(QObject *series)
{
    insertSeries(m_seriesList.size(), series);
}

/*!
    \qmlmethod void GraphsView::insertSeries(int index, AbstractSeries series)
    Inserts a \a series at the position specified by \a index.
    If the \a series is null, it will not be inserted. If the \a series already
    belongs to the graph, it will be moved into \a index.
*/
/*!
    Inserts a \a series at the position specified by \a index.
    If the \a series is null, it will not be inserted. If the \a series already
    belongs to the graph, it will be moved into \a index.
*/
void QGraphsView::insertSeries(qsizetype index, QObject *object)
{
    if (auto series = qobject_cast<QAbstractSeries *>(object)) {
        Q_TRACE(QGraphs2DGraphsViewInsertSeries_entry);
        series->setGraph(this);
        if (m_seriesList.contains(series)) {
            qsizetype oldIndex = m_seriesList.indexOf(series);
            if (index != oldIndex) {
                m_seriesList.removeOne(series);
                if (oldIndex < index)
                    index--;
                m_seriesList.insert(index, series);
                qCDebug(lcGraphs2D, "series was already in seriesList, removed old series at index: %" PRIdQSIZETYPE
                        " and inserted new one at index: %" PRIdQSIZETYPE,
                        oldIndex, index);
            }
        } else {
            m_seriesList.insert(index, series);

            QObject::connect(series,
                             &QAbstractSeries::update,
                             this,
                             &QGraphsView::updateComponentSizes);
            QObject::connect(series, &QAbstractSeries::update,
                             this, &QGraphsView::polishAndUpdate);
            QObject::connect(series, &QAbstractSeries::hoverEnter,
                             this, &QGraphsView::handleHoverEnter);
            QObject::connect(series, &QAbstractSeries::hoverExit,
                             this, &QGraphsView::handleHoverExit);
            QObject::connect(series, &QAbstractSeries::hover,
                             this, &QGraphsView::handleHover);

#if QT_CONFIG(graphs_2d_donut_pie)
            if (auto pie = qobject_cast<QPieSeries *>(series))
                connect(pie, &QPieSeries::removed, m_pieRenderer, &PieRenderer::markedDeleted);
#endif
            qCDebug(lcGraphs2D) << series << "added to a list at index of" << index;
        }
        Q_TRACE(QGraphs2DGraphsViewInsertSeries_exit);

        updateComponentSizes();
        polishAndUpdate();
    }
}

/*!
    \qmlmethod void GraphsView::removeSeries(AbstractSeries series)
    Removes the \a series from the graph.
*/
/*!
    Removes the \a series from the graph.
*/
void QGraphsView::removeSeries(QObject *object)
{
    if (auto series = qobject_cast<QAbstractSeries *>(object)) {
        series->setGraph(nullptr);
        m_seriesList.removeAll(series);
        auto &cleanupSeriesList = m_cleanupSeriesList[getSeriesRendererIndex(series)];

#if QT_CONFIG(graphs_2d_donut_pie)
        if (auto pie = qobject_cast<QPieSeries *>(series))
            disconnect(pie, &QPieSeries::removed, m_pieRenderer, &PieRenderer::markedDeleted);
#endif
        qCDebug(lcGraphs2D) << "removing" << series << "from seriesList";
        cleanupSeriesList.append(series);
#if QT_CONFIG(graphs_2d_area) || QT_CONFIG(graphs_2d_line) || QT_CONFIG(graphs_2d_scatter) || QT_CONFIG(graphs_2d_spline)
        if (m_pointRenderer)
            m_pointRenderer->seriesAboutToBeRemoved(series);
#endif
#if QT_CONFIG(graphs_2d_bar)
        if (m_barsRenderer)
            m_barsRenderer->seriesAboutToBeRemoved(series);
#endif
#if QT_CONFIG(graphs_2d_donut_pie)
        if (m_pieRenderer)
            m_pieRenderer->seriesAboutToBeRemoved(series);
#endif
#if QT_CONFIG(graphs_2d_area)
        if (m_areaRenderer)
            m_areaRenderer->seriesAboutToBeRemoved(series);
#endif
#if QT_CONFIG(graphs_2d_custom)
        if (m_customRenderer)
            m_customRenderer->seriesAboutToBeRemoved(series);
#endif
        updateComponentSizes();
        polishAndUpdate();
    }
}

/*!
    \qmlmethod void GraphsView::removeSeries(int index)
    Removes the series specified by \a index from the graph.
*/
/*!
    Removes the series specified by \a index from the graph.
*/
void QGraphsView::removeSeries(qsizetype index)
{
    if (index >= 0 && index < m_seriesList.size())
        removeSeries(m_seriesList[index]);
}

/*!
    \qmlmethod bool GraphsView::hasSeries(AbstractSeries series)
    Returns \c true if the \a series is in the graph.
*/
/*!
    Returns \c true if the \a series is in the graph.
*/
bool QGraphsView::hasSeries(QObject *series)
{
    return m_seriesList.contains(series);
}

QPointF QGraphsView::getDataPointCoordinates(QAbstractSeries *series, qreal x, qreal y)
{
#if QT_CONFIG(graphs_2d_line) || QT_CONFIG(graphs_2d_area)
    if (m_pointRenderer)
        return m_pointRenderer->reverseRenderCoordinates(series, x, y);
#else
    Q_UNUSED(series);
    Q_UNUSED(x);
    Q_UNUSED(y);
#endif
    return QPointF();
}


void QGraphsView::addAxis(QAbstractAxis *axis)
{
    if (axis) {
        axis->d_func()->setGraph(this);
        // Ensure AxisRenderer exists
        createAxisRenderer();
        polishAndUpdate();
        QObject::connect(axis, &QAbstractAxis::update, this, &QGraphsView::polishAndUpdate);
        QObject::connect(axis, &QAbstractAxis::update, this, &QGraphsView::updateComponentSizes);
    }
}

void QGraphsView::removeAxis(QAbstractAxis *axis, bool removeAllReferences)
{
    if (axis) {
        axis->d_func()->setGraph(nullptr);
        QObject::disconnect(axis, &QAbstractAxis::update, this, &QGraphsView::polishAndUpdate);
        QObject::disconnect(axis,
                            &QAbstractAxis::update,
                            this,
                             &QGraphsView::updateComponentSizes);
    }

    if (m_axisX == axis)
        m_axisX = nullptr;
    if (m_axisY == axis)
        m_axisY = nullptr;

    if (removeAllReferences) {
        for (auto&& s : m_seriesList) {
            if (auto series = qobject_cast<QAbstractSeries *>(s))
                series->d_func()->removeAxis(axis);
        }
    }

    updateComponentSizes();
    polishAndUpdate();
}

qsizetype QGraphsView::graphSeriesCount() const
{
    return m_graphSeriesCount;
}

void QGraphsView::setGraphSeriesCount(qsizetype count)
{
    if (count > m_graphSeriesCount)
        m_graphSeriesCount = count;
}

#if QT_CONFIG(graphs_2d_bar)
void QGraphsView::createBarsRenderer()
{
    Q_TRACE_SCOPE(QGraphs2DGraphsViewCreateBarsRenderer);
    if (!m_barsRenderer) {
        qCDebug(lcGraphs2D, "creating bars renderer");
        m_barsRenderer = new BarsRenderer(this, clipPlotArea());
        updateComponentSizes();
    }
}
#endif

void QGraphsView::createAxisRenderer()
{
    Q_TRACE_SCOPE(QGraphs2DGraphsViewCreateAxisRenderer);
    if (!m_axisRenderer) {
        qCDebug(lcGraphs2D) << "creating axis renderer.";
        m_axisRenderer = new AxisRenderer(this);
        m_axisRenderer->setZ(-1);
        updateComponentSizes();
    }
}

#if QT_CONFIG(graphs_2d_area) || QT_CONFIG(graphs_2d_line) || QT_CONFIG(graphs_2d_scatter) || QT_CONFIG(graphs_2d_spline)
void QGraphsView::createPointRenderer()
{
    Q_TRACE_SCOPE(QGraphs2DGraphsViewCreatePointRenderer);
    if (!m_pointRenderer) {
        qCDebug(lcGraphs2D, "creating point renderer.");
        m_pointRenderer = new PointRenderer(this, clipPlotArea());
        updateComponentSizes();
    }
}
#endif

#if QT_CONFIG(graphs_2d_donut_pie)
void QGraphsView::createPieRenderer()
{
    Q_TRACE_SCOPE(QGraphs2DGraphsViewCreatePieRenderer);
    if (!m_pieRenderer) {
        qCDebug(lcGraphs2D, "creating pie renderer.");
        m_pieRenderer = new PieRenderer(this, clipPlotArea());
        updateComponentSizes();
    }
}
#endif

#if QT_CONFIG(graphs_2d_area)
void QGraphsView::createAreaRenderer()
{
    Q_TRACE_SCOPE(QGraphs2DGraphsViewCreateAreaRenderer);
    if (!m_areaRenderer) {
        qCDebug(lcGraphs2D, "creating area renderer.");
        m_areaRenderer = new AreaRenderer(this, clipPlotArea());
        updateComponentSizes();
    }
}
#endif

#if QT_CONFIG(graphs_2d_custom)
void QGraphsView::createCustomRenderer()
{
    if (!m_customRenderer) {
        qCDebug(lcGraphs2D, "creating custom renderer.");
        m_customRenderer = new CustomRenderer(this, clipPlotArea());
        updateComponentSizes();
    }
}
#endif

#if QT_CONFIG(graphs_2d_high_performance_backend)
void QGraphsView::createCanvasPainter()
{
    if (!m_painterItem) {
        qCDebug(lcGraphs2D, "creating canvas painter item.");
        m_painterItem = new QCPainterItem(m_areaRenderer, m_barsRenderer, m_pieRenderer,
                                          m_pointRenderer, m_customRenderer, this);
        m_painterItem->setAlphaBlending(true);
        m_painterItem->setFillColor(Qt::transparent);
        m_painterItem->setAntialiasing(true);

        if (m_initialized)
            updateComponentSizes();
    }
}

void QGraphsView::removeCanvasPainter()
{
    if (m_painterItem) {
        m_painterItem->deleteLater();
        m_painterItem = nullptr;
    }
}
#endif

/*!
    \property QGraphsView::axisXSmoothing
    \brief Controls the graph X axis smoothing (antialiasing) amount.
    By default, the smoothing is \c 1.0.
*/
/*!
    \qmlproperty real GraphsView::axisXSmoothing
    Controls the graph X axis smoothing (antialiasing) amount.
    By default, the smoothing is \c 1.0.
*/
qreal QGraphsView::axisXSmoothing() const
{
    return m_axisXSmoothing;
}

void QGraphsView::setAxisXSmoothing(qreal smoothing)
{
    if (QtPrivate::fuzzyCompare(m_axisXSmoothing, smoothing)) {
        qCDebug(lcViewProperties2D, "%s axis smoothing is already set to: %.1f",
                qUtf8Printable(QLatin1String(__FUNCTION__)),
                smoothing);
        return;
    }
    m_axisXSmoothing = smoothing;
    emit axisXSmoothingChanged();
    polishAndUpdate();
}

/*!
    \property QGraphsView::axisYSmoothing
    \brief Controls the graph Y axis smoothing (antialiasing) amount.
    By default, the smoothing is \c 1.0.
*/
/*!
    \qmlproperty real GraphsView::axisYSmoothing
    Controls the graph Y axis smoothing (antialiasing) amount.
    By default, the smoothing is \c 1.0.
*/
qreal QGraphsView::axisYSmoothing() const
{
    return m_axisYSmoothing;
}

void QGraphsView::setAxisYSmoothing(qreal smoothing)
{
    if (QtPrivate::fuzzyCompare(m_axisYSmoothing, smoothing)) {
        qCDebug(lcViewProperties2D, "%s value is already set to: %.1f",
                qUtf8Printable(QLatin1String(__FUNCTION__)), smoothing);
        return;
    }
    m_axisYSmoothing = smoothing;
    emit axisYSmoothingChanged();
    polishAndUpdate();
}

/*!
    \property QGraphsView::gridSmoothing
    \brief Controls the graph grid smoothing (antialiasing) amount.
    By default, the smoothing is \c 1.0.
*/
/*!
    \qmlproperty real GraphsView::gridSmoothing
    Controls the graph grid smoothing (antialiasing) amount.
    By default, the smoothing is \c 1.0.
*/
qreal QGraphsView::gridSmoothing() const
{
    return m_gridSmoothing;
}

void QGraphsView::setGridSmoothing(qreal smoothing)
{
    if (QtPrivate::fuzzyCompare(m_gridSmoothing, smoothing)) {
        qCDebug(lcViewProperties2D, "%s value is already set to: %.1f",
                qUtf8Printable(QLatin1String(__FUNCTION__)), smoothing);
        return;
    }
    m_gridSmoothing = smoothing;
    emit gridSmoothingChanged();
    polishAndUpdate();
}

/*!
    \property QGraphsView::shadowVisible
    \brief Controls if the graph grid shadow is visible.
    By default, shadow visibility is set to \c false.
*/
/*!
    \qmlproperty bool GraphsView::shadowVisible
    Controls if the graph grid shadow is visible.
    By default, shadow visibility is set to \c false.
*/
bool QGraphsView::isShadowVisible() const
{
    return m_isShadowVisible;
}

void QGraphsView::setShadowVisible(bool newShadowVisibility)
{
    if (m_isShadowVisible == newShadowVisibility) {
        qCDebug(lcViewProperties2D) << __FUNCTION__
            << "value is already set to:" << newShadowVisibility;
        return;
    }
    m_isShadowVisible = newShadowVisibility;
    emit shadowVisibleChanged();
    polishAndUpdate();
}

/*!
    \property QGraphsView::shadowColor
    \brief Controls the graph grid shadow color.
    By default, shadow color is set to \c black.
*/
/*!
    \qmlproperty color GraphsView::shadowColor
    Controls the graph grid shadow color.
    By default, shadow color is set to \c black.
*/
QColor QGraphsView::shadowColor() const
{
    return m_shadowColor;
}

void QGraphsView::setShadowColor(QColor newShadowColor)
{
    if (m_shadowColor == newShadowColor) {
        qCDebug(lcViewProperties2D, "%s value is already set to: %s",
                qUtf8Printable(QLatin1String(__FUNCTION__)), qUtf8Printable(newShadowColor.name()));
        return;
    }
    m_shadowColor = newShadowColor;
    emit shadowColorChanged();
    polishAndUpdate();
}

/*!
    \property QGraphsView::shadowBarWidth
    \brief Controls the graph grid shadow width.
    By default, shadow width is set to \c 2.0.
*/
/*!
    \qmlproperty real GraphsView::shadowBarWidth
    Controls the graph grid shadow width.
    By default, shadow width is set to \c 2.0.
*/
qreal QGraphsView::shadowBarWidth() const
{
    return m_shadowBarWidth;
}

void QGraphsView::setShadowBarWidth(qreal newShadowBarWidth)
{
    if (QtPrivate::fuzzyCompare(m_shadowBarWidth, newShadowBarWidth)) {
        qCDebug(lcViewProperties2D, "%s value is already set to: %.1f",
                qUtf8Printable(QLatin1String(__FUNCTION__)), newShadowBarWidth);
        return;
    }
    m_shadowBarWidth = newShadowBarWidth;
    emit shadowBarWidthChanged();
    polishAndUpdate();
}

/*!
    \property QGraphsView::shadowXOffset
    \brief Controls the graph grid shadow X offset.
    By default, shadow X offset is set to \c 0.0.
*/
/*!
    \qmlproperty real GraphsView::shadowXOffset
    Controls the graph grid shadow X offset.
    By default, shadow X offset is set to \c 0.0.
*/
qreal QGraphsView::shadowXOffset() const
{
    return m_shadowXOffset;
}

void QGraphsView::setShadowXOffset(qreal newShadowXOffset)
{
    if (QtPrivate::fuzzyCompare(m_shadowXOffset, newShadowXOffset)) {
        qCDebug(lcViewProperties2D, "%s value is already set to: %.1f",
                qUtf8Printable(QLatin1String(__FUNCTION__)), newShadowXOffset);
        return;
    }
    m_shadowXOffset = newShadowXOffset;
    emit shadowXOffsetChanged();
    polishAndUpdate();
}

/*!
    \property QGraphsView::shadowYOffset
    \brief Controls the graph grid shadow Y offset.
    By default, shadow Y offset is set to \c 0.0.
*/
/*!
    \qmlproperty real GraphsView::shadowYOffset
    Controls the graph grid shadow Y offset.
    By default, shadow Y offset is set to \c 0.0.
*/
qreal QGraphsView::shadowYOffset() const
{
    return m_shadowYOffset;
}

void QGraphsView::setShadowYOffset(qreal newShadowYOffset)
{
    if (QtPrivate::fuzzyCompare(m_shadowYOffset, newShadowYOffset)) {
        qCDebug(lcViewProperties2D, "%s value is already set to: %.1f",
                qUtf8Printable(QLatin1String(__FUNCTION__)), newShadowYOffset);
        return;
    }
    m_shadowYOffset = newShadowYOffset;
    emit shadowYOffsetChanged();
    polishAndUpdate();
}

/*!
    \property QGraphsView::shadowSmoothing
    \brief Controls the graph grid shadow smoothing (antialiasing) amount.
    By default, shadow smoothing is set to \c 4.0.
*/
/*!
    \qmlproperty real GraphsView::shadowSmoothing
    Controls the graph grid shadow smoothing (antialiasing) amount.
    By default, shadow smoothing is set to \c 4.0.
*/
qreal QGraphsView::shadowSmoothing() const
{
    return m_shadowSmoothing;
}

void QGraphsView::setShadowSmoothing(qreal smoothing)
{
    if (QtPrivate::fuzzyCompare(m_shadowSmoothing, smoothing)) {
        qCDebug(lcViewProperties2D, "%s value is already set to: %.1f",
                qUtf8Printable(QLatin1String(__FUNCTION__)), smoothing);
        return;
    }
    m_shadowSmoothing = smoothing;
    emit shadowSmoothingChanged();
    polishAndUpdate();
}

void QGraphsView::handleHoverEnter(const QString &seriesName, QPointF position, QPointF value)
{
    if (m_hoverCount == 0)
        emit hoverEnter(seriesName, position, value);
    m_hoverCount++;
}

void QGraphsView::handleHoverExit(const QString &seriesName, QPointF position)
{
    m_hoverCount--;
    if (m_hoverCount == 0)
        emit hoverExit(seriesName, position);
}

void QGraphsView::handleHover(const QString &seriesName, QPointF position, QPointF value)
{
    emit hover(seriesName, position, value);
}

void QGraphsView::updateComponentSizes()
{
    qCDebug(lcEvents2D, "updating component sizes.");

    if (m_axisRenderer) {
        m_axisRenderer->updateAxisMeasurements();
        m_y1AxisWidth = m_axisRenderer->m_y1AxisWidth;
        m_y2AxisWidth = m_axisRenderer->m_y2AxisWidth;
        m_x1AxisHeight = m_axisRenderer->m_x1AxisHeight;
        m_x2AxisHeight = m_axisRenderer->m_x2AxisHeight;
    }

    updateAxisAreas();
    updatePlotArea();

    if (m_axisRenderer)
        m_axisRenderer->setSize(size());

#if QT_CONFIG(graphs_2d_bar)
    if (m_barsRenderer) {
        m_barsRenderer->setX(m_plotArea.x());
        m_barsRenderer->setY(m_plotArea.y());
        m_barsRenderer->setSize(m_plotArea.size());
        qCDebug(lcEvents2D) << "bars graph size:" << m_plotArea.size();
        qCDebug(lcEvents2D, "barsRenderer plotArea x: %f y: %f",
                m_plotArea.x(),
                m_plotArea.y());
    }
#endif
#if QT_CONFIG(graphs_2d_area) || QT_CONFIG(graphs_2d_line) || QT_CONFIG(graphs_2d_scatter) || QT_CONFIG(graphs_2d_spline)
    if (m_pointRenderer) {
        m_pointRenderer->setX(m_plotArea.x());
        m_pointRenderer->setY(m_plotArea.y());
        m_pointRenderer->setSize(m_plotArea.size());
        qCDebug(lcEvents2D) << "point graph size:" << m_plotArea.size();
        qCDebug(lcEvents2D, "pointRenderer plotArea x: %f y: %f",
                m_plotArea.x(),
                m_plotArea.y());

    }
#endif
#if QT_CONFIG(graphs_2d_donut_pie)
    if (m_pieRenderer) {
        m_pieRenderer->setX(m_plotArea.x());
        m_pieRenderer->setY(m_plotArea.y());
        m_pieRenderer->setSize(m_plotArea.size());
        qCDebug(lcEvents2D) << "pie graph size:" << m_plotArea.size();
        qCDebug(lcEvents2D, "pieRenderer plotArea x: %f y: %f",
                m_plotArea.x(),
                m_plotArea.y());

    }
#endif
#if QT_CONFIG(graphs_2d_area)
    if (m_areaRenderer) {
        m_areaRenderer->setX(m_plotArea.x());
        m_areaRenderer->setY(m_plotArea.y());
        m_areaRenderer->setSize(m_plotArea.size());
        qCDebug(lcEvents2D) << "area graph size:" << m_plotArea.size();
        qCDebug(lcEvents2D, "areaRenderer plotArea x: %f y: %f",
                m_plotArea.x(),
                m_plotArea.y());

    }
#endif
#if QT_CONFIG(graphs_2d_custom)
    if (m_customRenderer) {
        m_customRenderer->setX(m_plotArea.x());
        m_customRenderer->setY(m_plotArea.y());
        m_customRenderer->setSize(m_plotArea.size());
        qCDebug(lcEvents2D) << "custom graph size:" << m_plotArea.size();
        qCDebug(lcEvents2D, "customaRenderer plotArea x: %f y: %f", m_plotArea.x(), m_plotArea.y());
    }
#endif
#if QT_CONFIG(graphs_2d_high_performance_backend)
    if (m_painterItem) {
        m_painterItem->setX(m_plotArea.x());
        m_painterItem->setY(m_plotArea.y());
        m_painterItem->setSize(m_plotArea.size());
    }
#endif
}

void QGraphsView::componentComplete()
{
    Q_TRACE(QGraphs2DGraphsViewComponentComplete_entry);
    if (!m_zoomAreaDelegate && !m_zoomAreaItem) {

        QQmlComponent tempZoomAreaDelegate(qmlEngine(this));
        tempZoomAreaDelegate.loadFromModule(u"QtGraphs2D.impl"_s, u"ZoomAreaDelegate"_s);

        m_zoomAreaItem = qobject_cast<QQuickItem *>(
            tempZoomAreaDelegate.create(tempZoomAreaDelegate.creationContext()));
        m_zoomAreaItem->setParent(this);
        m_zoomAreaItem->setParentItem(this);
        m_zoomAreaItem->setVisible(false);
    }

    if (!m_theme) {
        m_theme = m_defaultTheme;
        QObject::connect(m_theme, &QGraphsTheme::update, this, &QQuickItem::update);
        QObject::connect(m_theme, &QGraphsTheme::update,
                         this, &QGraphsView::updateComponentSizes);
        m_theme->resetColorTheme();
    }
    Q_TRACE(QGraphs2DGraphsViewComponentComplete_exit);

    QQuickItem::componentComplete();

    qCDebug(lcEvents2D, "QGraphsView::componentComplete.");

    ensurePolished();
}

void QGraphsView::geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    qCDebug(lcEvents2D) << "QGraphsView::geometryChange."
                        << "oldGeometry:" << oldGeometry
                        << "newGeometry:" << newGeometry;

    QQuickItem::geometryChange(newGeometry, oldGeometry);

    updateComponentSizes();

    ensurePolished();
}

void QGraphsView::hoverMoveEvent(QHoverEvent *event)
{
    bool handled = false;

    // Adjust event position to renderers position
    QPointF localPos = event->position() - m_plotArea.topLeft();
    QHoverEvent mappedEvent(event->type(), localPos,event->globalPosition(),
                            event->oldPosF(), event->modifiers());
    mappedEvent.setAccepted(false);

#if QT_CONFIG(graphs_2d_bar)
    if (m_barsRenderer)
        handled |= m_barsRenderer->handleHoverMove(&mappedEvent);
#endif

#if QT_CONFIG(graphs_2d_area) || QT_CONFIG(graphs_2d_line) || QT_CONFIG(graphs_2d_scatter) || QT_CONFIG(graphs_2d_spline)
    if (m_pointRenderer)
        handled |= m_pointRenderer->handleHoverMove(&mappedEvent);
#endif

#if QT_CONFIG(graphs_2d_donut_pie)
    if (m_pieRenderer)
        handled |= m_pieRenderer->handleHoverMove(&mappedEvent);
#endif

#if QT_CONFIG(graphs_2d_area)
    if (m_areaRenderer)
        handled |= m_areaRenderer->handleHoverMove(&mappedEvent);
#endif

    if (!handled)
        event->ignore();
}

void QGraphsView::wheelEvent(QWheelEvent *event)
{
    bool handled = false;

    // Adjust event position to renderers position
    QPointF localPos = event->position() - m_plotArea.topLeft();
    QWheelEvent mappedEvent(localPos,
                            event->globalPosition(),
                            event->pixelDelta(),
                            event->angleDelta(),
                            event->buttons(),
                            event->modifiers(),
                            event->phase(),
                            event->inverted(),
                            event->source());
    mappedEvent.setAccepted(false);

    if (m_axisRenderer)
        handled |= m_axisRenderer->handleWheel(&mappedEvent);

    if (!handled)
        event->ignore();
    else
        polishAndUpdate();
}

QSGNode *QGraphsView::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *updatePaintNodeData)
{
    Q_UNUSED(updatePaintNodeData);

    for (auto series : std::as_const(m_seriesList)) {
        qCDebug(lcEvents2D) << "QGraphsView::updatePaintNode." << series;
#if QT_CONFIG(graphs_2d_bar)
        if (m_barsRenderer) {
            if (auto barSeries = qobject_cast<QBarSeries *>(series))
                m_barsRenderer->updateSeries(barSeries);
        }
#endif

#if QT_CONFIG(graphs_2d_area) || QT_CONFIG(graphs_2d_line) || QT_CONFIG(graphs_2d_scatter) || QT_CONFIG(graphs_2d_spline)
        if (m_pointRenderer) {
#if QT_CONFIG(graphs_2d_line) || QT_CONFIG(graphs_2d_area)
            if (auto lineSeries = qobject_cast<QLineSeries *>(series))
                m_pointRenderer->updateSeries(lineSeries);
#endif
#if QT_CONFIG(graphs_2d_scatter)
            if (auto scatterSeries = qobject_cast<QScatterSeries *>(series))
                m_pointRenderer->updateSeries(scatterSeries);
#endif
#if QT_CONFIG(graphs_2d_spline)
            if (auto splineSeries = qobject_cast<QSplineSeries *>(series))
                m_pointRenderer->updateSeries(splineSeries);
#endif
        }
#endif

#if QT_CONFIG(graphs_2d_donut_pie)
        if (m_pieRenderer) {
            if (auto pieSeries = qobject_cast<QPieSeries *>(series))
                m_pieRenderer->updateSeries(pieSeries);
        }
#endif

#if QT_CONFIG(graphs_2d_area)
        if (m_areaRenderer) {
            if (auto areaSeries = qobject_cast<QAreaSeries *>(series))
                m_areaRenderer->updateSeries(areaSeries);
        }
#endif

#if QT_CONFIG(graphs_2d_custom)
        if (m_customRenderer) {
            if (auto customSeries = qobject_cast<QCustomSeries *>(series))
                m_customRenderer->updateSeries(customSeries);
        }
#endif
    }

#if QT_CONFIG(graphs_2d_bar)
    if (m_barsRenderer) {
        auto &cleanupSeriesList = m_cleanupSeriesList[0];
        m_barsRenderer->afterUpdate(cleanupSeriesList);
        cleanupSeriesList.clear();
    }
#endif

#if QT_CONFIG(graphs_2d_area) || QT_CONFIG(graphs_2d_line) || QT_CONFIG(graphs_2d_scatter) || QT_CONFIG(graphs_2d_spline)
    if (m_pointRenderer) {
        auto &cleanupSeriesList = m_cleanupSeriesList[1];
        m_pointRenderer->afterUpdate(cleanupSeriesList);
        cleanupSeriesList.clear();
    }
#endif

#if QT_CONFIG(graphs_2d_area)
    if (m_areaRenderer) {
        auto &cleanupSeriesList = m_cleanupSeriesList[2];
        m_areaRenderer->afterUpdate(cleanupSeriesList);
    }
#endif

#if QT_CONFIG(graphs_2d_donut_pie)
    if (m_pieRenderer) {
        auto &cleanupSeriesList = m_cleanupSeriesList[3];
        m_pieRenderer->afterUpdate(cleanupSeriesList);
        cleanupSeriesList.clear();
    }
#endif

#if QT_CONFIG(graphs_2d_custom)
    if (m_customRenderer) {
        auto &cleanupSeriesList = m_cleanupSeriesList[4];
        m_customRenderer->afterUpdate(cleanupSeriesList);
        cleanupSeriesList.clear();
    }
#endif

    // Now possibly dirty theme has been taken into use
    m_theme->resetThemeDirty();

    return oldNode;
}

void QGraphsView::updatePolish()
{
    qCDebug(lcEvents2D, "QGraphsView::updatePolish. Start Update and polish.");

    Q_TRACE_SCOPE(QGraphs2DGraphsVieUpdatePolish);
    if (m_axisRenderer) {
        m_axisRenderer->handlePolish();
        // Initialize shaders after system's event queue
        QTimer::singleShot(0, m_axisRenderer, &AxisRenderer::initialize);
    }
    if (m_theme && m_theme->isBackgroundVisible()) {
        if (!m_backgroundRectangle) {
            // Create m_backgroundRectangle only when it is needed
            m_backgroundRectangle = new QQuickRectangle(this);
            m_backgroundRectangle->setZ(-2);
        }
        m_backgroundRectangle->setColor(m_theme->backgroundColor());
        m_backgroundRectangle->setWidth(width());
        m_backgroundRectangle->setHeight(height());
        m_backgroundRectangle->setVisible(true);
    } else if (m_backgroundRectangle) {
        // Hide and delete the m_backgroundRectangle
        m_backgroundRectangle->setVisible(false);
        m_backgroundRectangle->deleteLater();
        m_backgroundRectangle = nullptr;
    }

    std::sort(m_seriesList.begin(), m_seriesList.end(), [](QObject *lhs, QObject *rhs) {
        auto series1 = qobject_cast<QAbstractSeries *>(lhs);
        auto series2 = qobject_cast<QAbstractSeries *>(rhs);

        if (series1 && series2)
            return series1->zValue() < series2->zValue();
        return false;
    });

    #if QT_CONFIG(graphs_2d_area) || QT_CONFIG(graphs_2d_line) || QT_CONFIG(graphs_2d_scatter) || QT_CONFIG(graphs_2d_spline)
    if (m_pointRenderer)
        m_pointRenderer->resetShapePathCount();
    #endif

    #if QT_CONFIG(graphs_2d_area)
    if (m_areaRenderer)
        m_areaRenderer->resetShapePathCount();
    #endif


    // Polish for all series
#if QT_CONFIG(graphs_2d_bar)
    float highestBarsZ = std::numeric_limits<float>::lowest();
    int barSeriesIndex = 0;
    int barSeriesCount =
            std::count_if(m_seriesList.begin(), m_seriesList.end(),
                          [](const auto &series) { return qobject_cast<QBarSeries *>(series); });
#endif
    for (auto series : std::as_const(m_seriesList)) {
#if QT_CONFIG(graphs_2d_bar)
        if (m_barsRenderer) {
            if (auto barSeries = qobject_cast<QBarSeries *>(series)) {
                m_barsRenderer->handlePolish(barSeries, barSeriesIndex, barSeriesCount);
                if (barSeries->zValue() > highestBarsZ)
                    highestBarsZ = barSeries->zValue();
                barSeriesIndex++;
            }
        }
#endif

#if QT_CONFIG(graphs_2d_area) || QT_CONFIG(graphs_2d_line) || QT_CONFIG(graphs_2d_scatter) || QT_CONFIG(graphs_2d_spline)
        float highestPointZ = std::numeric_limits<float>::lowest();

        if (m_pointRenderer) {
#if QT_CONFIG(graphs_2d_line) || QT_CONFIG(graphs_2d_area)
            if (auto lineSeries = qobject_cast<QLineSeries *>(series)) {
                m_pointRenderer->handlePolish(lineSeries);
                if (lineSeries->zValue() > highestPointZ)
                    highestPointZ = lineSeries->zValue();
            }
#endif

#if QT_CONFIG(graphs_2d_scatter)
            if (auto scatterSeries = qobject_cast<QScatterSeries *>(series)) {
                m_pointRenderer->handlePolish(scatterSeries);
                if (scatterSeries->zValue() > highestPointZ)
                    highestPointZ = scatterSeries->zValue();
            }
#endif

#if QT_CONFIG(graphs_2d_spline)
            if (auto splineSeries = qobject_cast<QSplineSeries *>(series)) {
                m_pointRenderer->handlePolish(splineSeries);
                if (splineSeries->zValue() > highestPointZ)
                    highestPointZ = splineSeries->zValue();
            }
#endif
        }
#endif

#if QT_CONFIG(graphs_2d_donut_pie)
        float highestPieZ = std::numeric_limits<float>::lowest();

        if (m_pieRenderer) {
            if (auto pieSeries = qobject_cast<QPieSeries *>(series)) {
                m_pieRenderer->handlePolish(pieSeries);
                if (pieSeries->zValue() > highestPieZ)
                    highestPieZ = pieSeries->zValue();
            }
        }
#endif

#if QT_CONFIG(graphs_2d_area)
        float highestAreaZ = std::numeric_limits<float>::lowest();

        if (m_areaRenderer) {
            if (auto areaSeries = qobject_cast<QAreaSeries *>(series)) {
                m_areaRenderer->handlePolish(areaSeries);
                if (areaSeries->zValue() > highestAreaZ)
                    highestAreaZ = areaSeries->zValue();
            }
        }
#endif

#if QT_CONFIG(graphs_2d_custom)
        float highestCustomZ = std::numeric_limits<float>::lowest();

        if (m_customRenderer) {
            if (auto customSeries = qobject_cast<QCustomSeries *>(series)) {
                m_customRenderer->handlePolish(customSeries);
                if (customSeries->zValue() > highestCustomZ)
                    highestCustomZ = customSeries->zValue();
            }
        }
#endif
    }

#if QT_CONFIG(graphs_2d_bar)
    if (m_barsRenderer) {
        auto &cleanupSeriesList = m_cleanupSeriesList[0];
        m_barsRenderer->afterPolish(cleanupSeriesList);
        cleanupSeriesList.clear();
        if (highestBarsZ > std::numeric_limits<float>::lowest())
            m_barsRenderer->setZ(highestBarsZ);
    }
#endif
#if QT_CONFIG(graphs_2d_area) || QT_CONFIG(graphs_2d_line) || QT_CONFIG(graphs_2d_scatter) || QT_CONFIG(graphs_2d_spline)
    if (m_pointRenderer) {
        float highestPointZ = std::numeric_limits<float>::lowest();

        auto &cleanupSeriesList = m_cleanupSeriesList[1];
        m_pointRenderer->afterPolish(cleanupSeriesList);
       cleanupSeriesList.clear();
        if (highestPointZ > std::numeric_limits<float>::lowest())
            m_pointRenderer->setZ(highestPointZ);
    }
#endif
#if QT_CONFIG(graphs_2d_area)
    if (m_areaRenderer) {
        float highestAreaZ = std::numeric_limits<float>::lowest();

        auto &cleanupSeriesList = m_cleanupSeriesList[2];
        m_areaRenderer->afterPolish(cleanupSeriesList);
        cleanupSeriesList.clear();
        if (highestAreaZ > std::numeric_limits<float>::lowest())
            m_areaRenderer->setZ(highestAreaZ);
    }
#endif
#if QT_CONFIG(graphs_2d_donut_pie)
    if (m_pieRenderer) {
        float highestPieZ = std::numeric_limits<float>::lowest();

        auto &cleanupSeriesList = m_cleanupSeriesList[3];
        m_pieRenderer->afterPolish(cleanupSeriesList);
        cleanupSeriesList.clear();
        if (highestPieZ > std::numeric_limits<float>::lowest())
            m_pieRenderer->setZ(highestPieZ);
    }
#endif
#if QT_CONFIG(graphs_2d_custom)
    if (m_customRenderer) {
        float highestCustomZ = std::numeric_limits<float>::lowest();

        auto &cleanupSeriesList = m_cleanupSeriesList[4];
        m_customRenderer->afterPolish(cleanupSeriesList);
        cleanupSeriesList.clear();
        if (highestCustomZ > std::numeric_limits<float>::lowest())
            m_customRenderer->setZ(highestCustomZ);
    }
#endif
#if QT_CONFIG(graphs_2d_high_performance_backend)
    if (m_painterItem)
        m_painterItem->update();
#endif
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
    qsizetype count = realList.size();
    for (int i = 0; i < count; i++)
        declItems->removeSeries(realList.at(i));
}

/*!
    \qmlproperty GraphsTheme GraphsView::theme
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
    if (m_theme == newTheme) {
        qCDebug(lcViewProperties2D) << __FUNCTION__
            << "theme is already set to:" << newTheme;
        return;
    }

    if (m_theme)
        QObject::disconnect(m_theme, nullptr, this, nullptr);

    m_theme = newTheme;

    if (!m_theme) {
        m_theme = m_defaultTheme;
        m_theme->resetColorTheme();
    }

    QObject::connect(m_theme, &QGraphsTheme::update, this, &QGraphsView::polishAndUpdate);
    QObject::connect(m_theme, &QGraphsTheme::update,
                     this, &QGraphsView::updateComponentSizes);
    emit themeChanged();
    updateComponentSizes();
    polishAndUpdate();
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
    if (QtPrivate::fuzzyCompare(m_marginTop, newMarginTop)) {
        qCDebug(lcViewProperties2D, "%s value is already set to: %.1f",
                qUtf8Printable(QLatin1String(__FUNCTION__)), newMarginTop);
        return;
    }
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
    if (QtPrivate::fuzzyCompare(m_marginBottom, newMarginBottom)) {
        qCDebug(lcViewProperties2D, "%s value is already set to: %.1f",
                qUtf8Printable(QLatin1String(__FUNCTION__)), newMarginBottom);
        return;
    }
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
    if (QtPrivate::fuzzyCompare(m_marginLeft, newMarginLeft)) {
        qCDebug(lcViewProperties2D, "%s value is already set to: %.1f",
                qUtf8Printable(QLatin1String(__FUNCTION__)), newMarginLeft);
        return;
    }
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
    if (QtPrivate::fuzzyCompare(m_marginRight, newMarginRight)) {
        qCDebug(lcViewProperties2D, "%s value is already set to: %.1f",
                qUtf8Printable(QLatin1String(__FUNCTION__)), newMarginRight);
        return;
    }
    m_marginRight = newMarginRight;
    updateComponentSizes();
    polishAndUpdate();
    emit marginRightChanged();
}

/*!
    \property QGraphsView::clipPlotArea
    \since 6.10
    \brief Controls whether graph items should be clipped
    if they go outside of a plot area. The default value is \c true.

    \sa QGraphsView::plotArea
*/
/*!
    \qmlproperty bool GraphsView::clipPlotArea
    \since 6.10
    Controls whether graph items should be clipped
    if they go outside of a plot area. The default value is \c true.

    \sa plotArea
*/
bool QGraphsView::clipPlotArea() const
{
    return m_clipPlotArea;
}

void QGraphsView::setClipPlotArea(bool enabled)
{
    if (m_clipPlotArea == enabled) {
        qCDebug(lcViewProperties2D, "QGraphsView::setClipPlotArea is already set to %d",
                 enabled);
        return;
    }

    m_clipPlotArea = enabled;
    emit clipPlotAreaChanged();
#if QT_CONFIG(graphs_2d_area) || QT_CONFIG(graphs_2d_line) || QT_CONFIG(graphs_2d_scatter) || QT_CONFIG(graphs_2d_spline)
    if (m_pointRenderer)
        m_pointRenderer->setClip(m_clipPlotArea);
#endif
#if QT_CONFIG(graphs_2d_area)
    if (m_areaRenderer)
        m_areaRenderer->setClip(m_clipPlotArea);
#endif
#if QT_CONFIG(graphs_2d_donut_pie)
    if (m_pieRenderer)
        m_pieRenderer->setClip(m_clipPlotArea);
#endif
#if QT_CONFIG(graphs_2d_bar)
    if (m_barsRenderer)
        m_barsRenderer->setClip(m_clipPlotArea);
#endif
#if QT_CONFIG(graphs_2d_custom)
    if (m_customRenderer)
        m_customRenderer->setClip(m_clipPlotArea);
#endif
}

/*!
    \property QGraphsView::plotArea
    \since 6.9
    \brief The rectangle within which the graph is drawn.

    This is the QGraphsView area minus axis areas and margins.
    \sa marginTop, marginBottom, marginLeft, marginRight
*/
/*!
    \qmlproperty rect GraphsView::plotArea
    \since 6.9
    The rectangle within which the graph is drawn.
    This is the GraphsView area minus axis areas and margins.
    \sa marginTop, marginBottom, marginLeft, marginRight
*/
QRectF QGraphsView::plotArea() const
{
    return m_plotArea;
}

void QGraphsView::updateAxisAreas()
{
    QRectF r = { m_marginLeft,
                 m_marginTop,
                 width() - m_marginLeft - m_marginRight,
                 height() - m_marginTop - m_marginBottom };

    const qreal plotWidth = r.width() - m_y1AxisWidth - m_y2AxisWidth;
    const qreal plotHeight = r.height() - m_x1AxisHeight - m_x2AxisHeight;

    m_x1AxisArea = {r.x() + m_y1AxisWidth,
                    r.y() + r.height() - m_x1AxisHeight,
                    plotWidth,
                    m_x1AxisHeight};

    m_x2AxisArea = {r.x() + m_y1AxisWidth,
                    r.y(),
                    plotWidth,
                    m_x2AxisHeight};

    m_y1AxisArea = {r.x(),
                    r.y() + m_x2AxisHeight,
                    m_y1AxisWidth,
                    plotHeight};

    m_y2AxisArea = {r.x() + r.width() - m_y2AxisWidth,
                    r.y() + m_x2AxisHeight,
                    m_y2AxisWidth,
                    plotHeight};
}

void QGraphsView::updatePlotArea()
{
    // When axis are in left & bottom
    qreal x = m_marginLeft;
    qreal y = m_marginTop;
    qreal w = width() - x - m_marginRight;
    qreal h = height() - y - m_marginBottom;

    y += m_x2AxisHeight;
    x += m_y1AxisWidth;
    h -= m_x1AxisHeight + m_x2AxisHeight;
    w -= m_y1AxisWidth + m_y2AxisWidth;

    w = qMax(w, 0.0);
    h = qMax(h, 0.0);
    QRectF plotArea = QRectF(x, y, w, h);
    if (plotArea != m_plotArea) {
        m_plotArea = plotArea;
        emit plotAreaChanged();
    }
}

/*!
    \property QGraphsView::axisX
    \brief X-axis of this view.

    The x-axis used for the series inside this view.

    \note Setting the same axis to multiple QGraphsViews is not supported.
*/
/*!
    \qmlproperty AbstractAxis GraphsView::axisX
    The x-axis used for the series inside this view.
    \sa axisY

    \note Setting the same axis to multiple GraphsViews is not supported.
*/

QAbstractAxis *QGraphsView::axisX() const
{
    return m_axisX;
}

void QGraphsView::setAxisX(QAbstractAxis *axis)
{
    if (m_axisX == axis) {
        qCDebug(lcViewProperties2D) << __FUNCTION__
            << "value is already set to:" << axis;
        return;
    }
    if (m_axisX)
        removeAxis(m_axisX);
    m_axisX = axis;
    if (axis) {
        if (axis->alignment() != Qt::AlignBottom && axis->alignment() != Qt::AlignTop)
            axis->setAlignment(Qt::AlignBottom);
        addAxis(axis);
    }
    updateComponentSizes();
    emit axisXChanged();
    update();
    polishAndUpdate();
}

/*!
    \property QGraphsView::axisY
    \brief Y-axis of this view.

    The y-axis used for the series inside this view.

    \note Setting the same axis to multiple QGraphsViews is not supported.
*/
/*!
    \qmlproperty AbstractAxis GraphsView::axisY
    The y-axis used for the series inside this view.
    \sa axisX

    \note Setting the same axis to multiple GraphsViews is not supported.
*/

QAbstractAxis *QGraphsView::axisY() const
{
    return m_axisY;
}

void QGraphsView::setAxisY(QAbstractAxis *axis)
{
    if (m_axisY == axis) {
        qCDebug(lcViewProperties2D) << __FUNCTION__
            << "value is already set to:" << axis;
        return;
    }
    if (m_axisY)
        removeAxis(m_axisY);
    m_axisY = axis;
    if (axis) {
        if (axis->alignment() != Qt::AlignLeft && axis->alignment() != Qt::AlignRight)
            axis->setAlignment(Qt::AlignLeft);
        addAxis(axis);
    }
    updateComponentSizes();
    emit axisYChanged();
    update();
    polishAndUpdate();
}

/*!
    \property QGraphsView::orientation
    \brief Orientation of the GraphsView.

    Determines the orientation of the QGraphsView. When the orientation is
    \l {Qt::Horizontal}{Qt::Horizontal}, \l axisX and \l axisY will switch the
    positions so that \l axisX is rendered vertically and \l axisY horizontally.
    This property is currently used by the \l QBarSeries.
    The default value is \l {Qt::Vertical}{Qt::Vertical}.
*/
/*!
    \qmlproperty Qt.Orientation GraphsView::orientation
    Determines the orientation of the GraphsView. When the orientation is
    \l {Qt::Horizontal}{Qt.Horizontal}, \l axisX and \l axisY will switch the
    positions so that \l axisX is rendered vertically and \l axisY horizontally.
    This property is currently used by the \l BarSeries.
    The default value is \l {Qt::Vertical}{Qt.Vertical}.
*/
Qt::Orientation QGraphsView::orientation() const
{
    return m_orientation;
}

void QGraphsView::setOrientation(Qt::Orientation newOrientation)
{
    if (m_orientation == newOrientation) {
        qCDebug(lcViewProperties2D) << __FUNCTION__
            << "value is already set to:" << newOrientation;
        return;
    }
    m_orientation = newOrientation;
    emit orientationChanged();
    update();
    updateComponentSizes();
    polishAndUpdate();
}

#if QT_CONFIG(graphs_2d_custom)
/*
    mapX and mapY functions are used to map custom series data values
    to the real pixel values of the GraphsView
*/
qreal QGraphsView::mapX(QCustomSeries *series, qreal x)
{
    return m_customRenderer->mapX(m_axisRenderer, series, x);
}

qreal QGraphsView::mapY(QCustomSeries *series, qreal y)
{
    return m_customRenderer->mapY(m_axisRenderer, series, y);
}
#endif

CustomRenderer *QGraphsView::customRenderer() const
{
    return m_customRenderer;
}

/*!
    \enum QGraphsView::ZoomStyle
    This enum value describes the zoom style of the graph:

    \value None
        Zooming is disabled.
    \value Center
        Pinch zoom and mouse wheel zoom towards the center of the graph view.
*/

/*!
    \property QGraphsView::zoomStyle
    \brief Zoom style of the GraphsView.

    Determines the zoom style of the QGraphsView. Zooming works by
    manipulating the QValueAxis zoom property. The default value
    is \c {QGraphsView::ZoomStyle::None}.
*/
/*!
    \qmlproperty enumeration GraphsView::zoomStyle
    Determines the zoom style of the GraphsView. Zooming works by
    manipulating the ValueAxis zoom property. The default value
    is \c {GraphsView.ZoomStyle.None}.

    \value GraphsView.ZoomStyle.None
        Zooming is disabled.
    \value GraphsView.ZoomStyle.Center
        Pinch zoom and mouse wheel zoom towards the center of the graph view.

*/
QGraphsView::ZoomStyle QGraphsView::zoomStyle() const
{
    return m_zoomStyle;
}

void QGraphsView::setZoomStyle(ZoomStyle newZoomStyle)
{
    if (m_zoomStyle == newZoomStyle) {
        qCDebug(lcViewProperties2D) << __FUNCTION__
            << "value is already set to:" << newZoomStyle;
        return;
    }
    m_zoomStyle = newZoomStyle;
    emit zoomStyleChanged();
}

/*!
    \enum QGraphsView::PanStyle
    This enum value describes the pan style of the graph:

    \value None
        Panning is disabled.
    \value Drag
        Mouse and touch drag pan the view around.
*/

/*!
    \property QGraphsView::panStyle
    \brief Pan style of the GraphsView.

    Determines the pan style of the QGraphsView. Panning works by
    manipulating the pan property of a QValueAxis.
    The default value is \c {QGraphsView::PanStyle::None}.
*/
/*!
    \qmlproperty enumeration GraphsView::panStyle
    Determines the pan style of the GraphsView. Panning works by
    manipulating the pan property of a ValueAxis.
    The default value is \c {GraphsView.PanStyle.None}.

    \value GraphsView.PanStyle.None
        Panning is disabled.
    \value GraphsView.PanStyle.Drag
        Mouse and touch drag pan the view around.
*/
QGraphsView::PanStyle QGraphsView::panStyle() const
{
    return m_panStyle;
}

void QGraphsView::setPanStyle(PanStyle newPanStyle)
{
    if (m_panStyle == newPanStyle) {
        qCDebug(lcViewProperties2D) << __FUNCTION__
            << "value is already set to:" << newPanStyle;
        return;
    }
    m_panStyle = newPanStyle;
    emit panStyleChanged();
}

/*!
    \property QGraphsView::zoomAreaEnabled
    \brief Enables zoom area

    Zoom area changes mouse and touch dragging to draw a box determined
    by \c zoomAreaDelegate. Upon release the graph QValueAxis zoom and pan
    properties are changed so that the view covers only the area intersected
    by the drawn box.
    \sa zoomAreaDelegate
*/
/*!
    \qmlproperty bool GraphsView::zoomAreaEnabled
    Zoom area changes mouse and touch dragging to draw a box determined
    by \c zoomAreaDelegate. Upon release the graph ValueAxis zoom and pan
    properties are changed so that the view covers only the area intersected
    by the drawn box.
    \sa zoomAreaDelegate
*/
bool QGraphsView::zoomAreaEnabled() const
{
    return m_zoomAreaEnabled;
}

void QGraphsView::setZoomAreaEnabled(bool newZoomAreaEnabled)
{
    if (m_zoomAreaEnabled == newZoomAreaEnabled) {
        qCDebug(lcViewProperties2D) << __FUNCTION__
            << "value is already set to:" << newZoomAreaEnabled;
        return;
    }
    m_zoomAreaEnabled = newZoomAreaEnabled;
    emit zoomAreaEnabledChanged();
}

/*!
    \property QGraphsView::zoomAreaDelegate
    \brief Zoom area visual delegate

    Determines the QML element that is drawn when the user performs a drag
    motion to zoom in to an area.
*/
/*!
    \qmlproperty Component GraphsView::zoomAreaDelegate
    Determines the QML element that is drawn when the user performs a drag
    motion to zoom in to an area.
*/
QQmlComponent *QGraphsView::zoomAreaDelegate() const
{
    return m_zoomAreaDelegate;
}

void QGraphsView::setZoomAreaDelegate(QQmlComponent *newZoomAreaDelegate)
{
    if (m_zoomAreaDelegate == newZoomAreaDelegate) {
        qCDebug(lcViewProperties2D) << __FUNCTION__
            << "value is already set to:" << newZoomAreaDelegate;
        return;
    }
    m_zoomAreaDelegate = newZoomAreaDelegate;

    if (m_zoomAreaDelegate) {
        m_zoomAreaItem = qobject_cast<QQuickItem *>(
            m_zoomAreaDelegate->create(m_zoomAreaDelegate->creationContext()));
        m_zoomAreaItem->setParent(this);
        m_zoomAreaItem->setParentItem(this);
        m_zoomAreaItem->setVisible(false);
    }

    emit zoomAreaDelegateChanged();
}

/*!
    \property QGraphsView::zoomSensitivity
    \brief Zoom value change sensitivity

    Determines how fast zoom value changes while zooming.
*/
/*!
    \qmlproperty real GraphsView::zoomSensitivity
    Determines how fast zoom value changes while zooming.
*/
qreal QGraphsView::zoomSensitivity() const
{
    return m_zoomSensitivity;
}

void QGraphsView::setZoomSensitivity(qreal newZoomSensitivity)
{
    if (QtPrivate::fuzzyCompare(m_zoomSensitivity, newZoomSensitivity)) {
        qCDebug(lcViewProperties2D, "%s value is already set to: %.1f",
                qUtf8Printable(QLatin1String(__FUNCTION__)), newZoomSensitivity);
        return;
    }
    m_zoomSensitivity = newZoomSensitivity;
    emit zoomSensitivityChanged();
}

/*!
    \property QGraphsView::dynamicLabelMargins
    \brief Calculate label margins dynamically based on content

    This property holds whether label margins adjust dynamically.

    By default, labels wider than the allocated margin overlap other
    graph elements. When enabled, the renderer reserves extra space
    for such labels and repositions axes to prevent overlap.
*/
/*!
    \qmlproperty bool GraphsView::dynamicLabelMargins
    This property holds whether label margins adjust dynamically.

    By default, labels wider than the allocated margin overlap other
    graph elements. When enabled, the renderer reserves extra space
    for such labels and repositions axes to prevent overlap.
*/
bool QGraphsView::dynamicLabelMargins() const
{
    return m_dynamicLabelMargins;
}

void QGraphsView::setDynamicLabelMargins(bool newDynamicLabelMargins)
{
    if (m_dynamicLabelMargins == newDynamicLabelMargins) {
        qCDebug(lcViewProperties2D) << __FUNCTION__
                                    << "value is already set to:" << newDynamicLabelMargins;
        return;
    }
    m_dynamicLabelMargins = newDynamicLabelMargins;
    updateComponentSizes();
    emit dynamicLabelMarginsChanged();
    polishAndUpdate();
}

int QGraphsView::getSeriesRendererIndex(QAbstractSeries *series)
{
    int index = 0;
    if (series) {
        switch (QAbstractSeriesPrivate::get(series)->type()) {
        case QAbstractSeries::SeriesType::Bar:
            index = 0;
            break;
        case QAbstractSeries::SeriesType::Scatter:
        case QAbstractSeries::SeriesType::Line:
        case QAbstractSeries::SeriesType::Spline:
            index = 1;
            break;
        case QAbstractSeries::SeriesType::Area:
            index = 2;
            break;
        case QAbstractSeries::SeriesType::Pie:
            index = 3;
            break;
        case QAbstractSeries::SeriesType::Custom:
            index = 4;
            break;
        }
    }
    return index;
}

bool QGraphsView::useCanvasPainter() const
{
#if QT_CONFIG(graphs_2d_high_performance_backend)
    return m_useCanvasPainter;
#else
    return false;
#endif
}

void QGraphsView::setUseCanvasPainter(bool newUseCanvasPainter)
{
#if QT_CONFIG(graphs_2d_high_performance_backend)
    if (m_useCanvasPainter == newUseCanvasPainter)
        return;
    m_useCanvasPainter = newUseCanvasPainter;

    if (m_useCanvasPainter)
        createCanvasPainter();
    else
        removeCanvasPainter();

    emit useCanvasPainterChanged();
#else
    Q_UNUSED(newUseCanvasPainter);
#endif
}

QT_END_NAMESPACE

#include "moc_qgraphsview_p.cpp"
