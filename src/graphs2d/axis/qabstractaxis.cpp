// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGraphs2D/QAbstractAxis>
#include <private/qabstractaxis_p.h>

QT_BEGIN_NAMESPACE

/*!
    \class QAbstractAxis
    \inmodule QtGraphs
    \brief The QAbstractAxis class is a base class used for specialized axis classes.

    Each series can be bound to one or more horizontal and vertical axes, but mixing axis types
    that would result in different domains is not supported, such as specifying
    QValueAxis and QLogValueAxis on the same orientation.

    The properties and visibility of various axis elements, such as axis line, title, labels,
    and grid lines, can be individually controlled.
*/
/*!
    \qmltype AbstractAxis
    \instantiates QAbstractAxis
    \inqmlmodule QtGraphs
    \ingroup graphs_qml

    \brief A base type used for specialized axis types.

    Each series can be bound to only one horizontal and vertical axis.

    The properties and visibility of various axis elements, such as axis line, title, labels,
    and grid lines, can be individually controlled.
*/

/*!
    \enum QAbstractAxis::AxisType

    This enum type specifies the type of the axis object.

    \value AxisTypeNoAxis
    \value AxisTypeValue
    \value AxisTypeBarCategory
    \value AxisTypeCategory
    \value AxisTypeDateTime
    \value AxisTypeLogValue
    \value AxisTypeColor
*/

/*!
  \internal TODO: Causes build errors, find out why
  \fn void QAbstractAxis::type() const
  Returns the type of the axis.
*/

/*!
  \property QAbstractAxis::lineVisible
  \brief The visibility of the axis line.
*/
/*!
  \qmlproperty bool AbstractAxis::lineVisible
  The visibility of the axis line.
*/

/*!
  \property QAbstractAxis::labelsVisible
  \brief Whether axis labels are visible.
*/
/*!
  \qmlproperty bool AbstractAxis::labelsVisible
  The visibility of axis labels.
*/

/*!
  \property QAbstractAxis::labelsColor
  \brief The color used to draw the labels.
*/

/*!
  \property QAbstractAxis::visible
  \brief The visibility of the axis.
*/
/*!
  \qmlproperty bool AbstractAxis::visible
  The visibility of the axis.
*/

/*!
  \property QAbstractAxis::gridVisible
  \brief The visibility of the grid lines.
*/
/*!
  \qmlproperty bool AbstractAxis::gridVisible
  The visibility of the grid lines.
*/

/*!
  \property QAbstractAxis::minorGridVisible
  \brief The visibility of the minor grid lines.

  Applies only to axes that support minor grid lines.
*/
/*!
  \qmlproperty bool AbstractAxis::minorGridVisible
  The visibility of the minor grid lines. Applies only to axes that support
  minor grid lines.
*/

/*!
  \internal TODO: Causes build errors, find out why
  \property QAbstractAxis::color
  \brief The color of the axis and tick marks.
*/
/*!
  \qmlproperty color AbstractAxis::color
  The color of the axis and tick marks.
*/

/*!
  \property QAbstractAxis::gridLineColor
  \brief The color of the grid line.
*/

/*!
  \qmlproperty color AbstractAxis::gridLineColor
  The color of the grid line.
*/

/*!
  \property QAbstractAxis::minorGridLineColor
  \brief The color of the minor grid line.

  Applies only to axes that support minor grid lines.
*/

/*!
  \qmlproperty color AbstractAxis::minorGridLineColor
  The color of the minor grid line.

  Applies only to axes that support minor grid lines.
*/

/*!
  \property QAbstractAxis::labelsFont
  \brief The font of the axis labels.
*/

/*!
  \qmlproperty font AbstractAxis::labelsFont
  The font of the axis labels.

  For more information, see \l [QML]{font}.
*/

/*!
  \qmlproperty color AbstractAxis::labelsColor
  The color of the axis labels.
*/

/*!
  \property QAbstractAxis::labelsAngle
  \brief The angle of the axis labels in degrees.
*/
/*!
  \qmlproperty int AbstractAxis::labelsAngle
  The angle of the axis labels in degrees.
*/

/*!
  \property QAbstractAxis::labelsTruncated
  \brief Returns \c true if at least one label on the axis is truncated.

  Returned value will not be accurate before the axis is shown.
*/
/*!
  \qmlproperty int AbstractAxis::labelsTruncated
  Returns \c true if at least one label on the axis is truncated.

  Returned value will not be accurate before the axis is shown.
  \readonly
*/

