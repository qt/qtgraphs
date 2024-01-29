// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGraphs/QScatterSeries>
#include <QtTest/QtTest>

class tst_scatter : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void construct();

    void initialProperties();
    // TODO: QTBUG-121657
    // void initializeProperties();
    // void invalidProperties();

private:
    QScatterSeries *m_series;
};

QScatterSeries *newSeries()
{
    QScatterSeries *series = new QScatterSeries();
    return series;
}

void tst_scatter::initTestCase() {}

void tst_scatter::cleanupTestCase() {}

void tst_scatter::init()
{
    m_series = new QScatterSeries();
}

void tst_scatter::cleanup()
{
    delete m_series;
}

void tst_scatter::construct()
{
    QScatterSeries *series = new QScatterSeries();
    QVERIFY(series);
    delete series;
}

void tst_scatter::initialProperties()
{
    QVERIFY(m_series);
    QCOMPARE(m_series->axisX(), nullptr);
    QCOMPARE(m_series->axisY(), nullptr);
    QCOMPARE(m_series->pointMarker(), nullptr);
}

QTEST_MAIN(tst_scatter)
#include "tst_scatter.moc"
