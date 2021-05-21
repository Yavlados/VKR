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
    Forms/EditPerson.cpp \
    Encrypt/simplecrypt.cpp \
    Forms/PersonCard.cpp \
    New_db_MTM/_MTM_Contact.cpp \
    New_db_MTM/_MTM_Event.cpp \
    New_db_MTM/_MTM_Telephone.cpp \
    New_db_model/Analysis_models/analysisModels.cpp \
    New_db_model/_Contact.cpp \
    New_db_model/_Event.cpp \
    New_db_model/_Person.cpp \
    New_db_model/_Telephone.cpp \
    Singletons/Util.cpp \
    New_db_model/_Off_tels.cpp \
    Custom/custom_date_edit.cpp \
    Forms/editEvent.cpp \
    Utils/for_import.cpp \
    Utils/for_search.cpp \
        main.cpp \
    Utils/for_analysis.cpp \
    Singletons/db_connection.cpp \
    Forms/an_result_Form.cpp \
    Forms/analysis_Form.cpp \
    Forms/mainwindow_Form.cpp \
    Forms/search_Form.cpp \
    Forms/officialtelephones_Form.cpp \
    Forms/master_export_Form.cpp \
    Utils/for_export.cpp \
    New_db_MTM/_MTM_Off_tels.cpp \
    Forms/Import_Form.cpp \
    Forms/master_import_Form.cpp \
    Forms/settings_Form.cpp \
    Singletons/settings_connection.cpp \
    Utils/olddbservice.cpp \
    Custom/table_show_delegate.cpp \
    Forms/component.cpp \
    Forms/dialog_settings.cpp \
    Utils/text_handler.cpp \
    Forms/date_form.cpp \
    Custom/customtableview.cpp \
    Singletons/popup.cpp \
    Singletons/linksmanager.cpp \
    Forms/linkedperson.cpp

HEADERS += \
    Forms/EditPerson.h \
    Encrypt/simplecrypt.h \
    Forms/PersonCard.h \
    New_db_MTM/_MTM_Contact.h \
    New_db_MTM/_MTM_Event.h \
    New_db_MTM/_MTM_Telephone.h \
    New_db_model/Analysis_models/analysisModels.h \
    New_db_model/_Contact.h \
    New_db_model/_Event.h \
    New_db_model/_Person.h \
    New_db_model/_Telephone.h \
    Utils/enums.h \
    Singletons/Util.h \
    Custom/custom_date_edit.h \
    Singletons/db_connection.h \
    Forms/editEvent.h \
    Utils/for_analysis.h \
    New_db_model/_Off_tels.h \
    Forms/an_result_Form.h \
    Forms/analysis_Form.h \
    Forms/mainwindow_Form.h \
    Forms/search_Form.h \
    Forms/officialtelephones_Form.h \
    Forms/master_export_Form.h \
    Utils/for_export.h \
    New_db_MTM/_MTM_Off_tels.h \
    Forms/Import_Form.h \
    Utils/for_import.h \
    Utils/for_search.h \
    Forms/master_import_Form.h \
    Forms/settings_Form.h \
    Singletons/settings_connection.h \
    Utils/olddbservice.h \
    Custom/table_show_delegate.h \
    Forms/component.h \
    Forms/dialog_settings.h \
    Utils/text_handler.h \
    Forms/date_form.h \
    Custom/customtableview.h \
    Singletons/popup.h \
    Singletons/linksmanager.h \
    Forms/linkedperson.h

FORMS += \
    EditPerson.ui \
    PersonCard.ui \
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
    component.ui \
    dialog_settings.ui \
    date_form.ui \
    popup.ui \
    Forms/linkedperson.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

INCLUDEPATH += $$PWD\
               Forms\
               New_db_model\
               New_db_MTM\
               Singletons\
               New_db_model/Analysis_models\
               Encrypt\
               Custom\
               Utils\
