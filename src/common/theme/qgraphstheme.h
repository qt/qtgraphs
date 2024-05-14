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
    bool plotAreaBackgroundEnabledDirty : 1;
    bool seriesColorsDirty : 1;
    bool seriesGradientDirty : 1;
    bool colorSchemeDirty : 1;
    bool colorStyleDirty : 1;
    bool labelFontDirty : 1;
    bool gridEnabledDirty : 1;
    bool gridMainColorDirty : 1;
    bool gridSubColorDirty : 1;
    bool gridMainWidthDirty : 1;
    bool gridSubWidthDirty : 1;
    bool labelBackgroundColorDirty : 1;
    bool labelBackgroundEnabledDirty : 1;
    bool labelBorderEnabledDirty : 1;
    bool labelTextColorDirty : 1;
    bool axisXLabelColorDirty : 1;
    bool axisYLabelColorDirty : 1;
    bool axisZLabelColorDirty : 1;
    bool labelsEnabledDirty : 1;
    bool multiHighlightColorDirty : 1;
    bool multiHighlightGradientDirty : 1;
    bool singleHighlightColorDirty : 1;
    bool singleHighlightGradientDirty : 1;
    bool themeDirty : 1;
    bool backgroundColorDirty : 1;
    bool backgroundEnabledDirty : 1;

    QGraphsThemeDirtyBitField()
        : plotAreaBackgroundColorDirty(false)
          , plotAreaBackgroundEnabledDirty(false)
          , seriesColorsDirty(false)
          , seriesGradientDirty(false)
          , colorSchemeDirty(false)
          , colorStyleDirty(false)
          , labelFontDirty(false)
          , gridEnabledDirty(false)
          , gridMainColorDirty(false)
          , gridSubColorDirty(false)
          , gridMainWidthDirty(false)
          , gridSubWidthDirty(false)
          , labelBackgroundColorDirty(false)
          , labelBackgroundEnabledDirty(false)
          , labelBorderEnabledDirty(false)
          , labelTextColorDirty(false)
          , axisXLabelColorDirty(false)
          , axisYLabelColorDirty(false)
          , axisZLabelColorDirty(false)
          , labelsEnabledDirty(false)
          , multiHighlightColorDirty(false)
          , multiHighlightGradientDirty(false)
          , singleHighlightColorDirty(false)
          , singleHighlightGradientDirty(false)
          , themeDirty(false)
          , backgroundColorDirty(false)
          , backgroundEnabledDirty(false)
    {}
};

