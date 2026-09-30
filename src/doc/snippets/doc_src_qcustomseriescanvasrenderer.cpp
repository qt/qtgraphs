// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause
// Qt-Security score:significant reason:default

#include <QtCore/qlist.h>
#include <QtCanvasPainter/qcanvaspainteritem.h>
#include <QtGraphs/qcustomseries.h>
#include <QtGraphs/qcustomseriescanvasrenderer.h>

//! [customseries]
class ExampleCustomSeries : public QCustomSeries
{
public:
    explicit ExampleCustomSeries(QObject *parent = nullptr)
        : QCustomSeries(parent)
    {}

    struct CustomSeriesData
    {
        qreal x = 0;
        qreal y = 0;
        qreal size = 0;
        QColor color;
    };

    QList<CustomSeriesData> seriesData() const
    {
        return m_seriesData;
    }

private:
    QList<CustomSeriesData> m_seriesData;
};
//! [customseries]

//! [customseries_renderer]
class ExampleCustomSeriesRenderer : public QCustomSeriesCanvasRenderer
{
public:
    void synchronizeData(QCustomSeries *series, QCanvasPainterItem *item) override
    {
        Q_UNUSED(item);
        const auto customSeries = static_cast<ExampleCustomSeries *>(series);
        m_renderData.reserve(customSeries->seriesData().size());
        for (const auto &i : customSeries->seriesData())
            m_renderData.push_back({QRectF(i.x, i.y, i.size, i.size), i.color});
    }

    void canvasPaint(QCanvasPainter *p) override
    {
        for (const auto &i : std::as_const(m_renderData)) {
            p->setFillStyle(i.color);
            p->fillRect(i.rect);
        }
    }

private:
    struct CustomSeriesRenderData
    {
        QRectF rect;
        QColor color;
    };

    QList<CustomSeriesRenderData> m_renderData;
};
//! [customseries_renderer]
