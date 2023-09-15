// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

//
//  W A R N I N G
//  -------------
//
// This file is not part of the QtGraphs API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

#ifndef DECLARATIVETHEME_P_H
#define DECLARATIVETHEME_P_H

#include <private/graphsglobal_p.h>
#include <private/q3dtheme_p.h>

#include "declarativecolor_p.h"

#include <QtQml/qqml.h>
#include <QtQml/qqmlparserstatus.h>
#include <QtQuick/private/qquickrectangle_p.h>

QT_BEGIN_NAMESPACE

class DeclarativeTheme3D : public Q3DTheme, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(QQmlListProperty<QObject> themeChildren READ themeChildren CONSTANT)
    Q_PROPERTY(QQmlListProperty<DeclarativeColor> baseColors READ baseColors CONSTANT)
    Q_PROPERTY(QQmlListProperty<QObject> baseGradients READ baseGradients CONSTANT)
    Q_PROPERTY(QJSValue singleHighlightGradient READ singleHighlightGradient WRITE setSingleHighlightGradient NOTIFY singleHighlightGradientChanged)
    Q_PROPERTY(QJSValue multiHighlightGradient READ multiHighlightGradient WRITE setMultiHighlightGradient NOTIFY multiHighlightGradientChanged)
    Q_CLASSINFO("DefaultProperty", "themeChildren")
    QML_NAMED_ELEMENT(Theme3D)

public:
    DeclarativeTheme3D(QObject *parent = 0);
    virtual ~DeclarativeTheme3D();

    QQmlListProperty<QObject> themeChildren();
    static void appendThemeChildren(QQmlListProperty<QObject> *list, QObject *element);

    QQmlListProperty<DeclarativeColor> baseColors();
    static void appendBaseColorsFunc(QQmlListProperty<DeclarativeColor> *list,
                                     DeclarativeColor *color);
    static qsizetype countBaseColorsFunc(QQmlListProperty<DeclarativeColor> *list);
    static DeclarativeColor *atBaseColorsFunc(QQmlListProperty<DeclarativeColor> *list,
                                              qsizetype index);
    static void clearBaseColorsFunc(QQmlListProperty<DeclarativeColor> *list);

    QQmlListProperty<QObject> baseGradients();
    static void appendBaseGradientsFunc(QQmlListProperty<QObject> *list,
                                        QObject *gradient);
    static qsizetype countBaseGradientsFunc(QQmlListProperty<QObject> *list);
    static QObject *atBaseGradientsFunc(QQmlListProperty<QObject> *list,
                                        qsizetype index);
    static void clearBaseGradientsFunc(QQmlListProperty<QObject> *list);

    void setSingleHighlightGradient(QJSValue gradient);
    QJSValue singleHighlightGradient() const;

    void setMultiHighlightGradient(QJSValue gradient);
    QJSValue multiHighlightGradient() const;

    // From QQmlParserStatus
    void classBegin() override;
    void componentComplete() override;

Q_SIGNALS:
    void singleHighlightGradientChanged(QJSValue gradient);
    void multiHighlightGradientChanged(QJSValue gradient);

public Q_SLOTS:
    void handleTypeChange(Theme themeType);
    void handleBaseColorUpdate();
    void handleBaseGradientUpdate();
    void handleSingleHLGradientUpdate();
    void handleMultiHLGradientUpdate();

protected:
    enum class GradientType {
        Base = 0,
        SingleHL,
        MultiHL
    };

private:
    void addColor(DeclarativeColor *color);
    QList<DeclarativeColor *> colorList();
    void clearColors();
    void clearDummyColors();

    void addGradient(QJSValue gradient);
    QList<QQuickGradient *> gradientList();
    void clearGradients();
    void clearDummyGradients();

    void setThemeGradient(QJSValue gradient, GradientType type);
    QLinearGradient convertGradient(QJSValue gradient);

    QList<DeclarativeColor *> m_colors; // Not owned
    QList<QQuickGradient *> m_gradients; // Not owned
    QJSValue m_singleHLGradient; // Not owned
    QJSValue m_multiHLGradient; // Not owned

    bool m_dummyColors;
};

QT_END_NAMESPACE

#endif
