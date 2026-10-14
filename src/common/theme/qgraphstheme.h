// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
// Qt-Security score:significant reason:default


#ifndef QTGRAPHS_QGRAPHSTHEME_H
#define QTGRAPHS_QGRAPHSTHEME_H

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
class QGraphsThemePrivate;
struct QGraphsLinePrivate;

#if QT_VERSION < QT_VERSION_CHECK(7, 0, 0)
QT_DECLARE_QESDP_SPECIALIZATION_DTOR_WITH_EXPORT(QGraphsLinePrivate, Q_GRAPHS_EXPORT)
#else
QT_DECLARE_QESDP_SPECIALIZATION_DTOR(QGraphsLinePrivate)
#endif

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

class QGraphsLine
{
    Q_GADGET_EXPORT(Q_GRAPHS_EXPORT)
    QML_VALUE_TYPE(graphsline)
    QML_STRUCTURED_VALUE

    Q_PROPERTY(QColor mainColor READ mainColor WRITE setMainColor FINAL)
    Q_PROPERTY(QColor subColor READ subColor WRITE setSubColor FINAL)
    Q_PROPERTY(qreal mainWidth READ mainWidth WRITE setMainWidth FINAL)
    Q_PROPERTY(qreal subWidth READ subWidth WRITE setSubWidth FINAL)
    Q_PROPERTY(QColor labelTextColor READ labelTextColor WRITE setLabelTextColor FINAL)

public:
    Q_GRAPHS_EXPORT static QVariant create(const QJSValue &params);
    Q_GRAPHS_EXPORT QGraphsLine();
    Q_GRAPHS_EXPORT QGraphsLine(const QGraphsLine &other);
    QGraphsLine(QGraphsLine &&other) noexcept = default;
    Q_GRAPHS_EXPORT ~QGraphsLine();
    Q_GRAPHS_EXPORT QGraphsLine &operator=(const QGraphsLine &other);
    void swap(QGraphsLine &other) noexcept { d.swap(other.d); }
    QT_MOVE_ASSIGNMENT_OPERATOR_IMPL_VIA_PURE_SWAP(QGraphsLine)

    Q_GRAPHS_EXPORT QColor mainColor() const;
    Q_GRAPHS_EXPORT void setMainColor(QColor newColor);
    Q_GRAPHS_EXPORT QColor subColor() const;
    Q_GRAPHS_EXPORT void setSubColor(QColor newColor);
    Q_GRAPHS_EXPORT qreal mainWidth() const;
    Q_GRAPHS_EXPORT void setMainWidth(qreal newWidth);
    Q_GRAPHS_EXPORT qreal subWidth() const;
    Q_GRAPHS_EXPORT void setSubWidth(qreal newWidth);
    Q_GRAPHS_EXPORT QColor labelTextColor() const;
    Q_GRAPHS_EXPORT void setLabelTextColor(QColor newColor);
    Q_GRAPHS_EXPORT void detach();

    Q_GRAPHS_EXPORT operator QVariant() const;

private:
    QExplicitlySharedDataPointer<QGraphsLinePrivate> d;

    friend Q_GRAPHS_EXPORT bool comparesEqual(const QGraphsLine &lhs,
                                              const QGraphsLine &rhs) noexcept;
    Q_DECLARE_EQUALITY_COMPARABLE(QGraphsLine)

    friend class QGraphsTheme;
};
Q_DECLARE_SHARED(QGraphsLine)

class Q_GRAPHS_EXPORT QGraphsTheme : public QObject, public QQmlParserStatus
{
    Q_OBJECT

    Q_INTERFACES(QQmlParserStatus)
    Q_CLASSINFO("RegisterEnumClassesUnscoped", "false")
    Q_PROPERTY(QGraphsTheme::ColorScheme colorScheme READ colorScheme WRITE setColorScheme NOTIFY
                   colorSchemeChanged FINAL)
    Q_PROPERTY(QGraphsTheme::Theme theme READ theme WRITE setTheme NOTIFY themeChanged FINAL)
    Q_PROPERTY(QGraphsTheme::ColorStyle colorStyle READ colorStyle WRITE setColorStyle NOTIFY
                   colorStyleChanged FINAL)

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
    Q_PROPERTY(QList<QColor> borderColors READ borderColors WRITE setBorderColors NOTIFY borderColorsChanged FINAL)
    Q_PROPERTY(qreal borderWidth READ borderWidth WRITE setBorderWidth NOTIFY borderWidthChanged FINAL)

