// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QBARSERIES_H
#define QBARSERIES_H

#include <QtGraphs/qabstractseries.h>
#include <QtGraphs/qgraphsglobal.h>

QT_BEGIN_NAMESPACE

class QBarSet;
class QBarSeriesPrivate;

class Q_GRAPHS_EXPORT QBarSeries : public QAbstractSeries
{
    Q_OBJECT
    Q_CLASSINFO("RegisterEnumClassesUnscoped", "false")
    Q_PROPERTY(BarsType barsType READ barsType WRITE setBarsType NOTIFY barsTypeChanged)
    Q_PROPERTY(qreal barWidth READ barWidth WRITE setBarWidth NOTIFY barWidthChanged)
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(bool labelsVisible READ isLabelsVisible WRITE setLabelsVisible NOTIFY labelsVisibleChanged)
    Q_PROPERTY(QString labelsFormat READ labelsFormat WRITE setLabelsFormat NOTIFY labelsFormatChanged)
    Q_PROPERTY(LabelsPosition labelsPosition READ labelsPosition WRITE setLabelsPosition NOTIFY labelsPositionChanged)
    Q_PROPERTY(qreal labelsMargin READ labelsMargin WRITE setLabelsMargin NOTIFY labelsMarginChanged)
    Q_PROPERTY(qreal labelsAngle READ labelsAngle WRITE setLabelsAngle NOTIFY labelsAngleChanged)
    Q_PROPERTY(int labelsPrecision READ labelsPrecision WRITE setLabelsPrecision NOTIFY labelsPrecisionChanged)
    Q_PROPERTY(QQmlComponent *barComponent READ barComponent WRITE setBarComponent NOTIFY barComponentChanged FINAL)
    Q_PROPERTY(QList<QBarSet *> barSets READ barSets NOTIFY barSetsChanged)
    QML_NAMED_ELEMENT(BarSeries)

public:
    enum class LabelsPosition {
        Center,
        InsideEnd,
        InsideBase,
        OutsideEnd
    };
    Q_ENUM(LabelsPosition)

    enum class BarsType {
        Groups,
        Stacked,
        StackedPercent
    };
    Q_ENUM(BarsType)

    explicit QBarSeries(QObject *parent = nullptr);
    QAbstractSeries::SeriesType type() const override;

    void setBarsType(QBarSeries::BarsType type);
    QBarSeries::BarsType barsType() const;

    void setBarWidth(qreal width);
    qreal barWidth() const;

    Q_INVOKABLE bool append(QBarSet *set);
    Q_INVOKABLE bool take(QBarSet *set);
    Q_INVOKABLE int count() const;
    Q_INVOKABLE bool append(const QList<QBarSet *> &sets);
    Q_INVOKABLE bool remove(QBarSet *set);
    Q_INVOKABLE bool insert(int index, QBarSet *set);
    Q_INVOKABLE void clear();
    Q_INVOKABLE void replace(int index, QBarSet *set);
    Q_INVOKABLE QBarSet *at(int index);
    Q_INVOKABLE int find(QBarSet *set) const;
    Q_INVOKABLE void removeMultiple(int index, int count);
    Q_INVOKABLE bool remove(int index);
    Q_INVOKABLE bool replace(QBarSet *oldValue, QBarSet *newValue);
    Q_INVOKABLE bool replace(const QList<QBarSet *> &sets);

    QList<QBarSet *> barSets() const;

    void setLabelsVisible(bool visible = true);
    bool isLabelsVisible() const;

    void setLabelsFormat(const QString &format);
    QString labelsFormat() const;

    void setLabelsMargin(qreal margin);
    qreal labelsMargin() const;

    void setLabelsAngle(qreal angle);
    qreal labelsAngle() const;

    void setLabelsPosition(QBarSeries::LabelsPosition position);
    QBarSeries::LabelsPosition labelsPosition() const;

    void setLabelsPrecision(int precision);
    int labelsPrecision() const;

    QQmlComponent *barComponent() const;
    void setBarComponent(QQmlComponent *newBarComponent);

public Q_SLOTS:
    void selectAll();
    void deselectAll();

protected:
    QBarSeries(QBarSeriesPrivate &dd, QObject *parent = nullptr);
    void componentComplete() override;

Q_SIGNALS:
    void clicked(int index, QBarSet *barset);
    void hovered(bool status, int index, QBarSet *barset);
    void pressed(int index, QBarSet *barset);
    void released(int index, QBarSet *barset);
    void doubleClicked(int index, QBarSet *barset);
    void updatedBars();
    void updatedLayout();
    void restructuredBars();
    void countChanged();
    void barWidthChanged();
    void labelsVisibleChanged(bool visible);
    void labelsFormatChanged(const QString &format);
    void labelsPositionChanged(QBarSeries::LabelsPosition position);
    void barsTypeChanged(QBarSeries::BarsType type);
    void labelsMarginChanged(qreal margin);
    void labelsAngleChanged(qreal angle);
    void labelsPrecisionChanged(int precision);
    void barComponentChanged();

    void barsetsAdded(const QList<QBarSet *> &sets);
    void barsetsReplaced(const QList<QBarSet *> &sets);
    void barsetsRemoved(const QList<QBarSet *> &sets);
    void setValueChanged(int index, QBarSet *barset);
    void setValueAdded(int index, int count, QBarSet *barset);
    void setValueRemoved(int index, int count, QBarSet *barset);
    void barSetsChanged();

private Q_SLOTS:
    void handleSetValueChange(int index);
    void handleSetValueAdd(int index, int count);
    void handleSetValueRemove(int index, int count);

private:
    Q_DECLARE_PRIVATE(QBarSeries)
    Q_DISABLE_COPY(QBarSeries)
    friend class BarSet;
    friend class BarsRenderer;
};

QT_END_NAMESPACE

#endif // QBARSERIES_H
