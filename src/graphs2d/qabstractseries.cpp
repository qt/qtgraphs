// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGraphs/qabstractseries.h>
#include <private/qabstractseries_p.h>
#include <private/qgraphsview_p.h>

QT_BEGIN_NAMESPACE

/*!
    \class QAbstractSeries
    \inmodule QtGraphs
    \ingroup graphs_2D
    \brief The QAbstractSeries class is a base class for all Qt Chart series.

    Usually, the series type specific inherited classes are used instead of the base class.

    \sa QXYSeries, QLineSeries, QSplineSeries, QScatterSeries, QAreaSeries, QAbstractBarSeries
    \sa QBarSeries, QStackedBarSeries, QPercentBarSeries, QHorizontalBarSeries
    \sa QHorizontalStackedBarSeries, QHorizontalPercentBarSeries, QPieSeries
*/
/*!
    \qmltype AbstractSeries
    \instantiates QAbstractSeries
    \inqmlmodule QtGraphs
    \ingroup graphs_qml_2D
    \brief Base type for all Qt Chart series types.

    This type cannot be instantiated directly. Instead, one of the following derived types
    should be used to create a series: LineSeries, AreaSeries, BarSeries, StackedBarSeries,
    PercentBarSeries, HorizontalBarSeries, HorizontalStackedBarSeries, HorizontalPercentBarSeries,
    PieSeries, ScatterSeries, SplineSeries, BoxPlotSeries, or CandlestickSeries.
*/

/*!
    \enum QAbstractSeries::SeriesType

    This enum describes the type of the series.

    \value SeriesTypeLine A line chart.
    \value SeriesTypeArea An area chart.
    \value SeriesTypeBar A vertical bar chart.
    \value SeriesTypeStackedBar A vertical stacked bar chart.
    \value SeriesTypePercentBar A vertical percent bar chart.
    \value SeriesTypePie A pie chart.
    \value SeriesTypeScatter A scatter chart.
    \value SeriesTypeSpline A spline chart.
    \value SeriesTypeHorizontalBar A horizontal bar chart.
    \value SeriesTypeHorizontalStackedBar A horizontal stacked bar chart.
    \value SeriesTypeHorizontalPercentBar A horizontal percent bar chart.
    \value SeriesTypeBoxPlot A box plot chart.
    \value SeriesTypeCandlestick A candlestick chart.
*/

/*!
    \property QAbstractSeries::type
    \brief The type of the series.
*/
/*!
    \qmlproperty enumeration AbstractSeries::type

    The type of the series.

    \value AbstractSeries.SeriesTypeLine A line chart.
    \value AbstractSeries.SeriesTypeArea An area chart.
    \value AbstractSeries.SeriesTypeBar A vertical bar chart.
    \value AbstractSeries.SeriesTypeStackedBar A vertical stacked bar chart.
    \value AbstractSeries.SeriesTypePercentBar A vertical percent bar chart.
    \value AbstractSeries.SeriesTypePie A pie chart.
    \value AbstractSeries.SeriesTypeScatter A scatter chart.
    \value AbstractSeries.SeriesTypeSpline A spline chart.
    \value AbstractSeries.SeriesTypeHorizontalBar A horizontal bar chart.
    \value AbstractSeries.SeriesTypeHorizontalStackedBar A horizontal stacked bar chart.
    \value AbstractSeries.SeriesTypeHorizontalPercentBar A horizontal percent bar chart.
    \value AbstractSeries.SeriesTypeBoxPlot A box plot chart.
    \value AbstractSeries.SeriesTypeCandlestick A candlestick chart.
*/

/*!
    \property QAbstractSeries::name
    \brief The name of the series.

    The name is displayed in the legend for the series and it supports HTML formatting.
*/
/*!
    \qmlproperty string AbstractSeries::name
    The name of the series. It is displayed in the legend for the series and it
    supports HTML formatting.
*/

/*!
  \internal TODO: Causes build errors, find out why
    \fn void QAbstractSeries::nameChanged()
    This signal is emitted when the series name changes.
*/

