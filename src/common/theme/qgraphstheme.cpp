// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "qgraphstheme.h"

#include <QLinearGradient>
#include <QGuiApplication>
#include <QStyleHints>
#include "qquickgraphscolor_p.h"
#include <private/graphsglobal_p.h>
#include <QtQuick/private/qquickrectangle_p.h>
#include <QQmlListProperty>

QT_BEGIN_NAMESPACE

/*!
 * \class QGraphsTheme
 * \inmodule QtGraphs
 * \ingroup graphs
 * \brief QGraphsTheme class provides a visual style for graphs.
 *
 * Specifies visual properties that affect the whole graph. There are several
 * built-in themes that can be used as is or modified freely.
 *
 * Themes can be created from scratch using the UserDefined enum value.
 * Creating a theme using the default constructor produces a new user-defined
 * theme.
 *
 * \section1 Default Theme
 *
 * The following table lists the properties controlled by themes and the
 * default values for UserDefined.
 *
 * \table
 *   \header
 *     \li Property
 *     \li Default Value
 *   \row
 *     \li backgroundVisible
 *     \li \c true
 *   \row
 *     \li seriesColors
 *     \li Qt::black
 *   \row
 *     \li seriesGradients
 *     \li QLinearGradient. Essentially fully black.
 *   \row
 *     \li colorStyle
 *     \li Uniform
 *   \row
 *     \li \l labelFont
 *     \li QFont
 *   \row
 *     \li gridVisible
 *     \li \c true
 *   \row
 *     \li labelBackgroundVisible
 *     \li \c true
 *   \row
 *     \li labelBorderVisible
 *     \li \c true
 *   \row
 *     \li labelsVisible
 *     \li \c true
 * \endtable
 *
 * \section1 Usage Examples
 *
 * Creating a built-in theme without any modifications:
 *
 * \snippet doc_src_qgraphstheme.cpp 0
 *
 * Creating a built-in theme and modifying some properties:
 *
 * \snippet doc_src_qgraphstheme.cpp 1
 *
 * Creating a user-defined theme:
 *
 * \snippet doc_src_qgraphstheme.cpp 2
 *
 * Creating a built-in theme and modifying some properties after it has been
 * set:
 *
 * \snippet doc_src_qgraphstheme.cpp 3
 *
 */

/*!
 * \enum QGraphsTheme::ColorStyle
 *
 * Gradient types.
 *
 * \value Uniform
 *        Objects are rendered in a single color. The color used is specified in seriesColors,
 *        singleHighlightColor and multiHighlightColor properties.
 * \value ObjectGradient
 *        Objects are colored using a full gradient for each object regardless of object height. The
 *        gradient used is specified in seriesGradients, singleHighlightGradient and
 *        multiHighlightGradient properties.
 * \value RangeGradient
 *        Objects are colored using a portion of the full gradient determined by the object's
 *        height and its position on the Y-axis. The gradient used is specified in seriesGradients,
 *        singleHighlightGradient and multiHighlightGradient properties.
 */

/*!
 * \enum QGraphsTheme::Theme
 *
 * Built-in themes.
 *
 * \value QtGreen
 *        A light theme with green as the base color.
 * \value QtGreenNeon
 *        A light theme with green neon as the base color.
 * \value MixSeries
 *        A mixed theme with various colors.
 * \value OrangeSeries
 *        A theme with Orange as the base color.
 * \value YellowSeries
 *        A theme with Yellow as the base color.
 * \value BlueSeries
 *        A theme with Blue as the base color.
 * \value PurpleSeries
 *        A theme with Purple as the base color.
 * \value GreySeries
 *        A theme with Grey as the base color.
 * \value UserDefined
 *        A user-defined theme. For more information, see \l {Default Theme}.
 */

/*!
 * \qmlvaluetype graphsline
 * \ingroup graphs_qml
 * \brief a values for lines based on properties of QGraphsLine.
 *
 * The \c graphsline type refers to a line value with the properties of QGraphsLine.
 *
 * Properties of type \c graphsline follows the \l {QQuickGraphs::theme}{theme} or
 * defined separately.
 *
 * The available properties are:
 *
 * \list
 * \li \l color \c graphsline.mainColor
 * \li \l color \c graphsline.subColor
 * \li \l real \c graphsline.mainWidth
 * \li \l real \c graphsline.subWidth
 * \li \l color \c graphsline.labelTextColor
 * \endlist
 *
 * When integrating with C++, note that any QGraphsLine value
 * \l{qtqml-cppintegration-data.html}{passed into QML from C++} is automatically
 * converted into a \c graphsline value, and vice-versa.
 *
 * This value type is provided by the QtQuick import.
 *
 * \sa {QML Value Types}
 */

/*!
 * \qmltype GraphsTheme
 * \inqmlmodule QtGraphs
 * \ingroup graphs_qml
 * \instantiates QGraphsTheme
 * \brief A visual style for graphs.
 *
 * This type is used to specify visual properties that affect the whole graph.
 * There are several built-in themes that can be used as is or modified freely.
 *
 *
 * Themes can be created from scratch by using the
 * \l{QGraphsTheme::Theme::UserDefined}{GraphsTheme.Theme.UserDefined} enum value.
 *
 * \section1 Default Theme
 *
 * The following table lists the properties controlled by themes and the
 * default values for \l{QGraphsTheme::Theme::UserDefined}
 * {QGraphsTheme.Theme.UserDefined}.
 *
 * \table
 *   \header
 *     \li Property
 *     \li Default Value
 *   \row
 *     \li backgroundVisible
 *     \li \c true
 *   \row
 *     \li seriesColors
 *     \li Qt::black
 *   \row
 *     \li seriesGradients
 *     \li QLinearGradient. Essentially fully black.
 *   \row
 *     \li colorStyle
 *     \li Uniform
 *   \row
 *     \li \l labelFont
 *     \li QFont
 *   \row
 *     \li gridVisible
 *     \li \c true
 *   \row
 *     \li labelBackgroundVisible
 *     \li \c true
 *   \row
 *     \li labelBorderVisible
 *     \li \c true
 *   \row
 *     \li labelsVisible
 *     \li \c true
 * \endtable
 *
 * \section1 Usage examples
 *
 * Using a built-in theme without any modifications:
 *
 * \snippet doc_src_qgraphstheme.cpp 4
 *
 * Using a built-in theme and modifying some properties:
 *
 * \snippet doc_src_qgraphstheme.cpp 5
 *
 * Using a user-defined theme:
 *
 * \snippet doc_src_qgraphstheme.cpp 6
 *
 * For GraphsTheme enums, see \l GraphsTheme::colorStyle and \l{GraphsTheme::theme}.
 */

/*!
 * \qmlproperty list<color> GraphsTheme::seriesColors
 *
 * The list of base colors to be used for all the objects in the graph, series
 * by series. If there are more series than colors, color list wraps and starts
 * again with the first color in the list. Has no immediate effect if colorStyle
 * is not \c GraphsTheme.ColorStyle.Uniform.
 *
 * This can be overridden by setting \l{Abstract3DSeries::baseColor}
 * {Abstract3DSeries.baseColor} explicitly in the series.
 */

/*!
 * \qmlproperty list<color> GraphsTheme::borderColors
 *
 * The list of border colors to be used for all the objects in the graph,
 * series by series.
 *
 * If there are more series than colors, the color list wraps and starts again
 * with the first color in the list.
 *
 * Has no immediate effect if colorStyle is not Uniform.
 */

/*!
 * \qmlproperty color GraphsTheme::plotAreaBackgroundColor
 *
 * The color of the graph plot area background.
 * The default value depends on \l colorScheme.
 */

/*!
 * \qmlproperty color GraphsTheme::backgroundColor
 *
 * The color of the view the graph is drawn into.
 * The default value depends on \l colorScheme.
 */

/*!
 * \qmlproperty color GraphsTheme::labelTextColor
 *
 * The color of the font used for labels.
 * The default value depends on \l colorScheme.
 */

/*!
 * \qmlproperty color GraphsTheme::labelBackgroundColor
 *
 * The color of the label backgrounds. Has no effect if labelBackgroundVisible
 * is \c false.
 * The default value depends on \l colorScheme.
 */

