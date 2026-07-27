// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
// Qt-Security score:significant reason:default

#include <QtGraphs/qareaseries.h>
#if QT_CONFIG(graphs_2d_spline)
#include <QtGraphs/qsplineseries.h>
#endif
#include <QtQuick/private/qquicktaphandler_p.h>
#include <QtQuickShapes/private/qquickshape_p.h>
#include <private/arearenderer_p.h>
#include <private/axisrenderer_p.h>
#include <private/pointrenderer_p.h>
#include <private/qabstractseries_p.h>
#include <private/qareaseries_p.h>
#include <private/qgraphsview_p.h>
#include <private/qxyseries_p.h>
#if QT_CONFIG(graphs_2d_high_performance_backend)
#include <QtCanvasPainter/QCanvasPainter>
#include <QtCanvasPainter/QCanvasLinearGradient>
#include <QtCanvasPainter/QCanvasConicalGradient>
#include <QtCanvasPainter/QCanvasRadialGradient>
#include <QLinearGradient>
#include <QRadialGradient>
#include <QConicalGradient>
#endif

#include <qtgraphs_tracepoints_p.h>

QT_BEGIN_NAMESPACE

Q_TRACE_PREFIX(qtgraphs,
              "QT_BEGIN_NAMESPACE" \
              "class AreaRenderer;" \
              "QT_END_NAMESPACE"
          )

Q_TRACE_POINT(qtgraphs, QGraphs2DAreaRenderePointInArea_entry, int x, int y);
Q_TRACE_POINT(qtgraphs, QGraphs2DAreaRenderePointInArea_exit);

Q_TRACE_POINT(qtgraphs, QGraphs2DAreaRendereAfterPolish_entry);
Q_TRACE_POINT(qtgraphs, QGraphs2DAreaRendereAfterPolish_exit);

Q_TRACE_POINT(qtgraphs, QGraphs2DAreaRendererCalculateSeriesU_entry);
Q_TRACE_POINT(qtgraphs, QGraphs2DAreaRendererCalculateSeriesU_exit);

Q_TRACE_POINT(qtgraphs, QGraphs2DAreaRendererCalculateSeriesL_entry);
Q_TRACE_POINT(qtgraphs, QGraphs2DAreaRendererCalculateSeriesL_exit);

AreaRenderer::AreaRenderer(QGraphsView *graph, bool clipPlotArea)
    : QQuickItem(graph)
    , m_graph(graph)
{
    setFlag(QQuickItem::ItemHasContents);
    setClip(clipPlotArea);

#if QT_CONFIG(graphs_2d_high_quality_backend)
    m_shape.setParentItem(this);
    m_shape.setPreferredRendererType(QQuickShape::CurveRenderer);
#endif

    m_tapHandler = new QQuickTapHandler(this);
    connect(m_tapHandler, &QQuickTapHandler::singleTapped, this, &AreaRenderer::onSingleTapped);
    connect(m_tapHandler, &QQuickTapHandler::doubleTapped, this, &AreaRenderer::onDoubleTapped);
    connect(m_tapHandler, &QQuickTapHandler::pressedChanged, this, &AreaRenderer::onPressedChanged);
}

AreaRenderer::~AreaRenderer()
{
    qDeleteAll(m_groups);
}

void AreaRenderer::resetShapePathCount()
{
    m_currentShapePathIndex = 0;
}

#if QT_CONFIG(graphs_2d_high_performance_backend)
AreaRenderer::PaintSnapshot AreaRenderer::paintSnapshot() const
{
    return m_areaPaintSnapshot;
}

