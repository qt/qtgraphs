TEMPLATE = app

QT += graphs quick widgets \
    quickwidgets

SOURCES += main.cpp \
    piegraph.cpp \
    piewidget.cpp

HEADERS += piegraph.h \
    piewidget.h

OTHER_FILES += doc/src/* \
    doc/images/*
