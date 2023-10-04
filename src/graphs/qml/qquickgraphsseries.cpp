// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtCore/QMetaMethod>
#include "qquickgraphsseries_p.h"

QT_BEGIN_NAMESPACE

static void setSeriesGradient(QAbstract3DSeries *series, QJSValue gradient, GradientType type)
{
    auto newGradient = qobject_cast<QQuickGradient *>(gradient.toQObject());
    QLinearGradient linearGradient;
    linearGradient.setStops(newGradient->gradientStops());

    switch (type) {
    case GradientType::Base:
        series->setBaseGradient(linearGradient);
        break;
    case GradientType::Single:
        series->setSingleHighlightGradient(linearGradient);
        break;
    case GradientType::Multi:
        series->setMultiHighlightGradient(linearGradient);
        break;
    default: // Never goes here
        break;
    }
}

static void connectSeriesGradient(QAbstract3DSeries *series,
                                  QJSValue newGradient,
                                  GradientType type,
                                  QJSValue &memberGradient)
{
    // connect new / disconnect old
    if (newGradient.isQObject() && !newGradient.equals(memberGradient)) {
        auto quickGradient = qobject_cast<QQuickGradient *>(memberGradient.toQObject());
        if (quickGradient)
            QObject::disconnect(quickGradient, 0, series, 0);

        memberGradient = newGradient;
        quickGradient = qobject_cast<QQuickGradient *>(memberGradient.toQObject());

        const int updatedIndex = QMetaMethod::fromSignal(&QQuickGradient::updated).methodIndex();

        int handleIndex = -1;
        switch (type) {
        case GradientType::Base:
            handleIndex = series->metaObject()->indexOfSlot("handleBaseGradientUpdate()");
            break;
        case GradientType::Single:
            handleIndex = series->metaObject()->indexOfSlot(
                "handleSingleHighlightGradientUpdate()");
            break;
        case GradientType::Multi:
            handleIndex = series->metaObject()->indexOfSlot("handleMultiHighlightGradientUpdate()");
            break;
        default: // Never goes here
            break;
        }

        if (quickGradient)
            QMetaObject::connect(quickGradient, updatedIndex, series, handleIndex);
    }

    if (!memberGradient.isNull())
        setSeriesGradient(series, memberGradient, type);
}

QQuickGraphsBar3DSeries::QQuickGraphsBar3DSeries(QObject *parent)
    : QBar3DSeries(parent)
    , m_baseGradient(QJSValue(0))
    , m_singleHighlightGradient(QJSValue(0))
    , m_multiHighlightGradient(QJSValue(0))
    , m_dummyColors(false)
{
    QObject::connect(this,
                     &QBar3DSeries::selectedBarChanged,
                     this,
                     &QQuickGraphsBar3DSeries::selectedBarChanged);
}

QQuickGraphsBar3DSeries::~QQuickGraphsBar3DSeries() {}

QQmlListProperty<QObject> QQuickGraphsBar3DSeries::seriesChildren()
{
    return QQmlListProperty<QObject>(this,
                                     this,
                                     &QQuickGraphsBar3DSeries::appendSeriesChildren,
                                     0,
                                     0,
                                     0);
}

void QQuickGraphsBar3DSeries::appendSeriesChildren(QQmlListProperty<QObject> *list, QObject *element)
{
    QBarDataProxy *proxy = qobject_cast<QBarDataProxy *>(element);
    if (proxy)
        reinterpret_cast<QQuickGraphsBar3DSeries *>(list->data)->setDataProxy(proxy);
}

void QQuickGraphsBar3DSeries::setSelectedBar(const QPointF &position)
{
    QBar3DSeries::setSelectedBar(position.toPoint());
}

QPointF QQuickGraphsBar3DSeries::selectedBar() const
{
    return QPointF(QBar3DSeries::selectedBar());
}

QPointF QQuickGraphsBar3DSeries::invalidSelectionPosition() const
{
    return QPointF(QBar3DSeries::invalidSelectionPosition());
}

