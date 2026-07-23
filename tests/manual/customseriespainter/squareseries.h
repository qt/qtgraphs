// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef SQUARESERIES_H
#define SQUARESERIES_H

#include <QtGraphs/qcustomseries.h>
#include <QtGraphs/qcustomseriescanvasrenderer.h>
#include <QtCore/qlist.h>
#include <QtCore/qrect.h>
#include <QtGui/qcolor.h>

// Handed to SquareSeries via setCustomSeriesPainter() (see below), which takes
// ownership until the CanvasPainter-backend renderer adopts it; from then on
// it's owned and eventually destroyed by the renderer, never by SquareSeries.
// Copies square geometry/color out of SquareSeries in synchronizeData(), which
// runs on the GUI thread while it is blocked at the Qt Quick sync point, so
// that canvasPaint() (render thread) never touches the live series.
class SquarePainter : public QCustomSeriesCanvasRenderer
{
public:
    void synchronizeData(QCustomSeries *series, QCanvasPainterItem *item) override;
    void canvasPaint(QCanvasPainter *p) override;

private:
    struct RenderSquare
    {
        QRectF rect;
        QColor color;
    };

    QList<RenderSquare> m_squares;
};

class SquareSeries : public QCustomSeries
{
    Q_OBJECT
    QML_ELEMENT

public:
    struct Square
    {
        qreal x = 0;
        qreal y = 0;
        qreal size = 0;
        QColor color;
    };

    explicit SquareSeries(QObject *parent = nullptr);

    Q_INVOKABLE void addSquare(qreal x, qreal y, qreal size, const QColor &color);
    Q_INVOKABLE void clearSquares();

    QList<Square> squares() const;

private:
    QList<Square> m_squares;
};

#endif // SQUARESERIES_H
