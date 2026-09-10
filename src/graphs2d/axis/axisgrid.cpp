// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
// Qt-Security score:significant reason:default


#include "axisgrid_p.h"

#include <QtGraphs/QAbstractAxis>

QT_BEGIN_NAMESPACE

AxisGrid::AxisGrid(QQuickItem *parent) :
      QQuickShaderEffect(parent)
{
}

AxisGrid::~AxisGrid() {}

void AxisGrid::componentComplete()
{
    QQuickShaderEffect::componentComplete();
    setupShaders();
}

void AxisGrid::geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    m_iResolution = QVector3D(newGeometry.width(), newGeometry.height(), 1.0);
    emit iResolutionChanged();

    QQuickShaderEffect::geometryChange(newGeometry, oldGeometry);
}

void AxisGrid::setupShaders()
{
    setFragmentShader(QUrl(QStringLiteral("qrc:/shaders/gridshader.frag.qsb")));
    setVertexShader(QUrl(QStringLiteral("qrc:/shaders/gridshader.vert.qsb")));
}

QVector3D AxisGrid::iResolution() const
{
    return m_iResolution;
}

qreal AxisGrid::smoothing() const
{
    return m_smoothing;
}

void AxisGrid::setSmoothing(qreal newSmoothing)
{
    if (QtPrivate::fuzzyCompare(m_smoothing, newSmoothing)) {
        qCDebug(lcAxis2D) << "AxisGrid::setSmoothing. value is already set to:" << newSmoothing;
        return;
    }
    m_smoothing = newSmoothing;
    emit smoothingChanged();
}

int AxisGrid::origo() const
{
    return m_origo;
}

void AxisGrid::setOrigo(int newOrigo)
{
    if (m_origo == newOrigo) {
        qCDebug(lcAxis2D) << "AxisGrid::setOrigo. value is already set to:" << newOrigo;
        return;
    }
    m_origo = newOrigo;
    emit origoChanged();
}

QVector4D AxisGrid::gridVisibility() const
{
    return m_gridVisibility;
}

void AxisGrid::setGridVisibility(const QVector4D &newGridVisibility)
{
    if (m_gridVisibility == newGridVisibility) {
        qCDebug(lcAxis2D) << "AxisGrid::setGridVisibility. value is already set to:"
                           << newGridVisibility;
        return;
    }
    m_gridVisibility = newGridVisibility;
    emit gridVisibilityChanged();
}

qreal AxisGrid::gridWidth() const
{
    return m_gridWidth;
}

void AxisGrid::setGridWidth(qreal newGridWidth)
{
    if (QtPrivate::fuzzyCompare(m_gridWidth, newGridWidth)) {
        qCDebug(lcAxis2D) << "AxisGrid::setGridWidth. value is already set to:" << newGridWidth;
        return;
    }
    m_gridWidth = newGridWidth;
    emit gridWidthChanged();
}

qreal AxisGrid::gridHeight() const
{
    return m_gridHeight;
}

void AxisGrid::setGridHeight(qreal newGridHeight)
{
    if (QtPrivate::fuzzyCompare(m_gridHeight, newGridHeight)) {
        qCDebug(lcAxis2D) << "AxisGrid::setGridHeight. value is already set to:" << newGridHeight;
        return;
    }
    m_gridHeight = newGridHeight;
    emit gridHeightChanged();
}

QPointF AxisGrid::gridMovement() const
{
    return m_gridMovement;
}

void AxisGrid::setGridMovement(QPointF newGridMovement)
{
    if (m_gridMovement == newGridMovement) {
        qCDebug(lcAxis2D) << "AxisGrid::setGridMovement. value is already set to:"
                           << newGridMovement;
        return;
    }
    m_gridMovement = newGridMovement;
    emit gridMovementChanged();
}

QColor AxisGrid::subGridColor() const
{
    return m_subGridColor;
}

void AxisGrid::setSubGridColor(QColor newSubGridColor)
{
    if (m_subGridColor == newSubGridColor) {
        qCDebug(lcAxis2D) << "AxisGrid::setSubGridColor. value is already set to:"
                           << newSubGridColor;
        return;
    }
    m_subGridColor = newSubGridColor;
    emit subGridColorChanged();
}

QColor AxisGrid::gridColor() const
{
    return m_gridColor;
}

void AxisGrid::setGridColor(QColor newGridColor)
{
    if (m_gridColor == newGridColor) {
        qCDebug(lcAxis2D) << "AxisGrid::setGridColor. value is already set to:" << newGridColor;
        return;
    }
    m_gridColor = newGridColor;
    emit gridColorChanged();
}

QColor AxisGrid::plotAreaBackgroundColor() const
{
    return m_plotAreaBackgroundColor;
}

