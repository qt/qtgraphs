// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "graphtheme.h"

QT_BEGIN_NAMESPACE

GraphTheme::GraphTheme(QObject *parent)
    : QObject{parent}
{
}

void GraphTheme::classBegin()
{
}

void GraphTheme::componentComplete()
{
    // Set initial theme if not one set already
    resetColorTheme();
    m_componentComplete = true;
}

void GraphTheme::updateTheme()
{
    m_themeDirty = true;
    emit update();
}

void GraphTheme::resetColorTheme()
{
    setColorTheme(GraphTheme::ColorThemeDark);
}

GraphTheme::ColorTheme GraphTheme::colorTheme() const
{
    return m_colorTheme;
}

void GraphTheme::setColorTheme(const ColorTheme &newColorTheme)
{
    if (m_componentComplete) {
        qDebug() << "reseting theme!";
        m_customFlags = GraphTheme::CustomFlags();
        m_themeDirty = true;
    }

    if (m_colorTheme == newColorTheme && !m_themeDirty && m_componentComplete)
        return;
    m_colorTheme = newColorTheme;

    if (m_colorTheme == GraphTheme::ColorThemeLight) {
        setColorThemeLight();
    } else {
        setColorThemeDark();
    }

    update();
    emit colorThemeChanged();
}

// Theme that it suitable on top of light backgrounds
void GraphTheme::setColorThemeLight()
{
    if (!m_customFlags.gridMajorBarsColor)
        setGridMajorBarsColor(QColor(20, 20, 20));
    if (!m_customFlags.gridMinorBarsColor)
        setGridMinorBarsColor(QColor(50, 50, 50));
    if (!m_customFlags.axisYMajorColor)
        setAxisYMajorColor(QColor(20, 20, 20));
    if (!m_customFlags.axisYMinorColor)
        setAxisYMinorColor(QColor(50, 50, 50));
    if (!m_customFlags.axisYLabelsColor)
        setAxisYLabelsColor(QColor(20, 20, 20));
    if (!m_customFlags.axisXMajorColor)
        setAxisXMajorColor(QColor(20, 20, 20));
    if (!m_customFlags.axisXMinorColor)
        setAxisXMinorColor(QColor(50, 50, 50));
    if (!m_customFlags.axisXLabelsColor)
        setAxisXLabelsColor(QColor(20, 20, 20));
}

// Theme that it suitable on top of dark backgrounds
void GraphTheme::setColorThemeDark()
{
    if (!m_customFlags.gridMajorBarsColor)
        setGridMajorBarsColor(QColor(250, 250, 250));
    if (!m_customFlags.gridMinorBarsColor)
        setGridMinorBarsColor(QColor(150, 150, 150));
    if (!m_customFlags.axisYMajorColor)
        setAxisYMajorColor(QColor(250, 250, 250));
    if (!m_customFlags.axisYMinorColor)
        setAxisYMinorColor(QColor(150, 150, 150));
    if (!m_customFlags.axisYLabelsColor)
        setAxisYLabelsColor(QColor(250, 250, 250));
    if (!m_customFlags.axisXMajorColor)
        setAxisXMajorColor(QColor(250, 250, 250));
    if (!m_customFlags.axisXMinorColor)
        setAxisXMinorColor(QColor(150, 150, 150));
    if (!m_customFlags.axisXLabelsColor)
        setAxisXLabelsColor(QColor(250, 250, 250));
}

qreal GraphTheme::gridMajorBarsWidth() const
{
    return m_gridMajorBarsWidth;
}

void GraphTheme::setGridMajorBarsWidth(qreal newGridMajorBarsWidth)
{
    if (qFuzzyCompare(m_gridMajorBarsWidth, newGridMajorBarsWidth))
        return;
    m_gridMajorBarsWidth = newGridMajorBarsWidth;
    updateTheme();
    emit gridMajorBarsWidthChanged();
}

qreal GraphTheme::gridMinorBarsWidth() const
{
    return m_gridMinorBarsWidth;
}

