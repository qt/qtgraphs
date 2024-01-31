// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QPIESLICE_H
#define QPIESLICE_H

#include <QtCore/QObject>
#include <QtGraphs/qgraphsglobal.h>
#include <QtQml/QQmlEngine>

QT_BEGIN_NAMESPACE

class PieRenderer;
class QPieSlicePrivate;
class QPieSeries;

class Q_GRAPHS_EXPORT QPieSlice : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("RegisterEnumClassesUnscoped", "false")
    Q_PROPERTY(QString label READ label WRITE setLabel NOTIFY labelChanged)
    Q_PROPERTY(bool labelVisible READ isLabelVisible WRITE setLabelVisible NOTIFY labelVisibleChanged)
    Q_PROPERTY(LabelPosition labelPosition READ labelPosition WRITE setLabelPosition)
    Q_PROPERTY(QColor labelColor READ labelColor WRITE setLabelColor NOTIFY labelColorChanged)
    Q_PROPERTY(QFont labelFont READ labelFont WRITE setLabelFont NOTIFY labelFontChanged)
    Q_PROPERTY(qreal labelArmLengthFactor READ labelArmLengthFactor WRITE setLabelArmLengthFactor)
    Q_PROPERTY(qreal value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(bool exploded READ isExploded WRITE setExploded NOTIFY explodedChanged)
    Q_PROPERTY(qreal explodeDistanceFactor READ explodeDistanceFactor WRITE
                       setExplodeDistanceFactor NOTIFY explodeDistanceFactorChanged)
    Q_PROPERTY(qreal percentage READ percentage NOTIFY percentageChanged)
    Q_PROPERTY(qreal startAngle READ startAngle NOTIFY startAngleChanged)
    Q_PROPERTY(qreal angleSpan READ angleSpan NOTIFY angleSpanChanged)
    QML_NAMED_ELEMENT(PieSlice)

public:
    enum LabelPosition {
        LabelOutside,
        LabelInsideHorizontal,
        LabelInsideTangential,
        LabelInsideNormal
    };
    Q_ENUM(LabelPosition)

    explicit QPieSlice(QObject *parent = nullptr);
    QPieSlice(const QString &label, qreal value, QObject *parent = nullptr);
    virtual ~QPieSlice();

    void setLabel(const QString &label);
    QString label() const;
    void setLabelVisible(bool visible = true);
    bool isLabelVisible() const;
    void setLabelPosition(LabelPosition position);
    LabelPosition labelPosition();
    void setLabelColor(QColor color);
    QColor labelColor();
    void setLabelFont(const QFont &font);
    QFont labelFont() const;
    void setLabelArmLengthFactor(qreal factor);
    qreal labelArmLengthFactor() const;

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
    void labelVisibleChanged();
    void labelFontChanged();
    void labelColorChanged();
    void valueChanged();
    void explodedChanged();
    void explodeDistanceFactorChanged();
    void percentageChanged();
    void startAngleChanged();
    void angleSpanChanged();
    void sliceChanged();

private:
    friend class PieRenderer;
    friend class QPieSeries;
    friend class QPieSeriesPrivate;

    Q_DECLARE_PRIVATE(QPieSlice)
    Q_DISABLE_COPY(QPieSlice)
};

QT_END_NAMESPACE

#endif // QPIESLICE_H
