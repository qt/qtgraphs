// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGraphs/qareaseries.h>
#include <private/qareaseries_p.h>
#include <private/qgraphsview_p.h>

QT_BEGIN_NAMESPACE

/*!
    \class QAreaSeries
    \inmodule QtGraphs
    \ingroup graphs_2D
    \brief The QAreaSeries class presents data in area graphs.

    An area graph is used to draw an area composed by points.
    The points are defined by two series: upperSeries and lowerSeries.
    The area between the series is drawn as a graph. If only the upperSeries
    is defined, the area is then between the bottom of the graph and the upper series.
*/
/*!
    \qmltype AreaSeries
    \instantiates QAreaSeries
    \inqmlmodule QtGraphs
    \ingroup graphs_qml_2D
    \inherits AbstractSeries

    \brief Presents data in area graphs.

    An area graph is used to draw an area composed by points.
    The points are defined by two series: upperSeries and lowerSeries.
    The area between the series is drawn as a graph. If only the upperSeries
    is defined, the area is then between the bottom of the graph and the upper series.

    \image graphs2d-area.png
*/

/*!
    \qmlproperty color AreaSeries::color
    The fill color of the area.
*/

/*!
    \qmlproperty color AreaSeries::selectedColor
    The fill color of the area when selected.
*/

/*!
    \qmlproperty color AreaSeries::borderColor
    The border color of the area.
*/

/*!
    \qmlproperty color AreaSeries::selectedBorderColor
    The border color of the area when selected.
*/

/*!
    \qmlproperty real AreaSeries::borderWidth
    The width of the line that encloses the area.
*/

/*!
    \qmlproperty bool AreaSeries::selected
    Sets this area as selected.
*/

/*!
    \qmlproperty XYSeries AreaSeries::upperSeries
    Sets the upper boundary of the area. No area is drawn if this is null.
*/

/*!
    \qmlproperty XYSeries AreaSeries::lowerSeries
    Sets the lower boundary of the area. If this is null, the graph bottom
    is considered the lower bound.
*/

QAreaSeries::QAreaSeries(QObject *parent)
    : QAbstractSeries(*(new QAreaSeriesPrivate()), parent)
{}

QAreaSeries::QAreaSeries(QAreaSeriesPrivate &dd, QObject *parent)
    : QAbstractSeries(dd, parent)
{}

QAbstractSeries::SeriesType QAreaSeries::type() const
{
    return QAbstractSeries::SeriesType::Area;
}

QColor QAreaSeries::color() const
{
    Q_D(const QAreaSeries);
    return d->m_color;
}

void QAreaSeries::setColor(const QColor &newColor)
{
    Q_D(QAreaSeries);
    if (color() != newColor) {
        d->m_color = newColor;
        emit colorChanged(newColor);
    }
}

QColor QAreaSeries::selectedColor() const
{
    Q_D(const QAreaSeries);
    return d->m_selectedColor;
}

void QAreaSeries::setSelectedColor(const QColor &newSelectedColor)
{
    Q_D(QAreaSeries);
    if (selectedColor() != newSelectedColor) {
        d->m_selectedColor = newSelectedColor;
        emit selectedColorChanged(newSelectedColor);
    }
}

QColor QAreaSeries::borderColor() const
{
    Q_D(const QAreaSeries);
    return d->m_borderColor;
}

void QAreaSeries::setBorderColor(const QColor &newBorderColor)
{
    Q_D(QAreaSeries);
    if (d->m_borderColor == newBorderColor)
        return;
    d->m_borderColor = newBorderColor;
    emit borderColorChanged(newBorderColor);
}

QColor QAreaSeries::selectedBorderColor() const
{
    Q_D(const QAreaSeries);
    return d->m_selectedBorderColor;
}

void QAreaSeries::setSelectedBorderColor(const QColor &newSelectedBorderColor)
{
    Q_D(QAreaSeries);
    if (d->m_selectedBorderColor == newSelectedBorderColor)
        return;
    d->m_selectedBorderColor = newSelectedBorderColor;
    emit selectedBorderColorChanged(newSelectedBorderColor);
}

qreal QAreaSeries::borderWidth() const
{
    Q_D(const QAreaSeries);
    return d->m_borderWidth;
}

void QAreaSeries::setBorderWidth(qreal newBorderWidth)
{
    Q_D(QAreaSeries);
    if (qFuzzyCompare(d->m_borderWidth, newBorderWidth))
        return;
    d->m_borderWidth = newBorderWidth;
    emit borderWidthChanged();
}

bool QAreaSeries::selected() const
{
    Q_D(const QAreaSeries);
    return d->m_selected;
}

void QAreaSeries::setSelected(bool newSelected)
{
    Q_D(QAreaSeries);
    if (d->m_selected == newSelected)
        return;
    d->m_selected = newSelected;
    emit selectedChanged();
}

QXYSeries *QAreaSeries::upperSeries() const
{
    Q_D(const QAreaSeries);
    return d->m_upperSeries;
}

void QAreaSeries::setUpperSeries(QXYSeries *newUpperSeries)
{
    Q_D(QAreaSeries);
    if (d->m_upperSeries == newUpperSeries)
        return;
    d->m_upperSeries = newUpperSeries;
    emit upperSeriesChanged();
}

QXYSeries *QAreaSeries::lowerSeries() const
{
    Q_D(const QAreaSeries);
    return d->m_lowerSeries;
}

void QAreaSeries::setLowerSeries(QXYSeries *newLowerSeries)
{
    Q_D(QAreaSeries);
    if (d->m_lowerSeries == newLowerSeries)
        return;
    d->m_lowerSeries = newLowerSeries;
    emit lowerSeriesChanged();
}

QAreaSeriesPrivate::QAreaSeriesPrivate() {}

QT_END_NAMESPACE
