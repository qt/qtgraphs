// Copyright (C) 2025 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
#include "qquickgraphsbarsnode_p.h"

QT_BEGIN_NAMESPACE

QQuickGraphsBarsNode::QQuickGraphsBarsNode(QQuick3DNode *parent)
    : QQuickGraphsNode(parent)
    , m_multiSeriesUniform(false)
    , m_barSpacing(QSizeF(1.0f, 1.0f))
    , m_barThickness(1.0f)
    , m_barSpacingRelative(false)
    , m_floorLevel(0.0f)
{}

QQuickGraphsBarsNode::~QQuickGraphsBarsNode() {}

void QQuickGraphsBarsNode::componentComplete()
{
    const QString qmlData = QLatin1StringView(R"QML(
        import QtQuick;
        import QtGraphs;

        Bars3D{}
    )QML");

    QQmlComponent *component = new QQmlComponent(qmlEngine(this), this);
    component->setData(qmlData.toUtf8(), QUrl());
    m_graph.reset(qobject_cast<QQuickGraphsItem *>(component->create()));
    setGraphParent();
    QQuickGraphsNode::componentComplete();

    //initialize components

    if (m_axisX)
        graphBars()->setAxisX(static_cast<QCategory3DAxis *>(m_axisX));
    if (m_axisY)
        graphBars()->setAxisY(static_cast<QValue3DAxis *>(m_axisY));
    if (m_axisZ)
        graphBars()->setAxisZ(static_cast<QCategory3DAxis *>(m_axisZ));

    graphBars()->setSelectionMode(m_selectionMode);
    graphBars()->setMultiSeriesUniform(m_multiSeriesUniform);
    graphBars()->setBarSpacing(m_barSpacing);
    graphBars()->setBarThickness(m_barThickness);
    graphBars()->setBarSpacingRelative(m_barSpacingRelative);
    graphBars()->setFloorLevel(m_floorLevel);

    for (auto series : m_seriesList)
        graphBars()->addSeries(static_cast<QBar3DSeries *>(series));

    //connect signals
    connect(graphBars(),
            &QQuickGraphsBars::rowAxisChanged,
            this,
            &QQuickGraphsBarsNode::rowAxisChanged);
    connect(graphBars(),
            &QQuickGraphsBars::valueAxisChanged,
            this,
            &QQuickGraphsBarsNode::valueAxisChanged);
    connect(graphBars(),
            &QQuickGraphsBars::columnAxisChanged,
            this,
            &QQuickGraphsBarsNode::columnAxisChanged);
    connect(graphBars(),
            &QQuickGraphsBars::multiSeriesUniformChanged,
            this,
            &QQuickGraphsBarsNode::multiSeriesUniformChanged);
    connect(graphBars(),
            &QQuickGraphsBars::barThicknessChanged,
            this,
            &QQuickGraphsBarsNode::barThicknessChanged);
    connect(graphBars(),
            &QQuickGraphsBars::barSpacingChanged,
            this,
            &QQuickGraphsBarsNode::barSpacingChanged);
    connect(graphBars(),
            &QQuickGraphsBars::barSpacingRelativeChanged,
            this,
            &QQuickGraphsBarsNode::barSpacingRelativeChanged);
    connect(graphBars(),
            &QQuickGraphsBars::barSeriesMarginChanged,
            this,
            &QQuickGraphsBarsNode::barSeriesMarginChanged);
    connect(graphBars(),
            &QQuickGraphsBars::primarySeriesChanged,
            this,
            &QQuickGraphsBarsNode::primarySeriesChanged);
    connect(graphBars(),
            &QQuickGraphsBars::selectedSeriesChanged,
            this,
            &QQuickGraphsBarsNode::selectedSeriesChanged);
    connect(graphBars(),
            &QQuickGraphsBars::floorLevelChanged,
            this,
            &QQuickGraphsBarsNode::floorLevelChanged);
}

void QQuickGraphsBarsNode::setRowAxis(QCategory3DAxis *axis)
{
    if (m_axisZ == axis)
        return;

    m_axisZ = axis;
    if (graphBars())
        graphBars()->setRowAxis(axis);
}

