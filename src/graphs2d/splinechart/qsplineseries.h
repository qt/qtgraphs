// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QSPLINESERIES_H
#define QSPLINESERIES_H

#include <QtCore/QObject>
#include <QtGraphs/qxyseries.h>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class QSplineSeriesPrivate;

class Q_GRAPHS_EXPORT QSplineSeries : public QXYSeries
{
    Q_OBJECT
    Q_PROPERTY(qreal width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(Qt::PenCapStyle capStyle READ capStyle WRITE setCapStyle NOTIFY capStyleChanged)

public:
    explicit QSplineSeries(QObject *parent = nullptr);
    ~QSplineSeries();
    QAbstractSeries::SeriesType type() const override;

    QML_NAMED_ELEMENT(SplineSeries)

    qreal width() const;
    void setWidth(qreal newWidth);

    Qt::PenCapStyle capStyle() const;
    void setCapStyle(Qt::PenCapStyle newCapStyle);

Q_SIGNALS:
    void widthChanged();
    void capStyleChanged();

protected:
    QSplineSeries(QSplineSeriesPrivate &d, QObject *parent = nullptr);

    void componentComplete() override;

private:
    Q_DECLARE_PRIVATE(QSplineSeries)
    Q_DISABLE_COPY(QSplineSeries)
};

QT_END_NAMESPACE

#endif // QSPLINESERIES_H