/*!
 * \qmlproperty color GraphsTheme::singleHighlightColor
 *
 * The highlight color for a selected object. Used if
 * \l{GraphsItem3D::selectionMode}{selectionMode}
 * has the \c GraphsItem3D.SelectionFlag.Item flag set.
 * The default value depends on \l colorScheme.
 */

/*!
 * \qmlproperty color GraphsTheme::multiHighlightColor
 *
 * The highlight color for selected objects. Used if
 * \l{GraphsItem3D::selectionMode}{selectionMode}
 * has the \c GraphsItem3D.SelectionFlag.Row or \c GraphsItem3D.SelectionFlag.Column
 * flag set.
 * The default value depends on \l colorScheme.
 */

/*!
 * \qmlproperty list<Gradient> GraphsTheme::seriesGradients
 *
 * The list of base gradients to be used for all the objects in the graph,
 * series by series. If there are more series than gradients, the gradient list
 * wraps and starts again with the first gradient in the list.
 *
 * Has no immediate effect if colorStyle is \l{QGraphsTheme::ColorStyle::Uniform}
 * {GraphsTheme.ColorStyle.Uniform}.
 *
 * This value can be overridden by setting \l{Abstract3DSeries::baseGradient}
 *{Abstract3DSeries.baseGradient} explicitly in the series.
 */

/*!
 * \qmlproperty Gradient GraphsTheme::singleHighlightGradient
 *
 * The highlight gradient for a selected object. Used if
 * \l{GraphsItem3D::selectionMode}{selectionMode}
 * has the \c GraphsItem3D.SelectionFlag.Item flag set.
 * The default value depends on \l colorScheme.
 */

/*!
 * \qmlproperty Gradient GraphsTheme::multiHighlightGradient
 *
 * The highlight gradient for selected objects. Used if
 * \l{GraphsItem3D::selectionMode}{selectionMode}
 * has the \c GraphsItem3D.SelectionFlag.Row or \c GraphsItem3D.SelectionFlag.Column
 * flag set.
 * The default value depends on \l colorScheme.
 */

/*!
 * \qmlproperty bool GraphsTheme::labelBorderVisible
 *
 * Defines whether label borders are drawn for labels that have a background.
 * Has no effect if labelBackgroundVisible is \c false.
 * The default value is \c true.
 */

/*!
 * \qmlproperty font GraphsTheme::labelFont
 *
 * Sets the font to be used for labels.
 */

/*!
 * \qmlproperty font GraphsTheme::axisXLabelFont
 *
 * Sets the font to be used for labels on axisX.
 */

/*!
 * \qmlproperty font GraphsTheme::axisYLabelFont
 *
 * Sets the font to be used for labels on axisY.
 */

/*!
 * \qmlproperty font GraphsTheme::axisZLabelFont
 *
 * Sets the font to be used for labels on axisZ.
 */

/*!
 * \qmlproperty bool GraphsTheme::backgroundVisible
 *
 * Defines whether the view background is drawn by using the value of
 * backgroundColor.
 * The default value is \c true.
 */

/*!
 * \qmlproperty bool GraphsTheme::plotAreaBackgroundVisible
 *
 * Defines whether the plot area background is drawn by using the value of
 * plotAreaBackgroundColor.
 * The default value is \c true.
 */

/*!
 * \qmlproperty bool GraphsTheme::gridVisible
 *
 * Defines whether the grid lines are drawn. This value affects all grid lines.
 * The default value is \c true.
 */

/*!
 * \qmlproperty bool GraphsTheme::labelBackgroundVisible
 *
 * Defines whether the label is drawn with a background that uses
 * labelBackgroundColor (including alpha), or with a fully transparent
 * background. Labels with a background are drawn to equal sizes per axis based
 * on the longest label, and the text is centered in them. Labels without
 * a background are drawn as is and are left or right aligned based on their
 * position in the graph.
 * The default value is \c true.
 */

/*!
 * \qmlproperty GraphsTheme.ColorStyle GraphsTheme::colorStyle
 *
 * The style of the graph colors. One of QGraphsTheme::ColorStyle enum values.
 *
 * This value can be overridden by setting \l{Abstract3DSeries::colorStyle}
 * {Abstract3DSeries.colorStyle} explicitly in the series.
 * \note This property does not have an effect in Qt Graphs for 2D.
 *
 * \sa QGraphsTheme::ColorStyle
 */

/*!
 * \qmlproperty Qt.ColorScheme GraphsTheme::colorScheme
 *
 * The color scheme of the graph in use.
 *
 * \sa Qt::ColorScheme
 */

/*!
 * \qmlproperty bool GraphsTheme::labelsVisible
 *
 * Defines whether labels are drawn at all. If this is \c{false}, all other
 * label properties have no effect.
 * The default value is \c true.
 */

/*!
 * \qmlproperty GraphsTheme.Theme GraphsTheme::theme
 *
 * The type of the theme. If no type is set, the type is
 * \l{QGraphsTheme::Theme::UserDefined}{GraphsTheme.Theme.UserDefined}.
 * Changing the theme type after the item has been constructed will change all
 * other properties of the theme to what the predefined theme specifies.
 * Changing the theme type of the active theme of the graph will also reset all
 * attached series to use the new theme.
 */

/*!
 * \qmlproperty color GraphsTheme::GraphsLine.mainColor
 *
 * The color of the main lines.
 * The default value depends on \l colorScheme.
 */

/*!
 * \qmlproperty color GraphsTheme::GraphsLine.subColor
 *
 * The color of the sub lines.
 * The default value depends on \l colorScheme.
 */

/*!
 * \qmlproperty real GraphsTheme::GraphsLine.mainWidth
 *
 * The width of the main lines.
 * The default value is \c 2.0.
 *
 * If it is set for grid lines, only has effect if
 * \l{QAbstract3DGraphWidget::shaderGridEnabled} is \c true
 */

/*!
 * \qmlproperty real GraphsTheme::GraphsLine.subWidth
 *
 * The color of the main lines.
 * The default value is \c 1.0.
 *
 * If it is set for grid lines, only has effect if
 * \l{QAbstract3DGraphWidget::shaderGridEnabled} is \c true
 */

/*!
 * \qmlproperty color GraphsTheme::GraphsLine.labelTextColor
 *
 * The color of the text used for labels.
 * The default value depends on \l colorScheme.
 */

QGraphsTheme::QGraphsTheme(QObject *parent)
    : QObject(parent)
      , m_themeDirty(false)
      , m_colorStyle(ColorStyle::Uniform)
      , m_plotAreaBackgroundColor(QColor())
      , m_backgroundColor(QColor())
      , m_labelBackgroundColor(QColor())
      , m_labelTextColor(QColor())
      , m_singleHighlightColor(QColor())
      , m_multiHighlightColor(QColor())
      , m_multiHighlightGradient(QLinearGradient())
      , m_singleHighlightGradient(QLinearGradient())
      , m_borderWidth(1.0)
      , m_singleHLGradient(nullptr)
      , m_multiHLGradient(nullptr)
      , m_grid(QGraphsLine())
      , m_axisX(QGraphsLine())
      , m_axisY(QGraphsLine())
      , m_axisZ(QGraphsLine())
{
    setBackgroundVisible(true);
    setPlotAreaBackgroundVisible(true);
    setLabelBackgroundVisible(true);
    setGridVisible(true);
    setLabelsVisible(true);
    auto platformScheme = QGuiApplication::styleHints()->colorScheme();
    if (platformScheme != Qt::ColorScheme::Unknown)
        setColorScheme(platformScheme);
    else
        setColorScheme(Qt::ColorScheme::Light);
    setLabelBorderVisible(true);
    setTheme(Theme::UserDefined, true);
    setLabelFont(QFont(QLatin1String("Arial")));
    setAxisXLabelFont(QFont());
    setAxisYLabelFont(QFont());
    setAxisZLabelFont(QFont());
}

QGraphsTheme::~QGraphsTheme()
{
}

void QGraphsTheme::resetColorTheme()
{
    setTheme(QGraphsTheme::Theme::QtGreen);
}

QGraphsThemeDirtyBitField *QGraphsTheme::dirtyBits()
{
    return &m_dirtyBits;
}

