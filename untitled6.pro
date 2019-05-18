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
    for_analysis.cpp \
    db_connection.cpp \
    _Contacts.cpp \
    _Crud.cpp \
    _Owners_tel.cpp \
    MyTableModel.cpp \
    add_Form.cpp \
    an_result_Form.cpp \
    analysis_Form.cpp \
    mainwindow_Form.cpp \
    search_Form.cpp \
    update_Form.cpp \
    officialtelephones_Form.cpp \
    _Official_tel.cpp

HEADERS += \
    db_connection.h \
    for_analysis.h \
    _Contacts.h \
    _Owners_tel.h \
    _Crud.h \
    MyTableModel.h \
    add_Form.h \
    an_result_Form.h \
    analysis_Form.h \
    mainwindow_Form.h \
    search_Form.h \
    update_Form.h \
    officialtelephones_Form.h \
    _Official_tel.h

FORMS += \
        mainwindow.ui \
    analysis.ui \
    update.ui \
    search.ui \
    add_form.ui \
    an_result.ui \
    officialtelephones.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
