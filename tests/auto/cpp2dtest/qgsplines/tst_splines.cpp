// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGraphs/QSplineSeries>
#include <QtGraphs/QValueAxis>
#include <QtQml/QQmlComponent>
#include <QtTest/QtTest>

class tst_splines : public QObject
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
    QSplineSeries *m_series;
};

void tst_splines::initTestCase() {}

void tst_splines::cleanupTestCase() {}

void tst_splines::init()
{
    m_series = new QSplineSeries();
}

void tst_splines::cleanup()
{
    delete m_series;
}

void tst_splines::construct()
{
    QSplineSeries *series = new QSplineSeries();
    QVERIFY(series);
    delete series;
}

void tst_splines::initialProperties()
{
    QVERIFY(m_series);

    // Properties from QSplineSeries

    // Properties from QXYSeries
    QCOMPARE(m_series->color(), "#00000000");
    QCOMPARE(m_series->selectedColor(), "#00000000");
    QCOMPARE(m_series->width(), 1.0);
    QCOMPARE(m_series->capStyle(), Qt::PenCapStyle::SquareCap);
    QCOMPARE(m_series->pointDelegate(), nullptr);
    QCOMPARE(m_series->isDraggable(), false);

    // Properties from QAbstractSeries
    QCOMPARE(m_series->name(), "");
    QCOMPARE(m_series->isVisible(), true);
    QCOMPARE(m_series->isSelectable(), false);
    QCOMPARE(m_series->isHoverable(), false);
    QCOMPARE(m_series->opacity(), 1.0);
    QCOMPARE(m_series->valuesMultiplier(), 1.0);
}

void tst_splines::initializeProperties()
{
    QVERIFY(m_series);

    auto marker = new QQmlComponent(this);

    m_series->setWidth(5.0);
    m_series->setCapStyle(Qt::PenCapStyle::RoundCap);
    m_series->setPointDelegate(marker);

    m_series->setColor("#ff0000");
    m_series->setSelectedColor("#0000ff");
    m_series->setDraggable(true);

    m_series->setName("LineSeries");
    m_series->setVisible(false);
    m_series->setSelectable(true);
    m_series->setHoverable(true);
    m_series->setOpacity(0.5);
    m_series->setValuesMultiplier(0.5);

    QCOMPARE(m_series->width(), 5.0);
    QCOMPARE(m_series->capStyle(), Qt::PenCapStyle::RoundCap);
    QCOMPARE(m_series->pointDelegate(), marker);

    QCOMPARE(m_series->color(), "#ff0000");
    QCOMPARE(m_series->selectedColor(), "#0000ff");
    QCOMPARE(m_series->isDraggable(), true);

    QCOMPARE(m_series->name(), "LineSeries");
    QCOMPARE(m_series->isVisible(), false);
    QCOMPARE(m_series->isSelectable(), true);
    QCOMPARE(m_series->isHoverable(), true);
    QCOMPARE(m_series->opacity(), 0.5);
    QCOMPARE(m_series->valuesMultiplier(), 0.5);
}

void tst_splines::invalidProperties()
{
    QVERIFY(m_series);

    m_series->setWidth(-10.0);
    m_series->setValuesMultiplier(2.0); // range 0...1

    QCOMPARE(m_series->width(), 0.0);
    QCOMPARE(m_series->valuesMultiplier(), 1.0);

    m_series->setValuesMultiplier(-1.0); // range 0...1
    QCOMPARE(m_series->valuesMultiplier(), 0.0);
}

#include "tst_splines.moc"
QTEST_MAIN(tst_splines)