void AxisGrid::setPlotAreaBackgroundColor(QColor color)
{
    if (m_plotAreaBackgroundColor == color) {
        qCDebug(lcAxis2D) << "AxisGrid::setPlotAreaBackgroundColor. value is already set to:"
                           << color;
        return;
    }
    m_plotAreaBackgroundColor = color;
    emit plotAreaBackgroundColorChanged();
}

qreal AxisGrid::subGridLineWidth() const
{
    return m_subGridLineWidth;
}

void AxisGrid::setSubGridLineWidth(qreal newSubGridLineWidth)
{
    if (QtPrivate::fuzzyCompare(m_subGridLineWidth, newSubGridLineWidth)) {
        qCDebug(lcAxis2D) << "AxisGrid::setSubGridLineWidth. value is already set to:"
                           << newSubGridLineWidth;
        return;
    }
    m_subGridLineWidth = newSubGridLineWidth;
    emit subGridLineWidthChanged();
}

qreal AxisGrid::gridLineWidth() const
{
    return m_gridLineWidth;
}

void AxisGrid::setGridLineWidth(qreal newGridLineWidth)
{
    if (QtPrivate::fuzzyCompare(m_gridLineWidth, newGridLineWidth)) {
        qCDebug(lcAxis2D) << "AxisGrid::setGridLineWidth. value is already set to:"
                           << newGridLineWidth;
        return;
    }
    m_gridLineWidth = newGridLineWidth;
    emit gridLineWidthChanged();
}

qreal AxisGrid::verticalSubGridScale() const
{
    return m_verticalSubGridScale;
}

void AxisGrid::setVerticalSubGridScale(qreal newVerticalSubGridScale)
{
    if (QtPrivate::fuzzyCompare(m_verticalSubGridScale, newVerticalSubGridScale)) {
        qCDebug(lcAxis2D) << "AxisGrid::setVerticalSubGridScale. value is already set to:"
                           << newVerticalSubGridScale;
        return;
    }
    m_verticalSubGridScale = newVerticalSubGridScale;
    emit verticalSubGridScaleChanged();
}

qreal AxisGrid::horizontalSubGridScale() const
{
    return m_horizontalSubGridScale;
}

void AxisGrid::setHorizontalSubGridScale(qreal newHorizontalSubGridScale)
{
    if (QtPrivate::fuzzyCompare(m_horizontalSubGridScale, newHorizontalSubGridScale)) {
        qCDebug(lcAxis2D) << "AxisGrid::setHorizontalSubGridScale. value is already set to:"
                           << newHorizontalSubGridScale;
        return;
    }
    m_horizontalSubGridScale = newHorizontalSubGridScale;
    emit horizontalSubGridScaleChanged();
}

bool AxisGrid::isVerticalLogarithmic() const
{
    return m_isVerticalLogarithmic;
}

void AxisGrid::setVerticalLogarithmic(bool newLogarithmic)
{
    if (m_isVerticalLogarithmic == newLogarithmic) {
        qCDebug(lcAxis2D) << "AxisGrid::setVerticalLogarithmic. value is already set to:"
                           << newLogarithmic;
        return;
    }
    m_isVerticalLogarithmic = newLogarithmic;
    emit verticalLogarithmicChanged(newLogarithmic);
}

bool AxisGrid::isHorizontalLogarithmic() const
{
    return m_isHorizontalLogarithmic;
}

void AxisGrid::setHorizontalLogarithmic(bool newLogarithmic)
{
    if (m_isHorizontalLogarithmic == newLogarithmic) {
        qCDebug(lcAxis2D) << "AxisGrid::setHorizontalLogarithmic. value is already set to:"
                           << newLogarithmic;
        return;
    }
    m_isHorizontalLogarithmic = newLogarithmic;
    emit horizontalLogarithmicChanged(newLogarithmic);
}

qreal AxisGrid::verticalBase() const
{
    return m_verticalBase;
}

void AxisGrid::setVerticalBase(qreal newBase)
{
    if (qFuzzyCompare(m_verticalBase, newBase)) {
        qCDebug(lcAxis2D) << "AxisGrid::setVerticalBase. value is already set to:" << newBase;
        return;
    }
    m_verticalBase = newBase;
    emit verticalBaseChanged(newBase);
}

qreal AxisGrid::horizontalBase() const
{
    return m_horizontalBase;
}

void AxisGrid::setHorizontalBase(qreal newBase)
{
    if (qFuzzyCompare(m_horizontalBase, newBase)) {
        qCDebug(lcAxis2D) << "AxisGrid::setHorizontalBase. value is already set to:" << newBase;
        return;
    }
    m_horizontalBase = newBase;
    emit verticalBaseChanged(newBase);
}
QT_END_NAMESPACE

#include "moc_axisgrid_p.cpp"