class Q_GRAPHS_EXPORT QGraphsTheme : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_CLASSINFO("RegisterEnumClassesUnscoped", "false")
    Q_PROPERTY(Qt::ColorScheme colorScheme READ colorScheme WRITE setColorScheme NOTIFY colorSchemeChanged FINAL)
    Q_PROPERTY(Theme theme READ theme WRITE setTheme NOTIFY themeChanged FINAL)
    Q_PROPERTY(ColorStyle colorStyle READ colorStyle WRITE setColorStyle NOTIFY
                       colorStyleChanged)

    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
    Q_PROPERTY(bool backgroundEnabled READ isBackgroundEnabled WRITE setBackgroundEnabled NOTIFY
                       backgroundEnabledChanged)

    Q_PROPERTY(QColor plotAreaBackgroundColor READ plotAreaBackgroundColor WRITE setPlotAreaBackgroundColor NOTIFY
                       plotAreaBackgroundColorChanged)
    Q_PROPERTY(bool plotAreaBackgroundEnabled READ isPlotAreaBackgroundEnabled WRITE setPlotAreaBackgroundEnabled NOTIFY
                       plotAreaBackgroundEnabledChanged)

    Q_PROPERTY(bool gridEnabled READ isGridEnabled WRITE setGridEnabled NOTIFY gridEnabledChanged)
    Q_PROPERTY(QColor gridMainColor READ gridMainColor WRITE setGridMainColor NOTIFY
                       gridMainColorChanged)
    Q_PROPERTY(float gridMainWidth READ gridMainWidth WRITE setGridMainWidth NOTIFY
                       gridMainWidthChanged)
    Q_PROPERTY(QColor gridSubColor READ gridSubColor WRITE setGridSubColor NOTIFY
                       gridSubColorChanged)
    Q_PROPERTY(float gridSubWidth READ gridSubWidth WRITE setGridSubWidth NOTIFY
                       gridSubWidthChanged)

    Q_PROPERTY(QColor axisXMainColor READ axisXMainColor WRITE setAxisXMainColor NOTIFY axisXMainColorChanged FINAL)
    Q_PROPERTY(float axisXMainWidth READ axisXMainWidth WRITE setAxisXMainWidth NOTIFY axisXMainWidthChanged FINAL)
    Q_PROPERTY(QColor axisXSubColor READ axisXSubColor WRITE setAxisXSubColor NOTIFY axisXSubColorChanged FINAL)
    Q_PROPERTY(float axisXSubWidth READ axisXSubWidth WRITE setAxisXSubWidth NOTIFY axisXSubWidthChanged FINAL)
    Q_PROPERTY(QFont axisXLabelFont READ axisXLabelFont WRITE setAxisXLabelFont NOTIFY axisXLabelFontChanged FINAL)
    Q_PROPERTY(QColor axisXLabelColor READ axisXLabelColor WRITE setAxisXLabelColor NOTIFY axisXLabelColorChanged FINAL)

    Q_PROPERTY(QColor axisYMainColor READ axisYMainColor WRITE setAxisYMainColor NOTIFY axisYMainColorChanged FINAL)
    Q_PROPERTY(float axisYMainWidth READ axisYMainWidth WRITE setAxisYMainWidth NOTIFY axisYMainWidthChanged FINAL)
    Q_PROPERTY(QColor axisYSubColor READ axisYSubColor WRITE setAxisYSubColor NOTIFY axisYSubColorChanged FINAL)
    Q_PROPERTY(float axisYSubWidth READ axisYSubWidth WRITE setAxisYSubWidth NOTIFY axisYSubWidthChanged FINAL)
    Q_PROPERTY(QFont axisYLabelFont READ axisYLabelFont WRITE setAxisYLabelFont NOTIFY axisYLabelFontChanged FINAL)
    Q_PROPERTY(QColor axisYLabelColor READ axisYLabelColor WRITE setAxisYLabelColor NOTIFY axisYLabelColorChanged FINAL)

    Q_PROPERTY(QColor axisZMainColor READ axisZMainColor WRITE setAxisZMainColor NOTIFY axisZMainColorChanged FINAL)
    Q_PROPERTY(float axisZMainWidth READ axisZMainWidth WRITE setAxisZMainWidth NOTIFY axisZMainWidthChanged FINAL)
    Q_PROPERTY(QColor axisZSubColor READ axisZSubColor WRITE setAxisZSubColor NOTIFY axisZSubColorChanged FINAL)
    Q_PROPERTY(float axisZSubWidth READ axisZSubWidth WRITE setAxisZSubWidth NOTIFY axisZSubWidthChanged FINAL)
    Q_PROPERTY(QFont axisZLabelFont READ axisZLabelFont WRITE setAxisZLabelFont NOTIFY axisZLabelFontChanged FINAL)
    Q_PROPERTY(QColor axisZLabelColor READ axisZLabelColor WRITE setAxisZLabelColor NOTIFY axisZLabelColorChanged FINAL)

    Q_PROPERTY(QFont labelFont READ labelFont WRITE setLabelFont NOTIFY labelFontChanged)
    Q_PROPERTY(bool labelsEnabled READ isLabelsEnabled WRITE setLabelsEnabled NOTIFY labelsEnabledChanged)
    Q_PROPERTY(QColor labelBackgroundColor READ labelBackgroundColor WRITE setLabelBackgroundColor NOTIFY labelBackgroundColorChanged)
    Q_PROPERTY(QColor labelTextColor READ labelTextColor WRITE setLabelTextColor NOTIFY labelTextColorChanged)
    Q_PROPERTY(bool labelBackgroundEnabled READ isLabelBackgroundEnabled WRITE setLabelBackgroundEnabled NOTIFY labelBackgroundEnabledChanged)
    Q_PROPERTY(bool labelBorderEnabled READ isLabelBorderEnabled WRITE setLabelBorderEnabled NOTIFY labelBorderEnabledChanged)

    Q_PROPERTY(QList<QColor> seriesColors READ seriesColors WRITE setSeriesColors NOTIFY seriesColorsChanged)
    Q_PROPERTY(QList<QLinearGradient> seriesGradients READ seriesGradients WRITE setSeriesGradients NOTIFY seriesGradientsChanged)
    Q_PROPERTY(QList<QColor> borderColors READ borderColors WRITE setBorderColors NOTIFY borderColorsChanged FINAL)
    Q_PROPERTY(qreal borderWidth READ borderWidth WRITE setBorderWidth NOTIFY borderWidthChanged FINAL)

    Q_PROPERTY(QQmlListProperty<QQuickGraphsColor> baseColors READ baseColorsQML CONSTANT)
    Q_PROPERTY(QQmlListProperty<QObject> baseGradients READ baseGradientsQML CONSTANT)
    Q_PROPERTY(QQmlListProperty<QObject> themeChildren READ themeChildren CONSTANT)

    Q_PROPERTY(QColor singleHighlightColor READ singleHighlightColor WRITE setSingleHighlightColor NOTIFY singleHighlightColorChanged)
    Q_PROPERTY(QColor multiHighlightColor READ multiHighlightColor WRITE setMultiHighlightColor NOTIFY multiHighlightColorChanged)
    Q_PROPERTY(QJSValue singleHighlightGradient READ singleHighlightGradientQML WRITE setSingleHighlightGradient NOTIFY singleHighlightGradientQMLChanged)
    Q_PROPERTY(QJSValue multiHighlightGradient READ multiHighlightGradientQML WRITE setMultiHighlightGradient NOTIFY multiHighlightGradientQMLChanged)

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
    bool isBackgroundEnabled() const;
    void setBackgroundEnabled(bool newBackgroundEnabled);

    QColor plotAreaBackgroundColor() const;
    void setPlotAreaBackgroundColor(const QColor &newBackgroundColor);
    bool isPlotAreaBackgroundEnabled() const;
    void setPlotAreaBackgroundEnabled(bool newBackgroundEnabled);

    bool isGridEnabled() const;
    void setGridEnabled(bool newGridEnabled);
    QColor gridMainColor() const;
    void setGridMainColor(const QColor &newgridMainColor);
    float gridMainWidth() const;
    void setGridMainWidth(float newgridMainWidth);
    QColor gridSubColor() const;
    void setGridSubColor(const QColor &newgridSubColor);
    float gridSubWidth() const;
    void setGridSubWidth(float newgridSubWidth);

    bool isLabelsEnabled() const;
    void setLabelsEnabled(bool newLabelsEnabled);
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

    bool isLabelBackgroundEnabled() const;
    void setLabelBackgroundEnabled(bool newLabelBackgroundEnabled);

    bool isLabelBorderEnabled() const;
    void setLabelBorderEnabled(bool newLabelBorderEnabled);

    qreal borderWidth() const;
    void setBorderWidth(qreal newBorderWidth);

    QColor axisXMainColor() const;
    void setAxisXMainColor(const QColor &newAxisXMainColor);
    float axisXMainWidth() const;
    void setAxisXMainWidth(float newAxisXMainWidth);
    QColor axisXSubColor() const;
    void setAxisXSubColor(const QColor &newAxisXSubColor);
    float axisXSubWidth() const;
    void setAxisXSubWidth(float newAxisXSubWidth);

    QColor axisYMainColor() const;
    void setAxisYMainColor(const QColor &newAxisYMainColor);
    float axisYMainWidth() const;
    void setAxisYMainWidth(float newAxisYMainWidth);
    QColor axisYSubColor() const;
    void setAxisYSubColor(const QColor &newAxisYSubColor);
    float axisYSubWidth() const;
    void setAxisYSubWidth(float newAxisYSubWidth);

    QColor axisZMainColor() const;
    void setAxisZMainColor(const QColor &newAxisZMainColor);
    float axisZMainWidth() const;
    void setAxisZMainWidth(float newAxisZMainWidth);
    QColor axisZSubColor() const;
    void setAxisZSubColor(const QColor &newAxisZSubColor);
    float axisZSubWidth() const;
    void setAxisZSubWidth(float newAxisZSubWidth);

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
    void backgroundEnabledChanged();

    void plotAreaBackgroundColorChanged();
    void plotAreaBackgroundEnabledChanged();

    void gridEnabledChanged();
    void gridMainColorChanged();
    void gridMainWidthChanged();
    void gridSubColorChanged();
    void gridSubWidthChanged();

    void labelsEnabledChanged();
    void labelBackgroundColorChanged();
    void labelTextColorChanged();

    void singleHighlightColorChanged();
    void multiHighlightColorChanged();
    void singleHighlightGradientChanged();
    void multiHighlightGradientChanged();

    void labelFontChanged();

    void labelBackgroundEnabledChanged();
    void labelBorderEnabledChanged();

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
    float m_gridMainWidth;
    QColor m_gridSubColor;
    QColor m_gridSubThemeColor;
    float m_gridSubWidth;
    QColor m_plotAreaBackgroundColor;
    QColor m_plotAreaBackgroundThemeColor;
    bool m_backgroundEnabled;
    bool m_gridEnabled;
    QColor m_backgroundColor;
    QColor m_backgroundThemeColor;
    bool m_plotAreaBackgroundEnabled;
    bool m_labelsEnabled;
    QColor m_labelBackgroundColor;
    QColor m_labelBackgroundThemeColor;
    QColor m_labelTextColor;
    QColor m_labelTextThemeColor;
    bool m_labelBackgroundEnabled;
    bool m_labelBorderEnabled;
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
    float m_axisXMainWidth;
    QColor m_axisXSubColor;
    QColor m_axisXSubThemeColor;
    float m_axisXSubWidth;
    QColor m_axisYMainColor;
    QColor m_axisYMainThemeColor;
    float m_axisYMainWidth;
    QColor m_axisYSubColor;
    QColor m_axisYSubThemeColor;
    float m_axisYSubWidth;
    QColor m_axisZMainColor;
    QColor m_axisZMainThemeColor;
    float m_axisZMainWidth;
    QColor m_axisZSubColor;
    QColor m_axisZSubThemeColor;
    float m_axisZSubWidth;
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
