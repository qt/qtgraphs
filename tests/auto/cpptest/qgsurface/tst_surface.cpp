// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtTest/QtTest>

#include <QtGraphsWidgets/q3dsurfacewidget.h>

#include "cpptestutil.h"

class tst_surface: public QObject
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

    void addSeries();
    void addMultipleSeries();
    void selectSeries();
    void removeSeries();
    void removeMultipleSeries();
    void hasSeries();

private:
    Q3DSurfaceWidget *m_graph;
};

QSurface3DSeries *newSeries()
{
    QSurface3DSeries *series = new QSurface3DSeries;
    QSurfaceDataArray data;
    QSurfaceDataRow dataRow1;
    QSurfaceDataRow dataRow2;
    dataRow1 << QSurfaceDataItem(0.0f, 0.1f, 0.5f) << QSurfaceDataItem(1.0f, 0.5f, 0.5f);
    dataRow2 << QSurfaceDataItem(0.0f, 1.8f, 1.0f) << QSurfaceDataItem(1.0f, 1.2f, 1.0f);
    data << dataRow1 << dataRow2;
    series->dataProxy()->resetArray(data);

    return series;
}

void tst_surface::initTestCase()
{
    if (!CpptestUtil::isOpenGLSupported())
        QSKIP("OpenGL not supported on this platform");
}

void tst_surface::cleanupTestCase()
{
}

void tst_surface::init()
{
    m_graph = new Q3DSurfaceWidget();
}

void tst_surface::cleanup()
{
    delete m_graph;
}

void tst_surface::construct()
{
    Q3DSurfaceWidget *graph = new Q3DSurfaceWidget();
    QVERIFY(graph);
    delete graph;
}

void tst_surface::initialProperties()
{
    QVERIFY(m_graph);
    QCOMPARE(m_graph->seriesList().size(), 0);
    QVERIFY(!m_graph->selectedSeries());
    QCOMPARE(m_graph->flipHorizontalGrid(), false);
    QCOMPARE(m_graph->axisX()->orientation(), QAbstract3DAxis::AxisOrientation::X);
    QCOMPARE(m_graph->axisY()->orientation(), QAbstract3DAxis::AxisOrientation::Y);
    QCOMPARE(m_graph->axisZ()->orientation(), QAbstract3DAxis::AxisOrientation::Z);

    // Common properties
    QCOMPARE(m_graph->activeTheme()->theme(), QGraphsTheme::Theme::UserDefined);
    QCOMPARE(m_graph->selectionMode(), QGraphs3D::SelectionFlag::Item);
    QCOMPARE(m_graph->shadowQuality(), QGraphs3D::ShadowQuality::Medium);
    QVERIFY(m_graph->scene());
    QCOMPARE(m_graph->measureFps(), false);
    QCOMPARE(m_graph->isOrthoProjection(), false);
    QCOMPARE(m_graph->selectedElement(), QGraphs3D::ElementType::None);
    QCOMPARE(m_graph->aspectRatio(), 2.0);
    QCOMPARE(m_graph->optimizationHint(), QGraphs3D::OptimizationHint::Default);
    QCOMPARE(m_graph->isPolar(), false);
    QCOMPARE(m_graph->radialLabelOffset(), 1.0);
    QCOMPARE(m_graph->horizontalAspectRatio(), 0.0);
    QCOMPARE(m_graph->locale(), QLocale("C"));
    QCOMPARE(m_graph->queriedGraphPosition(), QVector3D(0, 0, 0));
    QCOMPARE(m_graph->margin(), -1.0);
}

