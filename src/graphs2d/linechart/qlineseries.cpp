// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGraphs2D/qlineseries.h>
#include <private/qxypoint_p.h>
#include <private/qlineseries_p.h>
#include <private/qgraphsview_p.h>

QT_BEGIN_NAMESPACE

QLineSeries::QLineSeries(QObject *parent)
    : QXYSeries(*new QLineSeriesPrivate(this), parent)
{

}

QLineSeries::QLineSeries(QLineSeriesPrivate &d, QObject *parent)
    : QXYSeries(d, parent)
{

}

void QLineSeries::componentComplete()
{
    for (auto *child : children()) {
        if (auto point = qobject_cast<QXYPoint *>(child))
            append(point->x(), point->y());
    }
}

QLineSeries::~QLineSeries()
{
    Q_D(QLineSeries);
    if (d->m_graph)
        d->m_graph->removeSeries(this);
}


QAbstractSeries::SeriesType QLineSeries::type() const
{
    return QAbstractSeries::SeriesTypeLine;
}

QLineSeriesPrivate::QLineSeriesPrivate(QLineSeries *q)
    : QXYSeriesPrivate(q)
{
}

void QLineSeriesPrivate::initializeGraphics(QGraphicsItem *parent)
{
    QAbstractSeriesPrivate::initializeGraphics(parent);
}

QAbstractAxis *QLineSeries::axisX() const
{
    Q_D(const QLineSeries);
    return d->m_axisX;
}

void QLineSeries::setAxisX(QAbstractAxis *axis)
{
    Q_D(QLineSeries);
    detachAxis(d->m_axisX);
    axis->setOrientation(Qt::Horizontal);
    d->m_axisX = axis;
    attachAxis(axis);
}

QAbstractAxis *QLineSeries::axisY() const
{
    Q_D(const QLineSeries);
    return d->m_axisY;
}

void QLineSeries::setAxisY(QAbstractAxis *axis)
{
    Q_D(QLineSeries);
    detachAxis(d->m_axisY);
    axis->setOrientation(Qt::Vertical);
    d->m_axisY = axis;
    attachAxis(axis);
}

qreal QLineSeries::width() const
{
    Q_D(const QLineSeries);
    return d->m_width;
}

void QLineSeries::setWidth(qreal newWidth)
{
    Q_D(QLineSeries);
    if (qFuzzyCompare(d->m_width, newWidth))
        return;
    d->m_width = newWidth;
    emit widthChanged();
}

Qt::PenCapStyle QLineSeries::capStyle() const
{
    Q_D(const QLineSeries);
    return d->m_capStyle;
}

void QLineSeries::setCapStyle(const Qt::PenCapStyle &newCapStyle)
{
    Q_D(QLineSeries);
    if (d->m_capStyle == newCapStyle)
        return;
    d->m_capStyle = newCapStyle;
    emit capStyleChanged();
}

QQmlComponent *QLineSeries::pointMarker() const
{
    Q_D(const QLineSeries);
    return d->m_marker;
}

void QLineSeries::setPointMarker(QQmlComponent *newPointMarker)
{
    Q_D(QLineSeries);
    if (d->m_marker == newPointMarker)
        return;
    d->m_marker = newPointMarker;
    emit pointMarkerChanged();
}

QT_END_NAMESPACE
