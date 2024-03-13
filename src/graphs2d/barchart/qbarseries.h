// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QBARSERIES_H
#define QBARSERIES_H

#include <QtGraphs/qabstractbarseries.h>
#include <QtGraphs/qabstractaxis.h>
#include <QtGraphs/qgraphsglobal.h>

QT_BEGIN_NAMESPACE

class QBarSeriesPrivate;

class Q_GRAPHS_EXPORT QBarSeries : public QAbstractBarSeries
{
    Q_OBJECT
    QML_NAMED_ELEMENT(BarSeries)

public:
    explicit QBarSeries(QObject *parent = nullptr);
    QAbstractSeries::SeriesType type() const override;

protected:
    QBarSeries(QBarSeriesPrivate &dd, QObject *parent = nullptr);

private:
    Q_DECLARE_PRIVATE(QBarSeries)
    Q_DISABLE_COPY(QBarSeries)

    friend class BarsRenderer;
};

QT_END_NAMESPACE

#endif // QBARSERIES_H
