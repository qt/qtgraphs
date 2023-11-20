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
    RESOURCES
        qml/testbed/images/arrow_icon.png
        qml/testbed/images/icon_settings.png
)

install(TARGETS testbed
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
