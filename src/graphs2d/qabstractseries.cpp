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
    \brief The QAbstractSeries class is a base class for all Qt Graphs for 2D series.

    Usually, the series type specific inherited classes are used instead of the base class.

    \sa QLineSeries, QSplineSeries, QScatterSeries, QBarSeries, QXYSeries
*/
/*!
    \qmltype AbstractSeries
    \instantiates QAbstractSeries
    \inqmlmodule QtGraphs
    \ingroup graphs_qml_2D
    \brief Base type for all Qt Graph series types.

    This type cannot be instantiated directly. Instead, one of the following derived types
    should be used to create a series: LineSeries, SplineSeries, BarSeries, or ScatterSeries.
*/

/*!
    \class QLegendData
    \inmodule QtGraphs
    \ingroup graphs_2D
    \brief The QLegendData struct contains information to display on a sets
    legend marker.

    The information needed to make a visual association between a set and a
    marker include properties such as color, border color, and a name of a set.

    \sa QLineSeries, QSplineSeries, QScatterSeries, QBarSeries, QXYSeries
*/

/*!
    \qmltype LegendData
    \instantiates QLegendData
    \inqmlmodule QtGraphs
    \ingroup graphs_qml__2D
    \brief The LegendData struct contains information to display on a sets
    legend marker.

    The information needed to make a visual association between a set and a
    marker include properties such as color, border color, and a name of a set.
*/

/*!
    \property QLegendData::color
    \brief A legend marker's color.
*/
/*!
    \qmlproperty color LegendData::color
    \brief A legend marker's color.
*/

/*!
    \property QLegendData::borderColor
    \brief A border color of a legend marker.
*/
/*!
    \qmlproperty color LegendData::borderColor
    \brief A border color of a legend marker.
*/

/*!
    \property QLegendData::label
    \brief A name of a legend marker.
*/
/*!
    \qmlproperty string LegendData::label
    \brief A name of a legend marker.
*/

/*!
    \enum QAbstractSeries::SeriesType

    This enum describes the type of the series.

    \value SeriesTypeLine A line graph.
    \value SeriesTypeBar A vertical bar graph.
    \value SeriesTypeScatter A scatter graph.
    \value SeriesTypePie A pie graph.
    \value SeriesTypeSpline A spline graph.
    \value SeriesTypeArea An area graph.
*/

/*!
    \property QAbstractSeries::type
    \brief The type of the series.
*/
/*!
    \qmlproperty enumeration AbstractSeries::type

    The type of the series.

    \value AbstractSeries.SeriesTypeLine A line graph.    
    \value AbstractSeries.SeriesTypeBar A vertical bar graph.
    \value AbstractSeries.SeriesTypeScatter A scatter graph.
    \value AbstractSeries.SeriesTypeSpline A spline graph.
*/

/*!
    \property QAbstractSeries::theme
    \brief The theme of the series.

    The theme used for this series.
*/
/*!
    \qmlproperty QSeriesTheme AbstractSeries::theme
    The theme used for this series.
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
    \property QAbstractSeries::visible
    \brief Visibility of the series.

    The visibility used for this series. By default, \a visible is set to \c true.
*/
/*!
    \qmlproperty bool AbstractSeries::visible
    The visibility used for this series. By default, \a visible is set to \c true.
*/

/*!
    \property QAbstractSeries::selectable
    \brief Controls if the series is selectable.

    Controls if the series can be selected with mouse/touch.
    By default, \a selectable is set to \c false.
*/
/*!
    \qmlproperty bool AbstractSeries::selectable
    Controls if the series can be selected with mouse/touch.
    By default, \a selectable is set to \c false.
*/

/*!
    \property QAbstractSeries::hoverable
    \brief Controls if the series is hoverable.

    Controls if the series can be hovered with mouse/touch.
    By default, \a hoverable is set to \c false.
*/
/*!
    \qmlproperty bool AbstractSeries::hoverable
    Controls if the series can be hovered with mouse/touch.
    By default, \a hoverable is set to \c false.
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
    \property QAbstractSeries::valuesMultiplier
    \brief Controls the series values effective visible value.

    This variable can be used for animating the series values so they scale from 0 to actual value size.
    By default, the valuesMultiplier is 1.0. The valid values range from 0.0 (height 0) to 1.0 (full value).
*/
/*!
    \qmlproperty real AbstractSeries::valuesMultiplier
    This variable can be used for animating the series values so they scale from 0 to actual value size.
    By default, the valuesMultiplier is 1.0. The valid values range from 0.0 (height 0) to 1.0 (full value).
*/

