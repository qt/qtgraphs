// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "q3dbars.h"
#include "qquickgraphsbars_p.h"

QT_BEGIN_NAMESPACE

/*!
 * \class Q3DBars
 * \inmodule QtGraphs
 * \ingroup graphs_3D
 * \brief The Q3DBars class provides methods for rendering 3D bar graphs.
 *
 * This class enables developers to render 3D bar graphs and view them by
 * freely rotating the scene. Rotation is achieved by holding down the right
 * mouse button and moving the mouse, while zooming is accomplished using
 * the mouse wheel. If enabled, selection is performed with the left mouse
 * button. The scene can be reset to the default camera view by clicking the
 * mouse wheel. On touch devices, rotation is achieved by tap-and-move,
 * selection by tap-and-hold, and zooming by pinch.
 *
 * If no axes are set explicitly for Q3DBars, temporary default axes without
 * labels are created. These default axes can be modified via axis accessors,
 * but as soon as any axis is set explicitly for the orientation, the default
 * axis for that orientation is destroyed.
 *
 * Q3DBars supports more than one visible series at the same time. All series
 * don't need to have the same number of rows and columns. Row and column
 * labels are taken from the first added series unless explicitly defined
 * for row and column axes.
 *
 * Q3DBars has transparency support. This feature allows you to adjust the
 * opacity of the bars, making them partially see-through, fully transparent,
 * or opaque.
 *
 * \section1 How to construct a minimal Q3DBars graph
 *
 * First, construct an instance of Q3DBars. Since we are running the graph as
 * a top-level window in this example, we need to clear the \c
 * Qt::FramelessWindowHint flag, which is set by default:
 *
 * \snippet doc_src_q3dbars_construction.cpp 4
 *
 * After constructing Q3DBars, you can set the data window by changing the range
 * on the row and column axes. It is not mandatory, as the data window will default
 * to showing all of the data in the series. If the amount of data is large, it
 * is usually preferable to show just a portion of it. For example, let's set
 * the data window to display the first five rows and columns:
 *
 * \snippet doc_src_q3dbars_construction.cpp 0
 *
 * Now, Q3DBars is ready to receive data to be rendered. Create a series with one
 * row of 5 values:
 *
 * \snippet doc_src_q3dbars_construction.cpp 1
 *
 * \note We set the data window to 5 x 5, but we are adding only one row of
 * data. This is okay; the rest of the rows will just be blank.
 *
 * Finally you will need to set it visible:
 *
 * \snippet doc_src_q3dbars_construction.cpp 2
 *
 * The complete code needed to create and display this graph is:
 *
 * \snippet doc_src_q3dbars_construction.cpp 3
 *
 * And this is what those few lines of code produce:
 *
 * \image q3dbars-minimal.png
 *
 * The scene can be rotated, zoomed into, and a bar can be selected to view its
 * value, but no other interactions are included in this minimal code example.
 * You can learn more by familiarizing yourself with the examples provided, like
 * the \l{Simple Bar Graph}.
 *
 * \sa Q3DScatter, Q3DSurface, {Qt Graphs C++ Classes for 3D}
 */

/*!
 * \qmltype Bars3D
 * \inherits AbstractGraph3D
 * \inqmlmodule QtGraphs
 * \ingroup graphs_qml_3D
 * \brief 3D bar graph.
 *
 * This type enables developers to render bar graphs in 3D with Qt Quick.
 *
 * You will need to import Qt Graphs module to use this type:
 *
 * \snippet doc_src_qmlgraphs.cpp 0
 *
 * After that you can use Bars3D in your qml files:
 *
 * \snippet doc_src_qmlgraphs.cpp 1
 *
 * See \l{Simple Bar Graph} for more thorough usage example.
 *
 * \sa Bar3DSeries, ItemModelBarDataProxy, Scatter3D, Surface3D, {Qt Graphs C++ Classes for 3D}
 */

