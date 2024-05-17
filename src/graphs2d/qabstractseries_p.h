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
#include <private/qobject_p.h>

QT_BEGIN_NAMESPACE

class QAbstractAxis;
class QGraphsView;

class QAbstractSeriesPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QAbstractSeries)
public:
    explicit QAbstractSeriesPrivate();
    ~QAbstractSeriesPrivate() override;

    void setLegendData(const QList<QLegendData> &legendData);
    void clearLegendData();

protected:
    QGraphsView *m_graph;

private:
    QString m_name;
    bool m_visible;
    bool m_loaded = false;
    bool m_selectable = false;
    bool m_hoverable = false;
    qreal m_opacity;
    qreal m_valuesMultiplier;
    QList<QLegendData> m_legendData;
};

QT_END_NAMESPACE

#endif
