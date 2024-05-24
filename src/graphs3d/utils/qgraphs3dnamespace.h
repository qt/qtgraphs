// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
#ifndef QGRAPHS3DNAMESPACE_H
#define QGRAPHS3DNAMESPACE_H
#include <QtCore/qobjectdefs.h>
#include <QtGraphs/qtgraphsexports.h>
#include <QtQmlIntegration/qqmlintegration.h>
QT_BEGIN_NAMESPACE
namespace QGraphs3D {
Q_GRAPHS_EXPORT Q_NAMESPACE
QML_NAMED_ELEMENT(Graphs3D)
enum SelectionFlag {
    SelectionNone = 0,
    SelectionItem = 1,
    SelectionRow = 2,
    SelectionItemAndRow = SelectionItem | SelectionRow,
    SelectionColumn = 4,
    SelectionItemAndColumn = SelectionItem | SelectionColumn,
    SelectionRowAndColumn = SelectionRow | SelectionColumn,
    SelectionItemRowAndColumn = SelectionItem | SelectionRow | SelectionColumn,
    SelectionSlice = 8,
    SelectionMultiSeries = 16
};
Q_FLAG_NS(SelectionFlag)
Q_DECLARE_FLAGS(SelectionFlags, SelectionFlag)

enum class ShadowQuality { None, Low, Medium, High, SoftLow, SoftMedium, SoftHigh };
Q_ENUM_NS(ShadowQuality)

enum class ElementType { None, Series, AxisXLabel, AxisYLabel, AxisZLabel, CustomItem };
Q_ENUM_NS(ElementType)

enum class OptimizationHint { Default, Legacy };
Q_ENUM_NS(OptimizationHint)

enum class RenderingMode { DirectToBackground, Indirect };
Q_ENUM_NS(RenderingMode)

enum class CameraPreset {
    NoPreset,
    FrontLow,
    Front,
    FrontHigh,
    LeftLow,
    Left,
    LeftHigh,
    RightLow,
    Right,
    RightHigh,
    BehindLow,
    Behind,
    BehindHigh,
    IsometricLeft,
    IsometricLeftHigh,
    IsometricRight,
    IsometricRightHigh,
    DirectlyAbove,
    DirectlyAboveCW45,
    DirectlyAboveCCW45,
    FrontBelow,
    LeftBelow,
    RightBelow,
    BehindBelow,
    DirectlyBelow
};
Q_ENUM_NS(CameraPreset)
} // namespace QGraphs3D
Q_DECLARE_OPERATORS_FOR_FLAGS(QGraphs3D::SelectionFlags)

QT_END_NAMESPACE
#endif // QGRAPHS3DNAMESPACE_H