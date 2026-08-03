// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
// Qt-Security score:significant reason:default

#include <QtCore/qdatetime.h>
#include <QtCore/qmath.h>
#include <QtGui/qpainterpath.h>
#include <qfontmetrics.h>
#include <QtQuick/private/qquicktext_p.h>
#include <QtGraphs/qabstractaxis.h>
#include <QtGraphs/qvalueaxis.h>
#include <QtGraphs/qdatetimeaxis.h>
#include <QtGraphs/qlogvalueaxis.h>
#include <QtGraphs/qgraphstheme.h>
#include <private/polaraxisrenderer_p.h>
#include <private/qgraphsview_p.h>
#include <private/qpolarview_p.h>

QT_BEGIN_NAMESPACE

static constexpr int kMaxTicks = 100;
static constexpr int kRadialLabelMargin = 16;

PolarAxisRenderer::PolarAxisRenderer(QQuickItem *parent)
    : AxisRenderer(parent)
{
    m_shape.setParentItem(this);
    m_shape.setPreferredRendererType(QQuickShape::CurveRenderer);

    m_ringsPath = new QQuickShapePath(&m_shape);
    m_ringsPath->setAsynchronous(true);
    auto ringsData = m_shape.data();
    ringsData.append(&ringsData, m_ringsPath);

    m_spokesPath = new QQuickShapePath(&m_shape);
    m_spokesPath->setAsynchronous(true);
    auto spokesData = m_shape.data();
    spokesData.append(&spokesData, m_spokesPath);
}

PolarAxisRenderer::~PolarAxisRenderer() = default;

void PolarAxisRenderer::computeAxisProperties(AxisProperties &ax, QAbstractAxis *axis)
{
    ax.axis = axis;
    ax.isLogarithmic = false;
    if (!axis) {
        // m_axes1[0]/m_axes2[0] are reused across polishes rather than
        // reconstructed, so the range fields have to be put back to their
        // defaults explicitly. Consumers such as
        // AreaRenderer::calculateRenderCoordinates() and
        // AreaRenderer::polarRadiusAffine() read them without checking axis
        // validity, and would otherwise keep mapping values through the range
        // of an axis that is no longer set.
        ax.maxValue = 20;
        ax.minValue = 0;
        ax.valueRange = 0;
        ax.valueRangeZoomless = 0;
        ax.minValueZoomless = 0;
        ax.valueStep = 1.0;
        ax.minLabel = 0;
        ax.tickCount = 0.0;
        ax.logBase = 10;
        return;
    }

    // Unlike a Cartesian axis, the visible range is anchored at the axis minimum
    // rather than its midpoint: for the radial axis that minimum is the polar
    // center, which must stay fixed so zooming scales uniformly from the center
    // instead of drifting away from it.
    if (auto vaxis = qobject_cast<QValueAxis *>(axis)) {
        qreal step = vaxis->tickInterval();


        qreal diff = (vaxis->max() - vaxis->min()) / vaxis->zoom();
        ax.minValue = vaxis->min() + vaxis->pan();
        ax.maxValue = ax.minValue + diff;
        ax.valueRange = ax.maxValue - ax.minValue;
        ax.valueRangeZoomless = vaxis->max() - vaxis->min();
        ax.minValueZoomless = vaxis->min();

        if (step <= 0)
            step = getValueStepsFromRange(vaxis->max() - vaxis->min());
        ax.valueStep = step;

        // Used by the radial axis to shift its first visible tick away from
        // the center (see radialTickValues()); irrelevant for the angular
        // axis, whose spokes are always evenly divided around the full circle.
        double minLabel = vaxis->tickAnchor();
        while (minLabel < ax.minValue)
            minLabel += step;
        while (minLabel >= ax.minValue + step)
            minLabel -= step;
        ax.minLabel = minLabel;
    } else if (auto vaxis = qobject_cast<QDateTimeAxis *>(axis)) {
        double tickCount = std::clamp<double>(vaxis->tickCount(), 0.0, kMaxTicks);

        qreal minMs = vaxis->min().toMSecsSinceEpoch();
        qreal diff = (vaxis->max().toMSecsSinceEpoch() - minMs) / vaxis->zoom();
        ax.minValue = minMs + vaxis->pan();
        ax.maxValue = ax.minValue + diff;
        ax.valueRange = std::abs(ax.maxValue - ax.minValue);
        ax.valueRangeZoomless = vaxis->max().toMSecsSinceEpoch() - minMs;
        ax.minValueZoomless = minMs;

        double segment;
        if (tickCount <= 0) {
            segment = getValueStepsFromRange(ax.valueRange);
            tickCount = ax.valueRange / segment;
        } else {
            segment = ax.valueRange / tickCount;
        }
        ax.valueStep = segment;
        ax.tickCount = tickCount;
        ax.minLabel = ax.minValue;
    } else if (auto vaxis = qobject_cast<QLogValueAxis *>(axis)) {
        ax.isLogarithmic = true;
        ax.logBase = vaxis->base();

        qreal logBase = log(vaxis->base());
        qreal logMin = log(vaxis->min()) / logBase;
        qreal logMax = log(vaxis->max()) / logBase;

        qreal diff = (logMax - logMin) / vaxis->zoom();
        ax.minValue = logMin + vaxis->pan();
        ax.maxValue = ax.minValue + diff;
        ax.valueRange = ax.maxValue - ax.minValue;
        ax.valueRangeZoomless = logMax - logMin;
        ax.minValueZoomless = logMin;
        ax.valueStep = 1;
        ax.minLabel = ax.minValue;
    }
}