/*!
    \property QAbstractSeries::visible
    \brief whether the series is visible or not.

    By default, \c true.
*/
/*!
    \qmlproperty bool AbstractSeries::visible
    Visibility of the series. By default, \c true.
*/

/*!
  \internal TODO: Causes build errors, find out why
    \fn void QAbstractSeries::visibleChanged()
    This signal is emitted when the series visibility changes.
*/

/*!
    \property QAbstractSeries::opacity
    \brief The opacity of the series.

    By default, the opacity is 1.0. The valid values range from 0.0 (transparent) to 1.0 (opaque).
*/
/*!
    \qmlproperty real AbstractSeries::opacity
    The opacity of the series. By default, the opacity is 1.0.
    The valid values range from 0.0 (transparent) to 1.0 (opaque).
*/

/*!
  \internal TODO: Causes build errors, find out why
    \fn void QAbstractSeries::opacityChanged()
    This signal is emitted when the opacity of the series changes.
*/

/*!
    \internal TODO: Causes build errors, find out why
    \property QAbstractSeries::useOpenGL
    \brief Specifies whether or not drawing the series is accelerated by using OpenGL.

    Acceleration using OpenGL is supported only for QLineSeries and QScatterSeries.
    A line series used as an edge series for QAreaSeries cannot use OpenGL acceleration.
    When a chart contains any series that are drawn with OpenGL, a transparent QOpenGLWidget
    is created on top of the chart plot area. The accelerated series are not drawn on the underlying
    QGraphicsView, but are instead drawn on the created QOpenGLWidget.

    Performance gained from using OpenGL to accelerate series drawing depends on the underlying
    hardware, but in most cases it is significant. For example, on a standard desktop computer,
    enabling OpenGL acceleration for a series typically allows rendering at least a hundred times
    more points without reduction on the frame rate.
    Chart size also has less effect on the frame rate.

    The OpenGL acceleration of series drawing is meant for use cases that need fast drawing of
    large numbers of points. It is optimized for efficiency, and therefore the series using
    it lack support for many features available to non-accelerated series:

    \list
    \li Series animations are not supported for accelerated series.
    \li Point labels are not supported for accelerated series.
    \li Pen styles, marker shapes and light markers are ignored for accelerated series.
        Only solid lines and plain scatter dots are supported.
        The scatter dots may be circular or rectangular, depending on the underlying graphics
        hardware and drivers.
    \li Polar charts do not support accelerated series.
    \li Enabling chart drop shadow or using transparent chart background color is not recommended
        when using accelerated series, as that can slow the frame rate down significantly.
    \endlist

    These additional restrictions stem from the fact that the accelerated series is drawn on a
    separate widget on top of the chart:

    \list
    \li If you draw any graphics items on top of a chart containing an accelerated series,
        the accelerated series is drawn over those items.
    \li To enable QOpenGLWidget to be partially transparent, it needs to be stacked on top of
        all other widgets. This means you cannot have other widgets partially covering the
        chart when using accelerated series.
    \li Accelerated series are not supported for use cases where the graphics scene has more than
        one graphics view attached to it.
    \li Accelerated series are not supported for use cases where the chart has non-default geometry.
        For example, adding transforms to the graphics view causes the accelerated series to
        be drawn in an incorrect position related to the chart.
    \endlist

    The default value is \c{false}.
*/
/*!
    \qmlproperty bool AbstractSeries::useOpenGL
    Specifies whether or not the series is drawn with OpenGL.

    Acceleration using OpenGL is supported only for LineSeries and ScatterSeries.
    A line series used as an edge series for a AreaSeries cannot use OpenGL acceleration.
    When a chart contains any series that are drawn with OpenGL, an additional transparent child
    node is created for the ChartView node. The accelerated series are not drawn on the
    ChartView node, but are instead drawn on the child node.

    Performance gained from using OpenGL to accelerate series drawing depends on the underlying
    hardware, but in most cases it is significant. For example, on a standard desktop computer,
    enabling OpenGL acceleration for a series typically allows rendering at least hundred times
    more points without reduction on the frame rate.
    Chart size also has less effect on the frame rate.
    The biggest performance sink when rendering ChartView is rendering and uploading the underlying
    chart texture. If the underlying chart itself is not changing rapidly, significant extra
    performance is gained from not needing to regenerate the chart texture for each frame.

    The OpenGL acceleration of series drawing is meant for use cases that need fast drawing of
    large numbers of points. It is optimized for efficiency, and therefore the series using
    it lack support for many features available to non-accelerated series:

    \list
    \li Series animations are not supported for accelerated series.
    \li Point labels are not supported for accelerated series.
    \li Pen styles, marker shapes and light markers are ignored for accelerated series.
        Only solid lines and plain scatter dots are supported.
        The scatter dots may be circular or rectangular, depending on the underlying graphics
        hardware and drivers.
    \li Polar charts do not support accelerated series.
    \li Mouse events for series are reported asynchronously.
    \li Enabling chart drop shadow or using transparent chart background color is not recommended
        when using accelerated series, as that can slow the frame rate down significantly.
    \endlist

    The default value is \c{false}.
*/

