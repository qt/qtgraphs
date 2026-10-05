// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
// Qt-Security score:significant reason:default

#include <QtCore/QAbstractItemModel>
#include <QtGraphs/QBarModelMapper>
#include <QtGraphs/QBarSeries>
#include <private/qabstractitemmodel_p.h>
#include <private/qbarmodelmapper_p.h>

#include <qtgraphs_tracepoints_p.h>

QT_BEGIN_NAMESPACE

Q_TRACE_PREFIX(qtgraphs,
              "QT_BEGIN_NAMESPACE" \
               "class QBarModelMapper;" \
              "QT_END_NAMESPACE"
          )

Q_TRACE_POINT(qtgraphs, QGraphs2DBarModelMapperInitBarsFromModel_entry);
Q_TRACE_POINT(qtgraphs, QGraphs2DBarModelMapperInitBarsFromModel_exit);

Q_TRACE_POINT(qtgraphs, QGraphs2DBarModelMapperInsertData_entry, int start, int end);
Q_TRACE_POINT(qtgraphs, QGraphs2DBarModelMapperInsertData_exit);

Q_TRACE_POINT(qtgraphs, QGraphs2DBarModelMapperRemoveData_entry, int start, int end);
Q_TRACE_POINT(qtgraphs, QGraphs2DBarModelMapperRemoveData_exit);

/*!
    \class QBarModelMapper
    \inmodule QtGraphs
    \ingroup graphs_2D
    \brief The QBarModelMapper class is a model mapper for bar series.

    Model mappers enable using a data model derived from the QAbstractItemModel class
    as a data source for a graph. A model mapper is used to create a connection
    between a data model and QBarSeries.

    Both model and bar series properties can be used to manipulate the data. The model mapper
    keeps the bar series and the data model in sync.

    The model mapper ensures that all the bar sets in the bar series have equal sizes.
    Therefore, adding or removing a value from a bar set causes the same change to be
    made in all the bar sets in the bar series.

*/
/*!
    \qmltype BarModelMapper
    \nativetype QBarModelMapper
    \inqmlmodule QtGraphs
    \ingroup graphs_qml_2D

    \brief Model mapper for bar series.

    The BarModelMapper type enables using a data model derived from the QAbstractItemModel
    class as a data source for a graph. A model mapper is used to create a connection
    between a data model and QBarSeries. You need to implement
    the data model and expose it to QML.

    Both model and bar series properties can be used to manipulate the data. The model mapper
    keeps the bar series and the data model in sync.

    The model mapper ensures that all the bar sets in the bar series have equal sizes.
    Therefore, adding or removing a value from a bar set causes the same change to be
    made in all the bar sets in the bar series.

    The following QML code snippet creates a bar series with three bar sets (assuming the model
    has at least four columns). Each bar set contains data starting from row 1. The name
    of a bar set is defined by the column header.

    \code
        BarSeries {
            BarModelMapper {
                model: myCustomModel // QAbstractItemModel derived implementation
                firstBarSetColumn: 1
                lastBarSetColumn: 3
                firstRow: 1
                orientation: Qt.Vertical
            }
        }
    \endcode

*/

/*!
    \property QBarModelMapper::series
    \brief The bar series that is used by the mapper.

    All the data in the series is discarded when it is set to the mapper.
    When a new series is specified, the old series is disconnected, but it preserves its data.
*/
/*!
    \qmlproperty BarSeries BarModelMapper::series
    The bar series that is used by the mapper. All the data in the series is discarded when it is
    set to the mapper. When the new series is specified, the old series is disconnected, but it
    preserves its data.
*/

/*!
    \property QBarModelMapper::model
    \brief The data model that is used by the mapper.
*/
/*!
    \qmlproperty model BarModelMapper::model
    The data model that is used by the mapper. You need to implement the model and expose it to QML.

    \note The model has to support adding and removing rows or columns and modifying
    the data in the cells.
*/

/*!
    \property QBarModelMapper::firstBarSetSection
    \brief The section of the model that is used as the data source for the first bar set.

    The default value is -1 (invalid mapping).

    \sa QBarModelMapper::orientation
*/
/*!
    \qmlproperty qsizetype BarModelMapper::firstBarSetSection
    The section of the model that is used as the data source for the first bar set. The default value
    is -1 (invalid mapping).

    \sa orientation
*/

/*!
    \property QBarModelMapper::lastBarSetSection
    \brief The section of the model that is used as the data source for the last bar set.

    The default value is -1 (invalid mapping).

    \sa QBarModelMapper::orientation
*/
/*!
    \qmlproperty qsizetype BarModelMapper::lastBarSetSection
    The section of the model that is used as the data source for the last bar set. The default
    value is -1 (invalid mapping).

    \sa orientation
*/