void AreaRenderer::synchronizeData()
{
    m_areaPaintSnapshot.clear();
    for (auto &&group : m_groups) {
        if (group->painterPath.elementCount() == 0)
            continue;

        const auto style = getSeriesStyle(group);
        AreaPaintData paintData;
        if (auto linear = qobject_cast<QQuickShapeLinearGradient *>(style.gradient)) {
            QLinearGradient gradient(linear->x1(), linear->y1(), linear->x2(), linear->y2());
            gradient.setStops(linear->gradientStops());
            paintData.gradient = gradient;
        } else if (auto radial = qobject_cast<QQuickShapeRadialGradient *>(style.gradient)) {
            QPointF center(radial->centerX(), radial->centerY());
            QPointF focal(radial->focalX(), radial->focalY());
            QRadialGradient gradient(center, radial->centerRadius(), focal, radial->focalRadius());
            gradient.setStops(radial->gradientStops());
            paintData.gradient = gradient;
        } else if (auto conical = qobject_cast<QQuickShapeConicalGradient *>(style.gradient)) {
            QConicalGradient gradient(conical->centerX(), conical->centerY(), conical->angle());
            gradient.setStops(conical->gradientStops());
            paintData.gradient = gradient;
        } else {
            paintData.resolvedColor = style.color;
        }
        paintData.resolvedBorderColor = style.borderColor;
        paintData.resolvedBorderWidth = style.borderWidth;
        paintData.painterPath = group->painterPath;
        m_areaPaintSnapshot.append(paintData);
    }
}

void AreaRenderer::paintSnapshot(const PaintSnapshot &snapshot, QCanvasPainter *p)
{
    for (const auto &areaPaintSnapshot : snapshot) {
        const auto &gradient = areaPaintSnapshot.gradient;
        switch (gradient.type()) {
        case QGradient::LinearGradient: {
            const auto &lg = static_cast<const QLinearGradient &>(gradient);
            QCanvasLinearGradient canvasGradient(lg.start().x(),
                                                 lg.start().y(),
                                                 lg.finalStop().x(),
                                                 lg.finalStop().y());
            const QGradientStops stops = lg.stops();
            for (const auto &stop : stops)
                canvasGradient.setColorAt(stop.first, stop.second);
            p->setFillStyle(canvasGradient);
        } break;
        case QGradient::RadialGradient: {
            const auto &rg = static_cast<const QRadialGradient &>(gradient);
            QCanvasRadialGradient canvasGradient(rg.center().x(),
                                                 rg.center().y(),
                                                 rg.centerRadius(),
                                                 rg.focalRadius());
            const QGradientStops stops = rg.stops();
            for (const auto &stop : stops)
                canvasGradient.setColorAt(stop.first, stop.second);
            p->setFillStyle(canvasGradient);
        } break;
        case QGradient::ConicalGradient: {
            const auto &cg = static_cast<const QConicalGradient &>(gradient);
            QCanvasConicalGradient canvasGradient(cg.center().x(), cg.center().y(), cg.angle());
            const QGradientStops stops = cg.stops();
            for (const auto &stop : stops)
                canvasGradient.setColorAt(stop.first, stop.second);
            p->setFillStyle(canvasGradient);
        } break;
        case QGradient::NoGradient: {
            p->setFillStyle(areaPaintSnapshot.resolvedColor);
        }
        }
        p->setStrokeStyle(areaPaintSnapshot.resolvedBorderColor);
        p->setLineWidth(areaPaintSnapshot.resolvedBorderWidth);
        p->beginPath();
        p->addPath(areaPaintSnapshot.painterPath);
        p->fill();
        p->stroke();
    }
}
#endif

AreaRenderer::SeriesStyle AreaRenderer::getSeriesStyle(PointGroup *group)
{
    auto theme = m_graph->theme();

    const auto &seriesColors = theme->seriesColors();
    qsizetype index = group->colorIndex % seriesColors.size();
    QColor color = group->series->color().alpha() != 0 ? group->series->color()
                                                       : seriesColors.at(index);
    const auto &borderColors = theme->borderColors();
    index = group->colorIndex % borderColors.size();
    QColor borderColor = group->series->borderColor().alpha() != 0 ? group->series->borderColor()
                                                                   : borderColors.at(index);

    QQuickShapeGradient *gradient = group->series->gradient();

    if (group->series->isSelected()) {
        color = group->series->selectedColor().alpha() != 0 ? group->series->selectedColor()
                                                            : color.lighter();
        borderColor = group->series->selectedBorderColor().alpha() != 0
                          ? group->series->selectedBorderColor()
                          : borderColor.lighter();
        if (group->series->selectedGradient())
            gradient = group->series->selectedGradient();
    }

    qreal borderWidth = group->series->borderWidth();
    if (qFuzzyCompare(borderWidth, qreal(-1.0)))
        borderWidth = theme->borderWidth();

    return {color, gradient, borderColor, borderWidth};
}

