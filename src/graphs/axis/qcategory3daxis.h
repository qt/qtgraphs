// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QCATEGORY3DAXIS_H
#define QCATEGORY3DAXIS_H

#include <QtGraphs/qabstract3daxis.h>

QT_BEGIN_NAMESPACE

class QCategory3DAxisPrivate;

class Q_GRAPHS_EXPORT QCategory3DAxis : public QAbstract3DAxis
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QCategory3DAxis)
    Q_PROPERTY(QStringList labels READ labels WRITE setLabels NOTIFY labelsChanged)

public:
    explicit QCategory3DAxis(QObject *parent = nullptr);
    virtual ~QCategory3DAxis();

    void setLabels(const QStringList &labels);
    QStringList labels() const;

private:
    Q_DISABLE_COPY(QCategory3DAxis)

    friend class QQuickGraphsBars;
    friend class QQuickGraphsItem;
};

QT_END_NAMESPACE

#endif
