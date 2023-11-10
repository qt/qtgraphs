// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef BARSRENDERER_H
#define BARSRENDERER_H

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
#include <QtQuick/private/qsgdefaultinternalrectanglenode_p.h>

class QQuickGraphs2DView;
class QBarSeries;
class QBarSet;

class BarsRenderer : public QQuickItem
{
    Q_OBJECT
    QML_ELEMENT
public:
    BarsRenderer(QQuickItem *parent = nullptr);

    void handlePolish(QBarSeries *series);
    void updateBarSeries(QBarSeries *series);
    void handleMousePress(QMouseEvent *event);

Q_SIGNALS:

private:
    struct BarSelectionRect {
        QBarSet *barSet = nullptr;
        QList<QRectF> rects;
    };
    QQuickGraphs2DView *m_graph = nullptr;
    QList<QSGDefaultInternalRectangleNode *> m_rectNodes;
    // QSG nodes rect has no getter so we store these separately.
    QList<BarSelectionRect> m_rectNodesInputRects;

};

#endif // BARSRENDERER_H