/*!
  \property QAbstractAxis::truncateLabels
  \brief The truncation state of labels.

  Indicates whether labels should be truncated if there is no enough space for full text.
  It is equal to \c true by default.
*/
/*!
  \qmlproperty int AbstractAxis::truncateLabels
  The truncation state of labels.

  Indicates whether labels should be truncated if there is no enough space for full text.
  It is equal to \c true by default.
*/

/*!
  \property QAbstractAxis::titleVisible
  \brief The visibility of the axis title.

    By default, the value is \c true.
*/
/*!
  \qmlproperty bool AbstractAxis::titleVisible
  The visibility of the axis title. By default, the value is \c true.
*/

/*!
  \property QAbstractAxis::titleText
  \brief The title of the axis.

  Empty by default. Axis titles support HTML formatting.
*/
/*!
  \qmlproperty string AbstractAxis::titleText
  The title of the axis. Empty by default. Axis titles support HTML formatting.
*/

/*!
  \property QAbstractAxis::titleColor
  \brief The color used to draw the title text.
*/

/*!
  \property QAbstractAxis::titleFont
  \brief The font of the title of the axis.
*/
/*!
  \qmlproperty font AbstractAxis::titleFont
  The font of the title of the axis.
*/

/*!
  \property QAbstractAxis::orientation
  \brief The orientation of the axis.

  Fixed to either Qt::Horizontal or Qt::Vertical when the axis is added to a chart.
*/
/*!
  \qmlproperty Qt.Orientation AbstractAxis::orientation
  The orientation of the axis. Fixed to either \l {Qt::Horizontal}{Qt.Horizontal}
  or \l{Qt::Vertical}{Qt.Vertical} when the axis is set to a series.
*/

/*!
  \property QAbstractAxis::alignment
  \brief The alignment of the axis.

  Can be Qt::AlignLeft, Qt::AlignRight, Qt::AlignBottom, or Qt::AlignTop.
*/
/*!
  \qmlproperty alignment AbstractAxis::alignment
  The alignment of the axis. Can be \l{Qt::AlignLeft}{Qt.AlignLeft},
  \l{Qt::AlignRight}{Qt.AlignRight}, \l{Qt::AlignBottom}{Qt.AlignBottom}, or
  \l{Qt::AlignTop}{Qt.AlignTop}.
*/

/*!
  \property QAbstractAxis::reverse
  \brief Whether a reverse axis is used.

  By default, the value is \c false.

  The reverse axis is supported with a line, spline, and scatter series, as well as an area series
  with a cartesian chart. All axes of the same orientation attached to the same series must be
  reversed if one is reversed or the behavior is undefined.
*/
/*!
  \qmlproperty alignment AbstractAxis::reverse
  Defines whether a reverse axis is used. By default, the value is \c false.

  The reverse axis is supported with a line, spline, and scatter series, as well as an area series
  with a cartesian chart. All axes of the same orientation attached to the same series must be
  reversed if one is reversed or the behavior is undefined.
*/

/*!
  \internal TODO: Causes build errors, find out why
  \fn void QAbstractAxis::visibleChanged(bool visible)
  This signal is emitted when the visibility of the axis changes to \a visible.
*/

/*!
  \internal TODO: Causes build errors, find out why
  \fn void QAbstractAxis::lineVisibleChanged(bool visible)
  This signal is emitted when the visibility of the axis line changes to \a visible.
*/

/*!
  \internal TODO: Causes build errors, find out why
  \fn void QAbstractAxis::labelsVisibleChanged(bool visible)
  This signal is emitted when the visibility of the labels of the axis changes to \a visible.
*/

/*!
  \internal TODO: Causes build errors, find out why
  \fn void QAbstractAxis::labelsFontChanged(const QFont& font)
  This signal is emitted when the font of the axis labels changes to \a font.
*/

/*!
  \internal TODO: Causes build errors, find out why
  \fn void QAbstractAxis::labelsColorChanged(const QColor& color)
  This signal is emitted when the color used to draw the axis labels changes to \a color.
*/

/*!
  \internal TODO: Causes build errors, find out why
  \fn void QAbstractAxis::labelsAngleChanged(int angle)
  This signal is emitted when the angle of the axis labels changes to \a angle.
*/

