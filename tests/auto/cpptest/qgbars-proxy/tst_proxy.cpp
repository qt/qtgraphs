// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtTest/QtTest>

#include <QtGraphs/QBar3DSeries>
#include <QtGraphs/QBarDataProxy>

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
    QBarDataProxy *m_proxy;
    QBar3DSeries *m_series;
};

void tst_proxy::initTestCase()
{
}

void tst_proxy::cleanupTestCase()
{
}

void tst_proxy::init()
{
    m_proxy = new QBarDataProxy();
    m_series = new QBar3DSeries(m_proxy);
}

void tst_proxy::cleanup()
{
    delete m_series;
    m_proxy = 0;
}

void tst_proxy::construct()
{
    QBarDataProxy *proxy = new QBarDataProxy();
    QVERIFY(proxy);
    QBar3DSeries *series = new QBar3DSeries(proxy);
    QVERIFY(series);
    QCOMPARE(proxy->series(), series);
    delete series;
    proxy = 0;
}

void tst_proxy::initialProperties()
{
    QVERIFY(m_proxy);
    QVERIFY(m_series);

    QCOMPARE(m_proxy->series()->columnLabels().size(), 0);
    QCOMPARE(m_proxy->rowCount(), 0);
    QCOMPARE(m_proxy->series()->rowLabels().size(), 0);

    QCOMPARE(m_proxy->type(), QAbstractDataProxy::DataType::Bar);
}

void tst_proxy::initializeProperties()
{
    QVERIFY(m_proxy);

    m_proxy->series()->setColumnLabels(QStringList() << "1"
                                                     << "2"
                                                     << "3");
    QBarDataRow data;
    data << QBarDataItem(1.0f) << QBarDataItem(3.0f) << QBarDataItem(7.5f);
    m_proxy->addRow(data);
    m_proxy->series()->setRowLabels(QStringList() << "1");

    QCOMPARE(m_proxy->series()->columnLabels().size(), 3);
    QCOMPARE(m_proxy->rowCount(), 1);
    QCOMPARE(m_proxy->series()->rowLabels().size(), 1);
}

QTEST_MAIN(tst_proxy)
#include "tst_proxy.moc"
