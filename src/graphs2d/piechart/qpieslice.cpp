// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <private/qpieslice_p.h>

QT_BEGIN_NAMESPACE

/*!
    \class QPieSlice
    \inmodule QtGraphs
    \ingroup graphs_2D
    \brief The QPieSlice class represents a single slice in a pie series.

    A pie slice has a value and a label. When the slice is added to a pie series, the
    QPieSeries object calculates the percentage of the slice compared with the sum of
    all slices in the series to determine the actual size of the slice in the graph.

    By default, the label is hidden. If it is visible, it can be either located outside
    the slice and connected to it with an arm or centered inside the slice either
    horizontally or in parallel with the tangential or normal of the slice's arc.

    By default, the visual appearance of the slice is set by a theme, but the theme can be
    overridden by specifying slice properties. However, if the theme is changed after the
    slices are customized, all customization will be lost.

    To enable user interaction with the pie graph, some basic signals are emitted when
    users click pie slices or hover the mouse over them.

    \sa QPieSeries
*/

/*!
    \qmltype PieSlice
    \instantiates QPieSlice
    \inqmlmodule QtGraphs
    \ingroup graphs_qml_2D
    \brief Represents a single slice in a pie series.

    A pie slice has a value and a label. When the slice is added to a pie series, the
    PieSeries type calculates the percentage of the slice compared with the sum of
    all slices in the series to determine the actual size of the slice in the graph.

    By default, the label is hidden. If it is visible, it can be either located outside
    the slice and connected to it with an arm or centered inside the slice either
    horizontally or in parallel with the tangential or normal of the slice's arc.

    By default, the visual appearance of the slice is set by a theme, but the theme can be
    overridden by specifying slice properties. However, if the theme is changed after the
    slices are customized, all customization will be lost.

    The PieSlice type should be used as a child of a PieSeries type. For example:

    Alternatively, slices can be added to a pie series by using the \l{PieSeries::append()}
    {PieSeries.append()} method.

    In that case, \l{PieSeries::at()}{PieSeries.at()} or \l {PieSeries::find}
    {PieSeries.find} can be used to access the properties of an individual PieSlice instance.
    \sa PieSeries
*/

/* TODO: Label implementation (QTBUG-121694)
    \enum QPieSlice::LabelPosition

    This enum describes the position of the slice label.

    \value  LabelOutside
         The label is located outside the slice connected to it with an arm.
         This is the default value.
    \value  LabelInsideHorizontal
         The label is centered within the slice and laid out horizontally.
    \value  LabelInsideTangential
         The label is centered within the slice and rotated to be parallel with
         the tangential of the slice's arc.
    \value  LabelInsideNormal
         The label is centered within the slice and rotated to be parallel with
         the normal of the slice's arc.
*/

/*!
    \property QPieSlice::label
    \brief The label of the slice.
    \note The string can be HTML formatted.

    \omit TODO: Label implementation (QTBUG-121694)
    \sa labelVisible, labelBrush, labelFont, labelArmLengthFactor
    \endomit
*/
/*!
    \qmlproperty string PieSlice::label
    The label of the slice.
    \note The string can be HTML formatted.
*/

/*!
    \fn void QPieSlice::labelChanged()
    This signal is emitted when the slice label changes.
    \sa label
*/

/*!
    \property QPieSlice::value
    \brief The value of the slice.
    \note A negative value is converted to a positive value.
    \sa percentage(), QPieSeries::sum()
*/
/*!
    \qmlproperty real PieSlice::value
    The value of the slice.

    \note A negative value is converted to a positive value.
*/

/*!
    \fn void QPieSlice::valueChanged()
    This signal is emitted when the slice value changes.
    \sa value
*/

/* TODO: Label implementation (QTBUG-121694)
    \property QPieSlice::labelVisible
    \brief The visibility of the slice label. By default, the label is not visible.
    \sa label, labelBrush, labelFont, labelArmLengthFactor
*/
/* TODO: Label implementation (QTBUG-121694)
    \qmlproperty bool PieSlice::labelVisible
    The visibility of the slice label. By default, the label is not visible.
*/

/* TODO: Label implementation (QTBUG-121694)
    \fn void QPieSlice::labelVisibleChanged()
    This signal is emitted when the visibility of the slice label changes.
    \sa labelVisible
*/

/*!
    \property QPieSlice::exploded
    \brief Whether the slice is separated from the pie.
    \sa explodeDistanceFactor
*/
/*!
    \qmlproperty bool PieSlice::exploded
    Whether the slice is separated from the pie.
    \sa explodeDistanceFactor
*/