void QGraphsTheme::resetDirtyBits()
{
    m_dirtyBits.plotAreaBackgroundColorDirty = true;
    m_dirtyBits.plotAreaBackgroundVisibilityDirty = true;
    m_dirtyBits.seriesColorsDirty = true;
    m_dirtyBits.seriesGradientDirty = true;
    m_dirtyBits.colorSchemeDirty = true;
    m_dirtyBits.colorStyleDirty = true;
    m_dirtyBits.labelFontDirty = true;
    m_dirtyBits.gridVisibilityDirty = true;
    m_dirtyBits.gridDirty = true;
    m_dirtyBits.labelBackgroundColorDirty = true;
    m_dirtyBits.labelBackgroundVisibilityDirty = true;
    m_dirtyBits.labelBorderVisibilityDirty = true;
    m_dirtyBits.labelTextColorDirty = true;
    m_dirtyBits.axisXDirty = true;
    m_dirtyBits.axisYDirty = true;
    m_dirtyBits.axisZDirty = true;
    m_dirtyBits.labelsVisibilityDirty = true;
    m_dirtyBits.multiHighlightColorDirty = true;
    m_dirtyBits.multiHighlightGradientDirty = true;
    m_dirtyBits.singleHighlightColorDirty = true;
    m_dirtyBits.singleHighlightGradientDirty = true;
    m_dirtyBits.themeDirty = true;
    m_dirtyBits.backgroundColorDirty = true;
    m_dirtyBits.backgroundVisibilityDirty = true;
}

/*!
 * \property QGraphsTheme::colorScheme
 *
 * The color scheme of the graph in use.
 *
 * \sa Qt::ColorScheme
 */
Qt::ColorScheme QGraphsTheme::colorScheme() const
{
    return m_colorScheme;
}

void QGraphsTheme::setColorScheme(Qt::ColorScheme newColorScheme)
{
    m_dirtyBits.colorSchemeDirty = true;
    m_colorScheme = newColorScheme;
    setColorSchemePalette();
    m_themeDirty = true;
    emit colorSchemeChanged();
    emit update();
}

/*!
 * \property QGraphsTheme::theme
 *
 * The type of the theme. If no type is set, the type is
 * \l{QGraphsTheme::Theme::UserDefined}{GraphsTheme.Theme.UserDefined}.
 * Changing the theme type after the item has been constructed will change all
 * other properties of the theme to what the predefined theme specifies.
 * Changing the theme type of the active theme of the graph will also reset all
 * attached series to use the new theme.
 */
QGraphsTheme::Theme QGraphsTheme::theme() const
{
    return m_theme;
}

void QGraphsTheme::setTheme(Theme newTheme, bool force)
{
    if (!force && m_theme == newTheme)
        return;
    m_dirtyBits.themeDirty = true;
    m_theme = newTheme;
    m_themeDirty = true;
    setThemePalette();
    emit themeChanged(m_theme);
    emit update();
}

/*!
 * \property QGraphsTheme::colorStyle
 *
 * The style of the graph colors. One of QGraphsTheme::ColorStyle enum values.
 *
 * This value can be overridden by setting \l{Abstract3DSeries::colorStyle}
 * {Abstract3DSeries.colorStyle} explicitly in the series.
 * \note This property does not have an effect in Qt Graphs for 2D.
 *
 * \sa QGraphsTheme::ColorStyle
 */
QGraphsTheme::ColorStyle QGraphsTheme::colorStyle() const
{
    return m_colorStyle;
}

void QGraphsTheme::setColorStyle(ColorStyle newColorStyle)
{
    if (m_colorStyle == newColorStyle)
        return;
    m_dirtyBits.colorStyleDirty = true;
    m_colorStyle = newColorStyle;
    emit colorStyleChanged(newColorStyle);
    emit update();
}

/*!
 * \property QGraphsTheme::axisXLabelFont
 *
 * \brief The font to be used for labels on x axis.
 */
QFont QGraphsTheme::axisXLabelFont() const
{
    return m_axisXLabelFont;
}

void QGraphsTheme::setAxisXLabelFont(const QFont &newAxisXLabelFont)
{
    m_customBits.axisXLabelFontCustom = true;
    if (m_axisXLabelFont == newAxisXLabelFont)
        return;
    m_axisXLabelFont = newAxisXLabelFont;
    emit axisXLabelFontChanged();
    emit update();
}

/*!
 * \property QGraphsTheme::axisYLabelFont
 *
 * \brief The font to be used for labels on y axis.
 */
QFont QGraphsTheme::axisYLabelFont() const
{
    return m_axisYLabelFont;
}

void QGraphsTheme::setAxisYLabelFont(const QFont &newAxisYLabelFont)
{
    m_customBits.axisYLabelFontCustom = true;
    if (m_axisYLabelFont == newAxisYLabelFont)
        return;
    m_axisYLabelFont = newAxisYLabelFont;
    emit axisYLabelFontChanged();
    emit update();
}

/*!
 * \property QGraphsTheme::axisZLabelFont
 *
 * \brief The font to be used for labels on z axis.
 */
QFont QGraphsTheme::axisZLabelFont() const
{
    return m_axisZLabelFont;
}

void QGraphsTheme::setAxisZLabelFont(const QFont &newAxisZLabelFont)
{
    m_customBits.axisZLabelFontCustom = true;
    if (m_axisZLabelFont == newAxisZLabelFont)
        return;
    m_axisZLabelFont = newAxisZLabelFont;
    emit axisZLabelFontChanged();
    emit update();
}

/*!
 * \property QGraphsTheme::plotAreaBackgroundColor
 *
 * \brief The color of the graph plot area background.
 * The default value depends on \l colorScheme.
 */
QColor QGraphsTheme::plotAreaBackgroundColor() const
{
    if (m_customBits.plotAreaBackgroundColorCustom)
        return m_plotAreaBackgroundColor;
    return m_plotAreaBackgroundThemeColor;
}

void QGraphsTheme::setPlotAreaBackgroundColor(const QColor &newBackgroundColor)
{
    m_customBits.plotAreaBackgroundColorCustom = true;
    if (m_plotAreaBackgroundColor == newBackgroundColor)
        return;
    m_dirtyBits.plotAreaBackgroundColorDirty = true;
    m_plotAreaBackgroundColor = newBackgroundColor;
    emit plotAreaBackgroundColorChanged();
    emit update();
}

/*!
 * \property QGraphsTheme::plotAreaBackgroundVisible
 *
 * \brief Whether the plot area background is visible.
 *
 * The background is drawn by using the value of plotAreaBackgroundColor.
 * The default value is \c true.
 */
bool QGraphsTheme::isPlotAreaBackgroundVisible() const
{
    return m_plotAreaBackgroundVisibility;
}

void QGraphsTheme::setPlotAreaBackgroundVisible(bool newBackgroundVisibility)
{
    if (m_plotAreaBackgroundVisibility == newBackgroundVisibility)
        return;
    m_dirtyBits.plotAreaBackgroundVisibilityDirty = true;
    m_plotAreaBackgroundVisibility = newBackgroundVisibility;
    emit plotAreaBackgroundVisibleChanged();
    emit update();
}

/*!
 * \property QGraphsTheme::backgroundVisible
 *
 * \brief Whether the background is visible.
 *
 * The background is drawn by using the value of backgroundColor.
 * The default value is \c true.
 */
bool QGraphsTheme::isBackgroundVisible() const
{
    return m_backgroundVisibility;
}

void QGraphsTheme::setBackgroundVisible(bool newBackgroundVisible)
{
    if (m_backgroundVisibility == newBackgroundVisible)
        return;
    m_dirtyBits.backgroundVisibilityDirty = true;
    m_backgroundVisibility = newBackgroundVisible;
    emit backgroundVisibleChanged();
    emit update();
}

/*!
 * \property QGraphsTheme::gridVisible
 *
 * \brief Whether the grid lines are drawn.
 *
 * This value affects all grid lines.
 * The default value is \c true.
 */
bool QGraphsTheme::isGridVisible() const
{
    return m_gridVisibility;
}

