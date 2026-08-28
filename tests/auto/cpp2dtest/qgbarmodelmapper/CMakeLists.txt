if(NOT QT_BUILD_STANDALONE_TESTS AND NOT QT_BUILDING_QT)
    cmake_minimum_required(VERSION 3.16)
    project(tst_qgbarmodelmapper2d LANGUAGES CXX)
    find_package(Qt6BuildInternals REQUIRED COMPONENTS STANDALONE_TEST)
endif()

qt_internal_add_test(tst_qgbarmodelmapper2d
    SOURCES
        tst_barmodelmapper.cpp
    INCLUDE_DIRECTORIES
        ../common
    LIBRARIES
        Qt::Gui
        Qt::GuiPrivate
        Qt::Graphs
)
