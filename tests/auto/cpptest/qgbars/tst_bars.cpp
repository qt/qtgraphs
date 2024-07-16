// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtTest/QtTest>

#include <QtGraphs/QCustom3DItem>
#include <QtGraphsWidgets/q3dbarswidgetitem.h>

#include "cpptestutil.h"

class tst_bars: public QObject
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

    void addTheme();
    void removeTheme();

    // TODO: QTBUG-99844
    //void addCustomItem();
    //void removeCustomItem();

    void renderToImage();

private:
    Q3DBarsWidgetItem *m_graph;
    QQuickWidget *m_quickWidget;
};

QBar3DSeries *newSeries()
{
    QBar3DSeries *series = new QBar3DSeries;
    QBarDataRow data;
    data << QBarDataItem(-1.0f) << QBarDataItem(3.0f) << QBarDataItem(7.5f) << QBarDataItem(5.0f)
         << QBarDataItem(2.2f);
    series->dataProxy()->addRow(data);
    return series;
}

void tst_bars::initTestCase()
{
    if (!CpptestUtil::isOpenGLSupported())
        QSKIP("OpenGL not supported on this platform");
}

void tst_bars::cleanupTestCase()
{
}

void tst_bars::init()
{
    m_graph = new Q3DBarsWidgetItem();
    m_quickWidget = new QQuickWidget();
    m_graph->setWidget(m_quickWidget);
}

void tst_bars::cleanup()
{
    delete m_graph;
    delete m_quickWidget;
}

void tst_bars::construct()
{
    Q3DBarsWidgetItem *graph = new Q3DBarsWidgetItem();
    QVERIFY(graph);
    delete graph;
}

void tst_bars::initialProperties()
{
    QVERIFY(m_graph);
    QCOMPARE(m_graph->isMultiSeriesUniform(), false);
    QCOMPARE(m_graph->barThickness(), 1.0);
    QCOMPARE(m_graph->barSpacing(), QSizeF(1.0f, 1.0f));
    QCOMPARE(m_graph->barSeriesMargin(), QSizeF(0.0f, 0.0f));
    QCOMPARE(m_graph->isBarSpacingRelative(), true);
    QCOMPARE(m_graph->seriesList().size(), 0);
    QVERIFY(!m_graph->selectedSeries());
    QVERIFY(!m_graph->primarySeries());
    QCOMPARE(m_graph->floorLevel(), 0.0);
    QCOMPARE(m_graph->columnAxis()->orientation(), QAbstract3DAxis::AxisOrientation::X);
    QCOMPARE(m_graph->valueAxis()->orientation(), QAbstract3DAxis::AxisOrientation::Y);
    QCOMPARE(m_graph->rowAxis()->orientation(), QAbstract3DAxis::AxisOrientation::Z);

    // Common properties
    QCOMPARE(m_graph->activeTheme()->theme(), QGraphsTheme::Theme::QtGreen);
    QCOMPARE(m_graph->selectionMode(), QtGraphs3D::SelectionFlag::Item);
    QCOMPARE(m_graph->shadowQuality(), QtGraphs3D::ShadowQuality::Medium);
    QVERIFY(m_graph->scene());
    QCOMPARE(m_graph->measureFps(), false);
    QCOMPARE(m_graph->isOrthoProjection(), false);
    QCOMPARE(m_graph->selectedElement(), QtGraphs3D::ElementType::None);
    QCOMPARE(m_graph->aspectRatio(), 2.0);
    QCOMPARE(m_graph->optimizationHint(), QtGraphs3D::OptimizationHint::Default);
    QCOMPARE(m_graph->isPolar(), false);
    QCOMPARE(m_graph->radialLabelOffset(), 1.0);
    QCOMPARE(m_graph->horizontalAspectRatio(), 0.0);
    QCOMPARE(m_graph->locale(), QLocale("C"));
    QCOMPARE(m_graph->queriedGraphPosition(), QVector3D(0, 0, 0));
    QCOMPARE(m_graph->margin(), -1.0);
    QCOMPARE(m_graph->labelMargin(), 0.1f);
    QCOMPARE(m_graph->ambientLightStrength(), 0.25);
    QCOMPARE(m_graph->lightColor(), QColor(Qt::white));
    QCOMPARE(m_graph->lightStrength(), 5.0f);
    QCOMPARE(m_graph->shadowStrength(), 25.0f);
}

