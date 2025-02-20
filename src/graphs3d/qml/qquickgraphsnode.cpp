// Copyright (C) 2025 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "qcustom3ditem.h"
#include "qquickgraphsnode_p.h"
#include <qtconfigmacros.h>

QT_BEGIN_NAMESPACE

QQuickGraphsNode::QQuickGraphsNode(QQuick3DNode *parent)
    : QQuick3DNode(parent)
    , m_selectionMode(QtGraphs3D::SelectionFlag::Item)
    , m_aspectRatio(2.0)
    , m_optimizationHint(QtGraphs3D::OptimizationHint::Default)
    , m_polar(false)
    , m_labelMargin(.1f)
    , m_radialLabelOffset(1.0f)
    , m_horizontalAspectRatio(0.0)
    , m_locale(QLocale::c())
    , m_margin(-1.0)
    , m_gridLineType(QtGraphs3D::GridLineType::Geometry)
{
    QGraphsTheme *theme = new QGraphsTheme(this);
    setTheme(theme);
}

QQuickGraphsNode::~QQuickGraphsNode() {}

void QQuickGraphsNode::componentComplete()
{
    QQuick3DNode::componentComplete();

    //initialize properties
    m_graph->setTheme(m_theme);
    m_graph->setAspectRatio(m_aspectRatio);
    m_graph->setOptimizationHint(m_optimizationHint);
    m_graph->setPolar(m_polar);
    m_graph->setLabelMargin(m_labelMargin);
    m_graph->setRadialLabelOffset(m_radialLabelOffset);
    m_graph->setHorizontalAspectRatio(m_horizontalAspectRatio);
    m_graph->setLocale(m_locale);
    m_graph->setMargin(m_margin);
    m_graph->setGridLineType(m_gridLineType);

    for (auto item : m_customItemList)
        m_graph->addCustomItem(item);

    //connect signals
    connect(m_graph.get(),
            &QQuickGraphsItem::selectionModeChanged,
            this,
            &QQuickGraphsNode::selectionModeChanged);
    connect(m_graph.get(), &QQuickGraphsItem::themeChanged, this, &QQuickGraphsNode::themeChanged);
    connect(m_graph.get(),
            &QQuickGraphsItem::aspectRatioChanged,
            this,
            &QQuickGraphsNode::aspectRatioChanged);
    connect(m_graph.get(),
            &QQuickGraphsItem::optimizationHintChanged,
            this,
            &QQuickGraphsNode::optimizationHintChanged);
    connect(m_graph.get(), &QQuickGraphsItem::polarChanged, this, &QQuickGraphsNode::polarChanged);
    connect(m_graph.get(),
            &QQuickGraphsItem::labelMarginChanged,
            this,
            &QQuickGraphsNode::labelMarginChanged);
    connect(m_graph.get(),
            &QQuickGraphsItem::radialLabelOffsetChanged,
            this,
            &QQuickGraphsNode::radialLabelOffsetChanged);
    connect(m_graph.get(),
            &QQuickGraphsItem::horizontalAspectRatioChanged,
            this,
            &QQuickGraphsNode::horizontalAspectRatioChanged);
    connect(m_graph.get(), &QQuickGraphsItem::localeChanged, this, &QQuickGraphsNode::localeChanged);
    connect(m_graph.get(),
            &QQuickGraphsItem::queriedGraphPositionChanged,
            this,
            &QQuickGraphsNode::queriedGraphPositionChanged);
    connect(m_graph.get(), &QQuickGraphsItem::marginChanged, this, &QQuickGraphsNode::marginChanged);
    connect(m_graph.get(),
            &QQuickGraphsItem::gridLineTypeChanged,
            this,
            &QQuickGraphsNode::gridLineTypeChanged);
}

void QQuickGraphsNode::setGraphParent()
{
    if (m_graph)
        m_graph->setParentNode(this);
}

void QQuickGraphsNode::setSelectionMode(QtGraphs3D::SelectionFlags selectionMode)
{
    if (m_selectionMode == selectionMode)
        return;

    m_selectionMode = selectionMode;
    if (m_graph)
        m_graph->setSelectionMode(m_selectionMode);
}