/*!
    \property QAbstractSeries::legendData
    \brief Contains information needed to create a legend marker for a data set in a graph.
    \sa QLegendData
 */
/*!
    \qmlproperty list<LegendData> QAbstractSeries::legendData
    Contains information needed to create a legend marker for a data set in a graph.
*/

/*!
    \fn void QAbstractSeries::legendDataChanged()
    This signal is emitted when legend data changes.
*/

/*!
    \fn void QAbstractSeries::themeChanged()
    This signal is emitted when the series theme changes.
*/

/*!
    \fn void QAbstractSeries::nameChanged()
    This signal is emitted when the series name changes.
*/

/*!
    \fn void QAbstractSeries::visibleChanged()
    This signal is emitted when the series visibility changes.
*/

/*!
    \fn void QAbstractSeries::selectableChanged()
    This signal is emitted when the series selectable changes.
*/

/*!
    \fn void QAbstractSeries::hoverableChanged()
    This signal is emitted when the series hoverable changes.
*/

/*!
    \fn void QAbstractSeries::opacityChanged()
    This signal is emitted when the opacity of the series changes.
*/

/*!
    \fn void QAbstractSeries::valuesMultiplierChanged()
    This signal is emitted when the valuesMultiplier of the series changes.
*/

/*!
    \fn void QAbstractSeries::hoverEnter(QString seriesName, QPointF position, QPointF value)
    This signal is emitted when the series hovering starts. The name of the series is in \a seriesName,
    the mouse/touch position in \a position, and the series value in \a value.
    \note This signal is only emitted when \l hoverable is set to true.
*/

/*!
    \fn void QAbstractSeries::hoverExit(QString seriesName, QPointF position)
    This signal is emitted when the series hovering ends. The name of the series is in \a seriesName,
    and the mouse/touch position in \a position.
    \note This signal is only emitted when \l hoverable is set to true.
*/

/*!
    \fn void QAbstractSeries::hover(QString seriesName, QPointF position, QPointF value)
    This signal is emitted when the series hovering changes. The name of the series is in \a seriesName,
    the mouse/touch position in \a position, and the series value in \a value.
    \note This signal is only emitted when \l hoverable is set to true.
*/

/*!
    \internal
    \brief Constructs QAbstractSeries object with \a parent.
*/

QAbstractSeries::QAbstractSeries(QAbstractSeriesPrivate &dd, QObject *parent)
    : QObject(dd, parent)
{}

/*!
    \brief Virtual destructor for the graph series.
*/
QAbstractSeries::~QAbstractSeries()
{
    //if (d->m_graph)
    //    qFatal("Series still bound to a graph when destroyed!");
    Q_D(QAbstractSeries);
    if (d->m_graph)
        d->m_graph->removeSeries(this);
}

QSeriesTheme *QAbstractSeries::theme() const
{
    const Q_D(QAbstractSeries);
    return d->m_theme;
}

void QAbstractSeries::setTheme(QSeriesTheme *newTheme)
{
    Q_D(QAbstractSeries);
    if (d->m_theme == newTheme)
        return;
    d->m_theme = newTheme;
    update();
    emit themeChanged();
}

QString QAbstractSeries::name() const
{
    const Q_D(QAbstractSeries);
    return d->m_name;
}

void QAbstractSeries::setName(const QString &name)
{
    Q_D(QAbstractSeries);
    if (name != d->m_name) {
        d->m_name = name;
        update();
        emit nameChanged();
    }
}

bool QAbstractSeries::isVisible() const
{
    const Q_D(QAbstractSeries);
    return d->m_visible;
}

void QAbstractSeries::setVisible(bool visible)
{
    Q_D(QAbstractSeries);
    if (visible != d->m_visible) {
        d->m_visible = visible;
        update();
        emit visibleChanged();
    }
}

bool QAbstractSeries::selectable() const
{
    const Q_D(QAbstractSeries);
    return d->m_selectable;
}

void QAbstractSeries::setSelectable(bool selectable)
{
    Q_D(QAbstractSeries);
    if (selectable != d->m_selectable) {
        d->m_selectable = selectable;
        update();
        emit selectableChanged();
    }
}

bool QAbstractSeries::hoverable() const
{
    const Q_D(QAbstractSeries);
    return d->m_hoverable;
}

void QAbstractSeries::setHoverable(bool hoverable)
{
    Q_D(QAbstractSeries);
    if (hoverable != d->m_hoverable) {
        d->m_hoverable = hoverable;
        update();
        emit hoverableChanged();
    }
}

qreal QAbstractSeries::opacity() const
{
    const Q_D(QAbstractSeries);
    return d->m_opacity;
}

