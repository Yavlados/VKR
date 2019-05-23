#include "mainwindow_Form.h"
#include "ui_mainwindow.h"
#include "db_connection.h"

#include <QRect>
#include <QSqlDatabase>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    set_connections();

     auto tabbar = ui->tabWidget->tabBar();
    tabbar->tabButton(0,QTabBar::RightSide)->deleteLater();
    tabbar->setTabButton(0, QTabBar::RightSide, nullptr);

    showMaximized();
    ShowThisTab();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_tableView_clicked(const QModelIndex &index) //Обрабатываем клик по таблице с Владельцами номеров
{
    (void)index;
    ui->action_delete->setEnabled(true);
    ui->action_update->setEnabled(true); //включаю кнопку редактировать

    QModelIndexList *indexes = new QModelIndexList;
    *indexes = ui->tableView->selectionModel()->selection().indexes();
    Crud *cr = new Crud(indexes->value(1).data().toInt());

    if(Owners_tel::selectZkTel(otList,cr->zk_id))
        ot_model->setOTList(otList);

    ui->tableView_2->setModel(ot_model);
    ui->tableView_2->setColumnWidth(0,250);
    contacts_model->reset_ContactModel();
    delete indexes;
    delete cr;
}

void MainWindow::on_tableView_2_clicked(const QModelIndex &index) //Обрабатываем клик по таблице с Номерами владельцев
{
    qDebug() << otList->at(index.row())->tel_id;

    if(Contacts::selectTelContacts(contactList,  otList->at(index.row())->tel_id))
        contacts_model->setContactList(contactList);

    ui->tableView_3->setModel(contacts_model);
    ui->tableView_3->setColumnWidth(0,250);
    ui->tableView_3->setColumnWidth(1,250);
}


void MainWindow::ShowThisTab() //Открытие main окна и рефреш таблиц
{
    Crud *cr =  new Crud();
    if(cr->selectAll(crudlist))
        crud_model->setCrudlist(crudlist);
    ui->tableView->setModel(crud_model);
    m_c_s = All_unchecked;
    delete cr;

    set_validators();
    ui->tabWidget->removeTab(1);
    ui->tabWidget->removeTab(2);
    ui->tabWidget->setCurrentIndex(0);

    ot_model->reset_OTModel();
    contacts_model->reset_ContactModel();

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
        Crud *cr = new Crud;
        QModelIndexList *indexes = new QModelIndexList;
        *indexes = ui->tableView->selectionModel()->selection().indexes();
        cr->del_zk(indexes->value(1).data().toInt());
        delete indexes;
        delete cr;
        emit Refresh_tab();
        break;
    }
}

