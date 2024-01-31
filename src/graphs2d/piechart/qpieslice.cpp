// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <private/qpieslice_p.h>
#include <QtQuick/private/qquicktext_p.h>
#include <QtQuickShapes/private/qquickshape_p.h>

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

    In that case, \l{PieSeries::at()}{PieSeries.at()} or \l {PieSeries::find()}
    {PieSeries.find()} can be used to access the properties of an individual PieSlice instance.
    \sa PieSeries
*/

/*!
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

    \sa labelVisible, labelFont, labelArmLengthFactor
*/
/*!
    \qmlproperty string PieSlice::label
    The label of the slice.
    \note The string can be HTML formatted.
*/

/*!
    \qmlsignal PieSlice::labelChanged()
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
    \qmlsignal PieSlice::valueChanged()
    This signal is emitted when the slice value changes.
    \sa value
*/

/*!
    \property QPieSlice::labelVisible
    \brief The visibility of the slice label. By default, the label is not visible.
    \sa label, labelFont, labelArmLengthFactor
*/
/*!
    \qmlproperty bool PieSlice::labelVisible
    The visibility of the slice label. By default, the label is not visible.
*/

/*!
    \qmlsignal PieSlice::labelVisibleChanged()
    This signal is emitted when the visibility of the slice label changes.
    \sa labelVisible
*/

/*!
    \property QPieSlice::labelColor
    \brief The color used to draw the slice label.
*/
/*!
    \qmlproperty color PieSlice::labelColor
    The color used to draw the slice label.
*/

/*!
    \qmlsignal PieSlice::labelColorChanged()
    This signal is emitted when the slice label color changes.
    \sa labelColor
*/

/*!
    \property QPieSlice::labelFont
    \brief The font used for drawing the label text.
    \sa label, labelVisible, labelArmLengthFactor
*/

/*!
    \qmlsignal PieSlice::labelFontChanged()
    This signal is emitted when the label font of the slice changes.
    \sa labelFont
*/

/*!
    \qmlproperty font PieSlice::labelFont

    The font used for the slice label.

    For more information, see \l [QML]{font}.

    \sa labelVisible, labelPosition
*/
/*!
    \qmlsignal PieSlice::labelFontChanged()
    This signal is emitted when the label font changes.
    \sa labelFont
*/

/*!
    \property QPieSlice::labelPosition
    \brief The position of the slice label.
    \sa label, labelVisible
*/
/*!
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
/*!
    \qmlsignal PieSlice::labelPositionChanged()
    This signal is emitted when the label position changes.
    \sa labelPosition
*/

/*!
    \property QPieSlice::labelArmLengthFactor
    \brief The length of the label arm.
    The factor is relative to the pie radius. For example:
    \list
        \li 1.0 means that the length is the same as the radius.
        \li 0.5 means that the length is half of the radius.
    \endlist
    By default, the arm length is 0.15
    \sa label, labelVisible, labelFont
*/
/*!
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
    \qmlsignal PieSlice::labelArmLengthFactorChanged()
    This signal is emitted when the label arm length factor changes.
    \sa labelArmLengthFactor
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
/*!
    \qmlsignal PieSlice::explodedChanged()
    This signal is emitted when the exploded property changes.
    \sa exploded
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
    \qmlsignal PieSlice::explodeDistanceFactorChanged()
    This signal is emitted when the explode distance factor changes.
    \sa explodeDistanceFactor
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
    \qmlsignal PieSlice::percentageChanged()
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
    \qmlsignal PieSlice::startAngleChanged()
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
    \qmlsignal PieSlice::angleSpanChanged()
    This signal is emitted when the angle span of the slice changes.
    \sa angleSpan
*/

/*!
    Constructs an empty slice with the parent \a parent.
    \sa QPieSeries::append(), QPieSeries::insert()
*/
QPieSlice::QPieSlice(QObject *parent)
    : QObject(*(new QPieSlicePrivate), parent)
{
}

/*!
    Constructs an empty slice with the specified \a value, \a label, and \a parent.
    \sa QPieSeries::append(), QPieSeries::insert()
*/
QPieSlice::QPieSlice(const QString &label, qreal value, QObject *parent)
    : QObject(*(new QPieSlicePrivate), parent)
{
    setLabel(label);
    setValue(value);
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
    d->m_labelItem->setText(label);
    emit labelChanged();
}

QString QPieSlice::label() const
{
    const Q_D(QPieSlice);
    return d->m_labelText;
}

void QPieSlice::setLabelVisible(bool visible)
{
    Q_D(QPieSlice);
    if (d->m_isLabelVisible == visible)
        return;

    d->setLabelVisible(visible);
    emit labelVisibleChanged();
}

bool QPieSlice::isLabelVisible() const
{
    const Q_D(QPieSlice);
    return d->m_isLabelVisible;
}

void QPieSlice::setLabelPosition(LabelPosition position)
{
    Q_D(QPieSlice);
    if (d->m_labelPosition == position)
        return;

    d->setLabelPosition(position);
}

