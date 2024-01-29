// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGraphs/QLineSeries>
#include <QtTest/QtTest>

class tst_lines : public QObject
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
    QLineSeries *m_series;
};

QLineSeries *newSeries()
{
    QLineSeries *series = new QLineSeries();
    return series;
}

void tst_lines::initTestCase() {}

void tst_lines::cleanupTestCase() {}

void tst_lines::init()
{
    m_series = new QLineSeries();
}

void tst_lines::cleanup()
{
    delete m_series;
}

void tst_lines::construct()
{
    QLineSeries *series = new QLineSeries();
    QVERIFY(series);
    delete series;
}

void tst_lines::initialProperties()
{
    QVERIFY(m_series);
    QCOMPARE(m_series->axisX(), nullptr);
    QCOMPARE(m_series->axisY(), nullptr);
    QCOMPARE(m_series->width(), 2.0);
    QCOMPARE(m_series->capStyle(), Qt::PenCapStyle::SquareCap);
    QCOMPARE(m_series->pointMarker(), nullptr);
}

QTEST_MAIN(tst_lines)
#include "tst_lines.moc"