/*!
    \property QBarModelMapper::first
    \brief The row or column of the model that contains the first values of the bar sets in the bar series.

    The minimum and default value is 0.

    \sa QBarModelMapper::orientation
*/
/*!
    \qmlproperty qsizetype BarModelMapper::first
    The row or column of the model that contains the first values of the bar sets in the bar series.
    The default value is 0.

    \sa orientation
*/

/*!
    \property QBarModelMapper::count
    \brief The number of rows or columns of the model that are mapped as the data for the bar series.

    The default value is \c{-1} which is also the minimum. The count is
    limited by the number of model's rows/columns.

    \sa QBarModelMapper::orientation
*/
/*!
    \qmlproperty qsizetype BarModelMapper::count
    The number of rows or columns of the model that are mapped as the data for the bar
    series. The default value is \c{-1} which is also the minimum. The count is
    limited by the number of model's rows/columns.

    \sa orientation
*/

/*!
    \property QBarModelMapper::orientation
    \brief Tells the modelmapper how to map data from a model. If
    \c{Qt::Vertical} is used, each of the model's columns defines a bar set, and the
    model's rows define the categories. When the orientation is set to
    \c{Qt::Horizontal}, each of the model's rows defines a bar set, and the model's
    columns define categories.

    The default value is \c{Qt::Vertical}
*/
/*!
    \qmlproperty  orientation BarModelMapper::orientation
    Tells the modelmapper how to map data from a model. If
    \c{Qt.Vertical} is used, each of the model's columns defines a bar set, and the
    model's rows define the categories. When the orientation is set to
    \c{Qt.Horizontal}, each of the model's rows defines a bar set, and the model's
    columns define categories.
*/

/*!
    \qmlsignal BarModelMapper::seriesChanged()

    This signal is emitted when the bar series that the mapper is connected to changes.
*/

/*!
    \qmlsignal BarModelMapper::modelChanged()

    This signal is emitted when the model that the mapper is connected to changes.
*/

/*!
    \qmlsignal BarModelMapper::firstBarSetSectionChanged()
    This signal is emitted when the first bar set section changes.
*/

/*!
    \qmlsignal BarModelMapper::lastBarSetSectionChanged()
    This signal is emitted when the last bar set section changes.
*/

/*!
    \qmlsignal BarModelMapper::firstChanged()
    This signal is emitted when the first row or column changes.
*/

/*!
    \qmlsignal BarModelMapper::countChanged()
    This signal is emitted when the number of rows or columns changes.
*/

/*!
    \qmlsignal BarModelMapper::orientationChanged()
    This signal is emitted when the orientation changes.
*/

QBarModelMapper::QBarModelMapper(QObject *parent)
    : QObject(*(new QBarModelMapperPrivate), parent)
{}

QBarModelMapper::QBarModelMapper(QBarModelMapperPrivate &dd, QObject *parent)
    : QObject(dd, parent)
{}

QBarModelMapper::~QBarModelMapper() {}

QAbstractItemModel *QBarModelMapper::model() const
{
    Q_D(const QBarModelMapper);
    return d->m_model;
}