void GraphTheme::setGridMinorBarsWidth(qreal newGridMinorBarsWidth)
{
    if (qFuzzyCompare(m_gridMinorBarsWidth, newGridMinorBarsWidth))
        return;
    m_gridMinorBarsWidth = newGridMinorBarsWidth;
    updateTheme();
    emit gridMinorBarsWidthChanged();
}

qreal GraphTheme::gridSmoothing() const
{
    return m_gridSmoothing;
}

void GraphTheme::setGridSmoothing(qreal newGridSmoothing)
{
    if (qFuzzyCompare(m_gridSmoothing, newGridSmoothing))
        return;
    m_gridSmoothing = newGridSmoothing;
    updateTheme();
    emit gridSmoothingChanged();
}

QColor GraphTheme::gridMajorBarsColor() const
{
    return m_gridMajorBarsColor;
}

void GraphTheme::setGridMajorBarsColor(const QColor &newGridMajorBarsColor)
{
    if (m_gridMajorBarsColor == newGridMajorBarsColor)
        return;
    m_gridMajorBarsColor = newGridMajorBarsColor;
    m_customFlags.gridMajorBarsColor = true;
    updateTheme();
    emit gridMajorBarsColorChanged();
}

QColor GraphTheme::gridMinorBarsColor() const
{
    return m_gridMinorBarsColor;
}

void GraphTheme::setGridMinorBarsColor(const QColor &newGridMinorBarsColor)
{
    if (m_gridMinorBarsColor == newGridMinorBarsColor)
        return;
    m_gridMinorBarsColor = newGridMinorBarsColor;
    m_customFlags.gridMinorBarsColor = true;
    updateTheme();
    emit gridMinorBarsColorChanged();
}

QColor GraphTheme::axisYMajorColor() const
{
    return m_axisYMajorColor;
}

void GraphTheme::setAxisYMajorColor(const QColor &newAxisYMajorColor)
{
    if (m_axisYMajorColor == newAxisYMajorColor)
        return;
    m_axisYMajorColor = newAxisYMajorColor;
    m_customFlags.axisYMajorColor = true;
    updateTheme();
    emit axisYMajorColorChanged();
}

QColor GraphTheme::axisYMinorColor() const
{
    return m_axisYMinorColor;
}

void GraphTheme::setAxisYMinorColor(const QColor &newAxisYMinorColor)
{
    if (m_axisYMinorColor == newAxisYMinorColor)
        return;
    m_axisYMinorColor = newAxisYMinorColor;
    m_customFlags.axisYMinorColor = true;
    updateTheme();
    emit axisYMinorColorChanged();
}

qreal GraphTheme::axisYMajorBarWidth() const
{
    return m_axisYMajorBarWidth;
}

void GraphTheme::setAxisYMajorBarWidth(qreal newAxisYMajorBarWidth)
{
    if (qFuzzyCompare(m_axisYMajorBarWidth, newAxisYMajorBarWidth))
        return;
    m_axisYMajorBarWidth = newAxisYMajorBarWidth;
    updateTheme();
    emit axisYMajorBarWidthChanged();
}

qreal GraphTheme::axisYMinorBarWidth() const
{
    return m_axisYMinorBarWidth;
}

void GraphTheme::setAxisYMinorBarWidth(qreal newAxisYMinorBarWidth)
{
    if (qFuzzyCompare(m_axisYMinorBarWidth, newAxisYMinorBarWidth))
        return;
    m_axisYMinorBarWidth = newAxisYMinorBarWidth;
    updateTheme();
    emit axisYMinorBarWidthChanged();
}

qreal GraphTheme::axisYSmoothing() const
{
    return m_axisYSmoothing;
}

void GraphTheme::setAxisYSmoothing(qreal newAxisYSmoothing)
{
    if (qFuzzyCompare(m_axisYSmoothing, newAxisYSmoothing))
        return;
    m_axisYSmoothing = newAxisYSmoothing;
    updateTheme();
    emit axisYSmoothingChanged();
}

QColor GraphTheme::axisYLabelsColor() const
{
    return m_axisYLabelsColor;
}