void MainWindow::on_action_update_triggered()
{
     QModelIndexList *indexes = new QModelIndexList;
     *indexes = ui->tableView->selectionModel()->selection().indexes();
     open_upd_tab(indexes->value(1).data().toInt());
      delete indexes;
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

void MainWindow::Search_result(QString Query)
{
    ui->tabWidget->setCurrentIndex(0);
    Crud *cr =  new Crud();
    if(cr->select_search(crudlist, Query))
       crud_model->setCrudlist(crudlist);
    ui->tableView->setModel(crud_model);
    MainWindow::add_cancel_button();
    delete cr;

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

void MainWindow::open_upd_tab(int temp_id)
{
    emit Send_data(temp_id);
     ui->tabWidget->insertTab( ui->tabWidget->count()+1 ,upd,"Редактировать");
     ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
}

void MainWindow::set_connections()
{

    connect (add, SIGNAL (toMainForm()), this, SLOT(ShowThisTab()));
    connect (add, SIGNAL (open_update_tab(int)), this, SLOT(open_upd_tab(int)));

    connect(upd, SIGNAL(Ready_for_update()), this, SLOT(ShowThisTab()));

    connect(this,SIGNAL(Send_data(int)), upd, SLOT(Recieve_data(int)));
    connect (this, SIGNAL(Refresh_tab()), this, SLOT(ShowThisTab()));
    connect(this, SIGNAL(Fill_table_add()),add, SLOT(Fill_table_in_add()));

    connect(sr, SIGNAL(Send_search_query(QString)),this, SLOT(Search_result(QString)));
    connect(sr,SIGNAL(Cancel_search()),this, SLOT(ShowThisTab()));

    connect(this,SIGNAL(Fill_table_of()), of, SLOT(Fill_table()));

    connect(exprt,SIGNAL(rb_zk_clicked()),this, SLOT(on_action_search_triggered()));
    connect(exprt,SIGNAL(rb_check_all()),this, SLOT(on_pb_check_model_clicked()));
    connect(exprt, SIGNAL(TESTING_export(QString,QString)),this,SLOT(testing_export(QString, QString)));
    connect(exprt, SIGNAL(TESTING_open(QString,QString)),this,SLOT(testing_opening(QString, QString)));
}

void MainWindow::set_validators()
{
    int min, max;
    Crud *cr = new Crud();
    cr->get_min_zk();   min = cr->zk_id;
    cr->get_max_zk();   max = cr->zk_id;
    ui->lineEdit->setValidator(new QIntValidator(min,max));
    delete cr;

}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    ui->tabWidget->removeTab(index);

}

void MainWindow::on_action_official_tel_triggered()
{
    ui->tabWidget->insertTab( ui->tabWidget->count()+1 ,of,"Cлужебные телефоны");
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
    emit Fill_table_of();
}

void MainWindow::on_pushButton_2_clicked()
{
    QString filename = QFileDialog::getSaveFileName(
                       this,
                       tr("Save Document"),
                       QDir::currentPath(),
                       tr("Text File (*.csv)") );
    if( !filename.isNull() )
    {
        QFile file(filename);
        if(file.open(QIODevice::WriteOnly))
        {
            QTextStream in(&file);
            for (int i=0; i < crudlist->size();i++)
            {
                Crud *cr = crudlist->at(i);
                if(cr->checkState_ == Checked)
                {
                    in <<  QString::number(cr->zk_id) +","+
                           cr->lastname+","+cr->name+","+
                           cr->mid_name+","+cr->birth_date+"\r\n";

                    cr->check();
                }
            }

            file.close();
           QMessageBox::information(this, "Information", "Текстовый файл сформирован и находится по следующему пути: \n"+filename); //выводим сообщение с числом
        }
    }

    QSqlDatabase dbsql;
    dbsql = QSqlDatabase::addDatabase("QSQLITE","conql");
    dbsql.setDatabaseName("E:\\database.db3");
    if (dbsql.open())
        qDebug() << "ok";
    QSqlQuery quer;
    quer.prepare("Select * from \"test\"");
    if(!quer.exec())
        qDebug() << quer.lastError().text() ;
    while (quer.next())
        qDebug() << quer.value(0).toString()+" " + quer.value(1).toString();
}

void MainWindow::on_pushButton_3_clicked()
{
//    Crud *cr = new Crud;
//    if(cr->selectAllDb(crudlist,otList,contactList))
//         qDebug() << "Aee";

          QStringList list = QSqlDatabase::drivers();
          for(int x = 0; x < list.length(); x++)
                  qDebug() << list.at(x);

//    db_connection *db = db_connection::instance();
//    db->set_Sql_type(SQLliteType);
//    qDebug() << db->db_connect()<<db->db().lastError();
//    QSqlQuery query(db->db());
//    //query.prepare();
//    if(!query.exec("SELECT * FROM ttt"))
//        qDebug() << query.lastError();
//    while (query.next())
//        qDebug() << query.value(0).toString()+" "+query.value(1).toString();

//    db->set_Sql_type(PSQLtype);
//    QSqlQuery query2(db->db());
//    query2.prepare("SELECT * FROM zk");
//    if(!query2.exec())
//        qDebug() << query2.lastError();
//    while (query2.next())
//        qDebug() << query2.value(0).toString()+" "+query2.value(1).toString();
}

void MainWindow::on_actionexport_triggered()
{
//    Crud *cr = new Crud;
//    if(cr->selectAllDb(crudlist,otList,contactList))/// Временный вариант с листами, нужно будет переделать
//         qDebug() << "Aee"; ///Заполнил все три листа указателями на данные

    exprt->show();
}


void MainWindow::testing_export(QString filename, QString password)
{
    QList<Crud*> *crud = new  QList<Crud*> ;
    For_export exprt;
    for (int i=0;i<crud_model->actcrudlist.size();i++) // пробегаюсь по отображаемому списку
    {
     if (crudlist->at(i)->checkState_ == Checked)
        {   qDebug() << crudlist->at(i)->zk_id;
             exprt.fill_crud_list(crud, crudlist->at(i)->zk_id);
        }
    }
   if( exprt.Do_export(filename,crud, password))
       qDebug() << "asd";

    //QFile::remove("E:/test.db");
//         db->set_Sql_type(SQLliteType);
//         qDebug() << db->db_connect()<<db->db().lastError();
//         QSqlQuery query(db->db());
//         //query.prepare();
//         if(!query.exec("SELECT * FROM ttt"))
//             qDebug() << query.lastError();
//         while (query.next())
//             qDebug() << query.value(0).toString()+" "+query.value(1).toString();
   delete crud;
}

void MainWindow::testing_opening(QString filename, QString password)
{
    For_export exprt;
    if(exprt.Testing_open_db(filename,password))
        qDebug()<< "Open is succesfull";
}


void MainWindow::on_pb_check_model_clicked()
{
    switch (m_c_s)
    {
    case All_unchecked:
        crud_model->setCheckedCrudlist(crudlist);
        ui->tableView->setModel(crud_model);
        m_c_s = All_checked;
    return;
    case All_checked:
        crud_model->setUnCheckedCrudlist(crudlist);
        ui->tableView->setModel(crud_model);
        m_c_s = All_unchecked;
    return;
    }
}
