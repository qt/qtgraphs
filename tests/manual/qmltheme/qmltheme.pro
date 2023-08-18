!include( ../examples.pri ) {
    error( "Couldn't find the examples.pri file!" )
}

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp

RESOURCES += qmltheme.qrc

OTHER_FILES += qml/qmltheme/*
