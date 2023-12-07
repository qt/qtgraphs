// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QVALUEAXIS_H
#define QVALUEAXIS_H

#include <QtGraphs2D/QAbstractAxis>
#include <QtQml/QQmlEngine>
#include <QtGraphs2D/qtgraphs2dexports.h>

QT_BEGIN_NAMESPACE

class QValueAxisPrivate;

class Q_GRAPHS2D_EXPORT QValueAxis : public QAbstractAxis
{
    Q_OBJECT
    Q_PROPERTY(int tickCount READ tickCount WRITE setTickCount NOTIFY tickCountChanged)
    Q_PROPERTY(qreal min READ min WRITE setMin NOTIFY minChanged)
    Q_PROPERTY(qreal max READ max WRITE setMax NOTIFY maxChanged)
    Q_PROPERTY(QString labelFormat READ labelFormat WRITE setLabelFormat NOTIFY labelFormatChanged)
    Q_PROPERTY(int minorTickCount READ minorTickCount WRITE setMinorTickCount NOTIFY minorTickCountChanged)
    Q_PROPERTY(qreal tickAnchor READ tickAnchor WRITE setTickAnchor NOTIFY tickAnchorChanged)
    Q_PROPERTY(qreal tickInterval READ tickInterval WRITE setTickInterval NOTIFY tickIntervalChanged)
    Q_PROPERTY(TickType tickType READ tickType WRITE setTickType NOTIFY tickTypeChanged)
    Q_PROPERTY(bool autoScale READ autoScale WRITE setAutoScale NOTIFY autoScaleChanged)
    Q_ENUMS(TickType)
    QML_NAMED_ELEMENT(ValueAxis)

public:
    enum TickType {
        TicksDynamic = 0,
        TicksFixed
    };

public:
    explicit QValueAxis(QObject *parent = nullptr);
    ~QValueAxis();

protected:
    QValueAxis(QValueAxisPrivate &d, QObject *parent = nullptr);

public:
    AxisType type() const override;

    //range handling
    void setMin(qreal min);
    qreal min() const;
    void setMax(qreal max);
    qreal max() const;
    void setRange(qreal min, qreal max);

    //ticks handling
    void setTickCount(int count);
    int tickCount() const;
    void setMinorTickCount(int count);
    int minorTickCount() const;
    void setTickAnchor(qreal anchor);
    qreal tickAnchor() const;
    void setTickInterval(qreal insterval);
    qreal tickInterval() const;
    void setTickType(QValueAxis::TickType type);
    QValueAxis::TickType tickType() const;

    void setLabelFormat(const QString &format);
    QString labelFormat() const;

    bool autoScale() const;
    void setAutoScale(bool newAutoScale);

public Q_SLOTS:
    void applyNiceNumbers();

Q_SIGNALS:
    void minChanged(qreal min);
    void maxChanged(qreal max);
    void rangeChanged(qreal min, qreal max);
    void tickCountChanged(int tickCount);
    void minorTickCountChanged(int tickCount);
    void labelFormatChanged(const QString &format);
    void tickIntervalChanged(qreal interval);
    void tickAnchorChanged(qreal anchor);
    void tickTypeChanged(QValueAxis::TickType type);
    void autoScaleChanged();

private:
    Q_DECLARE_PRIVATE(QValueAxis)
    Q_DISABLE_COPY(QValueAxis)
};

QT_END_NAMESPACE

#endif // QVALUEAXIS_H
