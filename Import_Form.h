#ifndef IMPORT_FORM_H
#define IMPORT_FORM_H
#include "_Crud.h"
#include "_Off_tels.h"
#include "list_master.h"

#include "_MTM_Crud.h"
#include "_MTM_OwTel.h"
#include "_MTM_Contacts.h"
#include "_MTM_Off_tels.h"

#include <QWidget>

enum Import_state { Password_incorrect = 0,
                    Import_succesful = 1,
                    Import_abort = 2,
                    Import_in_progress = 3};

namespace Ui {
class Import_Form;
}

class Import_Form : public QWidget
{
    Q_OBJECT

public:

    List_master *list; ///Работа со списками


    ///Список id зк, предстоящих к удалению
    QList<int> del;

    //////////////////////////////////////
    MTM_Crud *local_crud_model;
    /////////////////////////////////////

    MTM_Crud *crud_model;//Временная
    MTM_Crud *crud_model_pg;

    MTM_OwTel *tel_mod;
    MTM_OwTel *tel_mod_pg;
    ///Временные модели для отображения данных
    MTM_OwTel *tel_model;
    MTM_Contacts *cont_model;
    MTM_Off_Tels *off_model;

    void recieve_all(QList <Crud*> *crudlist, QList<Off_tels*>  *offtel, QList <Crud*> *crudlist_pg, QList<Off_tels*>  *offtel_pg);
    bool compare_dump_db();
    explicit Import_Form(QWidget *parent = nullptr);
    ~Import_Form();

    QList<Crud*>        *crud; ///Локальный список
    QList<Crud*>        *crud_from_pg;
    QList<Off_tels*>    *offtel;
    QList<Off_tels*>    *offtel_from_pg;

private slots:

    QList <Crud*> setCrud();

    Import_state Testing_open_db(QString filename, QString password);

    bool begin_import();

    void on_pb_save_import_slot(QString str);

    //void setOffTel();

    void closeEvent(QCloseEvent *event);

    void on_tableView_zk_clicked(const QModelIndex &index);

    void on_tableView_tels_clicked(const QModelIndex &index);

    void on_tableView_crud_pg_clicked(const QModelIndex &index);

    void clear_models();

    void clear_label();

    void on_pb_save_import_clicked();

    void on_pb_save_main_clicked();

    void on_pb_skip_import_clicked();

    void on_pb_skip_All_clicked();

signals:
    void on_pb_save_import(QString str);

private:
    QFile db_file;
    Ui::Import_Form *ui;
    QList <Crud*> *crud_local;
    QList <Crud*> *crud_pg_local;
    QList <Off_tels*> *offtel_local;
    QList <Off_tels*> *offtel_pg_local;

};

#endif // IMPORT_FORM_H
