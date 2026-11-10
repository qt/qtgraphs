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

#ifndef ABSTRACTAXISLINE_P_H
#define ABSTRACTAXISLINE_P_H

#include <QtGraphs/qgraphsglobal.h>

QT_BEGIN_NAMESPACE

class QColor;
class QQuickItem;

class AbstractAxisLine
{
public:
    virtual ~AbstractAxisLine() = default;
    virtual qreal smoothing() const = 0;
    virtual void setSmoothing(qreal newSmoothing) = 0;
    virtual qreal lineWidth() const = 0;
    virtual void setLineWidth(qreal newLineWidth) = 0;

    virtual void setColor(QColor newColor) = 0;
    virtual void setIsHorizontal(bool newIsHorizontal) = 0;

    virtual QQuickItem *item() = 0;
    virtual void completeItem() = 0;
};

QT_END_NAMESPACE

#endif