void QQuickGraphsBar3DSeries::setBaseGradient(QJSValue gradient)
{
    connectSeriesGradient(this, gradient, GradientType::Base, m_baseGradient);
}

QJSValue QQuickGraphsBar3DSeries::baseGradient() const
{
    return m_baseGradient;
}

void QQuickGraphsBar3DSeries::setSingleHighlightGradient(QJSValue gradient)
{
    connectSeriesGradient(this, gradient, GradientType::Single, m_singleHighlightGradient);
}

QJSValue QQuickGraphsBar3DSeries::singleHighlightGradient() const
{
    return m_singleHighlightGradient;
}

void QQuickGraphsBar3DSeries::setMultiHighlightGradient(QJSValue gradient)
{
    connectSeriesGradient(this, gradient, GradientType::Multi, m_multiHighlightGradient);
}

QJSValue QQuickGraphsBar3DSeries::multiHighlightGradient() const
{
    return m_multiHighlightGradient;
}

QQmlListProperty<DeclarativeColor> QQuickGraphsBar3DSeries::rowColors()
{
    return QQmlListProperty<DeclarativeColor>(this,
                                              this,
                                              &QQuickGraphsBar3DSeries::appendRowColorsFunc,
                                              &QQuickGraphsBar3DSeries::countRowColorsFunc,
                                              &QQuickGraphsBar3DSeries::atRowColorsFunc,
                                              &QQuickGraphsBar3DSeries::clearRowColorsFunc);
}

void QQuickGraphsBar3DSeries::appendRowColorsFunc(QQmlListProperty<DeclarativeColor> *list,
                                                 DeclarativeColor *color)
{
    reinterpret_cast<QQuickGraphsBar3DSeries *>(list->data)->addColor(color);
}

qsizetype QQuickGraphsBar3DSeries::countRowColorsFunc(QQmlListProperty<DeclarativeColor> *list)
{
    return reinterpret_cast<QQuickGraphsBar3DSeries *>(list->data)->colorList().size();
}

DeclarativeColor *QQuickGraphsBar3DSeries::atRowColorsFunc(QQmlListProperty<DeclarativeColor> *list,
                                                          qsizetype index)
{
    return reinterpret_cast<QQuickGraphsBar3DSeries *>(list->data)->colorList().at(index);
}

void QQuickGraphsBar3DSeries::clearRowColorsFunc(QQmlListProperty<DeclarativeColor> *list)
{
    reinterpret_cast<QQuickGraphsBar3DSeries *>(list->data)->clearColors();
}

void QQuickGraphsBar3DSeries::handleBaseGradientUpdate()
{
    if (m_baseGradient.isNull())
        setSeriesGradient(this, m_baseGradient, GradientType::Base);
}

void QQuickGraphsBar3DSeries::handleSingleHighlightGradientUpdate()
{
    if (m_singleHighlightGradient.isNull())
        setSeriesGradient(this, m_singleHighlightGradient, GradientType::Single);
}

void QQuickGraphsBar3DSeries::handleMultiHighlightGradientUpdate()
{
    if (m_multiHighlightGradient.isNull())
        setSeriesGradient(this, m_multiHighlightGradient, GradientType::Multi);
}

void QQuickGraphsBar3DSeries::handleRowColorUpdate()
{
    int colorCount = m_rowColors.size();
    int changed = 0;

    DeclarativeColor *color = qobject_cast<DeclarativeColor *>(QObject::sender());
    for (int i = 0; i < colorCount; i++) {
        if (color == m_rowColors.at(i)) {
            changed = i;
            break;
        }
    }
    QList<QColor> list = QBar3DSeries::rowColors();
    list[changed] = m_rowColors.at(changed)->color();
    QBar3DSeries::setRowColors(list);
}

