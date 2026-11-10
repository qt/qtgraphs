// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
// Qt-Security score:significant reason:default

//  W A R N I N G
//  -------------
//
// This file is not part of the QtGraphs API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

#ifndef ABSTRACTAXISGRID_P_H
#define ABSTRACTAXISGRID_P_H

#include <QtGraphs/qgraphsglobal.h>

QT_BEGIN_NAMESPACE

class QColor;
class QPointF;
class QQuickItem;
class QVector4D;

class AbstractAxisGrid
{
public:
    virtual ~AbstractAxisGrid() = default;
    virtual qreal smoothing() const = 0;
    virtual void setSmoothing(qreal newSmoothing) = 0;
    virtual QVector4D gridVisibility() const = 0;
    virtual void setGridVisibility(const QVector4D &newGridVisibility) = 0;

    virtual qreal gridWidth() const = 0;
    virtual void setGridWidth(qreal newGridWidth) = 0;

    virtual qreal gridHeight() const = 0;
    virtual void setGridHeight(qreal newGridHeight) = 0;

    virtual QPointF gridMovement() const = 0;
    virtual void setGridMovement(QPointF newGridMovement) = 0;

    virtual QColor subGridColor() const = 0;
    virtual void setSubGridColor(QColor newSubGridColor) = 0;

    virtual QColor gridColor() const = 0;
    virtual void setGridColor(QColor newGridColor) = 0;

    virtual QColor plotAreaBackgroundColor() const = 0;
    virtual void setPlotAreaBackgroundColor(QColor color) = 0;

    virtual qreal subGridLineWidth() const = 0;
    virtual void setSubGridLineWidth(qreal newSubGridLineWidth) = 0;

    virtual qreal gridLineWidth() const = 0;
    virtual void setGridLineWidth(qreal newGridLineWidth) = 0;

    virtual qreal verticalSubGridScale() const = 0;
    virtual void setVerticalSubGridScale(qreal newVerticalSubGridScale) = 0;

    virtual qreal horizontalSubGridScale() const = 0;
    virtual void setHorizontalSubGridScale(qreal newHorizontalSubGridScale) = 0;

    virtual bool isVerticalLogarithmic() const = 0;
    virtual void setVerticalLogarithmic(bool newLogarithmic) = 0;

    virtual bool isHorizontalLogarithmic() const = 0;
    virtual void setHorizontalLogarithmic(bool newLogarithmic) = 0;

    virtual qreal verticalBase() const = 0;
    virtual void setVerticalBase(qreal newBase) = 0;

    virtual qreal horizontalBase() const = 0;
    virtual void setHorizontalBase(qreal newBase) = 0;

    virtual int origo() const = 0;
    virtual void setOrigo(int newOrigo) = 0;

    virtual QQuickItem *item() = 0;
    virtual void completeItem() = 0;
};

QT_END_NAMESPACE

#endif
