// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGraphs/qabstractseries.h>
#include <private/qabstractseries_p.h>
#include <QtGraphs/qbarset.h>
#include <private/qbarset_p.h>
#include <QtGraphs/QBarCategoryAxis>
#include <private/qbarcategoryaxis_p.h>
#include <QtGraphs/QValueAxis>
#include <private/qvalueaxis_p.h>
#include <QtGraphs/qbarseries.h>
#include <private/qbarseries_p.h>

QT_BEGIN_NAMESPACE

/*!
    \class QBarSeries
    \inmodule QtGraphs
    \ingroup graphs_2D
    \brief The QBarSeries class presents data in bar graphs.

    This class draws data by default as a series of bars grouped by category,
    with one bar per category from each bar set added to the series.
    It also supports horizontal bars and grouping bars as stacked.

    \sa QBarSet, QAbstractSeries
*/
/*!
    \qmltype BarSeries
    \instantiates QBarSeries
    \inqmlmodule QtGraphs
    \ingroup graphs_qml_2D
    \inherits AbstractSeries

    \brief Presents data in bar graphs.

    Draws data by default as a series of bars grouped by category,
    with one bar per category from each bar set added to the series.
    It also supports horizontal bars and grouping bars as stacked.
*/

/*!
    \property QBarSeries::barWidth
    \brief The width of the bars of the series.

    The unit of width is the unit of the x-axis. The minimum width for bars is zero, and negative
    values are treated as zero. Setting the width to zero means that the width of the bar on the
    screen is one pixel regardless of the scale of the x-axis. Bars wider than zero are scaled
    using the x-axis scale.

    By default, the barWidth is 0.5 (bars will take 50% of the available width).
    The valid values range from 0.0 (0%) to 1.0 (100%).

    \note When used with QBarSeries, this value specifies the width of a group of bars instead of
    that of a single bar.
    \sa QBarSeries
*/

/*!
    \enum QBarSeries::BarsType

    This enum value describes the type of the bar series:

    \value BarsGroups Bar sets are grouped by category.
    \value BarsStacked Bar sets are stacked after each other by category.
*/
/*!
    \property QBarSeries::barsType
    \brief The type of the bar series.
*/
/*!
    \qmlproperty enumeration BarSeries::barsType

    The type of the bar series:

    \value BarSeries.BarsGroups
        Bar sets are grouped by category.
    \value BarSeries.BarsStacked
        Bar sets are stacked after each other by category.
*/

/*!
    \enum QBarSeries::BarsOrientation

    This enum value describes the orientation of the bar series:

    \value BarsVertical Bars appear vertically.
    \value BarsHorizontal Bars appear horizontally.
*/
/*!
    \property QBarSeries::barsOrientation
    \brief The orientation of the bar series. By default the bars appear vertically.
*/
/*!
    \qmlproperty enumeration BarSeries::barsOrientation

    The orientation of the bar series:

    \value BarSeries.BarsVertical
        Bars appear vertically. This is the default value.
    \value BarSeries.BarsHorizontal
        Bars appear horizontally.
*/

/*!
    \qmlproperty real BarSeries::barWidth
    The unit of width is the unit of the x-axis. The minimum width for bars is zero, and negative
    values are treated as zero. Setting the width to zero means that the width of the bar on the
    screen is one pixel regardless of the scale of the x-axis. Bars wider than zero are scaled
    using the x-axis scale.

    By default, the barWidth is 0.5 (bars will take 50% of the available width).
    The valid values range from 0.0 (0%) to 1.0 (100%).

    \note When used with the BarSeries type, this value specifies the width of a group of bars
    instead of that of a single bar.
*/

/*!
    \property QBarSeries::count
    \brief The number of bar sets in a bar series.
*/
/*!
    \qmlproperty int BarSeries::count
    The number of bar sets in a bar series.
*/

/*!
    \property QBarSeries::labelsVisible
    \brief The visibility of the labels in a bar series.
*/
/*!
    \qmlproperty bool BarSeries::labelsVisible
    The visibility of the labels in a bar series.
*/

/*!
    \property QBarSeries::labelsFormat
    \brief The format used for showing labels in a bar series.

    QBarSeries supports the following format tag:
    \table
    \row
    \li @value      \li The value of the bar
    \endtable

    For example, the following usage of the format tags would produce labels that show the value
    followed by the unit (u):
    \code
    series->setLabelsFormat("@value u");
    \endcode

    By default, the labels show the value of the bar. For the percent bar series, \e % is added
    after the value. The labels are shown on the plot area, if the bars are close to each other,
    the labels may overlap.

    \sa labelsVisible, labelsPosition, labelsPrecision
*/
/*!
    \qmlproperty string BarSeries::labelsFormat
    The format used for showing labels in a bar series.

    \sa QBarSeries::labelsFormat, labelsVisible, labelsPosition
*/
/*!
    \fn void QBarSeries::labelsFormatChanged(const QString &format)
    This signal is emitted when the \a format of data value labels changes.
*/

