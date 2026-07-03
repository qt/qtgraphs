// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
// Qt-Security score:significant reason:default


#include <QtGraphs/QLogValueAxis>
#include <private/qlogvalueaxis_p.h>
#include <private/charthelpers_p.h>
#include <QtCore/QtMath>

QT_BEGIN_NAMESPACE
/*!
    \class QLogValueAxis
    \inmodule QtGraphs
    \ingroup graphs_2D
    \since 6.12
    \brief The QLogValueAxis class adds logarithmic values to a graph's axes.

    A logarithmic value axis can be set up to show an axis line with tick marks, grid lines, and shades.
    The values on the axis are drawn at the positions of tick marks.
*/
/*!
    \qmltype LogValueAxis
    \nativetype QLogValueAxis
    \inqmlmodule QtGraphs
    \ingroup graphs_qml_2D
    \inherits AbstractAxis
    \since 6.12
    \brief Adds values to a graph's axes.

    The LogValueAxis type can be set up to show an axis line with tick marks, grid lines, and shades.
    The values on the axis are drawn at the positions of tick marks.

    The following example code illustrates how to use the LogValueAxis type:
    \code
    GraphsView {
        axisX: LogValueAxis {
            min: 1
            max: 10
        }
        axisY: LogValueAxis {
            min 0.1
            max: 100
        }
        LineSeries {
            // Add a few XYPoint data...
        }
    }
    \endcode
*/

/*!
  \property QLogValueAxis::min
  \brief The minimum value on the axis.

  When setting this property, the maximum value is adjusted if necessary, to ensure that
  the range remains valid.
  The default value is 0.0
*/
/*!
  \qmlproperty real LogValueAxis::min
  The minimum value on the axis.

  When setting this property, the maximum value is adjusted if necessary, to ensure that
  the range remains valid.
  The default value is 0.0
*/

/*!
  \property QLogValueAxis::max
  \brief The maximum value on the axis.

  When setting this property, the minimum value is adjusted if necessary, to ensure that
  the range remains valid.
  The default value is 10.0
*/
/*!
  \qmlproperty real LogValueAxis::max
  The maximum value on the axis.

  When setting this property, the minimum value is adjusted if necessary, to ensure that
  the range remains valid.
  The default value is 10.0
*/

/*!
  \property QLogValueAxis::base
  This base of the logarithm.

  The value has to be greater than 0 and cannot equal 1.
  The default value is 10.0
*/
/*!
  \qmlproperty real LogValueAxis::base
  This base of the logarithm.

  The value has to be greater than 0 and cannot equal 1.
  The default value is 10.0
*/

/*!
  \property QLogValueAxis::subTickCount
  \brief The number of subticks on the axis. This indicates how many subticks are drawn
  between major lines on the graph. Labels are not drawn for subticks.
  Set the value to -1 and the number of subticks will be set equal to the
  closest integer value of the logarithm base.
  The default value is -1.
*/
/*!
  \qmlproperty int LogValueAxis::subTickCount
  The number of subticks on the axis. This indicates how many subticks are drawn
  between major lines on the graph. Labels are not drawn for subticks.
  Set the value to -1 and the number of subticks will be set equal to the
  closest integer value of the logarithm base.
  The default value is -1.
*/

/*!
  \property QLogValueAxis::tickCount
  This property holds the number of tick marks on the axis. This value is read-only.
*/
/*!
  \qmlproperty int LogValueAxis::tickCount
  This property holds the number of tick marks on the axis. This value is read-only.
*/

/*!
  \property QLogValueAxis::labelFormat
  \brief The label format of the axis.

  The format string supports the following conversion specifiers, length modifiers, and flags
  provided by \c printf() in the standard C++ library: d, i, o, x, X, f, F, e, E, g, G, c.
  Then the formatted string goes through \l{QLogValueAxis::labelPostFormat}.

  The default value is empty, in which case 'f' format is used.

  \sa QString::asprintf()
*/
/*!
  \qmlproperty string LogValueAxis::labelFormat

  The format string supports the following conversion specifiers, length modifiers, and flags
  provided by \c printf() in the standard C++ library: d, i, o, x, X, f, F, e, E, g, G, c.
  Then the formatted string goes through \l{LogValueAxis::labelPostFormat}.

  The default value is empty, in which case 'f' format is used.

  \snippet doc_src_qmlaxis.qml 2

  \sa QString::asprintf()
*/

