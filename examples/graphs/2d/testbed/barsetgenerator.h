// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause
#ifndef BARSETGENERATOR_H
#define BARSETGENERATOR_H

#include <QObject>
#include <qbarset.h>
#include <qdebug.h>
#include <qqmlintegration.h>

class BarSetGenerator : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit BarSetGenerator(QObject *parent = nullptr);
    Q_INVOKABLE QBarSet *createNewBarSet();
};

#endif // BARSETGENERATOR_H
