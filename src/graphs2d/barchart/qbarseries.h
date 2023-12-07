// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QBARSERIES_H
#define QBARSERIES_H

#include <QtGraphs2D/qabstractbarseries.h>
#include <QtGraphs2D/qabstractaxis.h>

QT_BEGIN_NAMESPACE

class QBarSeriesPrivate;

class QBarSeries : public QAbstractBarSeries
{
    Q_OBJECT
    Q_PROPERTY(QAbstractAxis *axisX READ axisX WRITE setAxisX NOTIFY axisXChanged)
    Q_PROPERTY(QAbstractAxis *axisY READ axisY WRITE setAxisY NOTIFY axisYChanged)
    QML_NAMED_ELEMENT(BarSeries)

public:
    explicit QBarSeries(QObject *parent = nullptr);
    ~QBarSeries();
    QAbstractSeries::SeriesType type() const override;

    QAbstractAxis *axisX();
    void setAxisX(QAbstractAxis *axis);
    QAbstractAxis *axisY();
    void setAxisY(QAbstractAxis *axis);

Q_SIGNALS:
    void axisXChanged(QAbstractAxis *axis);
    void axisYChanged(QAbstractAxis *axis);
    void axisXTopChanged(QAbstractAxis *axis);
    void axisYRightChanged(QAbstractAxis *axis);

private:
    Q_DECLARE_PRIVATE(QBarSeries)
    Q_DISABLE_COPY(QBarSeries)
};

QT_END_NAMESPACE

#endif // QBARSERIES_H
