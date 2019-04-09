#-------------------------------------------------
#
# Project created by QtCreator 2019-03-12T08:44:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Mandala-Ensicaen
TEMPLATE = app
 CONFIG += c++11


SOURCES += src/main.cpp\
    src/myQGraphicsView.cpp \
    src/widgetDrawLineWidth.cpp \
    src/mainWindow.cpp

HEADERS  += \
    include/myQGraphicsView.h \
    include/mainWindow.h \
    include/widgetDrawLineWidth.h

FORMS    += ui/mainwindow.ui

INCLUDEPATH = ./include

RESOURCES += \
    icons.qrc \
    translations.qrc\

OBJECTS_DIR = .obj
RCC_DIR = .qrc
UI_DIR = .ui
MOC_DIR = .moc