/*!
 * \qmlproperty Category3DAxis Bars3D::rowAxis
 * The active row axis.
 *
 * If an axis is not given, a temporary default axis with no labels is created.
 * This temporary axis is destroyed if another axis is explicitly set to the
 * same orientation.
 */

/*!
 * \qmlproperty Value3DAxis Bars3D::valueAxis
 * The active value axis.
 *
 * If an axis is not given, a temporary default axis with no labels and an
 * automatically adjusting range is created.
 * This temporary axis is destroyed if another axis is explicitly set to the
 * same orientation.
 */

/*!
 * \qmlproperty Category3DAxis Bars3D::columnAxis
 * The active column axis.
 *
 * If an axis is not given, a temporary default axis with no labels is created.
 * This temporary axis is destroyed if another axis is explicitly set to the
 * same orientation.
 */

/*!
 * \qmlproperty bool Bars3D::multiSeriesUniform
 * Defines whether bars are to be scaled with proportions set to a single series bar even
 * if there are multiple series displayed. If set to \c {true}, \l{barSpacing}{bar spacing} will
 * be correctly applied only to the X-axis. Preset to \c false by default.
 */

/*!
 * \qmlproperty real Bars3D::barThickness
 * The bar thickness ratio between the X and Z dimensions. The value \c 1.0
 * means that the bars are as wide as they are deep, whereas \c 0.5
 * makes them twice as deep as they are wide.
 */

/*!
 * \qmlproperty size Bars3D::barSpacing
 * Bar spacing in X and Z dimensions.
 *
 * Preset to \c {(1.0, 1.0)} by default. Spacing is affected by the
 * barSpacingRelative property.
 */

/*!
 * \qmlproperty bool Bars3D::barSpacingRelative
 * Whether spacing is absolute or relative to bar thickness.
 *
 * If \c true, the value of \c 0.0 means that the bars are placed
 * side-to-side, \c 1.0 means that a space as wide as the thickness of one bar
 * is left between the bars, and so on. Preset to \c true.
 */

/*!
 * \qmlproperty size Bars3D::barSeriesMargin
 *
 * Margin between series columns in X and Z dimensions. Preset to \c {(0.0, 0.0)} by default.
 * Sensible values are on the range [0,1).
 */

/*!
 * \qmlproperty Bar3DSeries Bars3D::selectedSeries
 * The selected series or \c null. If \l {AbstractGraph3D::selectionMode}{selectionMode} has
 * the \c SelectionMultiSeries flag set, this property holds the series that
 * owns the selected bar.
 */

/*!
 * \qmlproperty list<Bar3DSeries> Bars3D::seriesList
 * \qmldefault
 * The series of the graph.
 * By default, this property contains an empty list.
 * To set the series, either use the addSeries() function or define them as children of the graph.
 */

/*!
 * \qmlproperty Bar3DSeries Bars3D::primarySeries
 * The primary series of the graph. It
 * is used to determine the row and column axis labels when the labels are not explicitly
 * set to the axes.
 *
 * If the specified series is not yet added to the graph, setting it as the
 * primary series will also implicitly add it to the graph.
 *
 * If the primary series itself is removed from the graph, this property
 * resets to default.
 *
 * If the series is null, this property resets to default.
 * Defaults to the first added series or zero if no series are added to the graph.
 */

/*!
 * \qmlproperty real Bars3D::floorLevel
 *
 * The floor level for the bar graph in Y-axis data coordinates.
 *
 * The actual floor level will be restricted by the Y-axis minimum and maximum
 * values.
 * Defaults to zero.
 */

/*!
 * \qmlmethod void Bars3D::addSeries(Bar3DSeries series)
 * Adds the \a series to the graph. A graph can contain multiple series, but only one set of axes,
 * so the rows and columns of all series must match for the visualized data to be meaningful.
 * If the graph has multiple visible series, only the first one added will
 * generate the row or column labels on the axes in cases where the labels are not explicitly set
 * to the axes. If the newly added series has specified a selected bar, it will be highlighted and
 * any existing selection will be cleared. Only one added series can have an active selection.
 * \sa AbstractGraph3D::hasSeries()
 */

