// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

//  W A R N I N G
//  -------------
//
// This file is not part of the QtGraphs API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

#ifndef QBARSERIES_P_H
#define QBARSERIES_P_H

#include <QtGraphs/qbarseries.h>
#include <QtGraphs/qabstractseries.h>
#include <private/qabstractseries_p.h>

QT_BEGIN_NAMESPACE

class QBarSeriesPrivate : public QAbstractSeriesPrivate
{
public:
    QBarSeriesPrivate();
    int categoryCount() const;

    void setBarWidth(qreal width);
    qreal barWidth() const;

    void setVisible(bool visible);
    void setLabelsVisible(bool visible);

    void initializeAxes() override;

    bool append(QBarSet *set);
    bool remove(QBarSet *set);
    bool append(const QList<QBarSet *> &sets);
    bool remove(const QList<QBarSet *> &sets);
    bool insert(int index, QBarSet *set);

    QBarSet *barsetAt(int index);
    qreal min();
    qreal max();
    qreal valueAt(int set, int category);
    qreal percentageAt(int set, int category);
    qreal categorySum(int category);
    qreal absoluteCategorySum(int category);
    qreal maxCategorySum();
    qreal minX();
    qreal maxX();
    qreal categoryTop(int category);
    qreal categoryBottom(int category);
    qreal top();
    qreal bottom();

    bool blockBarUpdate();

    qreal labelsAngle() const;
    void setVisualsDirty(bool dirty) { m_visualsDirty = dirty; }
    bool visualsDirty() const { return m_visualsDirty; }
    void setLabelsDirty(bool dirty) { m_labelsDirty = dirty; }
    bool labelsDirty() const { return m_labelsDirty; }

protected:
    QAbstractAxis *m_axisX = nullptr;
    QAbstractAxis *m_axisY = nullptr;
    QList<QBarSet *> m_barSets;
    qreal m_barWidth;
    bool m_labelsVisible;
    bool m_visible;
    bool m_blockBarUpdate;
    QString m_labelsFormat;
    QBarSeries::BarsType m_barsType = QBarSeries::BarsGroups;
    QBarSeries::BarsOrientation m_barsOrientation = QBarSeries::BarsVertical;
    QBarSeries::LabelsPosition m_labelsPosition;
    qreal m_labelsAngle;
    int m_labelsPrecision;
    bool m_visualsDirty;
    bool m_labelsDirty;
    QQmlComponent *m_barComponent = nullptr;

private:
    Q_DECLARE_PUBLIC(QBarSeries)
};

QT_END_NAMESPACE

#endif // QBARSERIES_P_H
