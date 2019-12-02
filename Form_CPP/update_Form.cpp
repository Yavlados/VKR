#include "update_Form.h"
#include "ui_update.h"
#include "_Crud.h"
#include "_Owners_tel.h"
#include "_Contacts.h"

//#include "table_show_delegate.h"
#include "list_master.h"
#include "dialog_conflict.h"

#include <QSqlRecord>
#include <QStringRef>

QList<Crud *> *Update::linked_crud()
{
    if (_linked_crud == nullptr)
    {
        _linked_crud = new QList<Crud *>;
    }
    return _linked_crud;
}

Update::Update(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Update)
{
    ui->setupUi(this);
    set_validators();
    new_cr = nullptr;
    set_splitter_lines();
    set_delegates_and_connections();
    contacts_model = new MTM_Contacts;
    ot_model = new MTM_OwTel;
}

Update::~Update()
{
    delete ui;
//    if(contacts_model != nullptr)
//    {
//       Contacts::delete_all(contacts_model->clist);
//       delete contacts_model;
//       //contacts_model = nullptr;
//    }
//    if(ot_model != nullptr)
//    {
//        //Owners_tel::delete_all(ot_model->otlist);
//        delete ot_model;
//        //ot_model = nullptr;
//    }
//    if(list != nullptr)
//    {
//        delete list;
//        list = nullptr;
//    }

//    if (main_cr == new_cr)
//    {
//        if(main_cr != nullptr)
//        {
//            delete main_cr;
//            main_cr = nullptr;
//        }
//    }
//    else
//    {
//        if(main_cr != nullptr)
//        {
//            delete main_cr;
//            main_cr = nullptr;
//        }

//        if(new_cr != nullptr)
//        {
//            delete new_cr;
//            new_cr = nullptr;
//        }
//    }

//    if(added_cr != nullptr)
//    {
//        delete added_cr;
//        added_cr = nullptr;
//    }
//    if(linked_crud_id != nullptr)
//    {
//        delete linked_crud_id;
//        linked_crud_id = nullptr;
//    }
//    if(list_for_destroy != nullptr)
//    {
//        delete list_for_destroy;
//        list_for_destroy = nullptr;
//    }

//    if(_linked_crud != nullptr)
//    {
//        delete _linked_crud;
//        _linked_crud = nullptr;
//    }
}
//-----------------------------------------------------------------------------------//
void Update::Recieve_data(Crud *cr)
{
    new_cr = cr;
    clear_Vl();
    Fill_fields_update(new_cr);//заполнение полей
    //Уже есть главная ЗК



    //сначала очистка имеющегося списка
    if (!ot_model->mark_rows.isEmpty())
        ot_model->mark_rows.clear();

    if( (main_cr != nullptr && main_cr != new_cr) || frm_t == Confluence_form   )
    {
        compare_linked_cruds();
    }
    if (frm_t == Update_form)
    {
        QLabel *lb = new QLabel("<font size = 10> <h1> <div align=\"center\"> Редактирование </div> </h1> </font>");
        ui->vl_for_label->addWidget(lb);
    }else if(frm_t == Confluence_form)
    {
        QLabel *lb = new QLabel;
        QString str;
        str = "<font size = 10> <h1> <div align=\"center\"> Слияние ЗК:  №"+QString::number(new_cr->zk_id)+
                "(основная) и №"+QString::number(added_cr->zk_id)+" </div> </h1> </font>";

        lb->setText(str);
        ui->vl_for_label->addWidget(lb);
    }

     QPushButton *p_b = new QPushButton;
     p_b->setText("Редактировать");

     ui->vl_for_button->addWidget(p_b);

     connect(p_b, SIGNAL(clicked()), this ,SLOT(on_pb_Update_clicked()));
}
//-----------------------------------------------------------------------------------//
void Update::Fill_fields_update(Crud *new_cr)
{
    ui->le_last_name->setText(new_cr->lastname);
    ui->le_name->setText(new_cr->name);
    ui->le_mid_name->setText(new_cr->mid_name);

    if(!new_cr->birth_date.isEmpty())
    {
        auto list = new_cr->birth_date.split("-");

        ui->le_birth_date_day->setText(list.at(2));
        ui->le_birth_date_month->setText(list.at(1));
        ui->le_birth_date_year->setText(list.at(0));
    }

    ui->le_check_for->setText(new_cr->check_for);
    ui->le_dop_info->setPlainText(new_cr->dop_info);

    ui->le_reg_city->setText(new_cr->reg_city);
    ui->le_reg_street->setText(new_cr->reg_street);
    ui->le_reg_house->setText(new_cr->reg_home);
    ui->le_reg_corp->setText(new_cr->reg_corp);
    ui->le_reg_flat->setText(new_cr->reg_flat);

    ui->le_liv_city->setText(new_cr->liv_city);
    ui->le_liv_street->setText(new_cr->liv_street);
    ui->le_liv_house->setText(new_cr->liv_home);
    ui->le_liv_corp->setText(new_cr->liv_corp);
    ui->le_liv_flat->setText(new_cr->liv_flat);

    QLabel *lb = new QLabel("<font size = 5>  <div align=\"left\"> Добавлен: "+new_cr->date_add+" "+new_cr->time_add+" </div>  </font>");
        ui->vl_for_date_add->addWidget(lb);
        if(!new_cr->date_upd.isEmpty())
        {
            QLabel *lb2 = new QLabel("<font size = 5> <div align=\"right\"> Изменен: "+new_cr->date_upd+" </div> </font>");
            ui->vl_for_date_upd->addWidget(lb2);
        }


    ///Если список пустой (редактирование телефона из базы),
    /// то достаю телефоны из БАЗЫ ЗАПРОСОМ и заполняю список
    if(new_cr->owt()->isEmpty())
        Owners_tel::selectZkTelForAdd(new_cr->owt(),new_cr->zk_id);
      ot_model->setOTList(new_cr->owt());

    ui->tableView->setModel(ot_model);

    ui->tableView->resizeColumnToContents(0);
    ui->tableView->resizeColumnToContents(1);
    ui->tableView->resizeColumnToContents(2);
    ui->tableView->setWordWrap(false);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    //ui->tableView->setColumnWidth(0,250);
    contacts_model->reset_ContactModel();

//    if(!main_cr->take_links()->isEmpty() && _linked_crud == nullptr && frm_t != Confluence_form)
//    {
//        main_cr = new_cr; //первая ЗК - главная(необходимо для сравнения)
//        linked_crud()->push_front(new_cr);//также заношу в список и текущий id
//         //take_linked_zk();
//    }
    if(imprt_t != Add_import_data);
     fill_vl();
}
//-----------------------------------------------------------------------------------//
void Update::on_pb_Update_clicked()
{
    ///Исключаю пропуски
    for (int a=0; a < ot_model->actotlist.size(); a++)
    {
        if((ot_model->actotlist.at(a)->tel_num == "") && (ot_model->actotlist.at(a)->state == IsNewing))
        {
            ot_model->actotlist.removeAt(a);
            ot_model->otlist->removeAt(a);
            a--;
        }
    }

    for (int a=0; a < contacts_model->actlist.size(); a++)
    {
        if((contacts_model->actlist.at(a)->contact_tel_num == "") && (contacts_model->actlist.at(a)->cont_state == IsNewing))
        {
            contacts_model->actlist.removeAt(a);
            contacts_model->clist->removeAt(a);
            a--;
        }
    }

    ///Сначала собираю дату рождения, ФИО + телефоны собираются динамически для проверки на совпадения
    new_cr->lastname = ui->le_last_name->text();
    new_cr->name = ui->le_name->text();
    new_cr->mid_name= ui->le_mid_name->text();

    get_birthdate();

   // msgbx.setGeometry(0,0, 900,210);
    msgbx.setText("<font size = '8'>Подтверждение</font> <br> <font size = '5'>Вы готовы завершить редактирование записной книги?</font>");
    msgbx.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    int ret = msgbx.exec();

    switch (ret) {
    case QMessageBox::Cancel:
        break;
    case QMessageBox::Ok:
    ///ПРОВЕРКА НОВЫХ НОМЕРОВ
        switch (imprt_t) {
        case Update_import_data:
            break;
        case Add_import_data:
            break;

        case Update_pg_data:            //Проверку номеров с базой
//          if(new_cr->zk_id == main_cr->zk_id)
//          {
              if (!compare_tel_num())     //Делаем только в случае редактирования
              {
                  return;
              }
                  break;
//          }
                break;

        case Update_pg_data_import:
            break;
             }




    new_cr->check_for = ui->le_check_for->text();
    new_cr->dop_info = ui->le_dop_info->toPlainText();
    new_cr->reg_city = ui->le_reg_city->text();
    new_cr->reg_street = ui->le_reg_street->text();
    new_cr->reg_home = ui->le_reg_house->text();
    new_cr->reg_corp = ui->le_reg_corp ->text();
    new_cr->reg_flat = ui->le_reg_flat->text();

    if (ui->cb_adres->checkState() == Qt::Checked)
    {
        new_cr->liv_city = new_cr->reg_city;
        new_cr->liv_street = new_cr->reg_street;
        new_cr->liv_home = new_cr->reg_home;
        new_cr->liv_corp = new_cr->reg_corp;
        new_cr->liv_flat = new_cr->reg_flat;
    }
    else
    {
        new_cr->liv_city = ui->le_liv_city->text();
        new_cr->liv_street = ui->le_liv_street->text();
        new_cr->liv_home = ui->le_liv_house->text();
        new_cr->liv_corp = ui->le_liv_corp->text();
        new_cr->liv_flat = ui->le_liv_flat->text();
    }


    switch (imprt_t) {
    case Update_import_data:
        emit updated_import_crud(new_cr);
         close();
        return;

    case Add_import_data:
        emit add_import_crud(new_cr);
         close();
        return;

    case Update_pg_data:
        if(frm_t == Confluence_form)
        {
            if(new_cr->zk_id == 0)
            {
                Add_zk_into_base();
                return;
            }else
                Crud::del_zk(added_cr->zk_id);
        }
        if( new_cr->update_zk(linked_crud_id) )
        {
            if(Crud::save_all_crud(new_cr)) /// Если сохранили телефоны
            {
                if(list_for_destroy != nullptr)
                {
                    Crud *m_cr = list_for_destroy->at(0);

                    for(int a = 1; a<list_for_destroy->size(); a++)
                    {
                        for(int b = 0 ; b< linked_crud_id->size(); b++)
                        {
                            if( list_for_destroy->at(a)->zk_id == linked_crud_id->at(b))
                            {
                                linked_crud_id->removeAt(b);
                                break;
                            }
                        }
                        destroy_link(m_cr,list_for_destroy->at(a));
                    }
                }

                QMessageBox::information(this,QObject::tr("Успех"),QObject::tr("Данные сохранены в БД!")); ///Хвалимся
               clear_ALL();
            }
            else
            {        /// Если не удалось добавить телефоны
         QMessageBox::critical(this,QObject::tr("Ошибка"),QObject::tr("Не удалось выполнить обновление данных!"));
                 clear_ALL();
            }
        }
        else {
            QMessageBox::critical(this,QObject::tr("Ошибка"),QObject::tr("Не удалось выполнить обновление данных!"));
                  clear_ALL();
             }
             return;

    case Update_pg_data_import:
        if( new_cr->update_zk() )
        {
            if(Crud::save_all_crud(new_cr)) /// Если сохранили телефоны
            {
                emit update_import_pg();
            }
            else
            {        /// Если не удалось добавить телефоны
         QMessageBox::critical(this,QObject::tr("Ошибка"),QObject::tr("Не удалось выполнить обновление данных!"));
            }
        }
        else {
            QMessageBox::critical(this,QObject::tr("Ошибка"),QObject::tr("Не удалось выполнить обновление данных!"));
             }
            close();
         }

    }
}
//-----------------------------------------------------------------------------------//
void Update::on_pb_Back_to_Main_clicked()
{
    clear_ALL();
}
//-----------------------------------------------------------------------------------//
void Update::on_tableView_clicked(const QModelIndex &index)
{
    //В добавлении мы работаем исключительно с моделью
                  //Также как и в импорте
    QList<Contacts*> *cont_temp = new QList<Contacts*>;
    cont_temp = ot_model->actotlist.at(index.row())->cont();

    contacts_model->setContactList(new_cr->owt()->at(index.row())->cont());
    qDebug() << new_cr->zk_id << new_cr->owt()->at(index.row())->tel_id << new_cr->owt()->at(index.row())->state;

    //contacts_model->state = Edit_cont;

    ui->tableView_2->setModel(contacts_model);
    ui->tableView_2->resizeColumnToContents(0);
    ui->tableView_2->resizeColumnToContents(1);
    ui->tableView_2->resizeColumnToContents(2);
    ui->tableView_2->setWordWrap(false);
    ui->tableView_2->horizontalHeader()->setStretchLastSection(true);

    qDebug() << new_cr->zk_id << new_cr->owt()->at(index.row())->tel_id << new_cr->owt()->at(index.row())->state;
}
//-----------------------------------------------------------------------------------//
void Update::on_pb_del_line_telephone_clicked()
{
    QModelIndex ind = ui->tableView->currentIndex();
    if( ind.isValid())
        {

            contacts_model->delBindedContacts(new_cr->owt()->at(ind.row())->tel_id);
            ot_model->delRow_owner_tel(ind);
            if(imprt_t == Update_import_data) //взаимодействие с моделью
            {
                Owners_tel *temp_owt  = new_cr->owt()->at(ind.row());
                 delete temp_owt;
                 temp_owt = nullptr;
                 new_cr->owt()->removeAt(ind.row());
            }
             if (new_cr->owt()->count() == 0)
             {
                 Owners_tel *owt = new Owners_tel(ot_model->otlist->count(),0,false,IsNewing);
                 owt->oldnum = false;
                 ot_model->addRow_owner_tel(owt);
                 ot_model->reset_OTModel();

             }
    }
    contacts_model->reset_ContactModel();
}
//-----------------------------------------------------------------------------------//
void Update::on_pb_del_contact_line_clicked()
{
    QModelIndex ind = ui->tableView_2->currentIndex();
    if( ind.isValid())
    {
        contacts_model->delRow_contact(ind);
    }
}
//-----------------------------------------------------------------------------------//
void Update::on_pb_add_contact_line_clicked()
{
    QModelIndex index = ui->tableView->currentIndex();
       if(index.isValid())
       {
           Component *comp = new Component();
           comp->set_type(new_cont_tel);

           Contacts *cnt = new Contacts();
           cnt->parent_OT_id = new_cr->owt()->at(index.row())->tel_id;

           cnt->oldnum = false;
           comp->set_index_data(nullptr,cnt);
           int st = comp->exec();
           switch(st)
           {
           case 1:
               cnt->oldnum = comp->content->Oldnum;
               cnt->internum = comp->content->Internum;
               cnt->contact_tel_num= comp->content->tel_num;
               cnt->mark = comp->content->mark;
               contacts_model->addRow_contact(cnt);
               //owt->state = IsChanged;
               //ot_model->setOTList(new_cr->owt());
               ui->tableView_2->setModel(contacts_model);
               ui->tableView_2->resizeColumnToContents(2);
               break;
           }
           delete comp;
       }else
        return;

//    QModelIndex index = ui->tableView->currentIndex();
//    if(index.isValid())
//        emit Add_contact_row(new_cr->owt()->at(index.row())->tel_id);
}
//-----------------------------------------------------------------------------------//
void Update::clear_ALL()
{
   emit Ready_for_update(new_cr->zk_id);
  if(frm_t == Confluence_form)
    close();
}
//-----------------------------------------------------------------------------------//
void Update::clear_Vl()
{
    while (ui->vl_for_label->count() > 0)
        delete ui->vl_for_label->takeAt(0);

    while (ui->vl_for_button->count() > 0)
        delete ui->vl_for_button->takeAt(0);

     //сброс текста и подсветки на лайнах
    foreach(QLineEdit *le, this->findChildren<QLineEdit*>() )
    {
        if(!le->text().isEmpty())
            le->clear();
        le->setStyleSheet("");
    }
    if(ui->vl_for_date_add->count() != 0)
    {
        QLayoutItem *item = ui->vl_for_date_add->takeAt(0);
        delete item->widget();
    }

    if (ui->vl_for_date_upd->count() != 0)
     {
      QLayoutItem *item = ui->vl_for_date_upd->takeAt(0);
      delete item->widget();
    }
}
//-----------------------------------------------------------------------------------//
void Update::set_validators()
{
    ui->le_birth_date_day->setValidator(new QIntValidator(1,31));
    ui->le_birth_date_month->setValidator(new QIntValidator(1,12));
    ui->le_birth_date_year->setValidator(new QIntValidator(1960,2100));
}
//-----------------------------------------------------------------------------------//
void Update::on_tableView_2_clicked(const QModelIndex &index)
{
    QModelIndex indexOT = ui->tableView->currentIndex();
    qDebug() << new_cr->owt()->at(indexOT.row())->cont()->at(index.row())->contact_id
             << new_cr->owt()->at(indexOT.row())->cont()->at(index.row())->parent_OT_id ;
}
//-----------------------------------------------------------------------------------//
void Update::set_splitter_lines()
{
    QSplitterHandle *handle = ui->splitter->handle(1);
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

    QSplitterHandle *handle_3 = ui->splitter_3->handle(1);
    QVBoxLayout *layout_3 = new QVBoxLayout(handle_3);
    layout_3->setSpacing(0);
    layout_3->setMargin(0);

    QFrame *line_3 = new QFrame(handle_3);
    line_3->setFrameShape(QFrame::HLine);
    line_3->setFrameShadow(QFrame::Sunken);
    layout_3->addWidget(line_3);
}
//-----------------------------------------------------------------------------------//
bool Update::compare_tel_num()
{
    /// Проверка на уникальность. Выполняется для добавления новых записей к линкед

//    if(frm_t == Update_form && main_cr != nullptr)
//        return true;


    QString query_for_nums, query_for_fio; //добавить иф на пустой список телефонов
    for (int i=0; i < new_cr->owt()->size(); i++)
    {
        if (!new_cr->owt()->at(i)->tel_num.isEmpty() && new_cr->owt()->at(i)->state != IsRemoved)
        //Составление запроса для проверки телефонов
        {
            if(new_cr->owt()->at(i)->tel_num.at(0) == "+")
            {
                if(new_cr->owt()->at(i)->tel_num.count() < 16)
                {
                    new_cr->owt()->removeAt(i);
                    i--;
                    break;

                }else
                {
                    QString temp = new_cr->owt()->at(i)->tel_num.at(1)+new_cr->owt()->at(i)->tel_num.mid(3,3)+
                            new_cr->owt()->at(i)->tel_num.mid(7,3)+
                            new_cr->owt()->at(i)->tel_num.mid(11,2)+new_cr->owt()->at(i)->tel_num.mid(14,2);
                new_cr->owt()->at(i)->tel_num = temp;
                }

            }

            if (query_for_nums.isEmpty())
                query_for_nums += " owners_tel.Telephone_num = ('"+new_cr->owt()->at(i)->tel_num+"') ";
            else
                query_for_nums += " OR owners_tel.Telephone_num = ('"+new_cr->owt()->at(i)->tel_num+"')";
            }
        }
    //Составление запроса для проверки фио и др
        query_for_fio = " zk.lastname = ('"+new_cr->lastname+"') AND"
        " zk.name = ('"+new_cr->name+"') AND"
        " zk.mid_name = ('"+new_cr->mid_name+"') AND";

        if(!new_cr->birth_date.isEmpty())
            query_for_fio += " zk.birth_date = ('"+new_cr->birth_date+"') ";
        else
            query_for_fio += " zk.birth_date is NULL";

    qDebug() << query_for_nums << query_for_fio;

    Crud *cr = new Crud();

     if (!cr->compare_with_base(query_for_nums,query_for_fio, new_cr->zk_id,linked_crud_id))
     {
        for (int i = 0 ; i<cr->compare_result->size(); i++)
        {
            Dialog_conflict *dlg = new Dialog_conflict;

            if(linked_crud_id == nullptr)
                linked_crud_id = new QList<int>;
            QSqlQuery query2(db_connection::instance()->db());

            query2.prepare("SELECT *"
                           " FROM zk_links"
                           " WHERE (row_id2 = '"+new_cr->row_id+ "' AND row_id1 = (SELECT row_id FROM zk WHERE zk_id = "+QString::number(cr->compare_result->at(i).id)+"))"
                           " OR"
                           " (row_id1 = '"+new_cr->row_id+ "' AND row_id2 = (SELECT row_id FROM zk WHERE zk_id = "+QString::number(cr->compare_result->at(i).id)+"))");

            if(query2.exec())
            {
                if(!query2.next())
                {
                    if (cr->compare_result->at(i).Tel_num != "NULL")//Если обнаружилось совпадение по номеру телефона
                    {
                        dlg->setText("<font size = '5'> ВНИМАНИЕ: введенный телефонный номер " +cr->compare_result->at(i).Tel_num+" "
                             "обнаружен принадлежащим владельцу записной книжки № "+QString::number(cr->compare_result->at(i).id)+"</font>");
                        dlg->setButtonText("Перейти к записной книжке № "+ QString::number( cr->compare_result->at(i).id));
                    }
                    else
                    {
                        dlg->setText("<font size = '5'> ВНИМАНИЕ: введенные фамилия, имя, отчество и дата рождения "
                                           "обнаружены принадлежащими владельцу записной книжки № "+QString::number(cr->compare_result->at(i).id)+"</font>");
                       dlg->setButtonText("Перейти к записной книжке № "+ QString::number( cr->compare_result->at(i).id));

                    }
                    dlg->exec();
                    switch (dlg->state)
                    {
                   case 1:
                    linked_crud_id->append(cr->compare_result->at(i).id);
                    delete dlg;
                    break;

                  case 2:
                    msg_before_confluence(Crud::id_zk_search(cr->compare_result->at(i).id));
                    delete dlg;
                    return false;

                  case 5:
                  emit open_update_tab(Crud::id_zk_search(cr->compare_result->at(i).id));
                  delete dlg;
                  return false; /// во всех случаях return - мы выходим из функции

                   case 3:
                   delete dlg;
                   return false;

                  case 4:
                  clear_ALL();
                  delete dlg;
                  return false;
                    }
                }
            }
            else
                qDebug() << query2.executedQuery();


           /*
//            int ret = msgbx.exec();
//            switch (ret)
//            {
//            case QMessageBox::Yes:
//                linked_crud_id->append(cr->compare_result->at(i).id);
//                break;

//            case QMessageBox::Retry:
//            msg_before_confluence(Crud::id_zk_search(cr->compare_result->at(i).id));
//                return false;

//            case QMessageBox::Ok:
//                emit open_update_tab(Crud::id_zk_search(cr->compare_result->at(i).id));
//               return false; /// во всех случаях return - мы выходим из функции

//            case QMessageBox::Open:
//                return false;

//            case QMessageBox::Cancel:
//             clear_ALL();
//                  return false;
//             }*/
        }
/*         if(cr->zk_id != 0)
//         {
//             msgbx.setText("<font size = '5'> ВНИМАНИЕ: введенный телефонный номер " +cr->owt()->at(0)->tel_num+" "
//                                "обнаружен принадлежим владельцу записной книжки № "+QString::number(cr->zk_id)+"</font>");
//             msgbx.setButtonText(QMessageBox::Ok,"Перейти к записной книжке № "+ QString::number( cr->zk_id));
//         }
//         else if( cr->owt()->at(0)->parentZK_id != 0)//Если обнаружилось совпадение по фио
//         {
//             cr->zk_id = cr->owt()->at(0)->parentZK_id;//переписываю
//          msgbx.setText("<font size = '5'> ВНИМАНИЕ: введенные фамилия, имя, отчество и дата рождения "
//                             "обнаружены принадлежащими владельцу записной книжки № "+QString::number(cr->zk_id)+"</font>");
//                      msgbx.setButtonText(QMessageBox::Ok,"Перейти к записной книжке № "+ QString::number( cr->zk_id));
//         } else
//         {
//            new_cr->linked_nums = cr->linked_nums;
//            return true;
//         }
//             int ret = msgbx.exec();

//             switch (ret)
//             {
//             case QMessageBox::Yes:
//                 new_cr->linked_nums = cr->linked_nums;
//                 linked_crud_id->append(cr->zk_id);
//                 goto repeat;

//             case QMessageBox::Retry:
//             msg_before_confluence(cr);
//                 return false;

//             case QMessageBox::Ok:
//                 emit open_update_tab(cr);
//                return false; /// во всех случаях return - мы выходим из функции

//             case QMessageBox::Open:
//                 return false;

//             case QMessageBox::Cancel:
//              clear_ALL();
//                   return false;
             }*/
         }/*else
            if(cr->linked_nums.isEmpty())//список опустел или нет связей
                new_cr->linked_nums = cr->linked_nums;*/
     delete cr;
     return true;
}
//-----------------------------------------------------------------------------------//
QString Update::get_birthdate()
{
    if (!ui->le_birth_date_day->text().isEmpty() && !ui->le_birth_date_month->text().isEmpty() && !ui->le_birth_date_year->text().isEmpty())
    {
        QString day,month,year;

        day = ui->le_birth_date_day->text();
        month = ui->le_birth_date_month->text();
        year=ui->le_birth_date_year->text();

        if (day.count() == 1)
              day.insert(0,"0");

        if (month.count() == 1)
            month.insert(0,"0");

        if(year.toInt()  <  1900)
        {
            msgbx.setText("<font size = '5'> Вы ввели не корректную дату рождения </font>");
            msgbx.setStandardButtons(QMessageBox::Ok);
            msgbx.setButtonText(QMessageBox::Ok,"Вернуться обратно");

            int ret = msgbx.exec();

            switch (ret) {
            case QMessageBox::Ok:
                return nullptr;
            }
        }
      return  new_cr->birth_date = year+"-"+month+"-"+day;
    } else
        return nullptr;
}
//-----------------------------------------------------------------------------------//
void Update::Fill_table_in_add()
{
    new_cr = new Crud();

    new_cr->zk_id = 0; //Зануляю, тк новая ЗК
    if(Owners_tel::selectZkTelForAdd(new_cr->owt(), new_cr->zk_id))
           ot_model->setOTList(new_cr->owt());

       //ot_model->state = Edit_Ot; ///меняю флаги для изменения
       ui->tableView->setModel(ot_model);

       ui->tableView->resizeColumnToContents(0);
       ui->tableView->resizeColumnToContents(1);
       ui->tableView->resizeColumnToContents(2);
       ui->tableView->setWordWrap(false);
       ui->tableView->horizontalHeader()->setStretchLastSection(true);

       //ui->tableView->setColumnWidth(0,250);

            clear_Vl();

       QLabel *lb = new QLabel("<font size = 10> <h1> <div align=\"center\"> Добавление новой записной книжки  </div></h1> </font>");
       ui->vl_for_label->addWidget(lb);


       QPushButton *p_b = new QPushButton;
       p_b->setText("Добавить новую записную книжку");
       ui->vl_for_button->addWidget(p_b);

       connect(p_b, SIGNAL(clicked()), this ,SLOT(Add_zk()));
}
//-----------------------------------------------------------------------------------//
void Update::Add_zk()
{
    /// СНАЧАЛА  СОБИРАЕМ НУЖНЫЕ ПОЛЯ, А ПОТОМ
    /// ПРОВЕРЯЕМ ВВЕДЕННЫЕ НОМЕРА

    get_birthdate();

    new_cr->lastname = ui->le_last_name->text();
    new_cr->name=ui->le_name->text();
    new_cr->mid_name = ui->le_mid_name->text();


    msgbx.setText("<font size = '5'><h1> Подтверждение </h1> <br>Вы готовы завершить добавление записной книги?</font>");
    msgbx.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgbx.setButtonText(QMessageBox::Ok,"Подтвердить");
    msgbx.setButtonText(QMessageBox::Cancel,"Отмена");
    int ret = msgbx.exec();

    switch (ret) {
    case QMessageBox::Cancel:
        return;
    case QMessageBox::Ok:
        if (!compare_tel_num())
            return;

        new_cr->check_for = ui->le_check_for->text();
        new_cr->dop_info = ui->le_dop_info->toPlainText();
        new_cr->reg_city = ui->le_reg_city->text();
        new_cr->reg_street = ui->le_reg_street->text();
        new_cr->reg_home = ui->le_reg_house->text();
        new_cr->reg_corp = ui->le_reg_corp ->text();
        new_cr->reg_flat = ui->le_reg_flat->text();

        if (ui->cb_adres->checkState() == Qt::Checked)
        {
            new_cr->liv_city = new_cr->reg_city;
            new_cr->liv_street = new_cr->reg_street;
            new_cr->liv_home = new_cr->reg_home;
            new_cr->liv_corp = new_cr->reg_corp;
            new_cr->liv_flat = new_cr->reg_flat;
        }
        else
        {
            new_cr->liv_city = ui->le_liv_city->text();
            new_cr->liv_street = ui->le_liv_street->text();
            new_cr->liv_home = ui->le_liv_house->text();
            new_cr->liv_corp = ui->le_liv_corp->text();
            new_cr->liv_flat = ui->le_liv_flat->text();
        }

        new_cr->date_add = QDate::currentDate().toString(Qt::ISODate);
        new_cr->time_add = QTime::currentTime().toString();
        new_cr->check();

        Add_zk_into_base();
    }
}
//-----------------------------------------------------------------------------------//
void Update::Add_zk_into_base()
{
    QList<Crud*> *temp_crudlist = new QList<Crud*>;
    temp_crudlist->append(new_cr);  ///Лист для работы с list

    if(list == nullptr)
        list = new List_master(Main_window_for_Update);
        if( list->insert_crud_in_db(temp_crudlist, linked_crud_id))
        {
            new_cr->zk_id = 0;
            QString str = "ЗК успешно №"+QString::number(new_cr->zk_id)+" добавлена!";
            QMessageBox::information(this,QObject::tr("Успех"),QObject::tr("ЗК успешно добавлена!"));
            delete list;
            list = nullptr;
            clear_ALL();
        }
        else {
            QMessageBox::critical(this,QObject::tr("Ошибка"),QObject::tr("Не удалось выполнить обновление данных!"));
             clear_ALL();
             }
}
//-----------------------------------------------------------------------------------//
void Update::cb_clicked()
{
  foreach (QCheckBox *ch, this->findChildren<QCheckBox*>())
  {
      if (ch->isChecked())
      {
          ui->le_liv_city->setEnabled(false);
          ui->le_liv_street->setEnabled(false);
          ui->le_liv_house->setEnabled(false);
          ui->le_liv_corp->setEnabled(false);
          ui->le_liv_flat->setEnabled(false);
      }
      else {
          ui->le_liv_city->setEnabled(true);
          ui->le_liv_street->setEnabled(true);
          ui->le_liv_house->setEnabled(true);
          ui->le_liv_corp->setEnabled(true);
          ui->le_liv_flat->setEnabled(true);
      }
  }
}
//-----------------------------------------------------------------------------------//
void Update::recieve_import_data(Crud *cr)
{
    new_cr = cr;
    ui->pb_Back_to_Main->setVisible(false);
    Fill_fields_update(new_cr);

    QLabel *lb = new QLabel();

    QPushButton *p_b = new QPushButton;

    QPushButton *p_b_back = new QPushButton;
    switch (imprt_t)
    {
    case Add_import_data:
        lb->setText("<font size = 10> <h1> <div align=\"center\"> Редактирование импортируемых данных </div></h1> </font>");
         p_b->setText("Редактировать импортируемые данные");
         p_b_back->setText("Вернуться обратно");
        connect(p_b, SIGNAL(clicked()), this ,SLOT(on_pb_Update_clicked()));
        break;
    case Update_pg_data_import:
        lb->setText("<font size = 10> <h1> <div align=\"center\"> Редактирование данных из базы </div></h1> </font>");
         p_b->setText("Редактировать данные из базы");
         p_b_back->setText("Вернуться обратно");
         connect(p_b, SIGNAL(clicked()), this ,SLOT(on_pb_Update_clicked()));
        break;
    case Update_import_data:
        lb->setText("<font size = 10> <h1> <div align=\"center\"> Редактирование импортируемых данных  </div></h1> </font>");
        p_b->setText("Редактировать импортируемые данные");
        p_b_back->setText("Вернуться обратно");
        connect(p_b, SIGNAL(clicked()), this ,SLOT(on_pb_Update_clicked()));
        break;
    }

    ui->vl_for_button->addWidget(p_b);
    ui->vl_for_label->addWidget(lb);
    ui->vl_for_button->addWidget(p_b_back);

    connect(p_b_back, SIGNAL(clicked()), this ,SLOT(close()));
}
//-----------------------------------------------------------------------------------//
void Update::close()
{
    delete this;
}
//-----------------------------------------------------------------------------------//
void Update::update_import_data()
{//РАБОТА с списком, а не с базой!!
    new_cr->lastname = ui->le_last_name->text();
    new_cr->name = ui->le_name->text();
    new_cr->mid_name= ui->le_mid_name->text();
    new_cr->check_for = ui->le_check_for->text();
    new_cr->dop_info = ui->le_dop_info->toPlainText();
    new_cr->reg_city = ui->le_reg_city->text();
    new_cr->reg_street = ui->le_reg_street->text();
    new_cr->reg_home = ui->le_reg_house->text();
    new_cr->reg_corp = ui->le_reg_corp ->text();
    new_cr->reg_flat = ui->le_reg_flat->text();

    get_birthdate();

    new_cr->liv_city = ui->le_liv_city->text();
    new_cr->liv_street = ui->le_liv_street->text();
    new_cr->liv_home = ui->le_liv_house->text();
    new_cr->liv_corp = ui->le_liv_corp->text();
    new_cr->liv_flat = ui->le_liv_flat->text();

    //Самoе главное - телефоны
    int a = 0;
    while (a<new_cr->owt()->size())
    {
        if(new_cr->owt()->at(a)->state == IsRemoved)
            new_cr->owt()->removeAt(a);
        else
            a++;
    }
    switch (imprt_t) {
    case Update_import_data:
        emit updated_import_crud(new_cr);
        break;
    case Add_import_data:
        emit add_import_crud(new_cr);
        break;
    }
    close();
}
//-----------------------------------------------------------------------------------//
void  Update::set_delegates_and_connections()
{
    connect(ot_model,SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(slot_for_model(QModelIndex, QModelIndex)));
    connect(contacts_model,SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(slot_for_model(QModelIndex, QModelIndex)));
//    delegate_ot = new Table_line_delegate(this);
//    delegate_ot->set_type(OT);
//    delegate_ot->set_MTM_model(ot_model, contacts_model);
//    //ui->tableView->setItemDelegateForColumn(0,delegate_ot);
//    ui->tableView->setItemDelegate(delegate_ot);

//    delegate_cont = new Table_line_delegate(this);
//    delegate_cont->set_type(Cont);
//    delegate_cont->set_MTM_model(ot_model, contacts_model);
//    /
}
//-----------------------------------------------------------------------------------//
void Update::slot_for_model(QModelIndex i1, QModelIndex i2)
{
    (void) i2;
    MTM_OwTel *Ot_sender_ptr = qobject_cast<MTM_OwTel*>(sender());
    MTM_Contacts *Cont_sender_ptr = qobject_cast<MTM_Contacts*>(sender());

        if (Ot_sender_ptr == nullptr)
        {
            qDebug() << "Mimo";
            if (Cont_sender_ptr == nullptr)
            {
                return;
            }
            else
                ui->tableView_2->selectRow(i1.row());

        }
        else {
            qDebug() << "Popal";
            ui->tableView->selectRow(i1.row());
        }
}
//-----------------------------------------------------------------------------------//
void Update::take_linked_zk(Crud *cr)
{
   main_cr = cr;
   QList<int> *temp = Crud::take_links(main_cr->row_id, PSQLtype) ;

   if (temp == nullptr)
   {
       main_cr = nullptr;
       delete temp;
       return;
   } else
   {
       linked_crud_id = temp;
       if(list == nullptr)
           list = new List_master(Main_window_for_Update);

       list->set_counters();
       for (int a = 0; a < temp->size();a++)
       {
           list->fill_crud_list(linked_crud(), temp->at(a), PSQLtype);
       }
       linked_crud()->push_front(cr);//также заношу в список и текущий id
       fill_vl();
       delete list;
       list = nullptr;
   }
}
//-----------------------------------------------------------------------------------//
void Update::compare_linked_cruds()
{
        QList<CompareResult> *list = nullptr;

        if (frm_t == Confluence_form)
            list = new_cr->compare_cruds(added_cr);
        else
            list = main_cr->compare_cruds(new_cr);

        if( !list->isEmpty())
        {
            for (int i =0; i<list->size(); i++)
            {
               switch (list->at(i)){
               case lastname_CR:
                   mark_le(ui->le_last_name);
                   break;
               case name_CR:
                   mark_le(ui->le_name);
                   break;
               case mid_name_CR:
                   mark_le(ui->le_mid_name);
                   break;
               case birth_date_CR:
                   mark_le(ui->le_birth_date_day,ui->le_birth_date_month, ui->le_birth_date_year);
                   break;
               case check_for_CR:
                   mark_le(ui->le_check_for);
                   break;
               case dop_info_CR:
                   //mark_le(ui->le_dop_info);
                   break;
               case reg_city_CR:
                   mark_le(ui->le_reg_city);
                   break;
               case reg_street_CR:
                   mark_le(ui->le_reg_street);
                   break;
               case reg_home_CR:
                   mark_le(ui->le_reg_house);
                   break;
               case reg_corp_CR:
                   mark_le(ui->le_reg_corp);
                   break;
               case reg_flat_CR:
                   mark_le(ui->le_reg_flat);
                   break;
               case liv_city_CR:
                   mark_le(ui->le_liv_city);
                   break;
               case liv_street_CR:
                   mark_le(ui->le_liv_street);
                   break;
               case liv_home_CR:
                   mark_le(ui->le_liv_house);
                   break;
               case liv_corp_CR:
                   mark_le(ui->le_liv_corp);
                   break;
               case liv_flat_CR:
                   mark_le(ui->le_liv_flat);
                   break;
               }
            }
        }
        //проверка телефонов
       if (frm_t == Confluence_form)
            ot_model->mark_rows = Owners_tel::compare_nums(new_cr->owt(),added_cr->owt());
       else
            ot_model->mark_rows = Owners_tel::compare_nums(new_cr->owt(),main_cr->owt());
}
//-----------------------------------------------------------------------------------//
void Update::fill_vl()
{
    if(new_cr != main_cr && _linked_crud != nullptr && ui->vl_for_cnfl_button->count() == 0)
    {
        QPushButton *pb = new QPushButton("Разрыв связи");
        ui->vl_for_cnfl_button->addWidget(pb);
            connect(pb,SIGNAL(clicked()),this,SLOT(add_for_destroy()));
    }
    if(main_cr == new_cr && ui->vl_for_cnfl_button->count() != 0)
    {
        QLayoutItem *item = ui->vl_for_cnfl_button->takeAt(0);
        delete item->widget();
    }

    if (_linked_crud != nullptr)
    {
        int i;
        QLabel *lab = nullptr;

        clear_vl_for_links();

        for (i =0 ;i<linked_crud()->size();i++)
        {
            if(linked_crud()->at(i) == new_cr)
            {
                lab = new QLabel("№ "+QString::number(i+1)+" из "+QString::number(linked_crud()->size()) + " (ЗК № "+QString::number(new_cr->zk_id)+")");
                break;
            }
        }
        if ( i != 0)
        {
            QPushButton *p_b_forward = new QPushButton;
            p_b_forward->setText("<<");
            index = i;
            ui->vl_for_prev_linked->addWidget(p_b_forward);
            connect(p_b_forward,SIGNAL(clicked()),this,SLOT(prev_page()));
        }
        if ( i != linked_crud()->size()-1)
        {
            QPushButton *p_b_forward = new QPushButton;
            p_b_forward->setText(">>");
            ui->vl_for_next_linked->addWidget(p_b_forward);
            index = i;
            connect(p_b_forward,SIGNAL(clicked()),this,SLOT(next_page()));
        }
        ui->vl_for_label_linked->addWidget(lab);
    }
}
//-----------------------------------------------------------------------------------//
void Update::prev_page()
{
    //upload_main_cr();
    index -= 1;
    Recieve_data(linked_crud()->at(index));
}
//-----------------------------------------------------------------------------------//
void Update::next_page()
{
    //upload_main_cr();
    index += 1;
    Recieve_data(linked_crud()->at(index));
}
//-----------------------------------------------------------------------------------//
void Update::clear_vl_for_links()
{
    if(ui->vl_for_label_linked->count() != 0)
    {
        QLayoutItem *item = ui->vl_for_label_linked->takeAt(0);
        delete item->widget();
    }

    if(ui->vl_for_next_linked->count() != 0)
    {
        QLayoutItem *item = ui->vl_for_next_linked->takeAt(0);
        delete item->widget();
    }

    if (ui->vl_for_prev_linked->count() != 0)
    {
        QLayoutItem *item = ui->vl_for_prev_linked->takeAt(0);
        delete item->widget();
    }

}
//-----------------------------------------------------------------------------------//
void Update::msg_before_confluence(Crud *cr)
{
    //QList<Crud*> *crudlist = new QList<Crud*>;
    if(list == nullptr)
        list = new List_master(Main_window_for_Update);
    list->set_counters();
    //crudlist->append(cr);
    //list->fill_crud_list(crudlist,cr->zk_id,PSQLtype);

    QMessageBox msgbx;
    msgbx.setText("<font size = '5'> Вы собираетесь объеденить две записи в одну. <br> Какую выбрать в качестве основной?</font>");
    msgbx.setStandardButtons(QMessageBox::Ok | QMessageBox::Open | QMessageBox::Cancel);
    msgbx.setButtonText(QMessageBox::Ok,"Совпавшую ЗК № "+ QString::number(cr->zk_id));

    if(new_cr->zk_id != 0)
        msgbx.setButtonText(QMessageBox::Open,"Текущую ЗК № " + QString::number(new_cr->zk_id));
    else
        msgbx.setButtonText(QMessageBox::Open,"Новую ЗК");

    msgbx.setButtonText(QMessageBox::Cancel,"Отмена");
    int ret = msgbx.exec();

    switch (ret)
    {
    case QMessageBox::Ok:
        prepare_confluence_crud(cr, new_cr);
        delete list;
        list = nullptr;
        clear_ALL();
        return;

    case QMessageBox::Open:
        prepare_confluence_crud(new_cr, cr);
        delete list;
        list = nullptr;
        clear_ALL();
        return;

    case QMessageBox::Cancel:
        delete list;
        list = nullptr;
        return;
    }
}

