// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QBARSERIES_H
#define QBARSERIES_H

//#include <QtCharts/QAbstractBarSeries>
#include <QtGraphs2D/qabstractbarseries.h>
#include <QtGraphs2D/qabstractaxis.h>

QT_BEGIN_NAMESPACE

class QBarSeriesPrivate;

class QBarSeries : public QAbstractBarSeries
{
    Q_OBJECT
    Q_PROPERTY(QAbstractAxis *axisX READ axisX WRITE setAxisX NOTIFY axisXChanged)
    Q_PROPERTY(QAbstractAxis *axisY READ axisY WRITE setAxisY NOTIFY axisYChanged)
    //Q_PROPERTY(QAbstractAxis *axisXTop READ axisXTop WRITE setAxisXTop NOTIFY axisXTopChanged REVISION(1, 2))
    //Q_PROPERTY(QAbstractAxis *axisYRight READ axisYRight WRITE setAxisYRight NOTIFY axisYRightChanged REVISION(1, 2))
    QML_NAMED_ELEMENT(BarSeries)
    //Q_CLASSINFO("DefaultProperty", "children")

public:
    explicit QBarSeries(QObject *parent = nullptr);
    ~QBarSeries();
    QAbstractSeries::SeriesType type() const override;

    QAbstractAxis *axisX();
    void setAxisX(QAbstractAxis *axis);
    QAbstractAxis *axisY();
    void setAxisY(QAbstractAxis *axis);
    /*QAbstractAxis *axisXTop() { return m_axes->axisXTop(); }
    void setAxisXTop(QAbstractAxis *axis) { m_axes->setAxisXTop(axis); }
    QAbstractAxis *axisYRight() { return m_axes->axisYRight(); }
    void setAxisYRight(QAbstractAxis *axis) { m_axes->setAxisYRight(axis); }
*/
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