QtGraphs3D::SelectionFlags QQuickGraphsNode::selectionMode() const
{
    if (m_graph)
        return m_graph->selectionMode();
    else
        return m_selectionMode;
}

void QQuickGraphsNode::setTheme(QGraphsTheme *theme)
{
    if (m_theme == theme)
        return;

    m_theme = theme;
    if (m_graph)
        m_graph->setTheme(m_theme);
}

QGraphsTheme *QQuickGraphsNode::theme() const
{
    if (m_graph)
        return m_graph->theme();
    else
        return m_theme;
}

QQmlListProperty<QCustom3DItem> QQuickGraphsNode::customItemList() const
{
    if (m_graph)
        return m_graph->customItemList();
    else
        return QQmlListProperty<QCustom3DItem>();
}


QtGraphs3D::ElementType QQuickGraphsNode::selectedElement() const
{
    if (m_graph)
        return m_graph->selectedElement();
    else
        return QtGraphs3D::ElementType::None;
}

void QQuickGraphsNode::setAspectRatio(qreal aspectRatio)
{
    if (m_aspectRatio == aspectRatio)
        return;

    m_aspectRatio = aspectRatio;
    if (m_graph)
        m_graph->setAspectRatio(aspectRatio);
}

qreal QQuickGraphsNode::aspectRatio() const
{
    if (m_graph)
        return m_graph->aspectRatio();
    else
        return m_aspectRatio;
}

void QQuickGraphsNode::setOptimizationHint(QtGraphs3D::OptimizationHint optimizationHint)
{
    if (m_optimizationHint == optimizationHint)
        return;

    m_optimizationHint = optimizationHint;
    if (m_graph)
        m_graph->setOptimizationHint(optimizationHint);
}
QtGraphs3D::OptimizationHint QQuickGraphsNode::optimizationHint() const
{
    if (m_graph)
        return m_graph->optimizationHint();
    else
        return m_optimizationHint;
}
void QQuickGraphsNode::setPolar(bool enabled)
{
    if (m_polar == enabled)
        return;

    m_polar = enabled;
    if (m_graph)
        m_graph->setPolar(enabled);
}

bool QQuickGraphsNode::isPolar() const
{
    if (m_graph)
        return m_graph->isPolar();
    else
        return m_polar;
}

void QQuickGraphsNode::setLabelMargin(float labelMargin)
{
    if (m_labelMargin == labelMargin)
        return;

    m_labelMargin = labelMargin;
    if (m_graph)
        m_graph->setLabelMargin(labelMargin);
}

float QQuickGraphsNode::labelMargin() const
{
    if (m_graph)
        return m_graph->labelMargin();
    else
        return m_labelMargin;
}

void QQuickGraphsNode::setRadialLabelOffset(float radialLabelOffset)
{
    if (m_radialLabelOffset == radialLabelOffset)
        return;

    m_radialLabelOffset = radialLabelOffset;
    if (m_graph)
        m_graph->setRadialLabelOffset(radialLabelOffset);
}

float QQuickGraphsNode::radialLabelOffset() const
{
    if (m_graph)
        return m_graph->radialLabelOffset();
    else
        return m_radialLabelOffset;
}

void QQuickGraphsNode::setHorizontalAspectRatio(qreal horizontalAspectRatio)
{
    if (m_horizontalAspectRatio == horizontalAspectRatio)
        return;

    m_horizontalAspectRatio = horizontalAspectRatio;
    if (m_graph)
        m_graph->setHorizontalAspectRatio(horizontalAspectRatio);
}

qreal QQuickGraphsNode::horizontalAspectRatio() const
{
    if (m_graph)
        return m_graph->horizontalAspectRatio();
    else
        return m_horizontalAspectRatio;
}

void QQuickGraphsNode::setLocale(QLocale locale)
{
    if (m_locale == locale)
        return;

    m_locale = locale;
    if (m_graph)
        m_graph->setLocale(locale);
}

