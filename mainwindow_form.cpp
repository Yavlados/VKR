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

    add = new Update();
    upd = new Update();

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
    index_tab1 = ui->tableView->currentIndex();
    if(index_tab1.isValid())
    {
        ui->action_delete->setEnabled(true);
        ui->action_update->setEnabled(true); //включаю кнопку редактировать

        if(Owners_tel::selectZkTelForAdd(crud_model->actcrudlist.at(index_tab1.row())->owt(), crud_model->actcrudlist.at(index_tab1.row())->zk_id))
        {
           ot_model->setOTList(crud_model->actcrudlist.at(index_tab1.row())->owt());
           if(ot_model->actotlist.at(0)->tel_num.isEmpty())
           ot_model->actotlist.removeAt(0);//Костыль, тк не могу не отображать пустые номера
        }
        ui->tableView_2->setModel(ot_model);
        ui->tableView_2->setColumnWidth(0,250);
        contacts_model->reset_ContactModel();
    }
    else
    {
        ui->action_delete->setEnabled(false);
        ui->action_update->setEnabled(false); //включаю кнопку редактировать
    }
}

void MainWindow::on_tableView_2_clicked(const QModelIndex &index) //Обрабатываем клик по таблице с Номерами владельцев
{
    if (index_tab1.isValid())
    {
        if(Contacts::selectTelContacts(crud_model->actcrudlist.at(index_tab1.row())->owt()->at(index.row())->cont(),
                                       crud_model->actcrudlist.at(index_tab1.row())->owt()->at(index.row())->tel_id))

        contacts_model->setContactList(crud_model->actcrudlist.at(index_tab1.row())->owt()->at(index.row())->cont());

        ui->tableView_3->setModel(contacts_model);
        ui->tableView_3->setColumnWidth(0,250);
        ui->tableView_3->setColumnWidth(1,250);
    }
}

void MainWindow::ShowThisTab() //Открытие main окна и рефреш таблиц
{
    if(Crud::selectAll(crudlist)) ///Вызов статичной функции
        crud_model->setCrudlist(crudlist);
    ui->tableView->setModel(crud_model);
    m_c_s = All_unchecked;

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
    set_validators();
}

void MainWindow::on_action_add_triggered()
{
    qDebug() << ui->tabWidget->count();
    ui->tabWidget->insertTab( ui->tabWidget->count()+1 ,add,"Добавление новой ЗК");
    emit Fill_table_add();
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
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
        if (index_tab1.isValid())
        {
            Crud::del_zk(crud_model->actcrudlist.at(index_tab1.row())->zk_id);
            emit Refresh_tab();
        }
        break;
    }
}

void MainWindow::on_action_update_triggered()
{
    if(index_tab1.isValid())
    {
        index_tab1 = ui->tableView->currentIndex();
        open_upd_tab(crud_model->actcrudlist.at(index_tab1.row()));
    }
}

void MainWindow::on_action_analysis_triggered()
{
       an = new class Analysis;
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
    if(Crud::select_search(crudlist, Query))
       crud_model->setCrudlist(crudlist);
    ui->tableView->setModel(crud_model);
    MainWindow::add_cancel_button();
}

