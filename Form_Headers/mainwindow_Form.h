#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "analysis_Form.h"
#include "update_Form.h"
#include "search_Form.h"

#include"master_export_Form.h"
#include "master_import_Form.h"
#include "settings_Form.h"

#include "db_connection.h"
#include "list_master.h" //При редактировании подгружаю круд целиком

#include "_MTM_Contacts.h"
#include "_MTM_Crud.h"
#include "_MTM_OwTel.h"

#include "officialtelephones_Form.h"
#include "for_export.h"/// ->_Crud.h->_Owners_tel.h->_Contacts.h->db_connection
#include "Import_Form.h"


#include <QMainWindow>
#include <QPushButton>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QFileDialog>

namespace Ui {
class MainWindow;
}
enum Model_check_state { All_checked = 1,
                         All_unchecked = 0};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QMessageBox msgbx;

    Model_check_state m_c_s;

    QList<Update*> *updlist = nullptr;
    QList<Update*> *addlist = nullptr;

    Search *sr = nullptr;

    Master_export_Form *exprt = nullptr;
    Master_import_form *imprt = nullptr;
    OfficialTelephones *of = nullptr;

     class Analysis *an = nullptr;

     MTM_Contacts *contacts_model = new MTM_Contacts;
     MTM_OwTel *ot_model = new MTM_OwTel;
     MTM_Crud *crud_model = nullptr;

     QList<Contacts*> *contactList = new QList<Contacts*>;
     QList<Owners_tel*> *otList = new QList<Owners_tel*>;


    ///Указатель на класс для экспорта
    For_export *form_exprt = nullptr;

    ///Указатель на индекс первой таблицы
    QModelIndex index_tab1;

    ///Счетчик для кнопки
    int p_b_counter = 0;

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:


private:
    Ui::MainWindow *ui;


private slots:
    void Add_pagination_buttons();

    void on_tableView_clicked(const QModelIndex &index);

    void on_tableView_2_clicked(const QModelIndex &index);

    void ShowThisTab(int index =0);

    void RefreshTab();

    void on_action_add_triggered();

    void on_action_delete_triggered();

    void on_action_update_triggered();

    void on_action_analysis_triggered();

    void on_action_search_triggered();

    void Search_result(QList<Crud*> *crudlist);

    void on_pushButton_clicked();

    void add_cancel_button();

    void open_upd_tab(Crud *cr);

    void set_validators();

    void on_tabWidget_tabCloseRequested(int index);

    void on_action_official_tel_triggered();

    void on_actionexport_triggered();

    void testing_export(QString, QString, bool, bool, bool);

    void testing_opening(QString, QString);

    void on_pb_check_model_clicked();

    void on_action_import_triggered();

    void on_action_Settings_triggered();

    void next_page();

    void previous_page();

    void set_fonts();

    void add_splitter_lines();
    void on_tabWidget_tabBarClicked(int index);

    void on_tabWidget_2_tabCloseRequested(int index);

signals:
    void Send_data(Crud *cr, int index);

    void Set_validators_an();

    void Fill_table_of();

    void Close_tab();
};



#endif // MAINWINDOW_H