void QBarModelMapper::setModel(QAbstractItemModel *model)
{
    Q_D(QBarModelMapper);
    if (d->m_model) {
        QObjectPrivate::disconnect(d->m_model,
                                   &QAbstractItemModel::modelReset,
                                   d,
                                   &QBarModelMapperPrivate::initializeBarsFromModel);
        QObjectPrivate::disconnect(d->m_model,
                                   &QAbstractItemModel::dataChanged,
                                   d,
                                   &QBarModelMapperPrivate::modelUpdated);
        QObjectPrivate::disconnect(d->m_model,
                                   &QAbstractItemModel::headerDataChanged,
                                   d,
                                   &QBarModelMapperPrivate::modelHeaderDataUpdated);
        QObjectPrivate::disconnect(d->m_model,
                                   &QAbstractItemModel::rowsInserted,
                                   d,
                                   &QBarModelMapperPrivate::modelRowsAdded);
        QObjectPrivate::disconnect(d->m_model,
                                   &QAbstractItemModel::rowsRemoved,
                                   d,
                                   &QBarModelMapperPrivate::modelRowsRemoved);
        QObjectPrivate::disconnect(d->m_model,
                                   &QAbstractItemModel::columnsInserted,
                                   d,
                                   &QBarModelMapperPrivate::modelColumnsAdded);
        QObjectPrivate::disconnect(d->m_model,
                                   &QAbstractItemModel::columnsRemoved,
                                   d,
                                   &QBarModelMapperPrivate::modelColumnsRemoved);
        QObjectPrivate::disconnect(d->m_model,
                                   &QAbstractItemModel::destroyed,
                                   d,
                                   &QBarModelMapperPrivate::handleModelDestroyed);
    }

    d->m_model = model;

    d->initializeBarsFromModel();

    if (d->m_model) {
        QObjectPrivate::connect(d->m_model,
                                &QAbstractItemModel::modelReset,
                                d,
                                &QBarModelMapperPrivate::initializeBarsFromModel);
        QObjectPrivate::connect(d->m_model,
                                &QAbstractItemModel::dataChanged,
                                d,
                                &QBarModelMapperPrivate::modelUpdated);
        QObjectPrivate::connect(d->m_model,
                                &QAbstractItemModel::headerDataChanged,
                                d,
                                &QBarModelMapperPrivate::modelHeaderDataUpdated);
        QObjectPrivate::connect(d->m_model,
                                &QAbstractItemModel::rowsInserted,
                                d,
                                &QBarModelMapperPrivate::modelRowsAdded);
        QObjectPrivate::connect(d->m_model,
                                &QAbstractItemModel::rowsRemoved,
                                d,
                                &QBarModelMapperPrivate::modelRowsRemoved);
        QObjectPrivate::connect(d->m_model,
                                &QAbstractItemModel::columnsInserted,
                                d,
                                &QBarModelMapperPrivate::modelColumnsAdded);
        QObjectPrivate::connect(d->m_model,
                                &QAbstractItemModel::columnsRemoved,
                                d,
                                &QBarModelMapperPrivate::modelColumnsRemoved);
        QObjectPrivate::connect(d->m_model,
                                &QAbstractItemModel::destroyed,
                                d,
                                &QBarModelMapperPrivate::handleModelDestroyed);
    }
    Q_EMIT modelChanged();
}

QBarSeries *QBarModelMapper::series() const
{
    Q_D(const QBarModelMapper);
    return d->m_series;
}

void QBarModelMapper::setSeries(QBarSeries *series)
{
    Q_D(QBarModelMapper);
    if (d->m_series) {
        QObjectPrivate::disconnect(d->m_series,
                                   &QBarSeries::barsetsAdded,
                                   d,
                                   &QBarModelMapperPrivate::barSetsAdded);
        QObjectPrivate::disconnect(d->m_series,
                                   &QBarSeries::barsetsRemoved,
                                   d,
                                   &QBarModelMapperPrivate::barSetsRemoved);
        QObjectPrivate::disconnect(d->m_series,
                                   &QBarSeries::destroyed,
                                   d,
                                   &QBarModelMapperPrivate::handleSeriesDestroyed);
    }

    d->m_series = series;

    d->initializeBarsFromModel();
    if (d->m_series) {
        QObjectPrivate::connect(d->m_series,
                                &QBarSeries::barsetsAdded,
                                d,
                                &QBarModelMapperPrivate::barSetsAdded);
        QObjectPrivate::connect(d->m_series,
                                &QBarSeries::barsetsRemoved,
                                d,
                                &QBarModelMapperPrivate::barSetsRemoved);
        QObjectPrivate::connect(d->m_series,
                                &QBarSeries::destroyed,
                                d,
                                &QBarModelMapperPrivate::handleSeriesDestroyed);
    }
    Q_EMIT seriesChanged();
}

qsizetype QBarModelMapper::first() const
{
    Q_D(const QBarModelMapper);
    return d->m_first;
}

void QBarModelMapper::setFirst(qsizetype newFirst)
{
    Q_D(QBarModelMapper);
    d->m_first = qMax(newFirst, 0);
    d->initializeBarsFromModel();
    Q_EMIT firstChanged();
}

Qt::Orientation QBarModelMapper::orientation() const
{
    Q_D(const QBarModelMapper);
    return d->m_orientation;
}

void QBarModelMapper::setOrientation(Qt::Orientation orientation)
{
    Q_D(QBarModelMapper);
    d->m_orientation = orientation;
    d->initializeBarsFromModel();
    Q_EMIT orientationChanged();
}

qsizetype QBarModelMapper::count() const
{
    Q_D(const QBarModelMapper);
    return d->m_count;
}

void QBarModelMapper::setCount(qsizetype newCount)
{
    Q_D(QBarModelMapper);
    d->m_count = qMax(newCount, -1);
    d->initializeBarsFromModel();
    Q_EMIT countChanged();
}

qsizetype QBarModelMapper::lastBarSetSection() const
{
    Q_D(const QBarModelMapper);
    return d->m_lastBarSetSection;
}

