#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "add_form.h"
#include "analysis_Form.h"
#include "update_Form.h"
#include "search_Form.h"
#include"master_export_Form.h"
#include "db_connection.h"

#include "_MTM_Contacts.h"
#include "_MTM_Crud.h"
#include "_MTM_OwTel.h"

#include "officialtelephones_Form.h"
#include "for_export.h"/// ->_Crud.h->_Owners_tel.h->_Contacts.h->db_connection

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

     Add_form *add = new Add_form(); //указатель на форму добавления

     Update *upd = new Update();

     Search *sr = new Search;

     master_export_Form *exprt = new master_export_Form;

    OfficialTelephones *of = new OfficialTelephones();

    bool search;

    MTM_Contacts *contacts_model = new MTM_Contacts;
    MTM_OwTel *ot_model = new MTM_OwTel;
    MTM_Crud *crud_model = new MTM_Crud;

    QList<Contacts*> *contactList = new QList<Contacts*>;
    QList<Owners_tel*> *otList = new QList<Owners_tel*>;
    QList<Crud*> *crudlist = new QList<Crud*>;


    int p_b_counter = 0;

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:


private:
    Ui::MainWindow *ui;


private slots:
    void on_tableView_clicked(const QModelIndex &index);

    void on_tableView_2_clicked(const QModelIndex &index);

    void ShowThisTab();

    void on_action_add_triggered();

    void on_action_delete_triggered();

    void on_action_update_triggered();

    void on_action_analysis_triggered();

    void on_action_search_triggered();

    void Search_result(QString);

    void on_pushButton_clicked();

    void add_cancel_button();

    void open_upd_tab(int);

    void set_connections();

    void set_validators();

    void on_tabWidget_tabCloseRequested(int index);

    void on_action_official_tel_triggered();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_actionexport_triggered();

    void testing_export(QString, QString);

    void testing_opening(QString, QString);

    void on_pb_check_model_clicked();

signals:
    void Send_data(int);

    void Refresh_tab();

    void Fill_table_add();

    void Set_validators_an();

    void Fill_table_of();
};



#endif // MAINWINDOW_H
