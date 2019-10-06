#ifndef IMPORT_FORM_H
#define IMPORT_FORM_H
#include "_Crud.h"
#include "_Off_tels.h"
#include "_Zk_links.h"

#include "list_master.h"
#include "update_Form.h"

#include "_MTM_Crud.h"
#include "_MTM_OwTel.h"
#include "_MTM_Contacts.h"
#include "_MTM_Off_tels.h"

#include <QWidget>
#include <QCloseEvent>
#include <QVector>

/**
 * @file Import_Form.h
 * @brief Форма импорта
*/

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
    explicit Import_Form(QWidget *parent = nullptr);
    ~Import_Form();

    List_master *list; ///Работа со списками

    ///Список id зк, предстоящих к удалению
    QList<int> del_list;

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
    SqlType type;

    bool Testing_open_db(QString filename, QString password);

    bool begin_import();

private slots:

    bool compare_dump_db();

    void on_pb_save_import_slot(QString str);

    //void setOffTel();

    void closeEvent(QCloseEvent *event);

    void recieve_updated_import_crud(Crud *new_cr);

    void recieve_added_import_crud(Crud *new_cr);

    ///Добавление в бд crud
    bool add_to_db();
    void prepare_main_to_add(Crud *main_crud, Crud  *added_crud);
    ////// Слоты формы ////////

    void on_tableView_crud_pg_clicked(const QModelIndex &index);

    void clear_models();

    void clear_label();

    void on_pb_save_import_clicked();

    void on_pb_save_main_clicked();

    void on_pb_skip_import_clicked();

    void on_pb_skip_All_clicked();

    void on_pb_update_import_zk_clicked();

    void on_pb_update_pg_clicked();

    void on_pushButton_clicked();

    void make_link_clicked();

    void begin_work_with_links();

    void abort_link_import();

    void import_list_with_link();



signals:
    void Refresh_tab();
    void Send_data(Crud *cr);
    void Send_main_local(Crud *cr);
    void Send_main_pg(Crud *cr);

private:
    QVector<QVector<QString>> *vector = nullptr;    //связанные ююайди
    QVector<QVector<int> > *links_vector =nullptr;   //для связывания цепочек
    QList<int> *linked_id_list = nullptr;
    QCloseEvent *event ;
    int a;  ///Итераторы для сравнений
    QFile db_file;
    QList<Crud*>        *crud;      ///Локальные списки
    QList<Off_tels*>    *offtel;    ///С данными из SQLite
    QList<zk_links*>    *links;
    Crud *crud_from_pg;             ///Локальный круд из БД
    QString Local_filename ;
    Ui::Import_Form *ui;
};

#endif // IMPORT_FORM_H
