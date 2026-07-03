// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
// Qt-Security score:significant reason:default


#ifndef QTGRAPHS_QLOGVALUEAXIS_H
#define QTGRAPHS_QLOGVALUEAXIS_H

#include <QtGraphs/qabstractaxis.h>
#include <QtGraphs/qgraphsglobal.h>
#include <QtQml/qqmlregistration.h>

QT_BEGIN_NAMESPACE

class QLogValueAxisPrivate;

class Q_GRAPHS_EXPORT QLogValueAxis : public QAbstractAxis
{
    Q_OBJECT
    Q_PROPERTY(qreal min READ min WRITE setMin NOTIFY minChanged FINAL)
    Q_PROPERTY(qreal max READ max WRITE setMax NOTIFY maxChanged FINAL)
    Q_PROPERTY(qreal base READ base WRITE setBase NOTIFY baseChanged FINAL)
    Q_PROPERTY(
        QString labelFormat READ labelFormat WRITE setLabelFormat NOTIFY labelFormatChanged FINAL)
    Q_PROPERTY(QString labelPostFormat READ labelPostFormat WRITE setLabelPostFormat NOTIFY
                   labelPostFormatChanged REVISION(6, 12))
    Q_PROPERTY(int labelPrecision READ labelPrecision WRITE setlabelPrecision NOTIFY
                   labelPrecisionChanged FINAL)
    Q_PROPERTY(qsizetype subTickCount READ subTickCount WRITE setSubTickCount NOTIFY
                   subTickCountChanged FINAL)
    Q_PROPERTY(
        int tickCount READ tickCount NOTIFY tickCountChanged FINAL)
    Q_PROPERTY(qreal zoom READ zoom WRITE setZoom NOTIFY zoomChanged FINAL)
    Q_PROPERTY(qreal pan READ pan WRITE setPan NOTIFY panChanged FINAL)
    Q_PROPERTY(qreal visualMin READ visualMin NOTIFY visualMinChanged FINAL)
    Q_PROPERTY(qreal visualMax READ visualMax NOTIFY visualMaxChanged FINAL)
    QML_NAMED_ELEMENT(LogValueAxis)
    QML_ADDED_IN_VERSION(6, 12)

public:
    explicit QLogValueAxis(QObject *parent = nullptr);
    ~QLogValueAxis() override;

protected:
    QLogValueAxis(QLogValueAxisPrivate &d, QObject *parent = nullptr);

public:
    AxisType type() const override;

    //range handling
    void setMin(qreal min);
    qreal min() const;
    void setMax(qreal max);
    qreal max() const;
    void setRange(qreal min, qreal max);

    void setBase(qreal base);
    qreal base() const;

    //ticks handling
    void setSubTickCount(qsizetype count);
    qsizetype subTickCount() const;
    qsizetype tickCount() const;

    //label formatting
    void setLabelFormat(const QString &format);
    QString labelFormat() const;
    void setLabelPostFormat(const QString &format);
    QString labelPostFormat() const;
    void setlabelPrecision(int decimals);
    int labelPrecision() const;

    void setZoom(qreal zoom);
    qreal zoom() const;

    void setPan(qreal pan);
    qreal pan() const;

    qreal visualMin() const;
    qreal visualMax() const;

Q_SIGNALS:
    void minChanged(qreal min);
    void maxChanged(qreal max);
    void baseChanged(qreal base);
    void rangeChanged(qreal min, qreal max);
    void subTickCountChanged(qsizetype subTickCount);
    void labelFormatChanged(const QString &format);
    Q_REVISION(6, 12) void labelPostFormatChanged(const QString &format);
    void labelPrecisionChanged(int precision);
    void tickAnchorChanged(qreal tickAnchor);
    void tickCountChanged(qreal tickCount);
    void zoomChanged(qreal zoom);
    void panChanged(qreal pan);
    void visualMinChanged(qreal visualMin);
    void visualMaxChanged(qreal visualMax);

private:
    Q_DECLARE_PRIVATE(QLogValueAxis)
    Q_DISABLE_COPY(QLogValueAxis)
};

QT_END_NAMESPACE

#endif // QTGRAPHS_QLOGVALUEAXIS_H
