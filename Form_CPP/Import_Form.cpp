#include "Import_Form.h"
#include "ui_import_form.h"

Import_Form::Import_Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Import_Form)
{
    parent_win = parent;
    ui->setupUi(this);

    event = nullptr;//Зануляю, тк игнорируется компилятором в методе

    list = new List_master(Import);

    local_crud_model = new MTM_Crud;
    crud_model = new MTM_Crud;
    crud_model_pg = new MTM_Crud;

    tel_model = new MTM_OwTel;
    off_model = new MTM_Off_Tels;

    a = 0; //Определение итераторов
    //Прячу первые колонки, тк там находится чекбокс
    ui->tableView_crud->setColumnHidden(1,true);
    ui->tableView_crud_pg->setColumnHidden(1,true);
    ui->pb_update_pg->setVisible(false);
        actual_size = this->size();
        setWindowTitle("Мастер импорта");
}

Import_Form::~Import_Form()
{
    if(crud != nullptr)
        Crud::delete_all(crud);
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

void Import_Form::abort_link_import()
{
    crud->removeOne(crud->at(a)); //Удаляю текущую ЗК
    for (int r = 0; r<linked_id_list->size(); r++)
    {
        for (int v = 0; v<crud->size(); v++)
        {
            if(crud->at(v)->zk_id == linked_id_list->at(r))
            {
                crud->removeOne(crud->at(v)); //Удаляю связанные с ней ЗК
                break;
            }

        }
    }
    delete linked_id_list;

    linked_id_list=nullptr;

    clear_models();
     clear_label();
     if (!begin_import()) //Если он еще раз провел сравнение
         closeEvent(event);// И ушел на импорт
}

void Import_Form::import_list_with_link()
{
    if(links_vector == nullptr)
        links_vector = new QVector<QVector<int> >;

    QList<Crud*> *new_crudlist = new QList<Crud*>;//Собираем цепочку
    linked_id_list->push_front(crud->at(a)->zk_id);//Как первый элемент цепочки

    for (int v = 0; v<linked_id_list->size(); v++)
    {
        for (int z = 0; z < crud->size(); z++)
        {
            if (crud->at(z)->zk_id == linked_id_list->at(v))
        {
            new_crudlist->append(crud->at(z));
            crud->removeAt(z);
            z--;
            break;
        }
        }
        QVector<int> temp_vector;
        temp_vector.append(linked_id_list->at(v));
        temp_vector.append(crud_model_pg->actcrudlist.at(0)->zk_id);
        links_vector->append(temp_vector);
    }
    for (int g=0;g<new_crudlist->size();g++)
    {
        crud->insert(0,new_crudlist->at(g));
    }
    a=a+new_crudlist->size(); //Сдвиг итератора

    delete new_crudlist;
    delete linked_id_list;
    linked_id_list=nullptr;
    clear_models();
    clear_label();
    if (!begin_import()) //
        closeEvent(event);;//снова иду на импорт
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

void Import_Form::make_link_clicked()
{
    //Сохранить со связью
    if(vector == nullptr)
        vector = new QVector<QVector<QString>   >;

    QString row_id1 = crud_model->actcrudlist.at(0)->row_id;
    QString row_id2 = crud_model_pg->actcrudlist.at(0)->row_id;
    QVector<QString> local_vector;
    local_vector.append(row_id1);
    local_vector.append(row_id2);
    vector->append(local_vector);
    a++;
    clear_models();
    clear_label();
    if (!begin_import()) //Если он еще раз провел сравнение
        closeEvent(event);// И ушел на импорт
}


bool Import_Form::add_to_db()
{
  if(!del_list.isEmpty())
  {
    switch(form_state)
    {
        case zk:
        list->del_zk_from_pg(del_list);
        break;
    case official_tel:
        list->del_offt_from_pg(del_list);
        break;
    }
  }

  if (!crud->isEmpty())
  {

    QList<int> *templist = nullptr;
    if(list->insert_crud_in_db(crud,templist,links_vector,vector, old_db))
    {
//        if(vector != nullptr)
//            if(!vector->isEmpty())
//            {
//                list->fill_links(vector);
//            }

        QMessageBox::information(this,QObject::tr("Успех"),QObject::tr("Импорт прошел успешно! Импортировано %1 записных книг").arg(QString::number(crud->size()))); ///Хвалимся
        emit Refresh_tab();

        if(ddb != nullptr)
            ddb->accepted();
        return true;
    } else
    {
        QMessageBox::critical(this,QObject::tr("Ошибка"),QObject::tr("При импорте возникли неполадки!")); ///Хвалимся
        if(ddb != nullptr)
            ddb->rejected();

        return false; //Если нет совпадениий, необходимо начинать импорт
    }
  } else if(!offtel->isEmpty())
  {
     bool isOk = list->insert_off_tel_in_db(offtel);
     if(!isOk)
     {
         QMessageBox::critical(this,QObject::tr("Ошибка"),QObject::tr("При импорте возникли неполадки!")); ///Хвалимся
         if(ddb != nullptr)
             ddb->rejected();
         return false;
     } else
     {
         QMessageBox::information(this,QObject::tr("Успех"),QObject::tr("Импорт прошел успешно! Импортировано %1 служебных телефонов").arg(QString::number(offtel->size()))); ///Хвалимся
         emit Refresh_tab();

         if(ddb != nullptr)
             ddb->accepted();
         return true;
     }
  }
  else
  {
      QMessageBox::critical(this,QObject::tr("Ошибка"),QObject::tr("При сравнении не осталось ни одной записи для импорта")); ///Хвалимся
      if(ddb != nullptr)
          ddb->rejected();
      return false;
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
    upd->setWindowTitle("Слияние ЗК");
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
    db_connection *db = db_connection::instance();
    db->set_Sql_type(PSQLtype);

    if(a<crud->size())
        while (a<crud->size())
        {
    ///Проверка по ююид
    if (crud->at(a)->row_id != QString::number(1) )
    {
        //Сперва проверим по роу айди
        QSqlQuery temp(db_connection::instance()->db());
        temp.prepare("SELECT zk.zk_id FROM zk WHERE zk.row_id = ('"+crud->at(a)->row_id+"')");
        if(!temp.exec())
            qDebug() << temp.executedQuery();
        else
        {
            while(temp.next())
            {
                QLabel *lb = new QLabel("<font size = 6>  <div align=\"left\"> Уникальный идентификатор ЗК  <b>"+QString::number(crud->at(a)->zk_id)+"</b>"+
                                        " совпадает с уникальным идентификатором "
                                        "<b>"+QString::number(temp.value(0).toInt())+"</b> </div>  </font>");
                ui->vl_for_label->addWidget(lb);

               QList<int> *import_crud_links = nullptr;

                if(pass != nullptr )
                    import_crud_links =  Crud::take_links(crud->at(a)->row_id, SQLlitechipher, Local_filename, pass);
                else
                   import_crud_links =  Crud::take_links(crud->at(a)->row_id, SQLliteType, Local_filename);


                if(import_crud_links != nullptr && !import_crud_links->isEmpty())
                {
                    if(linked_id_list != nullptr)
                        delete linked_id_list;

                    linked_id_list = new QList<int>;
                    linked_id_list = import_crud_links;         //Сделал список глобальным для класса
                    QLabel *label_for_compares  = new QLabel;
                    QString Label_text = "<font size = 10><i>РАБОТА СО СПИСКОМ:</i></font><font size = 6> ЗК №"+QString::number(crud->at(a)->zk_id)+" связана с импортируемыми ЗК: ";
                    for (int a =0; a<import_crud_links->size(); a++)
                    {
                        if(a == 0)
                            Label_text += QString::number(import_crud_links->at(a));
                        else
                            Label_text +=", " + QString::number(import_crud_links->at(a));
                    }
                    Label_text += "</font>";
                    label_for_compares->setText(Label_text);
                    ui->vl_for_count_of_compares->addWidget(label_for_compares);
                    QPushButton *pb = new QPushButton("Перейти к работе со списком");
                    ui->vl_for_pb->addWidget(pb);
                    connect(pb,SIGNAL(clicked()), this, SLOT(begin_work_with_links()));
                }

                QList<Crud*> *temp1 = new QList<Crud*>;
                QList<Crud*> *temp2 = new QList<Crud*>;

                crud_model =      new MTM_Crud;
                temp1->append(crud->at(a));
                crud_model->setCrudlist(temp1);
                ui->tableView_crud->setModel(crud_model);

                crud_model_pg =   new MTM_Crud;
                crud_from_pg = new Crud;//Обнуляю этот круд
                crud_from_pg = list->get_crud(temp.value(0).toInt());
                temp2->append(crud_from_pg);
                crud_model_pg->setCrudlist(temp2);
                ui->tableView_crud_pg->setModel(crud_model_pg);

                tel_mod =         new MTM_OwTel;

                tel_mod->setOTList(crud->at(a)->owt());
                ui->tableView_owt->setModel(tel_mod);

                tel_mod_pg =      new MTM_OwTel;


                tel_mod_pg->setOTList(crud_from_pg->owt());


                ui->tableView_owt_pg->setModel(tel_mod_pg);

                delete temp1; delete temp2;

                return true;

            }
        }
    }

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
            //После проверки на совпадения необходимо убедится - связана ли импортируемая ЗК
            if(crud->at(a)->row_id != QString::number(1) )
            {
                QList<int> *import_crud_links = Crud::take_links(crud->at(a)->row_id, SQLliteType, Local_filename);
               if(import_crud_links != nullptr)
                {
                    if(!import_crud_links->isEmpty())
                    {
                        if(linked_id_list != nullptr)
                            delete linked_id_list;

                        linked_id_list = new QList<int>;
                        linked_id_list = import_crud_links;         //Сделал список глобальным для класса

                        QLabel *label_for_compares  = new QLabel;
                        QString Label_text = "<font size = 10><i>РАБОТА СО СПИСКОМ:</i></font><font size = 6> ЗК №"+QString::number(crud->at(a)->zk_id)+" связана с импортируемыми ЗК: ";
                        for (int a =0; a<import_crud_links->size(); a++)
                        {
                            if(a == 0)
                                Label_text += QString::number(import_crud_links->at(a));
                            else
                                Label_text +=", " + QString::number(import_crud_links->at(a));
                        }
                        Label_text += "</font>";
                        label_for_compares->setText(Label_text);
                        ui->vl_for_count_of_compares->addWidget(label_for_compares);
                        QPushButton *pb = new QPushButton("Перейти к работе со списком");
                        ui->vl_for_pb->addWidget(pb);
                        connect(pb,SIGNAL(clicked()), this, SLOT(begin_work_with_links()));
                    }
                }
            }

                QLabel *lb= nullptr;
                if(cr->compare_result->at(0).Tel_num != "NULL") //Совпадение по телефону
                {
                    db->set_Sql_type(PSQLtype);
                    QString _temp = cr->compare_result->at(0).Tel_num;
                    if (cr->compare_result->at(0).Tel_num.size() == 11)
                    {
                        _temp.insert(0,"+");
                        _temp.insert(2,"(");
                        _temp.insert(6,")");
                        _temp.insert(10,"-");
                        _temp.insert(13,"-");
                    }

                    lb = new QLabel("<font size = 6>  <div align=\"left\"> Телефонный номер <b>"+_temp+"</b> "
                                            "принадлежащий владельцу импортируемой ЗК № <b>"+QString::number(crud->at(a)->zk_id)+"</b>"+
                                            " совпадает с телефонным номером владельца уже имеющейся ЗК № "
                                            "<b>"+QString::number(cr->compare_result->at(0).id)+"</b> </div>  </font>");
                } else
                    lb = new QLabel("<font size = 6>  <div align=\"left\"> ФИО и день рождения импортируемой ЗК №<b>"+QString::number(crud->at(a)->zk_id)+"</b> "
                                            " совпадает с ФИО и днем рождения уже имеющейся ЗК № "
                                            "<b>"+QString::number(cr->compare_result->at(0).id)+"</b> </div>  </font>");
                ui->vl_for_label->addWidget(lb);

                QList<Crud*> *temp = new QList<Crud*>;
                QList<Crud*> *temp2 = new QList<Crud*>;

                crud_model =      new MTM_Crud;
                temp->append(crud->at(a));
                crud_model->setCrudlist(temp);
                ui->tableView_crud->setModel(crud_model);

                crud_model_pg =   new MTM_Crud;
                crud_from_pg = new Crud;//Обнуляю этот круд
                crud_from_pg = list->get_crud(cr->compare_result->at(0).id);
                temp2->append(crud_from_pg);
                crud_model_pg->setCrudlist(temp2);
                ui->tableView_crud_pg->setModel(crud_model_pg);

                tel_mod =         new MTM_OwTel;
                for (int b =0; b< crud->at(a)->owt()->size(); b++) //помечаю телефон
                {
                if (crud->at(a)->owt()->at(b)->tel_num == cr->compare_result->at(0).Tel_num )
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
                if (crud_from_pg->owt()->at(c)->tel_num == cr->compare_result->at(0).Tel_num )
                    {
                        tel_mod_pg->mark_rows.append(c);
                        break;
                    }
                }

               tel_mod_pg->setOTList(crud_from_pg->owt());

               if(crud_model->actcrudlist.at(0)->row_id != crud_model_pg->actcrudlist.at(0)->row_id)
               {
                    QPushButton *pb = new QPushButton("Сохранить со связью");
                    ui->vl_pb_for_make_link->addWidget(pb);
                    connect(pb,SIGNAL(clicked()), this, SLOT(make_link_clicked()));
               }

               ui->tableView_owt_pg->setModel(tel_mod_pg);
               delete temp; delete temp2;

            return true;
        }
            //qDebug() << cr->zk_id << cr->owt()->at(0)->parentZK_id;
    a++;
    }
    else
        return false;
    return false;
}

Import_state Import_Form::compare_of_t()
{
    if (offtel_pg != nullptr)
    {
        Off_tels::clear_list(offtel_pg);
        delete offtel_pg;
        offtel_pg = nullptr;
    }

    if(a<offtel->size())
        while (a<offtel->size())
        {
            ///проверка
            QString query = " tel_num = '"+offtel->at(a)->tel_num+"' "
                    " OR service_name = '"+offtel->at(a)->service_name+"'";
            QList<Off_tels *> *matches = Off_tels::compare_with_base(query);
            if(matches == nullptr)
            {
                return Import_abort;        //ошибка при заполнении
            } else
                if(!matches->isEmpty())     //нашлись совпадения
                {
                    QLabel *lb;
                    if(matches->size() > 1)
                    lb = new QLabel("<font size = 6>  <div align=\"left\"> Служебный телефон  <b>"+offtel->at(a)->tel_num+"</b>"+
                                            " совпадает со следующими номерами служб: ");
                    else
                        lb = new QLabel("<font size = 6>  <div align=\"left\"> Служебный телефон  <b>"+offtel->at(a)->tel_num+"</b>"+
                                                " совпадает с номером службы:"+ matches->at(0)->service_name);
                    ui->vl_for_label->addWidget(lb);

                    off_model = new MTM_Off_Tels;
                    off_model_pg = new MTM_Off_Tels;

                    QList<Off_tels *> *templist = new QList<Off_tels *>;
                    templist->append(offtel->at(a));
                    off_model->setOffTList(templist);

                    off_model_pg->setOffTList(matches);

                    ui->tableView_crud->setModel(off_model);
                    ui->tableView_crud_pg->setModel(off_model_pg);
                    ui->tableView_crud->resizeColumnsToContents();
                             ui->tableView_crud->horizontalHeader()->setStretchLastSection(true);
                    ui->tableView_crud_pg->resizeColumnsToContents();
                             ui->tableView_crud_pg->horizontalHeader()->setStretchLastSection(true);

                    ui->tableView_owt->setVisible(false);
                    ui->tableView_owt_pg->setVisible(false);
                    ui->pb_update_pg->setVisible(false);
                    ui->pb_update_import_zk->setVisible(false);
                    ui->pushButton->setVisible(false);
                    ui->pb_skip_All->setVisible(false);
                    offtel_pg = matches;

                    return Import_conflict;
                }
                else
                    if(matches->isEmpty())
                    {
                        a++;
                        return Import_in_progress;  //совпадений нет
                    }
        }
    return Import_succesful;    //Все проверили

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
    QLayout *a = ui->vl_for_count_of_compares->layout();
    while(a->count() != 0)
    {
        QLayoutItem *item = a->takeAt(0);
        delete item->widget();
    }
    QLayout *b = ui->vl_for_pb->layout();
    while(b->count() != 0)
    {
        QLayoutItem *item = b->takeAt(0);
        delete item->widget();
    }
    QLayout *c = ui->vl_pb_for_make_link->layout();
    while(c->count() != 0)
    {
        QLayoutItem *item = c->takeAt(0);
        delete item->widget();
    }
}

void Import_Form::on_pb_save_import_clicked()
{
    /// Сохранение ЗК из дампа = удаление из БД (записываем номер)
    /// и удаление из локального списка
   if(!crud->isEmpty())
   {
       del_list.append(crud_from_pg->zk_id);
       form_state = zk;
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
   } else if(!offtel->isEmpty())
   {
       for (int i =0; i < offtel_pg->size(); i++)
       {

           del_list.append(offtel_pg->at(i)->of_t_id);
       }
       form_state = official_tel;

       if(a<offtel->size()-1)
       {
           a++;  //Смещаю итератор на следующую ЗК

           off_model->reset_off_t_Model();
           off_model_pg->reset_off_t_Model();

           clear_label();
           if (!begin_import_of_t()) //
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
}

void Import_Form::on_pb_save_main_clicked()
{
    if(!crud->isEmpty())
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
    } else
        if(!offtel->isEmpty())
        {
            ///Сохранение ЗК из БД = удаление из списка дампа
            offtel->removeAt(offtel->indexOf(off_model->actofflist.at(0)));
            if(a<offtel->size())
            {
                off_model->reset_off_t_Model();
                off_model_pg->reset_off_t_Model();
                clear_label();
                if(!begin_import_of_t())//снова иду на импорт
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

    db_file.setFileName(filename);
        if(filename.endsWith(".db"))
        {
            if (password != nullptr && !password.isEmpty())
                    pass = password;

            /// В зависимости от наличия пароля меняем драйвер
            crud = new QList <Crud*>;   /// Инициализируем списки
            offtel = new QList<Off_tels*>;

            Local_filename = filename;
            db_connection *db = db_connection::instance();

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
                    ///Закрываем файл и БД
                    db_file.close();
                    db->db().close();
                    db->set_Sql_type(PSQLtype); /// Перевожу обратно на PSQL, тк работаю в основном с ним
                    return false;
                }
            }

            db->db().setDatabaseName(db_file.fileName());

            switch(list->fill_all_crud_list(crud, type,password,filename))
            {
                case Password_abort:
                QMessageBox::critical(this,
                                      QObject::tr("Внимание"),
                                      QObject::tr("НЕВЕРНЫЙ ПАРОЛЬ"));
                ///Закрываем файл и БД
                db_file.close();
                db->db().close();
                db->set_Sql_type(PSQLtype); /// Перевожу обратно на PSQL, тк работаю в основном с ним
                return false;

            case Connection_trouble:
                QMessageBox::critical(this,
                                      QObject::tr("Внимание"),
                                      QObject::tr("Не удалось получить доступ к данным. Попробуйте использовать пароль"));
                ///Закрываем файл и БД
                db_file.close();
                db->db().close();
                db->set_Sql_type(PSQLtype); /// Перевожу обратно на PSQL, тк работаю в основном с ним
                return false;
            case Success:
                break;
            }

            list->fill_off_tels(offtel,type);

            ///Закрываем файл и БД
            db_file.close();
            db->db().close();
            db->set_Sql_type(PSQLtype); /// Перевожу обратно на PSQL, тк работаю в основном с ним

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
        else
        {
             if (!db_file.open(QIODevice::ReadOnly | QIODevice::Unbuffered) )
             {
                 QMessageBox::critical(this,
                                       QObject::tr("Внимание"),
                                       QObject::tr("Невозможно открыть файл"));;
                 return 0;
             }
             QByteArray arr=db_file.readAll();
             if(arr.count()==0)
             {
                 QMessageBox::critical(this,
                                       QObject::tr("Внимание"),
                                       QObject::tr("Файл либо не подходит под формат old_db, либо пуст!"));
                 return false;
             }

             if( !testHeadFile(&arr) ||
                     arr[0xC2]==0 )
             {
                 QMessageBox::critical(this,
                                       QObject::tr("Внимание"),
                                       QObject::tr("Файл не подходит под формат old_db"));
                return false;
             }else
             {
                 OldDbService *service;
                 OldDbZk *zk = new OldDbZk();
                 int pos = 0xC2;
                 if (service->Open_Old_db(zk,pos,arr))
                 {
                       qDebug() << "Крутяяк";
                       crud = new QList<Crud*>;
                       switch_zk_to_crud(crud, zk);
                       old_db = true;

                       db_file.close();

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
             }
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

bool Import_Form::begin_import_of_t()
{
  mark:
    Import_state state = compare_of_t();//возвращает true если есть совпадение
    switch (state) {
    case Import_abort :
        return false;
    case Import_succesful :
        QMessageBox::information(this,QObject::tr("Успех"),QObject::tr("Сравнение прошло успешно!")); ///Хвалимся
            if(add_to_db())//Начинаем импорт
            {//В любом случае - закрываем
                return false;
            }
            else
            {
                return false;
            }
        case Import_in_progress :
        goto mark;
        break;
    case Import_conflict :
        return true;
    }
}

bool Import_Form::testHeadFile(QByteArray *arr)
{
    return arr->mid(3,6)=="prolog";
}

void Import_Form::switch_zk_to_crud(QList<Crud *> *crud_list, OldDbZk *zk)
{
    QSqlQuery query1(db_connection::instance()->db());
    Crud *person = new Crud;
    person->lastname = zk->person.famil;
    person->name =zk->person.imya;
    person->mid_name = zk->person.otchestvo;
    person->reg_city = zk->person.city;
    person->reg_street = zk->person.street;
    person->reg_home = zk->person.dom;
    person->reg_corp = zk->person.korpus;
    person->reg_flat = zk->person.kvar;

    ///Сбор 5-ти телефонов
    if(zk->person.tel1 != "")
    {
        Owners_tel *owt = new Owners_tel;
        owt->tel_num = zk->person.tel1;
        owt->state = IsNewing;

        if (zk->person.tel1.size() == 7)
        {
            owt->oldnum = true;
            owt->internum = false;
        }
        else if (zk->person.tel1.size()  == 11)
        {
            owt->internum = false;
            owt->oldnum = false;
        }
        else
        {
            owt->oldnum = false;
            owt->internum = true;
        }

        person->owt()->append(owt);
    }
    /////////////////////////////////////////////
    if(zk->person.tel2 != "")
    {
        Owners_tel *owt = new Owners_tel;
        owt->tel_num = zk->person.tel2;
        owt->state = IsNewing;


        if (zk->person.tel2.size() == 7)
        {
            owt->oldnum = true;
            owt->internum = false;
        }
        else if (zk->person.tel2.size()  == 11)
        {
            owt->internum = false;
            owt->oldnum = false;
        }
        else
        {
            owt->oldnum = false;
            owt->internum = true;
        }

        person->owt()->append(owt);
    }
    /////////////////////////////////////////////
    if(zk->person.tel3 != "")
    {
        Owners_tel *owt = new Owners_tel;

        owt->tel_num = zk->person.tel3;
        owt->state = IsNewing;


        if (zk->person.tel3.size() == 7)
        {
            owt->oldnum = true;
            owt->internum = false;
        }
        else if (zk->person.tel3.size()  == 11)
        {
            owt->internum = false;
            owt->oldnum = false;
        }
        else
        {
            owt->oldnum = false;
            owt->internum = true;
        }
        person->owt()->append(owt);
    }
    /////////////////////////////////////////////
    if(zk->person.tel4 != "")
    {
        Owners_tel *owt = new Owners_tel;
        owt->tel_num = zk->person.tel4;
        owt->state = IsNewing;

        if (zk->person.tel4.size() == 7)
        {
            owt->oldnum = true;
            owt->internum = false;
        }
        else if (zk->person.tel4.size()  == 11)
        {
            owt->internum = false;
            owt->oldnum = false;
        }
        else
        {
            owt->oldnum = false;
            owt->internum = true;
        }
        person->owt()->append(owt);
    }
    /////////////////////////////////////////////
    if(zk->person.tel5 != "")
    {
        Owners_tel *owt = new Owners_tel;
        owt->tel_num = zk->person.tel5;
        owt->state = IsNewing;


        if (zk->person.tel5.size() == 7)
        {
            owt->oldnum = true;
            owt->internum = false;
        }
        else if (zk->person.tel5.size()  == 11)
        {
            owt->internum = false;
            owt->oldnum = false;
        }
        else
        {
            owt->oldnum = false;
            owt->internum = true;
        }
        person->owt()->append(owt);
    }
    /////////////////////////////////////////////

    query1.prepare("SELECT uuid_generate_v1()");
    if(query1.exec())
        while (query1.next())
          person->row_id = query1.value(0).toString();

    QString row_id1 = person->row_id;

    person->dop_info = "OLD ";
    person->dop_info += zk->person.dopinfo;
    person->dop_info += "; Кат.:"+zk->category+"; Дат: "+zk->data_zad+
            "; Мест:"+zk->keep_place+"; Кем: "+
            zk->kem_zad+"; Прич:"+zk->povod_zad+";";

    person->state = IsNewing;
    person->dop_info.append(", Кличка:" + zk->person.klichka);

    ///Временно
    person->zk_id = 1;
    crud_list->append(person);


    if(!zk->contacts.isEmpty())
    {
        for(int a = 0; a < zk->contacts.size(); a++)
        {
            Crud *cr = new Crud;
            cr->zk_id = a+2;
            cr->lastname = zk->contacts.at(a)->famil;
            cr->name = zk->contacts.at(a)->imya;
            cr->mid_name = zk->contacts.at(a)->otchestvo;
            cr->dop_info  = "Кличка: "+zk->contacts.at(a)->klichka;
            cr->state = IsNewing;

            if(zk->contacts.at(a)->tel1 != "")
            {
                Owners_tel *owt = new Owners_tel;


                if (zk->contacts.at(a)->tel1.size() == 7)
                {
                    owt->oldnum = true;
                    owt->internum = false;
                }
                else if (zk->person.tel1.size()  == 11)
                {
                    owt->internum = false;
                    owt->oldnum = false;
                }
                else
                {
                    owt->oldnum = false;
                    owt->internum = true;
                }

                owt->tel_num = zk->contacts.at(a)->tel1;
                cr->owt()->append(owt);
            }
            if(zk->contacts.at(a)->tel2 != "")
            {Owners_tel *owt = new Owners_tel;

                if (zk->contacts.at(a)->tel2.size() == 7)
                {
                    owt->oldnum = true;
                    owt->internum = false;
                }
                else if (zk->person.tel2.size()  == 11)
                {
                    owt->internum = false;
                    owt->oldnum = false;
                }
                else
                {
                    owt->oldnum = false;
                    owt->internum = true;
                }
                owt->tel_num = zk->contacts.at(a)->tel2;
                cr->owt()->append(owt);
            }
            if(zk->contacts.at(a)->tel3 != "")
            {
                Owners_tel *owt = new Owners_tel;

                if (zk->contacts.at(a)->tel3.size() == 7)
                {
                    owt->oldnum = true;
                    owt->internum = false;
                }
                else if (zk->person.tel3.size()  == 11)
                {
                    owt->internum = false;
                    owt->oldnum = false;
                }
                else
                {
                    owt->oldnum = false;
                    owt->internum = true;
                }

                owt->tel_num = zk->contacts.at(a)->tel3;
                cr->owt()->append(owt);
            }
            if(zk->contacts.at(a)->tel4 != "")
            {
                Owners_tel *owt = new Owners_tel;

                if (zk->contacts.at(a)->tel4.size() == 7)
                {
                    owt->oldnum = true;
                    owt->internum = false;
                }
                else if (zk->person.tel4.size()  == 11)
                {
                    owt->internum = false;
                    owt->oldnum = false;
                }
                else
                {
                    owt->oldnum = false;
                    owt->internum = true;
                }
                owt->tel_num = zk->contacts.at(a)->tel4;
                cr->owt()->append(owt);
            }
            if(zk->contacts.at(a)->tel5 != "")
            {
                Owners_tel *owt = new Owners_tel;

                if (zk->contacts.at(a)->tel5.size() == 7)
                {
                    owt->oldnum = true;
                    owt->internum = false;
                }
                else if (zk->person.tel5.size()  == 11)
                {
                    owt->internum = false;
                    owt->oldnum = false;
                }
                else
                {
                    owt->oldnum = false;
                    owt->internum = true;
                }
                owt->tel_num = zk->contacts.at(a)->tel5;
                cr->owt()->append(owt);
            }

            query1.prepare("SELECT uuid_generate_v1()");
            if(query1.exec())
                while (query1.next())
                  cr->row_id = query1.value(0).toString();

            QString row_id2 = cr->row_id;
            QVector<QString> local_vector;
            local_vector.append(row_id1);
            local_vector.append(row_id2);

            if(vector == nullptr)
                 vector = new QVector<QVector<QString>   >;

            vector->append(local_vector);
            //NEW
            QList<Contacts*> *list = new QList<Contacts*>;
            for(int a = 0; a<cr->owt()->size(); a++)
            {
                Contacts *cnt = new Contacts;
                cnt->contact_tel_num = cr->owt()->at(a)->tel_num;
                cnt->oldnum = cr->owt()->at(a)->oldnum;
                cnt->internum = cr->owt()->at(a)->internum;
                cnt->mark = cr->lastname+" "+cr->name+" "+cr->mid_name+" "+cr->dop_info;
                list->append(cnt);
            }



            if(!person->owt()->isEmpty())
                person->owt()->at(0)->cont()->append(*list);
           crud_list->append(cr);
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
    upd->setWindowTitle("Редактирование ЗК");
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
     upd->setWindowTitle("Редактирование ЗК");
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
    msgbx.setWindowTitle("Выберите действие");
    msgbx.setStandardButtons(QMessageBox::Ok | QMessageBox::Open | QMessageBox::Cancel);
    msgbx.setButtonText(QMessageBox::Ok,"Импортируемую ЗК №"+ QString::number(crud_model->actcrudlist.at(0)->zk_id));
    msgbx.setButtonText(QMessageBox::Open,"Основную ЗК №"+ QString::number(crud_model_pg->actcrudlist.at(0)->zk_id));
    msgbx.setButtonText(QMessageBox::Cancel,"Отмена");
    int ret = msgbx.exec();

    switch (ret)
    {
    case QMessageBox::Ok:
    prepare_main_to_add(crud_model->actcrudlist.at(0), crud_model_pg->actcrudlist.at(0));
        break;

    case QMessageBox::Open:
        prepare_main_to_add(crud_model_pg->actcrudlist.at(0), crud_model->actcrudlist.at(0));
        break;

    case QMessageBox::Cancel:
        break;
    }
}

void Import_Form::begin_work_with_links()
{
    QMessageBox msgbx;
    msgbx.setText("<div align=\"center\"><font size = '10'> Работа со списками </font></div>");
    msgbx.setWindowTitle("Выберите действие");
    msgbx.setStandardButtons(QMessageBox::Ok |  QMessageBox::Yes  | QMessageBox::Cancel);
    msgbx.setButtonText(QMessageBox::Ok,"Добавить список со связью к старому");
    msgbx.setButtonText(QMessageBox::Yes,"Отменить добавление цепочки");
    msgbx.setButtonText(QMessageBox::Cancel,"Отмена");
    int ret = msgbx.exec();

    switch (ret)
    {
    case QMessageBox::Ok:
        import_list_with_link();
        break;

    case QMessageBox::Yes:
        abort_link_import();
        break;

    case QMessageBox::Cancel:
        break;
    }
}

