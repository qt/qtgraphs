// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
#include "xymappermodel.h"

#include <QtCore/qmath.h>

static constexpr int ColumnCount = 3;
static constexpr int InitialRowCount = 8;

XYMapperModel::XYMapperModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    for (int i = 0; i < InitialRowCount; ++i)
        m_data.append({qreal(i), qSin(i * 0.8), qCos(i * 0.8)});

    connect(this, &QAbstractItemModel::modelReset, this, [this] {
        ++m_modelResetCount;
        Q_EMIT modelResetCountChanged();
    });
}

int XYMapperModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : m_data.size();
}

int XYMapperModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : ColumnCount;
}

QVariant XYMapperModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return {};

    if (orientation == Qt::Vertical)
        return section;

    switch (section) {
    case 0:
        return QStringLiteral("x");
    case 1:
        return QStringLiteral("y");
    default:
        return QStringLiteral("spare");
    }
}

QVariant XYMapperModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || (role != Qt::DisplayRole && role != Qt::EditRole))
        return {};

    return m_data.at(index.row()).at(index.column());
}

bool XYMapperModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::EditRole)
        return false;

    m_data[index.row()][index.column()] = value.toReal();
    Q_EMIT dataChanged(index, index);
    return true;
}

Qt::ItemFlags XYMapperModel::flags(const QModelIndex &index) const
{
    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

bool XYMapperModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if (parent.isValid() || row < 0 || row > m_data.size() || count < 1)
        return false;

    beginInsertRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i)
        m_data.insert(row, QList<qreal>(ColumnCount, 0.0));
    endInsertRows();
    if (m_resetOnStructuralChange)
        reportReset();
    return true;
}

bool XYMapperModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (parent.isValid() || row < 0 || count < 1 || row + count > m_data.size())
        return false;

    beginRemoveRows(parent, row, row + count - 1);
    m_data.remove(row, count);
    endRemoveRows();
    if (m_resetOnStructuralChange)
        reportReset();
    return true;
}

void XYMapperModel::setResetOnStructuralChange(bool enable)
{
    if (m_resetOnStructuralChange == enable)
        return;

    m_resetOnStructuralChange = enable;
    Q_EMIT resetOnStructuralChangeChanged();
}

void XYMapperModel::addRow(qreal x, qreal y)
{
    beginInsertRows({}, m_data.size(), m_data.size());
    m_data.append({x, y, 0.0});
    endInsertRows();
}

void XYMapperModel::removeLastRow()
{
    if (m_data.isEmpty())
        return;

    beginRemoveRows({}, m_data.size() - 1, m_data.size() - 1);
    m_data.removeLast();
    endRemoveRows();
}

void XYMapperModel::editCell(int row, int column, qreal value)
{
    if (row >= 0 && row < m_data.size() && column >= 0 && column < ColumnCount)
        setData(index(row, column), value);
}

void XYMapperModel::setCellSilently(int row, int column, qreal value)
{
    if (row >= 0 && row < m_data.size() && column >= 0 && column < ColumnCount)
        m_data[row][column] = value;
}

void XYMapperModel::resetNow()
{
    reportReset();
}

void XYMapperModel::reportReset()
{
    beginResetModel();
    endResetModel();
}
