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

#include <QtCore/private/qobject_p.h>
#include <QtGui/QColor>
#include <QtGui/QFont>
#include <QtGraphs/qpieslice.h>

QT_BEGIN_NAMESPACE

class PieRenderer;
class QColor;
class QFont;
class QPieSeries;
class QQuickPathArc;
class QQuickPathLine;
class QQuickShape;
class QQuickShapePath;
class QQuickText;

class QPieSlicePrivate : public QObjectPrivate
{
public:
    QPieSlicePrivate();
    ~QPieSlicePrivate();

    void setPercentage(qreal percentage);
    void setStartAngle(qreal angle);
    void setAngleSpan(qreal span);

private:
    friend class QPieSeries;
    friend class QPieSeriesPrivate;
    friend class PieRenderer;

    void setLabelVisible(bool visible);
    void setLabelPosition(QPieSlice::LabelPosition position);

    QString m_labelText;
    bool m_isLabelVisible;
    QPieSlice::LabelPosition m_labelPosition;
    QColor m_labelColor;
    QFont m_labelFont;
    qreal m_labelArmLengthFactor;
    qreal m_value;
    qreal m_percentage;
    qreal m_startAngle;
    qreal m_angleSpan;
    bool m_isExploded;
    qreal m_explodeDistanceFactor;

    bool m_labelDirty;

    QQuickShapePath *m_shapePath;
    QQuickPathArc *m_arc;
    QQuickPathLine *m_lineToCenter;
    QQuickPathLine *m_lineFromCenter;

    QQuickText *m_labelItem;
    QQuickShape *m_labelShape;
    QQuickShapePath *m_labelPath;
    QQuickPathLine *m_labelArm;
    QQuickPathLine *m_labelUnderline;

    QPieSeries *m_series;

    Q_DECLARE_PUBLIC(QPieSlice)
};

QT_END_NAMESPACE

#endif // QPIESLICE_P_H
