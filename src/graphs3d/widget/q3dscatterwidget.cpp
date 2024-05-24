// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGraphsWidgets/q3dscatterwidget.h>
#include <private/qquickgraphsscatter_p.h>

QT_BEGIN_NAMESPACE

/*!
 * \class Q3DScatterWidget
 * \inmodule QtGraphs
 * \ingroup graphs_3D
 * \brief The Q3DScatterWidget class provides methods for rendering 3D scatter graphs.
 *
 * This class enables developers to render 3D scatter graphs and view them by freely
 * rotating the scene. Rotation is achieved by holding down the right mouse button
 * and moving the mouse, while zooming is accomplished using the mouse wheel. If
 * enabled, selection is performed with the left mouse button. The scene can be
 * reset to the default camera view by clicking the mouse wheel. On touch devices,
 * rotation is achieved by tap-and-move, selection by tap-and-hold, and zooming
 * by pinch.
 *
 * If no axes are set explicitly to Q3DScatterWidget, temporary default axes with no
 * labels are created. These default axes can be modified via axis accessors,
 * but as soon any axis is set explicitly for the orientation, the default axis
 * for that orientation is destroyed.
 *
 * Q3DScatterWidget supports more than one series visible at the same time.
 *
 * Q3DScatterWidget has transparency support. This feature allows you to adjust
 * the opacity of the scatter points, making them partially see-through,
 * fully transparent, or opaque.
 *
 * \section1 How to construct a minimal Q3DScatterWidget graph
 *
 * First, construct Q3DScatterWidget. Since we are running the graph as the top-level
 * window in this example, we need to clear the \c Qt::FramelessWindowHint flag,
 * which is set by default:
 *
 * \snippet doc_src_q3dscatter_construction.cpp 0
 *
 * Now Q3DScatterWidget is ready to receive data to be rendered. Add one series of 3
 * QVector3D items:
 *
 * \note In the new proxy-series relationship, data is held in series.
 * Therefore, for the proxy to be able to add, delete, or edit the data, it is
 * a prerequisite to create a series first.
 *
 * \snippet doc_src_q3dscatter_construction.cpp 1
 *
 * Finally you will need to set it visible:
 *
 * \snippet doc_src_q3dscatter_construction.cpp 2
 *
 * The complete code needed to create and display this graph is:
 *
 * \snippet doc_src_q3dscatter_construction.cpp 3
 *
 * And this is what those few lines of code produce:
 *
 * \image q3dscatter-minimal.png
 *
 * The scene can be rotated, zoomed into, and an item can be selected to view
 * its position, but no other interactions are included in this minimal code
 * example. You can learn more by familiarizing yourself with the examples
 * provided, like the \l{Simple Scatter Graph}.
 *
 * \sa Q3DBarsWidget, Q3DSurfaceWidget, {Qt Graphs C++ Classes for 3D}
 */

/*!
  \qmltype Scatter3D
  \inherits Abstract3DGraph
  \inqmlmodule QtGraphs
  \ingroup graphs_qml_3D
  \brief 3D scatter graph.

  This type enables developers to render scatter graphs in 3D with Qt Quick.

  You will need to import Qt Graphs module to use this type:

  \snippet doc_src_qmlgraphs.cpp 0

  After that you can use Scatter3D in your qml files:

  \snippet doc_src_qmlgraphs.cpp 2

  See \l{Simple Scatter Graph} for more thorough usage example.

  \sa Scatter3DSeries, ScatterDataProxy, Bars3D, Surface3D, {Qt Graphs C++ Classes for 3D}
 */

/*!
  \qmlproperty Value3DAxis Scatter3D::axisX
  The active x-axis.

  If an axis is not given, a temporary default axis with no labels and an
  automatically adjusting range is created.
  This temporary axis is destroyed if another axis is explicitly set to the same
  orientation.
 */

/*!
  \qmlproperty Value3DAxis Scatter3D::axisY
  The active y-axis.

  If an axis is not given, a temporary default axis with no labels and an
  automatically adjusting range is created.
  This temporary axis is destroyed if another axis is explicitly set to the same
  orientation.
 */

/*!
  \qmlproperty Value3DAxis Scatter3D::axisZ
  The active z-axis.

  If an axis is not given, a temporary default axis with no labels and an
  automatically adjusting range is created.
  This temporary axis is destroyed if another axis is explicitly set to the same
  orientation.
 */

/*!
 * \qmlproperty Scatter3DSeries Scatter3D::selectedSeries
 * The selected series or null.
 */

/*!
 * \qmlproperty list<Scatter3DSeries> Scatter3D::seriesList
 * \qmldefault
 * This property holds the series of the graph.
 * By default, this property contains an empty list.
 * To set the series, either use the addSeries() method or define them as
 * children of the graph.
 */

/*!
 * \qmlmethod void Scatter3D::addSeries(Scatter3DSeries series)
 * Adds the \a series to the graph. A graph can contain multiple series, but has only one set of
 * axes. If the newly added series has specified a selected item, it will be highlighted and
 * any existing selection will be cleared. Only one added series can have an active selection.
 * \sa Abstract3DGraph::hasSeries()
 */

/*!
 * \qmlmethod void Scatter3D::removeSeries(Scatter3DSeries series)
 * Remove the \a series from the graph.
 * \sa Abstract3DGraph::hasSeries()
 */

/*!
 * Constructs a new 3D scatter graph.
 */
Q3DScatterWidget::Q3DScatterWidget()
    : QAbstract3DGraphWidget(QStringLiteral("Scatter3D"))
{}

/*!
 * Destroys the 3D scatter graph.
 */
Q3DScatterWidget::~Q3DScatterWidget() {}

