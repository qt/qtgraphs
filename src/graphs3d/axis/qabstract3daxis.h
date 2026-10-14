// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
// Qt-Security score:significant reason:default


#ifndef QTGRAPHS_QABSTRACT3DAXIS_H
#define QTGRAPHS_QABSTRACT3DAXIS_H

#include <QtCore/qobject.h>
#include <QtCore/qscopedpointer.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qtclasshelpermacros.h>
#include <QtGraphs/qgraphsglobal.h>
#include <QtQmlIntegration/qqmlintegration.h>

QT_BEGIN_NAMESPACE

class QAbstract3DAxisPrivate;

class Q_GRAPHS_EXPORT QAbstract3DAxis : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QAbstract3DAxis)
    Q_CLASSINFO("RegisterEnumClassesUnscoped", "false")
    Q_PROPERTY(bool visible READ isVisible WRITE setVisible NOTIFY visibilityChanged REVISION(6, 13))
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged FINAL)
    Q_PROPERTY(QStringList labels READ labels WRITE setLabels NOTIFY labelsChanged)
    Q_PROPERTY(bool labelsVisible READ labelsVisible WRITE setLabelsVisible NOTIFY
                   labelVisibleChanged FINAL)
    Q_PROPERTY(QAbstract3DAxis::AxisOrientation orientation READ orientation NOTIFY
                   orientationChanged FINAL)
    Q_PROPERTY(QAbstract3DAxis::AxisType type READ type CONSTANT)
    Q_PROPERTY(float min READ min WRITE setMin NOTIFY minChanged FINAL)
    Q_PROPERTY(float max READ max WRITE setMax NOTIFY maxChanged FINAL)
    Q_PROPERTY(bool autoAdjustRange READ isAutoAdjustRange WRITE setAutoAdjustRange NOTIFY
                   autoAdjustRangeChanged FINAL)
    Q_PROPERTY(float labelAutoAngle READ labelAutoAngle WRITE setLabelAutoAngle NOTIFY
                   labelAutoAngleChanged FINAL)
    Q_PROPERTY(bool scaleLabelsByCount READ isScaleLabelsByCount WRITE setScaleLabelsByCount NOTIFY
                   scaleLabelsByCountChanged REVISION(6, 9))
    Q_PROPERTY(qreal labelSize READ labelSize WRITE setLabelSize NOTIFY labelSizeChanged NOTIFY
                   labelSizeChanged REVISION(6, 9))
    Q_PROPERTY(bool titleVisible READ isTitleVisible WRITE setTitleVisible NOTIFY
                   titleVisibleChanged FINAL)
    Q_PROPERTY(bool titleFixed READ isTitleFixed WRITE setTitleFixed NOTIFY titleFixedChanged FINAL)
    Q_PROPERTY(
        float titleOffset READ titleOffset WRITE setTitleOffset NOTIFY titleOffsetChanged FINAL)
    QML_NAMED_ELEMENT(Abstract3DAxis)
    QML_UNCREATABLE("")

public:
    enum class AxisOrientation { None, X, Y, Z };
    Q_ENUM(AxisOrientation)

    enum class AxisType {
        None,
        Category,
        Value,
    };
    Q_ENUM(AxisType)

protected:
    explicit QAbstract3DAxis(QAbstract3DAxisPrivate &d, QObject *parent = nullptr);

public:
    ~QAbstract3DAxis() override;

    bool isVisible() const;

    QString title() const;

    QStringList labels() const;

    QAbstract3DAxis::AxisOrientation orientation() const;
    QAbstract3DAxis::AxisType type() const;

    float min() const;

    float max() const;

    bool isAutoAdjustRange() const;

    bool isScaleLabelsByCount() const;

    qreal labelSize() const;

    float labelAutoAngle() const;

    bool isTitleVisible() const;

    bool labelsVisible() const;

    bool isTitleFixed() const;

    float titleOffset() const;

public Q_SLOTS:
    void setVisible(bool visible);
    void setTitle(const QString &title);
    void setLabels(const QStringList &labels);
    void setMin(float min);
    void setMax(float max);
    void setAutoAdjustRange(bool autoAdjust);
    void setScaleLabelsByCount(bool adjust);
    void setLabelSize(qreal size);
    void setRange(float min, float max);
    void setLabelAutoAngle(float degree);
    void setTitleVisible(bool visible);
    void setLabelsVisible(bool visible);
    void setTitleFixed(bool fixed);
    void setTitleOffset(float offset);

Q_SIGNALS:
    Q_REVISION(6, 13) void visibilityChanged(bool visible);
    void titleChanged(const QString &newTitle);
    void labelsChanged();
    void orientationChanged(QAbstract3DAxis::AxisOrientation orientation);
    void minChanged(float value);
    void maxChanged(float value);
    void rangeChanged(float min, float max);
    void autoAdjustRangeChanged(bool autoAdjust);
    Q_REVISION(6, 9) void scaleLabelsByCountChanged(bool adjust);
    Q_REVISION(6, 9) void labelSizeChanged(qreal size);
    void labelAutoAngleChanged(float angle);
    void titleVisibleChanged(bool visible);
    void labelVisibleChanged(bool visible);
    void titleFixedChanged(bool fixed);
    void titleOffsetChanged(float offset);

private:
    Q_DISABLE_COPY(QAbstract3DAxis)

    friend class QQuickGraphsItem;
    friend class QQuickGraphsSurface;
    friend class QQuickGraphsScatter;
    friend class QQuickGraphsBars;
    friend class QScatterDataProxyPrivate;
    friend class QSurfaceDataProxyPrivate;
};

QT_END_NAMESPACE

#endif
