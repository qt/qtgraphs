!include( ../examples.pri ) {
    error( "Couldn't find the examples.pri file!" )
}

SOURCES += main.cpp

RESOURCES += testbed.qrc

OTHER_FILES += qml/testbed/*
