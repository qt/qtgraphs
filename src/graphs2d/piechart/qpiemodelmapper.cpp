// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
#include <QtCore/qabstractitemmodel.h>
#include <QtGraphs/qpiemodelmapper.h>
#include <QtGraphs/qpieseries.h>
#include <private/qpiemodelmapper_p.h>

QT_BEGIN_NAMESPACE

/*!
    \class QPieModelMapper
    \inmodule QtGraphs
    \ingroup graphs_2D
    \brief The QPieModelMapper is a model mapper for pie series.

    Model mappers enable using a data model derived from the QAbstractItemModel
    class as a data source for a chart. A vertical model mapper is used to create
    a connection between a data model and QPieSeries, so that each row in the
    data model defines a pie slice and each column maps to the label or the value
    of the pie slice.

    Both model and pie series properties can be used to manipulate the data. The
    model mapper keeps the pie series and the data model in sync.
*/
/*!
    \qmltype PieModelMapper
    \instantiates QPieModelMapper
    \inqmlmodule QtGraphs
    \ingroup graphs_qml_2D

    \brief Model mapper for pie series.

    Model mappers enable using a data model derived from the QAbstractItemModel
    class as a data source for a chart. A vertical model mapper is used to create
    a connection between a data model and PieSeries, so that each row in the data
    model defines a pie slice and each column maps to the label or the value of
    the pie slice.

    Both model and pie series properties can be used to manipulate the data. The
    model mapper keeps the pie series and the data model in sync.

    The following QML example creates a pie series with four slices (assuming
    the model has at least five rows). Each slice gets a label from column 1 and
    a value from column 2.
    \code
        PieModelMapper {
            series: pieSeries
            model: customModel
            labelsSection: 1
            valuesSection: 2
            firstRow: 1
            rowCount: 4
        }
    \endcode
*/

QPieModelMapper::~QPieModelMapper() {}

QPieModelMapper::QPieModelMapper(QObject *parent)
    : QObject{*(new QPieModelMapperPrivate), parent} {}

QPieModelMapper::QPieModelMapper(QPieModelMapperPrivate &dd, QObject *parent)
    : QObject(dd, parent) {}

void QPieModelMapper::onSliceLabelChanged() {
    Q_D(QPieModelMapper);
    d->onSliceLabelChanged(qobject_cast<QPieSlice *>(sender()));
}

void QPieModelMapper::onSliceValueChanged() {
    Q_D(QPieModelMapper);
    d->onSliceValueChanged(qobject_cast<QPieSlice *>(sender()));
}

QAbstractItemModel *QPieModelMapper::model() const {
    const Q_D(QPieModelMapper);
    return d->m_model;
}

void QPieModelMapper::setModel(QAbstractItemModel *model) {
    if (model == nullptr)
        return;
    Q_D(QPieModelMapper);
    if (d->m_model) {
        QObjectPrivate::disconnect(d->m_model,
                                   &QAbstractItemModel::modelReset,
                                   d,
                                   &QPieModelMapperPrivate::initializePieFromModel);
        QObjectPrivate::disconnect(d->m_model,
                                   &QAbstractItemModel::dataChanged,
                                   d,
                                   &QPieModelMapperPrivate::onModelUpdated);
        QObjectPrivate::disconnect(d->m_model,
                                   &QAbstractItemModel::rowsInserted,
                                   d,
                                   &QPieModelMapperPrivate::onModelRowsAdded);
        QObjectPrivate::disconnect(d->m_model,
                                   &QAbstractItemModel::rowsRemoved,
                                   d,
                                   &QPieModelMapperPrivate::onModelRowsRemoved);
        QObjectPrivate::disconnect(d->m_model,
                                   &QAbstractItemModel::columnsInserted,
                                   d,
                                   &QPieModelMapperPrivate::onModelColumnsAdded);
        QObjectPrivate::disconnect(d->m_model,
                                   &QAbstractItemModel::columnsRemoved,
                                   d,
                                   &QPieModelMapperPrivate::onModelColumnsRemoved);
        QObjectPrivate::disconnect(d->m_model,
                                   &QAbstractItemModel::destroyed,
                                   d,
                                   &QPieModelMapperPrivate::handleModelDestroyed);
    }
    d->m_model = model;
    d->initializePieFromModel();

    QObjectPrivate::connect(d->m_model,
                            &QAbstractItemModel::modelReset,
                            d,
                            &QPieModelMapperPrivate::initializePieFromModel);
    QObjectPrivate::connect(d->m_model,
                            &QAbstractItemModel::dataChanged,
                            d,
                            &QPieModelMapperPrivate::onModelUpdated);
    QObjectPrivate::connect(d->m_model,
                            &QAbstractItemModel::rowsInserted,
                            d,
                            &QPieModelMapperPrivate::onModelRowsAdded);
    QObjectPrivate::connect(d->m_model,
                            &QAbstractItemModel::rowsRemoved,
                            d,
                            &QPieModelMapperPrivate::onModelRowsRemoved);
    QObjectPrivate::connect(d->m_model,
                            &QAbstractItemModel::columnsInserted,
                            d,
                            &QPieModelMapperPrivate::onModelColumnsAdded);
    QObjectPrivate::connect(d->m_model,
                            &QAbstractItemModel::columnsRemoved,
                            d,
                            &QPieModelMapperPrivate::onModelColumnsRemoved);
    QObjectPrivate::connect(d->m_model,
                            &QAbstractItemModel::destroyed,
                            d,
                            &QPieModelMapperPrivate::handleModelDestroyed);
    Q_EMIT modelChanged();
}

