// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
// Qt-Security score:significant reason:default


#include "axisticker_p.h"

#include <QtGraphs/QAbstractAxis>

QT_BEGIN_NAMESPACE

AxisTicker::AxisTicker(QQuickItem *parent) :
      QQuickShaderEffect(parent)
{
}

AxisTicker::~AxisTicker() {}

void AxisTicker::componentComplete()
{
    QQuickShaderEffect::componentComplete();
    setupShaders();
}

void AxisTicker::geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    m_iResolution = QVector3D(newGeometry.width(), newGeometry.height(), 1.0);
    emit iResolutionChanged();

    QQuickShaderEffect::geometryChange(newGeometry, oldGeometry);
}

void AxisTicker::setupShaders()
{
    if (m_isHorizontal) {
        setFragmentShader(QUrl(QStringLiteral("qrc:/shaders/tickershaderhorizontal.frag.qsb")));
        setVertexShader(QUrl(QStringLiteral("qrc:/shaders/tickershaderhorizontal.vert.qsb")));
    } else {
        setFragmentShader(QUrl(QStringLiteral("qrc:/shaders/tickershader.frag.qsb")));
        setVertexShader(QUrl(QStringLiteral("qrc:/shaders/tickershader.vert.qsb")));
    }
}

QVector3D AxisTicker::iResolution() const
{
    return m_iResolution;
}

qreal AxisTicker::smoothing() const
{
    return m_smoothing;
}

void AxisTicker::setSmoothing(qreal newSmoothing)
{
    if (QtPrivate::fuzzyCompare(m_smoothing, newSmoothing)) {
        qCDebug(lcAxis2D) << "AxisTicker::setSmoothing. value is already set to:" << newSmoothing;
        return;
    }
    m_smoothing = newSmoothing;
    emit smoothingChanged();
}

int AxisTicker::origo() const
{
    return m_origo;
}

void AxisTicker::setOrigo(int newOrigo)
{
    if (m_origo == newOrigo) {
        qCDebug(lcAxis2D) << "AxisTicker::setOrigo. value is already set to:" << newOrigo;
        return;
    }
    m_origo = newOrigo;
    emit origoChanged();
}

bool AxisTicker::subTicksVisible() const
{
    return m_subTicksVisible;
}

void AxisTicker::setSubTicksVisible(bool newSubTicksVisible)
{
    if (m_subTicksVisible == newSubTicksVisible) {
        qCDebug(lcAxis2D) << "AxisTicker::setSubTicksVisible. value is already set to:"
                           << newSubTicksVisible;
        return;
    }
    m_subTicksVisible = newSubTicksVisible;
    emit subTicksVisibleChanged();
}

qreal AxisTicker::spacing() const
{
    return m_spacing;
}

void AxisTicker::setSpacing(qreal newSpacing)
{
    if (QtPrivate::fuzzyCompare(m_spacing, newSpacing)) {
        qCDebug(lcAxis2D) << "AxisTicker::setSpacing. value is already set to:" << newSpacing;
        return;
    }
    m_spacing = newSpacing;
    emit spacingChanged();
}

qreal AxisTicker::displacement() const
{
    return m_displacement;
}

void AxisTicker::setDisplacement(qreal newDisplacement)
{
    if (QtPrivate::fuzzyCompare(m_displacement, newDisplacement)) {
        qCDebug(lcAxis2D) << "AxisTicker::setDisplacement. value is already set to:"
                           << newDisplacement;
        return;
    }
    m_displacement = newDisplacement;
    emit displacementChanged();
}

QColor AxisTicker::subTickColor() const
{
    return m_subTickColor;
}

void AxisTicker::setSubTickColor(QColor newSubTickColor)
{
    if (m_subTickColor == newSubTickColor) {
        qCDebug(lcAxis2D) << "AxisTicker::setSubTickColor. value is already set to:"
                           << newSubTickColor;
        return;
    }
    m_subTickColor = newSubTickColor;
    emit subTickColorChanged();
}

QColor AxisTicker::tickColor() const
{
    return m_tickColor;
}