/*!
  \property QLogValueAxis::labelPostFormat
  \brief The label post format of the axis.
  \since 6.12

  The label first goes through \l{QLogValueAxis::labelFormat} to format the value,
  then goes through this.

  The default value is "%1".

  \sa QString::arg()
*/
/*!
  \qmlproperty string LogValueAxis::labelPostFormat
  \since 6.12

  The label first goes through \l{LogValueAxis::labelFormat} to format the value,
  then goes through this.

  The default value is "%1".

  \snippet doc_src_qmlaxis.qml 2

  \sa QString::arg()
*/

/*!
  \property QLogValueAxis::labelPrecision
  \brief The maximum number of significant digits used for showing the labels. The default value is 6.
  Any negative precision is ignored in favor of the default, 6.
*/
/*!
  \qmlproperty int LogValueAxis::labelPrecision
  \brief The maximum number of significant digits used for showing the labels. The default value is 6.
  Any negative precision is ignored in favor of the default, 6.
*/

/*!
  \property QLogValueAxis::zoom
  \brief The zoom value of the axis.

  The zoom value enlarges or shrinks the axis and thus the graph without affecting intervals
  of grid and labels. The default value is 1.
*/
/*!
  \qmlproperty real LogValueAxis::zoom
  The zoom value of the axis.

  The zoom value enlarges or shrinks the axis and thus the graph without affecting intervals
  of grid and labels. The default value is 1.
*/

/*!
  \property QLogValueAxis::pan
  \brief The pan value of the axis.

  The pan value moves the center of the axis without affecting intervals
  of grid and labels. The default value is 0.
*/
/*!
  \qmlproperty real LogValueAxis::pan
  The pan value of the axis.

  The pan value moves the center of the axis without affecting intervals
  of grid and labels. The default value is 0.
*/

/*!
 \property QLogValueAxis::visualMin
 \readonly
 \brief The visual minimum value of the axis.

 This property holds a a visual minimum axis value when axis has been
 panned or zoomed. The default value is \l{QLogValueAxis::min}
*/
/*!
 \qmlproperty real LogValueAxis::visualMin
 \readonly
 The visual minimum value of the axis.

 This property holds a a visual minimum axis value when axis has been
 panned or zoomed. The default value is \l min
*/

/*!
 \property QLogValueAxis::visualMax
 \readonly
 \brief The visual maximum value of the axis.

 This property holds a a visual maximum axis value when axis has been
 panned or zoomed. The default value is \l{QLogValueAxis::max}
*/
/*!
 \qmlproperty real LogValueAxis::visualMax
 \readonly
 The visual maximum value of the axis.

 This property holds a a visual maximum axis value when axis has been
 panned or zoomed. The default value is \l max
*/

/*!
  \qmlsignal LogValueAxis::minChanged(real min)
  This signal is emitted when the minimum value of the axis changes to \a min.
*/

/*!
  \qmlsignal LogValueAxis::maxChanged(real max)
  This signal is emitted when the maximum value of the axis changes to \a max.
*/

/*!
  \qmlsignal LogValueAxis::subTickCountChanged(int subTickCount)
  This signal is emitted when the number of subticks on the axis, specified by
  \a subTickCount, changes.
*/

/*!
    \qmlsignal LogValueAxis::rangeChanged(real min, real max)
  This signal is emitted when the minimum or maximum value of the axis
  changes to \a min and \a max, respectively.
*/

/*!
  \qmlsignal LogValueAxis::labelFormatChanged(string format)
  This signal is emitted when the format of axis labels changes to \a format.
*/

/*!
  \qmlsignal LogValueAxis::labelPostFormatChanged(string format)
  \since 6.12
  This signal is emitted when the post format of axis labels changes to \a format.
*/

/*!
  \qmlsignal LogValueAxis::labelPrecisionChanged(int precision)
  This signal is emitted when the axis label precision changes to \a precision.
*/

/*!
  \qmlsignal LogValueAxis::tickCountChanged(int tickCount)
  This signal is emitted when the tick Count value, changes to
  \a tickCount.
*/

