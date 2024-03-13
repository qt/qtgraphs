// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGraphs/QBarCategoryAxis>
#include <QtGraphs/QValueAxis>
#include <QtGraphs/qbarseries.h>
#include <private/qbarseries_p.h>
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
    : QAbstractBarSeries(*(new QBarSeriesPrivate()), parent)
{}

QBarSeries::QBarSeries(QBarSeriesPrivate &dd, QObject *parent)
    : QAbstractBarSeries(dd, parent)
{}

/*!
    Returns the bar series.
*/
QAbstractSeries::SeriesType QBarSeries::type() const
{
    return QAbstractSeries::SeriesTypeBar;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

QBarSeriesPrivate::QBarSeriesPrivate() {}

QT_END_NAMESPACE

#include "moc_qbarseries.cpp"
