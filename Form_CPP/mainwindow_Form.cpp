#include "mainwindow_Form.h"
#include "ui_mainwindow.h"
#include "db_connection.h"
#include "table_cb_delegate.h"

#include <QRect>
#include <QSqlDatabase>
#include <QSettings>
#include "settings_connection.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto tabbar = ui->tabWidget->tabBar();
    tabbar->tabButton(0,QTabBar::RightSide)->deleteLater();
    tabbar->setTabButton(0, QTabBar::RightSide, nullptr);

    auto tabbar2 = ui->tabWidget_2->tabBar();
    tabbar2->tabButton(0,QTabBar::RightSide)->deleteLater();
    tabbar2->setTabButton(0, QTabBar::RightSide, nullptr);

    showMaximized();
    RefreshTab();
    ui->tableView->selectRow(0);
    on_tableView_clicked(index_tab1);
    //set_fonts();
    add_splitter_lines();
    Settings_connection::instance();

    Table_cb_delegate *delegate = new Table_cb_delegate();
    ui->tableView_3->setItemDelegateForColumn(1, delegate);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Add_pagination_buttons()
{
    while(ui->hl_for_pagination_button_back->count() != 0)
    {
        QLayoutItem *item = ui->hl_for_pagination_button_back->takeAt(0);
        delete item->widget();
    }
    while(ui->hl_for_pagination_button_next->count() != 0)
    {
        QLayoutItem *item = ui->hl_for_pagination_button_next->takeAt(0);
        delete item->widget();
    }

     if(crud_model->actcrudlist.size() < crud_model->crudlist->size())
    {
        if(crud_model->crudlist->indexOf(crud_model->actcrudlist.at(0)) != 0)
        {
            QPushButton *p_b_back = new QPushButton;
            p_b_back->setText("<<");
            ui->hl_for_pagination_button_back->addWidget(p_b_back);
            connect(p_b_back,SIGNAL(clicked()),this,SLOT(previous_page()));
        }

        if(crud_model->crudlist->indexOf(crud_model->actcrudlist.at(crud_model->actcrudlist.size()-1)) < crud_model->crudlist->size()-1)
        {
            QPushButton *p_b_forward = new QPushButton;
            p_b_forward->setText(">>");
            ui->hl_for_pagination_button_next->addWidget(p_b_forward);
            connect(p_b_forward,SIGNAL(clicked()),this,SLOT(next_page()));
        }
    }
}

