#include "Import_Form.h"
#include "ui_import_form.h"

Import_Form::Import_Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Import_Form)
{
    ui->setupUi(this);

    list = new List_master(Import);

    local_crud_model = new MTM_Crud;
    crud_model = new MTM_Crud;
    crud_model_pg = new MTM_Crud;

    tel_model = new MTM_OwTel;
    cont_model = new MTM_Contacts;
    off_model = new MTM_Off_Tels;
}

Import_Form::~Import_Form()
{
    delete ui;
}

QList<Crud *> Import_Form::setCrud()
{

}

void Import_Form::closeEvent(QCloseEvent *event)
{
   (void)event;
    delete this;
}

void Import_Form::recieve_all(QList<Crud *> *crudlist, QList<Off_tels*>  *offtel, QList <Crud*> *crudlist_pg, QList<Off_tels*>  *offtel_pg)
{
    crud_local = crudlist;
    crud_pg_local = crudlist_pg;
    offtel_local = offtel;
    offtel_pg_local = offtel_pg;
}

bool Import_Form::compare_dump_db()
{
    crud_model->setCrudlist(crud_local);
    ui->tableView_zk->setModel(crud_model);

    off_model->setOffTList(offtel_local);
    ui->tableView_off_tels->setModel(off_model);

    for(int a =0; a < crud_pg_local->size(); a++)//Список бд
        for(int b=0; b<crud_local->size(); b++)//Список дампа
        {
            for(int c =0; c< crud_pg_local->at(a)->owt()->size(); c++) //Телефоны бд
                for(int d = 0; d< crud_local->at(b)->owt()->size(); d++) //Телефоны дампа
                {
                    if(crud_pg_local->at(a)->owt()->at(c)->tel_num == crud_local->at(b)->owt()->at(d)->tel_num)
                    {//если совпадают номера
                        QLabel *lb = new QLabel("<font size = 9> <h3> <div align=\"left\"> Телефонный номер "+crud_pg_local->at(a)->owt()->at(c)->tel_num+" "
                                               "принадлежащий владельцу импортируемой ЗК № "+QString::number(crud_local->at(b)->zk_id)+
                                               " совпадает с телефонным номером владельца уже имеющейся ЗК № "
                                               ""+QString::number(crud_pg_local->at(a)->zk_id)+" </div> </h3> </font>");
                        ui->vl_for_label->addWidget(lb);

                        //// делаю костыль, тк не знаю как исправить
                        QList<Crud*> *temp = new QList<Crud*>;
                        QList<Crud*> *temp2 = new QList<Crud*>;

                        temp->append(crud_local->at(b));
                        temp2->append(crud_pg_local->at(a));

                        local_crud_model->setCrudlist(temp);
                        crud_model_pg->setCrudlist(temp2);

                        ui->tableView_crud->setModel(local_crud_model);
                        ui->tableView_crud_pg->setModel(crud_model_pg);
                        ///Конец костылей

                        tel_mod = new MTM_OwTel;
                        tel_mod_pg = new MTM_OwTel;
                        tel_mod->setOTList(crud_local->at(b)->owt());
                        tel_mod_pg->setOTList(crud_pg_local->at(a)->owt());
                        ui->tableView_owt->setModel(tel_mod);
                        ui->tableView_owt_pg->setModel(tel_mod_pg);

                        delete temp;
                        delete temp2;
                        return false;
                    }
                }
        }
    return true;
}

void Import_Form::on_tableView_zk_clicked(const QModelIndex &index)
{
  tel_model->setOTList(crud_model->actcrudlist.at(index.row())->owt());
  ui->tableView_tels->setModel(tel_model);
}

void Import_Form::on_tableView_tels_clicked(const QModelIndex &index)
{
    cont_model->setContactList(tel_model->actotlist.at(index.row())->cont());
    ui->tableView_contacts->setModel(cont_model);
}

void Import_Form::on_tableView_crud_pg_clicked(const QModelIndex &index)
{
    crud_model_pg->actcrudlist.at(0)->check();
    qDebug() << crud_pg_local->indexOf(crud_model_pg->actcrudlist.at(0));
}

void Import_Form::clear_models()
{
    tel_mod->reset_OTModel();
    tel_mod_pg->reset_OTModel();
    crud_model->reset_CrudModel();
    crud_model_pg->reset_CrudModel();
}

