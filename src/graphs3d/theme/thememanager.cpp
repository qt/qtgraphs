// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "q3dtheme_p.h"
#include "qquickgraphsitem_p.h"
#include "thememanager_p.h"

QT_BEGIN_NAMESPACE

const float defaultBuiltInColorLevel = 0.7f; // for built-in gradient themes
const float defaultColorLevel = 0.5f;        // for built-in uniform themes

ThemeManager::ThemeManager(QQuickGraphsItem *graph)
    : m_activeTheme(0)
    , m_graph(graph)
{}

ThemeManager::~ThemeManager() {}

void ThemeManager::addTheme(Q3DTheme *theme)
{
    Q_ASSERT(theme);
    ThemeManager *owner = qobject_cast<ThemeManager *>(theme->parent());
    if (owner != this) {
        Q_ASSERT_X(!owner, "addTheme", "Theme already attached to a graph.");
        theme->setParent(this);
    }
    if (!m_themes.contains(theme))
        m_themes.append(theme);
}

void ThemeManager::releaseTheme(Q3DTheme *theme)
{
    if (theme && m_themes.contains(theme)) {
        // Clear the default status from released default theme
        if (theme->d_func()->isDefaultTheme())
            theme->d_func()->setDefaultTheme(false);

        // If the theme is in use, replace it with a temporary one
        if (theme == m_activeTheme)
            setActiveTheme(0);

        m_themes.removeAll(theme);
        theme->setParent(0);
    }
}

void ThemeManager::setActiveTheme(Q3DTheme *theme)
{
    // Setting null theme indicates using default theme
    if (!theme) {
        theme = new Q3DTheme;
        theme->d_func()->setDefaultTheme(true);
    }

    // If the old theme is default theme, delete it
    Q3DTheme *oldTheme = m_activeTheme;
    if (oldTheme) {
        if (oldTheme->d_func()->isDefaultTheme()) {
            m_themes.removeAll(oldTheme);
            delete oldTheme;
            oldTheme = 0;
        } else {
            // Disconnect the old theme from use
            disconnect(m_activeTheme, 0, m_graph, 0);
        }
    }

    // Assume ownership
    addTheme(theme);

    m_activeTheme = theme;

    // Reset all bits to dirty for sync
    if (theme->d_func()->isForcePredefinedType())
        m_activeTheme->d_func()->resetDirtyBits();

    // Connect signals from new one
    connectThemeSignals();
}

Q3DTheme *ThemeManager::activeTheme() const
{
    return m_activeTheme;
}

QList<Q3DTheme *> ThemeManager::themes() const
{
    return m_themes;
}

void ThemeManager::connectThemeSignals()
{
    connect(m_activeTheme,
            &Q3DTheme::colorStyleChanged,
            m_graph,
            &QQuickGraphsItem::handleThemeColorStyleChanged);
    connect(m_activeTheme,
            &Q3DTheme::baseColorsChanged,
            m_graph,
            &QQuickGraphsItem::handleThemeBaseColorsChanged);
    connect(m_activeTheme,
            &Q3DTheme::singleHighlightColorChanged,
            m_graph,
            &QQuickGraphsItem::handleThemeSingleHighlightColorChanged);
    connect(m_activeTheme,
            &Q3DTheme::multiHighlightColorChanged,
            m_graph,
            &QQuickGraphsItem::handleThemeMultiHighlightColorChanged);
    connect(m_activeTheme,
            &Q3DTheme::baseGradientsChanged,
            m_graph,
            &QQuickGraphsItem::handleThemeBaseGradientsChanged);
    connect(m_activeTheme,
            &Q3DTheme::singleHighlightGradientChanged,
            m_graph,
            &QQuickGraphsItem::handleThemeSingleHighlightGradientChanged);
    connect(m_activeTheme,
            &Q3DTheme::multiHighlightGradientChanged,
            m_graph,
            &QQuickGraphsItem::handleThemeMultiHighlightGradientChanged);
    connect(m_activeTheme,
            &Q3DTheme::typeChanged,
            m_graph,
            &QQuickGraphsItem::handleThemeTypeChanged);

    connect(m_activeTheme, &Q3DTheme::needRender, m_graph, &QQuickGraphsItem::needRender);
}