/*!
    \enum QBarSeries::LabelsPosition

    This enum value describes the position of the data value labels:

    \value LabelsCenter Label is located in the center of the bar.
    \value LabelsInsideEnd Label is located inside the bar at the top.
    \value LabelsInsideBase Label is located inside the bar at the bottom.
    \value LabelsOutsideEnd Label is located outside the bar at the top.
*/

/*!
    \property QBarSeries::labelsPosition
    \brief The position of value labels.

    \sa labelsVisible, labelsFormat
*/
/*!
    \qmlproperty enumeration BarSeries::labelsPosition

    The position of the data value labels:

    \value  BarSeries.LabelsCenter
        Label is located in the center of the bar.
    \value  BarSeries.LabelsInsideEnd
        Label is located inside the bar at the top.
    \value  BarSeries.LabelsInsideBase
        Label is located inside the bar at the bottom.
    \value  BarSeries.LabelsOutsideEnd
        Label is located outside the bar at the top.

    \sa labelsVisible, labelsFormat
*/
/*!
    \fn void QBarSeries::labelsPositionChanged(QBarSeries::LabelsPosition position)
    This signal is emitted when the \a position of value labels changes.
*/

/*!
    \property QBarSeries::labelsAngle
    \brief The angle of the value labels in degrees.
*/
/*!
    \qmlproperty real BarSeries::labelsAngle
    The angle of the value labels in degrees.
*/
/*!
    \fn void QBarSeries::labelsAngleChanged(qreal angle)
    This signal is emitted when the \a angle of the value labels changes.
*/

/*!
    \property QBarSeries::labelsPrecision
    \brief The maximum amount of significant digits shown in value labels.

    Default value is 6.
*/
/*!
    \qmlproperty real BarSeries::labelsPrecision
    The maximum amount of significant digits shown in value labels.

    Default value is 6.
*/
/*!
    \property QBarSeries::barComponent
    \brief A custom QML component used for visualizing each of the bars.
    Instance of this component is created for each of the bar.
    To get access into bars properties, use \l BarComponent.
    When this is not defined, a default rectangle visualization for bars is used.
*/
/*!
    \qmlproperty Component BarSeries::barComponent
    A custom QML component used for visualizing each of the bars.
    Instance of this component is created for each of the bar.
    To get access into bars properties, use \l BarComponent.
    When this is not defined, a default rectangle visualization for bars is used.
*/
/*!
    \fn void QBarSeries::labelsPrecisionChanged(int precision)
    This signal is emitted when the \a precision of the value labels changes.
*/

/*!
    \fn void QBarSeries::clicked(int index, QBarSet *barset)
    This signal is emitted when the user clicks the bar specified by \a index
    in the bar set specified by \a barset.
*/
/*!
    \qmlsignal BarSeries::clicked(int index, BarSet barset)
    This signal is emitted when the user clicks the bar specified by \a index
    in the bar set specified by \a barset.

    The corresponding signal handler is \c onClicked.
*/

/*!
    \fn void QBarSeries::pressed(int index, QBarSet *barset)
    This signal is emitted when the user clicks the bar specified by \a index
    in the bar set specified by \a barset and holds down the mouse button.
*/
/*!
    \qmlsignal BarSeries::pressed(int index, BarSet barset)
    This signal is emitted when the user clicks the bar specified by \a index
    in the bar set specified by \a barset and holds down the mouse button.

    The corresponding signal handler is \c onPressed.
*/

/*!
    \fn void QBarSeries::released(int index, QBarSet *barset)
    This signal is emitted when the user releases the mouse press on the bar
    specified by \a index in the bar set specified by \a barset.
*/
/*!
    \qmlsignal BarSeries::released(int index, BarSet barset)
    This signal is emitted when the user releases the mouse press on the bar
    specified by \a index in the bar set specified by \a barset.

    The corresponding signal handler is \c onReleased.
*/

/*!
    \fn void QBarSeries::doubleClicked(int index, QBarSet *barset)
    This signal is emitted when the user double-clicks the bar specified by \a index
    in the bar set specified by \a barset.
*/
/*!
    \qmlsignal BarSeries::doubleClicked(int index, BarSet barset)
    This signal is emitted when the user double-clicks the bar specified by \a index
    in the bar set specified by \a barset.

    The corresponding signal handler is \c onDoubleClicked.
*/

