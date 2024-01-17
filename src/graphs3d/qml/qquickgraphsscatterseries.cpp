// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtCore/QMetaMethod>
#include "qquickgraphsscatterseries_p.h"

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

QT_END_NAMESPACE
