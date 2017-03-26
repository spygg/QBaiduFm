#-------------------------------------------------
#
# Project created by QtCreator 2015-12-19T08:27:14
#
#-------------------------------------------------

QT       += core gui network multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QBaiduFm
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        widget.cpp \
    aristpicture.cpp \
    lyrics.cpp \
    channelswidget.cpp \
    logindlg.cpp \
    volpannel.cpp \
    getbaiduchannel.cpp \
    getsongplaylistid.cpp \
    getsongreallink.cpp \
    getaristpic.cpp \
    getlrc.cpp \
    getcirclescaledpixmap.cpp \
    eventlabel.cpp \
    downloadfile.cpp


HEADERS  += widget.h \
    aristpicture.h \
    lyrics.h \
    channelswidget.h \
    logindlg.h \
    volpannel.h \
    getbaiduchannel.h \
    getsongplaylistid.h \
    getsongreallink.h \
    getaristpic.h \
    getlrc.h \
    getcirclescaledpixmap.h \
    eventlabel.h \
    downloadfile.h


FORMS    += widget.ui \
    aristpicture.ui \
    lyrics.ui \
    channelswidget.ui \
    logindlg.ui

RESOURCES += \
    img.qrc
