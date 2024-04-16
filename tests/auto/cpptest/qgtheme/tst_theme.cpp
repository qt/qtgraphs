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
    m_theme->setColorScheme(Qt::ColorScheme::Light);
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
    theme->setColorScheme(Qt::ColorScheme::Light);
    QVERIFY(theme);
    QCOMPARE(theme->backgroundColor(), QColor(QRgb(0xFCFCFC)));
    QCOMPARE(theme->isBackgroundEnabled(), true);
    QCOMPARE(theme->seriesColors().size(), 5);
    QCOMPARE(theme->seriesColors().at(0), QColor(QRgb(0xFFA615)));
    QCOMPARE(theme->seriesColors().at(4), QColor(QRgb(0x0128F8)));
    QCOMPARE(theme->seriesGradients().size(), 5);
    QCOMPARE(theme->seriesGradients().at(0).stops().at(1).second, QColor(QRgb(0xFFA615)));
    QCOMPARE(theme->seriesGradients().at(4).stops().at(1).second, QColor(QRgb(0x0128F8)));
    QCOMPARE(theme->colorStyle(), QGraphsTheme::ColorStyle::Uniform);
    QCOMPARE(theme->labelFont(), QFont("Arial"));
    QCOMPARE(theme->isGridEnabled(), true);
    QCOMPARE(theme->gridMainColor(), QColor(QRgb(0x545151)));
    QCOMPARE(theme->gridSubColor(), QColor(QRgb(0xAFAFAF)));
    QCOMPARE(theme->gridMainWidth(), 0.25f);
    QCOMPARE(theme->labelBackgroundColor(), QColor(QRgb(0xE7E7E7)));
    QCOMPARE(theme->isLabelBackgroundEnabled(), true);
    QCOMPARE(theme->isLabelBorderEnabled(), true);
    QCOMPARE(theme->labelTextColor(), QColor(QRgb(0x6A6A6A)));
    QCOMPARE(theme->multiHighlightColor(), QColor(QRgb(0x22D47B)));
    QCOMPARE(theme->multiHighlightGradient().stops().at(1).second, QColor(QRgb(0x22D47B)));
    QCOMPARE(theme->singleHighlightColor(), QColor(QRgb(0xCCDC00)));
    QCOMPARE(theme->singleHighlightGradient().stops().at(1).second, QColor(QRgb(0xCCDC00)));
    QCOMPARE(theme->theme(), QGraphsTheme::Theme::MixSeries);
    QCOMPARE(theme->windowColor(), QColor(QRgb(0xF2F2F2)));
    delete theme;
}

