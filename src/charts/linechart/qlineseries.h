// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QLINESERIES_H
#define QLINESERIES_H

//#include <QtCharts/QChartGlobal>
//#if QT_CONFIG(charts_line_chart)
//#include <QtCharts/QXYSeries>
//#include <QtGui/QPen>

#include <QtGraphs2D/qxyseries.h>

QT_BEGIN_NAMESPACE

class QLineSeriesPrivate;

class QLineSeries : public QXYSeries
{
    Q_OBJECT
//    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(QAbstractAxis *axisX READ axisX WRITE setAxisX NOTIFY axisXChanged)
    Q_PROPERTY(QAbstractAxis *axisY READ axisY WRITE setAxisY NOTIFY axisYChanged)
//    Q_PROPERTY(QAbstractAxis *axisXTop READ axisXTop WRITE setAxisXTop NOTIFY axisXTopChanged REVISION(1, 2))
//    Q_PROPERTY(QAbstractAxis *axisYRight READ axisYRight WRITE setAxisYRight NOTIFY axisYRightChanged REVISION(1, 2))
//    Q_PROPERTY(QAbstractAxis *axisAngular READ axisAngular WRITE setAxisAngular NOTIFY axisAngularChanged REVISION(1, 3))
//    Q_PROPERTY(QAbstractAxis *axisRadial READ axisRadial WRITE setAxisRadial NOTIFY axisRadialChanged REVISION(1, 3))
    Q_PROPERTY(qreal width READ width WRITE setWidth NOTIFY widthChanged)
//    Q_PROPERTY(Qt::PenStyle style READ style WRITE setStyle NOTIFY styleChanged)
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
    friend class LineChartItem;
};

QT_END_NAMESPACE

//#endif // QT_CONFIG(charts_line_chart)
#endif // QLINESERIES_H