void GraphTheme::setAxisYLabelsColor(const QColor &newAxisYLabelsColor)
{
    if (m_axisYLabelsColor == newAxisYLabelsColor)
        return;
    m_axisYLabelsColor = newAxisYLabelsColor;
    m_customFlags.axisYLabelsColor = true;
    updateTheme();
    emit axisYLabelsColorChanged();
}

QFont GraphTheme::axisYLabelsFont() const
{
    return m_axisYLabelsFont;
}

void GraphTheme::setAxisYLabelsFont(const QFont &newAxisYLabelsFont)
{
    if (m_axisYLabelsFont == newAxisYLabelsFont)
        return;
    m_axisYLabelsFont = newAxisYLabelsFont;
    updateTheme();
    emit axisYLabelsFontChanged();
}

QColor GraphTheme::axisXMajorColor() const
{
    return m_axisXMajorColor;
}

void GraphTheme::setAxisXMajorColor(const QColor &newAxisXMajorColor)
{
    if (m_axisXMajorColor == newAxisXMajorColor)
        return;
    m_axisXMajorColor = newAxisXMajorColor;
    m_customFlags.axisXMajorColor = true;
    updateTheme();
    emit axisXMajorColorChanged();
}

QColor GraphTheme::axisXMinorColor() const
{
    return m_axisXMinorColor;
}

void GraphTheme::setAxisXMinorColor(const QColor &newAxisXMinorColor)
{
    if (m_axisXMinorColor == newAxisXMinorColor)
        return;
    m_axisXMinorColor = newAxisXMinorColor;
    m_customFlags.axisXMinorColor = true;
    updateTheme();
    emit axisXMinorColorChanged();
}

qreal GraphTheme::axisXMajorBarWidth() const
{
    return m_axisXMajorBarWidth;
}

void GraphTheme::setAxisXMajorBarWidth(qreal newAxisXMajorBarWidth)
{
    if (qFuzzyCompare(m_axisXMajorBarWidth, newAxisXMajorBarWidth))
        return;
    m_axisXMajorBarWidth = newAxisXMajorBarWidth;
    updateTheme();
    emit axisXMajorBarWidthChanged();
}

qreal GraphTheme::axisXMinorBarWidth() const
{
    return m_axisXMinorBarWidth;
}

void GraphTheme::setAxisXMinorBarWidth(qreal newAxisXMinorBarWidth)
{
    if (qFuzzyCompare(m_axisXMinorBarWidth, newAxisXMinorBarWidth))
        return;
    m_axisXMinorBarWidth = newAxisXMinorBarWidth;
    updateTheme();
    emit axisXMinorBarWidthChanged();
}

qreal GraphTheme::axisXSmoothing() const
{
    return m_axisXSmoothing;
}

void GraphTheme::setAxisXSmoothing(qreal newAxisXSmoothing)
{
    if (qFuzzyCompare(m_axisXSmoothing, newAxisXSmoothing))
        return;
    m_axisXSmoothing = newAxisXSmoothing;
    updateTheme();
    emit axisXSmoothingChanged();
}

QColor GraphTheme::axisXLabelsColor() const
{
    return m_axisXLabelsColor;
}

void GraphTheme::setAxisXLabelsColor(const QColor &newAxisXLabelsColor)
{
    if (m_axisXLabelsColor == newAxisXLabelsColor)
        return;
    m_axisXLabelsColor = newAxisXLabelsColor;
    m_customFlags.axisXLabelsColor = true;
    updateTheme();
    emit axisXLabelsColorChanged();
}

QFont GraphTheme::axisXLabelsFont() const
{
    return m_axisXLabelsFont;
}

void GraphTheme::setAxisXLabelsFont(const QFont &newAxisXLabelsFont)
{
    if (m_axisXLabelsFont == newAxisXLabelsFont)
        return;
    m_axisXLabelsFont = newAxisXLabelsFont;
    updateTheme();
    emit axisXLabelsFontChanged();
}

QT_END_NAMESPACE
