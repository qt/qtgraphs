// Copyright (C) 2024 The Qt Company Ltd.

// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
#ifndef QXYMODELMAPPER_H
#define QXYMODELMAPPER_H
#include <QtCore/qobject.h>
#include <QtGraphs/qgraphsglobal.h>
#include <QtQmlIntegration/qqmlintegration.h>

Q_MOC_INCLUDE(<QtGraphs / qxyseries.h>)
Q_MOC_INCLUDE(<QtCore / qabstractitemmodel.h>)
QT_BEGIN_NAMESPACE

class QXYModelMapperPrivate;
class QXYSeries;
class QAbstractItemModel;
class Q_GRAPHS_EXPORT QXYModelMapper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QXYSeries *series READ series WRITE setSeries NOTIFY seriesChanged)
    Q_PROPERTY(QAbstractItemModel *model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(int xSection READ xSection WRITE setXSection NOTIFY xSectionChanged)
    Q_PROPERTY(int ySection READ ySection WRITE setYSection NOTIFY ySectionChanged)
    Q_PROPERTY(int first READ first WRITE setFirst NOTIFY firstChanged)
    Q_PROPERTY(int count READ count WRITE setCount NOTIFY countChanged)
    Q_PROPERTY(Qt::Orientation orientation READ orientation WRITE setOrientation NOTIFY
                   orientationChanged FINAL)
    QML_NAMED_ELEMENT(XYModelMapper)
    Q_DECLARE_PRIVATE(QXYModelMapper)
public:
    explicit QXYModelMapper(QObject *parent = nullptr);
    ~QXYModelMapper() override;

    QAbstractItemModel *model() const;
    void setModel(QAbstractItemModel *model);

    QXYSeries *series() const;
    void setSeries(QXYSeries *series);

    int first() const;
    void setFirst(int first);

    int count() const;
    void setCount(int count);

    Qt::Orientation orientation() const;
    void setOrientation(Qt::Orientation orientation);

    int xSection() const;
    void setXSection(int xSection);

    int ySection() const;
    void setYSection(int ySection);
Q_SIGNALS:
    void seriesChanged();
    void modelChanged();
    void xSectionChanged();
    void ySectionChanged();
    void firstChanged();
    void countChanged();
    void orientationChanged();

protected:
    QXYModelMapper(QXYModelMapperPrivate &dd, QObject *parent = nullptr);
};
QT_END_NAMESPACE
#endif // QXYMODELMAPPER_H
