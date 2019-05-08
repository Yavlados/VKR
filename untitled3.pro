#-------------------------------------------------
#
# Project created by QtCreator 2019-04-19T13:11:32
#
#-------------------------------------------------

QT       += core gui \
                 sql \

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = untitled3
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
        main.cpp \
        mainwindow.cpp \
    analysis.cpp \
    update.cpp \
    crud.cpp \
    owners_tel.cpp \
    contacts.cpp \
    telephones_upd.cpp \
    search.cpp \
    add_form.cpp \
    for_analysis.cpp \
    an_result.cpp \
    db_connection.cpp

HEADERS += \
        mainwindow.h \
    db_connection.h \
    crud.h \
    analysis.h \
    update.h \
    owners_tel.h \
    contacts.h \
    telephones_upd.h \
    search.h \
    add_form.h \
    for_analysis.h \
    an_result.h

FORMS += \
        mainwindow.ui \
    analysis.ui \
    update.ui \
    telephones_upd.ui \
    search.ui \
    add_form.ui \
    an_result.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
