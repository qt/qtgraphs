// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef BARCOMPONENT_H
#define BARCOMPONENT_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the QtGraphs API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

#include <QtGraphs/qgraphsglobal.h>
#include <QQuickItem>
#include <QColor>
#include <QString>

QT_BEGIN_NAMESPACE

class Q_GRAPHS_EXPORT BarComponent : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(qreal barValue READ barValue NOTIFY barValueChanged)
    Q_PROPERTY(QColor barColor READ barColor NOTIFY barColorChanged)
    Q_PROPERTY(QColor barBorderColor READ barBorderColor NOTIFY barBorderColorChanged)
    Q_PROPERTY(qreal barBorderWidth READ barBorderWidth NOTIFY barBorderWidthChanged)
    Q_PROPERTY(QString barLabel READ barLabel NOTIFY barLabelChanged)
    Q_PROPERTY(bool barSelected READ barSelected NOTIFY barSelectedChanged)
    QML_ELEMENT

public:
    BarComponent(QQuickItem *parent = nullptr);

    qreal barValue() const;
    QColor barColor() const;
    QColor barBorderColor() const;
    qreal barBorderWidth() const;
    QString barLabel() const;
    bool barSelected() const;

Q_SIGNALS:
    void barValueChanged();
    void barColorChanged();
    void barBorderColorChanged();
    void barBorderWidthChanged();
    void barLabelChanged();
    void barSelectedChanged();

private:
    friend class BarsRenderer;
    void setBarValue(qreal value);
    void setBarColor(const QColor &color);
    void setBarBorderColor(const QColor &color);
    void setBarBorderWidth(qreal width);
    void setBarLabel(const QString &label);
    void setBarSelected(bool selected);

private:
    qreal m_barValue = 0;
    QColor m_barColor;
    QColor m_barBorderColor;
    qreal m_barBorderWidth = 0;
    QString m_barLabel;
    bool m_barSelected = false;
};

QT_END_NAMESPACE

#endif // BARCOMPONENT_H