void QAbstractSeries::setOpacity(qreal opacity)
{
    Q_D(QAbstractSeries);
    if (opacity != d->m_opacity) {
        d->m_opacity = opacity;
        update();
        emit opacityChanged();
    }
}

qreal QAbstractSeries::valuesMultiplier() const
{
    const Q_D(QAbstractSeries);
    return d->m_valuesMultiplier;
}

void QAbstractSeries::setValuesMultiplier(qreal valuesMultiplier)
{
    Q_D(QAbstractSeries);
    valuesMultiplier = std::clamp(valuesMultiplier, 0.0, 1.0);
    if (valuesMultiplier != d->m_valuesMultiplier) {
        d->m_valuesMultiplier = valuesMultiplier;
        update();
        emit valuesMultiplierChanged();
    }
}

/*!
    Returns the graph that the series belongs to.

    Set automatically when the series is added to the graph,
    and unset when the series is removed from the graph.
*/
QGraphsView *QAbstractSeries::graph() const
{
    const Q_D(QAbstractSeries);
    return d->m_graph;
}

void QAbstractSeries::setGraph(QGraphsView *graph)
{
    Q_D(QAbstractSeries);
    d->m_graph = graph;
    switch (type()) {
    case QAbstractSeries::SeriesTypeBar:
        graph->createBarsRenderer();
        graph->createAxisRenderer();
        break;
    case QAbstractSeries::SeriesTypeScatter:
    case QAbstractSeries::SeriesTypeLine:
    case QAbstractSeries::SeriesTypeSpline:
        graph->createPointRenderer();
        graph->createAxisRenderer();
        break;
    case QAbstractSeries::SeriesTypePie:
        graph->createPieRenderer();
        break;
    case QAbstractSeries::SeriesTypeArea:
        graph->createAreaRenderer();
        graph->createAxisRenderer();
        break;
    default:
        break;
    }

    if (d->m_graph) {
        // Attach pending axes
        for (auto axis : d->m_axes) {
            d->m_graph->addAxis(axis);
            QObject::connect(axis, &QAbstractAxis::update, d->m_graph, &QQuickItem::update);
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
*/
bool QAbstractSeries::attachAxis(QAbstractAxis* axis)
{
    Q_D(QAbstractSeries);
    d->m_axes.append(axis);
    if (d->m_graph) {
        d->m_graph->addAxis(axis);
        QObject::connect(axis, &QAbstractAxis::update, d->m_graph, &QQuickItem::update);
        return true;
    }
    //qWarning("Series not in the graph. Please addSeries to graph first.");
    return false;
}

/*!
    Detaches the axis specified by \a axis from the series.

    Returns \c true if the axis was detached successfully, \c false otherwise.
*/
bool QAbstractSeries::detachAxis(QAbstractAxis* axis)
{
    Q_D(QAbstractSeries);
    d->m_axes.removeAll(axis);
    if (d->m_graph) {
        d->m_graph->removeAxis(axis);
        QObject::disconnect(axis, &QAbstractAxis::update, d->m_graph, &QQuickItem::update);
        return true;
    }
    //qWarning("Series not in the graph. Please addSeries to graph first.");
    return false;
}

/*!
    Returns the list of axes attached to the series. Usually, an x-axis and a y-axis
    are attached to a series, except for QPieSeries, which does not have any axes attached.
 \sa attachAxis(), detachAxis()
 */
QList<QAbstractAxis*> QAbstractSeries::attachedAxes()
{
    Q_D(QAbstractSeries);
    return d->m_axes;
}

const QList<QLegendData> QAbstractSeries::legendData() const
{
    const Q_D(QAbstractSeries);
    return d->m_legendData;
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
    Q_D(QAbstractSeries);

    if (!d->m_theme) {
        d->m_theme = new QSeriesTheme(this);
        d->m_theme->resetColorTheme();
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////

QAbstractSeriesPrivate::QAbstractSeriesPrivate()
    : m_graph(nullptr)
    , m_visible(true)
    , m_opacity(1.0)
    , m_valuesMultiplier(1.0)
{
}

QAbstractSeriesPrivate::~QAbstractSeriesPrivate()
{
}

void QAbstractSeriesPrivate::setLegendData(const QList<QLegendData> &legendData)
{
    Q_Q(QAbstractSeries);
    if (legendData.data() != m_legendData.data()) {
        m_legendData = legendData;
        emit q->legendDataChanged();
    }
}

QT_END_NAMESPACE

#include "moc_qabstractseries.cpp"
#include "moc_qabstractseries_p.cpp"