void tst_theme::initialProperties()
{
    QVERIFY(m_theme);

    QCOMPARE(m_theme->backgroundColor(), QColor(QRgb(0xFCFCFC)));
    QCOMPARE(m_theme->isBackgroundEnabled(), true);
    QCOMPARE(m_theme->seriesColors().size(), 1);
    QCOMPARE(m_theme->seriesColors().at(0), QColor(Qt::black));
    QCOMPARE(m_theme->seriesGradients().size(), 1);
    QCOMPARE(m_theme->seriesGradients().at(0).stops().at(0).second, QColor(Qt::black));
    QCOMPARE(m_theme->seriesGradients().at(0).stops().at(1).second, QColor(Qt::black));
    QCOMPARE(m_theme->labelFont(), QFont(QLatin1String("Arial")));
    QCOMPARE(m_theme->isGridEnabled(), true);
    QCOMPARE(m_theme->gridMainColor(), QColor(QRgb(0x545151)));
    QCOMPARE(m_theme->gridSubColor(), QColor(QRgb(0xAFAFAF)));
    QCOMPARE(m_theme->gridMainWidth(), 0.25f);
    QCOMPARE(m_theme->labelBackgroundColor(), QColor(QRgb(0xE7E7E7)));
    QCOMPARE(m_theme->isLabelBackgroundEnabled(), true);
    QCOMPARE(m_theme->isLabelBorderEnabled(), true);
    QCOMPARE(m_theme->labelTextColor(), QColor(QRgb(0x6A6A6A)));
    QCOMPARE(m_theme->multiHighlightColor(), QColor(QRgb(0x22D47B)));
    QCOMPARE(m_theme->multiHighlightGradient().stops().at(1).second, QColor(QRgb(0x22D47B)));
    QCOMPARE(m_theme->singleHighlightColor(), QColor(QRgb(0xCCDC00)));
    QCOMPARE(m_theme->singleHighlightGradient().stops().at(1).second, QColor(QRgb(0xCCDC00)));
    QCOMPARE(m_theme->theme(), QGraphsTheme::Theme::UserDefined);
    QCOMPARE(m_theme->windowColor(), QColor(QRgb(0xF2F2F2)));
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
    m_theme->setColorScheme(Qt::ColorScheme::Dark);
    m_theme->setBackgroundColor(QColor(Qt::red));
    m_theme->setBackgroundEnabled(false);
    m_theme->setSeriesColors(basecolors);
    m_theme->setSeriesGradients(basegradients);
    m_theme->setColorStyle(QGraphsTheme::ColorStyle::RangeGradient);
    m_theme->setLabelFont(QFont("Arial"));
    m_theme->setGridEnabled(false);
    m_theme->setGridMainColor(QColor(Qt::green));
    m_theme->setGridSubColor(QColor(Qt::red));
    m_theme->setGridMainWidth(0.8f);
    m_theme->setLabelBackgroundColor(QColor(Qt::gray));
    m_theme->setLabelBackgroundEnabled(false);
    m_theme->setLabelBorderEnabled(false);
    m_theme->setLabelTextColor(QColor(Qt::cyan));
    m_theme->setMultiHighlightColor(QColor(Qt::darkBlue));
    m_theme->setMultiHighlightGradient(gradient3);
    m_theme->setSingleHighlightColor(QColor(Qt::darkRed));
    m_theme->setSingleHighlightGradient(gradient4);
    m_theme->setWindowColor(QColor(Qt::darkYellow));

    QCOMPARE(m_theme->backgroundColor(), QColor(Qt::red));
    QCOMPARE(m_theme->isBackgroundEnabled(), false);
    QCOMPARE(m_theme->seriesColors().size(), 2);
    QCOMPARE(m_theme->seriesColors().at(0), QColor(Qt::red));
    QCOMPARE(m_theme->seriesColors().at(1), QColor(Qt::blue));
    QCOMPARE(m_theme->seriesGradients().size(), 2);
    QCOMPARE(m_theme->seriesGradients().at(0), gradient1);
    QCOMPARE(m_theme->seriesGradients().at(0), gradient2);
    QCOMPARE(m_theme->colorStyle(), QGraphsTheme::ColorStyle::RangeGradient);
    QCOMPARE(m_theme->labelFont(), QFont("Arial"));
    QCOMPARE(m_theme->isGridEnabled(), false);
    QCOMPARE(m_theme->gridMainColor(), QColor(Qt::green));
    QCOMPARE(m_theme->gridSubColor(), QColor(Qt::red));
    QCOMPARE(m_theme->gridMainWidth(), 0.8f);
    QCOMPARE(m_theme->labelBackgroundColor(), QColor(Qt::gray));
    QCOMPARE(m_theme->isLabelBackgroundEnabled(), false);
    QCOMPARE(m_theme->isLabelBorderEnabled(), false);
    QCOMPARE(m_theme->labelTextColor(), QColor(Qt::cyan));
    QCOMPARE(m_theme->multiHighlightColor(), QColor(Qt::darkBlue));
    QCOMPARE(m_theme->multiHighlightGradient(), gradient3);
    QCOMPARE(m_theme->singleHighlightColor(), QColor(Qt::darkRed));
    QCOMPARE(m_theme->singleHighlightGradient(), gradient4);
    QCOMPARE(m_theme->theme(), QGraphsTheme::Theme::OrangeSeries);
    QCOMPARE(m_theme->windowColor(), QColor(Qt::darkYellow));
}

QTEST_MAIN(tst_theme)
#include "tst_theme.moc"
