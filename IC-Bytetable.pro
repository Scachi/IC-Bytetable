#-------------------------------------------------
#
# Project created by QtCreator 2019-07-04T02:52:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IC-Bytetable
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

CONFIG += c++11

SOURCES += \
        gpcreader.cpp \
        ic.cpp \
        icd.cpp \
        icmodel.cpp \
        icproxy.cpp \
        main.cpp \
        mainwindow.cpp \
        pmem.cpp \
        pmemd.cpp \
        pmemmodel.cpp \
        pmemwindow.cpp \
        xtra.cpp

HEADERS += \
        gpcreader.h \
        ic.h \
        icd.h \
        icmodel.h \
        icproxy.h \
        mainwindow.h \
        pmem.h \
        pmemd.h \
        pmemmodel.h \
        pmemwindow.h \
        xtra.h

FORMS += \
        mainwindow.ui \
        pmemwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

DISTFILES += \
    resources/Notes.txt \
    resources/ToDo.txt \
    resources/ic_info.xml

RC_FILE = IC-Bt.rc