/*!
  \internal TODO: Causes build errors, find out why
    \fn void QAbstractSeries::useOpenGLChanged()
    This signal is emitted when accelerating the drawing of the series by using OpenGL
    is enabled or disabled.
*/

/*!
    \internal
    \brief Constructs QAbstractSeries object with \a parent.
*/
QAbstractSeries::QAbstractSeries(QAbstractSeriesPrivate &d, QObject *parent) :
    QObject(parent),
    d_ptr(&d)
{
}

/*!
    \brief Virtual destructor for the chart series.
*/
QAbstractSeries::~QAbstractSeries()
{
    //if (d_ptr->m_graph)
    //    qFatal("Series still bound to a chart when destroyed!");
    if (d_ptr->m_graph)
        d_ptr->m_graph->removeSeries(this);
}

SeriesTheme *QAbstractSeries::theme() const
{
    return d_ptr->m_theme;
}

void QAbstractSeries::setTheme(SeriesTheme *newTheme)
{
    if (d_ptr->m_theme == newTheme)
        return;
    d_ptr->m_theme = newTheme;
    emit themeChanged();
}

void QAbstractSeries::setName(const QString &name)
{
    if (name != d_ptr->m_name) {
        d_ptr->m_name = name;
        emit nameChanged();
    }
}

QString QAbstractSeries::name() const
{
    return d_ptr->m_name;
}

void QAbstractSeries::setVisible(bool visible)
{
    if (visible != d_ptr->m_visible) {
        d_ptr->m_visible = visible;
        emit visibleChanged();
    }
}

bool QAbstractSeries::isVisible() const
{
    return d_ptr->m_visible;
}

void QAbstractSeries::setSelectable(bool selectable)
{
    if (selectable != d_ptr->m_selectable) {
        d_ptr->m_selectable = selectable;
        emit selectableChanged();
    }
}

bool QAbstractSeries::selectable() const
{
    return d_ptr->m_selectable;
}

void QAbstractSeries::setHoverable(bool hoverable)
{
    if (hoverable != d_ptr->m_hoverable) {
        d_ptr->m_hoverable = hoverable;
        emit hoverableChanged();
    }
}

bool QAbstractSeries::hoverable() const
{
    return d_ptr->m_hoverable;
}

qreal QAbstractSeries::opacity() const
{
    return d_ptr->m_opacity;
}

void QAbstractSeries::setOpacity(qreal opacity)
{
    if (opacity != d_ptr->m_opacity) {
        d_ptr->m_opacity = opacity;
        emit opacityChanged();
    }
}

qreal QAbstractSeries::valuesMultiplier() const
{
    return d_ptr->m_valuesMultiplier;
}

void QAbstractSeries::setValuesMultiplier(qreal valuesMultiplier)
{
    if (valuesMultiplier != d_ptr->m_valuesMultiplier) {
        d_ptr->m_valuesMultiplier = valuesMultiplier;
        emit valuesMultiplierChanged();
    }
}

/*!
    Returns the chart that the series belongs to.

    Set automatically when the series is added to the chart,
    and unset when the series is removed from the chart.
*/
QGraphsView *QAbstractSeries::graph() const
{
    return d_ptr->m_graph;
}

