#-------------------------------------------------
#
# Project created by QtCreator 2019-02-03T21:14:48
#
#-------------------------------------------------

lessThan(QT_MAJOR_VERSION, 4): error("requires Qt>4")
greaterThan(QT_MAJOR_VERSION, 5): error("requires Qt<6")

QT       += core gui widgets

TARGET = Algorithm_TD2_exo1
TEMPLATE = app
CONFIG += c++11

QMAKE_CXXFLAGS += -std=c++11
SOURCES += \
		exo1.cpp \
		../lib/mainwindow.cpp \
		../lib/thread.cpp \
		../lib/array.cpp \
		../lib/tp1.cpp \

HEADERS += \
		../lib/mainwindow.h \
		../lib/thread.h \
		../lib/array.h \
		../lib/tp1.h \

INCLUDEPATH += ../lib
