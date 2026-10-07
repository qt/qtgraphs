// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QStandardItemModel>
#include <QtGraphs/QPieModelMapper>
#include <QtGraphs/QPieSeries>
#include <QtGraphs/QPieSlice>
#include <QtTest/QtTest>

// Model that, like some custom/QML models, reports structural changes as a reset
class ResettingModel : public QStandardItemModel
{
public:
    using QStandardItemModel::QStandardItemModel;

    bool insertRows(int row, int count, const QModelIndex &parent = {}) override
    {
        const bool ok = QStandardItemModel::insertRows(row, count, parent);
        resetNow();
        return ok;
    }
    bool removeRows(int row, int count, const QModelIndex &parent = {}) override
    {
        const bool ok = QStandardItemModel::removeRows(row, count, parent);
        resetNow();
        return ok;
    }
    void resetNow()
    {
        beginResetModel();
        endResetModel();
    }
};

class tst_piemodelmapper : public QObject
{
    Q_OBJECT

public:
    tst_piemodelmapper();
    void createVerticalMapper();
    void createHorizontalMapper();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    void verticalMapper_data();
    void verticalMapper();
    void verticalMapperCustomMapping_data();
    void verticalMapperCustomMapping();
    void horizontalMapper_data();
    void horizontalMapper();
    void horizontalMapperCustomMapping_data();
    void horizontalMapperCustomMapping();
    void seriesUpdated();
    void verticalModelInsertRows();
    void verticalModelRemoveRows();
    void verticalModelInsertColumns();
    void verticalModelRemoveColumns();
    void horizontalModelInsertRows();
    void horizontalModelRemoveRows();
    void horizontalModelInsertColumns();
    void horizontalModelRemoveColumns();
    void modelUpdateCell();
    void verticalMapperSignals();
    void horizontalMapperSignals();
    void seriesEditDoesNotReinitialize();
    void externalModelResetReinitializes();
    void unchangedModelResetKeepsSeries();
    void unchangedSettersDoNothing();
    void sliceStyleSurvivesReconcile();
    void otherOrientationSectionShiftUpdatesInPlace();
    void setSeriesNullDetaches();
    void previousSeriesSlicesAreDisconnected();
    void setModelNullDetaches();

private:
    QStandardItemModel *m_model = nullptr;
    int m_modelRowCount = 10;
    int m_modelColumnCount = 8;
    QPieModelMapper *m_vMapper = nullptr;
    QPieModelMapper *m_hMapper = nullptr;

    QPieSeries *m_series = nullptr;
};

tst_piemodelmapper::tst_piemodelmapper() {}

void tst_piemodelmapper::createVerticalMapper()
{
    m_vMapper = new QPieModelMapper;
    QVERIFY(m_vMapper->model() == 0);
    m_vMapper->setValuesSection(0);
    m_vMapper->setLabelsSection(1);
    m_vMapper->setModel(m_model);
    m_vMapper->setSeries(m_series);
}

void tst_piemodelmapper::createHorizontalMapper()
{
    m_hMapper = new QPieModelMapper;
    QVERIFY(m_hMapper->model() == 0);
    m_hMapper->setOrientation(Qt::Horizontal);
    m_hMapper->setValuesSection(0);
    m_hMapper->setLabelsSection(1);
    m_hMapper->setModel(m_model);
    m_hMapper->setSeries(m_series);
}

void tst_piemodelmapper::init()
{
    m_series = new QPieSeries;

    m_model = new QStandardItemModel(m_modelRowCount, m_modelColumnCount, this);
    for (int row = 0; row < m_modelRowCount; ++row) {
        for (int column = 0; column < m_modelColumnCount; column++) {
            m_model->setData(m_model->index(row, column), row * column);
        }
    }
}

void tst_piemodelmapper::cleanup()
{
    m_series->deleteLater();
    m_series = 0;

    m_model->clear();
    m_model->deleteLater();
    m_model = 0;

    if (m_vMapper) {
        m_vMapper->deleteLater();
        m_vMapper = 0;
    }

    if (m_hMapper) {
        m_hMapper->deleteLater();
        m_hMapper = 0;
    }
}

void tst_piemodelmapper::initTestCase() {}

void tst_piemodelmapper::cleanupTestCase() {}

