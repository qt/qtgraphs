// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
#ifndef QPIEMODELMAPPER_H
#define QPIEMODELMAPPER_H

#include <QtCore/qobject.h>
#include <QtGraphs/qgraphsglobal.h>
#include <QtQmlIntegration/qqmlintegration.h>
Q_MOC_INCLUDE(<QtGraphs / qpieseries.h>)
Q_MOC_INCLUDE(<QtCore / qabstractitemmodel.h>)
QT_BEGIN_NAMESPACE
class QAbstractItemModel;
class QPieModelMapperPrivate;
class QPieSeries;
class Q_GRAPHS_EXPORT QPieModelMapper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QPieSeries *series READ series WRITE setSeries NOTIFY seriesChanged)
    Q_PROPERTY(QAbstractItemModel *model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(
        int valuesSection READ valuesSection WRITE setValuesSection NOTIFY valuesSectionChanged)
    Q_PROPERTY(
        int labelsSection READ labelsSection WRITE setLabelsSection NOTIFY labelsSectionChanged)
    Q_PROPERTY(int first READ first WRITE setFirst NOTIFY firstChanged)
    Q_PROPERTY(int count READ count WRITE setCount NOTIFY countChanged)
    Q_PROPERTY(Qt::Orientation orientation READ orientation WRITE setOrientation NOTIFY
                   orientationChanged FINAL)
    QML_NAMED_ELEMENT(PieModelMapper)

    Q_DECLARE_PRIVATE(QPieModelMapper)
public:
    explicit QPieModelMapper(QObject *parent = nullptr);
    ~QPieModelMapper() override;

    QPieSeries *series() const;
    void setSeries(QPieSeries *series);

    int first() const;
    void setFirst(int first);

    int count() const;
    void setCount(int count);

    int valuesSection() const;
    void setValuesSection(int valuesSection);

    int labelsSection() const;
    void setLabelsSection(int labelsSection);

    Qt::Orientation orientation() const;
    void setOrientation(Qt::Orientation orientation);

    QAbstractItemModel *model() const;
    void setModel(QAbstractItemModel *model);
Q_SIGNALS:
    void seriesChanged();
    void modelChanged();
    void valuesSectionChanged();
    void labelsSectionChanged();
    void firstChanged();
    void countChanged();
    void orientationChanged();

public Q_SLOTS:
    void onSliceLabelChanged();
    void onSliceValueChanged();

protected:
    QPieModelMapper(QPieModelMapperPrivate &dd, QObject *parent = nullptr);
};
QT_END_NAMESPACE
#endif // QPIEMODELMAPPER_H