    Q_PROPERTY(QQmlListProperty<QQuickGraphsColor> baseColors READ baseColorsQML CONSTANT FINAL)
    Q_PROPERTY(QQmlListProperty<QQuickGradient> baseGradients READ baseGradientsQML CONSTANT FINAL)
    Q_PROPERTY(QQmlListProperty<QObject> themeChildren READ themeChildren CONSTANT FINAL)

    Q_PROPERTY(QColor singleHighlightColor READ singleHighlightColor WRITE setSingleHighlightColor
                   NOTIFY singleHighlightColorChanged FINAL)
    Q_PROPERTY(QColor multiHighlightColor READ multiHighlightColor WRITE setMultiHighlightColor
                   NOTIFY multiHighlightColorChanged FINAL)
    Q_PROPERTY(QQuickGradient *singleHighlightGradient READ singleHighlightGradientQML WRITE
                   setSingleHighlightGradientQML NOTIFY singleHighlightGradientQMLChanged FINAL)
    Q_PROPERTY(QQuickGradient *multiHighlightGradient READ multiHighlightGradientQML WRITE
                   setMultiHighlightGradientQML NOTIFY multiHighlightGradientQMLChanged FINAL)
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
        UserDefined,
    };
    Q_ENUM(Theme)

    enum class ColorStyle {
        Uniform,
        ObjectGradient,
        RangeGradient,
    };
    Q_ENUM(ColorStyle)

    enum class ForceTheme { No, Yes };
    Q_ENUM(ForceTheme);

    enum class ColorScheme { Automatic, Light, Dark };
    Q_ENUM(ColorScheme);

    explicit QGraphsTheme(QObject *parent = nullptr);

    ~QGraphsTheme() override;

    bool themeDirty() const;
    void resetThemeDirty();
    void resetColorTheme();

    QGraphsThemeDirtyBitField *dirtyBits();
    void resetDirtyBits();

    ColorScheme colorScheme() const;

    Theme theme() const;

    ColorStyle colorStyle() const;

    QColor backgroundColor() const;
    bool isBackgroundVisible() const;

    QColor plotAreaBackgroundColor() const;
    bool isPlotAreaBackgroundVisible() const;

    bool isGridVisible() const;

    bool labelsVisible() const;
    QColor labelBackgroundColor() const;
    QColor labelTextColor() const;

    QColor singleHighlightColor() const;
    QColor multiHighlightColor() const;
    QLinearGradient singleHighlightGradient() const;
    QLinearGradient multiHighlightGradient() const;

    QFont labelFont() const;

    QList<QColor> seriesColors() const;
    QList<QColor> borderColors() const;
    QList<QLinearGradient> seriesGradients() const;

    bool isLabelBackgroundVisible() const;

    bool isLabelBorderVisible() const;

    qreal borderWidth() const;

    QFont axisXLabelFont() const;
    QFont axisYLabelFont() const;
    QFont axisZLabelFont() const;

    QGraphsLine grid() const;
    QGraphsLine axisX() const;
    QGraphsLine axisY() const;
    QGraphsLine axisZ() const;

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

    void singleHighlightColorChanged(QColor color);
    void multiHighlightColorChanged(QColor color);
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
    void setColorScheme(ColorScheme newColorScheme);
    void setTheme(Theme newTheme, ForceTheme force = ForceTheme::No);
    void setColorStyle(ColorStyle newColorStyle);
    void setBackgroundColor(QColor newBackgroundColor);
    void setBackgroundVisible(bool newBackgroundVisible);
    void setPlotAreaBackgroundColor(QColor newBackgroundColor);
    void setPlotAreaBackgroundVisible(bool newBackgroundVisibility);
    void setGridVisible(bool newGridVisibility);
    void setLabelsVisible(bool newLabelsVisibility);
    void setLabelBackgroundColor(QColor newLabelBackgroundColor);
    void setLabelTextColor(QColor newLabelTextColor);
    void setSingleHighlightColor(QColor newSingleHighlightColor);
    void setMultiHighlightColor(QColor newMultiHighlightColor);
    void setSingleHighlightGradient(const QLinearGradient &gradient);
    void setMultiHighlightGradient(const QLinearGradient &gradient);
    void setLabelFont(const QFont &newFont);
    void setSeriesColors(const QList<QColor> &newSeriesColors);
    void setBorderColors(const QList<QColor> &newBorderColors);
    void setSeriesGradients(const QList<QLinearGradient> &newSeriesGradients);
    void setLabelBackgroundVisible(bool newLabelBackgroundVisibility);
    void setLabelBorderVisible(bool newLabelBorderVisibility);
    void setBorderWidth(qreal newBorderWidth);
    void setAxisXLabelFont(const QFont &newAxisXLabelFont);
    void setAxisYLabelFont(const QFont &newAxisYLabelFont);
    void setAxisZLabelFont(const QFont &newAxisZLabelFont);
    void setGrid(const QGraphsLine &newGrid);
    void setAxisX(const QGraphsLine &newAxisX);
    void setAxisY(const QGraphsLine &newAxisY);
    void setAxisZ(const QGraphsLine &newAxisZ);