void tst_piemodelmapper::verticalMapper_data()
{
    QTest::addColumn<int>("valuesColumn");
    QTest::addColumn<int>("labelsColumn");
    QTest::addColumn<int>("expectedCount");
    QTest::newRow("different values and labels columns") << 0 << 1 << m_modelRowCount;
    QTest::newRow("same values and labels columns") << 1 << 1 << m_modelRowCount;
    QTest::newRow("invalid values column and correct labels column") << -3 << 1 << 0;
    QTest::newRow("values column beyond the size of model and correct labels column")
        << m_modelColumnCount << 1 << 0;
    QTest::newRow("values column beyond the size of model and invalid labels column")
        << m_modelColumnCount << -1 << 0;
}

void tst_piemodelmapper::verticalMapper()
{
    QFETCH(int, valuesColumn);
    QFETCH(int, labelsColumn);
    QFETCH(int, expectedCount);

    auto mapper = new QPieModelMapper;
    mapper->setValuesSection(valuesColumn);
    mapper->setLabelsSection(labelsColumn);
    mapper->setModel(m_model);
    mapper->setSeries(m_series);

    QCOMPARE(m_series->count(), expectedCount);
    QCOMPARE(mapper->valuesSection(), qMax(-1, valuesColumn));
    QCOMPARE(mapper->labelsSection(), qMax(-1, labelsColumn));

    delete mapper;
    mapper = 0;
}

void tst_piemodelmapper::verticalMapperCustomMapping_data()
{
    QTest::addColumn<int>("first");
    QTest::addColumn<int>("countLimit");
    QTest::addColumn<int>("expectedCount");
    QTest::newRow("first: 0, unlimited count") << 0 << -1 << m_modelRowCount;
    QTest::newRow("first: 3, unlimited count") << 3 << -1 << m_modelRowCount - 3;
    QTest::newRow("first: 0, count: 5") << 0 << 5 << qMin(5, m_modelRowCount);
    QTest::newRow("first: 3, count: 5") << 3 << 5 << qMin(5, m_modelRowCount - 3);
    QTest::newRow("first: +1 greater then the number of rows in the model, unlimited count")
        << m_modelRowCount + 1 << -1 << 0;
    QTest::newRow("first: +1 greater then the number of rows in the model, count: 5")
        << m_modelRowCount + 1 << 5 << 0;
    QTest::newRow("first: 0, count: +3 greater than the number of rows in the model (should limit "
                  "to the size of model)")
        << 0 << m_modelRowCount + 3 << m_modelRowCount;
    QTest::newRow("first: -3(invalid - should default to 0), unlimited count")
        << -3 << -1 << m_modelRowCount;
    QTest::newRow("first: 0, count: -3 (invalid - shlould default to -1)")
        << 0 << -3 << m_modelRowCount;
    QTest::newRow(
        "first: -3(invalid - should default to 0), count: -3 (invalid - shlould default to -1)")
        << -3 << -3 << m_modelRowCount;
}

void tst_piemodelmapper::verticalMapperCustomMapping()
{
    QFETCH(int, first);
    QFETCH(int, countLimit);
    QFETCH(int, expectedCount);

    QCOMPARE(m_series->count(), 0);

    auto mapper = new QPieModelMapper;
    mapper->setValuesSection(0);
    mapper->setLabelsSection(1);
    mapper->setModel(m_model);
    mapper->setSeries(m_series);
    mapper->setFirst(first);
    mapper->setCount(countLimit);

    QCOMPARE(m_series->count(), expectedCount);

    // change values column mapping to invalid
    mapper->setValuesSection(-1);
    mapper->setLabelsSection(1);

    QCOMPARE(m_series->count(), 0);

    delete mapper;
    mapper = 0;
}

void tst_piemodelmapper::horizontalMapper_data()
{
    QTest::addColumn<int>("valuesRow");
    QTest::addColumn<int>("labelsRow");
    QTest::addColumn<int>("expectedCount");
    QTest::newRow("different values and labels rows") << 0 << 1 << m_modelColumnCount;
    QTest::newRow("same values and labels rows") << 1 << 1 << m_modelColumnCount;
    QTest::newRow("invalid values row and correct labels row") << -3 << 1 << 0;
    QTest::newRow("values row beyond the size of model and correct labels row")
        << m_modelRowCount << 1 << 0;
    QTest::newRow("values row beyond the size of model and invalid labels row")
        << m_modelRowCount << -1 << 0;
}

