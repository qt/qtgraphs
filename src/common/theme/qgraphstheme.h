// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QGRAPHSTHEME_H
#define QGRAPHSTHEME_H

#include <QtCore/qobject.h>
#include <QtGraphs/qgraphsglobal.h>
#include <QtGui/qbrush.h>
#include <QtGui/qcolor.h>
#include <QtGui/qfont.h>
#include <QtQml/qqmlengine.h>
#include <QtQml/qqmlparserstatus.h>

QT_BEGIN_NAMESPACE

class QQuickGraphsColor;
class QQuickGradient;

struct QGraphsThemeDirtyBitField
{
    bool plotAreaBackgroundColorDirty : 1;
    bool plotAreaBackgroundVisibilityDirty : 1;
    bool seriesColorsDirty : 1;
    bool seriesGradientDirty : 1;
    bool colorSchemeDirty : 1;
    bool colorStyleDirty : 1;
    bool labelFontDirty : 1;
    bool gridVisibilityDirty : 1;
    bool gridDirty : 1;
    bool labelBackgroundColorDirty : 1;
    bool labelBackgroundVisibilityDirty : 1;
    bool labelBorderVisibilityDirty : 1;
    bool labelTextColorDirty : 1;
    bool axisXDirty : 1;
    bool axisYDirty : 1;
    bool axisZDirty : 1;
    bool labelsVisibilityDirty : 1;
    bool multiHighlightColorDirty : 1;
    bool multiHighlightGradientDirty : 1;
    bool singleHighlightColorDirty : 1;
    bool singleHighlightGradientDirty : 1;
    bool themeDirty : 1;
    bool backgroundColorDirty : 1;
    bool backgroundVisibilityDirty : 1;

    QGraphsThemeDirtyBitField()
        : plotAreaBackgroundColorDirty(false)
        , plotAreaBackgroundVisibilityDirty(false)
        , seriesColorsDirty(false)
        , seriesGradientDirty(false)
        , colorSchemeDirty(false)
        , colorStyleDirty(false)
        , labelFontDirty(false)
        , gridVisibilityDirty(false)
        , gridDirty(false)
        , labelBackgroundColorDirty(false)
        , labelBackgroundVisibilityDirty(false)
        , labelBorderVisibilityDirty(false)
        , labelTextColorDirty(false)
        , axisXDirty(false)
        , axisYDirty(false)
        , axisZDirty(false)
        , labelsVisibilityDirty(false)
        , multiHighlightColorDirty(false)
        , multiHighlightGradientDirty(false)
        , singleHighlightColorDirty(false)
        , singleHighlightGradientDirty(false)
        , themeDirty(false)
        , backgroundColorDirty(false)
        , backgroundVisibilityDirty(false)
    {}
};

class Q_GRAPHS_EXPORT QGraphsLine
{
    Q_GADGET
public:
    QGraphsLine();
    QGraphsLine(const QGraphsLine &graphsLine);
    ~QGraphsLine() = default;

    QColor mainColor() const;
    void setMainColor(const QColor& newColor);
    QColor subColor() const;
    void setSubColor(const QColor& newColor);
    qreal mainWidth() const;
    void setMainWidth(qreal newWidth);
    qreal subWidth() const;
    void setSubWidth(qreal newWidth);
    QColor labelTextColor() const;
    void setLabelTextColor(const QColor& newColor);

    QGraphsLine &operator=(const QGraphsLine &);
    bool operator==(const QGraphsLine &);
    operator QVariant() const;

private:
    struct QGraphsLineCustomField
    {
        bool mainColorCustom : 1;
        bool subColorCustom : 1;
        bool labelTextColorCustom : 1;

        QGraphsLineCustomField()
            : mainColorCustom(false)
              , subColorCustom(false)
              , labelTextColorCustom(false)
        {}
    };

    void resetCustomBits();

    QGraphsLineCustomField m_bits;

    QColor m_mainColor;
    QColor m_subColor;
    qreal m_mainWidth;
    qreal m_subWidth;
    QColor m_labelTextColor;

    QColor m_mainThemeColor;
    QColor m_subThemeColor;
    QColor m_labelTextThemeColor;

    friend class QGraphsTheme;
};

class Q_GRAPHS_EXPORT QQuickGraphsLineValueType
{
    QGraphsLine v;
    Q_GADGET

