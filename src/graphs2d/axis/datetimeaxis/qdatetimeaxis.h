// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QDATETIMEAXIS_H
#define QDATETIMEAXIS_H

#include <QtGraphs/qabstractaxis.h>

QT_BEGIN_NAMESPACE

class QDateTimeAxisPrivate;

class Q_GRAPHS_EXPORT QDateTimeAxis : public QAbstractAxis
{
    Q_OBJECT
    Q_PROPERTY(QDateTime min READ min WRITE setMin NOTIFY minChanged FINAL)
    Q_PROPERTY(QDateTime max READ max WRITE setMax NOTIFY maxChanged FINAL)
    Q_PROPERTY(
        QString labelFormat READ labelFormat WRITE setLabelFormat NOTIFY labelFormatChanged FINAL)
    Q_PROPERTY(
        int minorTickCount READ minorTickCount WRITE setMinorTickCount NOTIFY minorTickCountChanged)
    Q_PROPERTY(
        qreal tickInterval READ tickInterval WRITE setTickInterval NOTIFY tickIntervalChanged FINAL)
    QML_NAMED_ELEMENT(DateTimeAxis)

public:
    explicit QDateTimeAxis(QObject *parent = nullptr);
    ~QDateTimeAxis() override;

protected:
    QDateTimeAxis(QDateTimeAxisPrivate &d, QObject *parent = nullptr);

public:
    AxisType type() const override;

    //range handling
    void setMin(QDateTime min);
    QDateTime min() const;
    void setMax(QDateTime max);
    QDateTime max() const;

    void setLabelFormat(QString format);
    QString labelFormat() const;

    qreal tickInterval() const;
    void setTickInterval(qreal newTickInterval);

    int minorTickCount() const;
    void setMinorTickCount(int newMinorTickCount);

Q_SIGNALS:
    void minChanged(QDateTime min);
    void maxChanged(QDateTime max);
    void labelFormatChanged(QString format);
    void tickIntervalChanged();
    void minorTickCountChanged();

private:
    Q_DECLARE_PRIVATE(QDateTimeAxis)
    Q_DISABLE_COPY(QDateTimeAxis)
};

QT_END_NAMESPACE

#endif // QDATETIMEAXIS_H