void PolarAxisRenderer::updateAxisMeasurements()
{
    if (m_axes1.empty())
        m_axes1.emplace_back();
    if (m_axes2.empty())
        m_axes2.emplace_back();

    computeAxisProperties(m_axes1[0], m_graph->axisX());
    computeAxisProperties(m_axes2[0], m_graph->axisY());

    // Reserve a band for radial axis labels, read back by
    // QPolarView::updateComponentSizes() via radialLabelBand().
    m_radialLabelBand = 0;
    auto &radial = m_axes2[0];
    if (theme() && radial.axis && radial.axis->isVisible() && radial.axis->labelsVisible()) {
        const QFontMetricsF fm(theme()->axisYLabelFont());
        qreal labelWidth = std::max(fm.horizontalAdvance(QString::number(radial.minValue, 'g', 4)),
                                    fm.horizontalAdvance(QString::number(radial.maxValue, 'g', 4)));
        m_radialLabelBand = labelWidth + kRadialLabelMargin;
    }
}

void PolarAxisRenderer::handlePolish()
{
    if (m_axes1.empty())
        m_axes1.emplace_back();
    if (m_axes2.empty())
        m_axes2.emplace_back();

    computeAxisProperties(m_axes1[0], m_graph->axisX());
    computeAxisProperties(m_axes2[0], m_graph->axisY());

    const QList<qreal> radialTicks = radialTickValues(m_axes2[0]);
    updatePolarGrid(radialTicks);
    updatePolarLabels(radialTicks);
}

// Rings/labels must sit at their actual value's radius rather than at an even
// i/ringCount fraction: valueStep is derived from the unzoomed axis range, so
// once zoomed it no longer divides valueRange evenly and the two diverge,
// most visibly for the outermost ring. tickAnchor (QValueAxis only, folded
// into minLabel by computeAxisProperties()) shifts where the first tick
// falls, e.g. min: 0, max: 10, tickInterval: 2, tickAnchor: 1 yields
// 1, 3, 5, 7, 9 instead of 2, 4, 6, 8, 10.
QList<qreal> PolarAxisRenderer::radialTickValues(const AxisProperties &radial) const
{
    QList<qreal> values;
    if (!(radial.valueRange > 0 && radial.valueStep > 0))
        return values;

    const qreal epsilon = radial.valueStep * 1e-9;
    for (qreal value = radial.minLabel; value <= radial.maxValue + epsilon;
         value += radial.valueStep) {
        // A tick that lands exactly on the center (radius 0) has no ring to
        // draw and no sensible place to anchor a label.
        if (value > radial.minValue + epsilon)
            values.append(value);
        // Bail out rather than subdivide the range unboundedly; this also stops the
        // loop from stalling when valueStep is too small to advance value at all.
        if (values.size() >= kMaxTicks)
            break;
    }
    return values;
}

// Number of evenly spaced spokes/angular labels around the circle. The quotient is
// clamped before being narrowed: qRound() returns int, so casting a quotient beyond
// INT_MAX would be undefined, and an unvalidated tickInterval can produce one.
int PolarAxisRenderer::angularTickCount(const AxisProperties &angular) const
{
    if (!(angular.valueRange > 0 && angular.valueStep > 0))
        return 0;

    const qreal count = angular.valueRange / angular.valueStep;
    return qRound(std::clamp<qreal>(count, 1.0, kMaxTicks));
}

// Anchors an axis label so that its edge facing (anchor) stays a constant
// "pad" pixels away from it, however large the label item is, instead of
// centering the item on (anchor) and letting it grow inward over the grid.
// (dx, dy) is the outward unit direction from the polar center through
// (anchor).
static void positionOutwardLabel(QQuickItem *item, const QPointF &anchor, qreal dx, qreal dy,
                                  qreal pad)
{
    const qreal halfWidth = item->width() / 2.0;
    const qreal halfHeight = item->height() / 2.0;
    item->setX(anchor.x() - halfWidth + dx * (halfWidth + pad));
    item->setY(anchor.y() - halfHeight + dy * (halfHeight + pad));
}