void QBarModelMapper::setLastBarSetSection(qsizetype newLastBarSetSection)
{
    Q_D(QBarModelMapper);
    d->m_lastBarSetSection = qMax(-1, newLastBarSetSection);
    d->initializeBarsFromModel();
    Q_EMIT lastBarSetSectionChanged();
}

qsizetype QBarModelMapper::firstBarSetSection() const
{
    Q_D(const QBarModelMapper);
    return d->m_firstBarSetSection;
}

void QBarModelMapper::setFirstBarSetSection(qsizetype newFirstBarSetSection)
{
    Q_D(QBarModelMapper);
    d->m_firstBarSetSection = qMax(-1, newFirstBarSetSection);
    d->initializeBarsFromModel();
    Q_EMIT firstBarSetSectionChanged();
}

void QBarModelMapper::onValuesAdded(qsizetype index, qsizetype count)
{
    Q_D(QBarModelMapper);

    if (d->m_seriesSignalsBlock)
        return;
    d->handleValuesAdded(qobject_cast<QBarSet *>(sender()), index, count);
}

void QBarModelMapper::onBarLabelChanged()
{
    Q_D(QBarModelMapper);

    if (d->m_seriesSignalsBlock)
        return;
    d->handleBarLabelChanged(qobject_cast<QBarSet *>(sender()));
}

void QBarModelMapper::onBarValueChanged(qsizetype index)
{
    Q_D(QBarModelMapper);

    if (d->m_seriesSignalsBlock)
        return;
    d->handleBarValueChanged(qobject_cast<QBarSet *>(sender()), index);
}

/////////////////////////////////////////////////////////////////////////////////////
QBarModelMapperPrivate::QBarModelMapperPrivate() {}

QBarModelMapperPrivate::~QBarModelMapperPrivate() {}

QModelIndex QBarModelMapperPrivate::barModelIndex(qsizetype barSection, qsizetype posInBar)
{
    if (m_count != -1 && posInBar >= m_count)
        return QModelIndex(); // invalid

    if (barSection < m_firstBarSetSection || barSection > m_lastBarSetSection)
        return QModelIndex(); // invalid

    if (m_orientation == Qt::Vertical)
        return m_model->index(int(posInBar) + m_first, int(barSection));
    else
        return m_model->index(int(barSection), int(posInBar) + m_first);
}

void QBarModelMapperPrivate::blockSeriesSignals(const bool block)
{
    m_seriesSignalsBlock = block;
}

void QBarModelMapperPrivate::blockModelSignals(const bool block)
{
    m_modelSignalsBlock = block;
}

QBarSet *QBarModelMapperPrivate::barSet(QModelIndex index)
{
    if (!index.isValid())
        return nullptr;

    if (m_orientation == Qt::Vertical && index.column() >= m_firstBarSetSection
        && index.column() <= m_lastBarSetSection) {
        if (index.row() >= m_first && (m_count == -1 || index.row() < m_first + m_count))
            return m_barSets.value(index.column() - m_firstBarSetSection, nullptr);

    } else if (m_orientation == Qt::Horizontal && index.row() >= m_firstBarSetSection
               && index.row() <= m_lastBarSetSection) {
        if (index.column() >= m_first && (m_count == -1 || index.column() < m_first + m_count))
            return m_barSets.value(index.row() - m_firstBarSetSection, nullptr);
    }
    return nullptr; // This part of model has not been mapped to any bar set
}

qreal QBarModelMapperPrivate::barValue(qsizetype section, qsizetype posInBar)
{
    const QModelIndex index = barModelIndex(section, posInBar);
    if (!index.isValid())
        return 0.0;
    return m_model->data(index, Qt::DisplayRole).toDouble();
}

// Number of values each bar set should currently have, based on the model size and the mapping.
qsizetype QBarModelMapperPrivate::mappedValueCount() const
{
    if (m_model == nullptr)
        return 0;
    qsizetype count = (m_orientation == Qt::Vertical ? m_model->rowCount() : m_model->columnCount())
                      - m_first;
    count = qMax(count, qsizetype(0));
    if (m_count != -1)
        count = qMin(count, m_count);
    return count;
}

// Brings the bar set to the size the mapping allows, reading missing values from the model.
// Series signals must be blocked by the caller.
void QBarModelMapperPrivate::fillValues(QBarSet *set)
{
    const qsizetype barSetIndex = m_barSets.indexOf(set);
    if (barSetIndex < 0)
        return;

    const qsizetype target = mappedValueCount();
    const qsizetype current = set->count();
    if (current > target) {
        set->remove(target, current - target);
    } else if (current < target) {
        const qsizetype section = barSetIndex + m_firstBarSetSection;
        QList<qreal> values;
        values.reserve(target - current);
        for (qsizetype pos = current; pos < target; ++pos)
            values.append(barValue(section, pos));
        set->append(values);
    }
}

