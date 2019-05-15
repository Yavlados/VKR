#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "add_form.h"
#include "analysis.h"
#include "update.h"
#include "search.h"
#include "db_connection.h"
#include "MyTableModel.h"


#include <QMainWindow>
#include <QPushButton>
#include <QMessageBox>
#include <QSqlQueryModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QMessageBox msgbx;

     Add_form *add = new Add_form(); //указатель на форму добавления

     Update *upd = new Update();

     Search *sr = new Search;

    bool search;

    MyTableModel *contacts_model = new MyTableModel;
    MyTableModel *ot_model = new MyTableModel;

    QList<Contacts*> *contactList = new QList<Contacts*>;
    QList<Owners_tel*> *otList = new QList<Owners_tel*>;

    int p_b_counter = 0;
    int temp_id = 0;
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void reopen();

private:
    Ui::MainWindow *ui;


private slots:
    void on_tableView_clicked(const QModelIndex &index);

    void on_tableView_2_clicked(const QModelIndex &index);

    void on_tableView_3_clicked(const QModelIndex &index);

    void ShowThisTab();

    void on_action_add_triggered();

    void on_action_delete_triggered();

    void on_action_update_triggered();

    void on_action_analysis_triggered();

    void on_action_search_triggered();

    void Search_result(QSqlQueryModel*);

    void on_pushButton_clicked();

    void add_cancel_button();

    void open_upd_tab(int);

    void set_connections();
signals:
    void Send_data(int);

    void Refresh_tab();

    void start_radio_button();

    void Fill_table_add();

    void Set_validators_an();

};



#endif // MAINWINDOW_H
