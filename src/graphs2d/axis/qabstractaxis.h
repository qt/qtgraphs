// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QABSTRACTAXIS_H
#define QABSTRACTAXIS_H

#include <QtGui/QColor>
#include <QtGui/QFont>
#include <QtCore/QVariant>
#include <QtCore/QObject>
#include <QtQml/QQmlEngine>
#include <QtGraphs2D/qtgraphs2dexports.h>

QT_BEGIN_NAMESPACE

class QAbstractAxisPrivate;

class Q_GRAPHS2D_EXPORT QAbstractAxis : public QObject
{
    Q_OBJECT
    //visibility
    Q_PROPERTY(bool visible READ isVisible WRITE setVisible NOTIFY visibleChanged)
    Q_PROPERTY(bool lineVisible READ isLineVisible WRITE setLineVisible NOTIFY lineVisibleChanged)
    //labels
    Q_PROPERTY(bool labelsVisible READ labelsVisible WRITE setLabelsVisible NOTIFY labelsVisibleChanged)
    Q_PROPERTY(qreal labelsAngle READ labelsAngle WRITE setLabelsAngle NOTIFY labelsAngleChanged)
    //grid
    Q_PROPERTY(bool gridVisible READ isGridLineVisible WRITE setGridLineVisible NOTIFY gridVisibleChanged)
    Q_PROPERTY(bool minorGridVisible READ isMinorGridLineVisible WRITE setMinorGridLineVisible NOTIFY minorGridVisibleChanged)
    //title
    Q_PROPERTY(QString titleText READ titleText WRITE setTitleText NOTIFY titleTextChanged)
    Q_PROPERTY(QColor titleColor READ titleColor WRITE setTitleColor NOTIFY titleColorChanged)
    Q_PROPERTY(bool titleVisible READ isTitleVisible WRITE setTitleVisible NOTIFY titleVisibleChanged)
    Q_PROPERTY(QFont titleFont READ titleFont WRITE setTitleFont NOTIFY titleFontChanged)
    //orientation
    Q_PROPERTY(Qt::Orientation orientation READ orientation)
    //alignment
    Q_PROPERTY(Qt::Alignment alignment READ alignment)
    QML_FOREIGN(QAbstractAxis)
    QML_UNCREATABLE("Trying to create uncreatable: AbstractAxis.")
    QML_NAMED_ELEMENT(AbstractAxis)

public:

    enum AxisType {
        AxisTypeNoAxis = 0x0,
        AxisTypeValue = 0x1,
        AxisTypeBarCategory = 0x2,
        AxisTypeCategory = 0x4,
        AxisTypeDateTime = 0x8,
        AxisTypeLogValue = 0x10,
        AxisTypeColor = 0x20
    };

    Q_DECLARE_FLAGS(AxisTypes, AxisType)

protected:
    explicit QAbstractAxis(QAbstractAxisPrivate &d, QObject *parent = nullptr);

public:
    ~QAbstractAxis();

    virtual AxisType type() const = 0;

    //visibility handling
    bool isVisible() const;
    void setVisible(bool visible = true);
    void show();
    void hide();

    //arrow handling
    bool isLineVisible() const;
    void setLineVisible(bool visible = true);

    //grid handling
    bool isGridLineVisible() const;
    void setGridLineVisible(bool visible = true);
    bool isMinorGridLineVisible() const;
    void setMinorGridLineVisible(bool visible = true);

    //labels handling
    bool labelsVisible() const;
    void setLabelsVisible(bool visible = true);
    void setLabelsAngle(qreal angle);
    qreal labelsAngle() const;

    //title handling
    bool isTitleVisible() const;
    void setTitleVisible(bool visible = true);
    void setTitleColor(const QColor &color);
    QColor titleColor() const;
    void setTitleFont(const QFont &font);
    QFont titleFont() const;
    void setTitleText(const QString &title);
    QString titleText() const;

    Qt::Orientation orientation() const;
    void setOrientation(Qt::Orientation orientation);
    Qt::Alignment alignment() const;

    //range handling
    void setMin(const QVariant &min);
    void setMax(const QVariant &max);
    void setRange(const QVariant &min, const QVariant &max);

Q_SIGNALS:
    void visibleChanged(bool visible);
    void lineVisibleChanged(bool visible);
    void labelsVisibleChanged(bool visible);
    void labelsAngleChanged(qreal angle);
    void gridVisibleChanged(bool visible);
    void minorGridVisibleChanged(bool visible);
    void colorChanged(QColor color);
    void titleTextChanged(const QString &title);
    void titleColorChanged(const QColor &color);
    void titleVisibleChanged(bool visible);
    void titleFontChanged(const QFont &font);
    void update();

protected:
    QScopedPointer<QAbstractAxisPrivate> d_ptr;
    friend class ChartDataSet;
    friend class ChartPresenter;
    friend class ChartThemeManager;
    friend class AbstractDomain;
    friend class ChartAxisElement;
    friend class HorizontalAxis;
    friend class VerticalAxis;
    friend class XYChart;

private:
    Q_DISABLE_COPY(QAbstractAxis)
};

QT_END_NAMESPACE

#endif // QABSTRACTAXIS_H