/*!
    \fn void QBarSeries::hovered(bool status, int index, QBarSet* barset)

    This signal is emitted when a mouse is hovered over the bar specified by \a index in the
    bar set specified by \a barset. When the mouse moves over the bar, \a status turns \c true,
    and when the mouse moves away again, it turns \c false.
*/
/*!
    \qmlsignal BarSeries::hovered(bool status, int index, BarSet barset)

    This signal is emitted when a mouse is hovered over the bar specified by \a index in the
    bar set specified by \a barset. When the mouse moves over the bar, \a status turns \c true,
    and when the mouse moves away again, it turns \c false.

    The corresponding signal handler is \c onHovered.
*/

/*!
    \fn void QBarSeries::updatedBars()
    This signal is emitted when bars are updated.
*/

/*!
    \fn void QBarSeries::updatedLayout()
    This signal is emitted when the layout is updated.
*/
/*!
    \fn void QBarSeries::restructuredBars()
    This signal is emitted when the bars are restructured.
*/

/*!
    \fn void QBarSeries::countChanged()
    This signal is emitted when the number of bar sets is changed, for example by append() or
    remove().
*/

/*!
    \fn void QBarSeries::barWidthChanged()
    This signal is emitted when the bar width changes.
*/

/*!
    \fn void QBarSeries::labelsVisibleChanged(bool visible)
    This signal is emitted when the labels' visibility changes to \a visible.
    \sa isLabelsVisible(), setLabelsVisible()
*/

/*!
    \fn void QBarSeries::barsetsAdded(const QList<QBarSet *> &sets)
    This signal is emitted when the bar sets specified by \a sets are added to the series.
    \sa append(), insert()
*/
/*!
    \qmlsignal BarSeries::barsetsAdded()
    This signal is emitted when bar sets are added to the series.

    The corresponding signal handler is \c onBarsetsAdded.
*/

/*!
    \fn void QBarSeries::barsetsRemoved(const QList<QBarSet *> &sets)
    This signal is emitted when the bar sets specified by \a sets are removed from the series.
    \sa remove()
*/
/*!
    \qmlsignal BarSeries::barsetsRemoved()
    This signal is emitted when bar sets are removed from the series.

    The corresponding signal handler is \c onBarsetsRemoved.
*/

/*!
    \fn void QBarSeries::setValueChanged(int index, QBarSet *barset)
    This signal is emitted when a barset's value is changed. \a index is the index of
    the barset in the series. The \a barset is a pointer to the changed set.
*/
/*!
    \fn void QBarSeries::setValueAdded(int index, int count, QBarSet *barset)
    This signal is emitted when a barset's value is changed. \a index is the index of
    the barset in the series. The number of the added values is indicated \a count.
    The \a barset is a pointer to the changed set.
*/
/*!
    \fn void QBarSeries::setValueRemoved(int index, int count, QBarSet *barset)
    This signal is emitted when a barset's value is changed. \a index is the index of
    the barset in the series. The number of the removed values is indicated \a count.
    The \a barset is a pointer to the changed set.
*/

/*!
    \qmlmethod BarSet BarSeries::at(int index)
    Returns the bar set at \a index. Returns null if the index is not valid.
*/

/*!
    \qmlmethod BarSet BarSeries::append(string label, VariantList values)
    Adds a new bar set with \a label and \a values to the index. \a values is
    a list of real values.

    For example:
    \code
        myBarSeries.append("set 1", [0, 0.2, 0.2, 0.5, 0.4, 1.5, 0.9]);
    \endcode
*/

/*!
    \qmlmethod BarSet BarSeries::insert(int index, string label, VariantList values)
    Adds a new bar set with \a label and \a values to \a index. \a values can be a list
    of real values or a list of XYPoint types.

    If the index value is equal to or less than zero, the new bar set is prepended to the bar
    series. If the index value is equal to or greater than the number of bar sets in the bar
    series, the new bar set is appended to the bar series.

    \sa append()
*/

/*!
    \qmlmethod bool BarSeries::remove(BarSet barset)
    Removes the bar set specified by \a barset from the series. Returns \c true if successful,
    \c false otherwise.
*/

/*!
    \qmlmethod BarSeries::clear()
    Removes all bar sets from the series.
*/

/*!
    \internal
*/

/*!
    Constructs an empty bar series that is a QObject and a child of \a parent.
*/
QBarSeries::QBarSeries(QObject *parent)
    : QAbstractSeries(*(new QBarSeriesPrivate()), parent)
{}

