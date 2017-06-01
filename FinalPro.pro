TEMPLATE = app
TARGET = app

QT = core gui
QT += network widgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    main.cpp \
    server.cpp

HEADERS += \
    server.h
