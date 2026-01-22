// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QTGRAPHS_QDATETIMEAXIS_H
#define QTGRAPHS_QDATETIMEAXIS_H

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
        int subTickCount READ subTickCount WRITE setSubTickCount NOTIFY subTickCountChanged FINAL)
    Q_PROPERTY(
        qreal tickInterval READ tickInterval WRITE setTickInterval NOTIFY tickIntervalChanged FINAL)
    Q_PROPERTY(
        QString timeZone READ timeZone WRITE setTimeZone NOTIFY timeZoneChanged REVISION(6, 11))
    Q_PROPERTY(qreal zoom READ zoom WRITE setZoom NOTIFY zoomChanged REVISION(6, 11))
    Q_PROPERTY(qreal pan READ pan WRITE setPan NOTIFY panChanged REVISION(6, 11))
    Q_PROPERTY(QDateTime visualMin READ visualMin NOTIFY visualMinChanged REVISION(6, 11))
    Q_PROPERTY(QDateTime visualMax READ visualMax NOTIFY visualMaxChanged REVISION(6, 11))

    QML_NAMED_ELEMENT(DateTimeAxis)

public:
    explicit QDateTimeAxis(QObject *parent = nullptr);
    ~QDateTimeAxis() override;

protected:
    QDateTimeAxis(QDateTimeAxisPrivate &d, QObject *parent = nullptr);

public:
    AxisType type() const override;

    //range handling
    void setMin(const QDateTime &min);
    QDateTime min() const;
    void setMax(const QDateTime &max);
    QDateTime max() const;

    void setLabelFormat(const QString &format);
    QString labelFormat() const;

    qreal tickInterval() const;
    void setTickInterval(qreal newTickInterval);

    int subTickCount() const;
    void setSubTickCount(int newSubTickCount);

    QString timeZone() const;
    void setTimeZone(const QString &zoneId);

    void setZoom(qreal zoom);
    qreal zoom() const;

    qreal pan() const;
    void setPan(qreal pan);

    QDateTime visualMin() const;
    QDateTime visualMax() const;

Q_SIGNALS:
    void minChanged(const QDateTime &min);
    void maxChanged(const QDateTime &max);
    void labelFormatChanged(const QString &format);
    void tickIntervalChanged();
    void subTickCountChanged();
    Q_REVISION(6, 11) void timeZoneChanged(const QString &timeZone);
    Q_REVISION(6, 11) void zoomChanged(qreal zoom);
    Q_REVISION(6, 11) void panChanged(qreal pan);
    Q_REVISION(6, 11) void visualMinChanged(const QDateTime &min);
    Q_REVISION(6, 11) void visualMaxChanged(const QDateTime &max);

private:
    Q_DECLARE_PRIVATE(QDateTimeAxis)
    Q_DISABLE_COPY(QDateTimeAxis)
};

QT_END_NAMESPACE

#endif // QTGRAPHS_QDATETIMEAXIS_H
