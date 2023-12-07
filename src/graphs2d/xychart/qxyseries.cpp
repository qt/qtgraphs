// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#undef QT_NO_FOREACH // this file contains unported legacy Q_FOREACH uses

#include "qxyseries.h"
#include <private/qxyseries_p.h>
#include "charthelpers_p.h"

QT_BEGIN_NAMESPACE

QXYSeries::QXYSeries(QXYSeriesPrivate &d, QObject *parent)
    : QAbstractSeries(d, parent)
{
}

QXYSeries::~QXYSeries()
{
}

void QXYSeries::append(qreal x, qreal y)
{
    append(QPointF(x, y));
}

void QXYSeries::append(const QPointF &point)
{
    Q_D(QXYSeries);

    if (isValidValue(point)) {
        d->m_points << point;
        emit pointAdded(d->m_points.size() - 1);
    }
}

void QXYSeries::append(const QList<QPointF> &points)
{
    foreach (const QPointF &point , points)
        append(point);
}

void QXYSeries::replace(qreal oldX, qreal oldY, qreal newX, qreal newY)
{
    replace(QPointF(oldX, oldY), QPointF(newX, newY));
}

void QXYSeries::replace(const QPointF &oldPoint, const QPointF &newPoint)
{
    Q_D(QXYSeries);
    int index = d->m_points.indexOf(oldPoint);
    if (index == -1)
        return;
    replace(index, newPoint);
}

void QXYSeries::replace(int index, qreal newX, qreal newY)
{
    replace(index, QPointF(newX, newY));
}

void QXYSeries::replace(int index, const QPointF &newPoint)
{
    Q_D(QXYSeries);
    if (isValidValue(newPoint)) {
        d->m_points[index] = newPoint;
        emit pointReplaced(index);
    }
}

void QXYSeries::replace(const QList<QPointF> &points)
{
    Q_D(QXYSeries);
    d->m_points = points;
    emit pointsReplaced();
}

bool QXYSeries::isPointSelected(int index)
{
    Q_D(QXYSeries);
    return d->isPointSelected(index);
}

void QXYSeries::selectPoint(int index)
{
    setPointSelected(index, true);
}

void QXYSeries::deselectPoint(int index)
{
    setPointSelected(index, false);
}

void QXYSeries::setPointSelected(int index, bool selected)
{
    Q_D(QXYSeries);

    bool callSignal = false;
    d->setPointSelected(index, selected, callSignal);

    if (callSignal)
        emit selectedPointsChanged();
}

void QXYSeries::selectAllPoints()
{
    Q_D(QXYSeries);

    bool callSignal = false;
    for (int i = 0; i < d->m_points.size(); ++i)
        d->setPointSelected(i, true, callSignal);

    if (callSignal)
        emit selectedPointsChanged();
}

void QXYSeries::deselectAllPoints()
{
    Q_D(QXYSeries);

    bool callSignal = false;
    for (int i = 0; i < d->m_points.size(); ++i)
        d->setPointSelected(i, false, callSignal);

    if (callSignal)
        emit selectedPointsChanged();
}

void QXYSeries::selectPoints(const QList<int> &indexes)
{
    Q_D(QXYSeries);

    bool callSignal = false;
    for (const int &index : indexes)
        d->setPointSelected(index, true, callSignal);

    if (callSignal)
        emit selectedPointsChanged();
}

void QXYSeries::deselectPoints(const QList<int> &indexes)
{
    Q_D(QXYSeries);

    bool callSignal = false;
    for (const int &index : indexes)
        d->setPointSelected(index, false, callSignal);

    if (callSignal)
        emit selectedPointsChanged();
}

void QXYSeries::toggleSelection(const QList<int> &indexes)
{
    Q_D(QXYSeries);

    bool callSignal = false;
    for (const int &index : indexes)
        d->setPointSelected(index, !isPointSelected(index), callSignal);

    if (callSignal)
        emit selectedPointsChanged();
}

QList<int> QXYSeries::selectedPoints() const
{
    Q_D(const QXYSeries);
    return QList<int>(d->m_selectedPoints.begin(), d->m_selectedPoints.end());
}

void QXYSeries::remove(qreal x, qreal y)
{
    remove(QPointF(x, y));
}

void QXYSeries::remove(const QPointF &point)
{
    Q_D(QXYSeries);
    int index = d->m_points.indexOf(point);
    if (index == -1)
        return;
    remove(index);
}

void QXYSeries::remove(int index)
{
    Q_D(QXYSeries);
    d->m_points.remove(index);

    bool callSignal = false;
    d->setPointSelected(index, false, callSignal);

    emit pointRemoved(index);
    if (callSignal)
        emit selectedPointsChanged();
}

