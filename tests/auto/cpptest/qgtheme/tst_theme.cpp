// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtTest/QtTest>

#include <QtGraphs/QGraphsTheme>

class tst_theme: public QObject
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
    QGraphsTheme *m_theme;
};

void tst_theme::initTestCase()
{
}

void tst_theme::cleanupTestCase()
{
}

void tst_theme::init()
{
    m_theme = new QGraphsTheme();
    m_theme->setColorScheme(QGraphsTheme::ColorScheme::Light);
}

void tst_theme::cleanup()
{
    delete m_theme;
}

void tst_theme::construct()
{
    QGraphsTheme *theme = new QGraphsTheme();
    QVERIFY(theme);
    delete theme;

    theme = new QGraphsTheme();
    theme->setTheme(QGraphsTheme::Theme::MixSeries);
    theme->setColorScheme(QGraphsTheme::ColorScheme::Light);
    QVERIFY(theme);
    QCOMPARE(theme->plotAreaBackgroundColor(), QColor(QRgb(0xFCFCFC)));
    QCOMPARE(theme->isPlotAreaBackgroundVisible(), true);
    QCOMPARE(theme->seriesColors().size(), 5);
    QCOMPARE(theme->seriesColors().at(0), QColor(QRgb(0xFFA615)));
    QCOMPARE(theme->seriesColors().at(4), QColor(QRgb(0x0128F8)));
    QCOMPARE(theme->seriesGradients().size(), 5);
    QCOMPARE(theme->seriesGradients().at(0).stops().at(1).second, QColor(QRgb(0xFFA615)));
    QCOMPARE(theme->seriesGradients().at(4).stops().at(1).second, QColor(QRgb(0x0128F8)));
    QCOMPARE(theme->colorStyle(), QGraphsTheme::ColorStyle::Uniform);
    QCOMPARE(theme->labelFont(), QFont("Arial"));
    QCOMPARE(theme->isGridVisible(), true);
    QCOMPARE(theme->grid().mainColor(), QColor(QRgb(0x545151)));
    QCOMPARE(theme->grid().subColor(), QColor(QRgb(0xAFAFAF)));
    QCOMPARE(theme->grid().mainWidth(), 2.0f);
    QCOMPARE(theme->grid().subWidth(), 1.0f);
    QCOMPARE(theme->labelBackgroundColor(), QColor(QRgb(0xE7E7E7)));
    QCOMPARE(theme->isLabelBackgroundVisible(), true);
    QCOMPARE(theme->isLabelBorderVisible(), true);
    QCOMPARE(theme->labelTextColor(), QColor(QRgb(0x6A6A6A)));
    QCOMPARE(theme->multiHighlightColor(), QColor(QRgb(0x22D47B)));
    QCOMPARE(theme->multiHighlightGradient().stops().at(1).second, QColor(QRgb(0x22D47B)));
    QCOMPARE(theme->singleHighlightColor(), QColor(QRgb(0xCCDC00)));
    QCOMPARE(theme->singleHighlightGradient().stops().at(1).second, QColor(QRgb(0xCCDC00)));
    QCOMPARE(theme->theme(), QGraphsTheme::Theme::MixSeries);
    QCOMPARE(theme->backgroundColor(), QColor(QRgb(0xF2F2F2)));
    QCOMPARE(theme->isBackgroundVisible(), true);
    delete theme;
}

void tst_theme::initialProperties()
{
    QVERIFY(m_theme);

    QCOMPARE(m_theme->plotAreaBackgroundColor(), QColor(QRgb(0xFCFCFC)));
    QCOMPARE(m_theme->isPlotAreaBackgroundVisible(), true);
    QCOMPARE(m_theme->seriesColors().size(), 5);
    QCOMPARE(m_theme->seriesColors().at(0), QColor(QRgb(0xD5F8E7)));
    QCOMPARE(m_theme->seriesGradients().size(), 5);
    QCOMPARE(m_theme->seriesGradients().at(0).stops().at(0).second, QColor(QRgb(0x6A7C73)));
    QCOMPARE(m_theme->seriesGradients().at(0).stops().at(1).second, QColor(QRgb(0xD5F8E7)));
    QCOMPARE(m_theme->labelFont(), QFont(QLatin1String("Arial")));
    QCOMPARE(m_theme->isGridVisible(), true);
    QCOMPARE(m_theme->grid().mainColor(), QColor(QRgb(0x545151)));
    QCOMPARE(m_theme->grid().subColor(), QColor(QRgb(0xAFAFAF)));
    QCOMPARE(m_theme->grid().mainWidth(), 2.0f);
    QCOMPARE(m_theme->grid().subWidth(), 1.0f);
    QCOMPARE(m_theme->labelBackgroundColor(), QColor(QRgb(0xE7E7E7)));
    QCOMPARE(m_theme->isLabelBackgroundVisible(), true);
    QCOMPARE(m_theme->isLabelBorderVisible(), true);
    QCOMPARE(m_theme->labelTextColor(), QColor(QRgb(0x6A6A6A)));
    QCOMPARE(m_theme->multiHighlightColor(), QColor(QRgb(0x22D47B)));
    QCOMPARE(m_theme->multiHighlightGradient().stops().at(1).second, QColor(QRgb(0x22D47B)));
    QCOMPARE(m_theme->singleHighlightColor(), QColor(QRgb(0xCCDC00)));
    QCOMPARE(m_theme->singleHighlightGradient().stops().at(1).second, QColor(QRgb(0xCCDC00)));
    QCOMPARE(m_theme->theme(), QGraphsTheme::Theme::QtGreen);
    QCOMPARE(m_theme->backgroundColor(), QColor(QRgb(0xF2F2F2)));
    QCOMPARE(m_theme->isBackgroundVisible(), true);
}