void tst_bars::initializeProperties()
{
    QVERIFY(m_graph);

    m_graph->setMultiSeriesUniform(true);
    m_graph->setBarThickness(0.2f);
    m_graph->setBarSpacing(QSizeF(0.1f, 0.1f));
    m_graph->setBarSeriesMargin(QSizeF(0.3f, 0.3f));
    m_graph->setBarSpacingRelative(false);
    m_graph->setFloorLevel(1.0f);

    QCOMPARE(m_graph->isMultiSeriesUniform(), true);
    QCOMPARE(m_graph->barThickness(), 0.2f);
    QCOMPARE(m_graph->barSpacing(), QSizeF(0.1f, 0.1f));
    QCOMPARE(m_graph->barSeriesMargin(), QSizeF(0.3f, 0.3f));
    QCOMPARE(m_graph->isBarSpacingRelative(), false);
    QCOMPARE(m_graph->floorLevel(), 1.0f);

    QGraphsTheme *theme = new QGraphsTheme();
    theme->setColorScheme(QGraphsTheme::ColorScheme::Light);
    theme->setTheme(QGraphsTheme::Theme::QtGreenNeon);
    m_graph->setActiveTheme(theme);
    m_graph->setSelectionMode(QtGraphs3D::SelectionFlag::Item | QtGraphs3D::SelectionFlag::Row
                              | QtGraphs3D::SelectionFlag::Slice);
    m_graph->setShadowQuality(QtGraphs3D::ShadowQuality::SoftHigh);
    QCOMPARE(m_graph->shadowQuality(), QtGraphs3D::ShadowQuality::SoftHigh);
    m_graph->setMeasureFps(true);
    m_graph->setOrthoProjection(true);
    m_graph->setAspectRatio(1.0);
    m_graph->setOptimizationHint(QtGraphs3D::OptimizationHint::Default);
    m_graph->setPolar(true);
    m_graph->setRadialLabelOffset(0.1f);
    m_graph->setHorizontalAspectRatio(1.0);
    m_graph->setLocale(QLocale("FI"));
    m_graph->setMargin(1.0);
    m_graph->setLabelMargin(1.0f);
    m_graph->setAmbientLightStrength(0.3f);
    m_graph->setLightColor(QColor(Qt::yellow));
    m_graph->setLightStrength(2.5f);
    m_graph->setShadowStrength(50.f);

    QCOMPARE(m_graph->activeTheme()->theme(), QGraphsTheme::Theme::QtGreenNeon);
    QCOMPARE(m_graph->selectionMode(),
             QtGraphs3D::SelectionFlag::Item | QtGraphs3D::SelectionFlag::Row
                 | QtGraphs3D::SelectionFlag::Slice);
    QCOMPARE(m_graph->shadowQuality(),
             QtGraphs3D::ShadowQuality::None); // Ortho disables shadows
    QCOMPARE(m_graph->measureFps(), true);
    QCOMPARE(m_graph->isOrthoProjection(), true);
    QCOMPARE(m_graph->aspectRatio(), 1.0);
    QCOMPARE(m_graph->optimizationHint(), QtGraphs3D::OptimizationHint::Default);
    QCOMPARE(m_graph->isPolar(), true);
    QCOMPARE(m_graph->radialLabelOffset(), 0.1f);
    QCOMPARE(m_graph->horizontalAspectRatio(), 1.0);
    QCOMPARE(m_graph->locale(), QLocale("FI"));
    QCOMPARE(m_graph->margin(), 1.0);
    QCOMPARE(m_graph->labelMargin(), 1.0f);
    QCOMPARE(m_graph->ambientLightStrength(), 0.3f);
    QCOMPARE(m_graph->lightColor(), QColor(Qt::yellow));
    QCOMPARE(m_graph->lightStrength(), 2.5f);
    QCOMPARE(m_graph->shadowStrength(), 50.0f);
}

