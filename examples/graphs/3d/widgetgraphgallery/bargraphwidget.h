// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause
#ifndef BARGRAPHWIDGET_H
#define BARGRAPHWIDGET_H

#include <QtGraphsWidgets/q3dbarswidgetitem.h>
#include <QtQuickWidgets/qquickwidget.h>

class BarGraphWidget : public QQuickWidget
{
    Q_OBJECT
public:
    BarGraphWidget();

    Q3DBarsWidgetItem *barGraphs() const;
    void initialize();

private:
    Q3DBarsWidgetItem *m_barGraphs = nullptr;
};

#endif // BARGRAPHWIDGET_H
