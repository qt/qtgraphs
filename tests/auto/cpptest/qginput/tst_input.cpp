// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtTest/QtTest>

#include <QtGraphs/Q3DBars>

class tst_input: public QObject
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
    Q3DBars *m_input;
};

void tst_input::initTestCase()
{
}

void tst_input::cleanupTestCase()
{
}

void tst_input::init()
{
    m_input = new Q3DBars();
}

void tst_input::cleanup()
{
    delete m_input;
}

void tst_input::construct()
{
    Q3DBars *input = new Q3DBars();
    QVERIFY(input);
    delete input;
}

void tst_input::initialProperties()
{
    QVERIFY(m_input);

    QCOMPARE(m_input->zoomAtTargetEnabled(), true);
    QCOMPARE(m_input->zoomEnabled(), true);
    QCOMPARE(m_input->rotationEnabled(), true);
    QCOMPARE(m_input->selectionEnabled(), true);
}

void tst_input::initializeProperties()
{
    QVERIFY(m_input);

    m_input->setZoomAtTargetEnabled(false);
    m_input->setZoomEnabled(false);
    m_input->setRotationEnabled(false);
    m_input->setSelectionEnabled(false);

    QCOMPARE(m_input->zoomAtTargetEnabled(), false);
    QCOMPARE(m_input->zoomEnabled(), false);
    QCOMPARE(m_input->rotationEnabled(), false);
    QCOMPARE(m_input->selectionEnabled(), false);
}

// TODO: QTRD-3380 (mouse events)

QTEST_MAIN(tst_input)
#include "tst_input.moc"
