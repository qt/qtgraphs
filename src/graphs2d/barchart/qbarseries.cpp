// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGraphs/qbarseries.h>
#include <private/qbarseries_p.h>
#include <QtGraphs/QBarCategoryAxis>
#include <private/qgraphsview_p.h>

QT_BEGIN_NAMESPACE

/*!
    \class QBarSeries
    \inmodule QtGraphs
    \ingroup graphs_2D
    \brief The QBarSeries class presents a series of data as vertical bars grouped by category.

    This class draws data as a series of vertical bars grouped by category, with one bar per
    category from each bar set added to the series.

    \sa QBarSet, QAbstractBarSeries
*/
/*!
    \qmltype BarSeries
    \instantiates QBarSeries
    \inqmlmodule QtGraphs
    \ingroup graphs_qml_2D
    \inherits AbstractBarSeries

    \brief Presents a series of data as vertical bars grouped by category.

    The data is drawn as a series of vertical bars grouped by category, with one bar per
    category from each bar set added to the series.
*/

/*!
    Constructs an empty bar series that is a QObject and a child of \a parent.
*/
QBarSeries::QBarSeries(QObject *parent)
    : QAbstractBarSeries(*new QBarSeriesPrivate(this), parent)
{

}

/*!
    Returns the bar series.
*/
QAbstractSeries::SeriesType QBarSeries::type() const
{
    return QAbstractSeries::SeriesTypeBar;
}

/*!
    Removes the bar series from the chart.
*/
QBarSeries::~QBarSeries()
{
    Q_D(QBarSeries);
    if (d->m_graph)
        d->m_graph->removeSeries(this);
}

/*!
    \property QBarSeries::axisX
    \brief X-axis of the series.

    The x-axis used for the series. This should be QBarCategoryAxis.
*/
/*!
    \qmlproperty AbstractAxis BarSeries::axisX
    The x-axis used for the series. This should be BarCategoryAxis.
    \sa axisY
*/
QAbstractAxis *QBarSeries::axisX() {
    Q_D(const QBarSeries);
    return d->m_axisX;
}
void QBarSeries::setAxisX(QAbstractAxis *axis) {
    Q_D(QBarSeries);
    detachAxis(d->m_axisX);
    axis->setOrientation(Qt::Horizontal);
    d->m_axisX = axis;
    attachAxis(axis);
}

/*!
    \property QBarSeries::axisY
    \brief Y-axis of the series.

    The y-axis used for the series. This should be QValueAxis.
*/
/*!
    \qmlproperty AbstractAxis BarSeries::axisY
    The y-axis used for the series. This should be ValueAxis.
    \sa axisX
*/
QAbstractAxis *QBarSeries::axisY() {
    Q_D(const QBarSeries);
    return d->m_axisY;
}
void QBarSeries::setAxisY(QAbstractAxis *axis) {
    Q_D(QBarSeries);
    detachAxis(d->m_axisY);
    axis->setOrientation(Qt::Vertical);
    d->m_axisY = axis;
    attachAxis(axis);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

QBarSeriesPrivate::QBarSeriesPrivate(QBarSeries *q) : QAbstractBarSeriesPrivate(q)
{

}

QT_END_NAMESPACE

#include "moc_qbarseries.cpp"
