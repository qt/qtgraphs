// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtCore/qdebug.h>
#include "qabstract3daxis_p.h"

QT_BEGIN_NAMESPACE

/*!
 * \class QAbstract3DAxis
 * \inmodule QtGraphs
 * \ingroup graphs_3D
 * \brief The QAbstract3DAxis class is a base class for the axes of a 3D graph.
 *
 * This class specifies the enumerations, properties, and functions shared by
 * graph axes. It should not be used directly, but one of its subclasses should
 * be used instead.
 *
 * \sa QCategory3DAxis, QValue3DAxis
 */

/*!
 * \qmltype AbstractAxis3D
 * \inqmlmodule QtGraphs
 * \ingroup graphs_qml_3D
 * \instantiates QAbstract3DAxis
 * \brief A base type for the axes of a 3D graph.
 *
 * This type is uncreatable, but contains properties that are exposed via
 * subtypes.
 *
 * For AbstractAxis3D enums, see \l QAbstract3DAxis::AxisOrientation and
 * \l{QAbstract3DAxis::AxisType}.
 */

/*!
 * \qmlproperty string AbstractAxis3D::title
 * The title for the axis.
 *
 * \sa titleVisible, titleFixed
 */

/*!
 * \qmlproperty list AbstractAxis3D::labels
 * The labels for the axis.
 * \note Setting this property for ValueAxis3D does nothing, as it generates
 * labels automatically.
 */

/*!
 * \qmlproperty AbstractAxis3D.AxisOrientation AbstractAxis3D::orientation
 * The orientation of the axis.
 */

/*!
 * \qmlproperty AbstractAxis3D.AxisType AbstractAxis3D::type
 * The type of the axis.
 */

/*!
 * \qmlproperty real AbstractAxis3D::min
 *
 * The minimum value on the axis.
 * When setting this property, the maximum value is adjusted if necessary, to
 * ensure that the range remains valid.
 */

/*!
 * \qmlproperty real AbstractAxis3D::max
 *
 * The maximum value on the axis.
 * When setting this property, the minimum value is adjusted if necessary, to
 * ensure that the range remains valid.
 */

/*!
 * \qmlproperty bool AbstractAxis3D::autoAdjustRange
 *
 * Defines whether the axis will automatically adjust the range so that all data
 * fits in it.
 */

/*!
 * \qmlproperty real AbstractAxis3D::labelAutoRotation
 *
 * The maximum angle the labels can autorotate when the camera angle changes.
 * The angle can be between 0 and 90, inclusive. The default value is 0.
 * If the value is 0, axis labels do not automatically rotate.
 * If the value is greater than zero, labels attempt to orient themselves toward
 * the camera, up to the specified angle.
 */

/*!
 * \qmlproperty bool AbstractAxis3D::titleVisible
 *
 * Defines whether the axis title is visible in the primary graph view.
 *
 * The default value is \c{false}.
 *
 * \sa title, titleFixed
 */

/*!
 * \qmlproperty bool AbstractAxis3D::titleFixed
 *
 * The rotation of axis titles.
 *
 * If \c{true}, axis titles in the primary graph view will be rotated towards
 * the camera similarly to the axis labels. If \c{false}, axis titles are only
 * rotated around their axis but are not otherwise oriented towards the camera.
 * This property does not have any effect if the labelAutoRotation property
 * value is zero.
 * Default value is \c{true}.
 *
 * \sa labelAutoRotation, title, titleVisible
 */

/*!
 * \enum QAbstract3DAxis::AxisOrientation
 *
 * The orientation of the axis object.
 *
 * \value None
 * \value X
 * \value Y
 * \value Z
 */

/*!
 * \enum QAbstract3DAxis::AxisType
 *
 * The type of the axis object.
 *
 * \value None
 * \value Category
 * \value Value
 */

/*!
 * \internal
 */
QAbstract3DAxis::QAbstract3DAxis(QAbstract3DAxisPrivate &d, QObject *parent)
    : QObject(d, parent)

{}

/*!
 * Destroys QAbstract3DAxis.
 */
QAbstract3DAxis::~QAbstract3DAxis() {}