void AreaRenderer::calculateRenderCoordinates(
    QAreaSeries *series, qreal origX, qreal origY, qreal *renderX, qreal *renderY) const
{
    auto &axY = m_graph->m_axisRenderer->getAxisY(series);
    auto &axX = m_graph->m_axisRenderer->getAxisX(series);

    float x = origX;
    float y = origY;

    if (axX.isLogarithmic) {
        float logBase = log(axX.logBase);
        x = log(origX) / logBase;
    }

    if (axY.isLogarithmic) {
        float logBase = log(axY.logBase);
        y = log(origY) / logBase;
    }

    if (m_graph->orientation() != Qt::Vertical) {
        std::swap(x, y);
        y = axY.maxValue - y;
    }

    *renderX = m_areaWidth * x * m_maxHorizontal - m_horizontalOffset;
    *renderY = m_areaHeight - m_areaHeight * y * m_maxVertical
               + m_verticalOffset;
}

void AreaRenderer::calculateAxisCoordinates(
    QAreaSeries *series, qreal origX, qreal origY, qreal *axisX, qreal *axisY) const
{
    auto &axY = m_graph->m_axisRenderer->getAxisY(series);
    auto &axX = m_graph->m_axisRenderer->getAxisX(series);

    if (m_graph->orientation() != Qt::Vertical) {
        std::swap(origX, origY);
        origY = axY.maxValue - origY;
    }

    qreal x = (origX + m_horizontalOffset) / (m_areaWidth * m_maxHorizontal);
    qreal y = (origY - m_areaHeight - m_verticalOffset)
             / (-1 * m_areaHeight * m_maxVertical);

    if (axX.isLogarithmic)
        x = pow(axX.logBase, x);

    if (axY.isLogarithmic)
        y = pow(axY.logBase, y);

    *axisX = x;
    *axisY = y;
}

