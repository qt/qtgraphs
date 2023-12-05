// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "seriestheme.h"

QT_BEGIN_NAMESPACE

SeriesTheme::SeriesTheme(QObject *parent)
    : QObject{parent}
{
}

void SeriesTheme::classBegin()
{
}

void SeriesTheme::componentComplete()
{
    // Set initial theme if not one set already
    if (!m_useCustomColors)
        setColorTheme(m_colorTheme);
    m_componentComplete = true;
}

void SeriesTheme::resetColorTheme()
{
    setColorTheme(SeriesColorTheme::SeriesTheme1);
}

SeriesTheme::SeriesColorTheme SeriesTheme::colorTheme() const
{
    return m_colorTheme;
}

void SeriesTheme::setColorTheme(const SeriesTheme::SeriesColorTheme &newColorTheme)
{
    if (m_componentComplete)
        m_themeDirty = true;

    if (m_colorTheme == newColorTheme && !m_themeDirty && m_componentComplete)
        return;

    m_colorTheme = newColorTheme;

    if (m_colorTheme == SeriesTheme::SeriesTheme1) {
        setColorTheme1();
    } else {
        setColorTheme2();
    }

    emit update();
    emit colorThemeChanged();
}

void SeriesTheme::setColorTheme1()
{
    m_colors = { "#3d9c73", "#63b179", "#88c580", "#aed987", "#d6ec91", "#ffff9d",
                       "#fee17e", "#fcc267", "#f7a258", "#ef8250", "#e4604e", "#d43d51" };
    // TODO
    m_borderColors = { "#ffffff" };
}

void SeriesTheme::setColorTheme2()
{
    m_colors = { "#00429d", "#485ba8", "#6c77b3", "#8a94be", "#a4b2ca", "#b9d4d6",
                       "#ffd3bf", "#ffa59e", "#f4777f", "#dd4c65", "#be214d", "#93003a" };
    // TODO
    m_borderColors = { "#ffffff" };
}

QColor SeriesTheme::indexColorFrom(const QList<QColor> &colors, int index) const
{
    // Select colors from theme with as much separation as possible. So:
    // - if we need 2 series from 12 color palette, select indexes [0, 11]
    // - If we need 3 series from 12 color palette, select indexes [0, 5, 11]
    if (colors.isEmpty())
        return QColor();
    if (m_seriesCount <= 1) {
        if (!colors.isEmpty())
            return colors.first();
        else
            return QColor();
    }
    int ci = (float)index * ((float)(colors.size() - 1) / (m_seriesCount - 1));
    ci = std::min(ci, (int)colors.size() - 1);
    return colors.at(ci);
}

int SeriesTheme::graphSeriesCount() const
{
    return m_seriesCount;
}
void SeriesTheme::setGraphSeriesCount(int count)
{
    m_seriesCount = count;
}


QColor SeriesTheme::graphSeriesColor(int index) const
{
    return indexColorFrom(m_colors, index);
}

QColor SeriesTheme::graphSeriesBorderColor(int index) const
{
    return indexColorFrom(m_borderColors, index);
}

QList<QColor> SeriesTheme::colors() const
{
    return m_colors;
}

void SeriesTheme::setColors(const QList<QColor> &newColors)
{
    if (m_colors == newColors)
        return;
    m_colors = newColors;
    m_useCustomColors = !m_colors.isEmpty();
    emit colorsChanged();
}

QList<QColor> SeriesTheme::borderColors() const
{
    return m_borderColors;
}

void SeriesTheme::setBorderColors(const QList<QColor> &newBorderColors)
{
    if (m_borderColors == newBorderColors)
        return;
    m_borderColors = newBorderColors;
    emit borderColorsChanged();
}

qreal SeriesTheme::borderWidth() const
{
    return m_borderWidth;
}

void SeriesTheme::setBorderWidth(qreal newBorderWidth)
{
    if (qFuzzyCompare(m_borderWidth, newBorderWidth))
        return;
    m_borderWidth = newBorderWidth;
    emit borderWidthChanged();
}

QT_END_NAMESPACE