// Sets that shifted to another model section need the label of that section. Series signals must
// be blocked by the caller.
void QBarModelMapperPrivate::refreshLabels()
{
    const Qt::Orientation headerOrientation = m_orientation == Qt::Vertical ? Qt::Horizontal
                                                                            : Qt::Vertical;
    for (qsizetype i = 0; i < m_barSets.size(); ++i) {
        m_barSets.at(i)->setLabel(
            m_model->headerData(m_firstBarSetSection + i, headerOrientation).toString());
    }
}

void QBarModelMapperPrivate::connectBarSet(QBarSet *set)
{
    Q_Q(QBarModelMapper);
    QObjectPrivate::connect(set,
                            &QBarSet::valuesRemoved,
                            this,
                            &QBarModelMapperPrivate::valuesRemoved);
    QObject::connect(set, &QBarSet::valuesAdded, q, &QBarModelMapper::onValuesAdded);
    QObject::connect(set, &QBarSet::valueChanged, q, &QBarModelMapper::onBarValueChanged);
    QObject::connect(set, &QBarSet::labelChanged, q, &QBarModelMapper::onBarLabelChanged);
}

void QBarModelMapperPrivate::disconnectBarSet(QBarSet *set)
{
    Q_Q(QBarModelMapper);
    QObjectPrivate::disconnect(set,
                               &QBarSet::valuesRemoved,
                               this,
                               &QBarModelMapperPrivate::valuesRemoved);
    QObject::disconnect(set, &QBarSet::valuesAdded, q, &QBarModelMapper::onValuesAdded);
    QObject::disconnect(set, &QBarSet::valueChanged, q, &QBarModelMapper::onBarValueChanged);
    QObject::disconnect(set, &QBarSet::labelChanged, q, &QBarModelMapper::onBarLabelChanged);
}

// Creates a bar set for the model section and fills it with the mapped values. The set is not
// added to the series or to m_barSets. Returns nullptr if the section is not mapped.
QBarSet *QBarModelMapperPrivate::createBarSet(qsizetype section)
{
    if (!barModelIndex(section, 0).isValid())
        return nullptr;

    QBarSet *barSet = new QBarSet(
        m_model->headerData(section, m_orientation == Qt::Vertical ? Qt::Horizontal : Qt::Vertical)
            .toString(),
        m_series);
    QList<qreal> values;
    for (qsizetype pos = 0; barModelIndex(section, pos).isValid(); ++pos)
        values.append(barValue(section, pos));
    barSet->append(values);
    connectBarSet(barSet);
    return barSet;
}

// The values of every bar set live in the model rows (vertical) or columns (horizontal), so
// a change there means the same change in every bar set. Series signals must be blocked by the
// caller.
void QBarModelMapperPrivate::insertData(qsizetype start, qsizetype end)
{
    if (m_model == nullptr || m_series == nullptr)
        return;

    if (m_barSets.isEmpty()) {
        initializeBarsFromModel();
        return;
    }

    Q_TRACE_SCOPE(QGraphs2DBarModelMapperInsertData, int(start), int(end));

    // Positions before the first mapped row shift the whole window, which means that the new
    // values enter at the beginning of the bar sets.
    const qsizetype pos = qMax(start - m_first, qsizetype(0));
    if (m_count != -1 && pos >= m_count)
        return;

    const qsizetype added = end - start + 1;
    for (qsizetype i = 0; i < m_barSets.size(); ++i) {
        QBarSet *set = m_barSets.at(i);
        const qsizetype section = m_firstBarSetSection + i;
        const qsizetype at = qMin(pos, set->count());
        if (at == set->count()) {
            QList<qreal> values;
            for (qsizetype k = 0; k < added && barModelIndex(section, at + k).isValid(); ++k)
                values.append(barValue(section, at + k));
            if (!values.isEmpty())
                set->append(values);
        } else {
            for (qsizetype k = 0; k < added && barModelIndex(section, at + k).isValid(); ++k)
                set->insert(at + k, barValue(section, at + k));
        }
        // Drop the values that were pushed out of the mapped range
        fillValues(set);
    }
}