void MainWindow::on_tableView_clicked(const QModelIndex &index, QString num) //Обрабатываем клик по таблице с Владельцами номеров
{
    (void)index;
    index_tab1 = ui->tableView->currentIndex();
    if(index_tab1.isValid())
    {
       ui->action_delete->setEnabled(true);
       ui->action_update->setEnabled(true); //включаю кнопку редактировать

       if (!ot_model->mark_rows.isEmpty()) //Чищу список подсвечивания
           ot_model->mark_rows.clear();

           if(Owners_tel::selectZkTelForAdd(crud_model->actcrudlist.at(index_tab1.row())->owt(), crud_model->actcrudlist.at(index_tab1.row())->zk_id))
           {
               ot_model->setOTList(crud_model->actcrudlist.at(index_tab1.row())->owt());
               if(!num.isNull())
               {
                 for (int i = 0; i < ot_model->actotlist.size(); i++)
                 {
                     if(ot_model->actotlist.at(i)->tel_num == num)
                         ot_model->mark_rows.append(i);
                 }
               }
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

void MainWindow::ShowThisTab(int zk_id) //Открытие main окна и рефреш таблиц
{
    RefreshTab();

    if(zk_id == 0)//форма добавления
    {
        if(addlist != nullptr)
        {
            delete addlist->at(0);
            delete addlist;
            addlist = nullptr;
            return;
        }
    }else  //форма редактирования
    {
        if(updlist != nullptr)
        {
            for (int i=0; i<updlist->size(); i++)
            {
                if(updlist->at(i)->new_cr->zk_id == zk_id)
                {
                    delete  updlist->at(i);
                    updlist->removeAt(i);
                    if (updlist->isEmpty())
                        updlist = nullptr;
                    return;
                }
            }
        }
    }
}

void MainWindow::RefreshTab()
{
    crud_model = new  MTM_Crud;
    QList<Crud*> *crudlist = new QList<Crud*>;
    if(Crud::selectAll(crudlist)) ///Вызов статичной функции
        crud_model->setCrudlist(crudlist);

    Add_pagination_buttons();

    ui->tableView->setModel(crud_model);
    m_c_s = All_unchecked;
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
    ui->tabWidget->setCurrentIndex(0);
}

void MainWindow::on_action_add_triggered()
{
    if( addlist == nullptr)
    {
        addlist = new QList<Update*>;
        Update *add = new Update;
        add->frm_t = Add_form;
        addlist->append(add);

        ui->tabWidget->insertTab( ui->tabWidget->count()+1 ,addlist->at(0),"Добавление новой ЗК");
        addlist->at(0)->Fill_table_in_add();
        ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
        connect (addlist->at(0), SIGNAL (open_update_tab(Crud *)), this, SLOT(open_upd_tab(Crud *)));
        connect(addlist->at(0), SIGNAL(Ready_for_update(int)), this, SLOT(ShowThisTab(int)));
    }
    else
        ui->tabWidget->setCurrentIndex( ui->tabWidget->indexOf(addlist->at(0)));

}

void MainWindow::on_action_delete_triggered()
{
    if(index_tab1.isValid() && index_tab1 == ui->tableView->currentIndex())
    {
        msgbx.setText("Удаление");
        msgbx.setInformativeText("Вы действительно хотите удалить выбранную записную книгу № "+QString::number(crud_model->actcrudlist.at(index_tab1.row())->zk_id));
        msgbx.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        int ret = msgbx.exec();

        switch (ret) {
        case QMessageBox::Cancel:
            break;
        case QMessageBox::Ok:
            if (index_tab1.isValid())
            {
                if (updlist != nullptr)
                    for (int i =0; i< updlist->size(); i++)
                        if(updlist->at(i)->new_cr->zk_id == crud_model->actcrudlist.at(index_tab1.row())->zk_id)
                        {  updlist->at(i)->close(); updlist->removeAt(i); break; }
                Crud::del_zk(crud_model->actcrudlist.at(index_tab1.row())->zk_id);

                RefreshTab();
            }
            break;
        }
    }
    else {
        QMessageBox::critical(this,QObject::tr("Внимание"),QObject::tr("Вы не выбрали ЗК для удаления!")); ///Хвалимся
    }
}

void MainWindow::on_action_update_triggered()
{
    if(index_tab1.isValid() && index_tab1 == ui->tableView->currentIndex())
    {
        //index_tab1 = ui->tableView->currentIndex();

        open_upd_tab(crud_model->actcrudlist.at(index_tab1.row()));
    }
    else {
        QMessageBox::critical(this,QObject::tr("Внимание"),QObject::tr("Вы не выбрали ЗК для изменения!")); ///Хвалимся
    }
}

void MainWindow::on_action_analysis_triggered()
{
    if (an == nullptr)
    {
        an = new class Analysis;
        ui->tabWidget_2->insertTab( ui->tabWidget_2->count()+1 ,an,"Анализ");
        ui->tabWidget_2->setCurrentIndex(ui->tabWidget_2->count()-1);
        connect(this, SIGNAL(Set_validators_an()), an, SLOT(set_validators()));
        emit Set_validators_an();
    }
    else
        ui->tabWidget_2->setCurrentIndex( ui->tabWidget_2->indexOf(an));

}

void MainWindow::on_action_search_triggered()
{
    if(sr == nullptr)
    {
     sr = new Search;
     ui->tabWidget_2->insertTab( ui->tabWidget_2->count()+1 ,sr,"Расширенный поиск");
     ui->tabWidget_2->setCurrentIndex(ui->tabWidget_2->count()-1);
     connect(sr, SIGNAL(Show_search_result(QList<Crud*>*)),this, SLOT(Search_result(QList<Crud*>*)));
     connect(sr,SIGNAL(Cancel_search()),this, SLOT(RefreshTab()));
    }
    else
    {
        ui->tabWidget_2->setCurrentIndex( ui->tabWidget_2->indexOf(sr));
    }
}

void MainWindow::Search_result(QList<Crud*> *crudlist)
{
    crud_model->setCrudlist(crudlist);
    ui->tableView->setModel(crud_model);
    Add_pagination_buttons();
    MainWindow::add_cancel_button();
}

void MainWindow::on_pushButton_clicked()
{
    if (ui->lineEdit->text() == QString(""))
    {
        RefreshTab();
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
            ui->tabWidget->setCurrentIndex(0);
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

    connect(p_b, SIGNAL(clicked()), this ,SLOT(RefreshTab()));
    }
}

void MainWindow::open_upd_tab(Crud *cr)
{

        if(updlist == nullptr)
        {
            updlist = new QList<Update*>;
        }

        for (int i=0; i<updlist->size(); i++)
        {
            if(updlist->at(i)->new_cr->zk_id == cr->zk_id)
            {
                ui->tabWidget->setCurrentIndex(i+1);
                return;
            }
        }

        Update *upd = new Update; //указатель на форму добавления
        upd->frm_t = Update_form;
        upd->imprt_t = Update_pg_data;

        //connect(this,SIGNAL(Send_data(Crud*, int)), updlist->at(updlist->size()-1), SLOT(Recieve_data(Crud*, int)));
        ///создание списка обусловлено работой класса листмастер
        /// он работает только со списками
        QList<Crud*> *crudlist = new QList<Crud*>;
        List_master *list = new List_master(Main_window_for_Update);
        list->set_counters();
        //Загрузка ВСЕХ данных выбранной ЗК
        list->fill_crud_list(crudlist,cr->zk_id, PSQLtype);
        //emit Send_data(crudlist->at(0), updlist->size()-1);

        updlist->append(upd);
        upd->Recieve_data(crudlist->at(0));
        connect (updlist->at(updlist->size()-1), SIGNAL (open_update_tab(Crud *)), this, SLOT(open_upd_tab(Crud *)));
        connect(updlist->at(updlist->size()-1), SIGNAL(Ready_for_update(int)), this, SLOT(ShowThisTab(int)));
        ui->tabWidget->insertTab( ui->tabWidget->count()+1 ,updlist->at(updlist->size()-1),"Редактировать ЗК № "+QString::number(updlist->at(updlist->size()-1)->new_cr->zk_id));
        ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
}

void MainWindow::set_validators()
{
    qDebug() << crud_model->crudlist->first()->zk_id << crud_model->crudlist->last()->zk_id;
    ui->lineEdit->setValidator(new QIntValidator(crud_model->crudlist->first()->zk_id,crud_model->crudlist->last()->zk_id));
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    ///обработка редактирования/добавления
    if ( ui->tabWidget->widget(index)->objectName() == "Update")
    {
        ui->tabWidget->widget(index)->deleteLater();
        Update *upd = dynamic_cast<Update*>(ui->tabWidget->widget(index)); //Приведение типа от виджета к классу
        qDebug() << upd->new_cr->zk_id;
    if (upd->frm_t == Add_form)
    {//Работа с добавлением
        if(addlist != nullptr)
        {
            addlist->clear();
            delete addlist;
            addlist = nullptr;
            return;
        }
    }
    if (upd->frm_t == Update_form)
        {
            if(updlist != nullptr)
            {
                for (int i=0; i<updlist->size(); i++)
                {
                    if(updlist->at(i)->new_cr->zk_id == upd->new_cr->zk_id)
                    {
                        delete  updlist->at(i);
                        updlist->removeAt(i);
                        if (updlist->isEmpty())
                            updlist = nullptr;
                        return;
                    }
                }
            }
        }
    }
    if ( ui->tabWidget->widget(index)->objectName() == "OfficialTelephones")
    {
        ui->tabWidget->widget(index)->deleteLater();
        delete of;
        of = nullptr;
        return;
    }

}

void MainWindow::on_tabWidget_2_tabCloseRequested(int index)
{
    if ( ui->tabWidget_2->widget(index)->objectName() == "Search")
    {
        qDebug() <<  ui->tabWidget_2->widget(index)->objectName();
        ui->tabWidget_2->widget(index)->deleteLater();
        delete sr;
        sr = nullptr;
        return;
    }
    if ( ui->tabWidget_2->widget(index)->objectName() == "Analysis")
    {
        qDebug() <<  ui->tabWidget_2->widget(index)->objectName();
        ui->tabWidget_2->widget(index)->deleteLater();
        delete an;
        an = nullptr;
        return;
    }
    if ( ui->tabWidget_2->widget(index)->objectName() == "master_export_Form")
    {
        qDebug() <<  ui->tabWidget_2->widget(index)->objectName();
        ui->tabWidget_2->widget(index)->deleteLater();
        delete exprt;
        exprt = nullptr;
        return;
    }
    if ( ui->tabWidget_2->widget(index)->objectName() == "Master_import_form")
    {
        qDebug() <<  ui->tabWidget_2->widget(index)->objectName();
        ui->tabWidget_2->widget(index)->deleteLater();
        delete imprt;
        imprt = nullptr;
        return;
    }

}

void MainWindow::on_action_official_tel_triggered()
{
    if(of == nullptr)
        {
        of = new OfficialTelephones;
        ui->tabWidget->insertTab( ui->tabWidget->count()+1 ,of,"Cлужебные телефоны");
        ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
        connect(this,SIGNAL(Fill_table_of()), of, SLOT(Fill_table()));
        emit Fill_table_of();
        }
    else
        ui->tabWidget->setCurrentIndex( ui->tabWidget->indexOf(of));

}

void MainWindow::on_actionexport_triggered()
{
    if(exprt == nullptr)
    {
        exprt = new Master_export_Form;
        ui->tabWidget_2->insertTab( ui->tabWidget_2->count()+1 ,exprt,"Экспорт данных");
        ui->tabWidget_2->setCurrentIndex(ui->tabWidget_2->count()-1);
        connect(exprt,SIGNAL(rb_zk_clicked()),this, SLOT(on_action_search_triggered()));
        connect(exprt,SIGNAL(rb_check_all()),this, SLOT(on_pb_check_model_clicked()));
        connect(exprt, SIGNAL(TESTING_export(QString,QString, bool, bool, bool)),this,SLOT(testing_export(QString, QString, bool, bool, bool)));
    }
    else
        ui->tabWidget_2->setCurrentIndex( ui->tabWidget_2->indexOf(exprt));

}

void MainWindow::testing_export(QString filename, QString password, bool cb_off_tels, bool cb_set_password, bool cb_zk)
{
    if( form_exprt == nullptr)
    {
        form_exprt = new For_export();

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
        form_exprt =nullptr;
        delete offtel;
        delete crud;
    }
}

void MainWindow::testing_opening(QString filename, QString password)
{
    ///Класс для импорта
    Import_Form *import_form = new Import_Form; //необходим доступ для
    connect(import_form,SIGNAL(Refresh_tab()),this,SLOT(RefreshTab()));
   if (import_form->Testing_open_db( filename,password)) //Если есть совпадение, то
   {
       ///Идем сравнивать выгруженный в список дамп с БД
       /// Метод алгоритма сравнения и импорта
       if(import_form->begin_import())
          {
           import_form->show(); //открываем форму
           import_form->showMaximized();
          }
   }
   else
       delete import_form;
}

void MainWindow::on_pb_check_model_clicked()
{
    switch (m_c_s)
    {
    case All_unchecked:
        crud_model->setCheckedCrudlist(crud_model->crudlist);
        ui->tableView->setModel(crud_model);
        m_c_s = All_checked;
    return;
    case All_checked:
        crud_model->setUnCheckedCrudlist(crud_model->crudlist);
        ui->tableView->setModel(crud_model);
        m_c_s = All_unchecked;
    return;
    }
}

void MainWindow::on_action_import_triggered()
{
    if(imprt == nullptr)
    {
        imprt = new Master_import_form;
        ui->tabWidget_2->insertTab( ui->tabWidget_2->count()+1 ,imprt,"Импорт данных");
        ui->tabWidget_2->setCurrentIndex(ui->tabWidget_2->count()-1);
        connect(imprt, SIGNAL(TESTING_open(QString,QString)), this, SLOT(testing_opening(QString, QString)));
    }
    else
        ui->tabWidget_2->setCurrentIndex( ui->tabWidget_2->indexOf(imprt));
}

void MainWindow::on_action_Settings_triggered()
{
        Settings_Form *settings = new Settings_Form;
        settings->show();
        connect (settings, SIGNAL(Update_main()), this, SLOT(RefreshTab()));
}

void MainWindow::next_page()
{
    crud_model->next_page_crud();
    ui->tableView->setModel(crud_model);
    Add_pagination_buttons();
}

void MainWindow::previous_page()
{
    crud_model->previous_page_crud();
    ui->tableView->setModel(crud_model);
    Add_pagination_buttons();
}

void MainWindow::set_fonts()
{

}

void MainWindow::add_splitter_lines()
{
    QSplitterHandle *handle = ui->splitter_3->handle(1);
    QVBoxLayout *layout = new QVBoxLayout(handle);
    layout->setSpacing(0);
    layout->setMargin(0);

    QFrame *line = new QFrame(handle);
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Sunken);
    layout->addWidget(line);

    QSplitterHandle *handle_2 = ui->splitter_2->handle(1);
    QVBoxLayout *layout_2 = new QVBoxLayout(handle_2);
    layout_2->setSpacing(0);
    layout_2->setMargin(0);

    QFrame *line_2 = new QFrame(handle_2);
    line_2->setFrameShape(QFrame::HLine);
    line_2->setFrameShadow(QFrame::Sunken);
    layout_2->addWidget(line_2);
}
////////////////////////////////////////////////////
void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    qDebug() << ui->tabWidget->tabBar()->accessibleTabName(index);
}


void MainWindow::on_action_add_1_triggered()
{
    on_action_add_triggered();
}

void MainWindow::on_action_2_upd_triggered()
{
    on_action_update_triggered();
}

void MainWindow::on_action_3_del_triggered()
{
    on_action_delete_triggered();
}

void MainWindow::on_action_5_show_triggered()
{
       ui->tabWidget->setCurrentIndex(0);
}

void MainWindow::on_tableView_3_doubleClicked(const QModelIndex &index)
{
    if(index.column() == 4 && contacts_model->actlist.at(index.row())->linked_id != 0)
    {
        int zk_id = contacts_model->actlist.at(index.row())->linked_id;
        for (int i = 0; i < crud_model->actcrudlist.size(); i++)
        {
            if(crud_model->actcrudlist.at(i)->zk_id == zk_id)
            {
                zk_id = i;
                break;
            }
        }
        index_tab1 = crud_model->index(zk_id,0);
        ui->tableView->setCurrentIndex(index_tab1);
        on_tableView_clicked(index_tab1, contacts_model->actlist.at(index.row())->contact_tel_num);
    }
}
