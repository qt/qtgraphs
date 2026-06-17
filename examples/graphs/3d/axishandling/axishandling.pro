TEMPLATE = app

QT += graphs quick quick3d

CONFIG += qmltypes
QML_IMPORT_NAME = AxisHandlingExample
QML_IMPORT_MAJOR_VERSION = 1

SOURCES += main.cpp \
    customformatter.cpp

HEADERS += customformatter.h

RESOURCES += axishandling.qrc

OTHER_FILES += doc/src/* \
               doc/images/* \
               qml/axishandling/*