/*!
 * \qmlmethod void Bars3D::removeSeries(Bar3DSeries series)
 * Remove the \a series from the graph.
 * \sa AbstractGraph3D::hasSeries()
 */

/*!
 * \qmlmethod void Bars3D::insertSeries(int index, Bar3DSeries series)
 * Inserts the \a series into the position \a index in the series list.
 * If the \a series has already been added to the list, it is moved to the
 * new \a index.
 * \note When moving a series to a new \a index that is after its old index,
 * the new position in list is calculated as if the series was still in its old
 * index, so the final index is actually the \a index decremented by one.
 * \sa AbstractGraph3D::hasSeries()
 */

/*!
 * Constructs a new 3D bar graph.
 */
Q3DBars::Q3DBars()
    : QAbstract3DGraph(QStringLiteral("Bars3D"))
{}

/*!
 * Destroys the 3D bar graph.
 */
Q3DBars::~Q3DBars() {}

/*!
 * \property Q3DBars::primarySeries
 *
 * \brief The primary series of the graph.
 *
 * Sets \a series as the primary series of the graph. The primary series
 * determines the row and column axis labels when the labels are not explicitly
 * set to the axes.
 *
 * If the specified series is not yet added to the graph, setting it as the
 * primary series will also implicitly add it to the graph.
 *
 * If the primary series itself is removed from the graph, this property
 * resets to default.
 *
 * If \a series is null, this property resets to default.
 * Defaults to the first added series or zero if no series are added to the
 * graph.
 */
void Q3DBars::setPrimarySeries(QBar3DSeries *series)
{
    graphBars()->setPrimarySeries(series);
    emit primarySeriesChanged(series);
}

QBar3DSeries *Q3DBars::primarySeries() const
{
    return graphBars()->primarySeries();
}

/*!
 * Adds the \a series to the graph. A graph can contain multiple series, but
 * only one set of axes, so the rows and columns of all series must match for
 * the visualized data to be meaningful. If the graph has multiple visible
 * series, only the primary series will generate the row or column labels on the
 * axes in cases where the labels are not explicitly set for the axes. If the
 * newly added series has specified a selected bar, it will be highlighted and
 * any existing selection will be cleared. Only one added series can have an
 * active selection.
 *
 * \sa seriesList(), primarySeries, QAbstract3DGraph::hasSeries()
 */
void Q3DBars::addSeries(QBar3DSeries *series)
{
    graphBars()->addSeries(series);
}

/*!
 * Removes the \a series from the graph.
 *
 * \sa QAbstract3DGraph::hasSeries()
 */
void Q3DBars::removeSeries(QBar3DSeries *series)
{
    graphBars()->removeSeries(series);
}

/*!
 * Inserts the \a series into the position \a index in the series list.
 * If the \a series has already been added to the list, it is moved to the
 * new \a index.
 * \note When moving a series to a new \a index that is after its old index,
 * the new position in the list is calculated as if the series was still in its
 * old index, so the final index is actually the \a index decremented by one.
 *
 * \sa addSeries(), seriesList(), QAbstract3DGraph::hasSeries()
 */
void Q3DBars::insertSeries(int index, QBar3DSeries *series)
{
    graphBars()->insertSeries(index, series);
}

/*!
 * Returns the list of series added to this graph.
 *
 * \sa QAbstract3DGraph::hasSeries()
 */
QList<QBar3DSeries *> Q3DBars::seriesList() const
{
    QList<QBar3DSeries *> barSeriesList;
    for (QAbstract3DSeries *abstractSeries : graphBars()->m_seriesList) {
        QBar3DSeries *barSeries = qobject_cast<QBar3DSeries *>(abstractSeries);
        if (barSeries)
            barSeriesList.append(barSeries);
    }

    return barSeriesList;
}

