// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QCPAINTERITEM_H
#define QCPAINTERITEM_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the QtGraphs API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

#include <QtCanvasPainter/QCanvasPainter>
#include <QtCanvasPainter/QCanvasPainterItem>
#include <QtCanvasPainter/QCanvasPainterItemRenderer>

QT_BEGIN_NAMESPACE

class AreaRenderer;
class BarsRenderer;
class PieRenderer;
class PointRenderer;
class CustomRenderer;

class QCPainterItemRenderer : public QCanvasPainterItemRenderer
{
public:
    QCPainterItemRenderer(AreaRenderer *&areaRenderer,
                          BarsRenderer *&barsRenderer,
                          PieRenderer *&pieRenderer,
                          PointRenderer *&pointRenderer,
                          CustomRenderer *&customRenderer);

    void paint(QCanvasPainter *p) override;

    AreaRenderer *&areaRenderer;
    BarsRenderer *&barsRenderer;
    PieRenderer *&pieRenderer;
    PointRenderer *&pointRenderer;
    CustomRenderer *&customRenderer;
};

class QCPainterItem : public QCanvasPainterItem
{
    Q_OBJECT

public:
    QCPainterItem(AreaRenderer *&areaRenderer,
                  BarsRenderer *&barsRenderer,
                  PieRenderer *&pieRenderer,
                  PointRenderer *&pointRenderer,
                  CustomRenderer *&customRenderer,
                  QQuickItem *parent = nullptr);

    QCPainterItemRenderer *createItemRenderer() const override;

    AreaRenderer *&areaRenderer;
    BarsRenderer *&barsRenderer;
    PieRenderer *&pieRenderer;
    PointRenderer *&pointRenderer;
    CustomRenderer *&customRenderer;
};

QT_END_NAMESPACE

#endif // QCPAINTERITEM_H