QPieSeries *QPieModelMapper::series() const {
    const Q_D(QPieModelMapper);
    return d->m_series;
}

void QPieModelMapper::setSeries(QPieSeries *series) {
    Q_D(QPieModelMapper);
    if (d->m_series) {
        QObjectPrivate::disconnect(d->m_series,
                                   &QPieSeries::added,
                                   d,
                                   &QPieModelMapperPrivate::onSlicesAdded);
        QObjectPrivate::disconnect(d->m_series,
                                   &QPieSeries::removed,
                                   d,
                                   &QPieModelMapperPrivate::onSlicesRemoved);
        QObjectPrivate::disconnect(d->m_series,
                                   &QPieSeries::destroyed,
                                   d,
                                   &QPieModelMapperPrivate::handleSeriesDestroyed);
    }

    if (series == 0)
        return;

    d->m_series = series;
    d->initializePieFromModel();
    // connect the signals from the series
    QObjectPrivate::connect(d->m_series,
                            &QPieSeries::added,
                            d,
                            &QPieModelMapperPrivate::onSlicesAdded);
    QObjectPrivate::connect(d->m_series,
                            &QPieSeries::removed,
                            d,
                            &QPieModelMapperPrivate::onSlicesRemoved);
    QObjectPrivate::connect(d->m_series,
                            &QPieSeries::destroyed,
                            d,
                            &QPieModelMapperPrivate::handleSeriesDestroyed);
    Q_EMIT seriesChanged();
}

int QPieModelMapper::first() const {
    Q_D(const QPieModelMapper);
    return d->m_first;
}

void QPieModelMapper::setFirst(int first) {
    Q_D(QPieModelMapper);
    d->m_first = qMax(first, 0);
    d->initializePieFromModel();
    Q_EMIT firstChanged();
}

int QPieModelMapper::count() const {
    Q_D(const QPieModelMapper);
    return d->m_count;
}

void QPieModelMapper::setCount(int count) {
    Q_D(QPieModelMapper);
    d->m_count = qMax(count, -1);
    d->initializePieFromModel();
    Q_EMIT countChanged();
}

Qt::Orientation QPieModelMapper::orientation() const {
    Q_D(const QPieModelMapper);
    return d->m_orientation;
}

void QPieModelMapper::setOrientation(Qt::Orientation orientation) {
    Q_D(QPieModelMapper);
    d->m_orientation = orientation;
    d->initializePieFromModel();
    Q_EMIT orientationChanged();
}

int QPieModelMapper::valuesSection() const {
    Q_D(const QPieModelMapper);
    return d->m_valuesSection;
}

void QPieModelMapper::setValuesSection(int valuesSection) {
    Q_D(QPieModelMapper);
    d->m_valuesSection = qMax(-1, valuesSection);
    d->initializePieFromModel();
    Q_EMIT valuesSectionChanged();
}

int QPieModelMapper::labelsSection() const {
    Q_D(const QPieModelMapper);
    return d->m_labelsSection;
}