void ThemeManager::setPredefinedPropertiesToTheme(Q3DTheme *theme, Q3DTheme::Theme type)
{
    QList<QColor> baseColors;
    QList<QLinearGradient> baseGradients;
    switch (type) {
    case Q3DTheme::Theme::Qt: {
        baseColors.append(QColor(QRgb(0x80c342)));
        baseColors.append(QColor(QRgb(0x469835)));
        baseColors.append(QColor(QRgb(0x006325)));
        baseColors.append(QColor(QRgb(0x5caa15)));
        baseColors.append(QColor(QRgb(0x328930)));

        baseGradients.append(createGradient(baseColors.at(0), defaultColorLevel));
        baseGradients.append(createGradient(baseColors.at(1), defaultColorLevel));
        baseGradients.append(createGradient(baseColors.at(2), defaultColorLevel));
        baseGradients.append(createGradient(baseColors.at(3), defaultColorLevel));
        baseGradients.append(createGradient(baseColors.at(4), defaultColorLevel));

        setBackgroundEnabled(theme, true);
        setGridEnabled(theme, true);
        setFont(theme, QFont(QStringLiteral("Arial")));
        setLabelBackgroundEnabled(theme, true);
        setLightColor(theme, Qt::white);
        setBaseColors(theme, baseColors);
        setBackgroundColor(theme, QColor(QRgb(0xffffff)));
        setWindowColor(theme, QColor(QRgb(0xffffff)));
        setTextColor(theme, QColor(QRgb(0x35322f)));
        setTextBackgroundColor(theme, QColor(0xff, 0xff, 0xff, 0x99));
        setGridLineColor(theme, QColor(QRgb(0xd7d6d5)));
        setSingleHighlightColor(theme, QColor(QRgb(0x14aaff)));
        setMultiHighlightColor(theme, QColor(QRgb(0x6400aa)));
        setLightStrength(theme, 5.0f);
        setAmbientLightStrength(theme, 0.5f);
        setLabelBorderEnabled(theme, true);
        setLabelsEnabled(theme, true);
        setColorStyle(theme, Q3DTheme::ColorStyle::Uniform);
        setBaseGradients(theme, baseGradients);
        setSingleHighlightGradient(theme, createGradient(QColor(QRgb(0x14aaff)), defaultColorLevel));
        setMultiHighlightGradient(theme, createGradient(QColor(QRgb(0x6400aa)), defaultColorLevel));
        break;
    }

    case Q3DTheme::Theme::PrimaryColors: {
        baseColors.append(QColor(QRgb(0xffe400)));
        baseColors.append(QColor(QRgb(0xfaa106)));
        baseColors.append(QColor(QRgb(0xf45f0d)));
        baseColors.append(QColor(QRgb(0xfcba04)));
        baseColors.append(QColor(QRgb(0xf7800a)));

        baseGradients.append(createGradient(baseColors.at(0), defaultColorLevel));
        baseGradients.append(createGradient(baseColors.at(1), defaultColorLevel));
        baseGradients.append(createGradient(baseColors.at(2), defaultColorLevel));
        baseGradients.append(createGradient(baseColors.at(3), defaultColorLevel));
        baseGradients.append(createGradient(baseColors.at(4), defaultColorLevel));

        setBackgroundEnabled(theme, true);
        setGridEnabled(theme, true);
        setFont(theme, QFont(QStringLiteral("Arial")));
        setLabelBackgroundEnabled(theme, true);
        setLightColor(theme, Qt::white);
        setBaseColors(theme, baseColors);
        setBackgroundColor(theme, QColor(QRgb(0xffffff)));
        setWindowColor(theme, QColor(QRgb(0xffffff)));
        setTextColor(theme, QColor(QRgb(0x000000)));
        setTextBackgroundColor(theme, QColor(0xff, 0xff, 0xff, 0x99));
        setGridLineColor(theme, QColor(QRgb(0xd7d6d5)));
        setSingleHighlightColor(theme, QColor(QRgb(0x27beee)));
        setMultiHighlightColor(theme, QColor(QRgb(0xee1414)));
        setLightStrength(theme, 5.0f);
        setAmbientLightStrength(theme, 0.5f);
        setLabelBorderEnabled(theme, false);
        setLabelsEnabled(theme, true);
        setColorStyle(theme, Q3DTheme::ColorStyle::Uniform);
        setBaseGradients(theme, baseGradients);
        setSingleHighlightGradient(theme, createGradient(QColor(QRgb(0x27beee)), defaultColorLevel));
        setMultiHighlightGradient(theme, createGradient(QColor(QRgb(0xee1414)), defaultColorLevel));
        break;
    }

    case Q3DTheme::Theme::StoneMoss: {
        baseColors.append(QColor(QRgb(0xbeb32b)));
        baseColors.append(QColor(QRgb(0x928327)));
        baseColors.append(QColor(QRgb(0x665423)));
        baseColors.append(QColor(QRgb(0xa69929)));
        baseColors.append(QColor(QRgb(0x7c6c25)));

        baseGradients.append(createGradient(baseColors.at(0), defaultColorLevel));
        baseGradients.append(createGradient(baseColors.at(1), defaultColorLevel));
        baseGradients.append(createGradient(baseColors.at(2), defaultColorLevel));
        baseGradients.append(createGradient(baseColors.at(3), defaultColorLevel));
        baseGradients.append(createGradient(baseColors.at(4), defaultColorLevel));

        setBackgroundEnabled(theme, true);
        setGridEnabled(theme, true);
        setFont(theme, QFont(QStringLiteral("Arial")));
        setLabelBackgroundEnabled(theme, true);
        setLightColor(theme, Qt::white);
        setBaseColors(theme, baseColors);
        setBackgroundColor(theme, QColor(QRgb(0x4d4d4f)));
        setWindowColor(theme, QColor(QRgb(0x4d4d4f)));
        setTextColor(theme, QColor(QRgb(0xffffff)));
        setTextBackgroundColor(theme, QColor(0x4d, 0x4d, 0x4f, 0xcd));
        setGridLineColor(theme, QColor(QRgb(0x3e3e40)));
        setSingleHighlightColor(theme, QColor(QRgb(0xfbf6d6)));
        setMultiHighlightColor(theme, QColor(QRgb(0x442f20)));
        setLightStrength(theme, 5.0f);
        setAmbientLightStrength(theme, 0.5f);
        setLabelBorderEnabled(theme, true);
        setLabelsEnabled(theme, true);
        setColorStyle(theme, Q3DTheme::ColorStyle::Uniform);
        setBaseGradients(theme, baseGradients);
        setSingleHighlightGradient(theme, createGradient(QColor(QRgb(0xfbf6d6)), defaultColorLevel));
        setMultiHighlightGradient(theme, createGradient(QColor(QRgb(0x442f20)), defaultColorLevel));
        break;
    }

    case Q3DTheme::Theme::ArmyBlue: {
        baseColors.append(QColor(QRgb(0x495f76)));
        baseColors.append(QColor(QRgb(0x81909f)));
        baseColors.append(QColor(QRgb(0xbec5cd)));
        baseColors.append(QColor(QRgb(0x687a8d)));
        baseColors.append(QColor(QRgb(0xa3aeb9)));

        baseGradients.append(createGradient(baseColors.at(0), defaultBuiltInColorLevel));
        baseGradients.append(createGradient(baseColors.at(1), defaultBuiltInColorLevel));
        baseGradients.append(createGradient(baseColors.at(2), defaultBuiltInColorLevel));
        baseGradients.append(createGradient(baseColors.at(3), defaultBuiltInColorLevel));
        baseGradients.append(createGradient(baseColors.at(4), defaultBuiltInColorLevel));

        setBackgroundEnabled(theme, true);
        setGridEnabled(theme, true);
        setFont(theme, QFont(QStringLiteral("Arial")));
        setLabelBackgroundEnabled(theme, true);
        setLightColor(theme, Qt::white);
        setBaseColors(theme, baseColors);
        setBackgroundColor(theme, QColor(QRgb(0xd5d6d7)));
        setWindowColor(theme, QColor(QRgb(0xd5d6d7)));
        setTextColor(theme, QColor(QRgb(0x000000)));
        setTextBackgroundColor(theme, QColor(0xd5, 0xd6, 0xd7, 0xcd));
        setGridLineColor(theme, QColor(QRgb(0xaeadac)));
        setSingleHighlightColor(theme, QColor(QRgb(0x2aa2f9)));
        setMultiHighlightColor(theme, QColor(QRgb(0x103753)));
        setLightStrength(theme, 5.0f);
        setAmbientLightStrength(theme, 0.5f);
        setLabelBorderEnabled(theme, false);
        setLabelsEnabled(theme, true);
        setColorStyle(theme, Q3DTheme::ColorStyle::ObjectGradient);
        setBaseGradients(theme, baseGradients);
        setSingleHighlightGradient(theme,
                                   createGradient(QColor(QRgb(0x2aa2f9)), defaultBuiltInColorLevel));
        setMultiHighlightGradient(theme,
                                  createGradient(QColor(QRgb(0x103753)), defaultBuiltInColorLevel));
        break;
    }

    case Q3DTheme::Theme::Retro: {
        baseColors.append(QColor(QRgb(0x533b23)));
        baseColors.append(QColor(QRgb(0x83715a)));
        baseColors.append(QColor(QRgb(0xb3a690)));
        baseColors.append(QColor(QRgb(0x6b563e)));
        baseColors.append(QColor(QRgb(0x9b8b75)));

        baseGradients.append(createGradient(baseColors.at(0), defaultBuiltInColorLevel));
        baseGradients.append(createGradient(baseColors.at(1), defaultBuiltInColorLevel));
        baseGradients.append(createGradient(baseColors.at(2), defaultBuiltInColorLevel));
        baseGradients.append(createGradient(baseColors.at(3), defaultBuiltInColorLevel));
        baseGradients.append(createGradient(baseColors.at(4), defaultBuiltInColorLevel));

        setBackgroundEnabled(theme, true);
        setGridEnabled(theme, true);
        setFont(theme, QFont(QStringLiteral("Arial")));
        setLabelBackgroundEnabled(theme, true);
        setLightColor(theme, Qt::white);
        setBaseColors(theme, baseColors);
        setBackgroundColor(theme, QColor(QRgb(0xe9e2ce)));
        setWindowColor(theme, QColor(QRgb(0xe9e2ce)));
        setTextColor(theme, QColor(QRgb(0x000000)));
        setTextBackgroundColor(theme, QColor(0xe9, 0xe2, 0xce, 0xc0));
        setGridLineColor(theme, QColor(QRgb(0xd0c0b0)));
        setSingleHighlightColor(theme, QColor(QRgb(0x8ea317)));
        setMultiHighlightColor(theme, QColor(QRgb(0xc25708)));
        setLightStrength(theme, 5.0f);
        setAmbientLightStrength(theme, 0.5f);
        setLabelBorderEnabled(theme, false);
        setLabelsEnabled(theme, true);
        setColorStyle(theme, Q3DTheme::ColorStyle::ObjectGradient);
        setBaseGradients(theme, baseGradients);
        setSingleHighlightGradient(theme,
                                   createGradient(QColor(QRgb(0x8ea317)), defaultBuiltInColorLevel));
        setMultiHighlightGradient(theme,
                                  createGradient(QColor(QRgb(0xc25708)), defaultBuiltInColorLevel));
        break;
    }

    case Q3DTheme::Theme::Ebony: {
        baseColors.append(QColor(QRgb(0xffffff)));
        baseColors.append(QColor(QRgb(0x999999)));
        baseColors.append(QColor(QRgb(0x474747)));
        baseColors.append(QColor(QRgb(0xc7c7c7)));
        baseColors.append(QColor(QRgb(0x6b6b6b)));

        baseGradients.append(createGradient(baseColors.at(0), defaultColorLevel));
        baseGradients.append(createGradient(baseColors.at(1), defaultColorLevel));
        baseGradients.append(createGradient(baseColors.at(2), defaultColorLevel));
        baseGradients.append(createGradient(baseColors.at(3), defaultColorLevel));
        baseGradients.append(createGradient(baseColors.at(4), defaultColorLevel));

        setBackgroundEnabled(theme, true);
        setGridEnabled(theme, true);
        setFont(theme, QFont(QStringLiteral("Arial")));
        setLabelBackgroundEnabled(theme, true);
        setLightColor(theme, Qt::white);
        setBaseColors(theme, baseColors);
        setBackgroundColor(theme, QColor(QRgb(0x000000)));
        setWindowColor(theme, QColor(QRgb(0x000000)));
        setTextColor(theme, QColor(QRgb(0xaeadac)));
        setTextBackgroundColor(theme, QColor(0x00, 0x00, 0x00, 0xcd));
        setGridLineColor(theme, QColor(QRgb(0x35322f)));
        setSingleHighlightColor(theme, QColor(QRgb(0xf5dc0d)));
        setMultiHighlightColor(theme, QColor(QRgb(0xd72222)));
        setLightStrength(theme, 5.0f);
        setAmbientLightStrength(theme, 0.5f);
        setLabelBorderEnabled(theme, false);
        setLabelsEnabled(theme, true);
        setColorStyle(theme, Q3DTheme::ColorStyle::Uniform);
        setBaseGradients(theme, baseGradients);
        setSingleHighlightGradient(theme, createGradient(QColor(QRgb(0xf5dc0d)), defaultColorLevel));
        setMultiHighlightGradient(theme, createGradient(QColor(QRgb(0xd72222)), defaultColorLevel));
        break;
    }

    case Q3DTheme::Theme::Isabelle: {
        baseColors.append(QColor(QRgb(0xf9d900)));
        baseColors.append(QColor(QRgb(0xf09603)));
        baseColors.append(QColor(QRgb(0xe85506)));
        baseColors.append(QColor(QRgb(0xf5b802)));
        baseColors.append(QColor(QRgb(0xec7605)));

        baseGradients.append(createGradient(baseColors.at(0), defaultColorLevel));
        baseGradients.append(createGradient(baseColors.at(1), defaultColorLevel));
        baseGradients.append(createGradient(baseColors.at(2), defaultColorLevel));
        baseGradients.append(createGradient(baseColors.at(3), defaultColorLevel));
        baseGradients.append(createGradient(baseColors.at(4), defaultColorLevel));

        setBackgroundEnabled(theme, true);
        setGridEnabled(theme, true);
        setFont(theme, QFont(QStringLiteral("Arial")));
        setLabelBackgroundEnabled(theme, true);
        setLightColor(theme, Qt::white);
        setBaseColors(theme, baseColors);
        setBackgroundColor(theme, QColor(QRgb(0x000000)));
        setWindowColor(theme, QColor(QRgb(0x000000)));
        setTextColor(theme, QColor(QRgb(0xaeadac)));
        setTextBackgroundColor(theme, QColor(0x00, 0x00, 0x00, 0xc0));
        setGridLineColor(theme, QColor(QRgb(0x35322f)));
        setSingleHighlightColor(theme, QColor(QRgb(0xfff7cc)));
        setMultiHighlightColor(theme, QColor(QRgb(0xde0a0a)));
        setLightStrength(theme, 5.0f);
        setAmbientLightStrength(theme, 0.5f);
        setLabelBorderEnabled(theme, false);
        setLabelsEnabled(theme, true);
        setColorStyle(theme, Q3DTheme::ColorStyle::Uniform);
        setBaseGradients(theme, baseGradients);
        setSingleHighlightGradient(theme, createGradient(QColor(QRgb(0xfff7cc)), defaultColorLevel));
        setMultiHighlightGradient(theme, createGradient(QColor(QRgb(0xde0a0a)), defaultColorLevel));
        break;
    }
    default:
        break;
    }
}

