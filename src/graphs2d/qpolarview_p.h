// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
// Qt-Security score:significant reason:default


#ifndef QPOLARVIEW_H
#define QPOLARVIEW_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the QtGraphs API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

#include <private/qgraphsview_p.h>

QT_BEGIN_NAMESPACE

class Q_GRAPHS_EXPORT QPolarView : public QGraphsView
{
    Q_OBJECT
    Q_PROPERTY(QPointF polarCenter READ polarCenter NOTIFY polarCenterChanged FINAL)
    Q_PROPERTY(qreal polarRadius READ polarRadius NOTIFY polarRadiusChanged FINAL)
    Q_PROPERTY(qreal labelPadding READ labelPadding WRITE setLabelPadding NOTIFY
                   labelPaddingChanged FINAL)
    QML_NAMED_ELEMENT(PolarView)
    QML_ADDED_IN_VERSION(6, 13)

public:
    explicit QPolarView(QQuickItem *parent = nullptr);
    ~QPolarView() override;

    bool isSeriesSupported(QObject *series) const override;

    bool addAxis(QAbstractAxis *axis) override;
    void createAxisRenderer() override;

    QPointF polarCenter() const;
    qreal polarRadius() const;

    qreal labelPadding() const;
    void setLabelPadding(qreal padding);

Q_SIGNALS:
    void polarCenterChanged();
    void polarRadiusChanged();
    void labelPaddingChanged();

protected:
    void updateComponentSizes() override;

private:
    QPointF m_polarCenter;
    qreal m_polarRadius = 0;
    qreal m_labelPadding = 8;
};

QT_END_NAMESPACE

#endif // QPOLARVIEW_H