/* TODO: Label implementation (QTBUG-121694)
    \property QPieSlice::labelColor
    \brief The color used to draw the slice label.
    This is a convenience property for modifying the slice label brush.
    \sa labelBrush
*/
/* TODO: Label implementation (QTBUG-121694)
    \qmlproperty color PieSlice::labelColor
    The color used to draw the slice label.
*/

/* TODO: Label implementation (QTBUG-121694)
    \fn void QPieSlice::labelColorChanged()
    This signal is emitted when the slice label color changes.
    \sa labelColor
*/

/* TODO: Label implementation (QTBUG-121694)
    \property QPieSlice::labelFont
    \brief The font used for drawing the label text.
    \sa label, labelVisible, labelArmLengthFactor
*/

/* TODO: Label implementation (QTBUG-121694)
    \fn void QPieSlice::labelFontChanged()
    This signal is emitted when the label font of the slice changes.
    \sa labelFont
*/

/* TODO: Label implementation (QTBUG-121694)
    \qmlproperty font PieSlice::labelFont

    The font used for the slice label.

    For more information, see \l [QML]{font}.

    \sa labelVisible, labelPosition
*/

/* TODO: Label implementation (QTBUG-121694)
    \property QPieSlice::labelPosition
    \brief The position of the slice label.
    \sa label, labelVisible
*/
/* TODO: Label implementation (QTBUG-121694)
    \qmlproperty enumeration PieSlice::labelPosition

    Describes the position of the slice label.

    \value  PieSlice.LabelOutside
         The label is located outside the slice connected to it with an arm.
         This is the default value.
    \value  PieSlice.LabelInsideHorizontal
         The label is centered within the slice and laid out horizontally.
    \value  PieSlice.LabelInsideTangential
         The label is centered within the slice and rotated to be parallel with
         the tangential of the slice's arc.
    \value  PieSlice.LabelInsideNormal
         The label is centered within the slice and rotated to be parallel with
         the normal of the slice's arc.

    \sa labelVisible
*/

/* TODO: Label implementation (QTBUG-121694)
    \property QPieSlice::labelArmLengthFactor
    \brief The length of the label arm.
    The factor is relative to the pie radius. For example:
    \list
        \li 1.0 means that the length is the same as the radius.
        \li 0.5 means that the length is half of the radius.
    \endlist
    By default, the arm length is 0.15
    \sa label, labelVisible, labelBrush, labelFont
*/
/* TODO: Label implementation (QTBUG-121694)
    \qmlproperty real PieSlice::labelArmLengthFactor
    The length of the label arm.
    The factor is relative to the pie radius. For example:
    \list
        \li 1.0 means that the length is the same as the radius.
        \li 0.5 means that the length is half of the radius.
    \endlist
    By default, the arm length is 0.15

    \sa labelVisible
*/

/*!
    \property QPieSlice::explodeDistanceFactor
    \brief Determines how far away from the pie the slice is exploded.
    \list
        \li 1.0 means that the distance is the same as the radius.
        \li 0.5 means that the distance is half of the radius.
    \endlist
    By default, the distance is 0.15
    \sa exploded
*/
/*!
    \qmlproperty real PieSlice::explodeDistanceFactor
    Determines how far away from the pie the slice is exploded.
    \list
        \li 1.0 means that the distance is the same as the radius.
        \li 0.5 means that the distance is half of the radius.
    \endlist
    By default, the distance is 0.15

    \sa exploded
*/

/*!
    \property QPieSlice::percentage
    \brief The percentage of the slice compared to the sum of all slices in the series.
    The actual value ranges from 0.0 to 1.0.
    Updated automatically once the slice is added to the series.
    \sa value, QPieSeries::sum
*/
/*!
    \qmlproperty real PieSlice::percentage
    The percentage of the slice compared to the sum of all slices in the series.
    The actual value ranges from 0.0 to 1.0.
    Updated automatically once the slice is added to the series.
*/

/*!
    \fn void QPieSlice::percentageChanged()
    This signal is emitted when the percentage of the slice changes.
    \sa percentage
*/

/*!
    \property QPieSlice::startAngle
    \brief The starting angle of this slice in the series it belongs to.
    A full pie is 360 degrees, where 0 degrees is at 12 a'clock.
    Updated automatically once the slice is added to the series.
*/
/*!
    \qmlproperty real PieSlice::startAngle
    The starting angle of this slice in the series it belongs to.
    A full pie is 360 degrees, where 0 degrees is at 12 a'clock.
    Updated automatically once the slice is added to the series.
*/

/*!
    \fn void QPieSlice::startAngleChanged()
    This signal is emitted when the starting angle of the slice changes.
    \sa startAngle
*/