/*!
    Constructs an axis object that is a child of \a parent.
*/
QLogValueAxis::QLogValueAxis(QObject *parent)
    : QAbstractAxis(*(new QLogValueAxisPrivate), parent)
{}

/*!
    \internal
*/
QLogValueAxis::QLogValueAxis(QLogValueAxisPrivate &d, QObject *parent)
    : QAbstractAxis(d, parent)
{}

/*!
    Destroys the object.
*/
QLogValueAxis::~QLogValueAxis()
{
}

void QLogValueAxis::setMin(qreal min)
{
    Q_D(QLogValueAxis);
    setRange(min, qMax(d->m_max, min));
}

qreal QLogValueAxis::min() const
{
    Q_D(const QLogValueAxis);
    return d->m_min;
}

void QLogValueAxis::setMax(qreal max)
{
    Q_D(QLogValueAxis);
    // m_min=0 is the uninitialized default (invalid for log axis), so don't pass
    // it to setRange — just store max directly to avoid the min=0 rejection.
    if (qFuzzyCompare(d->m_min, 0.0)) {
        if (d->m_max != max) {
            d->m_max = max;
            emit maxChanged(max);
            emit update();
        }
    } else {
        setRange(qMin(d->m_min, max), max);
    }
}

qreal QLogValueAxis::max() const
{
    Q_D(const QLogValueAxis);
    return d->m_max;
}

void QLogValueAxis::setBase(qreal base)
{
    Q_D(QLogValueAxis);
    if (d->m_base == base)
        return;

    if (base <= 0) {
        qCWarning(lcAxis2D, "base has to be above 0");
        return;
    }
    if (qFuzzyCompare(base, 1.0)) {
        qCWarning(lcAxis2D, "base can not be 1");
        return;
    }

    d->m_base = base;
    emit baseChanged(base);
    emit update();
}

qreal QLogValueAxis::base() const
{
    Q_D(const QLogValueAxis);
    return d->m_base;
}

/*!
  Sets the range from \a min to \a max on the axis.
  If \a min is greater than \a max, this function returns without making any changes.
*/
void QLogValueAxis::setRange(qreal min, qreal max)
{
    Q_D(QLogValueAxis);
    d->setRange(min,max);
    emit update();
}

void QLogValueAxis::setSubTickCount(qsizetype count)
{
    Q_D(QLogValueAxis);
    if (d->m_subTickCount != count && count >= -1) {
        d->m_subTickCount = count;
        emit update();
        emit subTickCountChanged(count);
    }
}

qsizetype QLogValueAxis::subTickCount() const
{
    Q_D(const QLogValueAxis);
    return d->m_subTickCount;
}

qsizetype QLogValueAxis::tickCount() const
{
    Q_D(const QLogValueAxis);
    return d->m_tickCount;
}

void QLogValueAxis::setLabelFormat(const QString &format)
{
    Q_D(QLogValueAxis);
    d->m_format = format;
    emit update();
    emit labelFormatChanged(format);
}

QString QLogValueAxis::labelFormat() const
{
    Q_D(const QLogValueAxis);
    return d->m_format;
}

void QLogValueAxis::setLabelPostFormat(const QString &format)
{
    Q_D(QLogValueAxis);
    d->m_postFormat = format;
    emit update();
    emit labelPostFormatChanged(format);
}

QString QLogValueAxis::labelPostFormat() const
{
    Q_D(const QLogValueAxis);
    return d->m_postFormat;
}

void QLogValueAxis::setLabelPrecision(int precision)
{
    Q_D(QLogValueAxis);
    if (d->m_precision != precision) {
        d->m_precision = precision;
        emit update();
        emit labelPrecisionChanged(precision);
    } else {
        qCDebug(lcAxis2D, "QLogValueAxis::setlabelPrecision. Label decimals is already set to: %d",
            precision);
    }
}

int QLogValueAxis::labelPrecision() const
{
    Q_D(const QLogValueAxis);
    return d->m_precision;
}

void QLogValueAxis::setZoom(qreal zoom)
{
    Q_D(QLogValueAxis);
    if (d->m_zoom != zoom) {
        d->m_zoom = zoom;
        d->calculateVisualRange();
        emit update();
        emit zoomChanged(zoom);
    } else {
        qCDebug(lcAxis2D, "QLogValueAxis::setZoom. Zoom is already set to: %f",
                zoom);
    }
}

