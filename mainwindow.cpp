#include "crud.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "db_connection.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    connect(this, SIGNAL(start_radio_button()), add, SLOT(radio_button1_checked()));
    //connect(this, SIGNAL(start_radio_button()), upd, SLOT(radio_button1_checked()));

    connect (add, SIGNAL (toMainForm()), this, SLOT(ShowThisTab()));
    connect(this,SIGNAL(Send_data(int)),upd, SLOT(Recieve_data(int)));
    connect(upd, SIGNAL(Ready_for_update()), this, SLOT(ShowThisTab()));
    connect (this, SIGNAL(Refresh_tab()), this, SLOT(ShowThisTab()));
    connect(this, SIGNAL(Fill_table_add()),add, SLOT(Fill_table_in_add()));

    connect(sr, SIGNAL(Send_Model(QSqlQueryModel*)),this, SLOT(Search_result(QSqlQueryModel*)));
    connect(sr,SIGNAL(Cancel_search()),this, SLOT(ShowThisTab()));
    ui->setupUi(this);

    MainWindow::showMaximized();

    Connection *con = new Connection;   //подключение к бд
    con->db_connect();                  //через указатель
    delete con;                         //и удаление его
    Crud *cr = new Crud();
    cr->select_all();
    ui->tableView->setModel(cr->model);
    ui->tableView->resizeColumnsToContents();
    delete cr;
    emit start_radio_button();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_tableView_clicked(const QModelIndex &index) //Обрабатываем клик по таблице с Владельцами номеров
{
    (void)index;
    ui->action_delete->setEnabled(true); //включаю кнопку редактировать
    ui->action_update->setEnabled(true);
    QLayout *layout = ui->vl_cancel_button->layout();
    while (layout->count() != 0)
        {
            QLayoutItem *item = layout->takeAt(0);
            delete item->widget();
            if (p_b_counter > 0)
            {
                p_b_counter--;
            }
        }

    QModelIndexList *indexes = new QModelIndexList;
    *indexes = ui->tableView->selectionModel()->selection().indexes();
    Crud *cr = new Crud(indexes->value(0).data().toInt(),
                        indexes->value(1).data().toString(),
                        indexes->value(2).data().toString(),
                        indexes->value(3).data().toString(),
                        indexes->value(4).data().toString(),
                        indexes->value(5).data().toString(),
                        indexes->value(6).data().toString(),
                        indexes->value(7).data().toString(),
                        indexes->value(8).data().toString());
    temp_id = indexes->value(0).data().toInt();
    cr->select_telephone();
    ui->tableView_2->setModel(cr->model);
    ui->tableView_2->setColumnWidth(0,200);
    delete indexes;
    delete cr;

    //Очистка третьей таблицы
    Crud *clear = new Crud();
    clear->refresh_table();
    ui->tableView_3->setModel(clear->model);
    delete clear;
}


void MainWindow::on_tableView_2_clicked(const QModelIndex &index) //Обрабатываем клик по таблице с Номерами владельцев
{
    Crud *cr = new Crud(index.data().toString());
    cr->recieve_tel_list();
    ui->tableView_3->setModel(cr->model);
    ui->tableView_3->resizeColumnsToContents();


//    if (p_b_counter < 1)
//    {
//        QPushButton *p_b = new QPushButton;
//        p_b->setText("Редактировать номер телефона");
//        ui->verticalLayout_4->addWidget(p_b);
//            p_b_counter++;
//        qDebug()<<p_b_counter;

//        connect(p_b, SIGNAL(clicked()), an ,SLOT(show()));
//    }
    delete cr;
}

void MainWindow::on_tableView_3_clicked(const QModelIndex &index)
{
    (void)index;
}

void MainWindow::ShowThisTab() //Открытие main окна и рефреш таблиц
{
    ui->tabWidget->removeTab(1);
    ui->tabWidget->removeTab(2);
    ui->tabWidget->setCurrentIndex(0);

    Crud *cr = new Crud();
    cr->select_all();
    ui->tableView->setModel(cr->model);
    cr->refresh_table();
    ui->tableView_2->setModel(cr->model_2);
    ui->tableView_3->setModel(cr->model_2);

    QLayout *layout = ui->vl_cancel_button->layout();
    while (layout->count() != 0)
        {
            QLayoutItem *item = layout->takeAt(0);
            delete item->widget();
            if (p_b_counter > 0)
            {
                p_b_counter--;
            }
        }
    ui->lineEdit->clear();
    delete cr;
}


void MainWindow::on_action_add_triggered()
{
    qDebug() << ui->tabWidget->count();
    ui->tabWidget->insertTab( ui->tabWidget->count()+1 ,add,"Добавление новой ЗК");
    emit Fill_table_add();
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
    qDebug() << ui->tabWidget->count();
}

void MainWindow::on_action_delete_triggered()
{
    msgbx.setText("Удаление");
    msgbx.setInformativeText("Вы действительно хотите удалить выбранную записную книгу?");
    msgbx.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    int ret = msgbx.exec();

    switch (ret) {
    case QMessageBox::Cancel:
        emit Refresh_tab();
        break;
    case QMessageBox::Ok:
        Crud *cr = new Crud(temp_id);
        cr->del_zk();
        delete cr;
        emit Refresh_tab();
        break;
    }
}

void MainWindow::on_action_update_triggered()
{
    emit Send_data(temp_id);
     ui->tabWidget->insertTab( ui->tabWidget->count()+1 ,upd,"Редактировать");
     ui->tabWidget->autoFillBackground();
     ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
}

void MainWindow::on_action_analysis_triggered()
{
       Analysis *an = new Analysis;
       an->show();
       connect(this, SIGNAL(Set_validators_an()), an, SLOT(set_validators()));
       emit Set_validators_an();

}

void MainWindow::on_action_search_triggered()
{
      sr->show();

}

void MainWindow::Search_result(QSqlQueryModel *model)
{
    if(model->rowCount() > 0)
    {
        ui->tableView->setModel(model);
        MainWindow::add_cancel_button();
    }
}


void MainWindow::on_pushButton_clicked()
{
    if (ui->lineEdit->text() == QString(""))
    {
        emit Refresh_tab();

    }
    else {
     Crud *cr = new Crud(ui->lineEdit->text().toInt());
     cr->id_zk_search();
     ui->tableView->setModel(cr->model);

        MainWindow::add_cancel_button();

     delete cr;

    }
}

void MainWindow::add_cancel_button()
{
    if (p_b_counter < 1)
       {
    QPushButton *p_b = new QPushButton;
    p_b->setText("Сброс параметров поиска");
    ui->vl_cancel_button->addWidget(p_b);
    p_b_counter++;
    qDebug()<<p_b_counter;

    connect(p_b, SIGNAL(clicked()), this ,SLOT(ShowThisTab()));
    }
}

