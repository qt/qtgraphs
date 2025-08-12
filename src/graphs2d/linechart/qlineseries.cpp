// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "graphs2d/xychart/qxyseries_p.h"
#include <QtGraphs/qlineseries.h>
#include <private/qgraphsview_p.h>
#include <private/qlineseries_p.h>
#include <private/qxypoint_p.h>

QT_BEGIN_NAMESPACE

/*!
    \class QLineSeries
    \inmodule QtGraphs
    \ingroup graphs_2D
    \brief The QLineSeries class presents data in line graphs.

    A line graph is used to show information as a series of data points
    connected by straight lines or steps.
*/
/*!
    \qmltype LineSeries
    \nativetype QLineSeries
    \inqmlmodule QtGraphs
    \ingroup graphs_qml_2D
    \inherits XYSeries

    \brief Presents data in line graphs.

    A line graph is used to show information as a series of data points
    connected by straight lines or steps.

    \image graphs2d-line.png

    Series can contain gaps using NaN values as in :

    \snippet doc_src_qmlgraphs.cpp 14

    or

    \snippet doc_src_qtgraphs.cpp line gaps

    which result in :

    \image graphs2d-line-gaps.png

    \note As seen on the above example's first graph, alone points
    (like the first point of the series) won't be rendered as lines, unless
    there is a visible \l{LineSeries::pointDelegate}{pointDelegate}

    LineSeries uses mostly the same API as ScatterSeries so see ScatterSeries
    documentation for further usage examples.

    \sa ScatterSeries
*/

/*!
    \qmlproperty real LineSeries::width
    The width of the line. By default, the width is 2.0. Widths lower than 0
    are invalid and are automatically set to 0.
*/

/*!
    \qmlproperty Qt::PenCapStyle LineSeries::capStyle
    Controls the cap style of the line. Set to one of \l{Qt::FlatCap}{Qt.FlatCap},
    \l{Qt::SquareCap}{Qt.SquareCap} or \l{Qt::RoundCap}{Qt.RoundCap}. By
    default the cap style is Qt.SquareCap. Invalid values are automatically set
    to the default value.

    \sa Qt::PenCapStyle
*/

/*!
    \qmlproperty Qt::PenJoinStyle LineSeries::joinStyle
    \since 6.11
    Controls the join style of the line. Set to one of \l{Qt::BevelJoin}{Qt.BevelJoin},
    \l{Qt::MiterJoin}{Qt.MiterJoin} or \l{Qt::RoundJoin}{Qt.RoundJoin}. By
    default the join style is Qt.BevelJoin. Invalid values are automatically set
    to the default value.

    \sa Qt::PenJoinStyle
*/

/*!
    \qmlproperty QLineSeries::LineStyle LineSeries::lineStyle
    \since 6.11
    Controls the line style of the line. Set to one of \l{QLineSeries::Straight}{LineSeries.Straight},
    \l{QLineSeries::StepLeft}{LineSeries.StepLeft}, \l{QLineSeries::StepRight}{LineSeries.StepRight}
    or \l{QLineSeries::StepCenter}{LineSeries.StepCenter}. By default the line style is LineSeries.Straight.
    Invalid values are automatically set to the default value.

    \sa QLineSeries::LineStyle
*/

/*!
 * \enum QLineSeries::LineStyle
 * \since 6.11
 *
 * Represents the line style of the series.
 *
 * \value Straight
 *        The points are connected by a straight line.
 * \value StepLeft
 *        The points are connected by steps where each step height is the value from the left.
 * \value StepRight
 *        The points are connected by steps where each step height is the value from the right.
 * \value StepCenter
 *        The points are connected by steps which change in the middle between two values.
 */

/*!
    \qmlproperty Component LineSeries::pointDelegate
    Marks the point with the given QML component.

    \code
        pointDelegate: Image {
            source: "images/happy_box.png"
        }
    \endcode
*/

/*!
    \qmlsignal LineSeries::widthChanged()
    This signal is emitted when the line series width changes.
*/

/*!
    \qmlsignal LineSeries::capStyleChanged()
    This signal is emitted when the line series cap style changes.
*/

/*!
    \qmlsignal LineSeries::joinStyleChanged()
    \since 6.11
    This signal is emitted when the line series cap style changes.
*/

/*!
    \qmlsignal LineSeries::lineStyleChanged()
    \since 6.11
    This signal is emitted when the line series line style changes.
*/

QLineSeries::QLineSeries(QObject *parent)
    : QXYSeries(*(new QLineSeriesPrivate()), parent)
{}

QLineSeries::~QLineSeries() {}

QLineSeries::QLineSeries(QLineSeriesPrivate &dd, QObject *parent)
    : QXYSeries(dd, parent)
{}

