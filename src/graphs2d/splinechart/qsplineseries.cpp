// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGraphs/qsplineseries.h>
#include <private/qgraphsview_p.h>
#include <private/qsplineseries_p.h>
#include <private/qxypoint_p.h>

QT_BEGIN_NAMESPACE

QSplineSeries::QSplineSeries(QObject *parent)
    : QXYSeries(*new QSplineSeriesPrivate(this), parent)
{}

void QSplineSeries::componentComplete()
{
    for (auto *child : children()) {
        if (auto point = qobject_cast<QXYPoint *>(child))
            append(point->x(), point->y());
    }
}

QSplineSeries::~QSplineSeries()
{
    Q_D(QSplineSeries);
    if (d->m_graph)
        d->m_graph->removeSeries(this);
}

QAbstractSeries::SeriesType QSplineSeries::type() const
{
    return QAbstractSeries::SeriesTypeSpline;
}

QSplineSeriesPrivate::QSplineSeriesPrivate(QSplineSeries *q)
    : QXYSeriesPrivate(q)
{}

QSplineSeries::QSplineSeries(QSplineSeriesPrivate &d, QObject *parent)
    : QXYSeries(d, parent)
{}

qreal QSplineSeries::width() const
{
    Q_D(const QSplineSeries);
    return d->m_width;
}

void QSplineSeries::setWidth(qreal newWidth)
{
    Q_D(QSplineSeries);
    if (qFuzzyCompare(d->m_width, newWidth))
        return;
    d->m_width = newWidth;
    emit widthChanged();
    emit update();
}

Qt::PenCapStyle QSplineSeries::capStyle() const
{
    Q_D(const QSplineSeries);
    return d->m_capStyle;
}

void QSplineSeries::setCapStyle(Qt::PenCapStyle newCapStyle)
{
    Q_D(QSplineSeries);
    if (d->m_capStyle == newCapStyle)
        return;
    d->m_capStyle = newCapStyle;
    emit capStyleChanged();
    emit update();
}

QT_END_NAMESPACE