/*!
 * \property Q3DBars::multiSeriesUniform
 *
 * \brief Whether bars are to be scaled with proportions set to a single series
 * bar even if there are multiple series displayed.
 *
 * If set to \c {true}, \l{barSpacing}{bar spacing} will be correctly applied
 * only to the X-axis. Preset to \c false by default.
 */
void Q3DBars::setMultiSeriesUniform(bool uniform)
{
    graphBars()->setMultiSeriesUniform(uniform);
    emit multiSeriesUniformChanged(uniform);
}

bool Q3DBars::isMultiSeriesUniform() const
{
    return graphBars()->isMultiSeriesUniform();
}

/*!
 * \property Q3DBars::barThickness
 *
 * \brief The bar thickness ratio between the X and Z dimensions.
 *
 * The value \c 1.0 means that the bars are as wide as they are deep, whereas
 *\c 0.5 makes them twice as deep as they are wide. Preset to \c 1.0 by default.
 */
void Q3DBars::setBarThickness(float thicknessRatio)
{
    graphBars()->setBarThickness(thicknessRatio);
    emit barThicknessChanged(thicknessRatio);
}

float Q3DBars::barThickness() const
{
    return graphBars()->barThickness();
}

/*!
 * \property Q3DBars::barSpacing
 *
 * \brief Bar spacing in the X and Z dimensions.
 *
 * Preset to \c {(1.0, 1.0)} by default. Spacing is affected by the
 * barSpacingRelative property.
 *
 * \sa barSpacingRelative, multiSeriesUniform, barSeriesMargin
 */
void Q3DBars::setBarSpacing(const QSizeF &spacing)
{
    graphBars()->setBarSpacing(spacing);
    emit barSpacingChanged(spacing);
}

QSizeF Q3DBars::barSpacing() const
{
    return graphBars()->barSpacing();
}

/*!
 * \property Q3DBars::barSpacingRelative
 *
 * \brief Whether spacing is absolute or relative to bar thickness.
 *
 * If it is \c true, the value of \c 0.0 means that the bars are placed
 * side-to-side, \c 1.0 means that a space as wide as the thickness of one bar
 * is left between the bars, and so on. Preset to \c true.
 */
void Q3DBars::setBarSpacingRelative(bool relative)
{
    graphBars()->setBarSpacingRelative(relative);
    emit barSpacingRelativeChanged(relative);
}

bool Q3DBars::isBarSpacingRelative() const
{
    return graphBars()->isBarSpacingRelative();
}

/*!
 * \property Q3DBars::barSeriesMargin
 *
 * \brief Margin between series columns in X and Z dimensions.
 * Sensible values are on the range [0,1).
 *
 * Preset to \c {(0.0, 0.0)} by default. This property enables
 * showing bars from different series side by side, but with space between
 * columns.
 *
 * \sa barSpacing
 */
void Q3DBars::setBarSeriesMargin(const QSizeF &margin)
{
    graphBars()->setBarSeriesMargin(margin);
    emit barSeriesMarginChanged(margin);
}

QSizeF Q3DBars::barSeriesMargin() const
{
    return graphBars()->barSeriesMargin();
}

/*!
 * \property Q3DBars::rowAxis
 *
 * \brief The axis attached to the active row.
 *
 * Sets the axis of the active row to \a axis. Implicitly calls addAxis() to
 * transfer the ownership of the axis to this graph.
 *
 * If \a axis is null, a temporary default axis with no labels is created.
 * This temporary axis is destroyed if another axis is set explicitly to the
 * same orientation.
 *
 * \sa addAxis(), releaseAxis()
 */
void Q3DBars::setRowAxis(QCategory3DAxis *axis)
{
    graphBars()->setRowAxis(axis);
    emit rowAxisChanged(rowAxis());
}