void AreaRenderer::handlePolish(QAreaSeries *series)
{
    auto theme = m_graph->theme();
    if (!theme) {
        qCCritical(lcCritical2D, "theme not found.");
        return;
    }

    if (!m_graph->m_axisRenderer) {
        qCCritical(lcCritical2D, "axis renderer not found.");
        return;
    }

    QXYSeries *upper = series->upperSeries();
    QXYSeries *lower = series->lowerSeries();

    if (!upper) {
        qCCritical(lcCritical2D, "upperSeries not found.");
        return;
    }

    if (!m_groups.contains(series)) {
        PointGroup *group = new PointGroup();
        group->series = series;
        m_groups.insert(series, group);

#if QT_CONFIG(graphs_2d_high_quality_backend)
        group->shapePath = new QQuickShapePath(&m_shape);
        auto data = m_shape.data();
        data.append(&data, m_groups.value(series)->shapePath);
#endif
    }

    auto group = m_groups.value(series);

#if QT_CONFIG(graphs_2d_high_quality_backend)
    auto data = m_shape.data();
    group->shapePath = qobject_cast<QQuickShapePath *>(data.at(&data, m_currentShapePathIndex));
#endif

    m_currentShapePathIndex++;

    if (upper->points().count() < 2 || (lower && lower->points().count() < 2)) {
        group->painterPath.clear();
#if QT_CONFIG(graphs_2d_high_quality_backend)
        group->shapePath->setPath(group->painterPath);
#endif
        return;
    }

    m_areaWidth = width();
    m_areaHeight = height();

    auto &axisX = m_graph->m_axisRenderer->getAxisX(group->series);
    auto &axisY = m_graph->m_axisRenderer->getAxisY(group->series);

    m_maxVertical = axisY.valueRange > 0 ? 1.0 / axisY.valueRange : 100.0;
    m_maxHorizontal = axisX.valueRange > 0 ? 1.0 / axisX.valueRange : 100.0;
    m_verticalOffset = (axisY.minValue / axisY.valueRange) * m_areaHeight;
    m_horizontalOffset = (axisX.minValue / axisX.valueRange) * m_areaWidth;

    auto &painterPath = group->painterPath;
    painterPath.clear();

    if (group->colorIndex < 0) {
        group->colorIndex = m_graph->graphSeriesCount();
        m_graph->setGraphSeriesCount(group->colorIndex + 1);
    }

    const auto style = getSeriesStyle(group);

#if QT_CONFIG(graphs_2d_high_quality_backend)
    if (!m_graph->useCanvasPainter()) {
        group->shapePath->setStrokeWidth(style.borderWidth);
        group->shapePath->setStrokeColor(style.borderColor);
        group->shapePath->setFillColor(style.color);
        group->shapePath->setFillGradient(style.gradient);
        group->shapePath->setCapStyle(QQuickShapePath::CapStyle::SquareCap);
    }
#endif

    auto &&upperPoints = upper->points();
    QList<QPointF> fittedPoints;
#if QT_CONFIG(graphs_2d_spline)
    if (upper->type() == QAbstractSeries::SeriesType::Spline)
        fittedPoints = qobject_cast<QSplineSeries *>(upper)->getControlPoints();
#endif

    int extraPointCount = lower ? 0 : 3;

    if (series->isVisible()) {
        Q_TRACE_SCOPE(QGraphs2DAreaRendererCalculateSeriesU);
        for (int i = 0, j = 0; i < upperPoints.size() + extraPointCount; ++i, ++j) {
            qreal x;
            qreal y;
            qreal upperX;
            qreal upperY;
            if (i == upperPoints.size()) {
                upperX = upperPoints[upperPoints.size() - 1].x();
                upperY = 0;
            } else if (i == upperPoints.size() + 1) {
                upperX = upperPoints[0].x();
                upperY = 0;
            } else if (i == upperPoints.size() + 2) {
                upperX = upperPoints[0].x();
                upperY = upperPoints[0].y();
            } else {
                upperX = upperPoints[i].x();
                upperY = upperPoints[i].y();
            }
            calculateRenderCoordinates(series, upperX, upperY, &x, &y);

            if (i == 0) {
                painterPath.moveTo(x, y);
            } else {
                if (i < upper->points().size()
                    && upper->type() == QAbstractSeries::SeriesType::Spline) {
                    qreal x1, y1, x2, y2;
                    calculateRenderCoordinates(series,
                                               fittedPoints[j - 1].x(),
                                               fittedPoints[j - 1].y(),
                                               &x1,
                                               &y1);
                    calculateRenderCoordinates(series,
                                               fittedPoints[j].x(),
                                               fittedPoints[j].y(),
                                               &x2,
                                               &y2);

                    painterPath.cubicTo(x1, y1, x2, y2, x, y);
                    ++j;
                } else {
                    painterPath.lineTo(x, y);
                }
            }
        }
    }

    if (lower && series->isVisible()) {
        Q_TRACE_SCOPE(QGraphs2DAreaRendererCalculateSeriesL);
        auto &&lowerPoints = lower->points();
        QList<QPointF> fittedPoints;
#if QT_CONFIG(graphs_2d_spline)
        if (lower->type() == QAbstractSeries::SeriesType::Spline)
            fittedPoints = qobject_cast<QSplineSeries *>(lower)->getControlPoints();
#endif

        for (int i = 0, j = 0; i < lowerPoints.size(); ++i, ++j) {
            qreal x, y;
            calculateRenderCoordinates(series,
                                       lowerPoints[lowerPoints.size() - 1 - i].x(),
                                       lowerPoints[lowerPoints.size() - 1 - i].y(),
                                       &x,
                                       &y);

            if (i > 0 && lower->type() == QAbstractSeries::SeriesType::Spline) {
                qreal x1, y1, x2, y2;
                calculateRenderCoordinates(series,
                                           fittedPoints[fittedPoints.size() - 1 - j + 1].x(),
                                           fittedPoints[fittedPoints.size() - 1 - j + 1].y(),
                                           &x1,
                                           &y1);
                calculateRenderCoordinates(series,
                                           fittedPoints[fittedPoints.size() - 1 - j].x(),
                                           fittedPoints[fittedPoints.size() - 1 - j].y(),
                                           &x2,
                                           &y2);

                painterPath.cubicTo(x1, y1, x2, y2, x, y);
                ++j;
            } else {
                painterPath.lineTo(x, y);
            }
        }

        qreal x, y;
        calculateRenderCoordinates(series, upperPoints[0].x(), upperPoints[0].y(), &x, &y);
        painterPath.lineTo(x, y);
    }

#if QT_CONFIG(graphs_2d_high_quality_backend)
    if (!m_graph->useCanvasPainter())
        group->shapePath->setPath(painterPath);
#endif

    QList<QLegendData> legendDataList = {{style.color, style.borderColor, series->name()}};
    series->d_func()->setLegendData(legendDataList);
}