void QGraphsTheme::setGridVisible(bool newGridVisibility)
{
    if (m_gridVisibility == newGridVisibility)
        return;
    m_dirtyBits.gridVisibilityDirty = true;
    m_gridVisibility = newGridVisibility;
    emit gridVisibleChanged();
    emit update();
}

/*!
 * \property QGraphsTheme::backgroundColor
 *
 * \brief The color of the view the graph is drawn into.
 * The default value depends on \l colorScheme.
 */
QColor QGraphsTheme::backgroundColor() const
{
    if (m_customBits.backgroundColorCustom)
        return m_backgroundColor;
    return m_backgroundThemeColor;
}

void QGraphsTheme::setBackgroundColor(const QColor &newBackgroundColor)
{
    m_customBits.backgroundColorCustom = true;
    if (m_backgroundColor == newBackgroundColor)
        return;
    m_dirtyBits.backgroundColorDirty = true;
    m_backgroundColor = newBackgroundColor;
    emit backgroundColorChanged();
    emit update();
}

/*!
 * \property QGraphsTheme::labelsVisible
 *
 * \brief Whether labels are drawn at all.
 *
 * If this is \c{false}, all other label properties have no effect.
 * The default value is \c true.
 */
bool QGraphsTheme::labelsVisible() const
{
    return m_labelsVisibility;
}

void QGraphsTheme::setLabelsVisible(bool newLabelsVisibility)
{
    if (m_labelsVisibility == newLabelsVisibility)
        return;
    m_dirtyBits.labelsVisibilityDirty = true;
    m_labelsVisibility = newLabelsVisibility;
    emit labelsVisibleChanged();
    emit update();
}

/*!
 * \property QGraphsTheme::labelBackgroundColor
 *
 * \brief The color of the label backgrounds.
 *
 * Has no effect if labelBackgroundVisible is \c false.
 * The default value depends on \l colorScheme.
 */
QColor QGraphsTheme::labelBackgroundColor() const
{
    if (m_customBits.labelBackgroundColorCustom)
        return m_labelBackgroundColor;
    return m_labelBackgroundThemeColor;
}

void QGraphsTheme::setLabelBackgroundColor(const QColor &newLabelBackgroundColor)
{
    m_customBits.labelBackgroundColorCustom = true;
    if (m_labelBackgroundColor == newLabelBackgroundColor)
        return;
    m_dirtyBits.labelBackgroundColorDirty = true;
    m_labelBackgroundColor = newLabelBackgroundColor;
    emit labelBackgroundColorChanged();
    emit update();
}

/*!
 * \property QGraphsTheme::labelTextColor
 *
 * \brief The color of the font used for labels.
 * The default value depends on \l colorScheme.
 */
QColor QGraphsTheme::labelTextColor() const
{
    if (m_customBits.labelTextColorCustom)
        return m_labelTextColor;
    return m_labelTextThemeColor;
}

void QGraphsTheme::setLabelTextColor(const QColor &newLabelTextColor)
{
    m_customBits.labelTextColorCustom = true;
    if (m_labelTextColor == newLabelTextColor)
        return;
    m_dirtyBits.labelTextColorDirty = true;
    m_labelTextColor = newLabelTextColor;
    emit labelTextColorChanged();
    emit update();
}

/*!
 * \property QGraphsTheme::singleHighlightColor
 *
 * \brief The highlight color for a selected object.
 *
 * Used if \l{Q3DGraphsWidgetItem::selectionMode}{selectionMode} has the
 * \c QGraphs3D::SelectionFlag::Item flag set.
 * The default value depends on \l colorScheme.
 */
QColor QGraphsTheme::singleHighlightColor() const
{
    if (m_customBits.singleHighlightColorCustom)
        return m_singleHighlightColor;
    return m_singleHighlightThemeColor;
}

void QGraphsTheme::setSingleHighlightColor(const QColor &newSingleHighlightColor)
{
    m_customBits.singleHighlightColorCustom = true;
    if (m_singleHighlightColor == newSingleHighlightColor)
        return;
    m_dirtyBits.singleHighlightColorDirty = true;
    m_singleHighlightColor = newSingleHighlightColor;
    emit singleHighlightColorChanged(m_singleHighlightColor);
    emit update();
}

/*!
 * \property QGraphsTheme::multiHighlightColor
 *
 * \brief The highlight color for selected objects.
 *
 * Used if \l{Q3DGraphsWidgetItem::selectionMode}{selectionMode} has the
 * \c QGraphs3D::SelectionFlag::Row or \c QGraphs3D::SelectionFlag::Column
 * flag set.
 * The default value depends on \l colorScheme.
 */
QColor QGraphsTheme::multiHighlightColor() const
{
    if (m_customBits.multiHighlightColorCustom)
        return m_multiHighlightColor;
    return m_multiHighlightThemeColor;
}

void QGraphsTheme::setMultiHighlightColor(const QColor &newMultiHighlightColor)
{
    m_customBits.multiHighlightColorCustom = true;
    if (m_multiHighlightColor == newMultiHighlightColor)
        return;
    m_dirtyBits.multiHighlightColorDirty = true;
    m_multiHighlightColor = newMultiHighlightColor;
    emit multiHighlightColorChanged(m_multiHighlightColor);
    emit update();
}

/*!
 * \property QGraphsTheme::singleHighlightGradient
 *
 * \brief The highlight gradient for a selected object.
 *
 * Used if \l{Q3DGraphsWidgetItem::selectionMode}{selectionMode}
 * has the \c QGraphs3D::SelectionFlag::Item flag set.
 * The default value depends on \l colorScheme.
 */
void QGraphsTheme::setSingleHighlightGradient(const QLinearGradient &gradient)
{
    m_customBits.singleHighlightGradientCustom = true;
    if (m_singleHighlightGradient == gradient)
        return;

    m_dirtyBits.singleHighlightGradientDirty = true;
    m_singleHighlightGradient = gradient;
    emit singleHighlightGradientChanged(m_singleHighlightGradient);
    emit update();
}

QLinearGradient QGraphsTheme::singleHighlightGradient() const
{
    if (m_customBits.singleHighlightGradientCustom)
        return m_singleHighlightGradient;
    return m_singleHighlightThemeGradient;
}

/*!
 * \property QGraphsTheme::multiHighlightGradient
 *
 * \brief The highlight gradient for selected objects.
 *
 * Used if \l{Q3DGraphsWidgetItem::selectionMode}{selectionMode}
 * has the \c QGraphs3D::SelectionFlag::Row or
 * \c QGraphs3D::SelectionFlag::Column flag set.
 * The default value depends on \l colorScheme.
 */
void QGraphsTheme::setMultiHighlightGradient(const QLinearGradient &gradient)
{
    m_customBits.multiHighlightGradientCustom = true;
    if (m_multiHighlightGradient == gradient)
        return;

    m_dirtyBits.multiHighlightGradientDirty = true;
    m_multiHighlightGradient = gradient;
    emit multiHighlightGradientChanged(m_multiHighlightGradient);
    emit update();
}

QLinearGradient QGraphsTheme::multiHighlightGradient() const
{
    if (m_customBits.multiHighlightGradientCustom)
        return m_multiHighlightGradient;
    return m_multiHighlightThemeGradient;
}

/*!
 * \property QGraphsTheme::labelFont
 *
 * \brief The font to be used for labels.
 */
QFont QGraphsTheme::labelFont() const
{
    return m_labelFont;
}

void QGraphsTheme::setLabelFont(const QFont &newFont)
{
    if (m_labelFont == newFont)
        return;
    m_dirtyBits.labelFontDirty = true;
    m_labelFont = newFont;
    if (!m_customBits.axisXLabelFontCustom)
        m_axisXLabelFont = newFont;
    if (!m_customBits.axisYLabelFontCustom)
        m_axisYLabelFont = newFont;
    if (!m_customBits.axisZLabelFontCustom)
        m_axisZLabelFont = newFont;
    emit labelFontChanged();
    emit update();
}

/*!
 * \property QGraphsTheme::labelBackgroundVisible
 *
 *\brief Whether the label is drawn with a color background or with a fully
 * transparent background.
 *
 * The labelBackgroundColor value (including alpha) is used for drawing the
 * background.
 *
 * Labels with a background are drawn to equal sizes per axis based
 * on the longest label, and the text is centered in them. Labels without a
 * background are drawn as is and are left or right aligned based on their
 * position in the graph.
 * The default value is \c true.
 */
