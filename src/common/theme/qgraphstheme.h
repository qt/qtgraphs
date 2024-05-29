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
    bool gridMainColorDirty : 1;
    bool gridSubColorDirty : 1;
    bool gridMainWidthDirty : 1;
    bool gridSubWidthDirty : 1;
    bool labelBackgroundColorDirty : 1;
    bool labelBackgroundVisibilityDirty : 1;
    bool labelBorderVisibilityDirty : 1;
    bool labelTextColorDirty : 1;
    bool axisXLabelColorDirty : 1;
    bool axisYLabelColorDirty : 1;
    bool axisZLabelColorDirty : 1;
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
        , gridMainColorDirty(false)
        , gridSubColorDirty(false)
        , gridMainWidthDirty(false)
        , gridSubWidthDirty(false)
        , labelBackgroundColorDirty(false)
        , labelBackgroundVisibilityDirty(false)
        , labelBorderVisibilityDirty(false)
        , labelTextColorDirty(false)
        , axisXLabelColorDirty(false)
        , axisYLabelColorDirty(false)
        , axisZLabelColorDirty(false)
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
    Q_PROPERTY(QColor gridMainColor READ gridMainColor WRITE setGridMainColor NOTIFY
                   gridMainColorChanged FINAL)
    Q_PROPERTY(qreal gridMainWidth READ gridMainWidth WRITE setGridMainWidth NOTIFY
                   gridMainWidthChanged FINAL)
    Q_PROPERTY(QColor gridSubColor READ gridSubColor WRITE setGridSubColor NOTIFY
                   gridSubColorChanged FINAL)
    Q_PROPERTY(
        qreal gridSubWidth READ gridSubWidth WRITE setGridSubWidth NOTIFY gridSubWidthChanged FINAL)

    Q_PROPERTY(QColor axisXMainColor READ axisXMainColor WRITE setAxisXMainColor NOTIFY axisXMainColorChanged FINAL)
    Q_PROPERTY(qreal axisXMainWidth READ axisXMainWidth WRITE setAxisXMainWidth NOTIFY axisXMainWidthChanged FINAL)
    Q_PROPERTY(QColor axisXSubColor READ axisXSubColor WRITE setAxisXSubColor NOTIFY axisXSubColorChanged FINAL)
    Q_PROPERTY(qreal axisXSubWidth READ axisXSubWidth WRITE setAxisXSubWidth NOTIFY axisXSubWidthChanged FINAL)
    Q_PROPERTY(QFont axisXLabelFont READ axisXLabelFont WRITE setAxisXLabelFont NOTIFY axisXLabelFontChanged FINAL)
    Q_PROPERTY(QColor axisXLabelColor READ axisXLabelColor WRITE setAxisXLabelColor NOTIFY axisXLabelColorChanged FINAL)

    Q_PROPERTY(QColor axisYMainColor READ axisYMainColor WRITE setAxisYMainColor NOTIFY axisYMainColorChanged FINAL)
    Q_PROPERTY(qreal axisYMainWidth READ axisYMainWidth WRITE setAxisYMainWidth NOTIFY axisYMainWidthChanged FINAL)
    Q_PROPERTY(QColor axisYSubColor READ axisYSubColor WRITE setAxisYSubColor NOTIFY axisYSubColorChanged FINAL)
    Q_PROPERTY(qreal axisYSubWidth READ axisYSubWidth WRITE setAxisYSubWidth NOTIFY axisYSubWidthChanged FINAL)
    Q_PROPERTY(QFont axisYLabelFont READ axisYLabelFont WRITE setAxisYLabelFont NOTIFY axisYLabelFontChanged FINAL)
    Q_PROPERTY(QColor axisYLabelColor READ axisYLabelColor WRITE setAxisYLabelColor NOTIFY axisYLabelColorChanged FINAL)

    Q_PROPERTY(QColor axisZMainColor READ axisZMainColor WRITE setAxisZMainColor NOTIFY axisZMainColorChanged FINAL)
    Q_PROPERTY(qreal axisZMainWidth READ axisZMainWidth WRITE setAxisZMainWidth NOTIFY axisZMainWidthChanged FINAL)
    Q_PROPERTY(QColor axisZSubColor READ axisZSubColor WRITE setAxisZSubColor NOTIFY axisZSubColorChanged FINAL)
    Q_PROPERTY(qreal axisZSubWidth READ axisZSubWidth WRITE setAxisZSubWidth NOTIFY axisZSubWidthChanged FINAL)
    Q_PROPERTY(QFont axisZLabelFont READ axisZLabelFont WRITE setAxisZLabelFont NOTIFY axisZLabelFontChanged FINAL)
    Q_PROPERTY(QColor axisZLabelColor READ axisZLabelColor WRITE setAxisZLabelColor NOTIFY axisZLabelColorChanged FINAL)

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
    Q_PROPERTY(QJSValue singleHighlightGradient READ singleHighlightGradientQML WRITE
                   setSingleHighlightGradient NOTIFY singleHighlightGradientQMLChanged FINAL)
    Q_PROPERTY(QJSValue multiHighlightGradient READ multiHighlightGradientQML WRITE
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
    void setTheme(Theme newTheme, bool force = false);

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
    QColor gridMainColor() const;
    void setGridMainColor(const QColor &newgridMainColor);
    qreal gridMainWidth() const;
    void setGridMainWidth(qreal newgridMainWidth);
    QColor gridSubColor() const;
    void setGridSubColor(const QColor &newgridSubColor);
    qreal gridSubWidth() const;
    void setGridSubWidth(qreal newgridSubWidth);

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

    QColor axisXMainColor() const;
    void setAxisXMainColor(const QColor &newAxisXMainColor);
    qreal axisXMainWidth() const;
    void setAxisXMainWidth(qreal newAxisXMainWidth);
    QColor axisXSubColor() const;
    void setAxisXSubColor(const QColor &newAxisXSubColor);
    qreal axisXSubWidth() const;
    void setAxisXSubWidth(qreal newAxisXSubWidth);

    QColor axisYMainColor() const;
    void setAxisYMainColor(const QColor &newAxisYMainColor);
    qreal axisYMainWidth() const;
    void setAxisYMainWidth(qreal newAxisYMainWidth);
    QColor axisYSubColor() const;
    void setAxisYSubColor(const QColor &newAxisYSubColor);
    qreal axisYSubWidth() const;
    void setAxisYSubWidth(qreal newAxisYSubWidth);

    QColor axisZMainColor() const;
    void setAxisZMainColor(const QColor &newAxisZMainColor);
    qreal axisZMainWidth() const;
    void setAxisZMainWidth(qreal newAxisZMainWidth);
    QColor axisZSubColor() const;
    void setAxisZSubColor(const QColor &newAxisZSubColor);
    qreal axisZSubWidth() const;
    void setAxisZSubWidth(qreal newAxisZSubWidth);

    QFont axisXLabelFont() const;
    void setAxisXLabelFont(const QFont &newAxisXLabelFont);

    QFont axisYLabelFont() const;
    void setAxisYLabelFont(const QFont &newAxisYLabelFont);

    QFont axisZLabelFont() const;
    void setAxisZLabelFont(const QFont &newAxisZLabelFont);

    QColor axisXLabelColor() const;
    void setAxisXLabelColor(const QColor &newAxisXLabelColor);

    QColor axisYLabelColor() const;
    void setAxisYLabelColor(const QColor &newAxisYLabelColor);

    QColor axisZLabelColor() const;
    void setAxisZLabelColor(const QColor &newAxisZLabelColor);

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
    void gridMainColorChanged();
    void gridMainWidthChanged();
    void gridSubColorChanged();
    void gridSubWidthChanged();

    void labelsVisibleChanged();
    void labelBackgroundColorChanged();
    void labelTextColorChanged();

    void singleHighlightColorChanged();
    void multiHighlightColorChanged();
    void singleHighlightGradientChanged();
    void multiHighlightGradientChanged();

    void labelFontChanged();

    void labelBackgroundVisibleChanged();
    void labelBorderVisibleChanged();

    void seriesColorsChanged(const QList<QColor> &list);
    void seriesGradientsChanged(const QList<QLinearGradient> &list);
    void borderColorsChanged();
    void borderWidthChanged();

    void singleHighlightGradientQMLChanged();
    void multiHighlightGradientQMLChanged();

    void axisXMainColorChanged();
    void axisXMainWidthChanged();
    void axisXSubColorChanged();
    void axisXSubWidthChanged();

    void axisYMainColorChanged();
    void axisYMainWidthChanged();
    void axisYSubColorChanged();
    void axisYSubWidthChanged();

    void axisZMainColorChanged();
    void axisZMainWidthChanged();
    void axisZSubColorChanged();
    void axisZSubWidthChanged();

    void axisXLabelFontChanged();
    void axisYLabelFontChanged();
    void axisZLabelFontChanged();

    void axisXLabelColorChanged();
    void axisYLabelColorChanged();
    void axisZLabelColorChanged();

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
        bool gridMainColorCustom : 1;
        bool gridSubColorCustom : 1;
        bool labelBackgroundColorCustom : 1;
        bool labelTextColorCustom : 1;
        bool axisXLabelColorCustom : 1;
        bool axisYLabelColorCustom : 1;
        bool axisZLabelColorCustom : 1;
        bool multiHighlightColorCustom : 1;
        bool multiHighlightGradientCustom : 1;
        bool singleHighlightColorCustom : 1;
        bool singleHighlightGradientCustom : 1;
        bool backgroundColorCustom : 1;
        bool axisXMainColorCustom : 1;
        bool axisXSubColorCustom : 1;
        bool axisYMainColorCustom : 1;
        bool axisYSubColorCustom : 1;
        bool axisZMainColorCustom : 1;
        bool axisZSubColorCustom : 1;

        QGraphsCustomBitField()
            : plotAreaBackgroundColorCustom(false)
              , seriesColorsCustom(false)
              , borderColorsCustom(false)
              , seriesGradientCustom(false)
              , gridMainColorCustom(false)
              , gridSubColorCustom(false)
              , labelBackgroundColorCustom(false)
              , labelTextColorCustom(false)
              , axisXLabelColorCustom(false)
              , axisYLabelColorCustom(false)
              , axisZLabelColorCustom(false)
              , multiHighlightColorCustom(false)
              , multiHighlightGradientCustom(false)
              , singleHighlightColorCustom(false)
              , singleHighlightGradientCustom(false)
              , backgroundColorCustom(false)
              , axisXMainColorCustom(false)
              , axisXSubColorCustom(false)
              , axisYMainColorCustom(false)
              , axisYSubColorCustom(false)
              , axisZMainColorCustom(false)
              , axisZSubColorCustom(false)
        {}
    };

    enum class GradientQMLStyle { Base, SingleHL, MultiHL };

    void setColorSchemePalette();
    void setThemePalette();
    QLinearGradient createGradient(const QColor &color, float colorLevel);

    void setSingleHighlightGradient(QJSValue gradient);
    QJSValue singleHighlightGradientQML() const;
    void setMultiHighlightGradient(QJSValue gradient);
    QJSValue multiHighlightGradientQML() const;
    void setThemeGradient(QJSValue gradient, GradientQMLStyle type);
    QLinearGradient convertGradient(QJSValue gradient);

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
    QColor m_gridMainColor;
    QColor m_gridMainThemeColor;
    qreal m_gridMainWidth;
    QColor m_gridSubColor;
    QColor m_gridSubThemeColor;
    qreal m_gridSubWidth;
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
    QJSValue m_singleHLGradient;
    QJSValue m_multiHLGradient;

    bool m_dummyColors = false;

    bool m_componentComplete = false;
    QColor m_axisXMainColor;
    QColor m_axisXMainThemeColor;
    qreal m_axisXMainWidth;
    QColor m_axisXSubColor;
    QColor m_axisXSubThemeColor;
    qreal m_axisXSubWidth;
    QColor m_axisYMainColor;
    QColor m_axisYMainThemeColor;
    qreal m_axisYMainWidth;
    QColor m_axisYSubColor;
    QColor m_axisYSubThemeColor;
    qreal m_axisYSubWidth;
    QColor m_axisZMainColor;
    QColor m_axisZMainThemeColor;
    qreal m_axisZMainWidth;
    QColor m_axisZSubColor;
    QColor m_axisZSubThemeColor;
    qreal m_axisZSubWidth;
    QFont m_axisXLabelFont;
    QFont m_axisYLabelFont;
    QFont m_axisZLabelFont;
    QColor m_axisXLabelColor;
    QColor m_axisXLabelThemeColor;
    QColor m_axisYLabelColor;
    QColor m_axisYLabelThemeColor;
    QColor m_axisZLabelColor;
    QColor m_axisZLabelThemeColor;
};

QT_END_NAMESPACE

#endif