void Update::prepare_confluence_crud(Crud *main_crud, Crud *added_crud)
{
    Crud *cnfl_cr = new Crud;        //Новая запись

    cnfl_cr->operator+(main_crud);                  //Скопировал все поля ЗК

    for(int a=0; a < main_crud->owt()->size();a++) //Взял список номеров
        cnfl_cr->owt()->append(main_crud->owt()->at(a));

    ///-------- Заменить на функцию с .contains() -----------///

    int b=0;

    while(b<added_crud->owt()->size())
    {
        int i=0;
        while (i<cnfl_cr->owt()->size())
        {
            if(cnfl_cr->owt()->at(i)->tel_num == added_crud->owt()->at(b)->tel_num )
            {   //Если совпадение, то смещаем основной итератор
                goto mark;
            }
            else //Если нет, то проверяем со следующим телефоном
                i++;
        } //Прошло сравнение
        added_crud->owt()->at(b)->state = IsNewing;
        cnfl_cr->owt()->append(added_crud->owt()->at(b));
        mark:
        b++;
    }

    Update *upd = new Update;
    upd->frm_t = Confluence_form;
    upd->imprt_t = Update_pg_data_import;

      emit open_confluence_upd(cnfl_cr,main_crud,added_crud);

    connect(this,SIGNAL(Send_data(Crud*)), upd, SLOT(recieve_import_data(Crud*)));
    connect(upd,SIGNAL(add_import_crud(Crud*)), this, SLOT(recieve_added_import_crud(Crud*)));
    upd->Recieve_data(cnfl_cr);
        upd->show();
}
//-----------------------------------------------------------------------------------//
void Update::destroy_link(Crud *m_cr, Crud *n_cr)
{
    QSqlQuery querry(db_connection::instance()->db());

  querry.prepare( "DELETE FROM zk_links"
   " WHERE (row_id1 ='"+n_cr->row_id+"' AND row_id2='"+m_cr->row_id+"') OR "
   " (row_id1 ='"+m_cr->row_id+"' AND row_id2='"+n_cr->row_id+"')");

    if(!querry.exec())
    {
       qDebug() << querry.lastError();
    } else
    {
        qDebug() << querry.executedQuery();

    }
}
//-----------------------------------------------------------------------------------//