/*!
 * \property QAbstract3DAxis::orientation
 *
 * \brief The orientation of the axis.
 *
 * The value is one of AxisOrientation values.
 */
QAbstract3DAxis::AxisOrientation QAbstract3DAxis::orientation() const
{
    const Q_D(QAbstract3DAxis);
    return d->m_orientation;
}

/*!
 * \property QAbstract3DAxis::type
 *
 * \brief The type of the axis.
 *
 * The value is one of AxisType values.
 */
QAbstract3DAxis::AxisType QAbstract3DAxis::type() const
{
    const Q_D(QAbstract3DAxis);
    return d->m_type;
}

/*!
 * \property QAbstract3DAxis::title
 *
 * \brief The title for the axis.
 *
 * \sa titleVisible, titleFixed
 */
void QAbstract3DAxis::setTitle(const QString &title)
{
    Q_D(QAbstract3DAxis);
    if (d->m_title != title) {
        d->m_title = title;
        emit titleChanged(title);
    }
}

QString QAbstract3DAxis::title() const
{
    const Q_D(QAbstract3DAxis);
    return d->m_title;
}

/*!
 * \property QAbstract3DAxis::labels
 *
 * \brief The labels for the axis.
 * \note Setting this property for QValue3DAxis does nothing, as it generates
 * labels automatically.
 */
void QAbstract3DAxis::setLabels(const QStringList &labels)
{
    Q_UNUSED(labels);
}

QStringList QAbstract3DAxis::labels() const
{
    const Q_D(QAbstract3DAxis);
    const_cast<QAbstract3DAxisPrivate *>(d)->updateLabels();
    return const_cast<QAbstract3DAxisPrivate *>(d)->m_labels;
}

/*!
 * Sets the value range of the axis from \a min to \a max.
 * When setting the range, the maximum value is adjusted if necessary, to ensure
 * that the range remains valid.
 * \note For QCategory3DAxis, specifies the index range of rows or columns to
 * show.
 */
void QAbstract3DAxis::setRange(float min, float max)
{
    Q_D(QAbstract3DAxis);
    d->setRange(min, max);
    setAutoAdjustRange(false);
}

/*!
 * \property QAbstract3DAxis::labelAutoRotation
 *
 * \brief The maximum angle the labels can autorotate when the camera angle
 * changes.
 *
 * The angle can be between 0 and 90, inclusive. The default value is 0.
 * If the value is 0, axis labels do not automatically rotate.
 * If the value is greater than zero, labels attempt to orient themselves toward
 * the camera, up to the specified angle.
 */
void QAbstract3DAxis::setLabelAutoRotation(float angle)
{
    Q_D(QAbstract3DAxis);
    if (angle < 0.0f)
        angle = 0.0f;
    if (angle > 90.0f)
        angle = 90.0f;
    if (d->m_labelAutoRotation != angle) {
        d->m_labelAutoRotation = angle;
        emit labelAutoRotationChanged(angle);
    }
}

float QAbstract3DAxis::labelAutoRotation() const
{
    const Q_D(QAbstract3DAxis);
    return d->m_labelAutoRotation;
}

/*!
 * \property QAbstract3DAxis::titleVisible
 *
 * \brief Whether the axis title is visible in the primary graph view.
 *
 * The default value is \c{false}.
 *
 * \sa title, titleFixed
 */
void QAbstract3DAxis::setTitleVisible(bool visible)
{
    Q_D(QAbstract3DAxis);
    if (d->m_titleVisible != visible) {
        d->m_titleVisible = visible;
        emit titleVisibilityChanged(visible);
    }
}

bool QAbstract3DAxis::isTitleVisible() const
{
    const Q_D(QAbstract3DAxis);
    return d->m_titleVisible;
}

/*!
 * \property QAbstract3DAxis::titleFixed
 *
 * \brief The rotation of the axis titles.
 *
 * If \c{true}, axis titles in the primary graph view will be rotated towards
 * the camera similarly to the axis labels. If \c{false}, axis titles are only
 * rotated around their axis but are not otherwise oriented towards the camera.
 * This property does not have any effect if the labelAutoRotation property
 * value is zero.
 * Default value is \c{true}.
 *
 * \sa labelAutoRotation, title, titleVisible
 */
