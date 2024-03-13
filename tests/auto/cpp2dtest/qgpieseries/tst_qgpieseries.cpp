// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGraphs/QPieSeries>
#include <QtGraphs/QPieSlice>
#include <QtTest/QtTest>

QT_USE_NAMESPACE

class tst_qgpieseries : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    void construct();
    void properties();
    void append();
    void insert();
    void remove();
    void take();
    void calculatedValues();
    void sliceSeries();
    void destruction();

private:
    void verifyCalculatedData(const QPieSeries &series, bool *ok);
    QList<QPoint> slicePoints(QRectF rect);

private:
    QPieSeries *m_series;
};

void tst_qgpieseries::initTestCase() {}

void tst_qgpieseries::cleanupTestCase() {}

void tst_qgpieseries::init()
{
    m_series = new QPieSeries();
}

void tst_qgpieseries::cleanup()
{
    delete m_series;
}

void tst_qgpieseries::construct()
{
    QPieSeries *series = new QPieSeries();
    QVERIFY(series);
    delete series;
}

void tst_qgpieseries::properties()
{
    QSignalSpy countSpy(m_series, SIGNAL(countChanged()));
    QSignalSpy sumSpy(m_series, SIGNAL(sumChanged()));
    QSignalSpy opacitySpy(m_series, SIGNAL(opacityChanged()));

    QVERIFY(m_series->type() == QAbstractSeries::SeriesTypePie);
    QVERIFY(m_series->count() == 0);
    QVERIFY(m_series->isEmpty());
    QCOMPARE(m_series->sum(), 0.0);
    QCOMPARE(m_series->horizontalPosition(), 0.5);
    QCOMPARE(m_series->verticalPosition(), 0.5);
    QCOMPARE(m_series->size(), 0.7);
    QCOMPARE(m_series->startAngle(), 0.0);
    QCOMPARE(m_series->endAngle(), 360.0);
    QCOMPARE(m_series->opacity(), 1.0);

    m_series->append("s1", 1);
    m_series->append("s2", 1);
    m_series->append("s3", 1);
    m_series->insert(1, new QPieSlice("s4", 1));
    m_series->remove(m_series->slices().first());
    QCOMPARE(m_series->count(), 3);
    QCOMPARE(m_series->sum(), 3.0);
    m_series->clear();
    QCOMPARE(m_series->count(), 0);
    QCOMPARE(m_series->sum(), 0.0);
    QCOMPARE(countSpy.size(), 6);
    QCOMPARE(sumSpy.size(), 6);

    m_series->setSize(-1.0);
    QCOMPARE(m_series->size(), 0.0);
    m_series->setSize(0.0);
    m_series->setSize(0.9);
    m_series->setSize(2.0);
    QCOMPARE(m_series->size(), 1.0);

    m_series->setSize(0.7);
    QCOMPARE(m_series->size(), 0.7);

    m_series->setHoleSize(-1.0);
    QCOMPARE(m_series->holeSize(), 0.0);
    m_series->setHoleSize(0.5);
    QCOMPARE(m_series->holeSize(), 0.5);

    m_series->setHoleSize(0.8);
    QCOMPARE(m_series->holeSize(), 0.8);
    QCOMPARE(m_series->size(), 0.8);

    m_series->setSize(0.4);
    QCOMPARE(m_series->size(), 0.4);
    QCOMPARE(m_series->holeSize(), 0.4);

    m_series->setStartAngle(0);
    m_series->setStartAngle(-180);
    m_series->setStartAngle(180);
    QCOMPARE(m_series->startAngle(), 180.0);

    m_series->setEndAngle(360);
    m_series->setEndAngle(-180);
    m_series->setEndAngle(180);
    QCOMPARE(m_series->endAngle(), 180.0);

    m_series->setHorizontalPosition(0.5);
    m_series->setHorizontalPosition(-1.0);
    QCOMPARE(m_series->horizontalPosition(), 0.0);
    m_series->setHorizontalPosition(1.0);
    m_series->setHorizontalPosition(2.0);
    QCOMPARE(m_series->horizontalPosition(), 1.0);

    m_series->setVerticalPosition(0.5);
    m_series->setVerticalPosition(-1.0);
    QCOMPARE(m_series->verticalPosition(), 0.0);
    m_series->setVerticalPosition(1.0);
    m_series->setVerticalPosition(2.0);
    QCOMPARE(m_series->verticalPosition(), 1.0);

    m_series->setOpacity(0.5);
    QCOMPARE(m_series->opacity(), 0.5);
    QCOMPARE(opacitySpy.size(), 1);
    m_series->setOpacity(0.0);
    QCOMPARE(m_series->opacity(), 0.0);
    QCOMPARE(opacitySpy.size(), 2);
    m_series->setOpacity(1.0);
    QCOMPARE(m_series->opacity(), 1.0);
    QCOMPARE(opacitySpy.size(), 3);
}