QLinearGradient ThemeManager::createGradient(const QColor &color, float colorLevel)
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

void ThemeManager::setBaseColors(Q3DTheme *theme, const QList<QColor> &colors)
{
    if (theme->d_func()->isForcePredefinedType() || !theme->d_func()->m_dirtyBits.baseColorDirty)
        theme->setBaseColors(colors);
}

void ThemeManager::setBackgroundColor(Q3DTheme *theme, const QColor &color)
{
    if (theme->d_func()->isForcePredefinedType()
        || !theme->d_func()->m_dirtyBits.backgroundColorDirty) {
        theme->setBackgroundColor(color);
    }
}

void ThemeManager::setWindowColor(Q3DTheme *theme, const QColor &color)
{
    if (theme->d_func()->isForcePredefinedType() || !theme->d_func()->m_dirtyBits.windowColorDirty)
        theme->setWindowColor(color);
}

void ThemeManager::setTextColor(Q3DTheme *theme, const QColor &color)
{
    if (theme->d_func()->isForcePredefinedType()
        || !theme->d_func()->m_dirtyBits.labelTextColorDirty) {
        theme->setLabelTextColor(color);
    }
}

void ThemeManager::setTextBackgroundColor(Q3DTheme *theme, const QColor &color)
{
    if (theme->d_func()->isForcePredefinedType()
        || !theme->d_func()->m_dirtyBits.labelBackgroundColorDirty) {
        theme->setLabelBackgroundColor(color);
    }
}