void tst_piemodelmapper::horizontalMapper()
{
    QFETCH(int, valuesRow);
    QFETCH(int, labelsRow);
    QFETCH(int, expectedCount);

    auto mapper = new QPieModelMapper;
    mapper->setOrientation(Qt::Horizontal);
    mapper->setValuesSection(valuesRow);
    mapper->setLabelsSection(labelsRow);
    mapper->setModel(m_model);
    mapper->setSeries(m_series);

    QCOMPARE(m_series->count(), expectedCount);
    QCOMPARE(mapper->valuesSection(), qMax(-1, valuesRow));
    QCOMPARE(mapper->labelsSection(), qMax(-1, labelsRow));

    delete mapper;
    mapper = 0;
}

void tst_piemodelmapper::horizontalMapperCustomMapping_data()
{
    QTest::addColumn<int>("first");
    QTest::addColumn<int>("countLimit");
    QTest::addColumn<int>("expectedCount");
    QTest::newRow("first: 0, unlimited count") << 0 << -1 << m_modelColumnCount;
    QTest::newRow("first: 3, unlimited count") << 3 << -1 << m_modelColumnCount - 3;
    QTest::newRow("first: 0, count: 5") << 0 << 5 << qMin(5, m_modelColumnCount);
    QTest::newRow("first: 3, count: 5") << 3 << 5 << qMin(5, m_modelColumnCount - 3);
    QTest::newRow("first: +1 greater then the number of columns in the model, unlimited count")
        << m_modelColumnCount + 1 << -1 << 0;
    QTest::newRow("first: +1 greater then the number of columns in the model, count: 5")
        << m_modelColumnCount + 1 << 5 << 0;
    QTest::newRow("first: 0, count: +3 greater than the number of columns in the model (should "
                  "limit to the size of model)")
        << 0 << m_modelColumnCount + 3 << m_modelColumnCount;
    QTest::newRow("first: -3(invalid - should default to 0), unlimited count")
        << -3 << -1 << m_modelColumnCount;
    QTest::newRow("first: 0, count: -3 (invalid - shlould default to -1)")
        << 0 << -3 << m_modelColumnCount;
    QTest::newRow(
        "first: -3(invalid - should default to 0), count: -3 (invalid - shlould default to -1)")
        << -3 << -3 << m_modelColumnCount;
}

void tst_piemodelmapper::horizontalMapperCustomMapping()
{
    QFETCH(int, first);
    QFETCH(int, countLimit);
    QFETCH(int, expectedCount);

    QCOMPARE(m_series->count(), 0);

    auto mapper = new QPieModelMapper;
    mapper->setOrientation(Qt::Horizontal);
    mapper->setValuesSection(0);
    mapper->setLabelsSection(1);
    mapper->setModel(m_model);
    mapper->setSeries(m_series);
    mapper->setFirst(first);
    mapper->setCount(countLimit);

    QCOMPARE(m_series->count(), expectedCount);

    // change values row mapping to invalid
    mapper->setValuesSection(-1);
    mapper->setLabelsSection(1);

    QCOMPARE(m_series->count(), 0);

    delete mapper;
    mapper = 0;
}

void tst_piemodelmapper::seriesUpdated()
{
    // setup the mapper
    createVerticalMapper();
    QCOMPARE(m_series->count(), m_modelRowCount);
    QCOMPARE(m_vMapper->count(), -1);

    m_series->append("1000", 1000);
    QCOMPARE(m_series->count(), m_modelRowCount + 1);
    QCOMPARE(m_vMapper->count(),
             -1); // the value should not change as it indicates 'all' items there are in the model

    m_series->remove(m_series->slices().last());
    QCOMPARE(m_series->count(), m_modelRowCount);
    QCOMPARE(m_vMapper->count(),
             -1); // the value should not change as it indicates 'all' items there are in the model

    QPieSlice *slice = m_series->slices().first();
    slice->setValue(25.0);
    slice->setLabel(QString("25.0"));
    QCOMPARE(m_model->data(m_model->index(0, 0)).toReal(), 25.0);
    QCOMPARE(m_model->data(m_model->index(0, 1)).toString(), QString("25.0"));
}

