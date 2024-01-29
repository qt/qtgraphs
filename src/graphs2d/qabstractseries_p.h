// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QABSTRACTSERIES_P_H
#define QABSTRACTSERIES_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the QtGraphs API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

#include <QtGraphs/qabstractseries.h>
#include <memory>

QT_BEGIN_NAMESPACE
class QGraphicsItem;
QT_END_NAMESPACE

QT_BEGIN_NAMESPACE

class ChartPresenter;
class ChartElement;
class LegendMarker;
class QLegend;
class ChartDataSet;
class QAbstractAxis;
class QLegendMarker;
class ChartTheme;
class ChartAnimation;
class BoxPlotChartItem;
class QGraphsView;

class QAbstractSeriesPrivate : public QObject
{
    Q_OBJECT
public:
    QAbstractSeriesPrivate(QAbstractSeries *q);
    ~QAbstractSeriesPrivate();

    virtual void initializeAxes() = 0;
    virtual void initializeGraphics(QGraphicsItem* parent) = 0;

Q_SIGNALS:
    void countChanged();

protected:
    QAbstractSeries *q_ptr;
    QGraphsView *m_graph;
    QList<QAbstractAxis*> m_axes;

private:
    QSeriesTheme *m_theme = nullptr;
    QString m_name;
    bool m_visible;
    bool m_selectable = false;
    bool m_hoverable = false;
    qreal m_opacity;
    qreal m_valuesMultiplier;

    friend class QAbstractSeries;
    friend class ChartDataSet;
    friend class ChartPresenter;
    friend class QLegendPrivate;
    friend class BoxPlotChartItem;
};

QT_END_NAMESPACE

#endif