void QXYSeries::removePoints(int index, int count)
{
    // This function doesn't overload remove as there is chance for it to get mixed up with
    // remove(qreal, qreal) overload in some implicit casting cases.
    Q_D(QXYSeries);
    if (count > 0) {
        d->m_points.remove(index, count);

        bool callSignal = false;
        if (!d->m_selectedPoints.empty()) {
            QSet<int> selectedAfterRemoving;

            for (const int &selectedPointIndex : std::as_const(d->m_selectedPoints)) {
                if (selectedPointIndex < index) {
                    selectedAfterRemoving << selectedPointIndex;
                } else if (selectedPointIndex >= index + count) {
                    selectedAfterRemoving << selectedPointIndex - count;
                    callSignal = true;
                } else {
                    callSignal = true;
                }
            }

            d->m_selectedPoints = selectedAfterRemoving;
        }

        emit pointsRemoved(index, count);
        if (callSignal)
            emit selectedPointsChanged();
    }
}

void QXYSeries::insert(int index, const QPointF &point)
{
    Q_D(QXYSeries);
    if (isValidValue(point)) {
        index = qMax(0, qMin(index, d->m_points.size()));

        d->m_points.insert(index, point);

        bool callSignal = false;
        if (!d->m_selectedPoints.isEmpty()) {
            // if point was inserted we need to move already selected points by 1
            QSet<int> selectedAfterInsert;
            for (const auto &value : std::as_const(d->m_selectedPoints)) {
                if (value >= index) {
                    selectedAfterInsert << value + 1;
                    callSignal = true;
                } else {
                    selectedAfterInsert << value;
                }
            }
            d->m_selectedPoints = selectedAfterInsert;
        }

        emit pointAdded(index);
        if (callSignal)
            emit selectedPointsChanged();
    }
}

void QXYSeries::clear()
{
    Q_D(QXYSeries);
    removePoints(0, d->m_points.size());
}

QList<QPointF> QXYSeries::points() const
{
    Q_D(const QXYSeries);
    return d->m_points;
}

const QPointF &QXYSeries::at(int index) const
{
    Q_D(const QXYSeries);
    return d->m_points.at(index);
}

int QXYSeries::count() const
{
    Q_D(const QXYSeries);
    return d->m_points.size();
}

void QXYSeries::setColor(const QColor &newColor)
{
    Q_D(QXYSeries);
    if (color() != newColor) {
        d->m_color = newColor;
        emit colorChanged(newColor);
    }
}

QColor QXYSeries::color() const
{
    Q_D(const QXYSeries);
    return d->m_color;
}

void QXYSeries::setSelectedColor(const QColor &color)
{
    Q_D(QXYSeries);
    if (selectedColor() != color) {
        d->m_selectedColor = color;
        emit selectedColorChanged(color);
    }
}

QColor QXYSeries::selectedColor() const
{
    Q_D(const QXYSeries);
    return d->m_selectedColor;
}

void QXYSeries::setMarkerSize(qreal size)
{
    Q_D(QXYSeries);

    if (!qFuzzyCompare(d->m_markerSize, size)) {
        d->m_markerSizeDefault = false;
        d->setMarkerSize(size);
        emit d->seriesUpdated();
        emit markerSizeChanged(size);
    }
}

qreal QXYSeries::markerSize() const
{
    Q_D(const QXYSeries);
    return d->m_markerSize;
}

QXYSeries &QXYSeries::operator<< (const QPointF &point)
{
    append(point);
    return *this;
}

QXYSeries &QXYSeries::operator<< (const QList<QPointF>& points)
{
    append(points);
    return *this;
}

QXYSeriesPrivate::QXYSeriesPrivate(QXYSeries *q)
    : QAbstractSeriesPrivate(q),
      m_markerSize(15.0)
{
}

void QXYSeriesPrivate::initializeDomain()
{
    qreal minX(0);
    qreal minY(0);
    qreal maxX(1);
    qreal maxY(1);

    Q_Q(QXYSeries);

    const QList<QPointF> &points = q->points();

    if (!points.isEmpty()) {
        minX = points[0].x();
        minY = points[0].y();
        maxX = minX;
        maxY = minY;

        for (int i = 0; i < points.size(); i++) {
            qreal x = points[i].x();
            qreal y = points[i].y();
            minX = qMin(minX, x);
            minY = qMin(minY, y);
            maxX = qMax(maxX, x);
            maxY = qMax(maxY, y);
        }
    }

    domain()->setRange(minX, maxX, minY, maxY);
}

void QXYSeriesPrivate::initializeAxes()
{
}

void QXYSeriesPrivate::setPointSelected(int index, bool selected, bool &callSignal)
{
    if (index < 0 || index > m_points.size() - 1)
        return;

    if (selected) {
        if (!isPointSelected(index)) {
            m_selectedPoints << index;
            callSignal = true;
        }
    } else {
        if (isPointSelected(index)) {
            m_selectedPoints.remove(index);
            callSignal = true;
        }
    }
}

bool QXYSeriesPrivate::isPointSelected(int index)
{
    return m_selectedPoints.contains(index);
}

bool QXYSeriesPrivate::isMarkerSizeDefault()
{
    return m_markerSizeDefault;
}

void QXYSeriesPrivate::setMarkerSize(qreal markerSize)
{
    m_markerSize = markerSize;
}

QT_END_NAMESPACE
