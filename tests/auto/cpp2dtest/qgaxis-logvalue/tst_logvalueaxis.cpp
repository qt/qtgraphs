// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "qtestcase.h"
#include <QtGraphs/QLogValueAxis>

#include <QtTest/QtTest>

class tst_logvalueaxis : public QObject
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
    QLogValueAxis *m_axis;
};

void tst_logvalueaxis::initTestCase() {}

void tst_logvalueaxis::cleanupTestCase() {}

void tst_logvalueaxis::init()
{
    m_axis = new QLogValueAxis();
}

void tst_logvalueaxis::cleanup()
{
    delete m_axis;
}

void tst_logvalueaxis::construct()
{
    QLogValueAxis *axis = new QLogValueAxis();
    QVERIFY(axis);
    delete axis;
}

void tst_logvalueaxis::initialProperties()
{
    QVERIFY(m_axis);

    QCOMPARE(m_axis->min(), 0);
    QCOMPARE(m_axis->max(), 10);
    QCOMPARE(m_axis->base(), 10.0);
    QCOMPARE(m_axis->labelFormat(), "");
    QCOMPARE(m_axis->labelPostFormat(), "%1");
    QCOMPARE(m_axis->labelPrecision(), 6);
    QCOMPARE(m_axis->subTickCount(), -1);
    QCOMPARE(m_axis->zoom(), 1.0);
    QCOMPARE(m_axis->pan(), 0.0);
}

void tst_logvalueaxis::initializeProperties()
{
    QVERIFY(m_axis);

    QSignalSpy spy0(m_axis, &QLogValueAxis::minChanged);
    QSignalSpy spy1(m_axis, &QLogValueAxis::maxChanged);
    QSignalSpy spy2(m_axis, &QLogValueAxis::labelFormatChanged);
    QSignalSpy spy3(m_axis, &QLogValueAxis::baseChanged);
    QSignalSpy spy4(m_axis, &QLogValueAxis::subTickCountChanged);
    QSignalSpy spy5(m_axis, &QLogValueAxis::labelPostFormatChanged);

    m_axis->setMin(1);
    m_axis->setMax(100);
    m_axis->setLabelFormat("g");
    m_axis->setLabelPostFormat("%1 units");
    m_axis->setBase(2.0);
    m_axis->setSubTickCount(3);
    m_axis->setZoom(2.0);
    m_axis->setPan(1.0);

    QCOMPARE(m_axis->min(), 1);
    QCOMPARE(m_axis->max(), 100);
    QCOMPARE(m_axis->labelFormat(), "g");
    QCOMPARE(m_axis->labelPostFormat(), "%1 units");
    QCOMPARE(m_axis->base(), 2.0);
    QCOMPARE(m_axis->subTickCount(), 3);
    QCOMPARE(m_axis->zoom(), 2.0);
    QCOMPARE(m_axis->pan(), 1.0);

    m_axis->setLabelFormat("%.2f cakes");
    QCOMPARE(m_axis->labelFormat(), "%.2f cakes");

    //Constuct a string same way we do in axisRenderer.
    QByteArray format = m_axis->labelFormat().toLatin1();
    QString formatTest = m_axis->labelPostFormat().arg(QString::asprintf(format.constData(), m_axis->min()));

    QCOMPARE(formatTest, "1.00 cakes units");

    QCOMPARE(spy0.size(), 1);
    QCOMPARE(spy1.size(), 1);
    QCOMPARE(spy2.size(), 2);
    QCOMPARE(spy3.size(), 1);
    QCOMPARE(spy4.size(), 1);
    QCOMPARE(spy5.size(), 1);
}

void tst_logvalueaxis::invalidProperties()
{
    QVERIFY(m_axis);

    m_axis->setBase(-1.0);
    QCOMPARE(m_axis->base(), 10.0);

    m_axis->setBase(1.0);
    QCOMPARE(m_axis->base(), 10.0);
}

QTEST_MAIN(tst_logvalueaxis)
#include "tst_logvalueaxis.moc"
