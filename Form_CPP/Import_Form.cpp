#include "Import_Form.h"
#include "ui_import_form.h"

Import_Form::Import_Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Import_Form)
{
    ui->setupUi(this);

    event = nullptr;//Зануляю, тк игнорируется компилятором в методе

    list = new List_master(Import);

    local_crud_model = new MTM_Crud;
    crud_model = new MTM_Crud;
    crud_model_pg = new MTM_Crud;

    tel_model = new MTM_OwTel;
    cont_model = new MTM_Contacts;
    off_model = new MTM_Off_Tels;

    a = 0; //Определение итераторов
    //Прячу первые колонки, тк там находится чекбокс
    ui->tableView_crud->setColumnHidden(1,true);
    ui->tableView_crud_pg->setColumnHidden(1,true);
    ui->pb_update_pg->setVisible(false);
}

Import_Form::~Import_Form()
{
    delete ui;
}

void Import_Form::closeEvent(QCloseEvent *event)
{
   (void)event;
    delete this;
}

void Import_Form::recieve_updated_import_crud(Crud *new_cr)
{
   ///Замена отредактированной ЗК на старую из списка
   Crud *cr = crud->at(crud->indexOf(crud_model->actcrudlist.at(0)));
   cr = new_cr;
   clear_models();
   clear_label();
   if (!begin_import()) //Если он еще раз провел сравнение
       closeEvent(event);// И ушел на импорт
}

void Import_Form::recieve_added_import_crud(Crud *new_cr)
{   //Заменяю старый и смещаю итератор с удалением из основной БД
    crud->insert(crud->indexOf(crud_model->actcrudlist.at(0)),new_cr);
    crud->removeAt(crud->indexOf(crud_model->actcrudlist.at(0)));

    del_list.append(crud_from_pg->zk_id);
    if(a<crud->size()-1)
    {
        a++;//Смещаю итератор на следующую ЗК
        clear_models();
        clear_label();
        if (!begin_import()) //
            closeEvent(event);;//снова иду на импорт
    }
    else
    {//Список кончился,проверим еще раз и пора делать импорт
        if (!add_to_db()) //
            delete this;
        else
            delete this;
    }
}


bool Import_Form::add_to_db()
{
  if(!del_list.isEmpty())
    list->del_zk_from_pg(del_list);

  if (crud->isEmpty())
  {
      QMessageBox::critical(this,QObject::tr("Ошибка"),QObject::tr("При сравнении не осталось ни одной записи для импорта")); ///Хвалимся
    return false;
  }

    if(list->insert_crud_in_db(crud))
    {
        QMessageBox::information(this,QObject::tr("Успех"),QObject::tr("Импорт прошел успешно!")); ///Хвалимся
        emit Refresh_tab();
        return true;
    } else
    {
        QMessageBox::critical(this,QObject::tr("Ошибка"),QObject::tr("При импорте возникли неполадки!")); ///Хвалимся
        return false; //Если нет совпадениий, необходимо начинать импорт
    }
}

void Import_Form::prepare_main_to_add(Crud *main_crud, Crud *added_crud)
{
    Crud *new_cr = new Crud;        //Новая запись

    new_cr->operator+(main_crud); //Скопировал все поля ЗК

    for(int a=0; a < main_crud->owt()->size();a++) //Взял список номеров
        new_cr->owt()->append(main_crud->owt()->at(a));

    int b=0;
    while(b<added_crud->owt()->size())
    {
        int i=0;
        while (i<new_cr->owt()->size())
        {
            if(new_cr->owt()->at(i)->tel_num == added_crud->owt()->at(b)->tel_num )
            {   //Если совпадение, то смещаем основной итератор
                goto mark;
            }
            else //Если нет, то проверяем со следующим телефоном
                i++;
        } //Прошло сравнение
        new_cr->owt()->append(added_crud->owt()->at(b));
        mark:
        b++;
    }

    Update *upd = new Update;
    upd->frm_t=Add_form;
    upd->imprt_t = Add_import_data;
    connect(this,SIGNAL(Send_data(Crud*)), upd, SLOT(recieve_import_data(Crud*)));
    connect(upd,SIGNAL(add_import_crud(Crud*)), this, SLOT(recieve_added_import_crud(Crud*)));


    emit Send_data(new_cr);
    upd->show();
}

