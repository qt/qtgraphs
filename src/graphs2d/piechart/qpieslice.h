// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QPIESLICE_H
#define QPIESLICE_H

#include <QtQuick/qquickitem.h>
#include <QtGraphs/qgraphsglobal.h>

QT_BEGIN_NAMESPACE

class QPieSlicePrivate;
class QPieSeries;

class Q_GRAPHS_EXPORT QPieSlice : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QString label READ label WRITE setLabel NOTIFY labelChanged)
    Q_PROPERTY(qreal value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(bool exploded READ isExploded WRITE setExploded NOTIFY explodedChanged)
    Q_PROPERTY(qreal explodeDistanceFactor READ explodeDistanceFactor WRITE
                       setExplodeDistanceFactor NOTIFY explodeDistanceFactorChanged)
    Q_PROPERTY(qreal percentage READ percentage NOTIFY percentageChanged)
    Q_PROPERTY(qreal startAngle READ startAngle NOTIFY startAngleChanged)
    Q_PROPERTY(qreal angleSpan READ angleSpan NOTIFY angleSpanChanged)
    QML_NAMED_ELEMENT(PieSlice)

public:
    explicit QPieSlice(QQuickItem *parent = nullptr);
    QPieSlice(const QString &label, qreal value, QQuickItem *parent = nullptr);
    virtual ~QPieSlice();

    void setLabel(const QString &label);
    QString label() const;

    void setValue(qreal value);
    qreal value() const;

    void setExploded(bool exploded);
    bool isExploded() const;
    void setExplodeDistanceFactor(qreal factor);
    qreal explodeDistanceFactor() const;

    qreal percentage() const;
    qreal startAngle() const;
    qreal angleSpan() const;

    QPieSeries *series() const;

Q_SIGNALS:
    void labelChanged();
    void valueChanged();
    void explodedChanged();
    void explodeDistanceFactorChanged();
    void sliceChanged();
    void percentageChanged();
    void startAngleChanged();
    void angleSpanChanged();

private:
    Q_DECLARE_PRIVATE(QPieSlice)
    Q_DISABLE_COPY(QPieSlice)
};

QT_END_NAMESPACE

#endif // QPIESLICE_H