void QLineSeries::componentComplete()
{
    Q_D(QLineSeries);

    for (auto *child : children()) {
        if (auto point = qobject_cast<QXYPoint *>(child)) {
            append(point->x(), point->y());
            qCDebug(lcSeries2D, "append points x: %.1f, y: %.1f to lineSeries",
                    point->x(),
                    point->y());
        }
    }

    if (d->m_graphTransition)
        d->m_graphTransition->initialize();

    qCDebug(lcEvents2D) << "QLineSeries::componentComplete.";

    QAbstractSeries::componentComplete();
}

QAbstractSeries::SeriesType QLineSeries::type() const
{
    return QAbstractSeries::SeriesType::Line;
}

QLineSeriesPrivate::QLineSeriesPrivate()
    : QXYSeriesPrivate(QAbstractSeries::SeriesType::Line)
{}

/*!
    \property QLineSeries::width
    \brief the width of the line in the series.

    This property holds the pen width used to draw the series line.
*/
qreal QLineSeries::width() const
{
    Q_D(const QLineSeries);
    return d->m_width;
}

void QLineSeries::setWidth(qreal newWidth)
{
    Q_D(QLineSeries);
    if (newWidth < 0.0)
        newWidth = 0.0;
    if (qFuzzyCompare(d->m_width, newWidth)) {
        qCDebug(lcProperties2D, "QLineSeries::setWidth. Set value width is already %f",
                newWidth);
        return;
    }

    d->m_width = newWidth;
    emit widthChanged();
    emit update();
}

/*!
    \property QLineSeries::capStyle
    \brief the cap style used for line endings.

    This property holds the pen cap style used for rending line ends
    in the series.
 */
Qt::PenCapStyle QLineSeries::capStyle() const
{
    Q_D(const QLineSeries);
    return d->m_capStyle;
}

void QLineSeries::setCapStyle(Qt::PenCapStyle newCapStyle)
{
    Q_D(QLineSeries);
    Qt::PenCapStyle validCapStyle = newCapStyle;
    if (validCapStyle != Qt::PenCapStyle::FlatCap && validCapStyle != Qt::PenCapStyle::SquareCap
        && validCapStyle != Qt::PenCapStyle::RoundCap
        && validCapStyle != Qt::PenCapStyle::MPenCapStyle) {
        validCapStyle = Qt::PenCapStyle::SquareCap;
    }
    if (d->m_capStyle == validCapStyle) {
        qCDebug(lcProperties2D) << "QLineSeries::setCapStyle. CapStyle is already set to:"
                                << newCapStyle;
        return;
    }
    d->m_capStyle = validCapStyle;
    emit capStyleChanged();
    emit update();
}

Qt::PenJoinStyle QLineSeries::joinStyle() const
{
    Q_D(const QLineSeries);
    return d->m_joinStyle;
}

void QLineSeries::setJoinStyle(Qt::PenJoinStyle newJoinStyle)
{
    Q_D(QLineSeries);
    Qt::PenJoinStyle validJoinStyle = newJoinStyle;
    if (validJoinStyle != Qt::PenJoinStyle::BevelJoin && validJoinStyle != Qt::PenJoinStyle::MiterJoin
        && validJoinStyle != Qt::PenJoinStyle::RoundJoin) {
        validJoinStyle = Qt::PenJoinStyle::BevelJoin;
    }
    if (d->m_joinStyle == validJoinStyle) {
        qCDebug(lcProperties2D) << "QLineSeries::setJoinStyle. JoinStyle is already set to:"
                                << newJoinStyle;
        return;
    }
    d->m_joinStyle = validJoinStyle;
    emit joinStyleChanged();
    emit update();
}

QLineSeries::LineStyle QLineSeries::lineStyle() const
{
    Q_D(const QLineSeries);
    return d->m_lineStyle;
}

void QLineSeries::setLineStyle(QLineSeries::LineStyle newLineStyle)
{
    Q_D(QLineSeries);
    QLineSeries::LineStyle validLineStyle = newLineStyle;
    if (validLineStyle != QLineSeries::LineStyle::Straight && validLineStyle != QLineSeries::LineStyle::StepLeft
        && validLineStyle != QLineSeries::LineStyle::StepRight && validLineStyle != QLineSeries::LineStyle::StepCenter) {
        validLineStyle = QLineSeries::LineStyle::Straight;
    }
    if (d->m_lineStyle == validLineStyle) {
        qCDebug(lcProperties2D) << "QLineSeries::setLineStyle. LineStyle is already set to:"
                                << newLineStyle;
        return;
    }
    d->m_lineStyle = validLineStyle;
    emit lineStyleChanged();
    emit update();
}

/*!
    \qmlmethod LineSeries::dataPointCoordinatesAt(real x, real y)
    Returns \a x and \a y rendercoordinates converted into data point
    coordinates.
*/
/*!
    Returns \a x and \a y rendercoordinates converted into data point
    coordinates.

*/
QPointF QLineSeries::dataPointCoordinatesAt(qreal x, qreal y)
{
    Q_D(QLineSeries);

    auto oPoint = d->m_graph->getDataPointCoordinates(this, x, y);
    return oPoint;
}

QT_END_NAMESPACE
