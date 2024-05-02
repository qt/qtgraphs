// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QGRAPHSVIEW_H
#define QGRAPHSVIEW_H

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
#include <QtGraphs/qabstractseries.h>

#include <QBrush>
#include <QPen>
#include <QtGraphs/qgraphstheme.h>
#include <QtQuick/QSGClipNode>
#include <private/arearenderer_p.h>
#include <private/axisrenderer_p.h>
#include <private/barsrenderer_p.h>
#include <private/pierenderer_p.h>
#include <private/pointrenderer_p.h>

QT_BEGIN_NAMESPACE

class QAbstractAxis;

class QGraphsView : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QGraphsTheme *theme READ theme WRITE setTheme NOTIFY themeChanged)
    Q_PROPERTY(QQmlListProperty<QObject> seriesList READ seriesList CONSTANT)
    // TODO: Remove this?
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
    Q_PROPERTY(qreal marginTop READ marginTop WRITE setMarginTop NOTIFY marginTopChanged)
    Q_PROPERTY(qreal marginBottom READ marginBottom WRITE setMarginBottom NOTIFY marginBottomChanged)
    Q_PROPERTY(qreal marginLeft READ marginLeft WRITE setMarginLeft NOTIFY marginLeftChanged)
    Q_PROPERTY(qreal marginRight READ marginRight WRITE setMarginRight NOTIFY marginRightChanged)

    Q_PROPERTY(qreal axisXSmoothing READ axisXSmoothing WRITE setAxisXSmoothing NOTIFY axisXSmoothingChanged FINAL)
    Q_PROPERTY(qreal axisYSmoothing READ axisYSmoothing WRITE setAxisYSmoothing NOTIFY axisYSmoothingChanged FINAL)
    Q_PROPERTY(qreal gridSmoothing READ gridSmoothing WRITE setGridSmoothing NOTIFY gridSmoothingChanged FINAL)

    Q_PROPERTY(bool shadowEnabled READ shadowEnabled WRITE setShadowEnabled NOTIFY shadowEnabledChanged FINAL)
    Q_PROPERTY(QColor shadowColor READ shadowColor WRITE setShadowColor NOTIFY shadowColorChanged FINAL)
    Q_PROPERTY(qreal shadowBarWidth READ shadowBarWidth WRITE setShadowBarWidth NOTIFY shadowBarWidthChanged FINAL)
    Q_PROPERTY(qreal shadowXOffset READ shadowXOffset WRITE setShadowXOffset NOTIFY shadowXOffsetChanged FINAL)
    Q_PROPERTY(qreal shadowYOffset READ shadowYOffset WRITE setShadowYOffset NOTIFY shadowYOffsetChanged FINAL)
    Q_PROPERTY(qreal shadowSmoothing READ shadowSmoothing WRITE setShadowSmoothing NOTIFY shadowSmoothingChanged FINAL)

    Q_PROPERTY(QAbstractAxis *axisX READ axisX WRITE setAxisX NOTIFY axisXChanged)
    Q_PROPERTY(QAbstractAxis *axisY READ axisY WRITE setAxisY NOTIFY axisYChanged)
    Q_PROPERTY(GraphOrientation orientation READ orientation WRITE setOrientation NOTIFY
                   orientationChanged)

    Q_CLASSINFO("DefaultProperty", "seriesList")
    QML_NAMED_ELEMENT(GraphsView)