void Update::add_for_destroy()
{
    if(list_for_destroy == nullptr)
    {
        list_for_destroy = new QList<Crud*>;
        list_for_destroy->append(main_cr); //нулевой элемент - главная зк
    }
    list_for_destroy->append(new_cr);

    linked_crud()->removeOne(new_cr);

    index = 0;
    Recieve_data(main_cr);
}
//-----------------------------------------------------------------------------------//
void Update::start_confluence(Crud *confl_cr, Crud *m_cr, Crud *a_cr)
{
    main_cr = m_cr ;
    added_cr = a_cr;
    Recieve_data(confl_cr);
}
//-----------------------------------------------------------------------------------//
void Update::closeEvent(QCloseEvent *event)
{
    (void)event;
    close();
}

//-----------------------------------------------------------------------------------//
void Update::mark_le(QLineEdit *le, QLineEdit *le1, QLineEdit *le2)
{
  le->setStyleSheet("QLineEdit { background: rgb(0, 255, 255); selection-background-color: rgb(233, 99, 0); }");
   if(le1 != nullptr)
       le1->setStyleSheet("QLineEdit { background: rgb(0, 255, 255); selection-background-color: rgb(233, 99, 0); }");
   if(le2 != nullptr)
       le2->setStyleSheet("QLineEdit { background: rgb(0, 255, 255); selection-background-color: rgb(233, 99, 0); }");
}
//-----------------------------------------------------------------------------------//

