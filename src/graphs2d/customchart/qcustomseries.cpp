// Copyright (C) 2025 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGraphs/qcustomseries.h>
#include <private/charthelpers_p.h>
#include <private/qcustomseries_p.h>
#include <private/qcustomseriesdata_p.h>
#include <private/qgraphsview_p.h>

QT_BEGIN_NAMESPACE

/*!
    \class QCustomSeries
    \inmodule QtGraphs
    \brief The QCustomSeries class allows presenting customized graph types.
    \since 6.11

    //! TODO: Description
*/

/*!
    \qmltype CustomSeries
    \nativetype QCustomSeries
    \inqmlmodule QtGraphs
    \brief The CustomSeries type allows presenting customized graph types.
    \since 6.11

    //! TODO: Description
*/

QCustomSeries::QCustomSeries(QObject *parent)
    : QAbstractSeries(*(new QCustomSeriesPrivate()), parent)
{}

QCustomSeries::~QCustomSeries() {}

QCustomSeries::QCustomSeries(QCustomSeriesPrivate &dd, QObject *parent)
    : QAbstractSeries(dd, parent)
{}

QAbstractSeries::SeriesType QCustomSeries::type() const
{
    return QAbstractSeries::SeriesType::Custom;
}

QList<QCustomSeriesData *> QCustomSeries::dataItems() const
{
    Q_D(const QCustomSeries);
    return d->m_dataItems;
}

/*!
    \property QCustomSeries::delegate

    The delegate is used to insert elements to QML corresponding to provided
    custom data. Data is defined in the CustomSeries itself and given to
    the delegated elements through the "data" property.
*/
/*!
    \qmlproperty Component CustomSeries::delegate
    The delegate is used to insert elements to QML corresponding to provided
    custom data. Data is defined in the the CustomSeries itself and given to
    the delegated elements through the "data" property.

    \code
        delegate: Rectangle {
                property var data
                Rectangle {
                    color: "tan"
                    border.color: "black"
                    border.width: 4
                    y: series.mapY(data.index + 1) - height / 2 + 1
                    x: series.mapX(data.median) - width / 2
                    height: 32
                    width: Math.abs(series.mapX(data.upperQuartile) - series.mapX(data.lowerQuartile))
                }
            }
        }
    \endcode
*/
QQmlComponent *QCustomSeries::delegate() const
{
    Q_D(const QCustomSeries);
    return d->m_delegate;
}

void QCustomSeries::setDelegate(QQmlComponent *newDelegate)
{
    Q_D(QCustomSeries);
    if (d->m_delegate == newDelegate) {
        qCDebug(lcProperties2D) << "QCustomSeries::setDelegate. Point delegate is already set to:"
                                << newDelegate;
        return;
    }

    d->m_delegate = newDelegate;
    emit delegateChanged();
    emit update();
}

/*!
    \qmlmethod CustomSeries::mapX(real x)
    Returns \a x axis-space coordinate converted into render-space.
*/
/*!
    Returns \a x axis-space coordinate converted into render-space.
*/
qreal QCustomSeries::mapX(qreal x)
{
    Q_D(const QCustomSeries);
    if (d->m_graph)
        return d->m_graph->mapX(this, x);
    return x;
}

/*!
    \qmlmethod CustomSeries::mapY(real y)
    Returns \a y axis-space coordinate converted into render-space.
*/
/*!
    Returns \a y axis-space coordinate converted into render-space.
*/
qreal QCustomSeries::mapY(qreal y)
{
    Q_D(const QCustomSeries);
    if (d->m_graph)
        return d->m_graph->mapY(this, y);
    return y;
}

/*!
    \qmlmethod CustomSeries::append()
    Appends an empty data item to the series.
*/
/*!
    Appends an empty data item to the series.
*/
void QCustomSeries::append()
{
    append(QVariantMap());
}

/*!
    \qmlmethod CustomSeries::append(var data)
    Appends a data item defined by \a data to the series.
*/
/*!
    Appends a data item defined by \a data to the series.
*/
void QCustomSeries::append(QVariantMap data)
{
    Q_D(QCustomSeries);
    QCustomSeriesData *dataItem = new QCustomSeriesData(this);
    dataItem->setData(data);
    d->m_dataItems.append(dataItem);
}

/*!
    \qmlmethod CustomSeries::insert(int index)
    Appends an empty data item to the series at position \a index.
*/
/*!
    Appends an empty data item to the series at position \a index.
*/
void QCustomSeries::insert(qsizetype index)
{
    insert(index, QVariantMap());
}

/*!
    \qmlmethod CustomSeries::insert(int index, var data)
    Appends a data item defined by \a data to the series at position \a index.
*/
/*!
    Appends a data item defined by \a data to the series at position \a index.
*/
void QCustomSeries::insert(qsizetype index, QVariantMap data)
{
    Q_D(QCustomSeries);
    if (d->m_dataItems.size() <= index)
        return;

    QCustomSeriesData *dataItem = new QCustomSeriesData(this);
    dataItem->setData(data);
    d->m_dataItems.insert(index, dataItem);
}

/*!
    \qmlmethod CustomSeries::remove(int index)
    Remove a data item at position \a index.
*/
/*!
    Remove a data item at position \a index.
*/
void QCustomSeries::remove(qsizetype index)
{
    Q_D(QCustomSeries);
    if (d->m_dataItems.size() <= index)
        return;

    delete d->m_dataItems[index];

    d->m_dataItems.remove(index);
}

/*!
    \qmlmethod CustomSeries::clear()
    Clear all data items.
*/
/*!
    Clear all data items.
*/
void QCustomSeries::clear()
{
    Q_D(QCustomSeries);

    qDeleteAll(d->m_dataItems);

    d->m_dataItems.clear();
}

void QCustomSeries::componentComplete()
{
    Q_D(QCustomSeries);

    for (auto *child : children()) {
        if (auto dataItem = qobject_cast<QCustomSeriesData *>(child)) {
            if (!d->m_dataItems.contains(dataItem)) {
                d->m_dataItems.append(dataItem);
                qCDebug(lcSeries2D, "append dataItem to customSeries");
            }
        }
    }

    qCDebug(lcEvents2D, "QCustomSeries::componentComplete");

    QAbstractSeries::componentComplete();
}

/*!
    Called by the graph renderer to give access to the \a item
    representing the data, and the \a index corresponding to the
    data item.

    Override to implement custom property values for QQuickItem
    instance(s).
*/
void QCustomSeries::updateDelegate(QQuickItem *item, qsizetype index)
{
    auto &&data = dataItems()[index]->data();
    data[QStringLiteral("index")] = index;
    item->setProperty("data", data);
}

QCustomSeriesPrivate::QCustomSeriesPrivate()
    : QAbstractSeriesPrivate(QAbstractSeries::SeriesType::Custom)
{}

QT_END_NAMESPACE