public:
    enum class GraphOrientation { Vertical, Horizontal };
    Q_ENUM(GraphOrientation)

    explicit QGraphsView(QQuickItem *parent = nullptr);
    ~QGraphsView() override;

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

    QGraphsTheme *theme() const;
    void setTheme(QGraphsTheme *newTheme);

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

    int graphSeriesCount() const;
    void setGraphSeriesCount(int count);

    // Returns the graph series area.
    // So graphview - margins - axis.
    QRectF seriesRect() const;

    void createBarsRenderer();
    void createAxisRenderer();
    void createPointRenderer();
    void createPieRenderer();
    void createAreaRenderer();

    qreal axisXSmoothing() const;
    void setAxisXSmoothing(qreal smoothing);
    qreal axisYSmoothing() const;
    void setAxisYSmoothing(qreal smoothing);
    qreal gridSmoothing() const;
    void setGridSmoothing(qreal smoothing);

    bool shadowEnabled() const;
    void setShadowEnabled(bool newShadowEnabled);
    QColor shadowColor() const;
    void setShadowColor(const QColor &newShadowColor);
    qreal shadowBarWidth() const;
    void setShadowBarWidth(qreal newShadowBarWidth);
    qreal shadowXOffset() const;
    void setShadowXOffset(qreal newShadowXOffset);
    qreal shadowYOffset() const;
    void setShadowYOffset(qreal newShadowYOffset);
    qreal shadowSmoothing() const;
    void setShadowSmoothing(qreal smoothing);

    QAbstractAxis *axisX() const;
    void setAxisX(QAbstractAxis *axis);

    QAbstractAxis *axisY() const;
    void setAxisY(QAbstractAxis *axis);

    GraphOrientation orientation() const;
    void setOrientation(GraphOrientation newOrientation);

protected:
    void handleHoverEnter(QString seriesName, QPointF position, QPointF value);
    void handleHoverExit(QString seriesName, QPointF position);
    void handleHover(QString seriesName, QPointF position, QPointF value);
    void updateComponentSizes();
    void componentComplete() override;
    void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void hoverMoveEvent(QHoverEvent *event) override;
    QSGNode *updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *updatePaintNodeData) override;
    void updatePolish() override;

Q_SIGNALS:
    void backgroundColorChanged();
    void themeChanged();
    void marginTopChanged();
    void marginBottomChanged();
    void marginLeftChanged();
    void marginRightChanged();
    void hoverEnter(QString seriesName, QPointF position, QPointF value);
    void hoverExit(QString seriesName, QPointF position);
    void hover(QString seriesName, QPointF position, QPointF value);

    void axisXSmoothingChanged();
    void axisYSmoothingChanged();
    void gridSmoothingChanged();

    void shadowEnabledChanged();
    void shadowColorChanged();
    void shadowBarWidthChanged();
    void shadowXOffsetChanged();
    void shadowYOffsetChanged();
    void shadowSmoothingChanged();

    void axisXChanged();
    void axisYChanged();

    void orientationChanged();

private:
    friend class AxisRenderer;
    friend class BarsRenderer;
    friend class PointRenderer;
    friend class AreaRenderer;

    void polishAndUpdate();

    AxisRenderer *m_axisRenderer = nullptr;
    BarsRenderer *m_barsRenderer = nullptr;
    PointRenderer *m_pointRenderer = nullptr;
    PieRenderer *m_pieRenderer = nullptr;
    AreaRenderer *m_areaRenderer = nullptr;
    QList<QObject *> m_seriesList;
    QBrush m_backgroundBrush;
    QSGClipNode *m_backgroundNode = nullptr;

    QList<QAbstractAxis *> m_axis;

    QAbstractAxis *m_axisX = nullptr;
    QAbstractAxis *m_axisY = nullptr;
    GraphOrientation m_orientation = GraphOrientation::Vertical;

    QGraphsTheme *m_theme = nullptr;
    QGraphsTheme *m_defaultTheme = nullptr;

    int m_graphSeriesCount = 0;

    qreal m_marginTop = 20;
    qreal m_marginBottom = 20;
    qreal m_marginLeft = 20;
    qreal m_marginRight = 20;

    int m_hoverCount = 0;

    qreal m_axisXSmoothing = 1.0;
    qreal m_axisYSmoothing = 1.0;
    qreal m_gridSmoothing = 1.0;

    bool m_shadowEnabled;
    QColor m_shadowColor;
    qreal m_shadowBarWidth = 0.0;
    qreal m_shadowXOffset = 0.0;
    qreal m_shadowYOffset = 0.0;
    qreal m_shadowSmoothing = 5.0;
};

QT_END_NAMESPACE

#endif