qreal QLogValueAxis::zoom() const
{
    Q_D(const QLogValueAxis);
    return d->m_zoom;
}

void QLogValueAxis::setPan(qreal pan)
{
    Q_D(QLogValueAxis);
    if (d->m_pan != pan) {
        d->m_pan = pan;
        d->calculateVisualRange();
        emit update();
        emit panChanged(pan);
    } else {
        qCDebug(lcAxis2D, "QLogValueAxis::setPan. Panning is already set to: %f",
                pan);
    }
}

qreal QLogValueAxis::pan() const
{
    Q_D(const QLogValueAxis);
    return d->m_pan;
}

qreal QLogValueAxis::visualMin() const
{
    Q_D(const QLogValueAxis);
    return d->m_visualMin;
}

qreal QLogValueAxis::visualMax() const
{
    Q_D(const QLogValueAxis);
    return d->m_visualMax;
}

/*!
  Returns the type of the axis.
*/
QAbstractAxis::AxisType QLogValueAxis::type() const
{
    return QAbstractAxis::AxisType::LogValue;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

QLogValueAxisPrivate::QLogValueAxisPrivate()
    : m_min(0)
    , m_max(10)
    , m_base(10.0)
    , m_subTickCount(-1)
    , m_format()
    , m_postFormat(QLatin1StringView("%1"))
    , m_precision(6)
    , m_tickCount(-1)
{}

QLogValueAxisPrivate::~QLogValueAxisPrivate() {}

void QLogValueAxisPrivate::setMin(const QVariant &min)
{
    Q_Q(QLogValueAxis);
    bool ok;
    qreal value = min.toReal(&ok);
    if (ok)
        q->setMin(value);
}

void QLogValueAxisPrivate::setMax(const QVariant &max)
{
    Q_Q(QLogValueAxis);
    bool ok;
    qreal value = max.toReal(&ok);
    if (ok)
        q->setMax(value);
}

void QLogValueAxisPrivate::setRange(const QVariant &min, const QVariant &max)
{
    Q_Q(QLogValueAxis);
    bool ok1;
    bool ok2;
    qreal value1 = min.toReal(&ok1);
    qreal value2 = max.toReal(&ok2);
    if (ok1 && ok2)
        q->setRange(value1, value2);
}

void QLogValueAxisPrivate::setRange(qreal min, qreal max)
{
    Q_Q(QLogValueAxis);
    bool changed = false;

    if (qFuzzyCompare(min, 0)) {
        qCWarning(lcAxis2D, "min value cannot be zero.");
        return;
    }

    if (min > max) {
        qCWarning(lcAxis2D, "min value is higher than max vaue.");
        return;
    }

    if (!isValidValue(min, max)) {
        qCWarning(lcAxis2D, "attempting to set invalid range for value axis: [%f - %f]", min, max);
        return;
    }

    if (m_min != min) {
        m_min = min;
        changed = true;
        emit q->minChanged(min);
    }

    if (m_max != max) {
        m_max = max;
        changed = true;
        emit q->maxChanged(max);
    }

    if (changed) {
        calculateVisualRange();
        emit q->rangeChanged(min, max);
    }
}

void QLogValueAxisPrivate::calculateVisualRange()
{
    Q_Q(QLogValueAxis);
    qreal logBase = log(m_base);
    qreal logMin = log(m_min) / logBase;
    qreal logMax = log(m_max) / logBase;

    qreal diff = logMax - logMin;

    qreal center = diff / 2.0f + logMin + m_pan;
    diff /= m_zoom;
    logMin = center - diff / 2.0f;
    logMax = center + diff / 2.0f;

    qreal min = pow(m_base, logMin);
    qreal max = pow(m_base, logMax);

    if (!qFuzzyCompare(m_visualMin, min)) {
        m_visualMin = min;
        emit q->visualMinChanged(min);
    }
    if (!qFuzzyCompare(m_visualMax, max)) {
        m_visualMax = max;
        emit q->visualMaxChanged(max);
    }

    int tickCount = qFloor(logMax) - qCeil(logMin) + 1;
    if (m_tickCount != tickCount) {
        m_tickCount = tickCount;
        emit q->tickCountChanged(tickCount);
    }
}

QT_END_NAMESPACE

#include "moc_qlogvalueaxis.cpp"