void Update::on_tableView_doubleClicked(const QModelIndex &index)
{
    Component *comp = new Component();
    comp->set_type(Ow_tel_num);
    Owners_tel *owt =ot_model->actotlist.at(index.row());
    comp->set_index_data(owt, nullptr);
    int st = comp->exec();
    switch(st)
    {
        case 1:
        owt->oldnum = comp->content->Oldnum;
        owt->internum = comp->content->Internum;
        owt->tel_num = comp->content->tel_num;

        if(owt->state != IsNewing)
            owt->state = IsChanged;

        //ot_model->setOTList(new_cr->owt());
        ui->tableView->setModel(ot_model);
        break;
    }
    delete comp;
}

void Update::on_tableView_entered(const QModelIndex &index)
{
    qDebug() << "tut";
}

void Update::on_cb_adres_clicked()
{
    if (ui->cb_adres->isChecked())
    {
        ui->le_liv_city->setEnabled(false);
        ui->le_liv_street->setEnabled(false);
        ui->le_liv_house->setEnabled(false);
        ui->le_liv_corp->setEnabled(false);
        ui->le_liv_flat->setEnabled(false);
    }
    else {
        ui->le_liv_city->setEnabled(true);
        ui->le_liv_street->setEnabled(true);
        ui->le_liv_house->setEnabled(true);
        ui->le_liv_corp->setEnabled(true);
        ui->le_liv_flat->setEnabled(true);
    }

}