bool Import_Form::compare_dump_db()
{
    QString query_for_nums, query_for_fio;

    while (a<crud->size())
    {
//        query_for_fio = " zk.lastname = ('"+crud->at(a)->lastname+"') AND"
//                        " zk.name = ('"+crud->at(a)->name+"') AND"
//                        " zk.mid_name = ('"+crud->at(a)->mid_name+"')";

//        while( b < crud->at(a)->owt()->size() )
//        {
//            if (query_for_nums.isEmpty())
//                query_for_nums += " owners_tel.Telephone_num = ('"+crud->at(a)->owt()->at(b)->tel_num+"') ";
//            else
//                query_for_nums += " OR owners_tel.Telephone_num = ('"+crud->at(a)->owt()->at(b)->tel_num+"')";
//            b++;
//        }

        for (int i=0; i < crud->at(a)->owt()->size(); i++)
        {
            if (!crud->at(a)->owt()->at(i)->tel_num.isEmpty() && crud->at(a)->owt()->at(i)->state != IsRemoved)
            //Составление запроса для проверки телефонов
            {
                if (query_for_nums.isEmpty())
                    query_for_nums += " owners_tel.Telephone_num = ('"+crud->at(a)->owt()->at(i)->tel_num+"') ";
                else
                    query_for_nums += " OR owners_tel.Telephone_num = ('"+crud->at(a)->owt()->at(i)->tel_num+"')";
             }
        }
        //Составление запроса для проверки фио и др
            query_for_fio = " zk.lastname = ('"+crud->at(a)->lastname+"') AND"
            " zk.name = ('"+crud->at(a)->name+"') AND"
            " zk.mid_name = ('"+crud->at(a)->mid_name+"') AND";

            if(!crud->at(a)->birth_date.isEmpty())
                query_for_fio += " zk.birth_date = ('"+crud->at(a)->birth_date+"') ";
            else
                query_for_fio += " zk.birth_date is NULL";

        qDebug() << query_for_nums << query_for_fio;

        Crud *cr = new Crud();

        if (!cr->compare_with_base(query_for_nums,query_for_fio))
        {
                Owners_tel *ow_at_crud = cr->owt()->at(0);
                QString _temp = ow_at_crud->tel_num;

                _temp.insert(0,"+");
                _temp.insert(2,"(");
                _temp.insert(6,")");
                _temp.insert(10,"-");
                _temp.insert(13,"-");

                QLabel *lb = new QLabel("<font size = 6>  <div align=\"left\"> Телефонный номер <b>"+_temp+"</b> "
                                        "принадлежащий владельцу импортируемой ЗК № <b>"+QString::number(crud->at(a)->zk_id)+"</b>"+
                                        " совпадает с телефонным номером владельца уже имеющейся ЗК № "
                                        "<b>"+QString::number(cr->owt()->at(0)->parentZK_id)+"</b> </div>  </font>");
                ui->vl_for_label->addWidget(lb);

                QList<Crud*> *temp = new QList<Crud*>;
                QList<Crud*> *temp2 = new QList<Crud*>;

                crud_model =      new MTM_Crud;
                temp->append(crud->at(a));
                crud_model->setCrudlist(temp);
                ui->tableView_crud->setModel(crud_model);

                crud_model_pg =   new MTM_Crud;
                crud_from_pg = new Crud;//Обнуляю этот круд
                crud_from_pg = list->get_crud(cr->owt()->at(0)->parentZK_id);
                temp2->append(crud_from_pg);
                crud_model_pg->setCrudlist(temp2);
                ui->tableView_crud_pg->setModel(crud_model_pg);

                tel_mod =         new MTM_OwTel;
                for (int b =0; b< crud->at(a)->owt()->size(); b++) //помечаю телефон
                {
                if (crud->at(a)->owt()->at(b)->tel_num == ow_at_crud->tel_num )
                    {
                        tel_mod->mark_rows.append(b);     //помечаю телефон
                        break;
                    }
                }
                tel_mod->setOTList(crud->at(a)->owt());
                ui->tableView_owt->setModel(tel_mod);

                tel_mod_pg =      new MTM_OwTel;
                for (int c =0; c< crud_from_pg->owt()->size(); c++) //помечаю телефон
                {
                if (crud_from_pg->owt()->at(c)->tel_num == ow_at_crud->tel_num )
                    {
                        tel_mod_pg->mark_rows.append(c);
                        break;
                    }
                }

               tel_mod_pg->setOTList(crud_from_pg->owt());


               ui->tableView_owt_pg->setModel(tel_mod_pg);
               delete temp; delete temp2;

            return true;
        }
            //qDebug() << cr->zk_id << cr->owt()->at(0)->parentZK_id;
    a++;
    }

    return false;
}


void Import_Form::on_tableView_crud_pg_clicked(const QModelIndex &index)
{
    crud_model_pg->actcrudlist.at(0)->check();
    //qDebug() << crud_pg_local->indexOf(crud_model_pg->actcrudlist.at(0));
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
    del_list.append(crud_from_pg->zk_id);
    if(a<crud->size()-1)
    {
        a++;  //Смещаю итератор на следующую ЗК
        clear_models();
        clear_label();
        if (!begin_import()) //
            closeEvent(event);;//снова иду на импорт
    }
    else
    {//Список кончился,проверим еще раз и пора делать импорт
        if (!add_to_db()) //
            delete this;
        else
            delete this;
    }
}

void Import_Form::on_pb_save_main_clicked()
{
    ///Сохранение ЗК из БД = удаление из списка дампа
    crud->removeAt(crud->indexOf(crud_model->actcrudlist.at(0)));
    if(a<crud->size())
    {
        clear_models();
        clear_label();
       if(!begin_import())//снова иду на импорт
           delete this;
    }
    else
    {//Список кончился,проверим еще раз и пора делать импорт
        if (!add_to_db()) //
            delete this;
        else
            delete this;
    }
}