void tst_piemodelmapper::verticalModelInsertRows()
{
    // setup the mapper
    createVerticalMapper();
    QCOMPARE(m_series->count(), m_modelRowCount);
    QVERIFY(m_vMapper->model() != 0);

    int insertCount = 4;
    m_model->insertRows(3, insertCount);
    QCOMPARE(m_series->count(), m_modelRowCount + insertCount);

    int first = 3;
    m_vMapper->setFirst(3);
    QCOMPARE(m_series->count(), m_modelRowCount + insertCount - first);

    m_model->insertRows(3, insertCount);
    QCOMPARE(m_series->count(), m_modelRowCount + 2 * insertCount - first);

    int countLimit = 6;
    m_vMapper->setCount(countLimit);
    QCOMPARE(m_series->count(), qMin(countLimit, m_modelRowCount + 2 * insertCount - first));

    m_model->insertRows(3, insertCount);
    QCOMPARE(m_series->count(), qMin(countLimit, m_modelRowCount + 3 * insertCount - first));

    m_vMapper->setFirst(0);
    QCOMPARE(m_series->count(), qMin(countLimit, m_modelRowCount + 3 * insertCount));

    m_vMapper->setCount(-1);
    QCOMPARE(m_series->count(), m_modelRowCount + 3 * insertCount);
}

void tst_piemodelmapper::verticalModelRemoveRows()
{
    // setup the mapper
    createVerticalMapper();
    QCOMPARE(m_series->count(), m_modelRowCount);
    QVERIFY(m_vMapper->model() != 0);

    int removeCount = 2;
    m_model->removeRows(1, removeCount);
    QCOMPARE(m_series->count(), m_modelRowCount - removeCount);

    int first = 1;
    m_vMapper->setFirst(first);
    QCOMPARE(m_series->count(), m_modelRowCount - removeCount - first);

    m_model->removeRows(1, removeCount);
    QCOMPARE(m_series->count(), m_modelRowCount - 2 * removeCount - first);

    int countLimit = 3;
    m_vMapper->setCount(countLimit);
    QCOMPARE(m_series->count(), qMin(countLimit, m_modelRowCount - 2 * removeCount - first));

    m_model->removeRows(1, removeCount);
    QCOMPARE(m_series->count(), qMin(countLimit, m_modelRowCount - 3 * removeCount - first));

    m_vMapper->setFirst(0);
    QCOMPARE(m_series->count(), qMin(countLimit, m_modelRowCount - 3 * removeCount));

    m_vMapper->setCount(-1);
    QCOMPARE(m_series->count(), m_modelRowCount - 3 * removeCount);
}

void tst_piemodelmapper::verticalModelInsertColumns()
{
    // setup the mapper
    createVerticalMapper();
    QCOMPARE(m_series->count(), m_modelRowCount);
    QVERIFY(m_vMapper->model() != 0);

    int insertCount = 4;
    m_model->insertColumns(3, insertCount);
    QCOMPARE(m_series->count(), m_modelRowCount);
}

void tst_piemodelmapper::verticalModelRemoveColumns()
{
    // setup the mapper
    createVerticalMapper();
    QCOMPARE(m_series->count(), m_modelRowCount);
    QVERIFY(m_vMapper->model() != 0);

    int removeCount = m_modelColumnCount - 2;
    m_model->removeColumns(0, removeCount);
    QCOMPARE(m_series->count(), m_modelRowCount);

    // leave only one column
    m_model->removeColumns(0, m_modelColumnCount - removeCount - 1);
    QCOMPARE(m_series->count(), 0);
}

void tst_piemodelmapper::horizontalModelInsertRows()
{
    // setup the mapper
    createHorizontalMapper();
    QCOMPARE(m_series->count(), m_modelColumnCount);
    QVERIFY(m_hMapper->model() != 0);

    int insertCount = 4;
    m_model->insertRows(3, insertCount);
    QCOMPARE(m_series->count(), m_modelColumnCount);
}

void tst_piemodelmapper::horizontalModelRemoveRows()
{
    // setup the mapper
    createHorizontalMapper();
    QCOMPARE(m_series->count(), m_modelColumnCount);
    QVERIFY(m_hMapper->model() != 0);

    int removeCount = m_modelRowCount - 2;
    m_model->removeRows(0, removeCount);
    QCOMPARE(m_series->count(), m_modelColumnCount);

    // leave only one column
    m_model->removeRows(0, m_modelRowCount - removeCount - 1);
    QCOMPARE(m_series->count(), 0);
}

