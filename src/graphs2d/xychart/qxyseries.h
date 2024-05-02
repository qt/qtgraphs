// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QXYSERIES_H
#define QXYSERIES_H

#include <QtGraphs/qabstractseries.h>
#include <QtGraphs/qgraphsglobal.h>

QT_BEGIN_NAMESPACE
class QModelIndex;
QT_END_NAMESPACE

QT_BEGIN_NAMESPACE

class QXYSeriesPrivate;
class QXYModelMapper;

class Q_GRAPHS_EXPORT QXYSeries : public QAbstractSeries
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(
        QColor selectedColor READ selectedColor WRITE setSelectedColor NOTIFY selectedColorChanged)
    Q_PROPERTY(QQmlComponent *pointMarker READ pointMarker WRITE setPointMarker NOTIFY pointMarkerChanged FINAL)
    Q_PROPERTY(bool draggable READ draggable WRITE setDraggable NOTIFY draggableChanged)

protected:
    explicit QXYSeries(QXYSeriesPrivate &dd, QObject *parent = nullptr);

public:
    Q_INVOKABLE void append(qreal x, qreal y);
    Q_INVOKABLE void append(const QPointF &point);
    Q_INVOKABLE void append(const QList<QPointF> &points);
    Q_INVOKABLE void replace(qreal oldX, qreal oldY, qreal newX, qreal newY);
    Q_INVOKABLE void replace(const QPointF &oldPoint, const QPointF &newPoint);
    Q_INVOKABLE void replace(int index, qreal newX, qreal newY);
    Q_INVOKABLE void replace(int index, const QPointF &newPoint);
    Q_INVOKABLE void replace(const QList<QPointF> &points);
    Q_INVOKABLE void remove(qreal x, qreal y);
    Q_INVOKABLE void remove(const QPointF &point);
    Q_INVOKABLE void remove(int index);
    Q_INVOKABLE void insert(int index, const QPointF &point);
    Q_INVOKABLE void clear();
    Q_INVOKABLE const QPointF &at(int index) const;
    Q_INVOKABLE int find(const QPointF &) const;
    Q_INVOKABLE void removeMultiple(int index, int count);
    Q_INVOKABLE bool take(const QPointF &point);
    Q_INVOKABLE int count() const;

    QList<QPointF> points() const;

    QXYSeries &operator<<(const QPointF &point);
    QXYSeries &operator<<(const QList<QPointF> &points);

    void setColor(const QColor &newColor);
    QColor color() const;

    void setSelectedColor(const QColor &color);
    QColor selectedColor() const;

    bool isPointSelected(int index);
    void selectPoint(int index);
    void deselectPoint(int index);
    void setPointSelected(int index, bool selected);
    void selectAllPoints();
    void deselectAllPoints();
    void selectPoints(const QList<int> &indexes);
    void deselectPoints(const QList<int> &indexes);
    void toggleSelection(const QList<int> &indexes);
    QList<int> selectedPoints() const;

    QQmlComponent *pointMarker() const;
    void setPointMarker(QQmlComponent *newPointMarker);

    QML_ELEMENT
    QML_UNCREATABLE("XYSeries is an abstract base class.")

    bool draggable() const;
    void setDraggable(bool newDraggable);

Q_SIGNALS:
    void pointReplaced(int index);
    void pointRemoved(int index);
    void pointAdded(int index);
    void colorChanged(QColor color);
    void selectedColorChanged(const QColor &color);
    void pointsReplaced();
    void pointsRemoved(int index, int count);
    void selectedPointsChanged();
    void pointMarkerChanged();
    void draggableChanged();
    void seriesUpdated();

private:
    friend class PointRenderer;
    friend class QGraphPointAnimation;
    friend class QGraphTransition;
    Q_DECLARE_PRIVATE(QXYSeries)
    Q_DISABLE_COPY(QXYSeries)
};

QT_END_NAMESPACE

#endif // QXYSERIES_H
