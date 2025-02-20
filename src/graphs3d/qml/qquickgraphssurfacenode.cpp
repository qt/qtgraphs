// Copyright (C) 2025 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
#include "qquickgraphssurfacenode_p.h"

QT_BEGIN_NAMESPACE

QQuickGraphsSurfaceNode::QQuickGraphsSurfaceNode(QQuick3DNode *parent)
    : QQuickGraphsNode(parent)
    , m_flipHorizontalGrid(false)
{}

QQuickGraphsSurfaceNode::~QQuickGraphsSurfaceNode() {}

void QQuickGraphsSurfaceNode::componentComplete()
{
    const QString qmlData = QLatin1StringView(R"QML(
        import QtQuick;
        import QtGraphs;

        Surface3D{}
    )QML");

    QQmlComponent *component = new QQmlComponent(qmlEngine(this), this);
    component->setData(qmlData.toUtf8(), QUrl());
    m_graph.reset(qobject_cast<QQuickGraphsItem *>(component->create()));
    setGraphParent();
    QQuickGraphsNode::componentComplete();

    //initialize components

    if (m_axisX)
        graphSurface()->setAxisX(static_cast<QValue3DAxis *>(m_axisX));
    if (m_axisY)
        graphSurface()->setAxisY(static_cast<QValue3DAxis *>(m_axisY));
    if (m_axisZ)
        graphSurface()->setAxisZ(static_cast<QValue3DAxis *>(m_axisZ));

    graphSurface()->setSelectionMode(m_selectionMode);
    graphSurface()->setFlipHorizontalGrid(m_flipHorizontalGrid);

    for (auto series : m_seriesList)
        graphSurface()->addSeries(static_cast<QSurface3DSeries *>(series));

    //connect signals
    connect(graphSurface(),
            &QQuickGraphsSurface::axisXChanged,
            this,
            &QQuickGraphsSurfaceNode::axisXChanged);
    connect(graphSurface(),
            &QQuickGraphsSurface::axisYChanged,
            this,
            &QQuickGraphsSurfaceNode::axisYChanged);
    connect(graphSurface(),
            &QQuickGraphsSurface::axisZChanged,
            this,
            &QQuickGraphsSurfaceNode::axisZChanged);
    connect(graphSurface(),
            &QQuickGraphsSurface::flipHorizontalGridChanged,
            this,
            &QQuickGraphsSurfaceNode::flipHorizontalGridChanged);
}

QValue3DAxis *QQuickGraphsSurfaceNode::axisX() const
{
    if (graphSurface())
        return graphSurface()->axisX();
    else
        return static_cast<QValue3DAxis *>(m_axisX);
}

void QQuickGraphsSurfaceNode::setAxisX(QValue3DAxis *axis)
{
    if (m_axisX == axis)
        return;
    m_axisX = axis;
    if (graphSurface())
        graphSurface()->setAxisX(axis);
}

QValue3DAxis *QQuickGraphsSurfaceNode::axisY() const
{
    if (graphSurface())
        return graphSurface()->axisY();
    else
        return static_cast<QValue3DAxis *>(m_axisY);
}
void QQuickGraphsSurfaceNode::setAxisY(QValue3DAxis *axis)
{
    if (m_axisY == axis)
        return;
    m_axisY = axis;
    if (graphSurface())
        graphSurface()->setAxisY(axis);
}

QValue3DAxis *QQuickGraphsSurfaceNode::axisZ() const
{
    if (graphSurface())
        return graphSurface()->axisZ();
    else
        return static_cast<QValue3DAxis *>(m_axisZ);
}
void QQuickGraphsSurfaceNode::setAxisZ(QValue3DAxis *axis)
{
    if (m_axisZ == axis)
        return;
    m_axisZ = axis;
    if (graphSurface())
        graphSurface()->setAxisZ(axis);
}

QSurface3DSeries *QQuickGraphsSurfaceNode::selectedSeries() const
{
    if (graphSurface())
        return graphSurface()->selectedSeries();
    else
        return nullptr;
}

void QQuickGraphsSurfaceNode::setSelectionMode(QtGraphs3D::SelectionFlags mode)
{
    if (graphSurface())
        graphSurface()->setSelectionMode(mode);
    else
        setSelectionMode(mode);
}