QCategory3DAxis *QQuickGraphsBarsNode::rowAxis() const
{
    if (graphBars())
        return graphBars()->rowAxis();
    else
        return static_cast<QCategory3DAxis *>(m_axisZ);
}

void QQuickGraphsBarsNode::setValueAxis(QValue3DAxis *axis)
{
    if (m_axisY == axis)
        return;

    m_axisY = axis;
    if (graphBars())
        graphBars()->setValueAxis(axis);
}

QValue3DAxis *QQuickGraphsBarsNode::valueAxis() const
{
    if (graphBars())
        return graphBars()->valueAxis();
    else
        return static_cast<QValue3DAxis *>(m_axisY);
}

void QQuickGraphsBarsNode::setColumnAxis(QCategory3DAxis *axis)
{
    if (m_axisX == axis)
        return;

    m_axisX = axis;
    if (graphBars())
        graphBars()->setColumnAxis(axis);
}

QCategory3DAxis *QQuickGraphsBarsNode::columnAxis() const
{
    if (graphBars())
        return graphBars()->columnAxis();
    else
        return static_cast<QCategory3DAxis *>(m_axisX);
}

void QQuickGraphsBarsNode::setMultiSeriesUniform(bool uniform)
{
    if (m_multiSeriesUniform == uniform)
        return;
    m_multiSeriesUniform = uniform;
    if (graphBars())
        graphBars()->setMultiSeriesUniform(uniform);
}

bool QQuickGraphsBarsNode::isMultiSeriesUniform() const
{
    if (graphBars())
        return graphBars()->isMultiSeriesUniform();
    else
        return m_multiSeriesUniform;
}

void QQuickGraphsBarsNode::setBarThickness(float thickness)
{
    if (m_barThickness == thickness)
        return;
    m_barThickness = thickness;
    if (graphBars())
        graphBars()->setBarThickness(thickness);
}

float QQuickGraphsBarsNode::barThickness() const
{
    if (graphBars())
        return graphBars()->barThickness();
    else
        return m_barThickness;
}

void QQuickGraphsBarsNode::setBarSpacing(QSizeF spacing)
{
    if (m_barSpacing == spacing)
        return;
    m_barSpacing = spacing;
    if (graphBars())
        graphBars()->setBarSpacing(spacing);
}

QSizeF QQuickGraphsBarsNode::barSpacing() const
{
    if (graphBars())
        return graphBars()->barSpacing();
    else
        return m_barSpacing;
}

void QQuickGraphsBarsNode::setBarSpacingRelative(bool relative)
{
    if (m_barSpacingRelative == relative)
        return;
    m_barSpacingRelative = relative;

    if (graphBars())
        graphBars()->setBarSpacingRelative(relative);
}

bool QQuickGraphsBarsNode::isBarSpacingRelative() const
{
    if (graphBars())
        return graphBars()->isBarSpacingRelative();
    else
        return m_barSpacingRelative;
}

void QQuickGraphsBarsNode::setBarSeriesMargin(QSizeF margin)
{
    if (m_barSeriesMargin == margin)
        return;

    m_barSeriesMargin = margin;
    if (graphBars())
        graphBars()->setBarSeriesMargin(margin);
}

QSizeF QQuickGraphsBarsNode::barSeriesMargin() const
{
    if (graphBars())
        return graphBars()->barSeriesMargin();
    else
        return m_barSeriesMargin;
}

QBar3DSeries *QQuickGraphsBarsNode::selectedSeries() const
{
    if (graphBars())
        return graphBars()->selectedSeries();
    else
        return nullptr;
}

void QQuickGraphsBarsNode::setPrimarySeries(QBar3DSeries *series)
{
    if (m_primarySeries == series)
        return;

    m_primarySeries = series;
    if (graphBars())
        graphBars()->setPrimarySeries(series);
}

QBar3DSeries *QQuickGraphsBarsNode::primarySeries() const
{
    if (graphBars())
        return graphBars()->primarySeries();
    else
        return m_primarySeries;
}

