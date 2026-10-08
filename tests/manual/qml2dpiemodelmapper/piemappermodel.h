// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
#ifndef PIEMAPPERMODEL_H
#define PIEMAPPERMODEL_H

#include <QtCore/qabstractitemmodel.h>
#include <QtCore/qlist.h>
#include <QtCore/qvariant.h>
#include <QtQmlIntegration/qqmlintegration.h>

// Table model (label, value, spare columns) for exercising QPieModelMapper.
//
// With resetOnStructuralChange enabled, insertRows()/removeRows() additionally report a model
// reset, like some custom and QML models do. The mapper calls these when the series is edited, so
// this is the case where it must not rebuild the whole series.
class PieMapperModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(bool resetOnStructuralChange READ resetOnStructuralChange WRITE
                   setResetOnStructuralChange NOTIFY resetOnStructuralChangeChanged FINAL)
    Q_PROPERTY(int modelResetCount READ modelResetCount NOTIFY modelResetCountChanged FINAL)
    QML_ELEMENT

public:
    explicit PieMapperModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = {}) const override;
    int columnCount(const QModelIndex &parent = {}) const override;
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool insertRows(int row, int count, const QModelIndex &parent = {}) override;
    bool removeRows(int row, int count, const QModelIndex &parent = {}) override;

    bool resetOnStructuralChange() const { return m_resetOnStructuralChange; }
    void setResetOnStructuralChange(bool enable);
    int modelResetCount() const { return m_modelResetCount; }

    // Edits made "from outside", i.e. not on behalf of the mapper. These never emit a reset.
    Q_INVOKABLE void addRow(const QString &label, qreal value);
    Q_INVOKABLE void removeLastRow();
    Q_INVOKABLE void editCell(int row, int column, const QVariant &value);
    // Changes data without telling anybody. Use together with resetNow().
    Q_INVOKABLE void setCellSilently(int row, int column, const QVariant &value);
    Q_INVOKABLE void resetNow();

Q_SIGNALS:
    void resetOnStructuralChangeChanged();
    void modelResetCountChanged();

private:
    using Row = QList<QVariant>;

    static QVariant normalized(int column, const QVariant &value);
    bool isValidCell(int row, int column) const;
    void reportReset();

    QList<Row> m_data;
    int m_modelResetCount = 0;
    bool m_resetOnStructuralChange = true;
};

#endif // PIEMAPPERMODEL_H
