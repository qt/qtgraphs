// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

//  W A R N I N G
//  -------------
//
// This file is not part of the QtGraphs API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
#ifndef QPIEMODELMAPPER_P_H
#define QPIEMODELMAPPER_P_H
#include <QtGraphs/qpiemodelmapper.h>
#include <private/qobject_p.h>

QT_BEGIN_NAMESPACE
class QPieSlice;
class QPieSeries;
class QAbstractItemModel;

class QPieModelMapperPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QPieModelMapper)

public:
    explicit QPieModelMapperPrivate();
    ~QPieModelMapperPrivate() override;

    void onSliceLabelChanged(const QPieSlice *slice);
    void onSliceValueChanged(const QPieSlice *slice);
public Q_SLOTS:
    // for the model
    void onModelUpdated(QModelIndex topLeft, QModelIndex bottomRight);
    void onModelRowsAdded(QModelIndex parent, int start, int end);
    void onModelRowsRemoved(QModelIndex parent, int start, int end);
    void onModelColumnsAdded(QModelIndex parent, int start, int end);
    void onModelColumnsRemoved(QModelIndex parent, int start, int end);
    void handleModelDestroyed();

    // for the series
    void onSlicesAdded(const QList<QPieSlice *> &slices);
    void onSlicesRemoved(const QList<QPieSlice *> &slices);
    void handleSeriesDestroyed();

    void initializePieFromModel();

private:
    QPieSlice *pieSlice(QModelIndex index) const;
    bool isLabelIndex(QModelIndex index) const;
    bool isValueIndex(QModelIndex index) const;
    QModelIndex valueModelIndex(int slicePos);
    QModelIndex labelModelIndex(int slicePos);
    void insertData(int start, int end);
    void removeData(int start, int end);

    void blockModelSignals(bool block = true);
    void blockSeriesSignals(bool block = true);

private:
    QPieSeries *m_series = nullptr;
    QList<QPieSlice *> m_slices;
    QAbstractItemModel *m_model = nullptr;
    int m_first = 0;
    int m_count = -1;
    Qt::Orientation m_orientation = Qt::Vertical;
    int m_valuesSection = -1;
    int m_labelsSection = -1;
    bool m_seriesSignalsBlock = false;
    bool m_modelSignalsBlock = false;
};

QT_END_NAMESPACE
#endif // QPIEMODELMAPPER_P_H