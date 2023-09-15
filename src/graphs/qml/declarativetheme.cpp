// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "declarativetheme_p.h"
#include <QtQml/qjsengine.h>

QT_BEGIN_NAMESPACE

DeclarativeTheme3D::DeclarativeTheme3D(QObject *parent)
    : Q3DTheme(parent),
    m_colors(QList<DeclarativeColor *>()),
    m_gradients(QList<QQuickGradient *>()),
    m_singleHLGradient(QJSValue(0)),
    m_multiHLGradient(QJSValue(0)),
    m_dummyColors(false)
{
    connect(this, &Q3DTheme::typeChanged, this, &DeclarativeTheme3D::handleTypeChange);
}

DeclarativeTheme3D::~DeclarativeTheme3D()
{
}

QQmlListProperty<QObject> DeclarativeTheme3D::themeChildren()
{
    return QQmlListProperty<QObject>(this, this, &DeclarativeTheme3D::appendThemeChildren,
                                     0, 0, 0);
}

void DeclarativeTheme3D::appendThemeChildren(QQmlListProperty<QObject> *list, QObject *element)
{
    Q_UNUSED(list);
    Q_UNUSED(element);
    // Nothing to do, themeChildren is there only to enable scoping gradient items in Theme3D item.
}

void DeclarativeTheme3D::handleTypeChange(Theme themeType)
{
    Q_UNUSED(themeType);

    // Theme changed, disconnect base color/gradient connections
    if (!m_colors.isEmpty()) {
        for (DeclarativeColor *item : m_colors)
            disconnect(item, 0, this, 0);
        m_colors.clear();
    }
    if (!m_gradients.isEmpty()) {
        for (auto item : m_gradients)
            disconnect(item, 0, this, 0);
        m_gradients.clear();
    }
}

void DeclarativeTheme3D::handleBaseColorUpdate()
{
    int colorCount = m_colors.size();
    int changed = 0;
    // Check which one changed
    DeclarativeColor *color = qobject_cast<DeclarativeColor *>(QObject::sender());
    for (int i = 0; i < colorCount; i++) {
        if (color == m_colors.at(i)) {
            changed = i;
            break;
        }
    }
    // Update the changed one from the list
    QList<QColor> list = Q3DTheme::baseColors();
    list[changed] = m_colors.at(changed)->color();
    // Set the changed list
    Q3DTheme::setBaseColors(list);
}

void DeclarativeTheme3D::handleBaseGradientUpdate()
{
    // Find out which gradient has changed, and update the list with it
    int gradientCount = m_gradients.size();
    int changed = 0;

    // Check which one changed
    QQuickGradient *newGradient = qobject_cast<QQuickGradient *>(QObject::sender());
    QJSEngine engine;
    QJSValue updatedGradient = engine.newQObject(newGradient);

    for (int i = 0; i < gradientCount; ++i) {
        if (newGradient == m_gradients.at(i)) {
            changed = i;
            break;
        }
    }

    // Update the changed one from the list
    QList<QLinearGradient> list = Q3DTheme::baseGradients();
    list[changed] = convertGradient(updatedGradient);

    // Set the changed list
    Q3DTheme::setBaseGradients(list);
}

void DeclarativeTheme3D::handleSingleHLGradientUpdate()
{
    if (!m_singleHLGradient.isNull())
        setThemeGradient(m_singleHLGradient, GradientType::SingleHL);
}

void DeclarativeTheme3D::handleMultiHLGradientUpdate()
{
    if (!m_multiHLGradient.isNull())
        setThemeGradient(m_multiHLGradient, GradientType::MultiHL);
}

void DeclarativeTheme3D::setSingleHighlightGradient(QJSValue gradient)
{
    // connect new / disconnect old
    if (gradient.isQObject() && !gradient.equals(m_singleHLGradient)) {
        auto quickGradient = qobject_cast<QQuickGradient *>(m_singleHLGradient.toQObject());
        if (quickGradient)
            QObject::disconnect(quickGradient, 0, this, 0);

        m_singleHLGradient = gradient;

        const int signalIndex = QMetaMethod::fromSignal(&QQuickGradient::updated).methodIndex();

        if (quickGradient) {
            QMetaObject::connect(quickGradient, signalIndex, this,
                                 this->metaObject()->indexOfSlot("handleSingleHighlightGradientUpdate()"));
        }

        emit singleHighlightGradientChanged(m_singleHLGradient);
    }

    if (!m_singleHLGradient.isNull())
        setThemeGradient(m_singleHLGradient, GradientType::SingleHL);
}

