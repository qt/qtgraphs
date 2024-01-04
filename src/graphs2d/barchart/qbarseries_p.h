// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

//  W A R N I N G
//  -------------
//
// This file is not part of the Qt Chart API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

#ifndef QBARSERIES_P_H
#define QBARSERIES_P_H

#include <private/qabstractbarseries_p.h>

QT_BEGIN_NAMESPACE

class QBarSeries;
class QBarSeriesPrivate : public QAbstractBarSeriesPrivate
{
public:
    QBarSeriesPrivate(QBarSeries *q);
    void initializeGraphics(QGraphicsItem* parent) override;

private:
    Q_DECLARE_PUBLIC(QBarSeries)
    QAbstractAxis *m_axisX = nullptr;
    QAbstractAxis *m_axisY = nullptr;
};

QT_END_NAMESPACE

#endif // QBARSERIES_P_H