bool QQuickGraphsSurfaceNode::flipHorizontalGrid() const
{
    if (graphSurface())
        return graphSurface()->flipHorizontalGrid();
    else
        return m_flipHorizontalGrid;
}
void QQuickGraphsSurfaceNode::setFlipHorizontalGrid(bool flip)
{
    if (m_flipHorizontalGrid == flip)
        return;
    m_flipHorizontalGrid = flip;
    if (graphSurface())
        graphSurface()->setFlipHorizontalGrid(flip);
}

QList<QSurface3DSeries *> QQuickGraphsSurfaceNode::surfaceSeriesList()
{
    if (graphSurface()) {
        return graphSurface()->surfaceSeriesList();
    } else {
        QList<QSurface3DSeries *> surfaceSeriesList;
        for (QAbstract3DSeries *abstractSeries : m_seriesList) {
            QSurface3DSeries *surfaceSeries = qobject_cast<QSurface3DSeries *>(abstractSeries);
            if (surfaceSeries)
                surfaceSeriesList.append(surfaceSeries);
        }
        return surfaceSeriesList;
    }
}

QQmlListProperty<QSurface3DSeries> QQuickGraphsSurfaceNode::seriesList()
{
    if (graphSurface()) {
        return graphSurface()->seriesList();
    } else {
        return QQmlListProperty<QSurface3DSeries>(this,
                                                  this,
                                                  &QQuickGraphsSurfaceNode::appendSeriesFunc,
                                                  &QQuickGraphsSurfaceNode::countSeriesFunc,
                                                  &QQuickGraphsSurfaceNode::atSeriesFunc,
                                                  &QQuickGraphsSurfaceNode::clearSeriesFunc);
    }
}

void QQuickGraphsSurfaceNode::appendSeriesFunc(QQmlListProperty<QSurface3DSeries> *list,
                                               QSurface3DSeries *series)
{
    reinterpret_cast<QQuickGraphsSurfaceNode *>(list->data)->addSeries(series);
}

qsizetype QQuickGraphsSurfaceNode::countSeriesFunc(QQmlListProperty<QSurface3DSeries> *list)
{
    return reinterpret_cast<QQuickGraphsSurfaceNode *>(list->data)->surfaceSeriesList().size();
}

QSurface3DSeries *QQuickGraphsSurfaceNode::atSeriesFunc(QQmlListProperty<QSurface3DSeries> *list,
                                                        qsizetype index)
{
    return reinterpret_cast<QQuickGraphsSurfaceNode *>(list->data)->surfaceSeriesList().at(index);
}

void QQuickGraphsSurfaceNode::clearSeriesFunc(QQmlListProperty<QSurface3DSeries> *list)
{
    QQuickGraphsSurfaceNode *declSurfaceNode = reinterpret_cast<QQuickGraphsSurfaceNode *>(
        list->data);
    QList<QSurface3DSeries *> realList = declSurfaceNode->surfaceSeriesList();
    qsizetype count = realList.size();
    for (qsizetype i = 0; i < count; i++)
        declSurfaceNode->removeSeries(realList.at(i));
}

void QQuickGraphsSurfaceNode::addSeries(QSurface3DSeries *series)
{
    Q_ASSERT(series && series->type() == QAbstract3DSeries::SeriesType::Surface);

    if (graphSurface())
        graphSurface()->addSeries(series);

    QQuickGraphsNode::addSeriesInternal(series);
}

void QQuickGraphsSurfaceNode::removeSeries(QSurface3DSeries *series)
{
    if (graphSurface())
        graphSurface()->removeSeries(series);
    QQuickGraphsNode::removeSeriesInternal(series);
}

void QQuickGraphsSurfaceNode::clearSelection()
{
    if (graphSurface())
        graphSurface()->clearSelection();
}

/*!
 * \internal
 */
QQuickGraphsSurface *QQuickGraphsSurfaceNode::graphSurface()
{
    return static_cast<QQuickGraphsSurface *>(m_graph.get());
}

/*!
 * \internal
 */
const QQuickGraphsSurface *QQuickGraphsSurfaceNode::graphSurface() const
{
    return static_cast<QQuickGraphsSurface *>(m_graph.get());
}

QT_END_NAMESPACE