void QBarModelMapperPrivate::removeData(qsizetype start, qsizetype end)
{
    if (m_model == nullptr || m_series == nullptr)
        return;

    if (m_barSets.isEmpty()) {
        initializeBarsFromModel();
        return;
    }

    Q_TRACE_SCOPE(QGraphs2DBarModelMapperRemoveData, int(start), int(end));

    const qsizetype pos = qMax(start - m_first, qsizetype(0));
    const qsizetype removed = end - start + 1;
    for (QBarSet *set : std::as_const(m_barSets)) {
        if (pos < set->count())
            set->remove(pos, qMin(removed, set->count() - pos));
        // Refill from the rows that moved into the mapped range
        fillValues(set);
    }

    // No values left to show
    if (mappedValueCount() == 0)
        initializeBarsFromModel();
}

// Model sections inserted into/before the mapped range create new bar sets and push the last
// ones out of the mapped range. Series signals must be blocked by the caller.
void QBarModelMapperPrivate::insertBarSets(qsizetype start, qsizetype end)
{
    if (m_model == nullptr || m_series == nullptr)
        return;

    if (m_firstBarSetSection < 0 || m_lastBarSetSection < m_firstBarSetSection
        || start > m_lastBarSetSection) {
        return;
    }

    const qsizetype index = qMin(qMax(start - m_firstBarSetSection, qsizetype(0)),
                                 qsizetype(m_barSets.size()));
    const qsizetype added = end - start + 1;
    for (qsizetype i = 0; i < added; ++i) {
        const qsizetype section = m_firstBarSetSection + index + i;
        if (section > m_lastBarSetSection)
            break;
        QBarSet *set = createBarSet(section);
        if (!set)
            break;
        m_barSets.insert(index + i, set);
        m_series->insert(index + i, set);
    }

    const qsizetype maxSets = m_lastBarSetSection - m_firstBarSetSection + 1;
    while (m_barSets.size() > maxSets)
        m_series->remove(m_barSets.takeLast());

    refreshLabels();
}

// Model sections removed from the mapped range remove their bar sets, and the sections that
// shift into the range get new ones. Series signals must be blocked by the caller.
void QBarModelMapperPrivate::removeBarSets(qsizetype start, qsizetype end)
{
    if (m_model == nullptr || m_series == nullptr)
        return;

    if (m_firstBarSetSection < 0 || m_lastBarSetSection < m_firstBarSetSection
        || start > m_lastBarSetSection) {
        return;
    }

    const qsizetype index = qMax(start - m_firstBarSetSection, qsizetype(0));
    const qsizetype removed = end - start + 1;
    if (index < m_barSets.size()) {
        const qsizetype toRemove = qMin(removed, m_barSets.size() - index);
        for (qsizetype i = 0; i < toRemove; ++i)
            m_series->remove(m_barSets.takeAt(index));
    }

    // Sections that moved into the mapped range
    while (m_firstBarSetSection + m_barSets.size() <= m_lastBarSetSection) {
        QBarSet *set = createBarSet(m_firstBarSetSection + m_barSets.size());
        if (!set)
            break;
        m_barSets.append(set);
        m_series->append(set);
    }

    refreshLabels();
}

void QBarModelMapperPrivate::initializeBarsFromModel()
{
    if (m_model == nullptr || m_series == nullptr)
        return;
    blockSeriesSignals();
    m_series->clear();
    m_barSets.clear();

    Q_TRACE_SCOPE(QGraphs2DBarModelMapperInitBarsFromModel);

    // create the initial bar sets
    for (qsizetype i = m_firstBarSetSection; i <= m_lastBarSetSection; i++) {
        QBarSet *barSet = createBarSet(i);
        if (!barSet)
            break;
        m_series->append(barSet);
        m_barSets.append(barSet);
    }
    blockSeriesSignals(false);
}

void QBarModelMapperPrivate::modelUpdated(QModelIndex topLeft, QModelIndex bottomRight)
{
    if (m_model == nullptr || m_series == nullptr)
        return;

    if (m_modelSignalsBlock)
        return;

    blockSeriesSignals();
    QModelIndex index;
    for (int row = topLeft.row(); row <= bottomRight.row(); row++) {
        for (int column = topLeft.column(); column <= bottomRight.column(); column++) {
            index = topLeft.sibling(row, column);
            QBarSet *bar = barSet(index);
            if (bar) {
                if (m_orientation == Qt::Vertical)
                    bar->replace(row - m_first, m_model->data(index).toReal());
                else
                    bar->replace(column - m_first, m_model->data(index).toReal());
            }
        }
    }
    blockSeriesSignals(false);
}