    Q_PROPERTY(QColor mainColor READ mainColor WRITE setMainColor FINAL)
    Q_PROPERTY(QColor subColor READ subColor WRITE setSubColor FINAL)
    Q_PROPERTY(qreal mainWidth READ mainWidth WRITE setMainWidth FINAL)
    Q_PROPERTY(qreal subWidth READ subWidth WRITE setSubWidth FINAL)
    Q_PROPERTY(QColor labelTextColor READ labelTextColor WRITE setLabelTextColor FINAL)

    QML_VALUE_TYPE(graphsline)
    QML_FOREIGN(QGraphsLine)
    QML_EXTENDED(QQuickGraphsLineValueType)
    QML_STRUCTURED_VALUE

public:
    static QVariant create(const QJSValue &params);

    QColor mainColor() const;
    void setMainColor(const QColor& newColor);
    QColor subColor() const;
    void setSubColor(const QColor& newColor);
    qreal mainWidth() const;
    void setMainWidth(qreal newWidth);
    qreal subWidth() const;
    void setSubWidth(qreal newWidth);
    QColor labelTextColor() const;
    void setLabelTextColor(const QColor& newColor);

    operator QGraphsLine() const { return v; }
};

class Q_GRAPHS_EXPORT QGraphsTheme : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_CLASSINFO("RegisterEnumClassesUnscoped", "false")
    Q_PROPERTY(Qt::ColorScheme colorScheme READ colorScheme WRITE setColorScheme NOTIFY colorSchemeChanged FINAL)
    Q_PROPERTY(Theme theme READ theme WRITE setTheme NOTIFY themeChanged FINAL)
    Q_PROPERTY(
        ColorStyle colorStyle READ colorStyle WRITE setColorStyle NOTIFY colorStyleChanged FINAL)

    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY
                   backgroundColorChanged FINAL)
    Q_PROPERTY(bool backgroundVisible READ isBackgroundVisible WRITE setBackgroundVisible NOTIFY
                   backgroundVisibleChanged FINAL)

    Q_PROPERTY(QColor plotAreaBackgroundColor READ plotAreaBackgroundColor WRITE
                   setPlotAreaBackgroundColor NOTIFY plotAreaBackgroundColorChanged FINAL)
    Q_PROPERTY(bool plotAreaBackgroundVisible READ isPlotAreaBackgroundVisible WRITE
                   setPlotAreaBackgroundVisible NOTIFY plotAreaBackgroundVisibleChanged FINAL)

    Q_PROPERTY(
        bool gridVisible READ isGridVisible WRITE setGridVisible NOTIFY gridVisibleChanged FINAL)

    Q_PROPERTY(QFont axisXLabelFont READ axisXLabelFont WRITE setAxisXLabelFont NOTIFY axisXLabelFontChanged FINAL)
    Q_PROPERTY(QFont axisYLabelFont READ axisYLabelFont WRITE setAxisYLabelFont NOTIFY axisYLabelFontChanged FINAL)
    Q_PROPERTY(QFont axisZLabelFont READ axisZLabelFont WRITE setAxisZLabelFont NOTIFY axisZLabelFontChanged FINAL)

    Q_PROPERTY(QGraphsLine grid READ grid WRITE setGrid NOTIFY gridChanged FINAL)
    Q_PROPERTY(QGraphsLine axisX READ axisX WRITE setAxisX NOTIFY axisXChanged FINAL)
    Q_PROPERTY(QGraphsLine axisY READ axisY WRITE setAxisY NOTIFY axisYChanged FINAL)
    Q_PROPERTY(QGraphsLine axisZ READ axisZ WRITE setAxisZ NOTIFY axisZChanged FINAL)

    Q_PROPERTY(QFont labelFont READ labelFont WRITE setLabelFont NOTIFY labelFontChanged FINAL)
    Q_PROPERTY(bool labelsVisible READ labelsVisible WRITE setLabelsVisible NOTIFY
                   labelsVisibleChanged FINAL)
    Q_PROPERTY(QColor labelBackgroundColor READ labelBackgroundColor WRITE setLabelBackgroundColor
                   NOTIFY labelBackgroundColorChanged FINAL)
    Q_PROPERTY(QColor labelTextColor READ labelTextColor WRITE setLabelTextColor NOTIFY
                   labelTextColorChanged FINAL)
    Q_PROPERTY(bool labelBackgroundVisible READ isLabelBackgroundVisible WRITE
                   setLabelBackgroundVisible NOTIFY labelBackgroundVisibleChanged FINAL)
    Q_PROPERTY(bool labelBorderVisible READ isLabelBorderVisible WRITE setLabelBorderVisible NOTIFY
                   labelBorderVisibleChanged FINAL)

    Q_PROPERTY(QList<QColor> seriesColors READ seriesColors WRITE setSeriesColors NOTIFY
                   seriesColorsChanged FINAL)
    Q_PROPERTY(QList<QLinearGradient> seriesGradients READ seriesGradients WRITE setSeriesGradients
                   NOTIFY seriesGradientsChanged FINAL)
    Q_PROPERTY(QList<QColor> borderColors READ borderColors WRITE setBorderColors NOTIFY borderColorsChanged FINAL)
    Q_PROPERTY(qreal borderWidth READ borderWidth WRITE setBorderWidth NOTIFY borderWidthChanged FINAL)

    Q_PROPERTY(QQmlListProperty<QQuickGraphsColor> baseColors READ baseColorsQML CONSTANT FINAL)
    Q_PROPERTY(QQmlListProperty<QObject> baseGradients READ baseGradientsQML CONSTANT FINAL)
    Q_PROPERTY(QQmlListProperty<QObject> themeChildren READ themeChildren CONSTANT FINAL)

    Q_PROPERTY(QColor singleHighlightColor READ singleHighlightColor WRITE setSingleHighlightColor
                   NOTIFY singleHighlightColorChanged FINAL)
    Q_PROPERTY(QColor multiHighlightColor READ multiHighlightColor WRITE setMultiHighlightColor
                   NOTIFY multiHighlightColorChanged FINAL)
    Q_PROPERTY(QQuickGradient *singleHighlightGradient READ singleHighlightGradientQML WRITE
                   setSingleHighlightGradient NOTIFY singleHighlightGradientQMLChanged FINAL)
    Q_PROPERTY(QQuickGradient *multiHighlightGradient READ multiHighlightGradientQML WRITE
                   setMultiHighlightGradient NOTIFY multiHighlightGradientQMLChanged FINAL)

    Q_CLASSINFO("DefaultProperty", "themeChildren")
    QML_NAMED_ELEMENT(GraphsTheme)
