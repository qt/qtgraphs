// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "declarativeseries_p.h"
#include <QtCore/QMetaMethod>

QT_BEGIN_NAMESPACE

static void setSeriesGradient(QAbstract3DSeries *series, QJSValue gradient,
                              GradientType type)
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

static void connectSeriesGradient(QAbstract3DSeries *series, QJSValue newGradient,
                                  GradientType type, QJSValue &memberGradient)
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
            handleIndex = series->metaObject()->indexOfSlot("handleSingleHighlightGradientUpdate()");
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

DeclarativeBar3DSeries::DeclarativeBar3DSeries(QObject *parent)
    : QBar3DSeries(parent),
    m_baseGradient(QJSValue(0)),
    m_singleHighlightGradient(QJSValue(0)),
    m_multiHighlightGradient(QJSValue(0)),
    m_dummyColors(false)
{
    QObject::connect(this, &QBar3DSeries::selectedBarChanged, this,
                     &DeclarativeBar3DSeries::selectedBarChanged);
}

DeclarativeBar3DSeries::~DeclarativeBar3DSeries()
{
}

QQmlListProperty<QObject> DeclarativeBar3DSeries::seriesChildren()
{
    return QQmlListProperty<QObject>(this, this, &DeclarativeBar3DSeries::appendSeriesChildren
                                     , 0, 0, 0);
}

void DeclarativeBar3DSeries::appendSeriesChildren(QQmlListProperty<QObject> *list, QObject *element)
{
    QBarDataProxy *proxy = qobject_cast<QBarDataProxy *>(element);
    if (proxy)
        reinterpret_cast<DeclarativeBar3DSeries *>(list->data)->setDataProxy(proxy);
}

void DeclarativeBar3DSeries::setSelectedBar(const QPointF &position)
{
    QBar3DSeries::setSelectedBar(position.toPoint());
}

QPointF DeclarativeBar3DSeries::selectedBar() const
{
    return QPointF(QBar3DSeries::selectedBar());
}

QPointF DeclarativeBar3DSeries::invalidSelectionPosition() const
{
    return QPointF(QBar3DSeries::invalidSelectionPosition());
}

void DeclarativeBar3DSeries::setBaseGradient(QJSValue gradient)
{
    connectSeriesGradient(this, gradient, GradientType::Base, m_baseGradient);
}

QJSValue DeclarativeBar3DSeries::baseGradient() const
{
    return m_baseGradient;
}

void DeclarativeBar3DSeries::setSingleHighlightGradient(QJSValue gradient)
{
    connectSeriesGradient(this, gradient, GradientType::Single, m_singleHighlightGradient);
}

QJSValue DeclarativeBar3DSeries::singleHighlightGradient() const
{
    return m_singleHighlightGradient;
}

void DeclarativeBar3DSeries::setMultiHighlightGradient(QJSValue gradient)
{
    connectSeriesGradient(this, gradient, GradientType::Multi, m_multiHighlightGradient);
}

QJSValue DeclarativeBar3DSeries::multiHighlightGradient() const
{
    return m_multiHighlightGradient;
}

QQmlListProperty<DeclarativeColor> DeclarativeBar3DSeries::rowColors()
{
    return QQmlListProperty<DeclarativeColor>(this, this,
                                              &DeclarativeBar3DSeries::appendRowColorsFunc,
                                              &DeclarativeBar3DSeries::countRowColorsFunc,
                                              &DeclarativeBar3DSeries::atRowColorsFunc,
                                              &DeclarativeBar3DSeries::clearRowColorsFunc);
}

void DeclarativeBar3DSeries::appendRowColorsFunc(QQmlListProperty<DeclarativeColor> *list,
                                                 DeclarativeColor *color)
{
    reinterpret_cast<DeclarativeBar3DSeries *>(list->data)->addColor(color);
}

qsizetype DeclarativeBar3DSeries::countRowColorsFunc(QQmlListProperty<DeclarativeColor> *list)
{
    return reinterpret_cast<DeclarativeBar3DSeries *>(list->data)->colorList().size();
}

DeclarativeColor *DeclarativeBar3DSeries::atRowColorsFunc(QQmlListProperty<DeclarativeColor> *list,
                                                          qsizetype index)
{
    return reinterpret_cast<DeclarativeBar3DSeries *>(list->data)->colorList().at(index);
}

