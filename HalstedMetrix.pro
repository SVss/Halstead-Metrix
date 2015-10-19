QT += core
QT -= gui

TARGET = HalstedMetrix
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    cleaner.cpp \
    extractor.cpp \
    file_work.cpp \
    counter.cpp \
    regexes.cpp

HEADERS += \
    regexes.h \
    stuff.h \
    file_work.h \
    pascal_counter.h