/*!
  \internal TODO: Causes build errors, find out why
  \fn void QAbstractAxis::labelsEditableChanged(bool editable)
  This signal is emitted when the \a editable state of the label changes.
*/

/*!
  \internal TODO: Causes build errors, find out why
  \fn void QAbstractAxis::labelsTruncatedChanged(bool labelsTruncated)
  This signal is emitted in two cases; when the axis changes from having one or more truncated
  labels to having no truncated labels, and when the axis changes from having no truncated
  labels to having one or more truncated labels. Current state is identified by \a labelsTruncated.
*/

/*!
  \internal TODO: Causes build errors, find out why
  \fn void QAbstractAxis::truncateLabelsChanged(bool truncateLabels)
  This signal is emitted when the truncation of the labels changes to \a truncateLabels.
*/

/*!
  \internal TODO: Causes build errors, find out why
  \fn void QAbstractAxis::gridVisibleChanged(bool visible)
  This signal is emitted when the visibility of the grid lines of the axis changes to \a visible.
*/

/*!
  \internal TODO: Causes build errors, find out why
  \fn void QAbstractAxis::minorGridVisibleChanged(bool visible)
  This signal is emitted when the visibility of the minor grid lines of the axis
  changes to \a visible.
*/

/*!
  \internal TODO: Causes build errors, find out why
  \fn void QAbstractAxis::gridLineColorChanged(const QColor &color)
  This signal is emitted when the color of the grid line changes to \a color.
*/

/*!
  \internal TODO: Causes build errors, find out why
  \fn void QAbstractAxis::minorGridLineColorChanged(const QColor &color)
  This signal is emitted when the color of the minor grid line changes
  to \a color.
*/

/*!
  \internal TODO: Causes build errors, find out why
  \fn void QAbstractAxis::colorChanged(QColor color)
  This signal is emitted when the color of the axis changes to \a color.
*/

/*!
  \internal TODO: Causes build errors, find out why
  \fn void QAbstractAxis::labelsColorChanged(QColor color)
  This signal is emitted when the color of the axis labels changes to \a color.
*/

/*!
  \internal TODO: Causes build errors, find out why
  \fn void QAbstractAxis::titleVisibleChanged(bool visible)
  This signal is emitted when the visibility of the title text of the axis changes to \a visible.
*/

/*!
  \internal TODO: Causes build errors, find out why
  \fn void QAbstractAxis::titleTextChanged(const QString& text)
  This signal is emitted when the text of the axis title changes to \a text.
*/

/*!
  \internal TODO: Causes build errors, find out why
  \fn void QAbstractAxis::titleColorChanged(const QColor& color)
  This signal is emitted when the color used to draw the axis title changes to \a color.
*/

/*!
  \internal TODO: Causes build errors, find out why
  \fn void QAbstractAxis::titleFontChanged(const QFont& font)
  This signal is emitted when the font of the axis title changes to \a font.
*/

/*!
 \internal
  Constructs a new axis object that is a child of \a parent. The ownership is taken by
  graph when the axis is added.
*/

QAbstractAxis::QAbstractAxis(QAbstractAxisPrivate &d, QObject *parent)
    : QObject(parent),
      d_ptr(&d)
{
}

/*!
  Destructs the axis object. When the axis is added to a chart, the chart object takes ownership.
*/

QAbstractAxis::~QAbstractAxis()
{
    if (d_ptr->m_graph)
        qFatal("Still binded axis detected !");
}

/*!
  Determines whether the axis line and tick marks are \a visible.
 */
void QAbstractAxis::setLineVisible(bool visible)
{
    if (d_ptr->m_arrowVisible != visible) {
        d_ptr->m_arrowVisible = visible;
        update();
        emit lineVisibleChanged(visible);
    }
}

bool QAbstractAxis::isLineVisible() const
{
    return d_ptr->m_arrowVisible;
}

void QAbstractAxis::setGridLineVisible(bool visible)
{
    if (d_ptr->m_gridLineVisible != visible) {
        d_ptr->m_gridLineVisible = visible;
        update();
        emit gridVisibleChanged(visible);
    }
}

bool QAbstractAxis::isGridLineVisible() const
{
    return d_ptr->m_gridLineVisible;
}

void QAbstractAxis::setMinorGridLineVisible(bool visible)
{
    if (d_ptr->m_minorGridLineVisible != visible) {
        d_ptr->m_minorGridLineVisible = visible;
        update();
        emit minorGridVisibleChanged(visible);
    }
}