void QQuickGraphsBarsNode::setFloorLevel(float floorLevel)
{
    if (m_floorLevel == floorLevel)
        return;

    m_floorLevel = floorLevel;
    if (graphBars())
        graphBars()->setFloorLevel(floorLevel);
}

float QQuickGraphsBarsNode::floorLevel() const
{
    if (graphBars())
        return graphBars()->floorLevel();
    else
        return m_floorLevel;
}

QList<QBar3DSeries *> QQuickGraphsBarsNode::barSeriesList()
{
    if (graphBars()) {
        return graphBars()->barSeriesList();
    } else {
        QList<QBar3DSeries *> barSeriesList;
        for (QAbstract3DSeries *abstractSeries : m_seriesList) {
            QBar3DSeries *barSeries = qobject_cast<QBar3DSeries *>(abstractSeries);
            if (barSeries)
                barSeriesList.append(barSeries);
        }
        return barSeriesList;
    }
}

QQmlListProperty<QBar3DSeries> QQuickGraphsBarsNode::seriesList()
{
    if (graphBars()) {
        return graphBars()->seriesList();
    } else {
        return QQmlListProperty<QBar3DSeries>(this,
                                              this,
                                              &QQuickGraphsBarsNode::appendSeriesFunc,
                                              &QQuickGraphsBarsNode::countSeriesFunc,
                                              &QQuickGraphsBarsNode::atSeriesFunc,
                                              &QQuickGraphsBarsNode::clearSeriesFunc);
    }
}

void QQuickGraphsBarsNode::appendSeriesFunc(QQmlListProperty<QBar3DSeries> *list,
                                            QBar3DSeries *series)
{
    reinterpret_cast<QQuickGraphsBarsNode *>(list->data)->addSeries(series);
}

qsizetype QQuickGraphsBarsNode::countSeriesFunc(QQmlListProperty<QBar3DSeries> *list)
{
    return reinterpret_cast<QQuickGraphsBarsNode *>(list->data)->barSeriesList().size();
}

QBar3DSeries *QQuickGraphsBarsNode::atSeriesFunc(QQmlListProperty<QBar3DSeries> *list,
                                                 qsizetype index)
{
    return reinterpret_cast<QQuickGraphsBarsNode *>(list->data)->barSeriesList().at(index);
}

void QQuickGraphsBarsNode::clearSeriesFunc(QQmlListProperty<QBar3DSeries> *list)
{
    QQuickGraphsBarsNode *declBarsNode = reinterpret_cast<QQuickGraphsBarsNode *>(list->data);
    QList<QBar3DSeries *> realList = declBarsNode->barSeriesList();
    qsizetype count = realList.size();
    for (qsizetype i = 0; i < count; i++)
        declBarsNode->removeSeries(realList.at(i));
}

void QQuickGraphsBarsNode::addSeries(QBar3DSeries *series)
{
    Q_ASSERT(series && series->type() == QAbstract3DSeries::SeriesType::Bar);

    if (graphBars())
        graphBars()->addSeries(series);

    QQuickGraphsNode::addSeriesInternal(series);
}

void QQuickGraphsBarsNode::removeSeries(QBar3DSeries *series)
{
    if (graphBars())
        graphBars()->removeSeries(series);
    QQuickGraphsNode::removeSeriesInternal(series);
}

void QQuickGraphsBarsNode::insertSeries(qsizetype index, QBar3DSeries *series)
{
    if (graphBars())
        graphBars()->insertSeries(index, series);

    QQuickGraphsNode::insertSeries(index, series);
}

void QQuickGraphsBarsNode::clearSelection()
{
    if (graphBars())
        graphBars()->clearSelection();
}

/*!
 * \internal
 */
QQuickGraphsBars *QQuickGraphsBarsNode::graphBars()
{
    return static_cast<QQuickGraphsBars *>(m_graph.get());
}

/*!
 * \internal
 */
const QQuickGraphsBars *QQuickGraphsBarsNode::graphBars() const
{
    return static_cast<QQuickGraphsBars *>(m_graph.get());
}

QT_END_NAMESPACE