bool QGraphsTheme::isLabelBackgroundVisible() const
{
    return m_labelBackgroundVisibility;
}

void QGraphsTheme::setLabelBackgroundVisible(bool newLabelBackgroundVisibility)
{
    if (m_labelBackgroundVisibility == newLabelBackgroundVisibility)
        return;
    m_dirtyBits.labelBackgroundVisibilityDirty = true;
    m_labelBackgroundVisibility = newLabelBackgroundVisibility;
    emit labelBackgroundVisibleChanged();
    emit update();
}

/*!
 * \property QGraphsTheme::labelBorderVisible
 *
 * \brief Whether label borders are drawn for labels that have a background.
 *
 * Has no effect if labelBackgroundVisible is \c false.
 * The default value is \c true.
 */
bool QGraphsTheme::isLabelBorderVisible() const
{
    return m_labelBorderVisibility;
}

void QGraphsTheme::setLabelBorderVisible(bool newLabelBorderVisibility)
{
    if (m_labelBorderVisibility == newLabelBorderVisibility)
        return;
    m_dirtyBits.labelBorderVisibilityDirty = true;
    m_labelBorderVisibility = newLabelBorderVisibility;
    emit labelBorderVisibleChanged();
    emit update();
}

/*!
 * \property QGraphsTheme::seriesColors
 *
 * \brief The list of base colors to be used for all the objects in the graph,
 * series by series.
 *
 * If there are more series than colors, the color list wraps and starts again
 * with the first color in the list.
 *
 * Has no immediate effect if colorStyle is not Uniform.
 *
 * This value can be overridden by setting the \l{QAbstract3DSeries::baseColor}
 * {baseColor} explicitly in the series.
 */
QList<QColor> QGraphsTheme::seriesColors() const
{
    if (m_customBits.seriesColorsCustom && !m_seriesColors.isEmpty())
        return m_seriesColors;
    return m_seriesThemeColors;
}

void QGraphsTheme::setSeriesColors(const QList<QColor> &newSeriesColors)
{
    m_customBits.seriesColorsCustom = true;
    if (m_seriesColors == newSeriesColors)
        return;
    m_dirtyBits.seriesColorsDirty = true;
    m_seriesColors = newSeriesColors;
    emit seriesColorsChanged(m_seriesColors);
    emit update();
}

/*!
 * \property QGraphsTheme::borderColors
 *
 * \brief The list of border colors to be used for all the objects in the graph,
 * series by series.
 *
 * If there are more series than colors, the color list wraps and starts again
 * with the first color in the list.
 *
 * Has no immediate effect if colorStyle is not Uniform.
 */
QList<QColor> QGraphsTheme::borderColors() const
{
    if (m_customBits.borderColorsCustom && !m_borderColors.isEmpty())
        return m_borderColors;
    return m_borderThemeColors;
}

void QGraphsTheme::setBorderColors(const QList<QColor> &newBorderColors)
{
    m_customBits.borderColorsCustom = true;
    if (m_borderColors == newBorderColors)
        return;
    m_borderColors = newBorderColors;
    emit borderColorsChanged();
    emit update();
}

/*!
 * \property QGraphsTheme::seriesGradients
 *
 * \brief The list of base gradients to be used for all the objects in the
 * graph, series by series.
 *
 * If there are more series than gradients, the gradient list wraps and starts
 * again with the first gradient in the list
 *
 * Has no immediate effect if colorStyle is Uniform.
 *
 * This value can be overridden by setting the
 * \l{QAbstract3DSeries::baseGradient}{baseGradient} explicitly in the series.
 */
QList<QLinearGradient> QGraphsTheme::seriesGradients() const
{
    if (m_customBits.seriesGradientCustom && !m_seriesGradients.isEmpty())
        return m_seriesGradients;
    return m_seriesThemeGradients;
}

void QGraphsTheme::setSeriesGradients(const QList<QLinearGradient> &newSeriesGradients)
{
    m_customBits.seriesGradientCustom = true;
    if (newSeriesGradients.size()) {
        m_dirtyBits.seriesGradientDirty = true;
        if (m_seriesGradients != newSeriesGradients) {
            m_seriesGradients.clear();
            m_seriesGradients = newSeriesGradients;
            emit seriesGradientsChanged(newSeriesGradients);
            emit update();
        }
    } else {
        m_seriesGradients.clear();
        emit update();
    }
}

/*!
 * \property QGraphsTheme::borderWidth
 *
 * \brief The width of borders in graph if any
 * The default value is \c 1.0.
 */
qreal QGraphsTheme::borderWidth() const
{
    return m_borderWidth;
}

void QGraphsTheme::setBorderWidth(qreal newBorderWidth)
{
    if (qFuzzyCompare(m_borderWidth, newBorderWidth))
        return;
    m_borderWidth = newBorderWidth;
    emit borderWidthChanged();
    emit update();
}

void QGraphsTheme::handleBaseColorUpdate()
{
    qsizetype colorCount = m_colors.size();
    int changed = 0;
    // Check which one changed
    QQuickGraphsColor *color = qobject_cast<QQuickGraphsColor *>(QObject::sender());
    for (int i = 0; i < colorCount; i++) {
        if (color == m_colors.at(i)) {
            changed = i;
            break;
        }
    }
    // Update the changed one from the list
    QList<QColor> list = seriesColors();
    list[changed] = m_colors.at(changed)->color();
    // Set the changed list
    setSeriesColors(list);
}

void QGraphsTheme::handleBaseGradientUpdate()
{
    // Find out which gradient has changed, and update the list with it
    qsizetype gradientCount = m_gradients.size();
    int changed = 0;

    // Check which one changed
    QQuickGradient *newGradient = qobject_cast<QQuickGradient *>(QObject::sender());
    QJSEngine engine;
    QJSValue updatedGradient = engine.newQObject(newGradient);

    for (int i = 0; i < gradientCount; ++i) {
        if (newGradient == m_gradients.at(i)) {
            changed = i;
            break;
        }
    }

    // Update the changed one from the list
    QList<QLinearGradient> list = seriesGradients();
    list[changed] = convertGradient(updatedGradient);

    // Set the changed list
    setSeriesGradients(list);
}

void QGraphsTheme::classBegin()
{
}

void QGraphsTheme::componentComplete()
{
    m_componentComplete = true;
}