void tst_piemodelmapper::horizontalModelInsertColumns()
{
    // setup the mapper
    createHorizontalMapper();
    QCOMPARE(m_series->count(), m_modelColumnCount);
    QVERIFY(m_hMapper->model() != 0);

    int insertCount = 4;
    m_model->insertColumns(3, insertCount);
    QCOMPARE(m_series->count(), m_modelColumnCount + insertCount);

    int first = 3;
    m_hMapper->setFirst(3);
    QCOMPARE(m_series->count(), m_modelColumnCount + insertCount - first);

    m_model->insertColumns(3, insertCount);
    QCOMPARE(m_series->count(), m_modelColumnCount + 2 * insertCount - first);

    int countLimit = 6;
    m_hMapper->setCount(countLimit);
    QCOMPARE(m_series->count(), qMin(countLimit, m_modelColumnCount + 2 * insertCount - first));

    m_model->insertColumns(3, insertCount);
    QCOMPARE(m_series->count(), qMin(countLimit, m_modelColumnCount + 3 * insertCount - first));

    m_hMapper->setFirst(0);
    QCOMPARE(m_series->count(), qMin(countLimit, m_modelColumnCount + 3 * insertCount));

    m_hMapper->setCount(-1);
    QCOMPARE(m_series->count(), m_modelColumnCount + 3 * insertCount);
}

void tst_piemodelmapper::horizontalModelRemoveColumns()
{
    // setup the mapper
    createHorizontalMapper();
    QCOMPARE(m_series->count(), m_modelColumnCount);
    QVERIFY(m_hMapper->model() != 0);

    int removeCount = 2;
    m_model->removeColumns(1, removeCount);
    QCOMPARE(m_series->count(), m_modelColumnCount - removeCount);

    int first = 1;
    m_hMapper->setFirst(first);
    QCOMPARE(m_series->count(), m_modelColumnCount - removeCount - first);

    m_model->removeColumns(1, removeCount);
    QCOMPARE(m_series->count(), m_modelColumnCount - 2 * removeCount - first);

    int countLimit = 3;
    m_hMapper->setCount(countLimit);
    QCOMPARE(m_series->count(), qMin(countLimit, m_modelColumnCount - 2 * removeCount - first));

    m_model->removeColumns(1, removeCount);
    QCOMPARE(m_series->count(), qMin(countLimit, m_modelColumnCount - 3 * removeCount - first));

    m_hMapper->setFirst(0);
    QCOMPARE(m_series->count(), qMin(countLimit, m_modelColumnCount - 3 * removeCount));

    m_hMapper->setCount(-1);
    QCOMPARE(m_series->count(), m_modelColumnCount - 3 * removeCount);
}

void tst_piemodelmapper::modelUpdateCell()
{
    // setup the mapper
    createVerticalMapper();

    QVERIFY(m_model->setData(m_model->index(1, 0), 44));
    QCOMPARE(m_series->slices().at(1)->value(), 44.0);
    QCOMPARE(m_model->data(m_model->index(1, 0)).toReal(), 44.0);
}

void tst_piemodelmapper::verticalMapperSignals()
{
    auto mapper = new QPieModelMapper;

    QSignalSpy spy0(mapper, SIGNAL(firstChanged()));
    QSignalSpy spy1(mapper, SIGNAL(countChanged()));
    QSignalSpy spy2(mapper, SIGNAL(valuesSectionChanged()));
    QSignalSpy spy3(mapper, SIGNAL(labelsSectionChanged()));
    QSignalSpy spy4(mapper, SIGNAL(modelChanged()));
    QSignalSpy spy5(mapper, SIGNAL(seriesChanged()));

    mapper->setValuesSection(0);
    mapper->setLabelsSection(1);
    mapper->setModel(m_model);
    mapper->setSeries(m_series);
    mapper->setFirst(1);
    mapper->setCount(5);

    QCOMPARE(spy0.size(), 1);
    QCOMPARE(spy1.size(), 1);
    QCOMPARE(spy2.size(), 1);
    QCOMPARE(spy3.size(), 1);
    QCOMPARE(spy4.size(), 1);
    QCOMPARE(spy5.size(), 1);

    delete mapper;
}

void tst_piemodelmapper::horizontalMapperSignals()
{
    auto mapper = new QPieModelMapper;

    QSignalSpy spy0(mapper, SIGNAL(firstChanged()));
    QSignalSpy spy1(mapper, SIGNAL(countChanged()));
    QSignalSpy spy2(mapper, SIGNAL(valuesSectionChanged()));
    QSignalSpy spy3(mapper, SIGNAL(labelsSectionChanged()));
    QSignalSpy spy4(mapper, SIGNAL(modelChanged()));
    QSignalSpy spy5(mapper, SIGNAL(seriesChanged()));

    mapper->setOrientation(Qt::Horizontal);
    mapper->setValuesSection(0);
    mapper->setLabelsSection(1);
    mapper->setModel(m_model);
    mapper->setSeries(m_series);
    mapper->setFirst(1);
    mapper->setCount(5);

    QCOMPARE(spy0.size(), 1);
    QCOMPARE(spy1.size(), 1);
    QCOMPARE(spy2.size(), 1);
    QCOMPARE(spy3.size(), 1);
    QCOMPARE(spy4.size(), 1);
    QCOMPARE(spy5.size(), 1);
}

