QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = contactbook
TEMPLATE = app

SOURCES += \
    main.cpp \
    widget.cpp

HEADERS += \
    widget.h

FORMS += \
    widget.ui