void QPieModelMapper::setLabelsSection(int labelsSection) {
    Q_D(QPieModelMapper);
    d->m_labelsSection = qMax(-1, labelsSection);
    d->initializePieFromModel();
    Q_EMIT labelsSectionChanged();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

QPieModelMapperPrivate::QPieModelMapperPrivate() {}

QPieModelMapperPrivate::~QPieModelMapperPrivate() {}

void QPieModelMapperPrivate::blockModelSignals(bool block) {
    m_modelSignalsBlock = block;
}

void QPieModelMapperPrivate::blockSeriesSignals(bool block) {
    m_seriesSignalsBlock = block;
}

QPieSlice *QPieModelMapperPrivate::pieSlice(QModelIndex index) const {
    if (!index.isValid())
        return 0; // index is invalid

    if (m_orientation == Qt::Vertical
        && (index.column() == m_valuesSection || index.column() == m_labelsSection)) {
        if (index.row() >= m_first && (m_count == -1 || index.row() < m_first + m_count)) {
            if (m_model->index(index.row(), m_valuesSection).isValid()
                && m_model->index(index.row(), m_labelsSection).isValid())
                return m_series->slices().at(index.row() - m_first);
            else
                return 0;
        }
    } else if (m_orientation == Qt::Horizontal
               && (index.row() == m_valuesSection || index.row() == m_labelsSection)) {
        if (index.column() >= m_first && (m_count == -1 || index.column() < m_first + m_count)) {
            if (m_model->index(m_valuesSection, index.column()).isValid()
                && m_model->index(m_labelsSection, index.column()).isValid())
                return m_series->slices().at(index.column() - m_first);
            else
                return 0;
        }
    }
    return 0; // This part of model has not been mapped to any slice
}

QModelIndex QPieModelMapperPrivate::valueModelIndex(int slicePos) {
    if (m_count != -1 && slicePos >= m_count)
        return QModelIndex(); // invalid

    if (m_orientation == Qt::Vertical)
        return m_model->index(slicePos + m_first, m_valuesSection);
    else
        return m_model->index(m_valuesSection, slicePos + m_first);
}

QModelIndex QPieModelMapperPrivate::labelModelIndex(int slicePos) {
    if (m_count != -1 && slicePos >= m_count)
        return QModelIndex(); // invalid

    if (m_orientation == Qt::Vertical)
        return m_model->index(slicePos + m_first, m_labelsSection);
    else
        return m_model->index(m_labelsSection, slicePos + m_first);
}

bool QPieModelMapperPrivate::isLabelIndex(QModelIndex index) const {
    if (m_orientation == Qt::Vertical && index.column() == m_labelsSection)
        return true;
    else if (m_orientation == Qt::Horizontal && index.row() == m_labelsSection)
        return true;

    return false;
}

bool QPieModelMapperPrivate::isValueIndex(QModelIndex index) const {
    if (m_orientation == Qt::Vertical && index.column() == m_valuesSection)
        return true;
    else if (m_orientation == Qt::Horizontal && index.row() == m_valuesSection)
        return true;

    return false;
}

void QPieModelMapperPrivate::onSlicesAdded(const QList<QPieSlice *> &slices) {
    if (m_seriesSignalsBlock)
        return;

    if (slices.size() == 0)
        return;

    int firstIndex = m_series->slices().indexOf(slices.at(0));
    if (firstIndex == -1)
        return;

    if (m_count != -1)
        m_count += slices.size();
    Q_Q(QPieModelMapper);
    for (int i = firstIndex; i < firstIndex + slices.size(); i++) {
        m_slices.insert(i, slices.at(i - firstIndex));
        QObject::connect(slices.at(i - firstIndex),
                         &QPieSlice::labelChanged,
                         q,
                         &QPieModelMapper::onSliceLabelChanged);
        QObject::connect(slices.at(i - firstIndex),
                         &QPieSlice::valueChanged,
                         q,
                         &QPieModelMapper::onSliceValueChanged);
    }

    blockModelSignals();
    if (m_orientation == Qt::Vertical)
        m_model->insertRows(firstIndex + m_first, slices.size());
    else
        m_model->insertColumns(firstIndex + m_first, slices.size());

    for (int i = firstIndex; i < firstIndex + slices.size(); i++) {
        m_model->setData(valueModelIndex(i), slices.at(i - firstIndex)->value());
        m_model->setData(labelModelIndex(i), slices.at(i - firstIndex)->label());
    }
    blockModelSignals(false);
}

void QPieModelMapperPrivate::onSlicesRemoved(const QList<QPieSlice *> &slices) {
    if (m_seriesSignalsBlock)
        return;

    if (slices.size() == 0)
        return;

    int firstIndex = m_slices.indexOf(slices.at(0));
    if (firstIndex == -1)
        return;

    if (m_count != -1)
        m_count -= slices.size();

    for (int i = firstIndex + slices.size() - 1; i >= firstIndex; i--)
        m_slices.removeAt(i);

    blockModelSignals();
    if (m_orientation == Qt::Vertical)
        m_model->removeRows(firstIndex + m_first, slices.size());
    else
        m_model->removeColumns(firstIndex + m_first, slices.size());
    blockModelSignals(false);
}

void QPieModelMapperPrivate::onSliceLabelChanged(const QPieSlice *slice) {
    if (m_seriesSignalsBlock)
        return;

    blockModelSignals();
    m_model->setData(labelModelIndex(m_series->slices().indexOf(slice)), slice->label());
    blockModelSignals(false);
}

void QPieModelMapperPrivate::onSliceValueChanged(const QPieSlice *slice) {
    if (m_seriesSignalsBlock)
        return;

    blockModelSignals();
    m_model->setData(valueModelIndex(m_series->slices().indexOf(slice)), slice->value());
    blockModelSignals(false);
}

void QPieModelMapperPrivate::handleSeriesDestroyed() { m_series = nullptr; }

void QPieModelMapperPrivate::onModelUpdated(QModelIndex topLeft, QModelIndex bottomRight)
{
    if (m_model == nullptr || m_series == nullptr)
        return;

    if (m_modelSignalsBlock)
        return;

    blockSeriesSignals();
    QModelIndex index;
    QPieSlice *slice;
    for (int row = topLeft.row(); row <= bottomRight.row(); row++) {
        for (int column = topLeft.column(); column <= bottomRight.column(); column++) {
            index = topLeft.sibling(row, column);
            slice = pieSlice(index);
            if (slice) {
                if (isValueIndex(index))
                    slice->setValue(m_model->data(index, Qt::DisplayRole).toReal());
                if (isLabelIndex(index))
                    slice->setLabel(m_model->data(index, Qt::DisplayRole).toString());
            }
        }
    }
    blockSeriesSignals(false);
}

void QPieModelMapperPrivate::onModelRowsAdded(QModelIndex parent, int start, int end)
{
    Q_UNUSED(parent);
    if (m_modelSignalsBlock)
        return;

    blockSeriesSignals();
    if (m_orientation == Qt::Vertical) {
        insertData(start, end);
    } else if (start <= m_valuesSection
               || start <= m_labelsSection) { // if the changes affect the map - reinitialize the pie
        initializePieFromModel();
    }
    blockSeriesSignals(false);
}

void QPieModelMapperPrivate::onModelRowsRemoved(QModelIndex parent, int start, int end)
{
    Q_UNUSED(parent);
    if (m_modelSignalsBlock)
        return;

    blockSeriesSignals();
    if (m_orientation == Qt::Vertical) {
        removeData(start, end);
    } else if (start <= m_valuesSection || start <= m_labelsSection) {
        // if the changes affect the map - reinitialize the pie
        initializePieFromModel();
    }
    blockSeriesSignals(false);
}

void QPieModelMapperPrivate::onModelColumnsAdded(QModelIndex parent, int start, int end)
{
    Q_UNUSED(parent);
    if (m_modelSignalsBlock)
        return;

    blockSeriesSignals();
    if (m_orientation == Qt::Horizontal) {
        insertData(start, end);
    } else if (start <= m_valuesSection || start <= m_labelsSection) {
        // if the changes affect the map - reinitialize the pie
        initializePieFromModel();
    }
    blockSeriesSignals(false);
}

void QPieModelMapperPrivate::onModelColumnsRemoved(QModelIndex parent, int start, int end)
{
    Q_UNUSED(parent);
    if (m_modelSignalsBlock)
        return;

    blockSeriesSignals();
    if (m_orientation == Qt::Horizontal) {
        removeData(start, end);
    } else if (start <= m_valuesSection || start <= m_labelsSection) {
        // if the changes affect the map - reinitialize the pie
        initializePieFromModel();
    }
    blockSeriesSignals(false);
}

void QPieModelMapperPrivate::handleModelDestroyed() { m_model = nullptr; }

void QPieModelMapperPrivate::insertData(int start, int end) {
    if (m_model == nullptr || m_series == nullptr)
        return;

    if (m_count != -1 && start >= m_first + m_count) {
        return;
    } else {
        int addedCount = end - start + 1;
        if (m_count != -1 && addedCount > m_count)
            addedCount = m_count;
        int first = qMax(start, m_first);
        int last = qMin(first + addedCount - 1,
                        m_orientation == Qt::Vertical ? m_model->rowCount() - 1
                                                      : m_model->columnCount() - 1);
        Q_Q(QPieModelMapper);

        for (int i = first; i <= last; i++) {
            QModelIndex valueIndex = valueModelIndex(i - m_first);
            QModelIndex labelIndex = labelModelIndex(i - m_first);
            if (valueIndex.isValid() && labelIndex.isValid()) {
                QPieSlice *slice = new QPieSlice;
                slice->setValue(m_model->data(valueIndex, Qt::DisplayRole).toDouble());
                slice->setLabel(m_model->data(labelIndex, Qt::DisplayRole).toString());
                QObject::connect(slice,
                                 &QPieSlice::labelChanged,
                                 q,
                                 &QPieModelMapper::onSliceLabelChanged);
                QObject::connect(slice,
                                 &QPieSlice::valueChanged,
                                 q,
                                 &QPieModelMapper::onSliceValueChanged);
                m_series->insert(i - m_first, slice);
                m_slices.insert(i - m_first, slice);
            }
        }

        // remove excess of slices (abouve m_count)
        if (m_count != -1 && m_series->slices().size() > m_count) {
            for (int i = m_series->slices().size() - 1; i >= m_count; i--) {
                m_series->remove(m_series->slices().at(i));
                m_slices.removeAt(i);
            }
        }
    }
}

void QPieModelMapperPrivate::removeData(int start, int end) {
    if (m_model == 0 || m_series == 0)
        return;

    int removedCount = end - start + 1;
    if (m_count != -1 && start >= m_first + m_count) {
        return;
    } else {
        int toRemove = qMin(m_series->slices().size(),
                            removedCount); // first find how many items can actually be removed
        int first = qMax(start,
                         m_first); // get the index of the first item that will be removed.
        int last = qMin(first + toRemove - 1,
                        m_series->slices().size() + m_first
                            - 1); // get the index of the last item that will be removed.
        for (int i = last; i >= first; i--) {
            m_series->remove(m_series->slices().at(i - m_first));
            m_slices.removeAt(i - m_first);
        }

        if (m_count != -1) {
            int itemsAvailable; // check how many are available to be added
            if (m_orientation == Qt::Vertical)
                itemsAvailable = m_model->rowCount() - m_first - m_series->slices().size();
            else
                itemsAvailable = m_model->columnCount() - m_first - m_series->slices().size();
            int toBeAdded = qMin(itemsAvailable,
                                 m_count
                                     - m_series->slices().size()); // add not more items than there
                                                                   // is space left to be filled.
            int currentSize = m_series->slices().size();
            if (toBeAdded > 0) {
                for (int i = m_series->slices().size(); i < currentSize + toBeAdded; i++) {
                    QModelIndex valueIndex = valueModelIndex(i - m_first);
                    QModelIndex labelIndex = labelModelIndex(i - m_first);
                    if (valueIndex.isValid() && labelIndex.isValid()) {
                        QPieSlice *slice = new QPieSlice;
                        slice->setValue(m_model->data(valueIndex, Qt::DisplayRole).toDouble());
                        slice->setLabel(m_model->data(labelIndex, Qt::DisplayRole).toString());
                        m_series->insert(i, slice);
                        m_slices.insert(i, slice);
                    }
                }
            }
        }
    }
}

void QPieModelMapperPrivate::initializePieFromModel() {
    if (m_model == nullptr || m_series == nullptr)
        return;

    blockSeriesSignals();
    // clear current content
    m_series->clear();
    m_slices.clear();

    // create the initial slices set
    int slicePos = 0;
    QModelIndex valueIndex = valueModelIndex(slicePos);
    QModelIndex labelIndex = labelModelIndex(slicePos);
    Q_Q(QPieModelMapper);
    while (valueIndex.isValid() && labelIndex.isValid()) {
        QPieSlice *slice = new QPieSlice;
        slice->setLabel(m_model->data(labelIndex, Qt::DisplayRole).toString());
        slice->setValue(m_model->data(valueIndex, Qt::DisplayRole).toDouble());
        QObject::connect(slice, &QPieSlice::labelChanged, q, &QPieModelMapper::onSliceLabelChanged);
        QObject::connect(slice, &QPieSlice::valueChanged, q, &QPieModelMapper::onSliceValueChanged);
        m_series->append(slice);
        m_slices.append(slice);
        slicePos++;
        valueIndex = valueModelIndex(slicePos);
        labelIndex = labelModelIndex(slicePos);
    }
    blockSeriesSignals(false);
}

QT_END_NAMESPACE
