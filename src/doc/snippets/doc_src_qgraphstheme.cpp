// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include <QtGraphs/Q3DBars>
#include <QtGraphs/QGraphsTheme>

int main(int argc, char **argv)
{
    //! [0]
    QGraphsTheme *theme = new QGraphsTheme();
    //! [0]

    //! [1]
    QGraphsTheme *theme = new QGraphsTheme();
    theme->setBackgroundVisible(false);
    theme->setLabelBackgroundVisible(false);
    //! [1]

    //! [2]
    QGraphsTheme *theme = new QGraphsTheme();
    theme->setBackgroundColor(QColor(QRgb(0x99ca53)));
    theme->setBackgroundVisible(true);
    QList<QColor> colors = { QColor(QRgb(0x209fdf)) };
    theme->setSeriesColors(colors);
    theme->setColorStyle(QGraphsTheme::ColorStyle::Uniform);
    theme->setLabelFont(QFont(QStringLiteral("Impact"), 35));
    theme->setGridVisible(true);
    theme->setGridMainColor(QColor(QRgb(0x99ca53)));
    theme->setLabelBackgroundColor(QColor(0xf6, 0xa6, 0x25, 0xa0));
    theme->setLabelBackgroundVisible(true);
    theme->setLabelBorderVisible(true);
    theme->setLabelTextColor(QColor(QRgb(0x404044)));
    theme->setMultiHighlightColor(QColor(QRgb(0x6d5fd5)));
    theme->setSingleHighlightColor(QColor(QRgb(0xf6a625)));
    theme->setWindowColor(QColor(QRgb(0xffffff)));
    //! [2]

    //! [3]
    Q3DBars *graph = new Q3DBars();
    graph->activeTheme()->setTheme(QGraphsTheme::Theme::MixSeries);
    QList<QColor> color = { QColor(Qt::red) };
    graph->activeTheme()->setSeriesColors(color);
    graph->activeTheme()->setSingleHighlightColor(Qt::yellow);
    //! [3]
}

//! [4]
Scatter3D {
    ...
    theme: GraphsTheme { theme: GraphsTheme.Theme.YellowSeries }
    ...
}
//! [4]

//! [5]
Bars3D {
    ...
    theme: GraphsTheme {
        theme: GraphsTheme.Theme.QtGreenNeon
        labelBorderVisible: true
        labelFont.pointSize: 35
        labelBackgroundVisible: false
    }
    ...
}
//! [5]

//! [6]
Surface3D {
    ...
    theme: GraphsTheme {
        backgroundColor: "red"
        backgroundVisible: true
        seriesColors: ["blue"]
        colorStyle: GraphsTheme.ColorStyle.Uniform
        labelFont.family: "Lucida Handwriting"
        labelFont.pointSize: 35
        gridVisible: false
        gridMainColor: "black"
        labelBackgroundColor: "black"
        labelBackgroundVisible: true
        labelBorderVisible: false
        labelTextColor: "white"
        multiHighlightColor: "green"
        singleHighlightColor: "darkRed"
        windowColor: "white"
    }
    ...
}
//! [6]
