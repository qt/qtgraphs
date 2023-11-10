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

#ifndef QQUICKGRAPHSSERIES_P_H
#define QQUICKGRAPHSSERIES_P_H

#include "qbar3dseries.h"
#include "qquickgraphscolor_p.h"
#include "qscatter3dseries.h"
#include "qsurface3dseries.h"

#include <QtQml/qqml.h>
#include <QtQuick/private/qquickrectangle_p.h>
#include <private/graphsglobal_p.h>

QT_BEGIN_NAMESPACE

enum class GradientType { Base, Single, Multi };

class QQuickGraphsBar3DSeries : public QBar3DSeries
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<QObject> seriesChildren READ seriesChildren CONSTANT)
    // This property is overloaded to use QPointF instead of QPoint to work around
    // qml bug where Qt.point(0, 0) can't be assigned due to error "Cannot assign
    // QPointF to QPoint".
    Q_PROPERTY(QPointF selectedBar READ selectedBar WRITE setSelectedBar NOTIFY selectedBarChanged)
    // This is static method in parent class, overload as constant property for qml.
    Q_PROPERTY(QPointF invalidSelectionPosition READ invalidSelectionPosition CONSTANT)
    Q_PROPERTY(
        QJSValue baseGradient READ baseGradient WRITE setBaseGradient NOTIFY baseGradientChanged)
    Q_PROPERTY(QJSValue singleHighlightGradient READ singleHighlightGradient WRITE
                   setSingleHighlightGradient NOTIFY singleHighlightGradientChanged)
    Q_PROPERTY(QJSValue multiHighlightGradient READ multiHighlightGradient WRITE
                   setMultiHighlightGradient NOTIFY multiHighlightGradientChanged)
    Q_PROPERTY(QQmlListProperty<QQuickGraphsColor> rowColors READ rowColors CONSTANT)
    Q_CLASSINFO("DefaultProperty", "seriesChildren")

    QML_NAMED_ELEMENT(Bar3DSeries)

public:
    QQuickGraphsBar3DSeries(QObject *parent = 0);
    ~QQuickGraphsBar3DSeries() override;

    QQmlListProperty<QObject> seriesChildren();
    static void appendSeriesChildren(QQmlListProperty<QObject> *list, QObject *element);

    void setSelectedBar(const QPointF &position);
    QPointF selectedBar() const;
    QPointF invalidSelectionPosition() const;

    void setBaseGradient(QJSValue gradient);
    QJSValue baseGradient() const;
    void setSingleHighlightGradient(QJSValue gradient);
    QJSValue singleHighlightGradient() const;
    void setMultiHighlightGradient(QJSValue gradient);
    QJSValue multiHighlightGradient() const;

    QQmlListProperty<QQuickGraphsColor> rowColors();
    static void appendRowColorsFunc(QQmlListProperty<QQuickGraphsColor> *list,
                                    QQuickGraphsColor *color);
    static qsizetype countRowColorsFunc(QQmlListProperty<QQuickGraphsColor> *list);
    static QQuickGraphsColor *atRowColorsFunc(QQmlListProperty<QQuickGraphsColor> *list,
                                              qsizetype index);
    static void clearRowColorsFunc(QQmlListProperty<QQuickGraphsColor> *list);

public Q_SLOTS:
    void handleBaseGradientUpdate();
    void handleSingleHighlightGradientUpdate();
    void handleMultiHighlightGradientUpdate();
    void handleRowColorUpdate();

Q_SIGNALS:
    void selectedBarChanged(const QPointF &position);
    void baseGradientChanged(QJSValue gradient);
    void singleHighlightGradientChanged(QJSValue gradient);
    void multiHighlightGradientChanged(QJSValue gradient);

private:
    QJSValue m_baseGradient;            // Not owned
    QJSValue m_singleHighlightGradient; // Not owned
    QJSValue m_multiHighlightGradient;  // Not owned

    QList<QQuickGraphsColor *> m_rowColors;
    bool m_dummyColors;

    void addColor(QQuickGraphsColor *color);
    QList<QQuickGraphsColor *> colorList();
    void clearColors();
    void clearDummyColors();
};

