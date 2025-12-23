// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

//  W A R N I N G
//  -------------
//
// This file is not part of the QtGraphs API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

#ifndef QLOGVALUEAXIS_P_H
#define QLOGVALUEAXIS_P_H

#include <QtGraphs/QLogValueAxis>
#include <private/qabstractaxis_p.h>

QT_BEGIN_NAMESPACE

class QLogValueAxisPrivate : public QAbstractAxisPrivate
{
public:
    QLogValueAxisPrivate();
    ~QLogValueAxisPrivate() override;

public:
    qreal min() override { return m_min; }
    qreal max() override { return m_max; }
    void setRange(qreal min,qreal max) override;
    void calculateVisualRange();

protected:
    void setMin(const QVariant &min) override;
    void setMax(const QVariant &max) override;
    void setRange(const QVariant &min, const QVariant &max) override;

private:
    qreal m_min;
    qreal m_max;
    qreal m_base;
    qsizetype m_subTickCount;
    QString m_format;
    int m_precision;
    qreal m_tickAnchor;
    qsizetype m_tickCount;
    qreal m_zoom = 1.0;
    qreal m_pan = 0.0;
    qreal m_visualMin = m_min;
    qreal m_visualMax = m_max;
    Q_DECLARE_PUBLIC(QLogValueAxis)
};

QT_END_NAMESPACE

#endif // QLOGVALUEAXIS_P_H
