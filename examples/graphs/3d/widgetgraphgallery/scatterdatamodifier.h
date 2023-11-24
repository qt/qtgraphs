// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef SCATTERDATAMODIFIER_H
#define SCATTERDATAMODIFIER_H

#include <QtGraphs/q3dscatter.h>
#include <QtGraphs/qabstract3dseries.h>

class ScatterDataModifier : public QObject
{
    Q_OBJECT

    enum InputState { StateNormal = 0, StateDraggingX, StateDraggingZ, StateDraggingY };

public:
    explicit ScatterDataModifier(Q3DScatter *scatter, QObject *parent);
    ~ScatterDataModifier();

    void addData();

public Q_SLOTS:
    void setBackgroundEnabled(int enabled);
    void setGridEnabled(int enabled);
    void setSmoothDots(int smooth);
    void changePresetCamera();
    void toggleItemCount();
    void toggleRanges();
    void adjustMinimumRange(float range);
    void adjustMaximumRange(float range);
    void changeStyle(int style);
    void changeTheme(int theme);
    void changeShadowQuality(int quality);
    void shadowQualityUpdatedByVisual(QAbstract3DGraph::ShadowQuality shadowQuality);
    void handleElementSelected(QAbstract3DGraph::ElementType type);
    void handleAxisDragging(QVector2D delta);

Q_SIGNALS:
    void backgroundEnabledChanged(bool enabled);
    void gridEnabledChanged(bool enabled);
    void shadowQualityChanged(int quality);

private:
    QVector3D randVector();
    Q3DScatter *m_graph = nullptr;
    QAbstract3DSeries::Mesh m_style = QAbstract3DSeries::Mesh::Sphere;
    bool m_smooth = true;
    int m_itemCount;
    float m_curveDivider;

    bool m_autoAdjust = true;

    InputState m_state = StateNormal;
    float m_dragSpeedModifier = 15.f;
};

#endif
