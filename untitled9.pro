    #-------------------------------------------------
#
# Project created by QtCreator 2019-05-20T18:04:24
#
#-------------------------------------------------

QT       += core gui \
                 sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = untitled7
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

win32:RC_ICONS += png/MainPic.ico

 CONFIG += c++11

# CONFIG += release

SOURCES += \
    EditPerson.cpp \
    Encrypt/simplecrypt.cpp \
    Form_Headers/PersonCard.cpp \
    New_db_MTM/_MTM_Contact.cpp \
    New_db_MTM/_MTM_Event.cpp \
    New_db_MTM/_MTM_Telephone.cpp \
    New_db_model/Analysis_models/analysisModels.cpp \
    New_db_model/_Contact.cpp \
    New_db_model/_Event.cpp \
    New_db_model/_Person.cpp \
    New_db_model/_Telephone.cpp \
    Singletons/Util.cpp \
    Table_CPP/_Off_tels.cpp \
    custom_date_edit.cpp \
    editEvent.cpp \
    Utility/for_import.cpp \
    Utility/for_search.cpp \
        main.cpp \
    Utility/for_analysis.cpp \
    db_connection.cpp \
    Form_CPP/an_result_Form.cpp \
    Form_CPP/analysis_Form.cpp \
    Form_CPP/mainwindow_Form.cpp \
    Form_CPP/search_Form.cpp \
    Form_CPP/officialtelephones_Form.cpp \
    Form_CPP/master_export_Form.cpp \
    Utility/for_export.cpp \
    Model_CPP/_MTM_Off_tels.cpp \
    Form_CPP/Import_Form.cpp \
    Form_CPP/master_import_Form.cpp \
    Form_CPP/settings_Form.cpp \
    settings_connection.cpp \
    olddbservice.cpp \
    dialog_conflict.cpp \
    table_show_delegate.cpp \
    component.cpp \
    dialog_settings.cpp \
    text_handler.cpp \
    date_form.cpp \
    customtableview.cpp \
    popup.cpp

HEADERS += \
    EditPerson.h \
    Encrypt/simplecrypt.h \
    Form_Headers/PersonCard.h \
    New_db_MTM/_MTM_Contact.h \
    New_db_MTM/_MTM_Event.h \
    New_db_MTM/_MTM_Telephone.h \
    New_db_model/Analysis_models/analysisModels.h \
    New_db_model/_Contact.h \
    New_db_model/_Event.h \
    New_db_model/_Person.h \
    New_db_model/_Telephone.h \
    New_db_model/enums.h \
    Singletons/Util.h \
    Singletons/index.h \
    custom_date_edit.h \
    db_connection.h \
    editEvent.h \
    Utility/for_analysis.h \
    Table_Headers/_Off_tels.h \
    Form_Headers/an_result_Form.h \
    Form_Headers/analysis_Form.h \
    Form_Headers/mainwindow_Form.h \
    Form_Headers/search_Form.h \
    Form_Headers/officialtelephones_Form.h \
    Form_Headers/master_export_Form.h \
    Utility/for_export.h \
    Model_Headers/_MTM_Off_tels.h \
    Form_Headers/Import_Form.h \
    Utility/for_import.h \
    Utility/for_search.h \
    Form_Headers/master_import_Form.h \
    Form_Headers/settings_Form.h \
    settings_connection.h \
    olddbservice.h \
    dialog_conflict.h \
    table_show_delegate.h \
    component.h \
    dialog_settings.h \
    text_handler.h \
    date_form.h \
    customtableview.h \
    popup.h

FORMS += \
    EditPerson.ui \
    Form_Headers/PersonCard.ui \
    editEvent.ui \
        mainwindow.ui \
    analysis.ui \
    search.ui \
    an_result.ui \
    officialtelephones.ui \
    master_export_form.ui \
    import_form.ui \
    master_import_form.ui \
    settings_form.ui \
    dialog_conflict.ui \
    component.ui \
    dialog_settings.ui \
    date_form.ui \
    popup.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

INCLUDEPATH += $$PWD\
               Table_CPP\
               Form_CPP\
               Model_CPP\
               Table_Headers\
               Form_Headers\
               Model_Headers\
               New_db_model\
               New_db_MTM\
               Singletons\
               New_db_model/Analysis_models\
               Encrypt\
               misc\
               Utility\