void tst_surface::initializeProperties()
{
    m_graph->setFlipHorizontalGrid(true);

    QCOMPARE(m_graph->flipHorizontalGrid(), true);

    QGraphsTheme *theme = new QGraphsTheme();
    theme->setColorScheme(Qt::ColorScheme::Light);
    theme->setTheme(QGraphsTheme::Theme::QtGreenNeon);
    m_graph->setActiveTheme(theme);
    m_graph->setSelectionMode(QGraphs3D::SelectionFlag::Item | QGraphs3D::SelectionFlag::Row
                              | QGraphs3D::SelectionFlag::Slice);
    m_graph->setShadowQuality(QGraphs3D::ShadowQuality::SoftHigh);
    QCOMPARE(m_graph->shadowQuality(), QGraphs3D::ShadowQuality::SoftHigh);
    m_graph->setMeasureFps(true);
    m_graph->setOrthoProjection(true);
    m_graph->setAspectRatio(1.0);
    m_graph->setOptimizationHint(QGraphs3D::OptimizationHint::Default);
    m_graph->setPolar(true);
    m_graph->setRadialLabelOffset(0.1f);
    m_graph->setHorizontalAspectRatio(1.0);
    m_graph->setLocale(QLocale("FI"));
    m_graph->setMargin(1.0);

    QCOMPARE(m_graph->activeTheme()->theme(), QGraphsTheme::Theme::QtGreenNeon);
    QCOMPARE(m_graph->selectionMode(),
             QGraphs3D::SelectionFlag::Item | QGraphs3D::SelectionFlag::Row
                 | QGraphs3D::SelectionFlag::Slice);
    QCOMPARE(m_graph->shadowQuality(),
             QGraphs3D::ShadowQuality::None); // Ortho disables shadows
    QCOMPARE(m_graph->measureFps(), true);
    QCOMPARE(m_graph->isOrthoProjection(), true);
    QCOMPARE(m_graph->aspectRatio(), 1.0);
    QCOMPARE(m_graph->optimizationHint(), QGraphs3D::OptimizationHint::Default);
    QCOMPARE(m_graph->isPolar(), true);
    QCOMPARE(m_graph->radialLabelOffset(), 0.1f);
    QCOMPARE(m_graph->horizontalAspectRatio(), 1.0);
    QCOMPARE(m_graph->locale(), QLocale("FI"));
    QCOMPARE(m_graph->margin(), 1.0);
}

void tst_surface::invalidProperties()
{
    m_graph->setSelectionMode(QGraphs3D::SelectionFlag::Column | QGraphs3D::SelectionFlag::Row
                              | QGraphs3D::SelectionFlag::Slice);
    m_graph->setAspectRatio(-1.0);
    m_graph->setHorizontalAspectRatio(-1.0);
    m_graph->setLocale(QLocale("XX"));

    QCOMPARE(m_graph->selectionMode(), QGraphs3D::SelectionFlag::Item);
    QCOMPARE(m_graph->aspectRatio(), -1.0/*2.0*/); // TODO: Fix once QTRD-3367 is done
    QCOMPARE(m_graph->horizontalAspectRatio(), -1.0/*0.0*/); // TODO: Fix once QTRD-3367 is done
    QCOMPARE(m_graph->locale(), QLocale("C"));
}

void tst_surface::addSeries()
{
    m_graph->addSeries(newSeries());

    QCOMPARE(m_graph->seriesList().size(), 1);
    QVERIFY(!m_graph->selectedSeries());
}

void tst_surface::addMultipleSeries()
{
    QSurface3DSeries *series = newSeries();
    QSurface3DSeries *series2 = newSeries();
    QSurface3DSeries *series3 = newSeries();

    m_graph->addSeries(series);
    m_graph->addSeries(series2);
    m_graph->addSeries(series3);

    QCOMPARE(m_graph->seriesList().size(), 3);
}

void tst_surface::selectSeries()
{
    QSurface3DSeries *series = newSeries();

    m_graph->addSeries(series);
    m_graph->seriesList()[0]->setSelectedPoint(QPoint(0, 0));

    QCOMPARE(m_graph->seriesList().size(), 1);
    QCOMPARE(m_graph->selectedSeries(), series);

    m_graph->clearSelection();
    QVERIFY(!m_graph->selectedSeries());
}

void tst_surface::removeSeries()
{
    QSurface3DSeries *series = newSeries();

    m_graph->addSeries(series);
    m_graph->removeSeries(series);
    QCOMPARE(m_graph->seriesList().size(), 0);

    delete series;
}

void tst_surface::removeMultipleSeries()
{
    QSurface3DSeries *series = newSeries();
    QSurface3DSeries *series2 = newSeries();
    QSurface3DSeries *series3 = newSeries();

    m_graph->addSeries(series);
    m_graph->addSeries(series2);
    m_graph->addSeries(series3);

    m_graph->seriesList()[0]->setSelectedPoint(QPoint(0, 0));
    QCOMPARE(m_graph->selectedSeries(), series);

    m_graph->removeSeries(series);
    QCOMPARE(m_graph->seriesList().size(), 2);
    QVERIFY(!m_graph->selectedSeries());

    m_graph->removeSeries(series2);
    QCOMPARE(m_graph->seriesList().size(), 1);

    m_graph->removeSeries(series3);
    QCOMPARE(m_graph->seriesList().size(), 0);

    delete series;
    delete series2;
    delete series3;
}

void tst_surface::hasSeries()
{
    QSurface3DSeries *series1 = newSeries();
    m_graph->addSeries(series1);
    QCOMPARE(m_graph->hasSeries(series1), true);
    QSurface3DSeries *series2 = newSeries();
    QCOMPARE(m_graph->hasSeries(series2), false);
}

QTEST_MAIN(tst_surface)
#include "tst_surface.moc"