void QQuickGraphsBar3DSeries::addColor(DeclarativeColor *color)
{
    if (!color) {
        qWarning("Color is invalid, use ThemeColor");
        return;
    }
    clearDummyColors();
    m_rowColors.append(color);
    connect(color,
            &DeclarativeColor::colorChanged,
            this,
            &QQuickGraphsBar3DSeries::handleRowColorUpdate);
    QList<QColor> list = QBar3DSeries::rowColors();
    list.append(color->color());
    QBar3DSeries::setRowColors(list);
}

QList<DeclarativeColor *> QQuickGraphsBar3DSeries::colorList()
{
    if (m_rowColors.isEmpty()) {
        m_dummyColors = true;
        const QList<QColor> list = QBar3DSeries::rowColors();
        for (const QColor &item : list) {
            DeclarativeColor *color = new DeclarativeColor(this);
            color->setColor(item);
            m_rowColors.append(color);
            connect(color,
                    &DeclarativeColor::colorChanged,
                    this,
                    &QQuickGraphsBar3DSeries::handleRowColorUpdate);
        }
    }
    return m_rowColors;
}

void QQuickGraphsBar3DSeries::clearColors()
{
    clearDummyColors();
    for (const auto color : std::as_const(m_rowColors))
        disconnect(color, 0, this, 0);

    m_rowColors.clear();
    QBar3DSeries::setRowColors(QList<QColor>());
}

void QQuickGraphsBar3DSeries::clearDummyColors()
{
    if (m_dummyColors) {
        qDeleteAll(m_rowColors);
        m_rowColors.clear();
        m_dummyColors = false;
    }
}

QQuickGraphsScatter3DSeries::QQuickGraphsScatter3DSeries(QObject *parent)
    : QScatter3DSeries(parent)
    , m_baseGradient(QJSValue(0))
    , m_singleHighlightGradient(QJSValue(0))
    , m_multiHighlightGradient(QJSValue(0))
{}

QQuickGraphsScatter3DSeries::~QQuickGraphsScatter3DSeries() {}

QQmlListProperty<QObject> QQuickGraphsScatter3DSeries::seriesChildren()
{
    return QQmlListProperty<QObject>(this,
                                     this,
                                     &QQuickGraphsScatter3DSeries::appendSeriesChildren,
                                     0,
                                     0,
                                     0);
}

void QQuickGraphsScatter3DSeries::appendSeriesChildren(QQmlListProperty<QObject> *list,
                                                      QObject *element)
{
    QScatterDataProxy *proxy = qobject_cast<QScatterDataProxy *>(element);
    if (proxy)
        reinterpret_cast<QQuickGraphsScatter3DSeries *>(list->data)->setDataProxy(proxy);
}

void QQuickGraphsScatter3DSeries::setBaseGradient(QJSValue gradient)
{
    connectSeriesGradient(this, gradient, GradientType::Base, m_baseGradient);
}

QJSValue QQuickGraphsScatter3DSeries::baseGradient() const
{
    return m_baseGradient;
}

void QQuickGraphsScatter3DSeries::setSingleHighlightGradient(QJSValue gradient)
{
    connectSeriesGradient(this, gradient, GradientType::Single, m_singleHighlightGradient);
}

QJSValue QQuickGraphsScatter3DSeries::singleHighlightGradient() const
{
    return m_singleHighlightGradient;
}

void QQuickGraphsScatter3DSeries::setMultiHighlightGradient(QJSValue gradient)
{
    connectSeriesGradient(this, gradient, GradientType::Multi, m_multiHighlightGradient);
}

QJSValue QQuickGraphsScatter3DSeries::multiHighlightGradient() const
{
    return m_multiHighlightGradient;
}

int QQuickGraphsScatter3DSeries::invalidSelectionIndex() const
{
    return QScatter3DSeries::invalidSelectionIndex();
}

void QQuickGraphsScatter3DSeries::handleBaseGradientUpdate()
{
    if (!m_baseGradient.isNull())
        setSeriesGradient(this, m_baseGradient, GradientType::Base);
}