void QGraphsTheme::setColorSchemePalette()
{
    float defaultColorLevel = 0.5f;

    if (m_colorScheme == Qt::ColorScheme::Unknown)
        return;

    if (m_componentComplete) {
        // Reset all customizations which colorScheme changes
        m_customBits.plotAreaBackgroundColorCustom = false;
        m_customBits.labelBackgroundColorCustom = false;
        m_customBits.multiHighlightColorCustom = false;
        m_customBits.multiHighlightGradientCustom = false;
        m_customBits.singleHighlightColorCustom = false;
        m_customBits.singleHighlightGradientCustom = false;
        m_grid.resetCustomBits();
        m_axisX.resetCustomBits();
        m_axisY.resetCustomBits();
        m_axisZ.resetCustomBits();
    }

    if (m_colorScheme == Qt::ColorScheme::Dark) {
        m_backgroundThemeColor = QColor(QRgb(0x262626));
        m_plotAreaBackgroundThemeColor = QColor(QRgb(0x1F1F1F));
        m_labelBackgroundThemeColor = QColor(QRgb(0x2E2E2E));

        m_grid.m_mainThemeColor = QColor(QRgb(0xAEABAB));
        m_grid.m_subThemeColor = QColor(QRgb(0x6A6A6A));
        m_axisX.m_mainThemeColor = QColor(QRgb(0xAEABAB));
        m_axisX.m_subThemeColor = QColor(QRgb(0x6A6A6A));
        m_axisY.m_mainThemeColor = QColor(QRgb(0xAEABAB));
        m_axisY.m_subThemeColor = QColor(QRgb(0x6A6A6A));
        m_axisZ.m_mainThemeColor = QColor(QRgb(0xAEABAB));
        m_axisZ.m_subThemeColor = QColor(QRgb(0x6A6A6A));

        m_singleHighlightThemeColor = QColor(QRgb(0xDBEB00));
        m_multiHighlightThemeColor = QColor(QRgb(0x22D489));
        m_singleHighlightThemeGradient = createGradient(QColor(QRgb(0xDBEB00)), defaultColorLevel);
        m_multiHighlightThemeGradient = createGradient(QColor(QRgb(0x22D489)), defaultColorLevel);

        m_labelTextThemeColor = QColor(QRgb(0xAEAEAE));
        m_axisX.m_labelTextThemeColor = QColor(QRgb(0xAEAEAE));
        m_axisY.m_labelTextThemeColor = QColor(QRgb(0xAEAEAE));
        m_axisZ.m_labelTextThemeColor = QColor(QRgb(0xAEAEAE));
    } else {
        m_backgroundThemeColor = QColor(QRgb(0xF2F2F2));
        m_plotAreaBackgroundThemeColor = QColor(QRgb(0xFCFCFC));
        m_labelBackgroundThemeColor = QColor(QRgb(0xE7E7E7));

        m_grid.m_mainThemeColor = QColor(QRgb(0x545151));
        m_grid.m_subThemeColor = QColor(QRgb(0xAFAFAF));
        m_axisX.m_mainThemeColor = QColor(QRgb(0x545151));
        m_axisX.m_subThemeColor = QColor(QRgb(0xAFAFAF));
        m_axisY.m_mainThemeColor = QColor(QRgb(0x545151));
        m_axisY.m_subThemeColor = QColor(QRgb(0xAFAFAF));
        m_axisZ.m_mainThemeColor = QColor(QRgb(0x545151));
        m_axisZ.m_subThemeColor = QColor(QRgb(0xAFAFAF));

        m_singleHighlightThemeColor = QColor(QRgb(0xCCDC00));
        m_multiHighlightThemeColor = QColor(QRgb(0x22D47B));
        m_singleHighlightThemeGradient = createGradient(QColor(QRgb(0xCCDC00)), defaultColorLevel);
        m_multiHighlightThemeGradient = createGradient(QColor(QRgb(0x22D47B)), defaultColorLevel);

        m_labelTextThemeColor = QColor(QRgb(0x6A6A6A));
        m_axisX.m_labelTextThemeColor = QColor(QRgb(0x6A6A6A));
        m_axisY.m_labelTextThemeColor = QColor(QRgb(0x6A6A6A));
        m_axisZ.m_labelTextThemeColor = QColor(QRgb(0x6A6A6A));
    }

    m_dirtyBits.backgroundColorDirty = true;
    m_dirtyBits.plotAreaBackgroundColorDirty = true;
    m_dirtyBits.labelBackgroundColorDirty = true;
    m_dirtyBits.gridDirty = true;
    m_dirtyBits.axisXDirty = true;
    m_dirtyBits.axisYDirty = true;
    m_dirtyBits.axisZDirty = true;
    m_dirtyBits.singleHighlightColorDirty = true;
    m_dirtyBits.singleHighlightGradientDirty = true;
    m_dirtyBits.multiHighlightColorDirty = true;
    m_dirtyBits.multiHighlightGradientDirty = true;
    m_dirtyBits.labelTextColorDirty = true;
}

void QGraphsTheme::setThemePalette()
{
    float defaultColorLevel = 0.5f;
    m_seriesThemeColors.clear();
    switch (m_theme) {
    case Theme::QtGreen:
        m_seriesThemeColors.append(QColor(QRgb(0xD5F8E7)));
        m_seriesThemeColors.append(QColor(QRgb(0xABF2CE)));
        m_seriesThemeColors.append(QColor(QRgb(0x7BE6B1)));
        m_seriesThemeColors.append(QColor(QRgb(0x51E098)));
        m_seriesThemeColors.append(QColor(QRgb(0x22D478)));
        break;
    case Theme::QtGreenNeon:
        m_seriesThemeColors.append(QColor(QRgb(0x22D478)));
        m_seriesThemeColors.append(QColor(QRgb(0x00AF80)));
        m_seriesThemeColors.append(QColor(QRgb(0x00897B)));
        m_seriesThemeColors.append(QColor(QRgb(0x006468)));
        m_seriesThemeColors.append(QColor(QRgb(0x00414A)));
        break;
    case Theme::MixSeries:
        m_seriesThemeColors.append(QColor(QRgb(0xFFA615)));
        m_seriesThemeColors.append(QColor(QRgb(0x5E45DF)));
        m_seriesThemeColors.append(QColor(QRgb(0x759F1C)));
        m_seriesThemeColors.append(QColor(QRgb(0xF92759)));
        m_seriesThemeColors.append(QColor(QRgb(0x0128F8)));
        break;
    case Theme::OrangeSeries:
        m_seriesThemeColors.append(QColor(QRgb(0xFFC290)));
        m_seriesThemeColors.append(QColor(QRgb(0xFF9C4D)));
        m_seriesThemeColors.append(QColor(QRgb(0xFF7200)));
        m_seriesThemeColors.append(QColor(QRgb(0xD86000)));
        m_seriesThemeColors.append(QColor(QRgb(0xA24900)));
        break;
    case Theme::YellowSeries:
        m_seriesThemeColors.append(QColor(QRgb(0xFFE380)));
        m_seriesThemeColors.append(QColor(QRgb(0xFFC500)));
        m_seriesThemeColors.append(QColor(QRgb(0xE2B000)));
        m_seriesThemeColors.append(QColor(QRgb(0xB88F00)));
        m_seriesThemeColors.append(QColor(QRgb(0x8C6D02)));
        break;
    case Theme::BlueSeries:
        m_seriesThemeColors.append(QColor(QRgb(0x86AFFF)));
        m_seriesThemeColors.append(QColor(QRgb(0x4A86FC)));
        m_seriesThemeColors.append(QColor(QRgb(0x2B6EF1)));
        m_seriesThemeColors.append(QColor(QRgb(0x0750E9)));
        m_seriesThemeColors.append(QColor(QRgb(0x0023DB)));
        break;
    case Theme::PurpleSeries:
        m_seriesThemeColors.append(QColor(QRgb(0xE682E7)));
        m_seriesThemeColors.append(QColor(QRgb(0xB646B7)));
        m_seriesThemeColors.append(QColor(QRgb(0x9035B4)));
        m_seriesThemeColors.append(QColor(QRgb(0x6C2BA0)));
        m_seriesThemeColors.append(QColor(QRgb(0x3D2582)));
        break;
    case Theme::GreySeries:
        m_seriesThemeColors.append(QColor(QRgb(0xCCD0D6)));
        m_seriesThemeColors.append(QColor(QRgb(0xA7AEBB)));
        m_seriesThemeColors.append(QColor(QRgb(0x7A869A)));
        m_seriesThemeColors.append(QColor(QRgb(0x566070)));
        m_seriesThemeColors.append(QColor(QRgb(0x3E4654)));
        break;
    default: // UserDefined
        m_seriesThemeColors.append(QColor(Qt::black));
        break;
    }

    m_borderThemeColors = m_seriesThemeColors;

    m_seriesThemeGradients.clear();
    for (QColor color : m_seriesThemeColors)
        m_seriesThemeGradients.append(createGradient(color, defaultColorLevel));

    m_dirtyBits.seriesColorsDirty = true;
    m_dirtyBits.seriesGradientDirty = true;
}

QLinearGradient QGraphsTheme::createGradient(const QColor &color, float colorLevel)
{
    QColor startColor;
    QLinearGradient gradient = QLinearGradient(qreal(gradientTextureWidth),
                                               qreal(gradientTextureHeight),
                                               0.0,
                                               0.0);
    startColor.setRed(color.red() * colorLevel);
    startColor.setGreen(color.green() * colorLevel);
    startColor.setBlue(color.blue() * colorLevel);
    gradient.setColorAt(0.0, startColor);
    gradient.setColorAt(1.0, color);
    return gradient;
}

