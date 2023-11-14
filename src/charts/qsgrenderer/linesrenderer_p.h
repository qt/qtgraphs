// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef LINESRENDERER_H
#define LINESRENDERER_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the QtGraphs API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

#include <QQuickItem>
#include <QtQuickShapes/private/qquickshape_p.h>
#include <QtQuick/private/qsgdefaultinternalrectanglenode_p.h>

class QGraphsView;
class QLineSeries;

class LinesRenderer : public QQuickItem
{
    Q_OBJECT
    QML_ELEMENT
public:
    LinesRenderer(QQuickItem *parent = nullptr);
    virtual ~LinesRenderer();

    void handlePolish(QLineSeries *series);
    void updateLineSeries(QLineSeries *series);
    void handleMouseMove(QMouseEvent *event);
    void handleMousePress(QMouseEvent *event);
    void handleMouseRelease(QMouseEvent *event);

Q_SIGNALS:

private:
    struct LinePath {
        QLineSeries *series;
        QQuickShapePath *shapePath = nullptr;
        QList<QQuickPathLine *> paths;
        QList<QQuickItem *> markers;
        QList<QSGDefaultInternalRectangleNode *> selections;
        int colorIndex = -1;
    };

    QGraphsView *m_graph = nullptr;
    QQuickShape m_shape;
    QMap<QLineSeries *, LinePath *> m_linePaths;
    int m_currentColorIndex = 0;

    // Line point drag variables
    bool m_pointPressed = false;
    bool m_pointDragging = false;
    QPoint m_pressStart;
    LinePath *m_pressedLine = nullptr;
    int m_pressedPointIndex = 0;
};

#endif // LINESRENDERER_H