void tst_piemodelmapper::seriesEditDoesNotReinitialize()
{
    auto *model = new ResettingModel(m_modelRowCount, m_modelColumnCount, this);
    for (int row = 0; row < m_modelRowCount; ++row) {
        for (int column = 0; column < m_modelColumnCount; ++column)
            model->setData(model->index(row, column), row * column);
    }

    m_vMapper = new QPieModelMapper;
    m_vMapper->setValuesSection(0);
    m_vMapper->setLabelsSection(1);
    m_vMapper->setModel(model);
    m_vMapper->setSeries(m_series);
    QCOMPARE(m_series->count(), m_modelRowCount);
    const QList<QPieSlice *> before = m_series->slices();

    // A full rebuild clears the series first, which emits removed.
    QSignalSpy removedSpy(m_series, &QPieSeries::removed);

    QPieSlice *added = m_series->append("added", 100);
    before.at(0)->setValue(25);
    QCOMPARE(removedSpy.size(), 0);
    QCOMPARE(m_series->count(), m_modelRowCount + 1);
    QCOMPARE(model->rowCount(), m_modelRowCount + 1);
    QCOMPARE(model->data(model->index(0, 0)).toReal(), 25.0);

    QVERIFY(m_series->remove(added));
    QCOMPARE(removedSpy.size(), 1);
    QCOMPARE(m_series->count(), m_modelRowCount);
    QCOMPARE(model->rowCount(), m_modelRowCount);
    QCOMPARE(m_series->slices(), before);

    delete m_vMapper;
    m_vMapper = nullptr;
    delete model;
}

void tst_piemodelmapper::externalModelResetReinitializes()
{
    auto *model = new ResettingModel(m_modelRowCount, m_modelColumnCount, this);
    m_vMapper = new QPieModelMapper;
    m_vMapper->setValuesSection(0);
    m_vMapper->setLabelsSection(1);
    m_vMapper->setModel(model);
    m_vMapper->setSeries(m_series);
    QCOMPARE(m_series->count(), m_modelRowCount);

    // Change the model content without telling the mapper, then report a reset
    model->blockSignals(true);
    model->setData(model->index(2, 0), 42);
    model->setData(model->index(2, 1), 43);
    model->blockSignals(false);
    QCOMPARE_NE(m_series->slices().at(2)->value(), 42.0);

    model->resetNow();
    QCOMPARE(m_series->count(), m_modelRowCount);
    QCOMPARE(m_series->slices().at(2)->value(), 42.0);
    QCOMPARE(m_series->slices().at(2)->label(), QString("43"));

    // A reset that changes the row count adds and removes slices to match
    model->blockSignals(true);
    model->removeRows(m_modelRowCount - 3, 3);
    model->blockSignals(false);
    model->resetNow();
    QCOMPARE(m_series->count(), m_modelRowCount - 3);

    model->blockSignals(true);
    model->insertRows(m_modelRowCount - 3, 5);
    model->blockSignals(false);
    model->resetNow();
    QCOMPARE(m_series->count(), m_modelRowCount + 2);

    delete m_vMapper;
    m_vMapper = nullptr;
    delete model;
}

void tst_piemodelmapper::unchangedModelResetKeepsSeries()
{
    auto *model = new ResettingModel(m_modelRowCount, m_modelColumnCount, this);
    m_vMapper = new QPieModelMapper;
    m_vMapper->setValuesSection(0);
    m_vMapper->setLabelsSection(1);
    m_vMapper->setModel(model);
    m_vMapper->setSeries(m_series);
    const QList<QPieSlice *> before = m_series->slices();

    QSignalSpy removedSpy(m_series, &QPieSeries::removed);
    QSignalSpy addedSpy(m_series, &QPieSeries::added);
    model->resetNow();
    QCOMPARE(removedSpy.size(), 0);
    QCOMPARE(addedSpy.size(), 0);
    QCOMPARE(m_series->count(), m_modelRowCount);
    QCOMPARE(m_series->slices(), before);

    delete m_vMapper;
    m_vMapper = nullptr;
    delete model;
}