void Import_Form::on_pb_skip_import_clicked()
{
    ///Аналогично сохранению имеющейся ЗК
    ///Удаляем из списка бекапа совпавшую ЗК
    on_pb_save_main_clicked();
}

void Import_Form::on_pb_skip_All_clicked()
{
    /// Сделать рекурсию на метод compare_dump_db
    /// и пока false чистить локальный список как в методе
    /// on_pb_save_main_clicked()
    crud->removeAt(crud->indexOf(crud_model->actcrudlist.at(0)));
    if(a<crud->size())
    {
        clear_models();
        clear_label();
        //Если нашлось совпадение, вызываем этот метод снова
        if(begin_import())
            on_pb_skip_All_clicked();
        else
            delete this;
    }
    else
    {//Список кончился,проверим еще раз и пора делать импорт
        if (!add_to_db()) //
            delete this;
        else
            delete this;
    }
}


bool Import_Form::Testing_open_db(QString filename, QString password)
{ //true - признак открытия формы импорта, с выбором совпадений
  //false - либо успешный импорт, либо нет необходимости открывать
  //пустую форму, а достаточно вывести сообщение об ошибке

        /// В зависимости от наличия пароля меняем драйвер
        crud = new QList <Crud*>;   /// Инициализируем списки
        offtel = new QList<Off_tels*>;

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
          {
     QMessageBox::critical(this,
                   QObject::tr("Внимание"),
                   QObject::tr("НЕВЕРНЫЙ ПАРОЛЬ"));
              return false;
          }
       }

       db->db().setDatabaseName(db_file.fileName());

       list->fill_all_crud_list(crud, type); /// Заполняю списки из дампа
       list->fill_off_tels(offtel,type);

    ///Закрываем файл и БД
    db_file.close();
    db->db().close();
    db->set_Sql_type(PSQLtype); /// Перевожу обратно на PSQL, тк работаю в основном с ним
    qDebug() << db->db_connect()<<db->db().lastError();

       ///Перед операцией слияния дампа и основной бд необходимо проверить
       /// оба списка на повторы
    if(!crud->isEmpty() || !offtel->isEmpty()) // Удачно выгрузил даные в локальный список
    {
          return true;
    }
    else {
        QMessageBox::critical(this,
                      QObject::tr("Внимание"),
                      QObject::tr("Файл либо пуст либо не требует пароль для доступа"));
    return false;
    }
}


bool Import_Form::begin_import()
{

    if(compare_dump_db())//возвращает true если есть совпадение
        return true;
    else
    {
    QMessageBox::information(this,QObject::tr("Успех"),QObject::tr("Сравнение прошло успешно!")); ///Хвалимся
        if(add_to_db())//Начинаем импорт
        {//В любом случае - закрываем
            return false;
        }
        else
        {
            return false;
        }
    }
}

void Import_Form::on_pb_save_import_slot(QString str)
{
    qDebug() << str;
}


void Import_Form::on_pb_update_import_zk_clicked()
{
    Update *upd = new Update;
    upd->frm_t=Add_form;
    upd->imprt_t = Update_import_data;
    connect(this,SIGNAL(Send_data(Crud*)), upd, SLOT(recieve_import_data(Crud*)));
    connect(upd,SIGNAL(updated_import_crud(Crud*)), this, SLOT(recieve_updated_import_crud(Crud*)));
    emit Send_data(crud_model->actcrudlist.at(0));
    upd->show();
}

void Import_Form::on_pb_update_pg_clicked()
{
     Update *upd = new Update;
     upd->frm_t= Update_form; //Подгрузка из бд при клике
     upd->imprt_t = Update_pg_data_import;
     connect(this,SIGNAL(Send_data(Crud*)), upd, SLOT(recieve_import_data(Crud*)));
     connect(upd,SIGNAL(update_import_pg()), this, SLOT(begin_import()));
     emit Send_data(crud_model_pg->actcrudlist.at(0));

     upd->show();
}

void Import_Form::on_pushButton_clicked()
{
    QMessageBox msgbx;
    msgbx.setText("<font size = '5'> Вы собираетесь объеденить две записи в одну. <br> Какую выбрать в качестве основной?</font>");
    msgbx.setStandardButtons(QMessageBox::Ok | QMessageBox::Open | QMessageBox::Cancel);
    msgbx.setButtonText(QMessageBox::Ok,"Импортируемую ЗК №"+ QString::number(crud_model->actcrudlist.at(0)->zk_id));
    msgbx.setButtonText(QMessageBox::Open,"Основную ЗК №"+ QString::number(crud_model_pg->actcrudlist.at(0)->zk_id));
    msgbx.setButtonText(QMessageBox::Cancel,"Отмена");
    int ret = msgbx.exec();

    switch (ret)
    {
    case QMessageBox::Ok:
    prepare_main_to_add(crud_model->actcrudlist.at(0),crud_model_pg->actcrudlist.at(0));
        break;

    case QMessageBox::Open:
        prepare_main_to_add(crud_model_pg->actcrudlist.at(0),crud_model->actcrudlist.at(0));
        break;

    case QMessageBox::Cancel:
        break;
    }
}
