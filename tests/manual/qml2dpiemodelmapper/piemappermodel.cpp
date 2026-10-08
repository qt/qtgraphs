// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
#include "piemappermodel.h"

static constexpr int LabelColumn = 0;
static constexpr int ColumnCount = 3;

PieMapperModel::PieMapperModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    const qreal values[] = {10.0, 7.5, 8.5, 12.0, 5.0, 9.0, 3.0, 6.0};
    int i = 0;
    for (const qreal value : values)
        m_data.append({QStringLiteral("Slice %1").arg(i++), value, 0.0});

    connect(this, &QAbstractItemModel::modelReset, this, [this] {
        ++m_modelResetCount;
        Q_EMIT modelResetCountChanged();
    });
}

int PieMapperModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : m_data.size();
}

int PieMapperModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : ColumnCount;
}

QVariant PieMapperModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return {};

    if (orientation == Qt::Vertical)
        return section;

    switch (section) {
    case 0:
        return QStringLiteral("label");
    case 1:
        return QStringLiteral("value");
    default:
        return QStringLiteral("spare");
    }
}

QVariant PieMapperModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || (role != Qt::DisplayRole && role != Qt::EditRole))
        return {};

    return m_data.at(index.row()).at(index.column());
}

bool PieMapperModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::EditRole)
        return false;

    m_data[index.row()][index.column()] = normalized(index.column(), value);
    Q_EMIT dataChanged(index, index);
    return true;
}

Qt::ItemFlags PieMapperModel::flags(const QModelIndex &index) const
{
    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

bool PieMapperModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if (parent.isValid() || row < 0 || row > m_data.size() || count < 1)
        return false;

    beginInsertRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i)
        m_data.insert(row, {QString(), 0.0, 0.0});
    endInsertRows();
    if (m_resetOnStructuralChange)
        reportReset();
    return true;
}

bool PieMapperModel::removeRows(int row, int count, const QModelIndex &parent)
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

void PieMapperModel::setResetOnStructuralChange(bool enable)
{
    if (m_resetOnStructuralChange == enable)
        return;

    m_resetOnStructuralChange = enable;
    Q_EMIT resetOnStructuralChangeChanged();
}

void PieMapperModel::addRow(const QString &label, qreal value)
{
    beginInsertRows({}, m_data.size(), m_data.size());
    m_data.append({label, value, 0.0});
    endInsertRows();
}

void PieMapperModel::removeLastRow()
{
    if (m_data.isEmpty())
        return;

    beginRemoveRows({}, m_data.size() - 1, m_data.size() - 1);
    m_data.removeLast();
    endRemoveRows();
}

void PieMapperModel::editCell(int row, int column, const QVariant &value)
{
    if (isValidCell(row, column))
        setData(index(row, column), value);
}

void PieMapperModel::setCellSilently(int row, int column, const QVariant &value)
{
    if (isValidCell(row, column))
        m_data[row][column] = normalized(column, value);
}

void PieMapperModel::resetNow()
{
    reportReset();
}

QVariant PieMapperModel::normalized(int column, const QVariant &value)
{
    return column == LabelColumn ? QVariant(value.toString()) : QVariant(value.toReal());
}

bool PieMapperModel::isValidCell(int row, int column) const
{
    return row >= 0 && row < m_data.size() && column >= 0 && column < ColumnCount;
}

void PieMapperModel::reportReset()
{
    beginResetModel();
    endResetModel();
}