void QAbstract3DAxis::setTitleFixed(bool fixed)
{
    Q_D(QAbstract3DAxis);
    if (d->m_titleFixed != fixed) {
        d->m_titleFixed = fixed;
        emit titleFixedChanged(fixed);
    }
}

bool QAbstract3DAxis::isTitleFixed() const
{
    const Q_D(QAbstract3DAxis);
    return d->m_titleFixed;
}

/*!
 * \property QAbstract3DAxis::min
 *
 * \brief The minimum value on the axis.
 *
 * When setting this property, the maximum value is adjusted if necessary, to
 * ensure that the range remains valid.
 * \note For QCategory3DAxis, specifies the index of the first row or column to
 * show.
 */
void QAbstract3DAxis::setMin(float min)
{
    Q_D(QAbstract3DAxis);
    d->setMin(min);
    setAutoAdjustRange(false);
}

/*!
 * \property QAbstract3DAxis::max
 *
 * \brief The maximum value on the axis.
 *
 * When setting this property, the minimum value is adjusted if necessary, to
 * ensure that the range remains valid.
 * \note For QCategory3DAxis, specifies the index of the last row or column to
 * show.
 */
void QAbstract3DAxis::setMax(float max)
{
    Q_D(QAbstract3DAxis);
    d->setMax(max);
    setAutoAdjustRange(false);
}

float QAbstract3DAxis::min() const
{
    const Q_D(QAbstract3DAxis);
    return d->m_min;
}

float QAbstract3DAxis::max() const
{
    const Q_D(QAbstract3DAxis);
    return d->m_max;
}

/*!
 * \property QAbstract3DAxis::autoAdjustRange
 *
 * \brief Whether the axis will automatically adjust the range so that all data
 * fits in it.
 *
 * \sa setRange(), setMin(), setMax()
 */
void QAbstract3DAxis::setAutoAdjustRange(bool autoAdjust)
{
    Q_D(QAbstract3DAxis);
    if (d->m_autoAdjust != autoAdjust) {
        d->m_autoAdjust = autoAdjust;
        emit autoAdjustRangeChanged(autoAdjust);
    }
}

bool QAbstract3DAxis::isAutoAdjustRange() const
{
    const Q_D(QAbstract3DAxis);
    return d->m_autoAdjust;
}

/*!
 * \fn QAbstract3DAxis::rangeChanged(float min, float max)
 *
 * Emits the minimum and maximum values of the range, \a min and \a max, when
 * the range changes.
 */

// QAbstract3DAxisPrivate
QAbstract3DAxisPrivate::QAbstract3DAxisPrivate(QAbstract3DAxis::AxisType type)
    : m_orientation(QAbstract3DAxis::AxisOrientation::None)
    , m_type(type)
    , m_isDefaultAxis(false)
    , m_min(0.0f)
    , m_max(10.0f)
    , m_autoAdjust(true)
    , m_labelAutoRotation(0.0f)
    , m_titleVisible(false)
    , m_titleFixed(true)
{}

QAbstract3DAxisPrivate::~QAbstract3DAxisPrivate() {}

void QAbstract3DAxisPrivate::setOrientation(QAbstract3DAxis::AxisOrientation orientation)
{
    Q_Q(QAbstract3DAxis);
    if (m_orientation == QAbstract3DAxis::AxisOrientation::None) {
        m_orientation = orientation;
        emit q->orientationChanged(orientation);
    } else {
        Q_ASSERT("Attempted to reset axis orientation.");
    }
}

void QAbstract3DAxisPrivate::updateLabels()
{
    // Default implementation does nothing
}