class QQuickGraphsScatter3DSeries : public QScatter3DSeries
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<QObject> seriesChildren READ seriesChildren CONSTANT)
    Q_PROPERTY(
        QJSValue baseGradient READ baseGradient WRITE setBaseGradient NOTIFY baseGradientChanged)
    Q_PROPERTY(QJSValue singleHighlightGradient READ singleHighlightGradient WRITE
                   setSingleHighlightGradient NOTIFY singleHighlightGradientChanged)
    Q_PROPERTY(QJSValue multiHighlightGradient READ multiHighlightGradient WRITE
                   setMultiHighlightGradient NOTIFY multiHighlightGradientChanged)
    // This is static method in parent class, overload as constant property for qml.
    Q_PROPERTY(int invalidSelectionIndex READ invalidSelectionIndex CONSTANT)
    Q_CLASSINFO("DefaultProperty", "seriesChildren")

    QML_NAMED_ELEMENT(Scatter3DSeries)

public:
    QQuickGraphsScatter3DSeries(QObject *parent = 0);
    ~QQuickGraphsScatter3DSeries() override;

    QQmlListProperty<QObject> seriesChildren();
    static void appendSeriesChildren(QQmlListProperty<QObject> *list, QObject *element);

    void setBaseGradient(QJSValue gradient);
    QJSValue baseGradient() const;
    void setSingleHighlightGradient(QJSValue gradient);
    QJSValue singleHighlightGradient() const;
    void setMultiHighlightGradient(QJSValue gradient);
    QJSValue multiHighlightGradient() const;

    int invalidSelectionIndex() const;

public Q_SLOTS:
    void handleBaseGradientUpdate();
    void handleSingleHighlightGradientUpdate();
    void handleMultiHighlightGradientUpdate();

Q_SIGNALS:
    void baseGradientChanged(QJSValue gradient);
    void singleHighlightGradientChanged(QJSValue gradient);
    void multiHighlightGradientChanged(QJSValue gradient);

private:
    QJSValue m_baseGradient;            // Not owned
    QJSValue m_singleHighlightGradient; // Not owned
    QJSValue m_multiHighlightGradient;  // Not owned
};

class QQuickGraphsSurface3DSeries : public QSurface3DSeries
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<QObject> seriesChildren READ seriesChildren CONSTANT)
    // This property is overloaded to use QPointF instead of QPoint to work
    // around qml bug where Qt.point(0, 0) can't be assigned due to error
    // "Cannot assign QPointF to QPoint".
    Q_PROPERTY(
        QPointF selectedPoint READ selectedPoint WRITE setSelectedPoint NOTIFY selectedPointChanged)
    // This is static method in parent class, overload as constant property for
    // qml.
    Q_PROPERTY(QPointF invalidSelectionPosition READ invalidSelectionPosition CONSTANT)
    Q_PROPERTY(
        QJSValue baseGradient READ baseGradient WRITE setBaseGradient NOTIFY baseGradientChanged)
    Q_PROPERTY(QJSValue singleHighlightGradient READ singleHighlightGradient WRITE
                   setSingleHighlightGradient NOTIFY singleHighlightGradientChanged)
    Q_PROPERTY(QJSValue multiHighlightGradient READ multiHighlightGradient WRITE
                   setMultiHighlightGradient NOTIFY multiHighlightGradientChanged)
    Q_CLASSINFO("DefaultProperty", "seriesChildren")

    QML_NAMED_ELEMENT(Surface3DSeries)

public:
    QQuickGraphsSurface3DSeries(QObject *parent = 0);
    ~QQuickGraphsSurface3DSeries() override;

    void setSelectedPoint(const QPointF &position);
    QPointF selectedPoint() const;
    QPointF invalidSelectionPosition() const;

    QQmlListProperty<QObject> seriesChildren();
    static void appendSeriesChildren(QQmlListProperty<QObject> *list, QObject *element);

    void setBaseGradient(QJSValue gradient);
    QJSValue baseGradient() const;
    void setSingleHighlightGradient(QJSValue gradient);
    QJSValue singleHighlightGradient() const;
    void setMultiHighlightGradient(QJSValue gradient);
    QJSValue multiHighlightGradient() const;

public Q_SLOTS:
    void handleBaseGradientUpdate();
    void handleSingleHighlightGradientUpdate();
    void handleMultiHighlightGradientUpdate();

Q_SIGNALS:
    void selectedPointChanged(const QPointF &position);
    void baseGradientChanged(QJSValue gradient);
    void singleHighlightGradientChanged(QJSValue gradient);
    void multiHighlightGradientChanged(QJSValue gradient);

private:
    QJSValue m_baseGradient;            // Not owned
    QJSValue m_singleHighlightGradient; // Not owned
    QJSValue m_multiHighlightGradient;  // Not owned
};

QT_END_NAMESPACE

#endif