void Import_Form::clear_label()
{
    QLayout *l = ui->vl_for_label->layout();
    while(l->count() != 0)
    {
        QLayoutItem *item = l->takeAt(0);
        delete item->widget();
    }
}

void Import_Form::on_pb_save_import_clicked()
{
    /// Сохранение ЗК из дампа = удаление из БД (записываем номер)
    /// и удаление из локального списка

    del.append(crud_model_pg->actcrudlist.at(0)->zk_id);//??
    crud_pg_local->removeAt(crud_pg_local->indexOf(crud_model_pg->actcrudlist.at(0)));

    clear_models();
    clear_label();
    if(!compare_dump_db())
        qDebug() << del;
}

void Import_Form::on_pb_save_main_clicked()
{
    ///Сохранение ЗК из БД = удаление из списка дампа
    crud_local->removeAt(crud_local->indexOf(local_crud_model->actcrudlist.at(0)));


    clear_models();
    clear_label();
    if(!compare_dump_db())
        qDebug() << del;
}

void Import_Form::on_pb_skip_import_clicked()
{
    ///Аналогично сохранению имеющейся ЗК
    ///Удаляем из списка бекапа совпавшую ЗК
    on_pb_save_main_clicked();
}

void Import_Form::on_pb_skip_All_clicked()
{
    /// Отмена Импорта
    /// Сделать рекурсию на метод compare_dump_db
    /// и пока false чистить локальный список как в методе
    /// on_pb_save_main_clicked()
}


Import_state Import_Form::Testing_open_db(QString filename, QString password)
{
        SqlType type; /// В зависимости от наличия пароля меняем драйвер
        crud = new QList <Crud*>;
        offtel = new QList<Off_tels*>;
        crud_from_pg = new QList <Crud*>;
        offtel_from_pg = new  QList<Off_tels*>;
       db_connection *db = db_connection::instance();
       db_file.setFileName(filename);

       if (password.isEmpty())               ///Если пароля нет, то Склайт
       {
           type = SQLliteType;
           db->set_Sql_type(type);
       }
       else
       {
           type = SQLlitechipher;
           db->set_Sql_type(type);    ///Если есть,то заходим с паролем
          if (!db->db().open("user",password) )
                return Password_incorrect;
       }

       db->db().setDatabaseName(db_file.fileName());


       list->fill_all_crud_list(crud, type); /// Заполняю списки из дампа
       list->fill_off_tels(offtel,type);

       ///Перед операцией слияния дампа и основной бд необходимо проверить
       /// оба списка на повторы
if(!crud->isEmpty() || !offtel->isEmpty()) //либо пустой дамп
{                                          //либо неверный пароль
    list->fill_all_crud_list(crud_from_pg,PSQLtype); ///Заполняю списки из БД
    list->fill_off_tels(offtel_from_pg, PSQLtype);
       ///Возможно это нехорошо, тк занимает много времени

    /// Делаю отправленные списки локальными
    recieve_all(crud, offtel, crud_from_pg, offtel_from_pg);

    /// Импорт, он же и проверка на повторы
    if(begin_import())
    {
        qDebug()<<"Импорт с первого раза";
        db_file.close();
        db->db().close();
        db->set_Sql_type(PSQLtype); /// Перевожу обратно на PSQL, тк работаю в основном с ним
        qDebug() << db->db_connect()<<db->db().lastError();
        return Import_succesful;
    }
    else
    {
        db_file.close();
        db->db().close();
        db->set_Sql_type(PSQLtype); /// Перевожу обратно на PSQL, тк работаю в основном с ним
        qDebug() << db->db_connect()<<db->db().lastError();
        return Import_in_progress;
    }
}
else {
    db_file.close();
    db->db().close();
    db->set_Sql_type(PSQLtype); /// Перевожу обратно на PSQL, тк работаю в основном с ним
    qDebug() << db->db_connect()<<db->db().lastError();
    return Import_abort;
    }
}

bool Import_Form::begin_import()
{///Придется вызывать эту функцию рекурентно, до тех пор, пока
 /// не будет true
    if(compare_dump_db())
    {///Если данные совпали
  //QObject::connect(import, SIGNAL(on_pb_save_import(QString)), this, SLOT(on_pb_save_import_slot(QString)));
        return false;
    }
    else
    {
        return true;
    }
}

void Import_Form::on_pb_save_import_slot(QString str)
{
    qDebug() << str;
}