QBarSeries::QBarSeries(QBarSeriesPrivate &dd, QObject *parent)
    : QAbstractSeries(dd, parent)
{}

/*!
    Removes the bar series and the bar sets owned by it.
*/
QBarSeries::~QBarSeries()
{
    Q_D(QBarSeries);
    if (d->m_graph)
        d->m_graph->removeSeries(this);
}

/*!
    Returns the bar series.
*/
QAbstractSeries::SeriesType QBarSeries::type() const
{
    return QAbstractSeries::SeriesTypeBar;
}

/*!
    \property QBarSeries::axisX
    \brief X-axis of the series.

    The x-axis used for the series. This should be QBarCategoryAxis.
*/
/*!
    \qmlproperty AbstractAxis BarSeries::axisX
    The x-axis used for the series. This should be BarCategoryAxis.
    \sa axisY
*/
QAbstractAxis *QBarSeries::axisX()
{
    Q_D(const QBarSeries);
    return d->m_axisX;
}

void QBarSeries::setAxisX(QAbstractAxis *axis)
{
    if (axis != nullptr && !qobject_cast<QBarCategoryAxis *>(axis))
        return;

    Q_D(QBarSeries);
    detachAxis(d->m_axisX);
    d->m_axisX = axis;
    if (axis) {
        axis->setOrientation(Qt::Horizontal);
        attachAxis(axis);
    }
}

/*!
    \property QBarSeries::axisY
    \brief Y-axis of the series.

    The y-axis used for the series. This should be QValueAxis.
*/
/*!
    \qmlproperty AbstractAxis BarSeries::axisY
    The y-axis used for the series. This should be ValueAxis.
    \sa axisX
*/
QAbstractAxis *QBarSeries::axisY()
{
    Q_D(const QBarSeries);
    return d->m_axisY;
}

void QBarSeries::setAxisY(QAbstractAxis *axis)
{
    if (axis != nullptr && !qobject_cast<QValueAxis *>(axis))
        return;

    Q_D(QBarSeries);
    detachAxis(d->m_axisY);
    d->m_axisY = axis;
    if (axis) {
        axis->setOrientation(Qt::Vertical);
        attachAxis(axis);
    }
}

void QBarSeries::setBarsType(QBarSeries::BarsType type)
{
    Q_D(QBarSeries);
    if (d->m_barsType != type) {
        d->m_barsType = type;
        emit barsTypeChanged(type);
        emit update();
    }
}

QBarSeries::BarsType QBarSeries::barsType() const
{
    Q_D(const QBarSeries);
    return d->m_barsType;
}

void QBarSeries::setBarsOrientation(QBarSeries::BarsOrientation orientation)
{
    Q_D(QBarSeries);
    if (d->m_barsOrientation != orientation) {
        d->m_barsOrientation = orientation;
        emit barsOrientationChanged(orientation);
        emit update();
    }
}

QBarSeries::BarsOrientation QBarSeries::barsOrientation() const
{
    Q_D(const QBarSeries);
    return d->m_barsOrientation;
}

/*!
    Sets the width of the bars of the series to \a width.
*/
void QBarSeries::setBarWidth(qreal width)
{
    Q_D(QBarSeries);
    if (d->barWidth() != width) {
        d->setBarWidth(width);
        emit barWidthChanged();
    }
}

/*!
    Returns the width of the bars of the series.
    \sa setBarWidth()
*/
qreal QBarSeries::barWidth() const
{
    Q_D(const QBarSeries);
    return d->barWidth();
}

/*!
    Adds a set of bars specified by \a set to the bar series and takes ownership of it. If the set
    is null or it already belongs to the series, it will not be appended.
    Returns \c true if appending succeeded.
*/
bool QBarSeries::append(QBarSet *set)
{
    Q_D(QBarSeries);
    bool success = d->append(set);
    if (success) {
        QList<QBarSet *> sets;
        sets.append(set);
        set->setParent(this);
        QObject::connect(set, &QBarSet::update, this, &QBarSeries::update);
        emit barsetsAdded(sets);
        emit countChanged();
    }
    emit update();
    return success;
}

/*!
    Removes the bar set specified by \a set from the series and permanently deletes it if
    the removal succeeds. Returns \c true if the set was removed.
*/
bool QBarSeries::remove(QBarSet *set)
{
    Q_D(QBarSeries);
    bool success = d->remove(set);
    if (success) {
        QList<QBarSet *> sets;
        sets.append(set);
        set->setParent(0);
        QObject::disconnect(set, &QBarSet::update, this, &QBarSeries::update);
        emit barsetsRemoved(sets);
        emit countChanged();
        delete set;
        set = 0;
    }
    return success;
}

