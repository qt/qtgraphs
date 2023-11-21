// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef DATAGENERATOR_H
#define DATAGENERATOR_H

#include <QtGraphs/QScatter3DSeries>
#include <QtCore/QFile>

class DataGenerator  : public QObject
{
    Q_OBJECT
public:
    DataGenerator(QObject *parent = 0);
    ~DataGenerator() override;

public Q_SLOTS:
    void generateData(QScatter3DSeries *series, uint count);
    void add(QScatter3DSeries *series, uint count);
    void writeLine(int itemCount, float fps);

private:
    QScatter3DSeries m_series;
    QFile *m_file;
};

#endif // DATAGENERATOR_H
