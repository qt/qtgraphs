// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
// Qt-Security score:significant reason:default

#include "qcustomseriescanvasrenderer.h"

QT_BEGIN_NAMESPACE

/*!
    \class QCustomSeriesCanvasRenderer
    \inmodule QtGraphs
    \brief The QCustomSeriesCanvasRenderer class allows implementing
    customized rendering for a QCustomSeries when the high-performance
    canvas painter backend is in use.
    \since 6.12

    Implement canvasPaint() to draw a \l QCustomSeries. Set the renderer
    on the series with \l QCustomSeries::setCustomSeriesPainter(), which
    the series then exposes through \l QCustomSeries::customSeriesPainter().

    canvasPaint() and canvasPrePaint() are called from the \l {Scene Graph
    and Rendering}{Qt Quick scene graph render thread} and, unlike
    initializeResources() and synchronizeData(), do not receive the
    \l QCustomSeries they belong to. Copy whatever canvasPaint() needs into
    the renderer instance in synchronizeData(), which runs while the GUI
    thread is blocked and is the only place it is safe to read GUI-thread
    data.

    \sa QCustomSeries, QCustomSeries::setCustomSeriesPainter()
*/

QCustomSeriesCanvasRenderer::~QCustomSeriesCanvasRenderer() = default;

/*!
    Reimplement this method to initialize resources for \a series using
    \a p. This is called once \b before the first synchronizeData() call
    for \a series.

    \sa QCanvasPainter::addImage, QCanvasPainter::createCanvas
*/
void QCustomSeriesCanvasRenderer::initializeResources(QCustomSeries *series, QCanvasPainter *p)
{
    Q_UNUSED(series);
    Q_UNUSED(p);
}

/*!
    Reimplement this method to copy data needed for painting \a series out
    of \a item and into this renderer instance. This is called before
    canvasPaint() each time \a series needs to be repainted.

    This method is the only place where it is safe to read GUI-owned state
    of \a series or \a item directly, since it runs while the GUI thread is
    blocked at the Qt Quick sync point. Do not access that state from
    canvasPaint() or canvasPrePaint().
*/
void QCustomSeriesCanvasRenderer::synchronizeData(QCustomSeries *series, QCanvasPainterItem *item)
{
    Q_UNUSED(series);
    Q_UNUSED(item);
}

/*!
    Reimplement this method to prepare rendering using \a p, based on state
    previously copied into this instance by synchronizeData(). This is
    called immediately before canvasPaint() for this same instance, from the
    render thread; do not access series or item state here.

    \sa canvasPaint()
*/
void QCustomSeriesCanvasRenderer::canvasPrePaint(QCanvasPainter *p)
{
    Q_UNUSED(p);
}

QT_END_NAMESPACE
