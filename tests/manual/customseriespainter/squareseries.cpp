// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "squareseries.h"

#include <QtCanvasPainter/QCanvasPainter>

void SquarePainter::synchronizeData(QCustomSeries *series, QCanvasPainterItem *item)
{
    Q_UNUSED(item);

    m_squares.clear();
    auto *squareSeries = qobject_cast<SquareSeries *>(series);
    if (!squareSeries)
        return;

    const auto squares = squareSeries->squares();
    m_squares.reserve(squares.size());
    for (const auto &square : squares) {
        qreal x = series->mapX(square.x);
        qreal y = series->mapY(square.y);
        // square.size is in axis units, so it needs to go through the same
        // axis-to-render-space mapping as x/y, not be used as a pixel size
        // directly.
        qreal width = std::abs(series->mapX(square.size) - series->mapX(0));
        qreal height = std::abs(series->mapY(square.size) - series->mapY(0));
        m_squares.append({QRectF(x - width / 2, y - height / 2, width, height), square.color});
    }
}

void SquarePainter::canvasPaint(QCanvasPainter *p)
{
    for (const auto &square : std::as_const(m_squares)) {
        p->setFillStyle(square.color);
        p->fillRect(square.rect);
    }
}

SquareSeries::SquareSeries(QObject *parent)
    : QCustomSeries(parent)
{
    setCustomSeriesPainter(new SquarePainter);
    // A placeholder data item, so the renderer's per-series bookkeeping
    // (which requires at least one data item to exist) is set up; the actual
    // square data lives in m_squares and is exposed through squares().
    append();
}

void SquareSeries::addSquare(qreal x, qreal y, qreal size, const QColor &color)
{
    m_squares.append({x, y, size, color});
    update();
}

void SquareSeries::clearSquares()
{
    m_squares.clear();
    update();
}

QList<SquareSeries::Square> SquareSeries::squares() const
{
    return m_squares;
}