QJSValue DeclarativeTheme3D::singleHighlightGradient() const
{
    return m_singleHLGradient;
}

void DeclarativeTheme3D::setMultiHighlightGradient(QJSValue gradient)
{
    // connect new / disconnect old
    if (gradient.isQObject() && !gradient.equals(m_multiHLGradient)) {
        auto quickGradient = qobject_cast<QQuickGradient *>(m_multiHLGradient.toQObject());
        if (quickGradient)
            QObject::disconnect(quickGradient, 0, this, 0);

        m_multiHLGradient = gradient;

        const int signalIndex = QMetaMethod::fromSignal(&QQuickGradient::updated).methodIndex();

        if (quickGradient) {
            QMetaObject::connect(quickGradient, signalIndex, this,
                                 this->metaObject()->indexOfSlot("handleMultiHighlightGradientUpdate()"));
        }

        emit multiHighlightGradientChanged(m_multiHLGradient);
    }

    if (!m_multiHLGradient.isNull())
        setThemeGradient(m_multiHLGradient, GradientType::MultiHL);
}

QJSValue DeclarativeTheme3D::multiHighlightGradient() const
{
    return m_multiHLGradient;
}

void DeclarativeTheme3D::classBegin()
{
    // Turn off predefined type forcing for the duration of initial class construction
    // so that predefined type customization can be done.
    d_func()->setForcePredefinedType(false);
}

void DeclarativeTheme3D::componentComplete()
{
    d_func()->setForcePredefinedType(true);
}


void DeclarativeTheme3D::setThemeGradient(QJSValue gradient, GradientType type)
{
    QLinearGradient linearGradient = convertGradient(gradient);

    switch (type) {
    case GradientType::SingleHL:
        Q3DTheme::setSingleHighlightGradient(linearGradient);
        break;
    case GradientType::MultiHL:
        Q3DTheme::setMultiHighlightGradient(linearGradient);
        break;
    default:
        qWarning("Incorrect usage. Type may be GradientType::SingleHL or GradientType::MultiHL.");
        break;
    }
}

QLinearGradient DeclarativeTheme3D::convertGradient(QJSValue gradient)
{
    // Create QLinearGradient out of QJSValue
    QLinearGradient newGradient;
    if (gradient.isQObject()) {
        auto quickGradient = qobject_cast<QQuickGradient *>(gradient.toQObject());
        newGradient.setStops(quickGradient->gradientStops());
    }
    return newGradient;
}

void DeclarativeTheme3D::addColor(DeclarativeColor *color)
{
    if (!color) {
        qWarning("Color is invalid, use ThemeColor");
        return;
    }
    clearDummyColors();
    m_colors.append(color);
    connect(color, &DeclarativeColor::colorChanged,
            this, &DeclarativeTheme3D::handleBaseColorUpdate);
    QList<QColor> list = Q3DTheme::baseColors();
    list.append(color->color());
    Q3DTheme::setBaseColors(list);
}

QList<DeclarativeColor *> DeclarativeTheme3D::colorList()
{
    if (m_colors.isEmpty()) {
        // Create dummy ThemeColors from theme's colors
        m_dummyColors = true;
        QList<QColor> list = Q3DTheme::baseColors();
        for (const QColor &item : list) {
            DeclarativeColor *color = new DeclarativeColor(this);
            color->setColor(item);
            m_colors.append(color);
            connect(color, &DeclarativeColor::colorChanged,
                    this, &DeclarativeTheme3D::handleBaseColorUpdate);
        }
    }
    return m_colors;
}

void DeclarativeTheme3D::clearColors()
{
    clearDummyColors();
    for (DeclarativeColor *item : m_colors)
        disconnect(item, 0, this, 0);
    m_colors.clear();
    Q3DTheme::setBaseColors(QList<QColor>());
}

