// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtTest/QtTest>

#include <QtGraphs/QScatter3DSeries>
#include <QtGraphs/QScatterDataProxy>

class tst_proxy: public QObject
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

private:
    QScatterDataProxy *m_proxy;
    QScatter3DSeries *m_series;
};

void tst_proxy::initTestCase()
{
}

void tst_proxy::cleanupTestCase()
{
}

void tst_proxy::init()
{
    m_proxy = new QScatterDataProxy();
    m_series = new QScatter3DSeries(m_proxy);
}

void tst_proxy::cleanup()
{
    delete m_proxy;
    m_series = 0;
}

void tst_proxy::construct()
{
    QScatterDataProxy *proxy = new QScatterDataProxy();
    QScatter3DSeries *series = new QScatter3DSeries(proxy);
    QVERIFY(proxy);
    QVERIFY(series);
    delete proxy;
    delete series;
}

void tst_proxy::initialProperties()
{
    QVERIFY(m_proxy);
    QVERIFY(m_proxy->series());

    QCOMPARE(m_proxy->itemCount(), 0);

    QCOMPARE(m_proxy->type(), QAbstractDataProxy::DataType::Scatter);
}

void tst_proxy::initializeProperties()
{
    QVERIFY(m_proxy);
    QVERIFY(m_series);
    QCOMPARE(m_proxy->series(), m_series);

    QScatterDataArray data;
    data << QScatterDataItem(0.5f, 0.5f, 0.5f) << QScatterDataItem(-0.3f, -0.5f, -0.4f);
    m_proxy->addItems(data);

    QCOMPARE(m_proxy->itemCount(), 2);
}

QTEST_MAIN(tst_proxy)
#include "tst_proxy.moc"
