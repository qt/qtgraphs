TEMPLATE = app

QT += graphs quick quick3d

CONFIG += qmltypes
QML_IMPORT_NAME = SurfaceGalleryExample
QML_IMPORT_MAJOR_VERSION = 1

SOURCES += main.cpp \
           datasource.cpp
HEADERS += datasource.h

RESOURCES += surfacegallery.qrc

OTHER_FILES += doc/src/* \
               doc/images/* \
               qml/surfacegallery/*