public:
    enum class Theme {
        QtGreen = 0,
        QtGreenNeon,
        MixSeries,
        OrangeSeries,
        YellowSeries,
        BlueSeries,
        PurpleSeries,
        GreySeries,
        UserDefined
    };
    Q_ENUM(Theme)

    enum class ColorStyle { Uniform, ObjectGradient, RangeGradient };
    Q_ENUM(ColorStyle)

    enum class ForceTheme { No, Yes };
    Q_ENUM(ForceTheme);

    explicit QGraphsTheme(QObject *parent = nullptr);
    ~QGraphsTheme() override;

    bool themeDirty() const { return m_themeDirty; }
    void resetThemeDirty() { m_themeDirty = false; }
    void resetColorTheme();

    QGraphsThemeDirtyBitField *dirtyBits();
    void resetDirtyBits();

    Qt::ColorScheme colorScheme() const;
    void setColorScheme(Qt::ColorScheme newColorScheme);

    Theme theme() const;
    void setTheme(Theme newTheme, ForceTheme force = ForceTheme::No);

    ColorStyle colorStyle() const;
    void setColorStyle(ColorStyle newColorStyle);

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &newBackgroundColor);
    bool isBackgroundVisible() const;
    void setBackgroundVisible(bool newBackgroundVisible);

    QColor plotAreaBackgroundColor() const;
    void setPlotAreaBackgroundColor(const QColor &newBackgroundColor);
    bool isPlotAreaBackgroundVisible() const;
    void setPlotAreaBackgroundVisible(bool newBackgroundVisibility);

    bool isGridVisible() const;
    void setGridVisible(bool newGridVisibility);

    bool labelsVisible() const;
    void setLabelsVisible(bool newLabelsVisibility);
    QColor labelBackgroundColor() const;
    void setLabelBackgroundColor(const QColor &newLabelBackgroundColor);
    QColor labelTextColor() const;
    void setLabelTextColor(const QColor &newLabelTextColor);

    QColor singleHighlightColor() const;
    void setSingleHighlightColor(const QColor &newSingleHighlightColor);
    QColor multiHighlightColor() const;
    void setMultiHighlightColor(const QColor &newMultiHighlightColor);
    void setSingleHighlightGradient(const QLinearGradient &gradient);
    QLinearGradient singleHighlightGradient() const;
    void setMultiHighlightGradient(const QLinearGradient &gradient);
    QLinearGradient multiHighlightGradient() const;

    QFont labelFont() const;
    void setLabelFont(const QFont &newFont);

    QList<QColor> seriesColors() const;
    void setSeriesColors(const QList<QColor> &newSeriesColors);
    QList<QColor> borderColors() const;
    void setBorderColors(const QList<QColor> &newBorderColors);
    QList<QLinearGradient> seriesGradients() const;
    void setSeriesGradients(const QList<QLinearGradient> &newSeriesGradients);

    bool isLabelBackgroundVisible() const;
    void setLabelBackgroundVisible(bool newLabelBackgroundVisibility);

    bool isLabelBorderVisible() const;
    void setLabelBorderVisible(bool newLabelBorderVisibility);

    qreal borderWidth() const;
    void setBorderWidth(qreal newBorderWidth);

    QFont axisXLabelFont() const;
    void setAxisXLabelFont(const QFont &newAxisXLabelFont);
    QFont axisYLabelFont() const;
    void setAxisYLabelFont(const QFont &newAxisYLabelFont);
    QFont axisZLabelFont() const;
    void setAxisZLabelFont(const QFont &newAxisZLabelFont);

    QGraphsLine grid() const;
    void setGrid(const QGraphsLine &newGrid);
    QGraphsLine axisX() const;
    void setAxisX(const QGraphsLine &newAxisX);
    QGraphsLine axisY() const;
    void setAxisY(const QGraphsLine &newAxisY);
    QGraphsLine axisZ() const;
    void setAxisZ(const QGraphsLine &newAxisZ);