/*!
    Takes a single \a set from the series. Does not delete the bar set object.
    \note The series remains the barset's parent object. You must set the
    parent object to take full ownership.

    Returns \c true if the take operation succeeds.
*/
bool QBarSeries::take(QBarSet *set)
{
    Q_D(QBarSeries);
    bool success = d->remove(set);
    if (success) {
        QList<QBarSet *> sets;
        sets.append(set);
        QObject::disconnect(set, &QBarSet::update, this, &QBarSeries::update);
        emit barsetsRemoved(sets);
        emit countChanged();
    }
    return success;
}

/*!
    Adds a list of bar sets specified by \a sets to a bar series and takes ownership of the sets.
    Returns \c true if all sets were appended successfully. If any of the sets is null or was
    previously appended to the series, nothing is appended and this function returns \c false.
    If any of the sets appears in the list more than once, nothing is appended and this function
    returns \c false.
*/
bool QBarSeries::append(const QList<QBarSet *> &sets)
{
    Q_D(QBarSeries);
    if (!d->append(sets))
        return false;

    for (auto *set : sets) {
        set->setParent(this);
        QObject::connect(set, &QBarSet::update, this, &QBarSeries::update);
    }

    emit barsetsAdded(sets);
    emit countChanged();

    return true;
}

/*!
    Inserts a bar set specified by \a set to a series at the position specified by \a index
    and takes ownership of the set. If the set is null or already belongs to the series, it will
    not be appended. Returns \c true if inserting succeeds.
*/
bool QBarSeries::insert(int index, QBarSet *set)
{
    Q_D(QBarSeries);
    bool success = d->insert(index, set);
    if (success) {
        QList<QBarSet *> sets;
        sets.append(set);
        QObject::connect(set, &QBarSet::update, this, &QBarSeries::update);
        emit barsetsAdded(sets);
        emit countChanged();
    }
    return success;
}

/*!
    Removes all bar sets from the series and permanently deletes them.
*/
void QBarSeries::clear()
{
    Q_D(QBarSeries);
    const QList<QBarSet *> sets = barSets();
    bool success = d->remove(sets);
    if (success) {
        emit barsetsRemoved(sets);
        emit countChanged();
        for (QBarSet *set : sets) {
            QObject::disconnect(set, &QBarSet::update, this, &QBarSeries::update);
            delete set;
        }
    }
}

/*!
    Returns the number of bar sets in a bar series.
*/
int QBarSeries::count() const
{
    Q_D(const QBarSeries);
    return d->m_barSets.size();
}

/*!
    Returns a list of bar sets in a bar series. Keeps the ownership of the bar sets.
 */
QList<QBarSet *> QBarSeries::barSets() const
{
    Q_D(const QBarSeries);
    return d->m_barSets;
}

/*!
    Sets the visibility of labels in a bar series to \a visible.
*/
void QBarSeries::setLabelsVisible(bool visible)
{
    Q_D(QBarSeries);
    if (d->m_labelsVisible != visible) {
        d->setLabelsVisible(visible);
        emit labelsVisibleChanged(visible);
    }
}

/*!
    Returns the visibility of labels.
*/
bool QBarSeries::isLabelsVisible() const
{
    Q_D(const QBarSeries);
    return d->m_labelsVisible;
}

void QBarSeries::setLabelsFormat(const QString &format)
{
    Q_D(QBarSeries);
    if (d->m_labelsFormat != format) {
        d->m_labelsFormat = format;
        d->setLabelsDirty(true);
        emit labelsFormatChanged(format);
    }
}

QString QBarSeries::labelsFormat() const
{
    Q_D(const QBarSeries);
    return d->m_labelsFormat;
}

void QBarSeries::setLabelsAngle(qreal angle)
{
    Q_D(QBarSeries);
    if (d->m_labelsAngle != angle) {
        d->m_labelsAngle = angle;
        d->setLabelsDirty(true);
        emit labelsAngleChanged(angle);
    }
}

qreal QBarSeries::labelsAngle() const
{
    Q_D(const QBarSeries);
    return d->m_labelsAngle;
}

void QBarSeries::setLabelsPosition(QBarSeries::LabelsPosition position)
{
    Q_D(QBarSeries);
    if (d->m_labelsPosition != position) {
        d->m_labelsPosition = position;
        emit labelsPositionChanged(position);
    }
}

QBarSeries::LabelsPosition QBarSeries::labelsPosition() const
{
    Q_D(const QBarSeries);
    return d->m_labelsPosition;
}