protected:
    explicit QGraphsTheme(QGraphsThemePrivate &dd, QObject *parent = nullptr);
    // from QDeclarativeParserStatus
    void classBegin() override;
    void componentComplete() override;

private:
    enum class GradientQMLStyle {
        Base,
        SingleHL,
        MultiHL,
    };

    void updateAutomaticColorScheme();
    void setColorSchemePalette();
    void setThemePalette();
    QLinearGradient createGradient(QColor color, float colorLevel);

    void setThemeGradient(QQuickGradient *gradient, GradientQMLStyle type);
    QLinearGradient convertGradient(QQuickGradient *gradient);

    QQmlListProperty<QQuickGraphsColor> baseColorsQML();
    static void appendBaseColorsFunc(QQmlListProperty<QQuickGraphsColor> *list,
                                     QQuickGraphsColor *color);
    static qsizetype countBaseColorsFunc(QQmlListProperty<QQuickGraphsColor> *list);
    static QQuickGraphsColor *atBaseColorsFunc(QQmlListProperty<QQuickGraphsColor> *list,
                                               qsizetype index);
    static void clearBaseColorsFunc(QQmlListProperty<QQuickGraphsColor> *list);

    QQmlListProperty<QQuickGradient> baseGradientsQML();
    static void appendBaseGradientsFunc(QQmlListProperty<QQuickGradient> *list,
                                        QQuickGradient *gradient);
    static qsizetype countBaseGradientsFunc(QQmlListProperty<QQuickGradient> *list);
    static QQuickGradient *atBaseGradientsFunc(QQmlListProperty<QQuickGradient> *list,
                                               qsizetype index);
    static void clearBaseGradientsFunc(QQmlListProperty<QQuickGradient> *list);

    QQmlListProperty<QObject> themeChildren();
    static void appendThemeChildren(QQmlListProperty<QObject> *list, QObject *element);

    void addColor(QQuickGraphsColor *color);
    QList<QQuickGraphsColor *> colorList();
    void clearColors();
    void clearDummyColors();
    void clearGradients();
    QList<QQuickGradient *> gradientList();
    void addGradient(QQuickGradient *gradient);

    void setSingleHighlightGradientQML(QQuickGradient *gradient);
    QQuickGradient *singleHighlightGradientQML() const;

    void setMultiHighlightGradientQML(QQuickGradient *gradient);
    QQuickGradient *multiHighlightGradientQML() const;

    Q_DISABLE_COPY_MOVE(QGraphsTheme)
    Q_DECLARE_PRIVATE(QGraphsTheme)
};

QT_END_NAMESPACE

#endif