void QBarModelMapperPrivate::modelHeaderDataUpdated(Qt::Orientation orientation, qsizetype first, qsizetype last)
{
    if (m_model == nullptr || m_series == nullptr)
        return;

    if (m_modelSignalsBlock)
        return;

    blockSeriesSignals();
    if (orientation != m_orientation) {
        for (int section = first; section <= last; section++) {
            if (section >= m_firstBarSetSection && section <= m_lastBarSetSection) {
                QBarSet *bar = m_barSets.value(section - m_firstBarSetSection, nullptr);
                if (bar)
                    bar->setLabel(m_model->headerData(section, orientation).toString());
            }
        }
    }
    blockSeriesSignals(false);
}

void QBarModelMapperPrivate::modelRowsAdded(QModelIndex parent, qsizetype start, qsizetype end)
{
    // Only the top level of the model is mapped
    if (parent.isValid() || m_modelSignalsBlock)
        return;

    blockSeriesSignals();
    if (m_orientation == Qt::Vertical)
        insertData(start, end);
    else
        insertBarSets(start, end);
    blockSeriesSignals(false);
}

void QBarModelMapperPrivate::modelRowsRemoved(QModelIndex parent, qsizetype start, qsizetype end)
{
    // Only the top level of the model is mapped
    if (parent.isValid() || m_modelSignalsBlock)
        return;

    blockSeriesSignals();
    if (m_orientation == Qt::Vertical)
        removeData(start, end);
    else
        removeBarSets(start, end);
    blockSeriesSignals(false);
}

void QBarModelMapperPrivate::modelColumnsAdded(QModelIndex parent, qsizetype start, qsizetype end)
{
    // Only the top level of the model is mapped
    if (parent.isValid() || m_modelSignalsBlock)
        return;

    blockSeriesSignals();
    if (m_orientation == Qt::Horizontal)
        insertData(start, end);
    else
        insertBarSets(start, end);
    blockSeriesSignals(false);
}

void QBarModelMapperPrivate::modelColumnsRemoved(QModelIndex parent, qsizetype start, qsizetype end)
{
    // Only the top level of the model is mapped
    if (parent.isValid() || m_modelSignalsBlock)
        return;

    blockSeriesSignals();
    if (m_orientation == Qt::Horizontal)
        removeData(start, end);
    else
        removeBarSets(start, end);
    blockSeriesSignals(false);
}

void QBarModelMapperPrivate::handleModelDestroyed()
{
    m_model = nullptr;
}

void QBarModelMapperPrivate::barSetsAdded(const QList<QBarSet *> &sets)
{
    if (m_seriesSignalsBlock)
        return;

    if (sets.size() == 0)
        return;

    int firstIndex = int(m_series->barSets().indexOf(sets.at(0)));
    if (firstIndex == -1)
        return;

    qsizetype maxCount = 0;
    for (int i = 0; i < sets.size(); i++)
        maxCount = qMax(maxCount, sets.at(i)->count());

    if (m_count != -1 && m_count < maxCount)
        m_count = maxCount;

    m_lastBarSetSection += sets.size();

    blockModelSignals();
    int modelCapacity = m_orientation == Qt::Vertical ? m_model->rowCount() - m_first
                                                      : m_model->columnCount() - m_first;
    if (maxCount > modelCapacity) {
        if (m_orientation == Qt::Vertical)
            m_model->insertRows(m_model->rowCount(), int(maxCount) - modelCapacity);
        else
            m_model->insertColumns(m_model->columnCount(), int(maxCount) - modelCapacity);
    }

    if (m_orientation == Qt::Vertical)
        m_model->insertColumns(firstIndex + m_firstBarSetSection, int(sets.size()));
    else
        m_model->insertRows(firstIndex + m_firstBarSetSection, int(sets.size()));

    for (int i = firstIndex + m_firstBarSetSection;
         i < firstIndex + m_firstBarSetSection + sets.size();
         i++) {
        m_model->setHeaderData(i,
                               m_orientation == Qt::Vertical ? Qt::Horizontal : Qt::Vertical,
                               sets.at(i - firstIndex - m_firstBarSetSection)->label());
        for (int j = 0; j < sets.at(i - firstIndex - m_firstBarSetSection)->count(); j++) {
            m_model->setData(barModelIndex(i, j),
                             sets.at(i - firstIndex - m_firstBarSetSection)->at(j));
        }
    }
    blockModelSignals(false);

    // The sets are already in the series, so keep them and only bring all the sets to equal size
    blockSeriesSignals();
    for (int i = 0; i < sets.size(); i++) {
        connectBarSet(sets.at(i));
        m_barSets.insert(firstIndex + i, sets.at(i));
    }
    for (QBarSet *set : std::as_const(m_barSets))
        fillValues(set);
    blockSeriesSignals(false);
}