bool QAbstractAxis::isMinorGridLineVisible() const
{
    return d_ptr->m_minorGridLineVisible;
}

void QAbstractAxis::setGridLineColor(const QColor &color)
{
    if (color != d_ptr->m_gridLineColor) {
        d_ptr->m_gridLineColor = color;
        emit gridLineColorChanged(color);
    }
}

QColor QAbstractAxis::gridLineColor()
{
    return d_ptr->m_gridLineColor;
}

void QAbstractAxis::setMinorGridLineColor(const QColor &color)
{
    if (color != d_ptr->m_minorGridLineColor) {
        d_ptr->m_minorGridLineColor = color;
        emit minorGridLineColorChanged(color);
    }
}

QColor QAbstractAxis::minorGridLineColor()
{
    return d_ptr->m_minorGridLineColor;
}

void QAbstractAxis::setLabelsVisible(bool visible)
{
    if (d_ptr->m_labelsVisible != visible) {
        d_ptr->m_labelsVisible = visible;
        update();
        emit labelsVisibleChanged(visible);
    }
}

bool QAbstractAxis::labelsVisible() const
{
    return d_ptr->m_labelsVisible;
}

/*!
  Sets the font used to draw labels to \a font.
*/
void QAbstractAxis::setLabelsFont(const QFont &font)
{
    if (d_ptr->m_labelsFont != font) {
        d_ptr->m_labelsFont = font;
        emit labelsFontChanged(font);
    }
}

/*!
  Returns the font used to draw labels.
*/
QFont QAbstractAxis::labelsFont() const
{
    return d_ptr->m_labelsFont;
}

void QAbstractAxis::setLabelsAngle(int angle)
{
    if (d_ptr->m_labelsAngle != angle) {
        d_ptr->m_labelsAngle = angle;
        emit labelsAngleChanged(angle);
    }
}

int QAbstractAxis::labelsAngle() const
{
    return d_ptr->m_labelsAngle;
}
void QAbstractAxis::setLabelsColor(QColor color)
{
    if (d_ptr->m_labelsColor != color) {
        d_ptr->m_labelsColor = color;
        emit labelsColorChanged(color);
    }
}

QColor QAbstractAxis::labelsColor() const
{
    return d_ptr->m_labelsColor;
}

void QAbstractAxis::setTitleVisible(bool visible)
{
    if (d_ptr->m_titleVisible != visible) {
        d_ptr->m_titleVisible = visible;
        emit titleVisibleChanged(visible);
    }
}

bool QAbstractAxis::isTitleVisible() const
{
    return d_ptr->m_titleVisible;
}

/*!
  Sets the color used to draw titles to \a color.
 */
void QAbstractAxis::setTitleColor(const QColor &color)
{
    if (d_ptr->m_titleColor != color) {
        d_ptr->m_titleColor = color;
        emit titleColorChanged(color);
    }
}

/*!
  Returns the color used to draw titles.
*/
QColor QAbstractAxis::titleColor() const
{
    return d_ptr->m_titleColor;
}

/*!
  Sets the font used to draw titles to \a font.
*/
void QAbstractAxis::setTitleFont(const QFont &font)
{
    if (d_ptr->m_titleFont != font) {
        d_ptr->m_titleFont = font;
        emit titleFontChanged(font);
    }
}

/*!
  Returns the font used to draw titles.
*/
QFont QAbstractAxis::titleFont() const
{
    return d_ptr->m_titleFont;
}

void QAbstractAxis::setTitleText(const QString &title)
{
    if (d_ptr->m_title != title) {
        d_ptr->m_title = title;
        emit titleTextChanged(title);
    }
}

QString QAbstractAxis::titleText() const
{
    return d_ptr->m_title;
}

bool QAbstractAxis::isVisible() const
{
    return d_ptr->m_visible;
}

/*!
  Sets the visibility of the axis, labels, and grid lines to \a visible.
*/
void QAbstractAxis::setVisible(bool visible)
{
    if (d_ptr->m_visible != visible) {
        d_ptr->m_visible = visible;
        update();
        emit visibleChanged(visible);
    }
}


/*!
  Makes the axis, labels, and grid lines visible.
*/
void QAbstractAxis::show()
{
    setVisible(true);
}

