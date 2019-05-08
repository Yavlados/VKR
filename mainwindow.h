#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "add_form.h"
#include <QPushButton>
#include <QMessageBox>
#include "analysis.h"
#include "update.h"
#include "search.h"
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

    int p_b_counter = 0;
    int temp_id = 0;
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

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

signals:
    void Send_data(int id);

    void Refresh_tab();

    void start_radio_button();

    void Fill_table_add();

    void Set_validators_an();

private:
    Ui::MainWindow *ui;
};



#endif // MAINWINDOW_H