void ThemeManager::setGridLineColor(Q3DTheme *theme, const QColor &color)
{
    if (theme->d_func()->isForcePredefinedType() || !theme->d_func()->m_dirtyBits.gridLineColorDirty)
        theme->setGridLineColor(color);
}

void ThemeManager::setSingleHighlightColor(Q3DTheme *theme, const QColor &color)
{
    if (theme->d_func()->isForcePredefinedType()
        || !theme->d_func()->m_dirtyBits.singleHighlightColorDirty) {
        theme->setSingleHighlightColor(color);
    }
}

void ThemeManager::setMultiHighlightColor(Q3DTheme *theme, const QColor &color)
{
    if (theme->d_func()->isForcePredefinedType()
        || !theme->d_func()->m_dirtyBits.multiHighlightColorDirty) {
        theme->setMultiHighlightColor(color);
    }
}

void ThemeManager::setLightColor(Q3DTheme *theme, const QColor &color)
{
    if (theme->d_func()->isForcePredefinedType() || !theme->d_func()->m_dirtyBits.lightColorDirty)
        theme->setLightColor(color);
}

void ThemeManager::setBaseGradients(Q3DTheme *theme, const QList<QLinearGradient> &gradients)
{
    if (theme->d_func()->isForcePredefinedType() || !theme->d_func()->m_dirtyBits.baseGradientDirty)
        theme->setBaseGradients(gradients);
}

