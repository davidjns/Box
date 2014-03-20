#-------------------------------------------------
#
# Project created by QtCreator 2014-02-03T21:55:59
#
#-------------------------------------------------

QT += core gui
QT += network
QT += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HW2
TEMPLATE = app


SOURCES += main.cpp\
        imagebox.cpp \
    boximage.cpp \
    undoaddimages.cpp \
    undoremoveimages.cpp \
    imagelabel.cpp \
    undoremoveimage.cpp \
    undocopy.cpp \
    undocut.cpp \
    undopaste.cpp \
    flickrcollector.cpp \
    undoaddflickr.cpp

HEADERS  += imagebox.h \
    boximage.h \
    undoaddimages.h \
    undoremoveimages.h \
    imagelabel.h \
    undoremoveimage.h \
    undocopy.h \
    undocut.h \
    undopaste.h \
    flickrcollector.h \
    undoaddflickr.h
