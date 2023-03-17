#-------------------------------------------------
#
# Project created by QtCreator 2019-02-03T21:14:48
#
#-------------------------------------------------

lessThan(QT_MAJOR_VERSION, 4): error("requires Qt>4")

QT       += core gui widgets

TARGET = Algorithm_TD5_exo1
TEMPLATE = app
CONFIG += c++11

QMAKE_CXXFLAGS += -std=c++11
DEFINES += USE_NODE

SOURCES += \
		exo1.cpp \
        ../lib/mainwindow.cpp \
        ../lib/array.cpp \
        ../lib/binarytree.cpp \
        ../lib/hashtable.cpp \
        ../lib/heap.cpp \
        ../lib/thread.cpp \
		../lib/tp5.cpp \
		../lib/tp3.cpp

HEADERS += \
        ../lib/mainwindow.h \
        ../lib/array.h \
        ../lib/binarytree.h \
        ../lib/hashtable.cpp \
        ../lib/heap.h \
        ../lib/thread.h \
		../lib/tp5.h \
		../lib/tp3.h
        
INCLUDEPATH += ../lib