void ThemeManager::setSingleHighlightGradient(Q3DTheme *theme, const QLinearGradient &gradient)
{
    if (theme->d_func()->isForcePredefinedType()
        || !theme->d_func()->m_dirtyBits.singleHighlightGradientDirty) {
        theme->setSingleHighlightGradient(gradient);
    }
}

void ThemeManager::setMultiHighlightGradient(Q3DTheme *theme, const QLinearGradient &gradient)
{
    if (theme->d_func()->isForcePredefinedType()
        || !theme->d_func()->m_dirtyBits.multiHighlightGradientDirty) {
        theme->setMultiHighlightGradient(gradient);
    }
}

void ThemeManager::setLightStrength(Q3DTheme *theme, float strength)
{
    if (theme->d_func()->isForcePredefinedType() || !theme->d_func()->m_dirtyBits.lightStrengthDirty)
        theme->setLightStrength(strength);
}

void ThemeManager::setAmbientLightStrength(Q3DTheme *theme, float strength)
{
    if (theme->d_func()->isForcePredefinedType()
        || !theme->d_func()->m_dirtyBits.ambientLightStrengthDirty) {
        theme->setAmbientLightStrength(strength);
    }
}

void ThemeManager::setLabelBorderEnabled(Q3DTheme *theme, bool enabled)
{
    if (theme->d_func()->isForcePredefinedType()
        || !theme->d_func()->m_dirtyBits.labelBorderEnabledDirty) {
        theme->setLabelBorderEnabled(enabled);
    }
}

