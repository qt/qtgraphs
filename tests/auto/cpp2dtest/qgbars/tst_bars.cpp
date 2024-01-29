// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGraphs/QBarSeries>
#include <QtTest/QtTest>

class tst_bars : public QObject
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
    QBarSeries *m_series;
};

QBarSeries *newSeries()
{
    QBarSeries *series = new QBarSeries();
    return series;
}

void tst_bars::initTestCase() {}

void tst_bars::cleanupTestCase() {}

void tst_bars::init()
{
    m_series = new QBarSeries();
}

void tst_bars::cleanup()
{
    delete m_series;
}

void tst_bars::construct()
{
    QBarSeries *series = new QBarSeries();
    QVERIFY(series);
    delete series;
}

void tst_bars::initialProperties()
{
    QVERIFY(m_series);

    // Properties from QBarSeries
    QCOMPARE(m_series->axisX(), nullptr);
    QCOMPARE(m_series->axisY(), nullptr);

    // Properties from QAbstractBarSeries
    QCOMPARE(m_series->barWidth(), 0.5);
    QCOMPARE(m_series->count(), 0);
    QCOMPARE(m_series->isLabelsVisible(), false);
    QCOMPARE(m_series->labelsFormat(), "");
    QCOMPARE(m_series->labelsPosition(), QAbstractBarSeries::LabelsCenter);
    QCOMPARE(m_series->labelsAngle(), 0);
    QCOMPARE(m_series->labelsPrecision(), 6);
}

QTEST_MAIN(tst_bars)
#include "tst_bars.moc"
