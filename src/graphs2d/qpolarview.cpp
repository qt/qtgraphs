// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
// Qt-Security score:significant reason:default

#include <QtGraphs/private/qgraphsglobal_p.h>
#if QT_CONFIG(graphs_2d_area)
#include <QtGraphs/private/arearenderer_p.h>
#endif
#if QT_CONFIG(graphs_2d_area) || QT_CONFIG(graphs_2d_line) || QT_CONFIG(graphs_2d_scatter) || QT_CONFIG(graphs_2d_spline)
#include <QtGraphs/private/pointrenderer_p.h>
#endif
#if QT_CONFIG(graphs_2d_high_performance_backend)
#include "qcpainteritem_p.h"
#endif
#include <QtGraphs/qbarcategoryaxis.h>
#include <QtGraphs/private/axisrenderer_p.h>
#include <QtGraphs/private/polaraxisrenderer_p.h>
#include "qpolarview_p.h"

QT_BEGIN_NAMESPACE

/*!
    \qmltype PolarView
    \nativetype QPolarView
    \inqmlmodule QtGraphs
    \ingroup graphs_qml_2D
    \inherits GraphsView
    \since 6.13
    \brief Draws series on a polar coordinate system.

    PolarView plots series on a polar coordinate system instead of the Cartesian
    one that \l GraphsView uses. It reinterprets \l{GraphsView::axisX}{axisX} as
    the angular axis and \l{GraphsView::axisY}{axisY} as the radial axis.

    The angular axis maps its full range to one complete revolution, starting at
    the top of the graph and advancing clockwise. The radial axis maps its
    minimum to the center of the graph and its maximum to the outer edge. Values
    below the radial minimum collapse onto the center.

    PolarView supports \l LineSeries, \l ScatterSeries, \l SplineSeries, and
    \l AreaSeries. Adding any other series type prints a warning and leaves the
    graph unchanged. A polar view also rejects \l BarCategoryAxis and
    \l ColorAxis.

    The following example code illustrates how to use the PolarView type:
    \qml
    PolarView {
        anchors.fill: parent
        labelPadding: 12
        axisX: ValueAxis { min: 0; max: 360 }   // angular
        axisY: ValueAxis { min: 0; max: 10 }    // radial

        LineSeries {
            color: "#46f477"
            width: 2

            XYPoint { x: 0; y: 10 }
            XYPoint { x: 120; y: 3 }
            XYPoint { x: 240; y: 4 }
            XYPoint { x: 360; y: 10 }
        }
    }
    \endqml

    The image below illustrates the result:

    \image graphs2d-polarview.webp {A polar view with a single line series.}

    \sa GraphsView, LineSeries, ScatterSeries, SplineSeries, AreaSeries,
        ValueAxis
*/
QPolarView::QPolarView(QQuickItem *parent)
    : QGraphsView(parent)
{
}

QPolarView::~QPolarView() = default;

bool QPolarView::isSeriesSupported(QObject *object) const
{
    if (auto series = qobject_cast<QAbstractSeries *>(object)) {
        switch (series->type()) {
        case QAbstractSeries::SeriesType::Line:
        case QAbstractSeries::SeriesType::Scatter:
        case QAbstractSeries::SeriesType::Spline:
        case QAbstractSeries::SeriesType::Area:
            break;
        case QAbstractSeries::SeriesType::Bar:
        case QAbstractSeries::SeriesType::Pie:
        case QAbstractSeries::SeriesType::Custom:
            qWarning("Can not add series. Series type is not supported by a polar view.");
            return false;
        }
    }
    return QGraphsView::isSeriesSupported(object);
}

bool QPolarView::addAxis(QAbstractAxis *axis)
{
    if (axis->type() == QAbstractAxis::AxisType::BarCategory) {
        qWarning("QBarCategoryAxis is not a supported axis type for polar views.");
        return false;
    }
    return QGraphsView::addAxis(axis);
}