void QQuickGraphsScatter3DSeries::handleSingleHighlightGradientUpdate()
{
    if (!m_singleHighlightGradient.isNull())
        setSeriesGradient(this, m_singleHighlightGradient, GradientType::Single);
}

void QQuickGraphsScatter3DSeries::handleMultiHighlightGradientUpdate()
{
    if (!m_multiHighlightGradient.isNull())
        setSeriesGradient(this, m_multiHighlightGradient, GradientType::Multi);
}

QQuickGraphsSurface3DSeries::QQuickGraphsSurface3DSeries(QObject *parent)
    : QSurface3DSeries(parent)
    , m_baseGradient(QJSValue(0))
    , m_singleHighlightGradient(QJSValue(0))
    , m_multiHighlightGradient(QJSValue(0))
{
    QObject::connect(this,
                     &QSurface3DSeries::selectedPointChanged,
                     this,
                     &QQuickGraphsSurface3DSeries::selectedPointChanged);
}

QQuickGraphsSurface3DSeries::~QQuickGraphsSurface3DSeries() {}

void QQuickGraphsSurface3DSeries::setSelectedPoint(const QPointF &position)
{
    QSurface3DSeries::setSelectedPoint(position.toPoint());
}

QPointF QQuickGraphsSurface3DSeries::selectedPoint() const
{
    return QPointF(QSurface3DSeries::selectedPoint());
}

QPointF QQuickGraphsSurface3DSeries::invalidSelectionPosition() const
{
    return QPointF(QSurface3DSeries::invalidSelectionPosition());
}

QQmlListProperty<QObject> QQuickGraphsSurface3DSeries::seriesChildren()
{
    return QQmlListProperty<QObject>(this,
                                     this,
                                     &QQuickGraphsSurface3DSeries::appendSeriesChildren,
                                     0,
                                     0,
                                     0);
}

void QQuickGraphsSurface3DSeries::appendSeriesChildren(QQmlListProperty<QObject> *list,
                                                      QObject *element)
{
    QSurfaceDataProxy *proxy = qobject_cast<QSurfaceDataProxy *>(element);
    if (proxy)
        reinterpret_cast<QQuickGraphsSurface3DSeries *>(list->data)->setDataProxy(proxy);
}

void QQuickGraphsSurface3DSeries::setBaseGradient(QJSValue gradient)
{
    connectSeriesGradient(this, gradient, GradientType::Base, m_baseGradient);
}

QJSValue QQuickGraphsSurface3DSeries::baseGradient() const
{
    return m_baseGradient;
}

void QQuickGraphsSurface3DSeries::setSingleHighlightGradient(QJSValue gradient)
{
    connectSeriesGradient(this, gradient, GradientType::Single, m_singleHighlightGradient);
}

QJSValue QQuickGraphsSurface3DSeries::singleHighlightGradient() const
{
    return m_singleHighlightGradient;
}

void QQuickGraphsSurface3DSeries::setMultiHighlightGradient(QJSValue gradient)
{
    connectSeriesGradient(this, gradient, GradientType::Multi, m_multiHighlightGradient);
}

QJSValue QQuickGraphsSurface3DSeries::multiHighlightGradient() const
{
    return m_multiHighlightGradient;
}

void QQuickGraphsSurface3DSeries::handleBaseGradientUpdate()
{
    if (!m_baseGradient.isNull())
        setSeriesGradient(this, m_baseGradient, GradientType::Base);
}

void QQuickGraphsSurface3DSeries::handleSingleHighlightGradientUpdate()
{
    if (!m_singleHighlightGradient.isNull())
        setSeriesGradient(this, m_singleHighlightGradient, GradientType::Single);
}

void QQuickGraphsSurface3DSeries::handleMultiHighlightGradientUpdate()
{
    if (!m_multiHighlightGradient.isNull())
        setSeriesGradient(this, m_multiHighlightGradient, GradientType::Multi);
}

QT_END_NAMESPACE
