// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause
#include "bargraphwidget.h"

BarGraphWidget::BarGraphWidget() {}

Q3DBarsWidgetItem *BarGraphWidget::barGraphs() const
{
    return m_barGraphs;
}

void BarGraphWidget::initialize()
{
    m_barGraphs = new Q3DBarsWidgetItem();
    m_barGraphs->setWidget(this);
}
