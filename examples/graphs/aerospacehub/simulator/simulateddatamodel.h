// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef SIMULATEDDATAMODEL_H
#define SIMULATEDDATAMODEL_H

#include <QAbstractItemModel>
#include <QtQml/qqmlregistration.h>

class SimulatedDataModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString rowRole READ rowRole WRITE setRowRole NOTIFY rowRoleChanged FINAL)
    Q_PROPERTY(QString columnRole READ columnRole WRITE setColumnRole NOTIFY columnRoleChanged FINAL)
    Q_PROPERTY(QString valueRole READ valueRole WRITE setValueRole NOTIFY valueRoleChanged FINAL)

public:
    explicit SimulatedDataModel(QObject *parent = 0);

    void setRowRole(const QString &role);
    QString rowRole() const;
    void setColumnRole(const QString &role);
    QString columnRole() const;
    void setValueRole(const QString &role);
    QString valueRole() const;

    int numberOfRows() const;
    void setNumberOfRows(int numberOfRows);
    int numberOfColumns() const;
    void setNumberOfColumns(int numberOfColumns);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void generateModelData(QVariantList data, int numberOfColumns, int numberOfRows, int numberOfData);

Q_SIGNALS:
    void rowRoleChanged(const QString &role);
    void columnRoleChanged(const QString &role);
    void valueRoleChanged(const QString &role);

private:
    enum ModelRoles {
        RowRole = Qt::UserRole + 1,
        ColumnRole,
        ValueRole,
    };

    struct ModelData {
        QVariant row;
        QVariant column;
        QVariant value;
    };

    int m_numberOfRows = 1;
    int m_numberOfColumns = 1;
    QString m_rowRole;
    QString m_columnRole;
    QString m_valueRole;
    QList<ModelData> m_data;
};

#endif // SIMULATEDDATAMODEL_H
