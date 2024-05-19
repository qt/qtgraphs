// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef PIERENDERER_H
#define PIERENDERER_H

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

QT_BEGIN_NAMESPACE

class QGraphsView;
class QPieSeries;
class QQuickShape;

class PieRenderer : public QQuickItem
{
    Q_OBJECT
public:
    PieRenderer(QQuickItem *parent = nullptr);

    void handlePolish(QPieSeries *series);
    void updateSeries(QPieSeries *series);

    void setSize(QSizeF size);

private:
    QGraphsView *m_graph;
    QQuickShape *m_shape;

    qsizetype m_colorIndex = -1;
};

QT_END_NAMESPACE

#endif // QPIERENDERER_H