void tst_bars::invalidProperties()
{
    m_graph->setSelectionMode(QtGraphs3D::SelectionFlag::Column | QtGraphs3D::SelectionFlag::Row
                              | QtGraphs3D::SelectionFlag::Slice);
    m_graph->setAspectRatio(-1.0);
    m_graph->setHorizontalAspectRatio(-1.0);
    m_graph->setLocale(QLocale("XX"));

    QCOMPARE(m_graph->selectionMode(), QtGraphs3D::SelectionFlag::Item);
    QCOMPARE(m_graph->aspectRatio(), -1.0/*2.0*/); // TODO: Fix once QTRD-3367 is done
    QCOMPARE(m_graph->horizontalAspectRatio(), -1.0/*0.0*/); // TODO: Fix once QTRD-3367 is done
    QCOMPARE(m_graph->locale(), QLocale("C"));

    m_graph->setAmbientLightStrength(-1.0f);
    QCOMPARE(m_graph->ambientLightStrength(), 0.25f);
    m_graph->setAmbientLightStrength(1.1f);
    QCOMPARE(m_graph->ambientLightStrength(), 0.25f);

    m_graph->setLightStrength(-1.0f);
    QCOMPARE(m_graph->lightStrength(), 5.0f);
    m_graph->setLightStrength(10.1f);
    QCOMPARE(m_graph->lightStrength(), 5.0f);

    m_graph->setShadowStrength(-1.0f);
    QCOMPARE(m_graph->shadowStrength(), 25.0f);
    m_graph->setShadowStrength(100.1f);
    QCOMPARE(m_graph->shadowStrength(), 25.0f);
}

void tst_bars::addSeries()
{
    QBar3DSeries *series = newSeries();

    m_graph->addSeries(series);

    QCOMPARE(m_graph->seriesList().size(), 1);
    QVERIFY(!m_graph->selectedSeries());
    QCOMPARE(m_graph->primarySeries(), series);
}

void tst_bars::addMultipleSeries()
{
    QBar3DSeries *series = newSeries();
    QBar3DSeries *series2 = newSeries();
    QBar3DSeries *series3 = newSeries();

    m_graph->addSeries(series);
    m_graph->addSeries(series2);
    m_graph->addSeries(series3);

    QCOMPARE(m_graph->seriesList().size(), 3);
    QCOMPARE(m_graph->primarySeries(), series);

    m_graph->setPrimarySeries(series2);

    QCOMPARE(m_graph->primarySeries(), series2);
}

void tst_bars::selectSeries()
{
    QBar3DSeries *series = newSeries();

    m_graph->addSeries(series);
    m_graph->primarySeries()->setSelectedBar(QPoint(0, 0));

    QCOMPARE(m_graph->seriesList().size(), 1);
    QCOMPARE(m_graph->selectedSeries(), series);

    m_graph->clearSelection();
    QVERIFY(!m_graph->selectedSeries());
}

void tst_bars::removeSeries()
{
    QBar3DSeries *series = newSeries();

    m_graph->addSeries(series);
    m_graph->removeSeries(series);
    QCOMPARE(m_graph->seriesList().size(), 0);
    delete series;
}

void tst_bars::removeMultipleSeries()
{
    QBar3DSeries *series = newSeries();
    QBar3DSeries *series2 = newSeries();
    QBar3DSeries *series3 = newSeries();

    m_graph->addSeries(series);
    m_graph->addSeries(series2);
    m_graph->addSeries(series3);

    m_graph->primarySeries()->setSelectedBar(QPoint(0, 0));
    QCOMPARE(m_graph->selectedSeries(), series);

    m_graph->removeSeries(series);
    QCOMPARE(m_graph->seriesList().size(), 2);
    QCOMPARE(m_graph->primarySeries(), series2);
    QVERIFY(!m_graph->selectedSeries());

    m_graph->removeSeries(series2);
    QCOMPARE(m_graph->seriesList().size(), 1);
    QCOMPARE(m_graph->primarySeries(), series3);

    m_graph->removeSeries(series3);
    QCOMPARE(m_graph->seriesList().size(), 0);

    delete series;
    delete series2;
    delete series3;
}

