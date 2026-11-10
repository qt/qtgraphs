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

#ifndef ABSTRACTAXISTICKER_P_H
#define ABSTRACTAXISTICKER_P_H

#include <QtGraphs/qgraphsglobal.h>

QT_BEGIN_NAMESPACE

class QColor;
class QQuickItem;

class AbstractAxisTicker
{
public:
    virtual ~AbstractAxisTicker() = default;

    virtual qreal smoothing() const = 0;
    virtual void setSmoothing(qreal newSmoothing) = 0;

    virtual int origo() const = 0;
    virtual void setOrigo(int newOrigo) = 0;

    virtual bool subTicksVisible() const = 0;
    virtual void setSubTicksVisible(bool newSubTicksVisible) = 0;

    virtual qreal spacing() const = 0;
    virtual void setSpacing(qreal newSpacing) = 0;

    virtual qreal displacement() const = 0;
    virtual void setDisplacement(qreal newDisplacement) = 0;

    virtual void setSubTickColor(QColor newSubTickColor) = 0;
    virtual void setTickColor(QColor newTickColor) = 0;
    virtual void setIsHorizontal(bool newIsHorizontal) = 0;

    virtual bool isFlipped() const = 0;
    virtual void setFlipped(bool newFlipped) = 0;

    virtual bool isLogarithmic() const = 0;
    virtual void setLogarithmic(bool newLogarithmic) = 0;

    virtual qreal base() const = 0;
    virtual void setBase(qreal newBase) = 0;

    virtual qreal subTickLineWidth() const = 0;
    virtual void setSubTickLineWidth(qreal newSubTickLineWidth) = 0;

    virtual qreal tickLineWidth() const = 0;
    virtual void setTickLineWidth(qreal newTickLineWidth) = 0;

    virtual qreal subTickScale() const = 0;
    virtual void setSubTickScale(qreal newSubTickScale) = 0;

    virtual qreal subTickLength() const = 0;
    virtual void setSubTickLength(qreal newSubTickLength) = 0;

    virtual QQuickItem *item() = 0;
    virtual void completeItem() = 0;
};

QT_END_NAMESPACE

#endif
