// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QLINESERIES_H
#define QLINESERIES_H

#include <QtGraphs2D/qxyseries.h>

QT_BEGIN_NAMESPACE

class QLineSeriesPrivate;

class QLineSeries : public QXYSeries
{
    Q_OBJECT
    Q_PROPERTY(QAbstractAxis *axisX READ axisX WRITE setAxisX NOTIFY axisXChanged)
    Q_PROPERTY(QAbstractAxis *axisY READ axisY WRITE setAxisY NOTIFY axisYChanged)
    Q_PROPERTY(qreal width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(Qt::PenCapStyle capStyle READ capStyle WRITE setCapStyle NOTIFY capStyleChanged)
    Q_PROPERTY(QQmlComponent *pointMarker READ pointMarker WRITE setPointMarker NOTIFY pointMarkerChanged FINAL)

public:
    explicit QLineSeries(QObject *parent = nullptr);
    ~QLineSeries();
    QAbstractSeries::SeriesType type() const override;

    QML_NAMED_ELEMENT(LineSeries)

    QAbstractAxis *axisX() const;
    void setAxisX(QAbstractAxis *axis);

    QAbstractAxis *axisY() const;
    void setAxisY(QAbstractAxis *axis);

    qreal width() const;
    void setWidth(qreal newWidth);

    Qt::PenCapStyle capStyle() const;
    void setCapStyle(const Qt::PenCapStyle &newCapStyle);

    QQmlComponent *pointMarker() const;
    void setPointMarker(QQmlComponent *newPointMarker);

signals:
    void axisXChanged();
    void axisYChanged();
    void widthChanged();
    void capStyleChanged();
    void pointMarkerChanged();

protected:
    QLineSeries(QLineSeriesPrivate &d, QObject *parent = nullptr);

    void componentComplete() override;

private:
    Q_DECLARE_PRIVATE(QLineSeries)
    Q_DISABLE_COPY(QLineSeries)
};

QT_END_NAMESPACE

#endif // QLINESERIES_H