void QPolarView::createAxisRenderer()
{
    if (!m_axisRenderer) {
        qCDebug(lcGraphs2D) << "creating polar axis renderer.";
        m_axisRenderer = new PolarAxisRenderer(this);
        m_axisRenderer->setZ(-1);
        updateComponentSizes();
    }
}

/*!
    \qmlproperty point PolarView::polarCenter
    \readonly
    The center of the polar plot, in the coordinate space of the plot area.

    Use this property together with \l polarRadius to position gradients or
    custom items on top of the graph. The default value is \c{Qt.point(0, 0)}.
*/
QPointF QPolarView::polarCenter() const
{
    return m_polarCenter;
}

/*!
    \qmlproperty real PolarView::polarRadius
    \readonly
    The radius of the polar plot, in pixels.

    The radius is half the side of the largest square that fits the plot area,
    after subtracting the space that the radial axis labels need. The default
    value is 0.
*/
qreal QPolarView::polarRadius() const
{
    return m_polarRadius;
}

/*!
    \qmlproperty real PolarView::labelPadding
    The gap, in pixels, between the axis grid (the outer ring for angular
    labels, or the ring belonging to a given tick for radial labels) and
    the axis label items placed next to it. The default value is 8.
*/
qreal QPolarView::labelPadding() const
{
    return m_labelPadding;
}

void QPolarView::setLabelPadding(qreal padding)
{
    if (QtPrivate::fuzzyCompare(m_labelPadding, padding))
        return;

    m_labelPadding = qMax(qreal(0), padding);
    polishAndUpdate();
    emit labelPaddingChanged();
}

void QPolarView::updateComponentSizes()
{
    qreal radialLabelBand = 0;
    if (m_axisRenderer) {
        m_axisRenderer->updateAxisMeasurements();
        if (auto polarAxisRenderer = qobject_cast<PolarAxisRenderer*>(m_axisRenderer))
            radialLabelBand = polarAxisRenderer->radialLabelBand();
    }

    qreal x = m_marginLeft;
    qreal y = m_marginTop;
    qreal w = width() - x - m_marginRight;
    qreal h = height() - y - m_marginBottom;

    qreal side = std::max(std::min(w, h) - radialLabelBand, 0.0);
    qreal squareX = x + (w - side) / 2.0;
    qreal squareY = y + (h - side) / 2.0;

    QRectF plotArea = QRectF(squareX, squareY, side, side);
    if (plotArea != m_plotArea) {
        m_plotArea = plotArea;
        emit plotAreaChanged();
    }

    qreal polarRadius = side / 2.0;
    if (polarRadius != m_polarRadius) {
        m_polarRadius = polarRadius;
        emit polarRadiusChanged();
    }

    QPointF polarCenter(m_polarRadius, m_polarRadius);
    if (polarCenter != m_polarCenter) {
        m_polarCenter = polarCenter;
        emit polarCenterChanged();
    }

    if (m_axisRenderer) {
        m_axisRenderer->setX(m_plotArea.x());
        m_axisRenderer->setY(m_plotArea.y());
        m_axisRenderer->setSize(m_plotArea.size());
    }

#if QT_CONFIG(graphs_2d_area) || QT_CONFIG(graphs_2d_line) || QT_CONFIG(graphs_2d_scatter) || QT_CONFIG(graphs_2d_spline)
    if (m_pointRenderer) {
        m_pointRenderer->setX(m_plotArea.x());
        m_pointRenderer->setY(m_plotArea.y());
        m_pointRenderer->setSize(m_plotArea.size());
    }
#endif
#if QT_CONFIG(graphs_2d_area)
    if (m_areaRenderer) {
        m_areaRenderer->setX(m_plotArea.x());
        m_areaRenderer->setY(m_plotArea.y());
        m_areaRenderer->setSize(m_plotArea.size());
    }
#endif
#if QT_CONFIG(graphs_2d_high_performance_backend)
    if (m_painterItem) {
        m_painterItem->setX(m_plotArea.x());
        m_painterItem->setY(m_plotArea.y());
        m_painterItem->setSize(m_plotArea.size());
    }
#endif
}

QT_END_NAMESPACE

#include "moc_qpolarview_p.cpp"