void PolarAxisRenderer::updatePolarGrid(const QList<qreal> &radialTicks)
{
    if (!theme())
        return;

    auto polarView = qobject_cast<QPolarView *>(m_graph);
    if (!polarView)
        return;

    const QPointF center = polarView->polarCenter();
    const qreal radius = polarView->polarRadius();

    auto &radial = m_axes2[0];
    QPainterPath rings;
    if (radial.axis && radial.axis->isVisible() && radial.axis->isGridVisible()) {
        for (qreal value : radialTicks) {
            qreal r = radius * (value - radial.minValue) / radial.valueRange;
            rings.addEllipse(center, r, r);
        }
    }
    m_ringsPath->setStrokeColor(theme()->axisY().mainColor());
    m_ringsPath->setStrokeWidth(theme()->axisY().mainWidth());
    m_ringsPath->setFillColor(QColorConstants::Transparent);
    m_ringsPath->setPath(rings);

    auto &angular = m_axes1[0];
    QPainterPath spokes;
    if (angular.axis && angular.axis->isVisible() && angular.axis->isGridVisible()
        && angular.valueRange > 0 && angular.valueStep > 0) {
        int spokeCount = angularTickCount(angular);
        for (int i = 0; i < spokeCount; ++i) {
            qreal angleDeg = 360.0 * i / spokeCount;
            qreal rad = qDegreesToRadians(angleDeg);
            QPointF p = center + QPointF(qSin(rad) * radius, -qCos(rad) * radius);
            spokes.moveTo(center);
            spokes.lineTo(p);
        }
    }
    m_spokesPath->setStrokeColor(theme()->axisX().mainColor());
    m_spokesPath->setStrokeWidth(theme()->axisX().mainWidth());
    m_spokesPath->setFillColor(QColorConstants::Transparent);
    m_spokesPath->setPath(spokes);

    m_shape.setSize(QSizeF(width(), height()));
}

static QString polarAxisLabelText(QAbstractAxis *axis, qreal value)
{
    if (auto dtAxis = qobject_cast<QDateTimeAxis *>(axis))
        return QDateTime::fromMSecsSinceEpoch(qint64(value)).toString(dtAxis->labelFormat());
    if (auto logAxis = qobject_cast<QLogValueAxis *>(axis))
        return QString::number(value, 'g', std::max(1, logAxis->labelPrecision()));
    return QString::number(value, 'g', 4);
}

void PolarAxisRenderer::updatePolarLabels(const QList<qreal> &radialTicks)
{
    if (!theme())
        return;

    auto polarView = qobject_cast<QPolarView *>(m_graph);
    if (!polarView)
        return;

    const QPointF center = polarView->polarCenter();
    const qreal radius = polarView->polarRadius();
    const qreal pad = polarView->labelPadding();

    auto &angular = m_axes1[0];
    if (angular.axis && angular.axis->isVisible() && angular.axis->labelsVisible()
        && angular.valueRange > 0 && angular.valueStep > 0) {
        int tickCount = angularTickCount(angular);
        updateAxisLabelItems(angular.textItems, tickCount, angular.axis->labelDelegate());
        for (int i = 0; i < tickCount && i < angular.textItems.size(); ++i) {
            qreal value = angular.minValue + i * angular.valueStep;
            qreal angleDeg = 360.0 * (value - angular.minValue) / angular.valueRange;
            qreal rad = qDegreesToRadians(angleDeg);
            qreal dx = qSin(rad);
            qreal dy = -qCos(rad);
            QPointF p = center + QPointF(dx * radius, dy * radius);

            auto item = angular.textItems[i];
            item->setVisible(true);
            setLabelTextProperties(item, polarAxisLabelText(angular.axis, value), true);
            positionOutwardLabel(item, p, dx, dy, pad);
        }
    }

    auto &radial = m_axes2[0];
    if (radial.axis && radial.axis->isVisible() && radial.axis->labelsVisible()) {
        updateAxisLabelItems(radial.textItems, radialTicks.size(), radial.axis->labelDelegate());

        for (qsizetype i = 0; i < radialTicks.size() && i < radial.textItems.size(); ++i) {
            qreal value = radialTicks[i];
            qreal r = radius * (value - radial.minValue) / radial.valueRange;

            auto item = radial.textItems[i];
            item->setVisible(true);
            setLabelTextProperties(item, polarAxisLabelText(radial.axis, value), false);
            // The label's baseline sits right on its ring, on the vertical
            // (angle 0) radial axis line, offset sideways by "pad" so it
            // doesn't run into the angular axis's own label at its minimum
            // value, which is centered right above this same line.
            item->setX(center.x() + pad);
            item->setY(center.y() - r - item->height());
        }
    }
}

QT_END_NAMESPACE

#include "moc_polaraxisrenderer_p.cpp"