QCategory3DAxis *Q3DBars::rowAxis() const
{
    return graphBars()->rowAxis();
}

/*!
 * \property Q3DBars::columnAxis
 *
 * \brief The axis attached to the active column.
 *
 * Sets the axis of the active column to \a axis. Implicitly calls addAxis() to
 * transfer the ownership of the axis to this graph.
 *
 * If \a axis is null, a temporary default axis with no labels is created.
 * This temporary axis is destroyed if another axis is set explicitly to the
 * same orientation.
 *
 * \sa addAxis(), releaseAxis()
 */
void Q3DBars::setColumnAxis(QCategory3DAxis *axis)
{
    graphBars()->setColumnAxis(axis);
    emit columnAxisChanged(columnAxis());
}

QCategory3DAxis *Q3DBars::columnAxis() const
{
    return graphBars()->columnAxis();
}

/*!
 * \property Q3DBars::valueAxis
 *
 * Sets the active value axis (the Y-axis) to \a axis. Implicitly calls
 * addAxis() to transfer the ownership of \a axis to this graph.
 *
 * If \a axis is null, a temporary default axis with no labels and
 * an automatically adjusting range is created.
 * This temporary axis is destroyed if another axis is set explicitly to the
 * same orientation.
 *
 * \sa addAxis(), releaseAxis()
 */
void Q3DBars::setValueAxis(QValue3DAxis *axis)
{
    graphBars()->setValueAxis(axis);
    emit valueAxisChanged(valueAxis());
}

QValue3DAxis *Q3DBars::valueAxis() const
{
    return graphBars()->valueAxis();
}

/*!
 * \property Q3DBars::selectedSeries
 *
 * \brief The selected series or a null value.
 *
 * If selectionMode has the \c SelectionMultiSeries flag set, this
 * property holds the series that owns the selected bar.
 */
QBar3DSeries *Q3DBars::selectedSeries() const
{
    return graphBars()->selectedSeries();
}

/*!
 * \property Q3DBars::floorLevel
 *
 * \brief The floor level for the bar graph in Y-axis data coordinates.
 *
 * The actual floor level will be restricted by the Y-axis minimum and maximum
 * values.
 * Defaults to zero.
 */
void Q3DBars::setFloorLevel(float level)
{
    graphBars()->setFloorLevel(level);
    emit floorLevelChanged(level);
}

float Q3DBars::floorLevel() const
{
    return graphBars()->floorLevel();
}

/*!
 * Adds \a axis to the graph. The axes added via addAxis are not yet taken to
 * use, addAxis is simply used to give the ownership of the \a axis to the
 * graph. The \a axis must not be null or added to another graph.
 *
 * \sa releaseAxis(), setValueAxis(), setRowAxis(), setColumnAxis()
 */
void Q3DBars::addAxis(QAbstract3DAxis *axis)
{
    graphBars()->addAxis(axis);
}

/*!
 * Releases the ownership of the \a axis back to the caller, if it is added to
 * this graph. If the released \a axis is in use, a new default axis will be
 * created and set active.
 *
 * If the default axis is released and added back later, it behaves as any other
 * axis would.
 *
 * \sa addAxis(), setValueAxis(), setRowAxis(), setColumnAxis()
 */
void Q3DBars::releaseAxis(QAbstract3DAxis *axis)
{
    graphBars()->releaseAxis(axis);
}

/*!
 * Returns the list of all added axes.
 *
 * \sa addAxis()
 */
QList<QAbstract3DAxis *> Q3DBars::axes() const
{
    return graphBars()->axes();
}

/*!
 * \internal
 */
QQuickGraphsBars *Q3DBars::graphBars()
{
    return static_cast<QQuickGraphsBars *>(m_graphsItem.data());
}

/*!
 * \internal
 */
const QQuickGraphsBars *Q3DBars::graphBars() const
{
    return static_cast<const QQuickGraphsBars *>(m_graphsItem.data());
}

QT_END_NAMESPACE