/*!
 * Adds the \a series to the graph. A graph can contain multiple series, but has
 * only one set of axes. If the newly added series has specified a selected
 * item, it will be highlighted and any existing selection will be cleared. Only
 * one added series can have an active selection.
 *
 * \sa QAbstract3DGraphWidget::hasSeries()
 */
void Q3DScatterWidget::addSeries(QScatter3DSeries *series)
{
    graphScatter()->addSeries(series);
}

/*!
 * Removes the \a series from the graph.
 *
 * \sa QAbstract3DGraphWidget::hasSeries()
 */
void Q3DScatterWidget::removeSeries(QScatter3DSeries *series)
{
    graphScatter()->removeSeries(series);
}

/*!
 * Returns the list of series added to this graph.
 *
 * \sa QAbstract3DGraphWidget::hasSeries()
 */
QList<QScatter3DSeries *> Q3DScatterWidget::seriesList() const
{
    QList<QScatter3DSeries *> scatterSeriesList;
    for (QAbstract3DSeries *abstractSeries : graphScatter()->m_seriesList) {
        QScatter3DSeries *scatterSeries = qobject_cast<QScatter3DSeries *>(abstractSeries);
        if (scatterSeries)
            scatterSeriesList.append(scatterSeries);
    }

    return scatterSeriesList;
}

/*!
 * \property Q3DScatterWidget::axisX
 *
 * \brief The active x-axis.
 *
 * Sets \a axis as the active x-axis. Implicitly calls addAxis() to transfer the
 * ownership of the axis to this graph.
 *
 * If \a axis is null, a temporary default axis with no labels and an
 * automatically adjusting range is created. This temporary axis is destroyed if
 * another axis is set explicitly to the same orientation.
 *
 * \sa addAxis(), releaseAxis()
 */
void Q3DScatterWidget::setAxisX(QValue3DAxis *axis)
{
    graphScatter()->setAxisX(axis);
}

QValue3DAxis *Q3DScatterWidget::axisX() const
{
    return static_cast<QValue3DAxis *>(graphScatter()->axisX());
}

/*!
 * \property Q3DScatterWidget::axisY
 *
 * \brief The active y-axis.
 *
 * Sets \a axis as the active y-axis. Implicitly calls addAxis() to transfer the
 * ownership of the axis to this graph.
 *
 * If \a axis is null, a temporary default axis with no labels and an
 * automatically adjusting range is created. This temporary axis is destroyed if
 * another axis is set explicitly to the same orientation.
 *
 * \sa addAxis(), releaseAxis()
 */
void Q3DScatterWidget::setAxisY(QValue3DAxis *axis)
{
    graphScatter()->setAxisY(axis);
}

QValue3DAxis *Q3DScatterWidget::axisY() const
{
    return static_cast<QValue3DAxis *>(graphScatter()->axisY());
}

/*!
 * \property Q3DScatterWidget::axisZ
 *
 * \brief The active z-axis.
 *
 * Sets \a axis as the active z-axis. Implicitly calls addAxis() to transfer the
 * ownership of the axis to this graph.
 *
 * If \a axis is null, a temporary default axis with no labels and an
 * automatically adjusting range is created. This temporary axis is destroyed if
 * another axis is set explicitly to the same orientation.
 *
 * \sa addAxis(), releaseAxis()
 */
void Q3DScatterWidget::setAxisZ(QValue3DAxis *axis)
{
    graphScatter()->setAxisZ(axis);
}

QValue3DAxis *Q3DScatterWidget::axisZ() const
{
    return static_cast<QValue3DAxis *>(graphScatter()->axisZ());
}

/*!
 * \property Q3DScatterWidget::selectedSeries
 *
 * \brief The selected series or null.
 */
QScatter3DSeries *Q3DScatterWidget::selectedSeries() const
{
    return graphScatter()->selectedSeries();
}

/*!
 * Adds \a axis to the graph. The axes added via addAxis are not yet taken to
 * use, addAxis is simply used to give the ownership of the \a axis to the
 * graph. The \a axis must not be null or added to another graph.
 *
 * \sa releaseAxis(), setAxisX(), setAxisY(), setAxisZ()
 */
void Q3DScatterWidget::addAxis(QValue3DAxis *axis)
{
    graphScatter()->addAxis(axis);
}

/*!
 * Releases the ownership of the \a axis back to the caller, if it is added to
 * this graph. If the released \a axis is in use, a new default axis will be
 * created and set active.
 *
 * If the default axis is released and added back later, it behaves as any other
 * axis would.
 *
 * \sa addAxis(), setAxisX(), setAxisY(), setAxisZ()
 */
void Q3DScatterWidget::releaseAxis(QValue3DAxis *axis)
{
    graphScatter()->releaseAxis(axis);
}

/*!
 * Returns the list of all added axes.
 *
 * \sa addAxis()
 */
QList<QValue3DAxis *> Q3DScatterWidget::axes() const
{
    QList<QAbstract3DAxis *> abstractAxes = graphScatter()->axes();
    QList<QValue3DAxis *> retList;
    for (QAbstract3DAxis *axis : abstractAxes)
        retList.append(static_cast<QValue3DAxis *>(axis));

    return retList;
}

/*!
 * \internal
 */
QQuickGraphsScatter *Q3DScatterWidget::graphScatter()
{
    return static_cast<QQuickGraphsScatter *>(m_graphsItem.data());
}

/*!
 * \internal
 */
const QQuickGraphsScatter *Q3DScatterWidget::graphScatter() const
{
    return static_cast<const QQuickGraphsScatter *>(m_graphsItem.data());
}

QT_END_NAMESPACE