void Update::upload_main_cr()
{
    get_birthdate();

    main_cr->lastname = ui->le_last_name->text();
    main_cr->name=ui->le_name->text();
    main_cr->mid_name = ui->le_mid_name->text();
    get_birthdate();

    main_cr->check_for = ui->le_check_for->text();
    main_cr->dop_info = ui->le_dop_info->toPlainText();
    main_cr->reg_city = ui->le_reg_city->text();
    main_cr->reg_street = ui->le_reg_street->text();
    main_cr->reg_home = ui->le_reg_house->text();
    main_cr->reg_corp = ui->le_reg_corp ->text();
    main_cr->reg_flat = ui->le_reg_flat->text();

    if (ui->cb_adres->checkState() == Qt::Checked)
    {
        main_cr->liv_city = main_cr->reg_city;
        main_cr->liv_street = main_cr->reg_street;
        main_cr->liv_home = main_cr->reg_home;
        main_cr->liv_corp = main_cr->reg_corp;
        main_cr->liv_flat = main_cr->reg_flat;
    }
    else
    {
        main_cr->liv_city = ui->le_liv_city->text();
        main_cr->liv_street = ui->le_liv_street->text();
        main_cr->liv_home = ui->le_liv_house->text();
        main_cr->liv_corp = ui->le_liv_corp->text();
        main_cr->liv_flat = ui->le_liv_flat->text();
    }

    main_cr->date_add = QDate::currentDate().toString(Qt::ISODate);
    main_cr->time_add = QTime::currentTime().toString();
}