void QGraphsTheme::setSingleHighlightGradient(QQuickGradient *gradient)
{
    if (gradient != m_singleHLGradient) {
        if (m_singleHLGradient)
            QObject::disconnect(m_singleHLGradient, 0, this, 0);

        m_singleHLGradient = gradient;

        QObject::connect(m_singleHLGradient,
                         &QQuickGradient::updated,
                         this,
                         &QGraphsTheme::update);

        emit singleHighlightGradientQMLChanged();
    }

    if (m_singleHLGradient != nullptr)
        setThemeGradient(m_singleHLGradient, GradientQMLStyle::SingleHL);
}

QQuickGradient *QGraphsTheme::singleHighlightGradientQML() const
{
    return m_singleHLGradient;
}

void QGraphsTheme::setMultiHighlightGradient(QQuickGradient *gradient)
{
    // connect new / disconnect old
    if (gradient != nullptr) {
        if (m_multiHLGradient)
            QObject::disconnect(m_multiHLGradient, 0, this, 0);

        m_multiHLGradient = gradient;

        QObject::connect(m_multiHLGradient,
                         &QQuickGradient::updated,
                         this,
                         &QGraphsTheme::update);

        emit multiHighlightGradientQMLChanged();
    }

    if (m_multiHLGradient != nullptr)
        setThemeGradient(m_multiHLGradient, GradientQMLStyle::MultiHL);
}

QQuickGradient *QGraphsTheme::multiHighlightGradientQML() const
{
    return m_multiHLGradient;
}

void QGraphsTheme::setThemeGradient(QQuickGradient *gradient, GradientQMLStyle type)
{
    QLinearGradient linearGradient = convertGradient(gradient);

    switch (type) {
    case GradientQMLStyle::SingleHL:
        setSingleHighlightGradient(linearGradient);
        break;
    case GradientQMLStyle::MultiHL:
        setMultiHighlightGradient(linearGradient);
        break;
    default:
        qWarning("Incorrect usage. Type may be GradientQMLStyle::SingleHL or "
                 "GradientQMLStyle::MultiHL.");
        break;
    }
}

QLinearGradient QGraphsTheme::convertGradient(QJSValue gradient)
{
    // Create QLinearGradient out of QJSValue
    QLinearGradient newGradient;
    if (gradient.isQObject()) {
        auto quickGradient = qobject_cast<QQuickGradient *>(gradient.toQObject());
        newGradient.setStops(quickGradient->gradientStops());
    }
    return newGradient;
}

QLinearGradient QGraphsTheme::convertGradient(QQuickGradient *gradient)
{
    // Create QLinearGradient out of QQuickGradient
    QLinearGradient newGradient;
    newGradient.setStops(gradient->gradientStops());
    return newGradient;
}

QQmlListProperty<QQuickGraphsColor> QGraphsTheme::baseColorsQML()
{
    return QQmlListProperty<QQuickGraphsColor>(this,
                                               this,
                                               &QGraphsTheme::appendBaseColorsFunc,
                                               &QGraphsTheme::countBaseColorsFunc,
                                               &QGraphsTheme::atBaseColorsFunc,
                                               &QGraphsTheme::clearBaseColorsFunc);
}

void QGraphsTheme::appendBaseColorsFunc(QQmlListProperty<QQuickGraphsColor> *list,
                                    QQuickGraphsColor *color)
{
    reinterpret_cast<QGraphsTheme *>(list->data)->addColor(color);
}

qsizetype QGraphsTheme::countBaseColorsFunc(QQmlListProperty<QQuickGraphsColor> *list)
{
    return reinterpret_cast<QGraphsTheme *>(list->data)->colorList().size();
}

QQuickGraphsColor *QGraphsTheme::atBaseColorsFunc(QQmlListProperty<QQuickGraphsColor> *list,
                                              qsizetype index)
{
    return reinterpret_cast<QGraphsTheme *>(list->data)->colorList().at(index);
}

void QGraphsTheme::clearBaseColorsFunc(QQmlListProperty<QQuickGraphsColor> *list)
{
    reinterpret_cast<QGraphsTheme *>(list->data)->clearColors();
}

QQmlListProperty<QObject> QGraphsTheme::baseGradientsQML()
{
    return QQmlListProperty<QObject>(this,
                                     this,
                                     &QGraphsTheme::appendBaseGradientsFunc,
                                     &QGraphsTheme::countBaseGradientsFunc,
                                     &QGraphsTheme::atBaseGradientsFunc,
                                     &QGraphsTheme::clearBaseGradientsFunc);
}

void QGraphsTheme::appendBaseGradientsFunc(QQmlListProperty<QObject> *list, QObject *gradient)
{
    QJSEngine engine;
    QJSValue value = engine.newQObject(gradient);
    reinterpret_cast<QGraphsTheme *>(list->data)->addGradient(value);
}

qsizetype QGraphsTheme::countBaseGradientsFunc(QQmlListProperty<QObject> *list)
{
    return reinterpret_cast<QGraphsTheme *>(list->data)->gradientList().size();
}

QObject *QGraphsTheme::atBaseGradientsFunc(QQmlListProperty<QObject> *list, qsizetype index)
{
    return reinterpret_cast<QGraphsTheme *>(list->data)->gradientList().at(index);
}

void QGraphsTheme::clearBaseGradientsFunc(QQmlListProperty<QObject> *list)
{
    reinterpret_cast<QGraphsTheme *>(list->data)->clearGradients();
}

QQmlListProperty<QObject> QGraphsTheme::themeChildren()
{
    return QQmlListProperty<QObject>(this, this, &QGraphsTheme::appendThemeChildren, 0, 0, 0);
}

void QGraphsTheme::appendThemeChildren(QQmlListProperty<QObject> *list, QObject *element)
{
    Q_UNUSED(list);
    Q_UNUSED(element);
}

void QGraphsTheme::addColor(QQuickGraphsColor *color)
{
    if (!color) {
        qWarning("Color is invalid, use Color");
        return;
    }
    clearDummyColors();
    m_colors.append(color);
    connect(color, &QQuickGraphsColor::colorChanged, this, &QGraphsTheme::handleBaseColorUpdate);
    QList<QColor> list = m_seriesColors;
    list.append(color->color());
    setSeriesColors(list);
}

QList<QQuickGraphsColor *> QGraphsTheme::colorList()
{
    if (m_colors.isEmpty()) {
        // Create dummy Colors from theme's colors
        m_dummyColors = true;
        QList<QColor> list = seriesColors();
        for (const QColor &item : list) {
            QQuickGraphsColor *color = new QQuickGraphsColor(this);
            color->setColor(item);
            m_colors.append(color);
            connect(color, &QQuickGraphsColor::colorChanged, this, &QGraphsTheme::handleBaseColorUpdate);
        }
    }
    return m_colors;
}

void QGraphsTheme::clearColors()
{
    clearDummyColors();
    for (QQuickGraphsColor *item : m_colors)
        disconnect(item, 0, this, 0);
    m_colors.clear();
    setSeriesColors(QList<QColor>());
}

void QGraphsTheme::clearDummyColors()
{
    if (m_dummyColors) {
        for (QQuickGraphsColor *item : m_colors)
            delete item;
        m_colors.clear();
        m_dummyColors = false;
    }
}

void QGraphsTheme::addGradient(QJSValue gradient)
{
    auto quickGradient = qobject_cast<QQuickGradient *>(gradient.toQObject());
    m_gradients.append(quickGradient);

    connect(quickGradient,
            &QQuickGradient::updated,
            this,
            &QGraphsTheme::handleBaseGradientUpdate);

    QList<QLinearGradient> list = m_seriesGradients;
    list.append(convertGradient(gradient));
    setSeriesGradients(list);
}

QList<QQuickGradient *> QGraphsTheme::gradientList()
{
    return m_gradients;
}

void QGraphsTheme::clearGradients()
{
    m_gradients.clear();
    setSeriesGradients(QList<QLinearGradient>());
}

QGraphsLine QGraphsTheme::grid() const
{
    return m_grid;
}

void QGraphsTheme::setGrid(const QGraphsLine &newGrid)
{
    if (m_grid == newGrid)
        return;
    m_grid = newGrid;
    m_dirtyBits.gridDirty = true;
    emit gridChanged();
    emit update();
}

QGraphsLine QGraphsTheme::axisX() const
{
    return m_axisX;
}

