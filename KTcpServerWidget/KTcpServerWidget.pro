#-------------------------------------------------
#
# Project created by QtCreator 2023-07-28T11:14:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = KTcpServerWidget
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += $$PWD/../../../Workspace/KVIS16/Src/KVIS16.0/KAIVipInclude/
SOURCES += \
        main.cpp \
        MainWindow.cpp \
    KTcpServer.cpp

HEADERS += \
        MainWindow.h \
    KTcpServer.h

FORMS += \
        MainWindow.ui

CONFIG(debug,debug|release){
LIBS  +=  -L$$PWD/../../../Workspace/KVIS16/UNIX/Bin/Debug/ -lKAIVipFramework
}else{
LIBS  +=  -L$$PWD/../../../Workspace/KVIS16/UNIX/Bin/Release/ -lKAIVipFramework
}

unix{
    CONFIG      += link_pkgconfig
    PKGCONFIG   += opencv4
}