void QBarSeries::setLabelsPrecision(int precision)
{
    Q_D(QBarSeries);
    if (d->m_labelsPrecision != precision) {
        d->m_labelsPrecision = precision;
        d->setLabelsDirty(true);
        emit labelsPrecisionChanged(precision);
    }
}

int QBarSeries::labelsPrecision() const
{
    Q_D(const QBarSeries);
    return d->m_labelsPrecision;
}

QQmlComponent *QBarSeries::barComponent() const
{
    Q_D(const QBarSeries);
    return d->m_barComponent;
}

void QBarSeries::setBarComponent(QQmlComponent *newBarComponent)
{
    Q_D(QBarSeries);
    if (d->m_barComponent == newBarComponent)
        return;
    d->m_barComponent = newBarComponent;
    emit barComponentChanged();
    emit update();
}

// Select all the elements in the series
void QBarSeries::selectAll()
{
    Q_D(QBarSeries);
    for (auto s : d->m_barSets) {
        s->selectAllBars();
    }
}

// Deselect all the elements in the series
void QBarSeries::deselectAll()
{
    Q_D(QBarSeries);
    for (auto s : d->m_barSets) {
        s->deselectAllBars();
    }
}

void QBarSeries::componentComplete()
{
    for (auto *child : children()) {
        if (auto bs = qobject_cast<QBarSet *>(child))
            append(bs);
    }
    QAbstractSeries::componentComplete();
}

void QBarSeries::handleSetValueChange(int index)
{
    QBarSet *set = qobject_cast<QBarSet *>(sender());
    if (set)
        emit setValueChanged(index, set);
    emit update();
}

void QBarSeries::handleSetValueAdd(int index, int count)
{
    QBarSet *set = qobject_cast<QBarSet *>(sender());
    if (set)
        emit setValueAdded(index, count, set);
    emit update();
}

void QBarSeries::handleSetValueRemove(int index, int count)
{
    QBarSet *set = qobject_cast<QBarSet *>(sender());
    if (set)
        emit setValueRemoved(index, count, set);
    emit update();
}

QBarSeriesPrivate::QBarSeriesPrivate()
    : m_barWidth(0.5) // Default value is 50% of category width
      , m_labelsVisible(false)
      , m_visible(true)
      , m_blockBarUpdate(false)
      , m_labelsFormat()
      , m_labelsPosition(QBarSeries::LabelsCenter)
      , m_labelsAngle(0)
      , m_labelsPrecision(6)
      , m_visualsDirty(true)
      , m_labelsDirty(true)
{
}

int QBarSeriesPrivate::categoryCount() const
{
    // No categories defined. return count of longest set.
    int count = 0;
    for (int i = 0; i < m_barSets.size(); i++) {
        if (m_barSets.at(i)->count() > count)
            count = m_barSets.at(i)->count();
    }

    return count;
}

void QBarSeriesPrivate::setBarWidth(qreal width)
{
    Q_Q(QBarSeries);
    width = std::clamp(width, 0.0, 1.0);
    if (!qFuzzyCompare(width, m_barWidth)) {
        m_barWidth = width;
        q->update();
    }
}

qreal QBarSeriesPrivate::barWidth() const
{
    return m_barWidth;
}

QBarSet *QBarSeriesPrivate::barsetAt(int index)
{
    return m_barSets.at(index);
}

void QBarSeriesPrivate::setVisible(bool visible)
{
    Q_Q(QBarSeries);
    m_visible = visible;
    emit q->visibleChanged();
}

void QBarSeriesPrivate::setLabelsVisible(bool visible)
{
    Q_Q(QBarSeries);
    m_labelsVisible = visible;
    emit q->labelsVisibleChanged(visible);
}

qreal QBarSeriesPrivate::min()
{
    if (m_barSets.size() <= 0)
        return 0;

    qreal min = INT_MAX;

    for (int i = 0; i < m_barSets.size(); i++) {
        int categoryCount = m_barSets.at(i)->count();
        for (int j = 0; j < categoryCount; j++) {
            qreal temp = m_barSets.at(i)->at(j);
            if (temp < min)
                min = temp;
        }
    }
    return min;
}

qreal QBarSeriesPrivate::max()
{
    if (m_barSets.size() <= 0)
        return 0;

    qreal max = INT_MIN;

    for (int i = 0; i < m_barSets.size(); i++) {
        int categoryCount = m_barSets.at(i)->count();
        for (int j = 0; j < categoryCount; j++) {
            qreal temp = m_barSets.at(i)->at(j);
            if (temp > max)
                max = temp;
        }
    }

    return max;
}

qreal QBarSeriesPrivate::valueAt(int set, int category)
{
    if ((set < 0) || (set >= m_barSets.size()))
        return 0; // No set, no value.
    else if ((category < 0) || (category >= m_barSets.at(set)->count()))
        return 0; // No category, no value.

    return m_barSets.at(set)->at(category);
}

