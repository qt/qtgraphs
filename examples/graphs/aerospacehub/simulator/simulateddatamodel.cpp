// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "simulateddatamodel.h"

SimulatedDataModel::SimulatedDataModel(QObject *parent)
    : QAbstractListModel(parent)
{ }

int SimulatedDataModel::numberOfRows() const
{
    return m_numberOfRows;
}

void SimulatedDataModel::setNumberOfRows(int numberOfRows)
{
    if (m_numberOfRows == numberOfRows)
        return;

    m_numberOfRows = numberOfRows;
}

int SimulatedDataModel::numberOfColumns() const
{
    return m_numberOfColumns;
}

void SimulatedDataModel::setNumberOfColumns(int numberOfColumns)
{
    if (m_numberOfColumns == numberOfColumns)
        return;

    m_numberOfColumns = numberOfColumns;
}

void SimulatedDataModel::setRowRole(const QString &role)
{
    if (m_rowRole == role)
        return;

    m_rowRole = role;
    emit rowRoleChanged(role);
}

QString SimulatedDataModel::rowRole() const
{
    return m_rowRole;
}

void SimulatedDataModel::setColumnRole(const QString &role)
{
    if (m_columnRole == role)
        return;

    m_columnRole = role;
    emit columnRoleChanged(role);
}

QString SimulatedDataModel::columnRole() const
{
    return m_columnRole;
}

void SimulatedDataModel::setValueRole(const QString &role)
{
    if (m_valueRole == role)
        return;

    m_valueRole = role;
    emit valueRoleChanged(role);
}

QString SimulatedDataModel::valueRole() const
{
    return m_valueRole;
}

int SimulatedDataModel::rowCount(const QModelIndex &parent) const
{
    return m_data.size();
}

QVariant SimulatedDataModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_data.size())
        return QVariant();

    const ModelData &modelData = m_data.at(index.row());

    switch (role) {
        case RowRole:
            return modelData.row;
        case ColumnRole:
            return modelData.column;
        case ValueRole:
            return modelData.value;
        default:
            return QVariant();
    }
}

QHash<int, QByteArray> SimulatedDataModel::roleNames() const
{
    QHash<int, QByteArray> ret;
    ret[RowRole] = m_rowRole.toLatin1();
    ret[ColumnRole] = m_columnRole.toLatin1();
    ret[ValueRole] = m_valueRole.toLatin1();
    return ret;
}

void SimulatedDataModel::generateModelData(QVariantList data, int numberOfColumns, int numberOfRows, int numberOfData)
{
    Q_UNUSED(numberOfColumns)

    beginResetModel();
    m_data.clear();
    for (int i = 0; i < data.size() / (numberOfRows * numberOfData); i++) {
        for (int j = 0; j < numberOfRows; j++) {
            ModelData modelData;
            modelData.row = j + 1;
            modelData.column = i + 1;
            modelData.value = data.at(i * numberOfRows + j);
            m_data.append(modelData);
        }
    }
    endResetModel();
}