void QGraphsTheme::setAxisX(const QGraphsLine &newAxisX)
{
    if (m_axisX == newAxisX)
        return;
    m_axisX = newAxisX;
    m_dirtyBits.axisXDirty = true;
    emit axisXChanged();
    emit update();
}

QGraphsLine QGraphsTheme::axisY() const
{
    return m_axisY;
}

void QGraphsTheme::setAxisY(const QGraphsLine &newAxisY)
{
    if (m_axisY == newAxisY)
        return;
    m_axisY = newAxisY;
    m_dirtyBits.axisYDirty = true;
    emit axisYChanged();
    emit update();
}

QGraphsLine QGraphsTheme::axisZ() const
{
    return m_axisZ;
}

void QGraphsTheme::setAxisZ(const QGraphsLine &newAxisZ)
{
    if (m_axisZ == newAxisZ)
        return;
    m_axisZ = newAxisZ;
    m_dirtyBits.axisZDirty = true;
    emit axisZChanged();
    emit update();
}


QVariant QQuickGraphsLineValueType::create(const QJSValue &params)
{
    if (!params.isObject())
        return QVariant();

    QGraphsLine line;

    const QJSValue mainColor = params.property(QStringLiteral("mainColor"));
    if (mainColor.isString())
        line.setMainColor(QColor::fromString(mainColor.toString()));
    const QJSValue subColor = params.property(QStringLiteral("subColor"));
    if (subColor.isString())
        line.setSubColor(QColor::fromString(subColor.toString()));
    const QJSValue mainWidth = params.property(QStringLiteral("mainWidth"));
    if (mainWidth.isNumber())
        line.setMainWidth(mainWidth.toNumber());
    const QJSValue subWidth = params.property(QStringLiteral("subWidth"));
    if (subWidth.isNumber())
        line.setSubWidth(subWidth.toNumber());
    const QJSValue labelTextColor = params.property(QStringLiteral("labelTextColor"));
    if (labelTextColor.isString())
        line.setLabelTextColor(QColor::fromString(labelTextColor.toString()));

    return line;
}

QColor QQuickGraphsLineValueType::mainColor() const
{
    return v.mainColor();
}

void QQuickGraphsLineValueType::setMainColor(const QColor &newColor)
{
    v.setMainColor(newColor);
}

QColor QQuickGraphsLineValueType::subColor() const
{
    return v.subColor();
}

void QQuickGraphsLineValueType::setSubColor(const QColor &newColor)
{
    v.setSubColor(newColor);
}

qreal QQuickGraphsLineValueType::mainWidth() const
{
    return v.mainWidth();
}

void QQuickGraphsLineValueType::setMainWidth(qreal newWidth)
{
    v.setMainWidth(newWidth);
}

qreal QQuickGraphsLineValueType::subWidth() const
{
    return v.subWidth();
}

void QQuickGraphsLineValueType::setSubWidth(qreal newWidth)
{
    v.setSubWidth(newWidth);
}

QColor QQuickGraphsLineValueType::labelTextColor() const
{
    return v.labelTextColor();
}

void QQuickGraphsLineValueType::setLabelTextColor(const QColor &newColor)
{
    v.setLabelTextColor(newColor);
}

QGraphsLine::QGraphsLine()
    : m_bits(QGraphsLineCustomField())
      , m_mainColor(QColor())
      , m_subColor(QColor())
      , m_mainWidth(2.0f)
      , m_subWidth(1.0f)
      , m_labelTextColor(QColor())
      , m_mainThemeColor(QColor())
      , m_subThemeColor(QColor())
      , m_labelTextThemeColor(QColor())
{
}

QGraphsLine::QGraphsLine(const QGraphsLine &graphsLine)
{
    m_bits = graphsLine.m_bits;
    m_mainColor = graphsLine.m_mainColor;
    m_subColor = graphsLine.m_subColor;
    m_mainWidth = graphsLine.m_mainWidth;
    m_subWidth = graphsLine.m_subWidth;
    m_labelTextColor = graphsLine.m_labelTextColor;

    m_mainThemeColor = graphsLine.m_mainThemeColor;
    m_subThemeColor = graphsLine.m_subThemeColor;
    m_labelTextThemeColor = graphsLine.m_labelTextThemeColor;
}

QColor QGraphsLine::mainColor() const
{
    if (m_bits.mainColorCustom)
        return m_mainColor;
    return m_mainThemeColor;
}

void QGraphsLine::setMainColor(const QColor &newColor)
{
    m_bits.mainColorCustom = true;
    if (m_mainColor == newColor)
        return;
    m_mainColor = newColor;
}

QColor QGraphsLine::subColor() const
{
    if (m_bits.subColorCustom)
        return m_subColor;
    return m_subThemeColor;
}

void QGraphsLine::setSubColor(const QColor &newColor)
{
    m_bits.subColorCustom = true;
    if (m_subColor == newColor)
        return;
    m_subColor = newColor;
}

qreal QGraphsLine::mainWidth() const
{
    return m_mainWidth;
}

void QGraphsLine::setMainWidth(qreal newWidth)
{
    if (qFuzzyCompare(m_mainWidth, newWidth))
        return;
    m_mainWidth = newWidth;
}

qreal QGraphsLine::subWidth() const
{
    return m_subWidth;
}

void QGraphsLine::setSubWidth(qreal newWidth)
{
    if (qFuzzyCompare(m_subWidth, newWidth))
        return;
    m_subWidth = newWidth;
}

QColor QGraphsLine::labelTextColor() const
{
    if (m_bits.labelTextColorCustom)
        return m_labelTextColor;
    return m_labelTextThemeColor;
}

void QGraphsLine::setLabelTextColor(const QColor &newColor)
{
    m_bits.labelTextColorCustom = true;
    if (m_labelTextColor == newColor)
        return;
    m_labelTextColor = newColor;
}

QGraphsLine &QGraphsLine::operator=(const QGraphsLine &graphsLine)
{
    m_bits = graphsLine.m_bits;
    m_mainColor = graphsLine.m_mainColor;
    m_subColor = graphsLine.m_subColor;
    m_mainWidth = graphsLine.m_mainWidth;
    m_subWidth = graphsLine.m_subWidth;
    m_labelTextColor = graphsLine.m_labelTextColor;

    m_mainThemeColor = graphsLine.m_mainThemeColor;
    m_subThemeColor = graphsLine.m_subThemeColor;
    m_labelTextThemeColor = graphsLine.m_labelTextThemeColor;

    return *this;
}

bool QGraphsLine::operator==(const QGraphsLine &graphsLine)
{
    bool ret = true;
    ret = ret && (m_bits.mainColorCustom == graphsLine.m_bits.mainColorCustom);
    if (!ret)
        return ret;
    ret = ret && (m_bits.subColorCustom == graphsLine.m_bits.subColorCustom);
    if (!ret)
        return ret;
    ret = ret && (m_bits.labelTextColorCustom == graphsLine.m_bits.labelTextColorCustom);
    if (!ret)
        return ret;
    ret = ret && (m_mainColor == graphsLine.m_mainColor);
    if (!ret)
        return ret;
    ret = ret && (m_subColor == graphsLine.m_subColor);
    if (!ret)
        return ret;
    ret = ret && qFuzzyCompare(m_mainWidth, graphsLine.m_mainWidth);
    if (!ret)
        return ret;
    ret = ret && qFuzzyCompare(m_subWidth, graphsLine.m_subWidth);
    if (!ret)
        return ret;
    ret = ret && (m_labelTextColor == graphsLine.m_labelTextColor);
    if (!ret)
        return ret;
    ret = ret && (m_mainThemeColor == graphsLine.m_mainThemeColor);
    if (!ret)
        return ret;
    ret = ret && (m_subThemeColor == graphsLine.m_subThemeColor);
    if (!ret)
        return ret;
    ret = ret && (m_labelTextThemeColor == graphsLine.m_labelTextThemeColor);
    if (!ret)
        return ret;

    return ret;
}

QGraphsLine::operator QVariant() const
{
    return QVariant::fromValue(*this);
}

void QGraphsLine::resetCustomBits()
{
    m_bits.mainColorCustom = false;
    m_bits.subColorCustom = false;
    m_bits.labelTextColorCustom = false;
}

QT_END_NAMESPACE