void AreaRenderer::afterPolish(QList<QAbstractSeries *> &cleanupSeries)
{
    Q_TRACE_SCOPE(QGraphs2DAreaRendereAfterPolish);
    Q_UNUSED(cleanupSeries);
#if QT_CONFIG(graphs_2d_high_quality_backend)
    auto data = m_shape.data();
    for (qsizetype i = m_currentShapePathIndex, count = data.count(&data); i < count; ++i) {
        auto shapePath = qobject_cast<QQuickShapePath *>(data.at(&data, i));
        if (shapePath) {
            QPainterPath empty;
            shapePath->setPath(empty);
        }
    }
#endif
}

void AreaRenderer::afterUpdate(QList<QAbstractSeries *> &cleanupSeries)
{
    Q_UNUSED(cleanupSeries);
}

void AreaRenderer::updateSeries(QAreaSeries *series)
{
    Q_UNUSED(series);
}

void AreaRenderer::seriesAboutToBeRemoved(QAbstractSeries *series)
{
    if (auto *areaSeries = qobject_cast<QAreaSeries *>(series)) {
        auto iter = m_groups.find(areaSeries);

        if (iter != m_groups.end()) {
            delete *iter;
            m_groups.erase(iter);
        }
    }
}

// Point inside triangle code from
// https://stackoverflow.com/questions/2049582/how-to-determine-if-a-point-is-in-a-2d-triangle
float sign(QPoint p1, QPoint p2, QPoint p3)
{
    return (p1.x() - p3.x()) * (p2.y() - p3.y()) - (p2.x() - p3.x()) * (p1.y() - p3.y());
}

bool pointInTriangle(QPoint pt, QPoint v1, QPoint v2, QPoint v3)
{
    float d1, d2, d3;
    bool hasNeg, hasPos;

    d1 = sign(pt, v1, v2);
    d2 = sign(pt, v2, v3);
    d3 = sign(pt, v3, v1);

    hasNeg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    hasPos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(hasNeg && hasPos);
}

bool AreaRenderer::pointInArea(QPoint pt, QAreaSeries *series) const
{
    Q_TRACE_SCOPE(QGraphs2DAreaRenderePointInArea, pt.x(), pt.y());
    QList<QPointF> upperPoints = series->upperSeries()->points();
    QList<QPointF> lowerPoints;

    if (series->lowerSeries())
        lowerPoints = series->lowerSeries()->points();

    QList<QPointF> *firstPoints = &upperPoints;
    if (lowerPoints.size() > upperPoints.size())
        firstPoints = &lowerPoints;

    for (int i = 0; i < firstPoints->size() - 1; ++i) {
        qreal x1, y1, x2, y2, x3, y3, x4, y4;
        calculateRenderCoordinates(series, (*firstPoints)[i].x(), (*firstPoints)[i].y(), &x1, &y1);
        calculateRenderCoordinates(series,
                                   (*firstPoints)[i + 1].x(),
                                   (*firstPoints)[i + 1].y(),
                                   &x2,
                                   &y2);

        bool needSecondTriangleTest = true;
        if (series->lowerSeries()) {
            QList<QPointF> *secondPoints = &lowerPoints;
            if (lowerPoints.size() > upperPoints.size())
                secondPoints = &upperPoints;

            qsizetype firstIndex = i;
            qsizetype secondIndex = i + 1;

            if (firstIndex >= secondPoints->size())
                firstIndex = secondPoints->size() - 1;
            if (secondIndex >= secondPoints->size())
                needSecondTriangleTest = false;

            calculateRenderCoordinates(series,
                                       (*secondPoints)[firstIndex].x(),
                                       (*secondPoints)[firstIndex].y(),
                                       &x3,
                                       &y3);

            if (needSecondTriangleTest) {
                calculateRenderCoordinates(series,
                                           (*secondPoints)[secondIndex].x(),
                                           (*secondPoints)[secondIndex].y(),
                                           &x4,
                                           &y4);
            } else {
                x4 = 0.0;
                y4 = 0.0;
            }
        } else {
            calculateRenderCoordinates(series, upperPoints[i].x(), 0, &x3, &y3);
            calculateRenderCoordinates(series, upperPoints[i + 1].x(), 0, &x4, &y4);
        }

        QPoint point1(x1, y1);
        QPoint point2(x2, y2);
        QPoint point3(x3, y3);
        QPoint point4(x4, y4);

        if (pointInTriangle(pt, point1, point2, point3)
            || (needSecondTriangleTest && pointInTriangle(pt, point2, point3, point4))) {
            return true;
        }
    }

    return false;
}