void tst_qgpieseries::append()
{
    QSignalSpy addedSpy(m_series, SIGNAL(added(QList<QPieSlice *>)));

    // append pointer
    QPieSlice *slice1 = 0;
    QVERIFY(!m_series->append(slice1));
    slice1 = new QPieSlice("slice 1", 1);
    QVERIFY(m_series->append(slice1));
    QVERIFY(!m_series->append(slice1));
    QCOMPARE(m_series->count(), 1);
    QCOMPARE(addedSpy.size(), 1);
    QList<QPieSlice *> added = qvariant_cast<QList<QPieSlice *> >(addedSpy.at(0).at(0));
    QCOMPARE(added.size(), 1);
    QCOMPARE(added.first(), slice1);

    // try to append same slice to another series
    QPieSeries series2;
    QVERIFY(!series2.append(slice1));

    // append pointer list
    QList<QPieSlice *> list;
    QVERIFY(!m_series->append(list));
    list << nullptr;
    QVERIFY(!m_series->append(list));
    list.clear();
    list << new QPieSlice("slice 2", 2);
    list << new QPieSlice("slice 3", 3);
    QVERIFY(m_series->append(list));
    QVERIFY(!m_series->append(list));
    QCOMPARE(m_series->count(), 3);
    QCOMPARE(addedSpy.size(), 2);
    added = qvariant_cast<QList<QPieSlice *> >(addedSpy.at(1).at(0));
    QCOMPARE(added.size(), 2);
    QCOMPARE(added, list);

    // append operator
    QPieSlice *slice4 = new QPieSlice("slice 4", 4);
    *m_series << slice4;
    *m_series << slice1; // fails because already added
    QCOMPARE(m_series->count(), 4);
    QCOMPARE(addedSpy.size(), 3);
    added = qvariant_cast<QList<QPieSlice *> >(addedSpy.at(2).at(0));
    QCOMPARE(added.size(), 1);
    QCOMPARE(added.first(), slice4);

    // append with params
    QPieSlice *slice5 = m_series->append("slice 5", 5);
    QVERIFY(slice5 != 0);
    QCOMPARE(slice5->value(), 5.0);
    QCOMPARE(slice5->label(), QString("slice 5"));
    QCOMPARE(m_series->count(), 5);
    QCOMPARE(addedSpy.size(), 4);
    added = qvariant_cast<QList<QPieSlice *> >(addedSpy.at(3).at(0));
    QCOMPARE(added.size(), 1);
    QCOMPARE(added.first(), slice5);

    // check slices
    QVERIFY(!m_series->isEmpty());
    for (int i=0; i < m_series->count(); i++) {
        QCOMPARE(m_series->slices().at(i)->value(), (qreal) i+1);
        QCOMPARE(m_series->slices().at(i)->label(), QString("slice ") + QString::number(i+1));
    }
}

void tst_qgpieseries::insert()
{
    QSignalSpy addedSpy(m_series, SIGNAL(added(QList<QPieSlice *>)));

    // insert one slice
    QPieSlice *slice1 = 0;
    QVERIFY(!m_series->insert(0, slice1));
    slice1 = new QPieSlice("slice 1", 1);
    QVERIFY(!m_series->insert(-1, slice1));
    QVERIFY(!m_series->insert(5, slice1));
    QVERIFY(m_series->insert(0, slice1));
    QVERIFY(!m_series->insert(0, slice1));
    QCOMPARE(m_series->count(), 1);
    QCOMPARE(addedSpy.size(), 1);
    QList<QPieSlice *> added = qvariant_cast<QList<QPieSlice *> >(addedSpy.at(0).at(0));
    QCOMPARE(added.size(), 1);
    QCOMPARE(added.first(), slice1);

    // try to insert same slice to another series
    QPieSeries series2;
    QVERIFY(!series2.insert(0, slice1));

    // add some more slices
    QPieSlice *slice2 = m_series->append("slice 2", 2);
    QPieSlice *slice4 = m_series->append("slice 4", 4);
    QCOMPARE(m_series->count(), 3);
    QCOMPARE(addedSpy.size(), 3);
    added = qvariant_cast<QList<QPieSlice *> >(addedSpy.at(1).at(0));
    QCOMPARE(added.size(), 1);
    QCOMPARE(added.first(), slice2);
    added = qvariant_cast<QList<QPieSlice *> >(addedSpy.at(2).at(0));
    QCOMPARE(added.size(), 1);
    QCOMPARE(added.first(), slice4);

    // insert between slices
    QPieSlice *slice3 = new QPieSlice("slice 3", 3);
    m_series->insert(2, slice3);
    QCOMPARE(m_series->count(), 4);
    QCOMPARE(addedSpy.size(), 4);
    added = qvariant_cast<QList<QPieSlice *> >(addedSpy.at(3).at(0));
    QCOMPARE(added.size(), 1);
    QCOMPARE(added.first(), slice3);

    // check slices
    for (int i=0; i < m_series->count(); i++) {
        QCOMPARE(m_series->slices().at(i)->value(), (qreal) i+1);
        QCOMPARE(m_series->slices().at(i)->label(), QString("slice ") + QString::number(i+1));
        QVERIFY(m_series->slices().at(i)->parent() == m_series);
    }
}