/*!
  Makes the axis, labels, and grid lines invisible.
*/
void QAbstractAxis::hide()
{
    setVisible(false);
}

/*!
  Sets the minimum value shown on the axis.
  Depending on the actual axis type, the \a min parameter is converted to the appropriate type
  of value. If the conversion is impossible, the function call does nothing.
*/
void QAbstractAxis::setMin(const QVariant &min)
{
    d_ptr->setMin(min);
}

/*!
  Sets the maximum value shown on the axis.
  Depending on the actual axis type, the \a max parameter is converted to the appropriate type
  of value. If the conversion is impossible, the function call does nothing.
*/
void QAbstractAxis::setMax(const QVariant &max)
{
    d_ptr->setMax(max);
}

/*!
  Sets the range shown on the axis.
  Depending on the actual axis type, the \a min and \a max parameters are converted to
  appropriate types of values. If the conversion is impossible, the function call does nothing.
*/
void QAbstractAxis::setRange(const QVariant &min, const QVariant &max)
{
    d_ptr->setRange(min, max);
}


/*!
  Returns the orientation of the axis (vertical or horizontal).
*/
Qt::Orientation QAbstractAxis::orientation() const
{
    return d_ptr->orientation();
}

void QAbstractAxis::setOrientation(Qt::Orientation orientation)
{
    d_ptr->m_orientation = orientation;
}

Qt::Alignment QAbstractAxis::alignment() const
{
    return d_ptr->alignment();
}

bool QAbstractAxis::isReverse() const
{
    return d_ptr->m_reverse;
}

/*!
  Sets axis labels editability to \a editable.

  When the labels are editable the user will be able to change the range of the
  axis conveniently by editing any of the labels. This feature is only supported
  for the QValueAxis and the QDateTimeAxis.

  By default, labels are not editable.
*/
void QAbstractAxis::setLabelsEditable(bool editable)
{
    if (d_ptr->m_labelsEditable != editable) {
        // In the case if the axis already added to a chart
        // set the labels editability on the axisItem().
        // Otherwise the labels editability will be set in the
        // QValueAxisPrivate::initializeGraphics.
//        if (d_ptr->axisItem() != nullptr)
//            d_ptr->axisItem()->setLabelsEditable(editable);
        d_ptr->m_labelsEditable = editable;
        emit labelsEditableChanged(editable);
    }
}

/*!
  Returns \c true if axis labels are editable.
*/
bool QAbstractAxis::labelsEditable() const
{
    return d_ptr->m_labelsEditable;
}

bool QAbstractAxis::labelsTruncated() const
{
    return d_ptr->m_labelsTruncated;
}

void QAbstractAxis::setTruncateLabels(bool truncateLabels)
{
    if (d_ptr->m_truncateLabels != truncateLabels) {
        d_ptr->m_truncateLabels = truncateLabels;
        emit truncateLabelsChanged(d_ptr->m_truncateLabels);
    }
}

bool QAbstractAxis::truncateLabels() const
{
    return d_ptr->m_truncateLabels;
}

void QAbstractAxis::setReverse(bool reverse)
{
    if (d_ptr->m_reverse != reverse && type() != QAbstractAxis::AxisTypeBarCategory) {
        d_ptr->m_reverse = reverse;
        emit reverseChanged(reverse);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

QAbstractAxisPrivate::QAbstractAxisPrivate(QAbstractAxis *q)
    : q_ptr(q)
{
}

QAbstractAxisPrivate::~QAbstractAxisPrivate()
{
}

void QAbstractAxisPrivate::setAlignment(Qt::Alignment alignment)
{
    switch (alignment) {
    case Qt::AlignTop:
    case Qt::AlignBottom:
        m_orientation = Qt::Horizontal;
        break;
    case Qt::AlignLeft:
    case Qt::AlignRight:
        m_orientation = Qt::Vertical;
        break;
    default:
        qWarning("No alignment specified !");
        break;
    }
    m_alignment = alignment;
}

void QAbstractAxisPrivate::setLabelsTruncated(bool labelsTruncated)
{
    Q_Q(QAbstractAxis);
    if (m_labelsTruncated != labelsTruncated) {
        m_labelsTruncated = labelsTruncated;
        emit q->labelsTruncatedChanged(m_labelsTruncated);
    }
}

void QAbstractAxisPrivate::handleRangeChanged(qreal min, qreal max)
{
    setRange(min,max);
}

QT_END_NAMESPACE