bool AreaRenderer::handleHoverMove(QHoverEvent *event)
{
    bool handled = false;
    const QPointF &position = event->position();

    for (auto &&group : m_groups) {
        if (!group->series->isHoverable() || !group->series->isVisible())
            continue;

        if (!group->series->upperSeries() || group->series->upperSeries()->count() < 2)
            continue;

        if (group->series->lowerSeries() && group->series->lowerSeries()->count() < 2)
            continue;

        const QString &name = group->series->name();

        bool hovering = false;
        if (pointInArea(position.toPoint(), group->series)) {
            qreal x, y;
            calculateAxisCoordinates(group->series, position.x(), position.y(), &x, &y);

            if (!group->hover) {
                group->hover = true;
                group->series->setHovered(true);
                emit group->series->hoverEnter(name, position, QPointF(x, y));
            }

            emit group->series->hover(name, position, QPointF(x, y));
            hovering = true;
            handled = true;
        }

        if (!hovering && group->hover) {
            group->hover = false;
            group->series->setHovered(false);
            emit group->series->hoverExit(name, position);
            handled = true;
        }
    }
    return handled;
}

void AreaRenderer::onSingleTapped(QEventPoint eventPoint, Qt::MouseButton button)
{
    Q_UNUSED(button)

    for (auto &&group : m_groups) {
        if (!group->series->isSelectable() || !group->series->isVisible())
            continue;

        if (!group->series->upperSeries() || group->series->upperSeries()->count() < 2)
            continue;

        if (group->series->lowerSeries() && group->series->lowerSeries()->count() < 2)
            continue;

        if (pointInArea(eventPoint.position().toPoint(), group->series)) {
            group->series->setSelected(!group->series->isSelected());
            m_graph->polishAndUpdate();
            qreal x;
            qreal y;
            calculateAxisCoordinates(group->series,
                                     eventPoint.position().x(),
                                     eventPoint.position().y(),
                                     &x,
                                     &y);
            emit group->series->clicked(QPoint(x, y));
        }
    }
}

void AreaRenderer::onDoubleTapped(QEventPoint eventPoint, Qt::MouseButton button)
{
    Q_UNUSED(button)

    for (auto &&group : m_groups) {
        if (!group->series->isSelectable() || !group->series->isVisible())
            continue;

        if (!group->series->upperSeries() || group->series->upperSeries()->count() < 2)
            continue;

        if (group->series->lowerSeries() && group->series->lowerSeries()->count() < 2)
            continue;

        if (pointInArea(eventPoint.position().toPoint(), group->series)) {
            qreal x;
            qreal y;
            calculateAxisCoordinates(group->series,
                                     eventPoint.position().x(),
                                     eventPoint.position().y(),
                                     &x,
                                     &y);
            emit group->series->doubleClicked(QPoint(x, y));
        }
    }
}

void AreaRenderer::onPressedChanged()
{
    QPointF position = m_tapHandler->point().position();
    for (auto &&group : m_groups) {
        if (!group->series->isSelectable() || !group->series->isVisible())
            continue;

        if (!group->series->upperSeries() || group->series->upperSeries()->count() < 2)
            continue;

        if (group->series->lowerSeries() && group->series->lowerSeries()->count() < 2)
            continue;

        if (pointInArea(position.toPoint(), group->series)) {
            qreal x;
            qreal y;
            calculateAxisCoordinates(group->series, position.x(), position.y(), &x, &y);
            if (m_tapHandler->isPressed())
                emit group->series->pressed(QPoint(x, y));
            else
                emit group->series->released(QPoint(x, y));
        }
    }
}

QT_END_NAMESPACE

#include "moc_arearenderer_p.cpp"