void tst_qgpieseries::remove()
{
    QSignalSpy removedSpy(m_series, SIGNAL(removed(QList<QPieSlice *>)));

    // add some slices
    QPieSlice *slice1 = m_series->append("slice 1", 1);
    QPieSlice *slice2 = m_series->append("slice 2", 2);
    QPieSlice *slice3 = m_series->append("slice 3", 3);
    QSignalSpy spy1(slice1, SIGNAL(destroyed()));
    QSignalSpy spy2(slice2, SIGNAL(destroyed()));
    QSignalSpy spy3(slice3, SIGNAL(destroyed()));
    QCOMPARE(m_series->count(), 3);

    // null pointer remove
    QVERIFY(!m_series->remove(0));

    // remove first
    QVERIFY(m_series->remove(slice1));
    QVERIFY(!m_series->remove(slice1));
    QCOMPARE(m_series->count(), 2);
    QCOMPARE(m_series->slices().at(0)->label(), slice2->label());
    QCOMPARE(removedSpy.size(), 1);
    QList<QPieSlice *> removed = qvariant_cast<QList<QPieSlice *> >(removedSpy.at(0).at(0));
    QCOMPARE(removed.size(), 1);
    QCOMPARE(static_cast<const void *>(removed.first()), static_cast<const void *>(slice1));

    // remove all
    m_series->clear();
    QVERIFY(m_series->isEmpty());
    QVERIFY(m_series->slices().isEmpty());
    QCOMPARE(m_series->count(), 0);
    QCOMPARE(removedSpy.size(), 2);
    removed = qvariant_cast<QList<QPieSlice *> >(removedSpy.at(1).at(0));
    QCOMPARE(removed.size(), 2);
    QCOMPARE(static_cast<const void *>(removed.first()), static_cast<const void *>(slice2));
    QCOMPARE(static_cast<const void *>(removed.last()), static_cast<const void *>(slice3));
}

void tst_qgpieseries::take()
{
    QSignalSpy removedSpy(m_series, SIGNAL(removed(QList<QPieSlice *>)));

    // add some slices
    QPieSlice *slice1 = m_series->append("slice 1", 1);
    QPieSlice *slice2 = m_series->append("slice 2", 2);
    m_series->append("slice 3", 3);
    QSignalSpy spy1(slice1, SIGNAL(destroyed()));
    QCOMPARE(m_series->count(), 3);

    // null pointer remove
    QVERIFY(!m_series->take(0));

    // take first
    QVERIFY(m_series->take(slice1));
    QCOMPARE(spy1.size(), 0);
    QVERIFY(slice1->parent() == m_series); // series is still the parent object
    QVERIFY(!m_series->take(slice1));
    QCOMPARE(m_series->count(), 2);
    QCOMPARE(m_series->slices().at(0)->label(), slice2->label());
    QCOMPARE(removedSpy.size(), 1);
    QList<QPieSlice *> removed = qvariant_cast<QList<QPieSlice *> >(removedSpy.at(0).at(0));
    QCOMPARE(removed.size(), 1);
    QCOMPARE(removed.first(), slice1);
}

