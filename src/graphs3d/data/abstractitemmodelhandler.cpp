// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "abstractitemmodelhandler_p.h"

QT_BEGIN_NAMESPACE

AbstractItemModelHandler::AbstractItemModelHandler(QObject *parent)
    : QObject(parent)
    , resolvePending(0)
    , m_fullReset(true)
{
    m_resolveTimer.setSingleShot(true);
    QObject::connect(&m_resolveTimer,
                     &QTimer::timeout,
                     this,
                     &AbstractItemModelHandler::handlePendingResolve);
}

AbstractItemModelHandler::~AbstractItemModelHandler() {}

void AbstractItemModelHandler::setItemModel(QAbstractItemModel *itemModel)
{
    if (itemModel != m_itemModel.data()) {
        if (!m_itemModel.isNull())
            QObject::disconnect(m_itemModel, 0, this, 0);

        m_itemModel = itemModel;

        if (!m_itemModel.isNull()) {
            QObject::connect(m_itemModel.data(),
                             &QAbstractItemModel::columnsInserted,
                             this,
                             &AbstractItemModelHandler::handleColumnsInserted);
            QObject::connect(m_itemModel.data(),
                             &QAbstractItemModel::columnsMoved,
                             this,
                             &AbstractItemModelHandler::handleColumnsMoved);
            QObject::connect(m_itemModel.data(),
                             &QAbstractItemModel::columnsRemoved,
                             this,
                             &AbstractItemModelHandler::handleColumnsRemoved);
            QObject::connect(m_itemModel.data(),
                             &QAbstractItemModel::dataChanged,
                             this,
                             &AbstractItemModelHandler::handleDataChanged);
            QObject::connect(m_itemModel.data(),
                             &QAbstractItemModel::layoutChanged,
                             this,
                             &AbstractItemModelHandler::handleLayoutChanged);
            QObject::connect(m_itemModel.data(),
                             &QAbstractItemModel::modelReset,
                             this,
                             &AbstractItemModelHandler::handleModelReset);
            QObject::connect(m_itemModel.data(),
                             &QAbstractItemModel::rowsInserted,
                             this,
                             &AbstractItemModelHandler::handleRowsInserted);
            QObject::connect(m_itemModel.data(),
                             &QAbstractItemModel::rowsMoved,
                             this,
                             &AbstractItemModelHandler::handleRowsMoved);
            QObject::connect(m_itemModel.data(),
                             &QAbstractItemModel::rowsRemoved,
                             this,
                             &AbstractItemModelHandler::handleRowsRemoved);
        }
        if (!m_resolveTimer.isActive())
            m_resolveTimer.start(0);

        emit itemModelChanged(itemModel);
    }
}

QAbstractItemModel *AbstractItemModelHandler::itemModel() const
{
    return m_itemModel.data();
}

void AbstractItemModelHandler::handleColumnsInserted(const QModelIndex &parent, int start, int end)
{
    Q_UNUSED(parent);
    Q_UNUSED(start);
    Q_UNUSED(end);

    // Manipulating columns changes all rows in proxies that map rows/columns
    // directly, and its effects are not clearly defined in others -> always do
    // full reset.
    if (!m_resolveTimer.isActive()) {
        m_fullReset = true;
        m_resolveTimer.start(0);
    }
}

void AbstractItemModelHandler::handleColumnsMoved(const QModelIndex &sourceParent,
                                                  int sourceStart,
                                                  int sourceEnd,
                                                  const QModelIndex &destinationParent,
                                                  int destinationColumn)
{
    Q_UNUSED(sourceParent);
    Q_UNUSED(sourceStart);
    Q_UNUSED(sourceEnd);
    Q_UNUSED(destinationParent);
    Q_UNUSED(destinationColumn);

    // Manipulating columns changes all rows in proxies that map rows/columns
    // directly, and its effects are not clearly defined in others -> always do
    // full reset.
    if (!m_resolveTimer.isActive()) {
        m_fullReset = true;
        m_resolveTimer.start(0);
    }
}