void tst_piemodelmapper::unchangedSettersDoNothing()
{
    createVerticalMapper();
    const QList<QPieSlice *> before = m_series->slices();
    QSignalSpy removedSpy(m_series, &QPieSeries::removed);
    QSignalSpy addedSpy(m_series, &QPieSeries::added);
    QSignalSpy firstSpy(m_vMapper, &QPieModelMapper::firstChanged);
    QSignalSpy countSpy(m_vMapper, &QPieModelMapper::countChanged);
    QSignalSpy orientationSpy(m_vMapper, &QPieModelMapper::orientationChanged);
    QSignalSpy valuesSpy(m_vMapper, &QPieModelMapper::valuesSectionChanged);
    QSignalSpy labelsSpy(m_vMapper, &QPieModelMapper::labelsSectionChanged);
    QSignalSpy modelSpy(m_vMapper, &QPieModelMapper::modelChanged);
    QSignalSpy seriesSpy(m_vMapper, &QPieModelMapper::seriesChanged);

    m_vMapper->setFirst(0);
    m_vMapper->setCount(-1);
    m_vMapper->setOrientation(Qt::Vertical);
    m_vMapper->setValuesSection(0);
    m_vMapper->setLabelsSection(1);
    m_vMapper->setModel(m_model);
    m_vMapper->setSeries(m_series);

    QCOMPARE(removedSpy.size(), 0);
    QCOMPARE(addedSpy.size(), 0);
    QCOMPARE(firstSpy.size(), 0);
    QCOMPARE(countSpy.size(), 0);
    QCOMPARE(orientationSpy.size(), 0);
    QCOMPARE(valuesSpy.size(), 0);
    QCOMPARE(labelsSpy.size(), 0);
    QCOMPARE(modelSpy.size(), 0);
    QCOMPARE(seriesSpy.size(), 0);
    QCOMPARE(m_series->slices(), before);

    // A real change still takes effect
    m_vMapper->setCount(3);
    QCOMPARE(countSpy.size(), 1);
    QCOMPARE(m_series->count(), 3);
}

void tst_piemodelmapper::sliceStyleSurvivesReconcile()
{
    auto *model = new ResettingModel(m_modelRowCount, m_modelColumnCount, this);
    for (int row = 0; row < m_modelRowCount; ++row) {
        for (int column = 0; column < m_modelColumnCount; ++column)
            model->setData(model->index(row, column), row * column);
    }
    m_vMapper = new QPieModelMapper;
    m_vMapper->setValuesSection(0);
    m_vMapper->setLabelsSection(1);
    m_vMapper->setModel(model);
    m_vMapper->setSeries(m_series);

    QPieSlice *styled = m_series->slices().at(0);
    styled->setColor(Qt::magenta);
    styled->setExploded(true);

    // Mapping changes keep the slice object and its styling, and only refresh its data
    m_vMapper->setFirst(1);
    QCOMPARE(m_series->count(), m_modelRowCount - 1);
    QCOMPARE(m_series->slices().at(0), styled);
    QCOMPARE(styled->label(), QString("1"));
    QCOMPARE(styled->color(), QColor(Qt::magenta));
    QVERIFY(styled->isExploded());

    m_vMapper->setCount(5);
    QCOMPARE(m_series->count(), 5);
    QCOMPARE(m_series->slices().at(0), styled);
    QCOMPARE(styled->color(), QColor(Qt::magenta));

    m_vMapper->setCount(-1);
    QCOMPARE(m_series->count(), m_modelRowCount - 1);
    QCOMPARE(m_series->slices().at(0), styled);

    m_vMapper->setLabelsSection(2);
    QCOMPARE(m_series->slices().at(0), styled);
    QCOMPARE(styled->label(), QString("2"));
    QCOMPARE(styled->color(), QColor(Qt::magenta));

    model->resetNow();
    QCOMPARE(m_series->slices().at(0), styled);
    QCOMPARE(styled->color(), QColor(Qt::magenta));
    QVERIFY(styled->isExploded());

    delete m_vMapper;
    m_vMapper = nullptr;
    delete model;
}

void tst_piemodelmapper::otherOrientationSectionShiftUpdatesInPlace()
{
    createHorizontalMapper();
    QCOMPARE(m_series->count(), m_modelColumnCount);
    const QList<QPieSlice *> before = m_series->slices();
    QSignalSpy removedSpy(m_series, &QPieSeries::removed);
    QSignalSpy addedSpy(m_series, &QPieSeries::added);
    QCOMPARE(before.at(3)->label(), QString("3")); // row 1, column 3

    // The new empty row 0 shifts the labels row (1) onto what was row 0
    QVERIFY(m_model->insertRow(0));
    QCOMPARE(m_series->slices(), before);
    QCOMPARE(before.at(3)->label(), QString("0"));
    QCOMPARE(before.at(3)->value(), 0.0);

    // Removing it restores the old mapping
    QVERIFY(m_model->removeRow(0));
    QCOMPARE(m_series->slices(), before);
    QCOMPARE(before.at(3)->label(), QString("3"));

    QCOMPARE(removedSpy.size(), 0);
    QCOMPARE(addedSpy.size(), 0);
}