qreal QBarSeriesPrivate::percentageAt(int set, int category)
{
    if ((set < 0) || (set >= m_barSets.size()))
        return 0; // No set, no value.
    else if ((category < 0) || (category >= m_barSets.at(set)->count()))
        return 0; // No category, no value.

    qreal value = m_barSets.at(set)->at(category);
    qreal sum = categorySum(category);
    if (qFuzzyCompare(sum, 0))
        return 0;

    return value / sum;
}

qreal QBarSeriesPrivate::categorySum(int category)
{
    qreal sum(0);
    int count = m_barSets.size(); // Count sets
    for (int set = 0; set < count; set++) {
        if (category < m_barSets.at(set)->count())
            sum += m_barSets.at(set)->at(category);
    }
    return sum;
}

qreal QBarSeriesPrivate::absoluteCategorySum(int category)
{
    qreal sum(0);
    int count = m_barSets.size(); // Count sets
    for (int set = 0; set < count; set++) {
        if (category < m_barSets.at(set)->count())
            sum += qAbs(m_barSets.at(set)->at(category));
    }
    return sum;
}

qreal QBarSeriesPrivate::maxCategorySum()
{
    qreal max = INT_MIN;
    int count = categoryCount();
    for (int i = 0; i < count; i++) {
        qreal sum = categorySum(i);
        if (sum > max)
            max = sum;
    }
    return max;
}

qreal QBarSeriesPrivate::minX()
{
    if (m_barSets.size() <= 0)
        return 0;

    qreal min = INT_MAX;

    for (int i = 0; i < m_barSets.size(); i++) {
        int categoryCount = m_barSets.at(i)->count();
        for (int j = 0; j < categoryCount; j++) {
            qreal temp = m_barSets.at(i)->d_func()->m_values.at(j).x();
            if (temp < min)
                min = temp;
        }
    }
    return min;
}

qreal QBarSeriesPrivate::maxX()
{
    if (m_barSets.size() <= 0)
        return 0;

    qreal max = INT_MIN;

    for (int i = 0; i < m_barSets.size(); i++) {
        int categoryCount = m_barSets.at(i)->count();
        for (int j = 0; j < categoryCount; j++) {
            qreal temp = m_barSets.at(i)->d_func()->m_values.at(j).x();
            if (temp > max)
                max = temp;
        }
    }

    return max;
}

qreal QBarSeriesPrivate::categoryTop(int category)
{
    // Returns top (sum of all positive values) of category.
    // Returns 0, if all values are negative
    qreal top(0);
    int count = m_barSets.size();
    for (int set = 0; set < count; set++) {
        if (category < m_barSets.at(set)->count()) {
            qreal temp = m_barSets.at(set)->at(category);
            if (temp > 0) {
                top += temp;
            }
        }
    }
    return top;
}

qreal QBarSeriesPrivate::categoryBottom(int category)
{
    // Returns bottom (sum of all negative values) of category
    // Returns 0, if all values are positive
    qreal bottom(0);
    int count = m_barSets.size();
    for (int set = 0; set < count; set++) {
        if (category < m_barSets.at(set)->count()) {
            qreal temp = m_barSets.at(set)->at(category);
            if (temp < 0) {
                bottom += temp;
            }
        }
    }
    return bottom;
}
qreal QBarSeriesPrivate::top()
{
    // Returns top of all categories
    qreal top(0);
    int count = categoryCount();
    for (int i = 0; i < count; i++) {
        qreal temp = categoryTop(i);
        if (temp > top)
            top = temp;
    }
    return top;
}

qreal QBarSeriesPrivate::bottom()
{
    // Returns bottom of all categories
    qreal bottom(0);
    int count = categoryCount();
    for (int i = 0; i < count; i++) {
        qreal temp = categoryBottom(i);
        if (temp < bottom)
            bottom = temp;
    }
    return bottom;
}

bool QBarSeriesPrivate::blockBarUpdate()
{
    return m_blockBarUpdate;
}

qreal QBarSeriesPrivate::labelsAngle() const
{
    return m_labelsAngle;
}