void QAbstract3DAxisPrivate::setRange(float min, float max, bool suppressWarnings)
{
    Q_Q(QAbstract3DAxis);
    bool adjusted = false;
    if (!allowNegatives()) {
        if (allowZero()) {
            if (min < 0.0f) {
                min = 0.0f;
                adjusted = true;
            }
            if (max < 0.0f) {
                max = 0.0f;
                adjusted = true;
            }
        } else {
            if (min <= 0.0f) {
                min = 1.0f;
                adjusted = true;
            }
            if (max <= 0.0f) {
                max = 1.0f;
                adjusted = true;
            }
        }
    }
    // If min >= max, we adjust ranges so that
    // m_max becomes (min + 1.0f)
    // as axes need some kind of valid range.
    bool minDirty = false;
    bool maxDirty = false;
    if (m_min != min) {
        m_min = min;
        minDirty = true;
    }
    if (m_max != max || min > max || (!allowMinMaxSame() && min == max)) {
        if (min > max || (!allowMinMaxSame() && min == max)) {
            m_max = min + 1.0f;
            adjusted = true;
        } else {
            m_max = max;
        }
        maxDirty = true;
    }

    if (minDirty || maxDirty) {
        if (adjusted && !suppressWarnings) {
            qWarning() << "Warning: Tried to set invalid range for axis."
                          " Range automatically adjusted to a valid one:"
                       << min << "-" << max << "-->" << m_min << "-" << m_max;
        }
        emit q->rangeChanged(m_min, m_max);
    }

    if (minDirty)
        emit q->minChanged(m_min);
    if (maxDirty)
        emit q->maxChanged(m_max);
}

void QAbstract3DAxisPrivate::setMin(float min)
{
    Q_Q(QAbstract3DAxis);
    if (!allowNegatives()) {
        if (allowZero()) {
            if (min < 0.0f) {
                min = 0.0f;
                qWarning() << "Warning: Tried to set negative minimum for an axis that only"
                              "supports positive values and zero:"
                           << min;
            }
        } else {
            if (min <= 0.0f) {
                min = 1.0f;
                qWarning() << "Warning: Tried to set negative or zero minimum for an "
                              "axis that only"
                              "supports positive values:"
                           << min;
            }
        }
    }

    if (m_min != min) {
        bool maxChanged = false;
        if (min > m_max || (!allowMinMaxSame() && min == m_max)) {
            float oldMax = m_max;
            m_max = min + 1.0f;
            qWarning() << "Warning: Tried to set minimum to equal or larger than maximum for"
                          " value axis. Maximum automatically adjusted to a valid one:"
                       << oldMax << "-->" << m_max;
            maxChanged = true;
        }
        m_min = min;

        emit q->rangeChanged(m_min, m_max);
        emit q->minChanged(m_min);
        if (maxChanged)
            emit q->maxChanged(m_max);
    }
}

void QAbstract3DAxisPrivate::setMax(float max)
{
    Q_Q(QAbstract3DAxis);
    if (!allowNegatives()) {
        if (allowZero()) {
            if (max < 0.0f) {
                max = 0.0f;
                qWarning() << "Warning: Tried to set negative maximum for an axis that only"
                              "supports positive values and zero:"
                           << max;
            }
        } else {
            if (max <= 0.0f) {
                max = 1.0f;
                qWarning() << "Warning: Tried to set negative or zero maximum for an "
                              "axis that only"
                              "supports positive values:"
                           << max;
            }
        }
    }

    if (m_max != max) {
        bool minChanged = false;
        if (m_min > max || (!allowMinMaxSame() && m_min == max)) {
            float oldMin = m_min;
            m_min = max - 1.0f;
            if (!allowNegatives() && m_min < 0.0f) {
                if (allowZero())
                    m_min = 0.0f;
                else
                    m_min = max / 2.0f; // Need some positive value smaller than max

                if (!allowMinMaxSame() && max == 0.0f) {
                    m_min = oldMin;
                    qWarning() << "Unable to set maximum value to zero.";
                    return;
                }
            }
            qWarning() << "Warning: Tried to set maximum to equal or smaller than minimum "
                          "for"
                          " value axis. Minimum automatically adjusted to a valid one:"
                       << oldMin << "-->" << m_min;
            minChanged = true;
        }
        m_max = max;
        emit q->rangeChanged(m_min, m_max);
        emit q->maxChanged(m_max);
        if (minChanged)
            emit q->minChanged(m_min);
    }
}

QT_END_NAMESPACE
