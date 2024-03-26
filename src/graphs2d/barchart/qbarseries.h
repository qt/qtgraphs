// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QBARSERIES_H
#define QBARSERIES_H

#include <QtGraphs/qabstractseries.h>
#include <QtGraphs/qabstractaxis.h>
#include <QtGraphs/qgraphsglobal.h>

QT_BEGIN_NAMESPACE

class QBarSet;
class QBarSeriesPrivate;

class Q_GRAPHS_EXPORT QBarSeries : public QAbstractSeries
{
    Q_OBJECT
    Q_PROPERTY(QAbstractAxis *axisX READ axisX WRITE setAxisX NOTIFY axisXChanged)
    Q_PROPERTY(QAbstractAxis *axisY READ axisY WRITE setAxisY NOTIFY axisYChanged)
    Q_PROPERTY(BarsType barsType READ barsType WRITE setBarsType NOTIFY barsTypeChanged)
    Q_PROPERTY(BarsOrientation barsOrientation READ barsOrientation WRITE setBarsOrientation NOTIFY barsOrientationChanged)
    Q_PROPERTY(qreal barWidth READ barWidth WRITE setBarWidth NOTIFY barWidthChanged)
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(bool labelsVisible READ isLabelsVisible WRITE setLabelsVisible NOTIFY labelsVisibleChanged)
    Q_PROPERTY(QString labelsFormat READ labelsFormat WRITE setLabelsFormat NOTIFY labelsFormatChanged)
    Q_PROPERTY(LabelsPosition labelsPosition READ labelsPosition WRITE setLabelsPosition NOTIFY labelsPositionChanged)
    Q_PROPERTY(qreal labelsAngle READ labelsAngle WRITE setLabelsAngle NOTIFY labelsAngleChanged)
    Q_PROPERTY(int labelsPrecision READ labelsPrecision WRITE setLabelsPrecision NOTIFY labelsPrecisionChanged)
    Q_PROPERTY(QQmlComponent *barComponent READ barComponent WRITE setBarComponent NOTIFY barComponentChanged FINAL)
    QML_NAMED_ELEMENT(BarSeries)

public:
    enum LabelsPosition {
        LabelsCenter = 0,
        LabelsInsideEnd,
        LabelsInsideBase,
        LabelsOutsideEnd
    };
    Q_ENUM(LabelsPosition)

    enum BarsType {
        BarsGroups = 0,
        BarsStacked
    };
    Q_ENUM(BarsType)

    enum BarsOrientation {
        BarsVertical = 0,
        BarsHorizontal
    };
    Q_ENUM(BarsOrientation)

    explicit QBarSeries(QObject *parent = nullptr);
    QAbstractSeries::SeriesType type() const override;
    ~QBarSeries() override;

    QAbstractAxis *axisX();
    void setAxisX(QAbstractAxis *axis);
    QAbstractAxis *axisY();
    void setAxisY(QAbstractAxis *axis);

    void setBarsType(QBarSeries::BarsType type);
    QBarSeries::BarsType barsType() const;

    void setBarsOrientation(QBarSeries::BarsOrientation orientation);
    QBarSeries::BarsOrientation barsOrientation() const;

    void setBarWidth(qreal width);
    qreal barWidth() const;

           // TODO: Consider making these slots, available from QML.
    bool append(QBarSet *set);
    bool remove(QBarSet *set);
    bool take(QBarSet *set);
    bool append(const QList<QBarSet *> &sets);
    bool insert(int index, QBarSet *set);
    int count() const;
    QList<QBarSet *> barSets() const;
    void clear();

    void setLabelsVisible(bool visible = true);
    bool isLabelsVisible() const;

    void setLabelsFormat(const QString &format);
    QString labelsFormat() const;

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
    void axisXChanged(QAbstractAxis *axis);
    void axisYChanged(QAbstractAxis *axis);
    void barWidthChanged();
    void labelsVisibleChanged(bool visible);
    void labelsFormatChanged(const QString &format);
    void labelsPositionChanged(QBarSeries::LabelsPosition position);
    void barsTypeChanged(QBarSeries::BarsType type);
    void barsOrientationChanged(QBarSeries::BarsOrientation orientation);
    void labelsAngleChanged(qreal angle);
    void labelsPrecisionChanged(int precision);
    void barComponentChanged();

    void barsetsAdded(const QList<QBarSet *> &sets);
    void barsetsRemoved(const QList<QBarSet *> &sets);
    void setValueChanged(int index, QBarSet *barset);
    void setValueAdded(int index, int count, QBarSet *barset);
    void setValueRemoved(int index, int count, QBarSet *barset);

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
