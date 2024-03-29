// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QABSTRACTSERIES_H
#define QABSTRACTSERIES_H

#include <QtGraphs/QAbstractAxis>
#include <QtCore/QObject>
#include <QtGui/QPen>
#include <QtQml/QQmlParserStatus>
#include <QtQml/QQmlListProperty>
#include <QtGraphs/qseriestheme.h>
#include <QtGraphs/qgraphsglobal.h>

QT_BEGIN_NAMESPACE

class QAbstractSeriesPrivate;
class QGraphsView;

struct Q_GRAPHS_EXPORT QLegendData
{
    Q_GADGET
    Q_PROPERTY(QColor color MEMBER color)
    Q_PROPERTY(QColor borderColor MEMBER borderColor)
    Q_PROPERTY(QString label MEMBER label)

public:
    QColor color;
    QColor borderColor;
    QString label;
};

class Q_GRAPHS_EXPORT QAbstractSeries : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QAbstractSeries)
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(QSeriesTheme *theme READ theme WRITE setTheme NOTIFY themeChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(bool visible READ isVisible WRITE setVisible NOTIFY visibleChanged)
    Q_PROPERTY(bool selectable READ selectable WRITE setSelectable NOTIFY selectableChanged)
    Q_PROPERTY(bool hoverable READ hoverable WRITE setHoverable NOTIFY hoverableChanged)
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity NOTIFY opacityChanged)
    Q_PROPERTY(qreal valuesMultiplier READ valuesMultiplier WRITE setValuesMultiplier NOTIFY valuesMultiplierChanged)
    Q_PROPERTY(SeriesType type READ type CONSTANT)
    Q_PROPERTY(QQmlListProperty<QObject> seriesChildren READ seriesChildren CONSTANT)
    Q_PROPERTY(QList<QLegendData> legendData READ legendData NOTIFY legendDataChanged FINAL)
    Q_CLASSINFO("DefaultProperty", "seriesChildren")

public:
    enum SeriesType {
        SeriesTypeLine,
        SeriesTypeArea,
        SeriesTypeBar,
        //SeriesTypeStackedBar,
        //SeriesTypePercentBar,
        SeriesTypePie,
        SeriesTypeScatter,
        SeriesTypeSpline,
        //SeriesTypeHorizontalBar,
        //SeriesTypeHorizontalStackedBar,
        //SeriesTypeHorizontalPercentBar,
        //SeriesTypeBoxPlot,
    };
    Q_ENUM(SeriesType)

protected:
    explicit QAbstractSeries(QAbstractSeriesPrivate &dd, QObject *parent = nullptr);

    // from QDeclarativeParserStatus
    void classBegin() override;
    void componentComplete() override;

public:
    ~QAbstractSeries() override;
    virtual SeriesType type() const = 0;

    QSeriesTheme *theme() const;
    void setTheme(QSeriesTheme *newTheme);

    QString name() const;
    void setName(const QString &name);

    bool isVisible() const;
    void setVisible(bool visible = true);

    bool selectable() const;
    void setSelectable(bool selectable);

    qreal opacity() const;
    void setOpacity(qreal opacity);

    qreal valuesMultiplier() const;
    void setValuesMultiplier(qreal valuesMultiplier);

    QGraphsView *graph() const;
    void setGraph(QGraphsView *graph);

    bool attachAxis(QAbstractAxis *axis);
    bool detachAxis(QAbstractAxis *axis);
    QList<QAbstractAxis*> attachedAxes();

    const QList<QLegendData> legendData() const;

    void show();
    void hide();

    QQmlListProperty<QObject> seriesChildren();

    bool hoverable() const;
    void setHoverable(bool newHoverable);

public Q_SLOTS:
    static void appendSeriesChildren(QQmlListProperty<QObject> *list, QObject *element);

Q_SIGNALS:
    void update();
    void themeChanged();
    void nameChanged();
    void visibleChanged();
    void selectableChanged();
    void hoverableChanged();
    void opacityChanged();
    void valuesMultiplierChanged();
    void legendDataChanged();
    void hoverEnter(QString seriesName, QPointF position, QPointF value);
    void hoverExit(QString seriesName, QPointF position);
    void hover(QString seriesName, QPointF position, QPointF value);

protected:
    friend class BarsRenderer;
    friend class PointRenderer;
    friend class PieRenderer;
    friend class AreaRenderer;
};

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QLegendData)

#endif // QABSTRACTSERIES_H
