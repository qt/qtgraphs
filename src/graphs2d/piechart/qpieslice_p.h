// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QPIESLICE_P_H
#define QPIESLICE_P_H

//  W A R N I N G
//  -------------
//
// This file is not part of the QtGraphs API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

#include <QtGraphs/qpieslice.h>
#include <QtQuick/private/qquickitem_p.h>

QT_BEGIN_NAMESPACE

class PieRenderer;
class QPieSeries;
class QQuickShapePath;
class QQuickPathArc;
class QQuickPathLine;

class QPieSlicePrivate : public QQuickItemPrivate
{
public:
    QPieSlicePrivate();
    ~QPieSlicePrivate();

    static QPieSlicePrivate *fromSlice(QPieSlice *slice);

    void setPercentage(qreal percentage);
    void setStartAngle(qreal angle);
    void setAngleSpan(qreal span);

private:
    friend class QPieSeries;
    friend class PieRenderer;

    QString m_labelText;
    qreal m_value;
    qreal m_percentage;
    qreal m_startAngle;
    qreal m_angleSpan;
    bool m_isExploded;
    qreal m_explodeDistanceFactor;

    QQuickShapePath *m_shapePath;
    QQuickPathArc *m_arc;
    QQuickPathLine *m_lineToCenter;
    QQuickPathLine *m_lineFromCenter;

    QPieSeries *m_series;

    Q_DECLARE_PUBLIC(QPieSlice)
};

QT_END_NAMESPACE

#endif // QPIESLICE_P_H