void ThemeManager::setFont(Q3DTheme *theme, const QFont &font)
{
    if (theme->d_func()->isForcePredefinedType() || !theme->d_func()->m_dirtyBits.fontDirty)
        theme->setFont(font);
}

void ThemeManager::setBackgroundEnabled(Q3DTheme *theme, bool enabled)
{
    if (theme->d_func()->isForcePredefinedType()
        || !theme->d_func()->m_dirtyBits.backgroundEnabledDirty) {
        theme->setBackgroundEnabled(enabled);
    }
}

void ThemeManager::setGridEnabled(Q3DTheme *theme, bool enabled)
{
    if (theme->d_func()->isForcePredefinedType() || !theme->d_func()->m_dirtyBits.gridEnabledDirty)
        theme->setGridEnabled(enabled);
}

void ThemeManager::setLabelBackgroundEnabled(Q3DTheme *theme, bool enabled)
{
    if (theme->d_func()->isForcePredefinedType()
        || !theme->d_func()->m_dirtyBits.labelBackgroundEnabledDirty) {
        theme->setLabelBackgroundEnabled(enabled);
    }
}

void ThemeManager::setColorStyle(Q3DTheme *theme, Q3DTheme::ColorStyle style)
{
    if (theme->d_func()->isForcePredefinedType() || !theme->d_func()->m_dirtyBits.colorStyleDirty)
        theme->setColorStyle(style);
}

void ThemeManager::setLabelsEnabled(Q3DTheme *theme, bool enabled)
{
    if (theme->d_func()->isForcePredefinedType() || !theme->d_func()->m_dirtyBits.labelsEnabledDirty)
        theme->setLabelsEnabled(enabled);
}

QT_END_NAMESPACE