bool QBarSeriesPrivate::append(QBarSet *set)
{
    if ((m_barSets.contains(set)) || (set == 0))
        return false; // Fail if set is already in list or set is null.

    m_barSets.append(set);
    Q_Q(QBarSeries);
    QObject::connect(set, &QBarSet::updatedBars, q, &QBarSeries::updatedBars);
    QObject::connect(set, &QBarSet::valueChanged, q, &QBarSeries::handleSetValueChange);
    QObject::connect(set, &QBarSet::valueAdded, q, &QBarSeries::handleSetValueAdd);
    QObject::connect(set, &QBarSet::valueRemoved, q, &QBarSeries::handleSetValueRemove);
    QObject::connect(set, &QBarSet::selectedBarsChanged, q, &QBarSeries::updatedBars);

    emit q->restructuredBars(); // this notifies barchartitem
    return true;
}

bool QBarSeriesPrivate::remove(QBarSet *set)
{
    if (!m_barSets.contains(set))
        return false; // Fail if set is not in list

    m_barSets.removeOne(set);
    Q_Q(QBarSeries);
    QObject::disconnect(set, &QBarSet::updatedBars, q, &QBarSeries::updatedBars);
    QObject::disconnect(set, &QBarSet::valueChanged, q, &QBarSeries::handleSetValueChange);
    QObject::disconnect(set, &QBarSet::valueAdded, q, &QBarSeries::handleSetValueAdd);
    QObject::disconnect(set, &QBarSet::valueRemoved, q, &QBarSeries::handleSetValueRemove);
    QObject::disconnect(set, &QBarSet::selectedBarsChanged, q, &QBarSeries::updatedBars);

    emit q->restructuredBars(); // this notifies barchartitem
    return true;
}

bool QBarSeriesPrivate::append(const QList<QBarSet *> &sets)
{
    for (auto *set : sets) {
        if ((set == 0) || (m_barSets.contains(set)))
            return false; // Fail if any of the sets is null or is already appended.
        if (sets.count(set) != 1)
            return false; // Also fail if same set is more than once in given list.
    }

    Q_Q(QBarSeries);
    for (auto *set : sets) {
        m_barSets.append(set);
        QObject::connect(set, &QBarSet::updatedBars, q, &QBarSeries::updatedBars);
        QObject::connect(set, &QBarSet::valueChanged, q, &QBarSeries::handleSetValueChange);
        QObject::connect(set, &QBarSet::valueAdded, q, &QBarSeries::handleSetValueAdd);
        QObject::connect(set, &QBarSet::valueRemoved, q, &QBarSeries::handleSetValueRemove);
        QObject::connect(set, &QBarSet::selectedBarsChanged, q, &QBarSeries::updatedBars);
    }

    emit q->restructuredBars(); // this notifies barchartitem
    return true;
}

bool QBarSeriesPrivate::remove(const QList<QBarSet *> &sets)
{
    if (sets.size() == 0)
        return false;

    for (QBarSet *set : sets) {
        if ((set == 0) || (!m_barSets.contains(set)))
            return false; // Fail if any of the sets is null or is not in series
        if (sets.count(set) != 1)
            return false; // Also fail if same set is more than once in given list.
    }

    Q_Q(QBarSeries);
    for (QBarSet *set : sets) {
        m_barSets.removeOne(set);
        QObject::disconnect(set, &QBarSet::updatedBars, q, &QBarSeries::updatedBars);
        QObject::disconnect(set,
                            &QBarSet::valueChanged,
                            q,
                            &QBarSeries::handleSetValueChange);
        QObject::disconnect(set, &QBarSet::valueAdded, q, &QBarSeries::handleSetValueAdd);
        QObject::disconnect(set,
                            &QBarSet::valueRemoved,
                            q,
                            &QBarSeries::handleSetValueRemove);
        QObject::disconnect(set, &QBarSet::selectedBarsChanged, q, &QBarSeries::updatedBars);
    }

    emit q->restructuredBars(); // this notifies barchartitem

    return true;
}

bool QBarSeriesPrivate::insert(int index, QBarSet *set)
{
    if ((m_barSets.contains(set)) || (set == 0))
        return false; // Fail if set is already in list or set is null.

    m_barSets.insert(index, set);
    Q_Q(QBarSeries);
    QObject::connect(set, &QBarSet::updatedBars, q, &QBarSeries::updatedBars);
    QObject::connect(set, &QBarSet::valueChanged, q, &QBarSeries::handleSetValueChange);
    QObject::connect(set, &QBarSet::valueAdded, q, &QBarSeries::handleSetValueAdd);
    QObject::connect(set, &QBarSet::valueRemoved, q, &QBarSeries::handleSetValueRemove);
    QObject::disconnect(set, &QBarSet::selectedBarsChanged, q, &QBarSeries::updatedBars);

    emit q->restructuredBars(); // this notifies barchartitem
    return true;
}

void QBarSeriesPrivate::initializeAxes()
{
}

QT_END_NAMESPACE

#include "moc_qbarseries.cpp"