void tst_theme::initializeProperties()
{
    QVERIFY(m_theme);

    QLinearGradient gradient1;
    QLinearGradient gradient2;
    QLinearGradient gradient3;
    QLinearGradient gradient4;

    QList<QColor> basecolors;
    basecolors << QColor(Qt::red) << QColor(Qt::blue);

    QList<QLinearGradient> basegradients;
    basegradients << gradient1 << gradient2;

    m_theme->setTheme(QGraphsTheme::Theme::OrangeSeries); // We'll override default values with the following setters
    m_theme->setColorScheme(QGraphsTheme::ColorScheme::Dark);
    m_theme->setPlotAreaBackgroundColor(QColor(Qt::red));
    m_theme->setPlotAreaBackgroundVisible(false);
    m_theme->setSeriesColors(basecolors);
    m_theme->setSeriesGradients(basegradients);
    m_theme->setColorStyle(QGraphsTheme::ColorStyle::RangeGradient);
    m_theme->setLabelFont(QFont("Arial"));
    m_theme->setGridVisible(false);
    QGraphsLine grid = m_theme->grid();
    grid.setMainColor(QColor(Qt::green));
    grid.setSubColor(QColor(Qt::red));
    grid.setMainWidth(0.8f);
    grid.setSubWidth(0.5f);
    m_theme->setGrid(grid);
    m_theme->setLabelBackgroundColor(QColor(Qt::gray));
    m_theme->setLabelBackgroundVisible(false);
    m_theme->setLabelBorderVisible(false);
    m_theme->setLabelTextColor(QColor(Qt::cyan));
    m_theme->setMultiHighlightColor(QColor(Qt::darkBlue));
    m_theme->setMultiHighlightGradient(gradient3);
    m_theme->setSingleHighlightColor(QColor(Qt::darkRed));
    m_theme->setSingleHighlightGradient(gradient4);
    m_theme->setBackgroundColor(QColor(Qt::darkYellow));
    m_theme->setBackgroundVisible(false);

    QCOMPARE(m_theme->plotAreaBackgroundColor(), QColor(Qt::red));
    QCOMPARE(m_theme->isPlotAreaBackgroundVisible(), false);
    QCOMPARE(m_theme->seriesColors().size(), 2);
    QCOMPARE(m_theme->seriesColors().at(0), QColor(Qt::red));
    QCOMPARE(m_theme->seriesColors().at(1), QColor(Qt::blue));
    QCOMPARE(m_theme->seriesGradients().size(), 2);
    QCOMPARE(m_theme->seriesGradients().at(0), gradient1);
    QCOMPARE(m_theme->seriesGradients().at(0), gradient2);
    QCOMPARE(m_theme->colorStyle(), QGraphsTheme::ColorStyle::RangeGradient);
    QCOMPARE(m_theme->labelFont(), QFont("Arial"));
    QCOMPARE(m_theme->isGridVisible(), false);
    QCOMPARE(m_theme->grid().mainColor(), QColor(Qt::green));
    QCOMPARE(m_theme->grid().subColor(), QColor(Qt::red));
    QCOMPARE(m_theme->grid().mainWidth(), 0.8f);
    QCOMPARE(m_theme->grid().subWidth(), 0.5f);
    QCOMPARE(m_theme->labelBackgroundColor(), QColor(Qt::gray));
    QCOMPARE(m_theme->isLabelBackgroundVisible(), false);
    QCOMPARE(m_theme->isLabelBorderVisible(), false);
    QCOMPARE(m_theme->labelTextColor(), QColor(Qt::cyan));
    QCOMPARE(m_theme->multiHighlightColor(), QColor(Qt::darkBlue));
    QCOMPARE(m_theme->multiHighlightGradient(), gradient3);
    QCOMPARE(m_theme->singleHighlightColor(), QColor(Qt::darkRed));
    QCOMPARE(m_theme->singleHighlightGradient(), gradient4);
    QCOMPARE(m_theme->theme(), QGraphsTheme::Theme::OrangeSeries);
    QCOMPARE(m_theme->backgroundColor(), QColor(Qt::darkYellow));
    QCOMPARE(m_theme->isBackgroundVisible(), false);
}

QTEST_MAIN(tst_theme)
#include "tst_theme.moc"