QPieSlice::LabelPosition QPieSlice::labelPosition()
{
    Q_D(QPieSlice);
    return d->m_labelPosition;
}

void QPieSlice::setLabelColor(QColor color)
{
    Q_D(QPieSlice);
    if (d->m_labelColor == color)
        return;

    d->m_labelColor = color;
    emit labelColorChanged();
}

QColor QPieSlice::labelColor()
{
    Q_D(QPieSlice);
    return d->m_labelColor;
}

void QPieSlice::setLabelFont(const QFont &font)
{
    Q_D(QPieSlice);
    d->m_labelFont = font;
    d->m_labelItem->setFont(font);
    emit labelFontChanged();
}

QFont QPieSlice::labelFont() const
{
    const Q_D(QPieSlice);
    return d->m_labelFont;
}

void QPieSlice::setLabelArmLengthFactor(qreal factor)
{
    Q_D(QPieSlice);

    if (qFuzzyCompare(d->m_labelArmLengthFactor, factor))
        return;

    d->m_labelArmLengthFactor = factor;
}

qreal QPieSlice::labelArmLengthFactor() const
{
    const Q_D(QPieSlice);
    return d->m_labelArmLengthFactor;
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
    : m_isLabelVisible(false)
    , m_labelPosition(QPieSlice::LabelOutside)
    , m_labelArmLengthFactor(.15)
    , m_value(0)
    , m_percentage(0)
    , m_startAngle(0)
    , m_angleSpan(0)
    , m_isExploded(false)
    , m_explodeDistanceFactor(.15)
    , m_labelDirty(false)
    , m_shapePath(new QQuickShapePath)
    , m_arc(new QQuickPathArc(m_shapePath))
    , m_lineToCenter(new QQuickPathLine(m_shapePath))
    , m_lineFromCenter(new QQuickPathLine(m_shapePath))
    , m_labelItem(new QQuickText)
    , m_labelShape(new QQuickShape)
    , m_labelPath(new QQuickShapePath)
    , m_labelArm(new QQuickPathLine(m_labelPath))
    , m_labelUnderline(new QQuickPathLine(m_labelPath))
    , m_series(0)
{
    m_labelItem->setColor(Qt::white);
    m_labelItem->setVisible(m_isLabelVisible);

    auto pathElements = m_shapePath->pathElements();
    pathElements.append(&pathElements, m_arc);
    pathElements.append(&pathElements, m_lineToCenter);
    pathElements.append(&pathElements, m_lineFromCenter);

    m_labelShape->setVisible(m_isLabelVisible);
    m_labelPath->setParent(m_labelShape);
    auto data = m_labelShape->data();
    data.append(&data, m_labelPath);
    m_labelPath->setFillColor(Qt::transparent);
    auto labelElements = m_labelPath->pathElements();
    labelElements.append(&labelElements, m_labelArm);
    labelElements.append(&labelElements, m_labelUnderline);
}

QPieSlicePrivate::~QPieSlicePrivate()
{}

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

void QPieSlicePrivate::setLabelVisible(bool visible)
{
    m_isLabelVisible = visible;
    m_labelItem->setVisible(visible);
    if (m_labelPosition == QPieSlice::LabelOutside)
        m_labelShape->setVisible(visible);
}

void QPieSlicePrivate::setLabelPosition(QPieSlice::LabelPosition position)
{
    m_labelPosition = position;

    if (position == QPieSlice::LabelOutside) {
        m_labelShape->setVisible(m_isLabelVisible);
        qreal radian = qDegreesToRadians(m_startAngle + (m_angleSpan * .5));
        QQuickText *labelItem = m_labelItem;
        qreal height = labelItem->height();
        qreal labelWidth = radian > M_PI ? -labelItem->width() : labelItem->width();
        if (labelWidth > 0)
            labelItem->setX(m_labelArm->x());
        else
            labelItem->setX(m_labelArm->x() + labelWidth);
        labelItem->setY(m_labelArm->y() - height);
        labelItem->setRotation(0);

        m_labelUnderline->setX(m_labelArm->x() + labelWidth);
        m_labelUnderline->setY(m_labelArm->y());
    } else {
        m_labelShape->setVisible(false);
        qreal centerX = (m_shapePath->startX() + m_arc->x() + m_lineToCenter->x()) / 3.0;
        qreal centerY = (m_shapePath->startY() + m_arc->y() + m_lineToCenter->y()) / 3.0;
        QQuickText *labelItem = m_labelItem;
        centerX -= labelItem->width() * .5;
        centerY -= labelItem->height() * .5;
        labelItem->setPosition(QPointF(centerX, centerY));

        if (position == QPieSlice::LabelInsideHorizontal)
            labelItem->setRotation(0);
        else if (position == QPieSlice::LabelInsideNormal)
            labelItem->setRotation(m_startAngle + (m_angleSpan * .5));
        else if (position == QPieSlice::LabelInsideTangential)
            labelItem->setRotation(m_startAngle + (m_angleSpan * .5) - 90);
    }
}

QT_END_NAMESPACE