void tst_piemodelmapper::setSeriesNullDetaches()
{
    createVerticalMapper();
    QCOMPARE(m_series->count(), m_modelRowCount);
    const qreal value = m_series->slices().at(1)->value();

    m_vMapper->setSeries(nullptr);
    QCOMPARE(m_vMapper->series(), nullptr);

    // Neither side affects the other any more
    m_series->slices().at(0)->setValue(99);
    m_series->append("added", 1);
    QCOMPARE(m_model->rowCount(), m_modelRowCount);
    QCOMPARE(m_model->data(m_model->index(0, 0)).toReal(), 0.0);

    m_model->setData(m_model->index(1, 0), value + 5);
    QVERIFY(m_model->insertRow(0));
    QCOMPARE(m_series->slices().at(1)->value(), value);
    QCOMPARE(m_series->count(), m_modelRowCount + 1);

    // Setting a series again maps it from scratch
    m_vMapper->setSeries(m_series);
    QCOMPARE(m_vMapper->series(), m_series);
    QCOMPARE(m_series->count(), m_model->rowCount());
}

void tst_piemodelmapper::previousSeriesSlicesAreDisconnected()
{
    createVerticalMapper();
    m_vMapper->setFirst(1);
    QCOMPARE(m_series->count(), m_modelRowCount - 1);

    QList<QList<QVariant>> snapshot;
    for (int row = 0; row < m_model->rowCount(); ++row) {
        QList<QVariant> rowData;
        for (int column = 0; column < m_model->columnCount(); ++column)
            rowData.append(m_model->data(m_model->index(row, column)));
        snapshot.append(rowData);
    }
    auto modelUnchanged = [&] {
        for (int row = 0; row < m_model->rowCount(); ++row) {
            for (int column = 0; column < m_model->columnCount(); ++column) {
                if (m_model->data(m_model->index(row, column)) != snapshot.at(row).at(column))
                    return false;
            }
        }
        return m_model->rowCount() == snapshot.size();
    };

    auto *other = new QPieSeries;
    m_vMapper->setSeries(other);
    QCOMPARE(other->count(), m_modelRowCount - 1);

    // The old series keeps its data, but is no longer mapped to the model
    QCOMPARE(m_series->count(), m_modelRowCount - 1);
    m_series->slices().at(0)->setValue(99);
    m_series->slices().at(0)->setLabel("old");
    m_series->append("added", 1);
    m_series->remove(m_series->slices().at(2));
    QVERIFY(modelUnchanged());
    QCOMPARE(other->count(), m_modelRowCount - 1);

    // The new series is mapped
    other->slices().at(0)->setValue(7);
    QCOMPARE(m_model->data(m_model->index(1, 0)).toReal(), 7.0);

    delete other;
    QCOMPARE(m_vMapper->series(), nullptr);
}

void tst_piemodelmapper::setModelNullDetaches()
{
    createVerticalMapper();
    QCOMPARE(m_series->count(), m_modelRowCount);
    const qreal value = m_series->slices().at(1)->value();

    m_vMapper->setModel(nullptr);
    QCOMPARE(m_vMapper->model(), nullptr);

    // Neither side affects the other any more
    m_series->slices().at(0)->setValue(99);
    m_series->slices().at(0)->setLabel("changed");
    m_series->append("added", 1);
    QCOMPARE(m_model->rowCount(), m_modelRowCount);
    QCOMPARE(m_model->data(m_model->index(0, 0)).toReal(), 0.0);

    m_model->setData(m_model->index(1, 0), value + 5);
    QVERIFY(m_model->insertRow(0));
    QCOMPARE(m_series->slices().at(1)->value(), value);
    QCOMPARE(m_series->count(), m_modelRowCount + 1);

    // Setting a model again maps it from scratch
    m_vMapper->setModel(m_model);
    QCOMPARE(m_vMapper->model(), m_model);
    QCOMPARE(m_series->count(), m_model->rowCount());
}

QTEST_MAIN(tst_piemodelmapper)

#include "tst_piemodelmapper.moc"