QLocale QQuickGraphsNode::locale() const
{
    if (m_graph)
        return m_graph->locale();
    else
        return m_locale;
}

QVector3D QQuickGraphsNode::queriedGraphPosition() const
{
    if (m_graph)
        return m_graph->queriedGraphPosition();
    else
        return QVector3D();
}

qreal QQuickGraphsNode::margin() const
{
    if (m_graph)
        return m_graph->margin();
    else
        return m_margin;
}

void QQuickGraphsNode::setMargin(qreal margin)
{
    if (m_margin == margin)
        return;

    m_margin = margin;
    if (m_graph)
        m_graph->setMargin(margin);
}

QtGraphs3D::GridLineType QQuickGraphsNode::gridLineType() const
{
    if (m_graph)
        return m_graph->gridLineType();
    else
        return m_gridLineType;
}
void QQuickGraphsNode::setGridLineType(const QtGraphs3D::GridLineType &gridLineType)
{
    if (m_gridLineType == gridLineType)
        return;

    m_gridLineType = gridLineType;
    if (m_graph)
        m_graph->setGridLineType(gridLineType);
}

bool QQuickGraphsNode::hasSeries(QAbstract3DSeries *series)
{
    if (m_graph)
        return m_graph->hasSeries(series);
    else
        return m_seriesList.contains(series);
}
void QQuickGraphsNode::addSeriesInternal(QAbstract3DSeries *series)
{
    insertSeries(m_seriesList.size(), series);
}

void QQuickGraphsNode::insertSeries(qsizetype index, QAbstract3DSeries *series)
{
    if (series) {
        if (m_seriesList.contains(series)) {
            qsizetype oldIndex = m_seriesList.indexOf(series);
            if (index != oldIndex) {
                m_seriesList.removeOne(series);
                if (oldIndex < index)
                    index--;
                m_seriesList.insert(index, series);
            }
        } else {
            m_seriesList.insert(index, series);
        }
    }
}

void QQuickGraphsNode::removeSeriesInternal(QAbstract3DSeries *series)
{
    if (series)
        m_seriesList.removeAll(series);
}

QList<QAbstract3DSeries *> QQuickGraphsNode::seriesList()
{
    return m_seriesList;
}

qsizetype QQuickGraphsNode::addCustomItem(QCustom3DItem *item)
{
    if (m_graph)
        m_graph->addCustomItem(item);

    m_customItemList.append(item);
    return m_customItemList.size() - 1;
}

void QQuickGraphsNode::removeCustomItems()
{
    if (m_graph)
        m_graph->removeCustomItems();

    m_customItemList.clear();
}

void QQuickGraphsNode::removeCustomItem(QCustom3DItem *item)
{
    if (m_graph)
        m_graph->deleteCustomItem(item);

    m_customItemList.removeOne(item);
}

void QQuickGraphsNode::removeCustomItemAt(QVector3D position)
{
    if (m_graph)
        m_graph->removeCustomItemAt(position);

    for (QCustom3DItem *item : m_customItemList) {
        if (item->position() == position)
            m_customItemList.removeOne(item);
    }
}

void QQuickGraphsNode::releaseCustomItem(QCustom3DItem *item)
{
    if (m_graph)
        m_graph->releaseCustomItem(item);

    m_customItemList.removeOne(item);
}

int QQuickGraphsNode::selectedLabelIndex() const
{
    if (m_graph)
        return selectedLabelIndex();
    else
        return -1;
}

QAbstract3DAxis *QQuickGraphsNode::selectedAxis() const
{
    if (m_graph)
        return m_graph->selectedAxis();
    return nullptr;
}

qsizetype QQuickGraphsNode::selectedCustomItemIndex() const
{
    if (m_graph)
        return m_graph->selectedCustomItemIndex();
    else
        return -1;
}

QCustom3DItem *QQuickGraphsNode::selectedCustomItem() const
{
    if (m_graph)
        return m_graph->selectedCustomItem();
    else
        return nullptr;
}

QT_END_NAMESPACE
