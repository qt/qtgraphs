// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
// Qt-Security score:significant reason:default


#ifndef QPIESERIES_P_H
#define QPIESERIES_P_H

//  W A R N I N G
//  -------------
//
// This file is not part of the QtGraphs API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

#include <private/qabstractseries_p.h>
#include <QtGraphs/qpieseries.h>

QT_BEGIN_NAMESPACE

class QPieSeriesPrivate : public QAbstractSeriesPrivate
{
public:
    enum class DataInput {
        Unset,
        Declarative,
        Imperative,
    };

    QPieSeriesPrivate();
    ~QPieSeriesPrivate() = default;

    void updateData(bool clearHidden = false);
    void updateLabels();
    void setSizes(qreal innerSize, qreal outerSize);
    void handleSliceDataChanged();
    void handleSliceLabelsChanged(const QStringList &labels);
    void setDataInput(DataInput inputMode) { m_dataInput = inputMode; }
    DataInput dataInput() { return m_dataInput; }
    bool appendImperative(const QList<QPieSlice *> &slices);
    bool appendDeclarative(const QList<QPieSlice *> &slices);
    bool append(const QList<QPieSlice *> &slices);
    void removeMultiple(qsizetype index, int count);

private:
    QList<QPieSlice *> m_slices;
    qreal m_pieRelativeHorPos;
    qreal m_pieRelativeVerPos;
    qreal m_pieRelativeSize;
    qreal m_pieStartAngle;
    qreal m_pieEndAngle;
    qreal m_sum;
    qreal m_holeRelativeSize;
    qreal m_angleSpanVisibleLimit;
    QPieSeries::LabelVisibility m_angleSpanVisibleMode;
    QList<qreal> m_sliceData;
    QStringList m_sliceLabels;
    DataInput m_dataInput;

    Q_DECLARE_PUBLIC(QPieSeries)
};

QT_END_NAMESPACE

#endif // QPIESERIES_P_H
