// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGraphs/QScatterSeries>
#include <QtGraphs/QValueAxis>
#include <QtQml/QQmlComponent>
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
    void initializeProperties();
    void invalidProperties();

private:
    QScatterSeries *m_series;
};

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

    // Properties from QScatterSeries
    QCOMPARE(m_series->axisX(), nullptr);
    QCOMPARE(m_series->axisY(), nullptr);
    QCOMPARE(m_series->pointMarker(), nullptr);

    // Properties from QXYSeries
    QCOMPARE(m_series->color(), "#ffffff");
    // TODO: QTBUG-121695
    //QCOMPARE(m_series->selectedColor(), "#ff000000");
    QCOMPARE(m_series->markerSize(), 15.0);

    // Properties from QAbstractSeries
    QCOMPARE(m_series->theme(), nullptr);
    QCOMPARE(m_series->name(), "");
    QCOMPARE(m_series->isVisible(), true);
    QCOMPARE(m_series->selectable(), false);
    QCOMPARE(m_series->hoverable(), false);
    QCOMPARE(m_series->opacity(), 1.0);
    QCOMPARE(m_series->valuesMultiplier(), 1.0);
}

void tst_scatter::initializeProperties()
{
    QVERIFY(m_series);

    auto axisX = new QValueAxis(this);
    auto axisY = new QValueAxis(this);
    auto marker = new QQmlComponent(this);
    auto theme = new QSeriesTheme(this);

    m_series->setAxisX(axisX);
    m_series->setAxisY(axisY);
    m_series->setPointMarker(marker);

    m_series->setColor("#ff0000");
    m_series->setSelectedColor("#0000ff");
    m_series->setMarkerSize(5.0);

    m_series->setTheme(theme);
    m_series->setName("LineSeries");
    m_series->setVisible(false);
    m_series->setSelectable(true);
    m_series->setHoverable(true);
    m_series->setOpacity(0.5);
    m_series->setValuesMultiplier(0.5);

    QCOMPARE(m_series->axisX(), axisX);
    QCOMPARE(m_series->axisY(), axisY);
    QCOMPARE(m_series->pointMarker(), marker);

    QCOMPARE(m_series->color(), "#ff0000");
    QCOMPARE(m_series->selectedColor(), "#0000ff");
    QCOMPARE(m_series->markerSize(), 5.0);

    QCOMPARE(m_series->theme(), theme);
    QCOMPARE(m_series->name(), "LineSeries");
    QCOMPARE(m_series->isVisible(), false);
    QCOMPARE(m_series->selectable(), true);
    QCOMPARE(m_series->hoverable(), true);
    QCOMPARE(m_series->opacity(), 0.5);
    QCOMPARE(m_series->valuesMultiplier(), 0.5);
}

void tst_scatter::invalidProperties()
{
    QVERIFY(m_series);

    m_series->setValuesMultiplier(2.0); // range 0...1

    // TODO: QTBUG-121721
    // QCOMPARE(m_series->valuesMultiplier(), 1.0);

    m_series->setValuesMultiplier(-1.0); // range 0...1
    // TODO: QTBUG-121721
    // QCOMPARE(m_series->valuesMultiplier(), 0.0);
}

QTEST_MAIN(tst_scatter)
#include "tst_scatter.moc"