Q_SIGNALS:
    void update();

    void colorSchemeChanged();
    void themeChanged(QGraphsTheme::Theme theme);
    void colorStyleChanged(QGraphsTheme::ColorStyle type);

    void backgroundColorChanged();
    void backgroundVisibleChanged();

    void plotAreaBackgroundColorChanged();
    void plotAreaBackgroundVisibleChanged();

    void gridVisibleChanged();

    void labelsVisibleChanged();
    void labelBackgroundColorChanged();
    void labelTextColorChanged();

    void singleHighlightColorChanged(const QColor &color);
    void multiHighlightColorChanged(const QColor &color);
    void singleHighlightGradientChanged(const QLinearGradient &gradient);
    void multiHighlightGradientChanged(const QLinearGradient &gradient);

    void labelFontChanged();

    void labelBackgroundVisibleChanged();
    void labelBorderVisibleChanged();

    void seriesColorsChanged(const QList<QColor> &list);
    void seriesGradientsChanged(const QList<QLinearGradient> &list);
    void borderColorsChanged();
    void borderWidthChanged();

    void singleHighlightGradientQMLChanged();
    void multiHighlightGradientQMLChanged();

    void axisXLabelFontChanged();
    void axisYLabelFontChanged();
    void axisZLabelFontChanged();

    void gridChanged();
    void axisXChanged();
    void axisYChanged();
    void axisZChanged();

public Q_SLOTS:
    void handleBaseColorUpdate();
    void handleBaseGradientUpdate();

protected:
    // from QDeclarativeParserStatus
    void classBegin() override;
    void componentComplete() override;

