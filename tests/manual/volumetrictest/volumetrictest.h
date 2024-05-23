// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef VOLUMETRICMODIFIER_H
#define VOLUMETRICMODIFIER_H

#include <QtGraphs/qcustom3dvolume.h>
#include <QtGraphs/qcustom3ditem.h>
#include <QtGraphsWidgets/q3dscatterwidget.h>
#include <QtGraphsWidgets/q3dsurfacewidget.h>
#include <QtGraphsWidgets/q3dbarswidget.h>
#include <QtWidgets/QLabel>

class VolumetricModifier : public QObject
{
    Q_OBJECT
public:
    explicit VolumetricModifier(QAbstract3DGraphWidget *scatter);
    ~VolumetricModifier();

    void setFpsLabel(QLabel *fpsLabel);
    void setSliceLabels(QLabel *xLabel, QLabel *yLabel, QLabel *zLabel);

public Q_SLOTS:
    void sliceX(int enabled);
    void sliceY(int enabled);
    void sliceZ(int enabled);
    void adjustSliceX(int value);
    void adjustSliceY(int value);
    void adjustSliceZ(int value);
    void handleFpsChange();
    void testSubtextureSetting();
    void adjustRangeX(int value);
    void adjustRangeY(int value);
    void adjustRangeZ(int value);
    void testBoundsSetting();

private:
    void createVolume();
    void createAnotherVolume();
    void createYetAnotherVolume();
    void checkRenderCase(int id, Qt::Axis axis, int index, const QList<uchar> &dataBefore,
                         QCustom3DVolume *volumeItem);

    QAbstract3DGraphWidget *m_graph;
    Q3DScatterWidget *m_scatterGraph;
    Q3DSurfaceWidget *m_surfaceGraph;
    Q3DBarsWidget *m_barGraph;
    QCustom3DVolume *m_volumeItem;
    QCustom3DVolume *m_volumeItem2;
    QCustom3DVolume *m_volumeItem3;
    QCustom3DItem *m_plainItem;
    int m_sliceIndexX;
    int m_sliceIndexY;
    int m_sliceIndexZ;
    QLabel *m_fpsLabel;
    QLabel *m_sliceLabelX;
    QLabel *m_sliceLabelY;
    QLabel *m_sliceLabelZ;
};

#endif
