// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QQUICKGRAPHSITEM_H
#define QQUICKGRAPHSITEM_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the QtGraphs API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

#include <QtQuick/QQuickItem>
#include <QList>
#include <QQmlListProperty>
#include <QtGraphs2D/qabstractseries.h>

#include <QPen>
#include <QBrush>
#include <QtQuick/private/qsgdefaultinternalrectanglenode_p.h>
#include <QtQuick/private/qquicktext_p.h>
#include <private/axisgrid_p.h>
#include <private/axisticker_p.h>
#include <QtGraphs2D/graphtheme.h>
#include <QtQuickShapes/private/qquickshape_p.h>
#include <QtQuick/QSGClipNode>

QT_BEGIN_NAMESPACE

class QBarSeries;
class QBarCategoryAxis;
class QValueAxis;
class QLineSeries;
class QBarSet;

// KG: TODO
class QChartPrivate
{
public:
    static QPen &defaultPen() { static QPen temp; return temp; };
    static QBrush &defaultBrush() { static QBrush temp; return temp; };
    static QFont &defaultFont() { static QFont temp; return temp; };
};

class QQuickGraphs2DView : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(GraphTheme *theme READ theme WRITE setTheme NOTIFY themeChanged)
    Q_PROPERTY(QQmlListProperty<QObject> seriesList READ seriesList CONSTANT)
    // TODO: Remove this?
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
    Q_PROPERTY(qreal marginTop READ marginTop WRITE setMarginTop NOTIFY marginTopChanged)
    Q_PROPERTY(qreal marginBottom READ marginBottom WRITE setMarginBottom NOTIFY marginBottomChanged)
    Q_PROPERTY(qreal marginLeft READ marginLeft WRITE setMarginLeft NOTIFY marginLeftChanged)
    Q_PROPERTY(qreal marginRight READ marginRight WRITE setMarginRight NOTIFY marginRightChanged)
    Q_CLASSINFO("DefaultProperty", "seriesList")
    QML_NAMED_ELEMENT(ChartView)

public:
    explicit QQuickGraphs2DView(QQuickItem *parent = nullptr);
    virtual ~QQuickGraphs2DView();

    void setBackgroundColor(QColor color);
    QColor backgroundColor();

    Q_INVOKABLE void addSeries(QObject *series);
    Q_INVOKABLE void removeSeries(QObject *series);
    Q_INVOKABLE void insertSeries(int index, QObject *series);
    Q_INVOKABLE bool hasSeries(QObject *series);

    QList<QObject *> getSeriesList() const {
        return m_seriesList;
    }

    QQmlListProperty<QObject> seriesList();
    static void appendSeriesFunc(QQmlListProperty<QObject> *list, QObject *series);
    static qsizetype countSeriesFunc(QQmlListProperty<QObject> *list);
    static QObject *atSeriesFunc(QQmlListProperty<QObject> *list, qsizetype index);
    static void clearSeriesFunc(QQmlListProperty<QObject> *list);

    GraphTheme *theme() const;
    void setTheme(GraphTheme *newTheme);

    qreal marginTop() const;
    void setMarginTop(qreal newMarginTop);

    qreal marginBottom() const;
    void setMarginBottom(qreal newMarginBottom);

    qreal marginLeft() const;
    void setMarginLeft(qreal newMarginLeft);

    qreal marginRight() const;
    void setMarginRight(qreal newMarginRight);

    void addAxis(QAbstractAxis *axis);
    void removeAxis(QAbstractAxis *axis);

protected:
    void componentComplete() override;
    void mousePressEvent(QMouseEvent *event) override;
    QSGNode *updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *updatePaintNodeData) override;
    void updatePolish() override;

Q_SIGNALS:
    void backgroundColorChanged();
    void themeChanged();
    void marginTopChanged();
    void marginBottomChanged();
    void marginLeftChanged();
    void marginRightChanged();

private:
    void updateAxis();
    void updateAxisTickers();
    void updateAxisGrid();
    void updateBarXAxis(QBarCategoryAxis *axis, const QRectF &rect);
    void updateBarYAxis(QValueAxis *axis, const QRectF &rect);
    void updateBarSeries(QBarSeries *series);
    void updateLineSeries(QLineSeries *series);

private:
    struct LinePath {
        QQuickShapePath *shapePath = nullptr;
        QList<QQuickPathLine *> paths;
    };
    struct BarSelectionRect {
        QBarSet *barSet = nullptr;
        QList<QRectF> rects;
    };

    QList<QObject *> m_seriesList;
    QBrush m_backgroundBrush;
    //QSGDefaultInternalRectangleNode *m_backgroundNode = nullptr;
    QSGClipNode *m_backgroundNode = nullptr;
    QList<QSGDefaultInternalRectangleNode *> m_rectNodes;
    // QSG nodes rect has no getter so we store these separately.
    QList<BarSelectionRect> m_rectNodesInputRects;
    QQuickShape m_shape;
    QMap<QLineSeries *, LinePath *> m_linePaths;
    QList<QQuickText *> m_xAxisTextItems;
    QList<QQuickText *> m_yAxisTextItems;
    QList<QAbstractAxis *> m_axis;
    AxisGrid *m_axisGrid = nullptr;
    QAbstractAxis *m_axisVertical = nullptr;
    QAbstractAxis *m_axisHorizontal = nullptr;
    AxisTicker *m_axisTickerVertical = nullptr;
    AxisTicker *m_axisTickerHorizontal = nullptr;
    // Min value
    double m_axisVerticalMaxValue = 20;
    // Max value
    double m_axisVerticalMinValue = 0;
    // Values range, so m_axisVerticalMaxValue - m_axisVerticalMinValue
    double m_axisVerticalValueRange;
    // px between major ticks
    double m_axisHorizontalStepPx = 0;
    // Ticks movement, between -m_axisHorizontalStepPx .. m_axisHorizontalStepPx.
    double m_axisYMovement = 0;
    double m_axisHorizontalMaxValue = 6;
    double m_axisHorizontalMinValue = 0;
    double m_axisHorizontalValueRange;
    double m_axisVerticalMinorTickScale = 0.5;
    double m_axisHorizontalMinorTickScale = 0.5;
    bool m_gridHorizontalMajorTicksVisible = true;
    bool m_gridVerticalMajorTicksVisible = true;
    bool m_gridHorizontalMinorTicksVisible = false;
    bool m_gridVerticalMinorTicksVisible = false;
    GraphTheme *m_theme = nullptr;
    qreal m_marginTop = 20;
    qreal m_marginBottom = 20;
    qreal m_marginLeft = 20;
    qreal m_marginRight = 20;
    // Sizes required of axis labels
    // TODO: Should these come from QAbstactAxis?
    qreal m_axisWidth = 40;
    qreal m_axisHeight = 20;
    qreal m_axisTickersWidth = 15;
    qreal m_axisTickersHeight = 15;
};

QT_END_NAMESPACE

#endif