void MainWindow::on_pushButton_clicked()
{
    if (ui->lineEdit->text() == QString(""))
    {
        emit Refresh_tab();
    }
    else {
   ///Проверяю условие: если в результате поиска, полученная книга = переданному айди
        Crud *cr = Crud::id_zk_search(ui->lineEdit->text().toInt());
        if (cr == nullptr)
            QMessageBox::critical(this,QObject::tr("Внимание"),QObject::tr("По вашему запросу ничего не найдено!")); ///Ругаемся
        else
        {
            crud_model->setOneCrud(cr);
            ui->tableView->setModel(crud_model);
            MainWindow::add_cancel_button();
        }
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

void MainWindow::open_upd_tab(Crud *cr)
{
    emit Send_data(cr);
     ui->tabWidget->insertTab( ui->tabWidget->count()+1 ,upd,"Редактировать");
     ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
}

void MainWindow::set_connections()
{
    connect (add, SIGNAL (toMainForm()), this, SLOT(ShowThisTab()));
    connect (add, SIGNAL (open_update_tab(Crud *)), this, SLOT(open_upd_tab(Crud *)));

    connect(upd, SIGNAL(Ready_for_update()), this, SLOT(ShowThisTab()));
    connect(add, SIGNAL(Ready_for_update()), this, SLOT(ShowThisTab()));

    connect(this,SIGNAL(Send_data(Crud*)), upd, SLOT(Recieve_data(Crud*)));
    connect (this, SIGNAL(Refresh_tab()), this, SLOT(ShowThisTab()));
    connect(this, SIGNAL(Fill_table_add()),add, SLOT(Fill_table_in_add()));

    connect(sr, SIGNAL(Send_search_query(QString)),this, SLOT(Search_result(QString)));
    connect(sr,SIGNAL(Cancel_search()),this, SLOT(ShowThisTab()));

    connect(this,SIGNAL(Fill_table_of()), of, SLOT(Fill_table()));

    connect(exprt,SIGNAL(rb_zk_clicked()),this, SLOT(on_action_search_triggered()));
    connect(exprt,SIGNAL(rb_check_all()),this, SLOT(on_pb_check_model_clicked()));
    connect(exprt, SIGNAL(TESTING_export(QString,QString, bool, bool, bool)),this,SLOT(testing_export(QString, QString, bool, bool, bool)));
    connect(exprt, SIGNAL(TESTING_open(QString,QString)), this, SLOT(testing_opening(QString, QString)));
}

void MainWindow::set_validators()
{
    qDebug() << crudlist->first()->zk_id << crudlist->last()->zk_id;
    ui->lineEdit->setValidator(new QIntValidator(crudlist->first()->zk_id,crudlist->last()->zk_id));
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

void MainWindow::on_actionexport_triggered()
{
    exprt->show();
}

void MainWindow::testing_export(QString filename, QString password, bool cb_off_tels, bool cb_set_password, bool cb_zk)
{
    if (filename.isEmpty())
    {
        QMessageBox::critical(exprt,QObject::tr("Ошибка"),QObject::tr("Вы не указали путь!")); ///Хвалимся
        return;
    }

    QList<Crud*> *crud = new  QList<Crud*> ;
    QList<Off_tels*> *offtel = new QList<Off_tels*>;

    form_exprt->list->set_counters();

 if(cb_zk)
    for (int i=0;i<crud_model->actcrudlist.size();i++) // пробегаюсь по отображаемому списку
    {
     if (crud_model->actcrudlist.at(i)->checkState_ == Checked)
        {
            qDebug() << crud_model->actcrudlist.at(i)->zk_id;
             form_exprt->list->fill_crud_list(crud, crud_model->actcrudlist.at(i)->zk_id, PSQLtype);
        }
    }

if(cb_off_tels)
     form_exprt->list->fill_off_tels(offtel,PSQLtype);

if(!crud->isEmpty() || !offtel->isEmpty())
{
    if( form_exprt->Do_export(filename,crud, password, cb_off_tels, cb_set_password, offtel))
        QMessageBox::information(exprt,QObject::tr("Успех"),QObject::tr("Данные сохранены в файл, расположенный по пути : %1").arg(filename)); ///Хвалимся
    else
        QMessageBox::critical(exprt,QObject::tr("Ошибка"),QObject::tr("Во время экспорта данных что-то пошло не так!")); ///Хвалимся
}
else {
    QMessageBox::warning(exprt,QObject::tr("Внимание"),QObject::tr("Экспорт не был выполнен, так как вы не выбрали данные!")); ///Хвалимся
}
    delete offtel;
   delete crud;
}

void MainWindow::testing_opening(QString filename, QString password)
{
//    Import_state temp_i_s = form_import->Testing_open_db( filename,password);
//    switch (temp_i_s)
//    {
//    case Import_in_progress:
//        form_import->import->show();
//        break;
//    case Import_succesful:
//        qDebug()<< "Open is succesfull";
//        break;
//    case Password_incorrect:
//        QMessageBox::critical(exprt,QObject::tr("Ошибка!"),QObject::tr("Вы ввели не верный пароль")); ///Хвалимся
//        break;
//    case Import_abort:
//        QMessageBox::critical(exprt,QObject::tr("Ошибка!"),QObject::tr("Что-то пошло не так...<br> Бэкап базы либо пуст, либо для доступа к нему не требуется пароль")); ///Хвалимся
//        break;
//    }
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