void Update::on_pb_add_line_telephone_clicked()
{
    Component *comp = new Component();
    comp->set_type(new_ow_tel);
    Owners_tel *owt = new Owners_tel(ot_model->otlist->count(),0,false,IsNewing);
    owt->oldnum = false;
    comp->set_index_data(owt);
    int st = comp->exec();
    switch(st)
    {
        case 1:
        owt->oldnum = comp->content->Oldnum;
        owt->internum = comp->content->Internum;
        owt->tel_num = comp->content->tel_num;
        ot_model->addRow_owner_tel(owt);
        //owt->state = IsChanged;
        //ot_model->setOTList(new_cr->owt());
        ui->tableView->setModel(ot_model);
        break;
    }
    delete comp;
}

void Update::on_tableView_2_doubleClicked(const QModelIndex &index)
{
    Component *comp = new Component();
    comp->set_type(Contact_num);
    Contacts *cnt = contacts_model->actlist.at(index.row());
    comp->set_index_data(nullptr,cnt);
    int st = comp->exec();
    switch(st)
    {
        case 1:
        cnt->oldnum = comp->content->Oldnum;
        cnt->internum = comp->content->Internum;
        cnt->contact_tel_num = comp->content->tel_num;
        cnt->mark = comp->content->mark;

        if(cnt->cont_state != IsNewing)
            cnt->cont_state = IsChanged;

        //ot_model->setOTList(new_cr->owt());
        ui->tableView_2->setModel(contacts_model);
        ui->tableView_2->resizeColumnToContents(2);
        break;
    }
    delete comp;
}