void tst_bars::hasSeries()
{
    QBar3DSeries *series1 = newSeries();
    m_graph->addSeries(series1);
    QCOMPARE(m_graph->hasSeries(series1), true);
    QBar3DSeries *series2 = newSeries();
    QCOMPARE(m_graph->hasSeries(series2), false);
}

void tst_bars::addTheme()
{
    QGraphsTheme *theme = new QGraphsTheme();
    theme->setColorScheme(QGraphsTheme::ColorScheme::Light);
    theme->setTheme(QGraphsTheme::Theme::QtGreenNeon);
    QGraphsTheme *theme2 = new QGraphsTheme();
    theme->setColorScheme(QGraphsTheme::ColorScheme::Light);
    QGraphsTheme *initialTheme = m_graph->activeTheme();
    m_graph->addTheme(theme);
    m_graph->addTheme(theme2);

    QCOMPARE(m_graph->themes().size(), 3); // Default, plus added ones
    QCOMPARE(m_graph->activeTheme(), initialTheme);
    m_graph->setActiveTheme(theme2);
    QCOMPARE(m_graph->activeTheme(), theme2);
}

void tst_bars::removeTheme()
{
    QGraphsTheme *theme = new QGraphsTheme();
    theme->setColorScheme(QGraphsTheme::ColorScheme::Light);
    theme->setTheme(QGraphsTheme::Theme::QtGreenNeon);
    QGraphsTheme *theme2 = new QGraphsTheme();
    theme->setColorScheme(QGraphsTheme::ColorScheme::Light);
    m_graph->addTheme(theme);
    m_graph->addTheme(theme2);

    m_graph->setActiveTheme(theme2);
    QCOMPARE(m_graph->activeTheme(), theme2);
    m_graph->releaseTheme(theme2);
    QCOMPARE(m_graph->themes().size(), 2);
    m_graph->releaseTheme(theme);
    QCOMPARE(m_graph->themes().size(), 1); // Default theme remains

    delete theme2;
    delete theme;
}
// TODO: QTBUG-99844
/*
void tst_bars::addCustomItem()
{
    QCustom3DItem *item = new QCustom3DItem();
    QCustom3DItem *item2 = new QCustom3DItem();

    m_graph->addCustomItem(item);
    QCOMPARE(m_graph->customItems().size(), 1);
    m_graph->addCustomItem(item2);
    QCOMPARE(m_graph->customItems().size(), 2);
}

void tst_bars::removeCustomItem()
{
    QCustom3DItem *item = new QCustom3DItem();
    QCustom3DItem *item2 = new QCustom3DItem();
    QCustom3DItem *item3 = new QCustom3DItem();
    item3->setPosition(QVector3D(1, 1, 1));

    m_graph->addCustomItem(item);
    m_graph->addCustomItem(item2);
    m_graph->addCustomItem(item3);

    m_graph->releaseCustomItem(item);
    QCOMPARE(m_graph->customItems().size(), 2);
    m_graph->removeCustomItem(item2);
    QCOMPARE(m_graph->customItems().size(), 1);
    m_graph->addCustomItem(item);
    m_graph->removeCustomItemAt(QVector3D(1, 1, 1));
    QCOMPARE(m_graph->customItems().size(), 1);
    m_graph->removeCustomItems();
    QCOMPARE(m_graph->customItems().size(), 0);
}
*/
void tst_bars::renderToImage()
{
    /* Crashes on some CI machines using Mesa, but can't repro locally, so commented out for now.
    m_graph->addSeries(newSeries());

    QImage image = m_graph->renderToImage();
    QCOMPARE(image.size(), m_graph->size());

    image = m_graph->renderToImage(8);
    QCOMPARE(image.size(), m_graph->size());

    image = m_graph->renderToImage(4, QSize(300, 300));
    QCOMPARE(image.size(), QSize(300, 300));
    */
}

QTEST_MAIN(tst_bars)
#include "tst_bars.moc"