/*!
    \property QPieSlice::angleSpan
    \brief  The span of the slice in degrees.
    A full pie is 360 degrees, where 0 degrees is at 12 a'clock.
    Updated automatically once the slice is added to the series.
*/
/*!
    \qmlproperty real PieSlice::angleSpan
    The span of the slice in degrees.
    A full pie is 360 degrees, where 0 degrees is at 12 a'clock.
    Updated automatically once the slice is added to the series.
*/

/*!
    \fn void QPieSlice::angleSpanChanged()
    This signal is emitted when the angle span of the slice changes.
    \sa angleSpan
*/

/*!
    Constructs an empty slice with the parent \a parent.
    \sa QPieSeries::append(), QPieSeries::insert()
*/
QPieSlice::QPieSlice(QQuickItem *parent)
    : QQuickItem(*(new QPieSlicePrivate), parent)
{
}

/*!
    Constructs an empty slice with the specified \a value, \a label, and \a parent.
    \sa QPieSeries::append(), QPieSeries::insert()
*/
QPieSlice::QPieSlice(const QString &label, qreal value, QQuickItem *parent)
    : QQuickItem(*(new QPieSlicePrivate), parent)
{
    Q_D(QPieSlice);
    d->m_labelText = label;
    d->m_value = value;
}

/*!
    Removes the slice. The slice should not be removed if it has been added to a series.
*/
QPieSlice::~QPieSlice()
{
}

/*!
    Returns the series that this slice belongs to.

    \sa QPieSeries::append()
*/
QPieSeries *QPieSlice::series() const
{
    const Q_D(QPieSlice);
    return d->m_series;
}

qreal QPieSlice::percentage() const
{
    const Q_D(QPieSlice);
    return d->m_percentage;
}

qreal QPieSlice::startAngle() const
{
    const Q_D(QPieSlice);
    return d->m_startAngle;
}

qreal QPieSlice::angleSpan() const
{
    const Q_D(QPieSlice);
    return d->m_angleSpan;
}

void QPieSlice::setLabel(const QString &label)
{
    Q_D(QPieSlice);
    if (d->m_labelText == label)
        return;
    d->m_labelText = label;
    emit labelChanged();
}

QString QPieSlice::label() const
{
    const Q_D(QPieSlice);
    return d->m_labelText;
}

void QPieSlice::setValue(qreal value)
{
    Q_D(QPieSlice);
    value = qAbs(value); // negative values not allowed
    if (qFuzzyCompare(d->m_value, value))
        return;

    d->m_value = value;
    emit sliceChanged();
    emit valueChanged();
}

qreal QPieSlice::value() const
{
    const Q_D(QPieSlice);
    return d->m_value;
}

void QPieSlice::setExploded(bool exploded)
{
    Q_D(QPieSlice);

    if (d->m_isExploded == exploded)
        return;

    d->m_isExploded = exploded;
    emit sliceChanged();
    emit explodedChanged();
}

bool QPieSlice::isExploded() const
{
    const Q_D(QPieSlice);
    return d->m_isExploded;
}

void QPieSlice::setExplodeDistanceFactor(qreal factor)
{
    Q_D(QPieSlice);

    if (qFuzzyCompare(d->m_explodeDistanceFactor, factor))
        return;

    d->m_explodeDistanceFactor = factor;
    emit sliceChanged();
    emit explodeDistanceFactorChanged();
}

qreal QPieSlice::explodeDistanceFactor() const
{
    const Q_D(QPieSlice);
    return d->m_explodeDistanceFactor;
}

QPieSlicePrivate::QPieSlicePrivate()
    : m_value(0)
    , m_percentage(0)
    , m_startAngle(0)
    , m_angleSpan(0)
    , m_isExploded(false)
    , m_explodeDistanceFactor(.15)
    , m_shapePath(0)
    , m_arc(0)
    , m_lineToCenter(0)
    , m_lineFromCenter(0)
    , m_series(0)
{}

QPieSlicePrivate::~QPieSlicePrivate()
{}

QPieSlicePrivate *QPieSlicePrivate::fromSlice(QPieSlice *slice)
{
    return slice->d_func();
}

void QPieSlicePrivate::setPercentage(qreal percentage)
{
    Q_Q(QPieSlice);
    if (qFuzzyCompare(m_percentage, percentage))
        return;
    m_percentage = percentage;
    emit q->percentageChanged();
}

void QPieSlicePrivate::setStartAngle(qreal angle)
{
    Q_Q(QPieSlice);
    if (qFuzzyCompare(m_startAngle, angle))
        return;
    m_startAngle = angle;
    emit q->startAngleChanged();
}

void QPieSlicePrivate::setAngleSpan(qreal span)
{
    Q_Q(QPieSlice);
    if (qFuzzyCompare(m_angleSpan, span))
        return;
    m_angleSpan = span;
    emit q->angleSpanChanged();
}


QT_END_NAMESPACE
