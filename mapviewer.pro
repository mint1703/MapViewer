QT       += core gui widgets network

CONFIG   += c++17

TARGET = MapViewer
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    mapwidget.cpp \
    utmconverter.cpp

HEADERS += \
    mainwindow.h \
    mapwidget.h \
    utmconverter.h
