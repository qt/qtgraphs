// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
// Qt-Security score:significant reason:default


#ifndef AXISRENDERER_H
#define AXISRENDERER_H

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
#include <QRectF>
#include <QList>
#include <QList>
#include <QtQuick/private/qquicktext_p.h>
#include <private/axisgrid_p.h>
#include <private/axisticker_p.h>
#include <private/axisline_p.h>

QT_BEGIN_NAMESPACE

class QAbstractAxis;
class QGraphsView;
class QBarCategoryAxis;
class QValueAxis;
class QGraphsTheme;
class QDateTimeAxis;
class QQuickDragHandler;
class QAbstractSeries;

class AxisRenderer : public QQuickItem
{
    Q_OBJECT
public:
    AxisRenderer(QQuickItem *parent = nullptr);
    ~AxisRenderer() override;

    void handlePolish();
    void initialize();

    bool handleWheel(QWheelEvent *event);
    void handlePinchScale(qreal delta);
    void handlePinchGrab(QPointingDevice::GrabTransition transition, QEventPoint point);

Q_SIGNALS:

private:
    friend class QGraphsView;
    friend class BarsRenderer;
    friend class LinesRenderer;
    friend class PointRenderer;
    friend class AreaRenderer;
    friend class CustomRenderer;

    struct AxisProperties {
        qreal x = 0;
        qreal y = 0;

        QAbstractAxis *axis = nullptr;

        QList<QQuickItem *> textItems;
        QQuickText *title = nullptr;
        AxisTicker *ticker = nullptr;
        AxisLine *line = nullptr;
        AxisTicker *tickerShadow = nullptr;
        AxisLine *lineShadow = nullptr;

        // Max value
        double maxValue = 20;
        // Min value
        double minValue = 0;
        // Values range, so m_axisVerticalMaxValue - m_axisVerticalMinValue
        double valueRange = 0;
        double valueRangeZoomless = 0;
        // How much each major value step is
        double valueStep = 1.0;
        // px between major ticks
        double stepPx = 0;
        // Ticks movement, between -m_axisHorizontalStepPx .. m_axisHorizontalStepPx.
        double displacement = 0;
        // The value of smallest label
        double minLabel = 0;
        double subGridScale = 0.5;
        double tickCount = 0.0;
        // Pre-measured label size based on contents
        qreal labelSize = 0;
        // Total space needed by axis: labelSize + labels margin + tickers size
        qreal size = 0;
        // Per-axis labels and tickers rectangles
        QRectF labelsRect;
        QRectF tickersRect;

        bool isLogarithmic = false;
        double logBase = 10;
    };

    void populateAxisItems();
    void updateAxis();
    void updateAxisTickers();
    void updateAxisTickersShadow();
    void updateAxisGrid();
    void updateAxisGridShadow();
    void updateAxisTitles();

    bool hasAxisTitle(const AxisProperties &ax) const;
    bool isAxisBottomOrRight(const AxisProperties &ax) const;

#ifdef USE_BARGRAPH
    void updateBarXAxisLabels(AxisProperties &ax, const QRectF rect);
    void updateBarYAxisLabels(AxisProperties &ax, const QRectF rect);
#endif
    void updateValueYAxisLabels(AxisProperties &ax, const QRectF rect);
    void updateValueXAxisLabels(AxisProperties &ax, const QRectF rect);
    void updateDateTimeYAxisLabels(AxisProperties &ax, const QRectF rect);
    void updateDateTimeXAxisLabels(AxisProperties &ax, const QRectF rect);
    void updateLogValueYAxisLabels(AxisProperties &ax, const QRectF rect);
    void updateLogValueXAxisLabels(AxisProperties &ax, const QRectF rect);

    void updateAxisMeasurements();
    QString formatValueLabel(double number, int decimals, const QString &format,
                             char defaultFormat = 'f') const;

    void createDragHandler();
    void deleteDragHandler();
    void onTranslationChanged(QVector2D delta);
    void onGrabChanged(QPointingDevice::GrabTransition transition, QEventPoint point);

    double getValueStepsFromRange(double range) const;
    int getValueDecimalsFromRange(double range) const;
    void setLabelTextProperties(QQuickItem *item, const QString &text, bool xAxis,
                                QQuickText::HAlignment hAlign = QQuickText::HAlignment::AlignHCenter,
                                QQuickText::VAlignment vAlign = QQuickText::VAlignment::AlignVCenter,
                                Qt::TextElideMode elide = Qt::ElideNone);
    void updateAxisLabelItems(QList<QQuickItem *> &textItems, qsizetype neededSize,
                              QQmlComponent *component);

    QVector2D windowToAxisCoords(QVector2D coords);
    bool calculateZoom(QAbstractAxis *axis, qreal delta);
    bool zoom(qreal delta);

    const AxisProperties &getAxisX(QAbstractSeries *series) const;
    const AxisProperties &getAxisY(QAbstractSeries *series) const;

    QGraphsView *m_graph = nullptr;
    QGraphsTheme *theme() const;
    bool m_initialized = false;
    bool m_wasVertical = false;

    QList<AxisProperties> m_axes1;
    QList<AxisProperties> m_axes2;
    QList<AxisProperties> *m_horzAxes = &m_axes1;
    QList<AxisProperties> *m_vertAxes = &m_axes2;

    AxisGrid *m_axisGrid = nullptr;
    AxisGrid *m_axisGridShadow = nullptr;

    bool m_gridHorizontalLinesVisible = true;
    bool m_gridVerticalLinesVisible = true;
    bool m_gridHorizontalSubLinesVisible = false;
    bool m_gridVerticalSubLinesVisible = false;

    qreal m_y1AxisWidth = 0;
    qreal m_y2AxisWidth = 0;
    qreal m_x1AxisHeight = 0;
    qreal m_x2AxisHeight = 0;

    QQuickDragHandler *m_dragHandler = nullptr;

    struct DragState
    {
        bool dragging = false;
        QVector2D touchPositionAtPress;
        QVector2D panAtPress;
        QVector2D delta;
    };

    DragState m_dragState;
};

QT_END_NAMESPACE

#endif // AXISRENDERER_H
