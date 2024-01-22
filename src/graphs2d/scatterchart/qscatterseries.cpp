// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGraphs/qscatterseries.h>
#include <private/qxypoint_p.h>
#include <private/qscatterseries_p.h>
#include <private/qgraphsview_p.h>

QT_BEGIN_NAMESPACE

QScatterSeries::QScatterSeries(QObject *parent)
    : QXYSeries(*new QScatterSeriesPrivate(this), parent)
{

}

QScatterSeries::QScatterSeries(QScatterSeriesPrivate &d, QObject *parent)
    : QXYSeries(d, parent)
{

}

void QScatterSeries::componentComplete()
{
    for (auto *child : children()) {
        if (auto point = qobject_cast<QXYPoint *>(child))
            append(point->x(), point->y());
    }
}

QScatterSeries::~QScatterSeries()
{
    Q_D(QScatterSeries);
    if (d->m_graph)
        d->m_graph->removeSeries(this);
}


QAbstractSeries::SeriesType QScatterSeries::type() const
{
    return QAbstractSeries::SeriesTypeLine;
}

QScatterSeriesPrivate::QScatterSeriesPrivate(QScatterSeries *q)
    : QXYSeriesPrivate(q)
{
}

void QScatterSeriesPrivate::initializeGraphics(QGraphicsItem *parent)
{
    QAbstractSeriesPrivate::initializeGraphics(parent);
}

QAbstractAxis *QScatterSeries::axisX() const
{
    Q_D(const QScatterSeries);
    return d->m_axisX;
}

void QScatterSeries::setAxisX(QAbstractAxis *axis)
{
    Q_D(QScatterSeries);
    detachAxis(d->m_axisX);
    axis->setOrientation(Qt::Horizontal);
    d->m_axisX = axis;
    attachAxis(axis);
    update();
}

QAbstractAxis *QScatterSeries::axisY() const
{
    Q_D(const QScatterSeries);
    return d->m_axisY;
}

void QScatterSeries::setAxisY(QAbstractAxis *axis)
{
    Q_D(QScatterSeries);
    detachAxis(d->m_axisY);
    axis->setOrientation(Qt::Vertical);
    d->m_axisY = axis;
    attachAxis(axis);
    update();
}

QQmlComponent *QScatterSeries::pointMarker() const
{
    Q_D(const QScatterSeries);
    return d->m_marker;
}

void QScatterSeries::setPointMarker(QQmlComponent *newPointMarker)
{
    Q_D(QScatterSeries);
    if (d->m_marker == newPointMarker)
        return;
    d->m_marker = newPointMarker;
    emit pointMarkerChanged();
    update();
}

QT_END_NAMESPACE