void QAbstractSeries::setGraph(QGraphsView *graph)
{
    d_ptr->m_graph = graph;
    switch (type()) {
    case QAbstractSeries::SeriesTypeBar:
        graph->createBarsRenderer();
        graph->createAxisRenderer();
        break;
    case QAbstractSeries::SeriesTypeScatter:
    case QAbstractSeries::SeriesTypeLine:
        graph->createPointRenderer();
        graph->createAxisRenderer();
        break;
    default:
        break;
    }

    if (d_ptr->m_graph) {
        // Attach pending axes
        for (auto axis : d_ptr->m_axes) {
            d_ptr->m_graph->addAxis(axis);
            QObject::connect(axis, &QAbstractAxis::update, d_ptr->m_graph, &QQuickItem::update);
        }
    }
}

/*!
    Sets the visibility of the series to \c true.

    \sa setVisible(), isVisible()
*/
void QAbstractSeries::show()
{
    setVisible(true);
}

/*!
    Sets the visibility of the series to \c false.

    \sa setVisible(), isVisible()
*/
void QAbstractSeries::hide()
{
    setVisible(false);
}

/*!
    Attaches the axis specified by \a axis to the series.

    Returns \c true if the axis was attached successfully, \c false otherwise.

    \note If multiple axes of the same orientation are attached to the same series,
    they will have the same minimum and maximum values.

 \sa QGraphsView::addAxis()
 */
bool QAbstractSeries::attachAxis(QAbstractAxis* axis)
{
    d_ptr->m_axes.append(axis);
    if (d_ptr->m_graph) {
        d_ptr->m_graph->addAxis(axis);
        QObject::connect(axis, &QAbstractAxis::update, d_ptr->m_graph, &QQuickItem::update);
        return true;
    }
    //qWarning("Series not in the chart. Please addSeries to chart first.");
    return false;
}

/*!
    Detaches the axis specified by \a axis from the series.

    Returns \c true if the axis was detached successfully, \c false otherwise.

 \sa QGraphsView::removeAxis()
 */
bool QAbstractSeries::detachAxis(QAbstractAxis* axis)
{
    d_ptr->m_axes.removeAll(axis);
    if (d_ptr->m_graph) {
        d_ptr->m_graph->removeAxis(axis);
        QObject::disconnect(axis, &QAbstractAxis::update, d_ptr->m_graph, &QQuickItem::update);
        return true;
    }
    //qWarning("Series not in the chart. Please addSeries to chart first.");
    return false;
}

/*!
    Returns the list of axes attached to the series. Usually, an x-axis and a y-axis
    are attached to a series, except for QPieSeries, which does not have any axes attached.
 \sa attachAxis(), detachAxis()
 */
QList<QAbstractAxis*> QAbstractSeries::attachedAxes()
{
    return d_ptr->m_axes;
}

QQmlListProperty<QObject> QAbstractSeries::seriesChildren()
{
    return QQmlListProperty<QObject>(this, 0, &QAbstractSeries::appendSeriesChildren ,0,0,0);
}

void QAbstractSeries::appendSeriesChildren(QQmlListProperty<QObject> *list, QObject *element)
{
    // Empty implementation; the children are parsed in componentComplete instead
    Q_UNUSED(list);
    Q_UNUSED(element);
}

void QAbstractSeries::classBegin()
{
}

void QAbstractSeries::componentComplete()
{
    if (!d_ptr->m_theme) {
        qDebug() << "Using default seriestheme!";
        d_ptr->m_theme = new SeriesTheme(this);
        d_ptr->m_theme->resetColorTheme();
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////

QAbstractSeriesPrivate::QAbstractSeriesPrivate(QAbstractSeries *q)
    : q_ptr(q),
      m_graph(nullptr),
      m_visible(true),
      m_opacity(1.0),
      m_valuesMultiplier(1.0)
{
}

QAbstractSeriesPrivate::~QAbstractSeriesPrivate()
{
}

void QAbstractSeriesPrivate::initializeGraphics(QGraphicsItem* parent)
{
    Q_UNUSED(parent);
}

QT_END_NAMESPACE

#include "moc_qabstractseries.cpp"
#include "moc_qabstractseries_p.cpp"
