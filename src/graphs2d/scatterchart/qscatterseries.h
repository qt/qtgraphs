// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QSCATTERSERIES_H
#define QSCATTERSERIES_H

#include <QtGraphs/qxyseries.h>
#include <QtGraphs/qgraphsglobal.h>

QT_BEGIN_NAMESPACE

class QScatterSeriesPrivate;

class Q_GRAPHS_EXPORT QScatterSeries : public QXYSeries
{
    Q_OBJECT
    Q_PROPERTY(QAbstractAxis *axisX READ axisX WRITE setAxisX NOTIFY axisXChanged)
    Q_PROPERTY(QAbstractAxis *axisY READ axisY WRITE setAxisY NOTIFY axisYChanged)
    Q_PROPERTY(QQmlComponent *pointMarker READ pointMarker WRITE setPointMarker NOTIFY pointMarkerChanged FINAL)

public:
    explicit QScatterSeries(QObject *parent = nullptr);
    ~QScatterSeries();
    QAbstractSeries::SeriesType type() const override;

    QML_NAMED_ELEMENT(ScatterSeries)

    QAbstractAxis *axisX() const;
    void setAxisX(QAbstractAxis *axis);

    QAbstractAxis *axisY() const;
    void setAxisY(QAbstractAxis *axis);

    QQmlComponent *pointMarker() const;
    void setPointMarker(QQmlComponent *newPointMarker);

Q_SIGNALS:
    void axisXChanged();
    void axisYChanged();
    void pointMarkerChanged();

protected:
    QScatterSeries(QScatterSeriesPrivate &d, QObject *parent = nullptr);

    void componentComplete() override;

private:
    Q_DECLARE_PRIVATE(QScatterSeries)
    Q_DISABLE_COPY(QScatterSeries)
};

QT_END_NAMESPACE

#endif // QSCATTERSERIES_H