void AbstractItemModelHandler::handleColumnsRemoved(const QModelIndex &parent, int start, int end)
{
    Q_UNUSED(parent);
    Q_UNUSED(start);
    Q_UNUSED(end);

    // Manipulating columns changes all rows in proxies that map rows/columns
    // directly, and its effects are not clearly defined in others -> always do
    // full reset.
    if (!m_resolveTimer.isActive()) {
        m_fullReset = true;
        m_resolveTimer.start(0);
    }
}

void AbstractItemModelHandler::handleDataChanged(const QModelIndex &topLeft,
                                                 const QModelIndex &bottomRight,
                                                 const QList<int> &roles)
{
    Q_UNUSED(topLeft);
    Q_UNUSED(bottomRight);
    Q_UNUSED(roles);

    // Default handling for dataChanged is to do full reset, as it cannot be
    // optimized in a general case, where we do not know which row/column/index
    // the item model item actually ended up to in the proxy.
    if (!m_resolveTimer.isActive()) {
        m_fullReset = true;
        m_resolveTimer.start(0);
    }
}

void AbstractItemModelHandler::handleLayoutChanged(const QList<QPersistentModelIndex> &parents,
                                                   QAbstractItemModel::LayoutChangeHint hint)
{
    Q_UNUSED(parents);
    Q_UNUSED(hint);

    // Resolve entire model if layout changes
    if (!m_resolveTimer.isActive()) {
        m_fullReset = true;
        m_resolveTimer.start(0);
    }
}

void AbstractItemModelHandler::handleModelReset()
{
    // Data cleared, reset array
    if (!m_resolveTimer.isActive()) {
        m_fullReset = true;
        m_resolveTimer.start(0);
    }
}

void AbstractItemModelHandler::handleRowsInserted(const QModelIndex &parent, int start, int end)
{
    Q_UNUSED(parent);
    Q_UNUSED(start);
    Q_UNUSED(end);

    // Default handling for rowsInserted is to do full reset, as it cannot be
    // optimized in a general case, where we do not know which row/column/index
    // the item model item actually ended up to in the proxy.
    if (!m_resolveTimer.isActive()) {
        m_fullReset = true;
        m_resolveTimer.start(0);
    }
}

void AbstractItemModelHandler::handleRowsMoved(const QModelIndex &sourceParent,
                                               int sourceStart,
                                               int sourceEnd,
                                               const QModelIndex &destinationParent,
                                               int destinationRow)
{
    Q_UNUSED(sourceParent);
    Q_UNUSED(sourceStart);
    Q_UNUSED(sourceEnd);
    Q_UNUSED(destinationParent);
    Q_UNUSED(destinationRow);

    // Default handling for rowsMoved is to do full reset, as it cannot be
    // optimized in a general case, where we do not know which row/column/index
    // the item model item actually ended up to in the proxy.
    if (!m_resolveTimer.isActive()) {
        m_fullReset = true;
        m_resolveTimer.start(0);
    }
}

void AbstractItemModelHandler::handleRowsRemoved(const QModelIndex &parent, int start, int end)
{
    Q_UNUSED(parent);
    Q_UNUSED(start);
    Q_UNUSED(end);

    // Default handling for rowsRemoved is to do full reset, as it cannot be
    // optimized in a general case, where we do not know which row/column/index
    // the item model item actually ended up to in the proxy.
    if (!m_resolveTimer.isActive()) {
        m_fullReset = true;
        m_resolveTimer.start(0);
    }
}

void AbstractItemModelHandler::handleMappingChanged()
{
    if (!m_resolveTimer.isActive())
        m_resolveTimer.start(0);
}

void AbstractItemModelHandler::handlePendingResolve()
{
    resolveModel();
    m_fullReset = false;
}

QT_END_NAMESPACE