void tst_qgpieseries::calculatedValues()
{
    QPieSlice *slice1 = new QPieSlice("slice 1", 1);
    QSignalSpy percentageSpy(slice1, SIGNAL(percentageChanged()));
    QSignalSpy startAngleSpy(slice1, SIGNAL(startAngleChanged()));
    QSignalSpy angleSpanSpy(slice1, SIGNAL(angleSpanChanged()));

    // add a slice
    m_series->append(slice1);
    bool ok;
    verifyCalculatedData(*m_series, &ok);
    if (!ok)
        return;
    QCOMPARE(percentageSpy.size(), 1);
    QCOMPARE(startAngleSpy.size(), 0);
    QCOMPARE(angleSpanSpy.size(), 1);

    // add some more slices
    QList<QPieSlice *> list;
    list << new QPieSlice("slice 2", 2);
    list << new QPieSlice("slice 3", 3);
    m_series->append(list);
    verifyCalculatedData(*m_series, &ok);
    if (!ok)
        return;
    QCOMPARE(percentageSpy.size(), 2);
    QCOMPARE(startAngleSpy.size(), 0);
    QCOMPARE(angleSpanSpy.size(), 2);

    // remove a slice
    m_series->remove(list.first()); // remove slice 2
    verifyCalculatedData(*m_series, &ok);
    if (!ok)
        return;
    QCOMPARE(percentageSpy.size(), 3);
    QCOMPARE(startAngleSpy.size(), 0);
    QCOMPARE(angleSpanSpy.size(), 3);

    // insert a slice
    m_series->insert(0, new QPieSlice("Slice 4", 4));
    verifyCalculatedData(*m_series, &ok);
    if (!ok)
        return;
    QCOMPARE(percentageSpy.size(), 4);
    QCOMPARE(startAngleSpy.size(), 1);
    QCOMPARE(angleSpanSpy.size(), 4);

    // modify pie angles
    m_series->setStartAngle(-90);
    m_series->setEndAngle(90);
    verifyCalculatedData(*m_series, &ok);
    if (!ok)
        return;
    QCOMPARE(percentageSpy.size(), 4);
    QCOMPARE(startAngleSpy.size(), 3);
    QCOMPARE(angleSpanSpy.size(), 6);

    // clear all
    m_series->clear();
    verifyCalculatedData(*m_series, &ok);
    if (!ok)
        return;
    QCOMPARE(percentageSpy.size(), 4);
    QCOMPARE(startAngleSpy.size(), 3);
    QCOMPARE(angleSpanSpy.size(), 6);
}

void tst_qgpieseries::verifyCalculatedData(const QPieSeries &series, bool *ok)
{
    *ok = false;

    qreal sum = 0;
    for (const QPieSlice *slice : series.slices())
        sum += slice->value();
    QCOMPARE(series.sum(), sum);

    qreal startAngle = series.startAngle();
    qreal pieAngleSpan = series.endAngle() - series.startAngle();
    for (const QPieSlice *slice : series.slices()) {
        qreal ratio = slice->value() / sum;
        qreal sliceSpan = pieAngleSpan * ratio;
        QCOMPARE(slice->startAngle(), startAngle);
        QCOMPARE(slice->angleSpan(), sliceSpan);
        QCOMPARE(slice->percentage(), ratio);
        startAngle += sliceSpan;
    }

    if (!series.isEmpty())
        QCOMPARE(series.slices().last()->startAngle() + series.slices().last()->angleSpan(), series.endAngle());

    *ok = true;
}

void tst_qgpieseries::sliceSeries()
{
    QPieSlice *slice = new QPieSlice();
    QVERIFY(!slice->series());
    delete slice;

    slice = new QPieSlice(m_series);
    QVERIFY(!slice->series());

    m_series->append(slice);
    QCOMPARE(slice->series(), m_series);

    slice = new QPieSlice();
    m_series->insert(0, slice);
    QCOMPARE(slice->series(), m_series);

    m_series->take(slice);
    QCOMPARE(slice->series(), nullptr);
}

void tst_qgpieseries::destruction()
{
    // add some slices
    QPieSlice *slice1 = m_series->append("slice 1", 1);
    QPieSlice *slice2 = m_series->append("slice 2", 2);
    QPieSlice *slice3 = m_series->append("slice 3", 3);
    QSignalSpy spy1(slice1, SIGNAL(destroyed()));
    QSignalSpy spy2(slice2, SIGNAL(destroyed()));
    QSignalSpy spy3(slice3, SIGNAL(destroyed()));

    // destroy series
    delete m_series;
    m_series = 0;

    // check that series has destroyed its slices
    QCOMPARE(spy1.size(), 1);
    QCOMPARE(spy2.size(), 1);
    QCOMPARE(spy3.size(), 1);
}

QList<QPoint> tst_qgpieseries::slicePoints(QRectF rect)
{
    qreal x1 = rect.topLeft().x() + (rect.width() / 4);
    qreal x2 = rect.topLeft().x() + (rect.width() / 4) * 3;
    qreal y1 = rect.topLeft().y() + (rect.height() / 4);
    qreal y2 = rect.topLeft().y() + (rect.height() / 4) * 3;
    QList<QPoint> points;
    points << QPoint(x2, y1);
    points << QPoint(x2, y2);
    points << QPoint(x1, y2);
    points << QPoint(x1, y1);
    return points;
}

QTEST_MAIN(tst_qgpieseries)
#include "tst_qgpieseries.moc"