void QBarModelMapperPrivate::barSetsRemoved(const QList<QBarSet *> &sets)
{
    if (m_seriesSignalsBlock)
        return;

    if (sets.size() == 0)
        return;

    int firstIndex = int(m_barSets.indexOf(sets.at(0)));
    if (firstIndex == -1)
        return;

    m_lastBarSetSection -= sets.size();

    for (qsizetype i = firstIndex + sets.size() - 1; i >= firstIndex; i--)
        m_barSets.removeAt(i);
    // The sets can outlive the removal (QBarSeries::take()), so they must not call us anymore
    for (QBarSet *set : sets)
        disconnectBarSet(set);

    blockModelSignals();
    if (m_orientation == Qt::Vertical)
        m_model->removeColumns(firstIndex + m_firstBarSetSection, int(sets.size()));
    else
        m_model->removeRows(firstIndex + m_firstBarSetSection, int(sets.size()));
    blockModelSignals(false);

    // Sections that moved into the mapped range after the removal
    blockSeriesSignals();
    while (m_firstBarSetSection + m_barSets.size() <= m_lastBarSetSection) {
        QBarSet *set = createBarSet(m_firstBarSetSection + m_barSets.size());
        if (!set)
            break;
        m_barSets.append(set);
        m_series->append(set);
    }
    refreshLabels();
    blockSeriesSignals(false);
}

void QBarModelMapperPrivate::handleValuesAdded(QBarSet *set, qsizetype index, qsizetype count)
{
    if (m_seriesSignalsBlock)
        return;

    qsizetype barSetIndex = m_barSets.indexOf(set);
    if (barSetIndex < 0)
        return;

    if (m_count != -1)
        m_count += count;

    blockModelSignals();
    if (m_orientation == Qt::Vertical)
        m_model->insertRows(int(index) + m_first, int(count));
    else
        m_model->insertColumns(int(index) + m_first, int(count));

    for (qsizetype j = index; j < index + count; j++) {
        m_model->setData(barModelIndex(barSetIndex + m_firstBarSetSection, int(j)),
                         m_barSets.at(barSetIndex)->at(j));
    }
    blockModelSignals(false);

    // All the bar sets have to stay the same size
    blockSeriesSignals();
    for (qsizetype i = 0; i < m_barSets.size(); i++) {
        QBarSet *other = m_barSets.at(i);
        if (other == set)
            continue;
        const qsizetype at = qMin(index, other->count());
        for (qsizetype j = 0; j < count; j++)
            other->insert(at + j, barValue(m_firstBarSetSection + i, index + j));
    }
    blockSeriesSignals(false);
}

void QBarModelMapperPrivate::valuesRemoved(qsizetype index, qsizetype count)
{
    if (m_seriesSignalsBlock)
        return;

    QBarSet *set = qobject_cast<QBarSet *>(q_func()->sender());
    if (!set || !m_barSets.contains(set))
        return;

    if (m_count != -1)
        m_count = qMax(m_count - count, qsizetype(0));

    blockModelSignals();
    if (m_orientation == Qt::Vertical)
        m_model->removeRows(int(index) + m_first, int(count));
    else
        m_model->removeColumns(int(index) + m_first, int(count));
    blockModelSignals(false);

    // All the bar sets have to stay the same size
    blockSeriesSignals();
    for (QBarSet *other : std::as_const(m_barSets)) {
        if (other != set && index < other->count())
            other->remove(index, count);
    }
    blockSeriesSignals(false);
}

void QBarModelMapperPrivate::handleBarLabelChanged(QBarSet *set)
{
    if (m_seriesSignalsBlock)
        return;

    Q_ASSERT(set);
    int barSetIndex = int(m_barSets.indexOf(set));
    if (barSetIndex < 0)
        return;

    blockModelSignals();
    m_model->setHeaderData(barSetIndex + m_firstBarSetSection,
                           m_orientation == Qt::Vertical ? Qt::Horizontal : Qt::Vertical,
                           m_barSets.at(barSetIndex)->label());
    blockModelSignals(false);
}

void QBarModelMapperPrivate::handleBarValueChanged(QBarSet *set, qsizetype index)
{
    if (m_seriesSignalsBlock)
        return;

    Q_ASSERT(set);
    int barSetIndex = int(m_barSets.indexOf(set));
    if (barSetIndex < 0)
        return;

    blockModelSignals();
    m_model->setData(barModelIndex(barSetIndex + m_firstBarSetSection, index),
                     m_barSets.at(barSetIndex)->at(index));
    blockModelSignals(false);
}

void QBarModelMapperPrivate::handleSeriesDestroyed()
{
    m_series = nullptr;
}

QT_END_NAMESPACE

#include "moc_qbarmodelmapper.cpp"