void DeclarativeBar3DSeries::clearRowColorsFunc(QQmlListProperty<DeclarativeColor> *list)
{
    reinterpret_cast<DeclarativeBar3DSeries *>(list->data)->clearColors();
}

void DeclarativeBar3DSeries::handleBaseGradientUpdate()
{
    if (m_baseGradient.isNull())
        setSeriesGradient(this, m_baseGradient, GradientType::Base);
}

void DeclarativeBar3DSeries::handleSingleHighlightGradientUpdate()
{
    if (m_singleHighlightGradient.isNull())
        setSeriesGradient(this, m_singleHighlightGradient, GradientType::Single);
}

void DeclarativeBar3DSeries::handleMultiHighlightGradientUpdate()
{
    if (m_multiHighlightGradient.isNull())
        setSeriesGradient(this, m_multiHighlightGradient, GradientType::Multi);
}

void DeclarativeBar3DSeries::handleRowColorUpdate()
{
    int colorCount = m_rowColors.size();
    int changed = 0;

    DeclarativeColor *color = qobject_cast<DeclarativeColor*>(QObject::sender());
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

void DeclarativeBar3DSeries::addColor(DeclarativeColor *color)
{
    if (!color) {
        qWarning("Color is invalid, use ThemeColor");
        return;
    }
    clearDummyColors();
    m_rowColors.append(color);
    connect(color, &DeclarativeColor::colorChanged, this,
            &DeclarativeBar3DSeries::handleRowColorUpdate);
    QList<QColor> list = QBar3DSeries::rowColors();
    list.append(color->color());
    QBar3DSeries::setRowColors(list);
}

QList<DeclarativeColor *> DeclarativeBar3DSeries::colorList()
{
    if (m_rowColors.isEmpty()) {
        m_dummyColors = true;
        const QList<QColor> list = QBar3DSeries::rowColors();
        for (const QColor &item : list) {
            DeclarativeColor *color = new DeclarativeColor(this);
            color->setColor(item);
            m_rowColors.append(color);
            connect(color, &DeclarativeColor::colorChanged, this,
                    &DeclarativeBar3DSeries::handleRowColorUpdate);
        }
    }
    return m_rowColors;
}

void DeclarativeBar3DSeries::clearColors()
{
    clearDummyColors();
    for (const auto color : std::as_const(m_rowColors))
        disconnect(color, 0, this, 0);

    m_rowColors.clear();
    QBar3DSeries::setRowColors(QList<QColor>());
}

void DeclarativeBar3DSeries::clearDummyColors()
{
    if (m_dummyColors) {
        qDeleteAll(m_rowColors);
        m_rowColors.clear();
        m_dummyColors = false;
    }
}

DeclarativeScatter3DSeries::DeclarativeScatter3DSeries(QObject *parent)
    : QScatter3DSeries(parent),
    m_baseGradient(QJSValue(0)),
    m_singleHighlightGradient(QJSValue(0)),
    m_multiHighlightGradient(QJSValue(0))
{
}

DeclarativeScatter3DSeries::~DeclarativeScatter3DSeries()
{
}

QQmlListProperty<QObject> DeclarativeScatter3DSeries::seriesChildren()
{
    return QQmlListProperty<QObject>(this, this, &DeclarativeScatter3DSeries::appendSeriesChildren
                                     , 0, 0, 0);
}

void DeclarativeScatter3DSeries::appendSeriesChildren(QQmlListProperty<QObject> *list,
                                                      QObject *element)
{
    QScatterDataProxy *proxy = qobject_cast<QScatterDataProxy *>(element);
    if (proxy)
        reinterpret_cast<DeclarativeScatter3DSeries *>(list->data)->setDataProxy(proxy);
}

void DeclarativeScatter3DSeries::setBaseGradient(QJSValue gradient)
{
    connectSeriesGradient(this, gradient, GradientType::Base, m_baseGradient);
}

QJSValue DeclarativeScatter3DSeries::baseGradient() const
{
    return m_baseGradient;
}

void DeclarativeScatter3DSeries::setSingleHighlightGradient(QJSValue gradient)
{
    connectSeriesGradient(this, gradient, GradientType::Single, m_singleHighlightGradient);
}

QJSValue DeclarativeScatter3DSeries::singleHighlightGradient() const
{
    return m_singleHighlightGradient;
}

void DeclarativeScatter3DSeries::setMultiHighlightGradient(QJSValue gradient)
{
    connectSeriesGradient(this, gradient, GradientType::Multi, m_multiHighlightGradient);
}

QJSValue DeclarativeScatter3DSeries::multiHighlightGradient() const
{
    return m_multiHighlightGradient;
}

int DeclarativeScatter3DSeries::invalidSelectionIndex() const
{
    return QScatter3DSeries::invalidSelectionIndex();
}

void DeclarativeScatter3DSeries::handleBaseGradientUpdate()
{
    if (!m_baseGradient.isNull())
        setSeriesGradient(this, m_baseGradient, GradientType::Base);
}

void DeclarativeScatter3DSeries::handleSingleHighlightGradientUpdate()
{
    if (!m_singleHighlightGradient.isNull())
        setSeriesGradient(this, m_singleHighlightGradient, GradientType::Single);
}

void DeclarativeScatter3DSeries::handleMultiHighlightGradientUpdate()
{
    if (!m_multiHighlightGradient.isNull())
        setSeriesGradient(this, m_multiHighlightGradient, GradientType::Multi);
}

DeclarativeSurface3DSeries::DeclarativeSurface3DSeries(QObject *parent)
    : QSurface3DSeries(parent),
    m_baseGradient(QJSValue(0)),
    m_singleHighlightGradient(QJSValue(0)),
    m_multiHighlightGradient(QJSValue(0))
{
    QObject::connect(this, &QSurface3DSeries::selectedPointChanged, this,
                     &DeclarativeSurface3DSeries::selectedPointChanged);
}

DeclarativeSurface3DSeries::~DeclarativeSurface3DSeries()
{
}

void DeclarativeSurface3DSeries::setSelectedPoint(const QPointF &position)
{
    QSurface3DSeries::setSelectedPoint(position.toPoint());
}

QPointF DeclarativeSurface3DSeries::selectedPoint() const
{
    return QPointF(QSurface3DSeries::selectedPoint());
}

QPointF DeclarativeSurface3DSeries::invalidSelectionPosition() const
{
    return QPointF(QSurface3DSeries::invalidSelectionPosition());
}

QQmlListProperty<QObject> DeclarativeSurface3DSeries::seriesChildren()
{
    return QQmlListProperty<QObject>(this, this, &DeclarativeSurface3DSeries::appendSeriesChildren
                                     , 0, 0, 0);
}

void DeclarativeSurface3DSeries::appendSeriesChildren(QQmlListProperty<QObject> *list,
                                                      QObject *element)
{
    QSurfaceDataProxy *proxy = qobject_cast<QSurfaceDataProxy *>(element);
    if (proxy)
        reinterpret_cast<DeclarativeSurface3DSeries *>(list->data)->setDataProxy(proxy);
}

void DeclarativeSurface3DSeries::setBaseGradient(QJSValue gradient)
{
    connectSeriesGradient(this, gradient, GradientType::Base, m_baseGradient);
}

QJSValue DeclarativeSurface3DSeries::baseGradient() const
{
    return m_baseGradient;
}

void DeclarativeSurface3DSeries::setSingleHighlightGradient(QJSValue gradient)
{
    connectSeriesGradient(this, gradient, GradientType::Single, m_singleHighlightGradient);
}

QJSValue DeclarativeSurface3DSeries::singleHighlightGradient() const
{
    return m_singleHighlightGradient;
}

void DeclarativeSurface3DSeries::setMultiHighlightGradient(QJSValue gradient)
{
    connectSeriesGradient(this, gradient, GradientType::Multi, m_multiHighlightGradient);
}

QJSValue DeclarativeSurface3DSeries::multiHighlightGradient() const
{
    return m_multiHighlightGradient;
}

void DeclarativeSurface3DSeries::handleBaseGradientUpdate()
{
    if (!m_baseGradient.isNull())
        setSeriesGradient(this, m_baseGradient, GradientType::Base);
}

void DeclarativeSurface3DSeries::handleSingleHighlightGradientUpdate()
{
    if (!m_singleHighlightGradient.isNull())
        setSeriesGradient(this, m_singleHighlightGradient, GradientType::Single);
}

void DeclarativeSurface3DSeries::handleMultiHighlightGradientUpdate()
{
    if (!m_multiHighlightGradient.isNull())
        setSeriesGradient(this, m_multiHighlightGradient, GradientType::Multi);
}

QT_END_NAMESPACE
