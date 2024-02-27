# Copyright (C) 2023 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(testbed LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
  set(INSTALL_EXAMPLESDIR "examples")
endif()

set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}")

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Gui)
find_package(Qt6 COMPONENTS Quick)
find_package(Qt6 COMPONENTS Graphs)

qt_add_executable(testbed
    main.cpp
)
set_target_properties(testbed PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(testbed PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Quick
    Qt::Graphs
)

qt6_add_qml_module(testbed
    URI Testbed
    NO_RESOURCE_TARGET_PATH
    SOURCES
        custombarseries.h custombarseries.cpp
        customlineseries.h customlineseries.cpp
    QML_FILES
        qml/testbed/main.qml
        qml/testbed/StartupView.qml
        qml/testbed/SettingsView.qml
        qml/testbed/CustomCheckBox.qml
        qml/testbed/CustomLabel.qml
        qml/testbed/CustomSelectionBox.qml
        qml/testbed/CustomSlider.qml
        qml/testbed/BarThemes.qml
        qml/testbed/BarSelection.qml
        qml/testbed/LineProperties.qml
        qml/testbed/AxisGridSetup.qml
        qml/testbed/Callout.qml
        qml/testbed/ScatterProperties.qml
        qml/testbed/CppBarSeries.qml
        qml/testbed/CppLineSeries.qml
        qml/testbed/PieChart.qml
        qml/testbed/SplineSeries.qml
        qml/testbed/CustomBars.qml
        qml/testbed/BarsEffect/export/BarsEffect.qml
        qml/testbed/AreaSeries.qml
    RESOURCES
        qml/testbed/images/arrow_icon.png
        qml/testbed/images/icon_settings.png
        qml/testbed/images/happy_box.png
        qml/testbed/images/Sevillana-Regular.ttf
        qml/testbed/images/bar_fg_l.png
        qml/testbed/images/bar_fg_r.png
        qml/testbed/images/bar_fg_c.png
        qml/testbed/images/bar_mask_l.png
        qml/testbed/images/bar_mask_r.png
        qml/testbed/images/bar_mask_c.png
        qml/testbed/BarsEffect/export/barseffect.frag.qsb
        qml/testbed/BarsEffect/export/barseffect.vert.qsb
        qml/testbed/BarsEffect/export/bar_mask.png
)

install(TARGETS testbed
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