void AxisTicker::setTickColor(QColor newTickColor)
{
    if (m_tickColor == newTickColor) {
        qCDebug(lcAxis2D) << "AxisTicker::setTickColor. value is already set to:" << newTickColor;
        return;
    }
    m_tickColor = newTickColor;
    emit tickColorChanged();
}

qreal AxisTicker::subTickLineWidth() const
{
    return m_subTickLineWidth;
}

void AxisTicker::setSubTickLineWidth(qreal newSubTickLineWidth)
{
    if (QtPrivate::fuzzyCompare(m_subTickLineWidth, newSubTickLineWidth)) {
        qCDebug(lcAxis2D) << "AxisTicker::setSubTickLineWidth. value is already set to:"
                           << newSubTickLineWidth;
        return;
    }
    m_subTickLineWidth = newSubTickLineWidth;
    emit subTickLineWidthChanged();
}

qreal AxisTicker::tickLineWidth() const
{
    return m_tickLineWidth;
}

void AxisTicker::setTickLineWidth(qreal newTickLineWidth)
{
    if (QtPrivate::fuzzyCompare(m_tickLineWidth, newTickLineWidth)) {
        qCDebug(lcAxis2D) << "AxisTicker::setTickLineWidth. value is already set to:"
                           << newTickLineWidth;
        return;
    }
    m_tickLineWidth = newTickLineWidth;
    emit tickLineWidthChanged();
}

qreal AxisTicker::subTickScale() const
{
    return m_subTickScale;
}

void AxisTicker::setSubTickScale(qreal newSubTickScale)
{
    if (QtPrivate::fuzzyCompare(m_subTickScale, newSubTickScale)) {
        qCDebug(lcAxis2D) << "AxisTicker::setSubTickScale. value is already set to:"
                           << newSubTickScale;
        return;
    }
    m_subTickScale = newSubTickScale;
    emit subTickScaleChanged();
}

qreal AxisTicker::subTickLength() const
{
    return m_subTickLength;
}

void AxisTicker::setSubTickLength(qreal newSubTickLength)
{
    if (QtPrivate::fuzzyCompare(m_subTickLength, newSubTickLength)) {
        qCDebug(lcAxis2D) << "AxisTicker::setSubTickLength. value is already set to:"
                           << newSubTickLength;
        return;
    }
    m_subTickLength = newSubTickLength;
    emit subTickLengthChanged();
}

bool AxisTicker::isHorizontal() const
{
    return m_isHorizontal;
}

void AxisTicker::setIsHorizontal(bool newIsHorizontal)
{
    if (m_isHorizontal == newIsHorizontal) {
        qCDebug(lcAxis2D) << "AxisTicker::setIsHorizontal. value is already set to:"
                           << newIsHorizontal;
        return;
    }
    m_isHorizontal = newIsHorizontal;
    setupShaders();
    emit isHorizontalChanged();
}

bool AxisTicker::isFlipped() const
{
    return m_flipped;
}

void AxisTicker::setFlipped(bool newFlipped)
{
    if (m_flipped == newFlipped) {
        qCDebug(lcAxis2D) << "AxisTicker::setFlipped. value is already set to:" << newFlipped;
        return;
    }
    m_flipped = newFlipped;
    emit flippedChanged();
}

bool AxisTicker::isLogarithmic() const
{
    return m_isLogarithmic;
}

void AxisTicker::setLogarithmic(bool newLogarithmic)
{
    if (m_isLogarithmic == newLogarithmic) {
        qCDebug(lcAxis2D) << "AxisTicker::setLogarithmic. value is already set to:"
                           << newLogarithmic;
        return;
    }

    m_isLogarithmic = newLogarithmic;
    emit logarithmicChanged();
}

qreal AxisTicker::base() const
{
    return m_base;
}

void AxisTicker::setBase(qreal newBase)
{
    if (qFuzzyCompare(m_base, newBase)) {
        qCDebug(lcAxis2D) << "AxisTicker::setBase. value is already set to:" << newBase;
        return;
    }

    m_base = newBase;
    emit baseChanged();
}

QT_END_NAMESPACE

#include "moc_axisticker_p.cpp"
