// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QABSTRACTDATAPROXY_H
#define QABSTRACTDATAPROXY_H

#include <QtCore/QObject>
#include <QtCore/QScopedPointer>
#include <QtGraphs/qgraphsglobal.h>

QT_BEGIN_NAMESPACE

class QAbstractDataProxyPrivate;

class Q_GRAPHS_EXPORT QAbstractDataProxy : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QAbstractDataProxy)
    Q_CLASSINFO("RegisterEnumClassesUnscoped", "false")
    Q_PROPERTY(QAbstractDataProxy::DataType type READ type CONSTANT)

public:
    enum class DataType { None = 0, Bar = 1, Scatter = 2, Surface = 4 };
    Q_ENUM(DataType)

protected:
    explicit QAbstractDataProxy(QAbstractDataProxyPrivate *d, QObject *parent = nullptr);

public:
    virtual ~QAbstractDataProxy();

    QAbstractDataProxy::DataType type() const;

protected:
    QScopedPointer<QAbstractDataProxyPrivate> d_ptr;

private:
    Q_DISABLE_COPY(QAbstractDataProxy)

    friend class QAbstract3DSeriesPrivate;
};

QT_END_NAMESPACE

#endif