private:
    struct QGraphsCustomBitField
    {
        bool plotAreaBackgroundColorCustom : 1;
        bool seriesColorsCustom : 1;
        bool borderColorsCustom : 1;
        bool seriesGradientCustom : 1;
        bool labelBackgroundColorCustom : 1;
        bool labelTextColorCustom : 1;
        bool multiHighlightColorCustom : 1;
        bool multiHighlightGradientCustom : 1;
        bool singleHighlightColorCustom : 1;
        bool singleHighlightGradientCustom : 1;
        bool backgroundColorCustom : 1;
        bool axisXLabelFontCustom : 1;
        bool axisYLabelFontCustom : 1;
        bool axisZLabelFontCustom : 1;

        QGraphsCustomBitField()
            : plotAreaBackgroundColorCustom(false)
              , seriesColorsCustom(false)
              , borderColorsCustom(false)
              , seriesGradientCustom(false)
              , labelBackgroundColorCustom(false)
              , labelTextColorCustom(false)
              , multiHighlightColorCustom(false)
              , multiHighlightGradientCustom(false)
              , singleHighlightColorCustom(false)
              , singleHighlightGradientCustom(false)
              , backgroundColorCustom(false)
              , axisXLabelFontCustom(false)
              , axisYLabelFontCustom(false)
              , axisZLabelFontCustom(false)
        {}
    };

    enum class GradientQMLStyle { Base, SingleHL, MultiHL };

    void setColorSchemePalette();
    void setThemePalette();
    QLinearGradient createGradient(const QColor &color, float colorLevel);

    void setSingleHighlightGradient(QQuickGradient *gradient);
    QQuickGradient *singleHighlightGradientQML() const;
    void setMultiHighlightGradient(QQuickGradient *gradient);
    QQuickGradient *multiHighlightGradientQML() const;
    void setThemeGradient(QQuickGradient *gradient, GradientQMLStyle type);
    QLinearGradient convertGradient(QJSValue gradient);
    QLinearGradient convertGradient(QQuickGradient *gradient);

    QQmlListProperty<QQuickGraphsColor> baseColorsQML();
    static void appendBaseColorsFunc(QQmlListProperty<QQuickGraphsColor> *list,
                                     QQuickGraphsColor *color);
    static qsizetype countBaseColorsFunc(QQmlListProperty<QQuickGraphsColor> *list);
    static QQuickGraphsColor *atBaseColorsFunc(QQmlListProperty<QQuickGraphsColor> *list,
                                               qsizetype index);
    static void clearBaseColorsFunc(QQmlListProperty<QQuickGraphsColor> *list);

    QQmlListProperty<QObject> baseGradientsQML();
    static void appendBaseGradientsFunc(QQmlListProperty<QObject> *list, QObject *gradient);
    static qsizetype countBaseGradientsFunc(QQmlListProperty<QObject> *list);
    static QObject *atBaseGradientsFunc(QQmlListProperty<QObject> *list, qsizetype index);
    static void clearBaseGradientsFunc(QQmlListProperty<QObject> *list);

    QQmlListProperty<QObject> themeChildren();
    static void appendThemeChildren(QQmlListProperty<QObject> *list, QObject *element);

    void addColor(QQuickGraphsColor *color);
    QList<QQuickGraphsColor *> colorList();
    void clearColors();
    void clearDummyColors();
    void clearGradients();
    QList<QQuickGradient *> gradientList();
    void addGradient(QJSValue gradient);

    QGraphsThemeDirtyBitField m_dirtyBits;
    QGraphsCustomBitField m_customBits;

    bool m_themeDirty;
    Qt::ColorScheme m_colorScheme;
    Theme m_theme;
    ColorStyle m_colorStyle;
    QColor m_plotAreaBackgroundColor;
    QColor m_plotAreaBackgroundThemeColor;
    bool m_backgroundVisibility;
    bool m_gridVisibility;
    QColor m_backgroundColor;
    QColor m_backgroundThemeColor;
    bool m_plotAreaBackgroundVisibility;
    bool m_labelsVisibility;
    QColor m_labelBackgroundColor;
    QColor m_labelBackgroundThemeColor;
    QColor m_labelTextColor;
    QColor m_labelTextThemeColor;
    bool m_labelBackgroundVisibility;
    bool m_labelBorderVisibility;
    QColor m_singleHighlightColor;
    QColor m_singleHighlightThemeColor;
    QColor m_multiHighlightColor;
    QColor m_multiHighlightThemeColor;
    QLinearGradient m_multiHighlightGradient;
    QLinearGradient m_multiHighlightThemeGradient;
    QLinearGradient m_singleHighlightGradient;
    QLinearGradient m_singleHighlightThemeGradient;
    QFont m_labelFont;
    QList<QColor> m_seriesColors;
    QList<QColor> m_seriesThemeColors;
    QList<QColor> m_borderColors;
    QList<QColor> m_borderThemeColors;
    qreal m_borderWidth;
    QList<QLinearGradient> m_seriesGradients;
    QList<QLinearGradient> m_seriesThemeGradients;

    QList<QQuickGraphsColor *> m_colors;
    QList<QQuickGradient *> m_gradients;
    QQuickGradient *m_singleHLGradient;
    QQuickGradient *m_multiHLGradient;

    QFont m_axisXLabelFont;
    QFont m_axisYLabelFont;
    QFont m_axisZLabelFont;

    QGraphsLine m_grid;
    QGraphsLine m_axisX;
    QGraphsLine m_axisY;
    QGraphsLine m_axisZ;

    bool m_dummyColors = false;

    bool m_componentComplete = false;
};

QT_END_NAMESPACE

Q_DECLARE_OPAQUE_POINTER(QQuickGradient *)

#endif