void DeclarativeTheme3D::clearDummyColors()
{
    if (m_dummyColors) {
        for (DeclarativeColor *item : m_colors)
            delete item;
        m_colors.clear();
        m_dummyColors = false;
    }
}

void DeclarativeTheme3D::addGradient(QJSValue gradient)
{
    auto quickGradient = qobject_cast<QQuickGradient *>(gradient.toQObject());
    m_gradients.append(quickGradient);

    const int updatedIndex = QMetaMethod::fromSignal(&QQuickGradient::updated).methodIndex();

    QMetaObject::connect(quickGradient, updatedIndex,
                         this, this->metaObject()->indexOfSlot("handleBaseGradientUpdate()"));

    QList<QLinearGradient> list = Q3DTheme::baseGradients();
    list.append(convertGradient(gradient));
    Q3DTheme::setBaseGradients(list);
}

QList<QQuickGradient *> DeclarativeTheme3D::gradientList()
{
    return m_gradients;
}

void DeclarativeTheme3D::clearGradients()
{
    m_gradients.clear();
    Q3DTheme::setBaseGradients(QList<QLinearGradient>());
}

QQmlListProperty<DeclarativeColor> DeclarativeTheme3D::baseColors()
{
    return QQmlListProperty<DeclarativeColor>(this, this,
                                              &DeclarativeTheme3D::appendBaseColorsFunc,
                                              &DeclarativeTheme3D::countBaseColorsFunc,
                                              &DeclarativeTheme3D::atBaseColorsFunc,
                                              &DeclarativeTheme3D::clearBaseColorsFunc);
}

void DeclarativeTheme3D::appendBaseColorsFunc(QQmlListProperty<DeclarativeColor> *list,
                                              DeclarativeColor *color)
{
    reinterpret_cast<DeclarativeTheme3D *>(list->data)->addColor(color);
}

qsizetype DeclarativeTheme3D::countBaseColorsFunc(QQmlListProperty<DeclarativeColor> *list)
{
    return reinterpret_cast<DeclarativeTheme3D *>(list->data)->colorList().size();
}

DeclarativeColor *DeclarativeTheme3D::atBaseColorsFunc(QQmlListProperty<DeclarativeColor> *list,
                                                       qsizetype index)
{
    return reinterpret_cast<DeclarativeTheme3D *>(list->data)->colorList().at(index);
}

void DeclarativeTheme3D::clearBaseColorsFunc(QQmlListProperty<DeclarativeColor> *list)
{
    reinterpret_cast<DeclarativeTheme3D *>(list->data)->clearColors();
}

QQmlListProperty<QObject> DeclarativeTheme3D::baseGradients()
{
    return QQmlListProperty<QObject>(this, this,
                                     &DeclarativeTheme3D::appendBaseGradientsFunc,
                                     &DeclarativeTheme3D::countBaseGradientsFunc,
                                     &DeclarativeTheme3D::atBaseGradientsFunc,
                                     &DeclarativeTheme3D::clearBaseGradientsFunc);
}

void DeclarativeTheme3D::appendBaseGradientsFunc(QQmlListProperty<QObject> *list,
                                                 QObject *gradient)
{
    QJSEngine engine;
    QJSValue value = engine.newQObject(gradient);
    reinterpret_cast<DeclarativeTheme3D *>(list->data)->addGradient(value);
}

qsizetype DeclarativeTheme3D::countBaseGradientsFunc(QQmlListProperty<QObject> *list)
{
    return reinterpret_cast<DeclarativeTheme3D *>(list->data)->gradientList().size();
}

QObject *DeclarativeTheme3D::atBaseGradientsFunc(QQmlListProperty<QObject> *list,
                                                 qsizetype index)
{
    return (reinterpret_cast<DeclarativeTheme3D *>(list->data)->gradientList().at(index));
}

void DeclarativeTheme3D::clearBaseGradientsFunc(QQmlListProperty<QObject> *list)
{
    reinterpret_cast<DeclarativeTheme3D *>(list->data)->clearGradients();
}

QT_END_NAMESPACE
