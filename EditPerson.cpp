#include "EditPerson.h"
#include "ui_EditPerson.h"

EditPerson::EditPerson(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditPerson)
{
    ui->setupUi(this);

    this->ot_model = new MTM_Telephone();
    this->contacts_model = new MTM_Contact();

    ui->tableView->setType(UpdateOT);
    connect(ui->tableView, SIGNAL(getCont(QModelIndex)), this, SLOT(on_tableView_clicked(QModelIndex)));
    connect(ui->tableView, SIGNAL(stopFocus()), ui->le_last_name, SLOT( setFocus() ) );
    connect(ui->tableView, SIGNAL(openUpdOT(QModelIndex) ), this, SLOT(on_tableView_doubleClicked(QModelIndex)));
    connect(ui->tableView, SIGNAL(f1Pressed() ), this, SLOT( ShowPopUp()));
    connect(ui->tableView, SIGNAL(deleteTelephone()), this, SLOT(on_pb_del_line_telephone_clicked()));
    connect(ui->tableView, SIGNAL(addNewTelephone()), this, SLOT(on_pb_add_line_telephone_clicked()));
    connect(ui->tableView, SIGNAL(addNewCont()), this, SLOT(on_pb_add_contact_line_clicked()));

    ui->tableView_2->setType(UpdateCont);
    connect(ui->tableView_2, SIGNAL(backOnOt()), ui->tableView, SLOT( setFocus() ) );
    connect(ui->tableView_2, SIGNAL(stopFocus()), ui->le_last_name, SLOT( setFocus() ));
    connect(ui->tableView_2, SIGNAL(openUpdCont(QModelIndex) ), this, SLOT(on_tableView_2_doubleClicked(QModelIndex)));
    connect(ui->tableView_2, SIGNAL(f1Pressed() ), this, SLOT( ShowPopUp()));
    connect(ui->tableView_2, SIGNAL(addNewTelephone()), this, SLOT(on_pb_add_contact_line_clicked()));
    connect(ui->tableView_2, SIGNAL(deleteTelephone()), this, SLOT(on_pb_del_contact_line_clicked()));
    connect(ui->tableView_2, SIGNAL(addNewCont()), this, SLOT(on_pb_add_contact_line_clicked()));

    this->setFocus();
}

void EditPerson::setPerson(Person *person)
{
    this->person = person;
    this->editablePerson = person;
    this->fillFields();

}

EditPerson::~EditPerson()
{
    delete ui;
}

void EditPerson::keyPressEvent(QKeyEvent *event)
{
    QItemSelectionModel::SelectionFlags flags = QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows;
    QModelIndex ind1 = ui->tableView->model()->index(0, 0);

        switch(event->key())
        {
        case Qt::Key_Enter:
            on_pb_save_clicked();
            return;
        case Qt::Key_Escape:
            this->closeWidget();
            return;
            case Qt::Key_Space:
                if(ui->tableView->currentIndex().isValid())
                    on_tableView_clicked(ui->tableView->currentIndex());
            return;
        case Qt::Key_PageDown:
//            prev_page();
            return;
        case Qt::Key_PageUp:
//            next_page();
            return;
         case Qt::Key_F1:
            ShowPopUp();
            return;
            case Qt::Key_T:
                ui->tableView->selectionModel()->select(ind1, flags);
                ui->tableView->setFocus();
            return;
            case Qt::Key_G:
            return;
        }
}

void EditPerson::setType(formStates s)
{
    this->state = s;
}

//void EditPerson::setParent(editEvent *parent)
//{
//    this->parent = parent;
//}

void EditPerson::on_tableView_clicked(const QModelIndex &index)
{

    //В добавлении мы работаем исключительно с моделью
                  //Также как и в импорте
    QList<Contact*> *cont_temp = new QList<Contact*>;
    Telephone *selectedTel= ot_model->actotlist.at(index.row());
    cont_temp = selectedTel->cont();
    if(cont_temp->isEmpty() && selectedTel->state != IsNewing){
        Contact::selectContacts(cont_temp, selectedTel->id);
    }

    contacts_model->setContactList(this->editablePerson->telephones()->at(index.row())->cont());
    //contacts_model->state = Edit_cont;

    ui->tableView_2->setModel(contacts_model);
    ui->tableView_2->resizeColumnToContents(0);
    ui->tableView_2->resizeColumnToContents(1);
    ui->tableView_2->resizeColumnToContents(2);
    ui->tableView_2->setWordWrap(false);
    ui->tableView_2->horizontalHeader()->setStretchLastSection(true);

    QString temp = "Номера контактов("+QString::number(contacts_model->actlist.count())+")";
    ui->label_cont->setText(temp);
    if(ui->tableView_2->model()->rowCount() > 0)
    {
        QItemSelectionModel::SelectionFlags flags = QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows;
        QModelIndex tempIndex = ui->tableView_2->model()->index(0, 0);
        ui->tableView_2->selectionModel()->select(tempIndex, flags);
        ui->tableView_2->setFocus();
    }
}

void EditPerson::on_tableView_doubleClicked(const QModelIndex &index)
{
    Component *comp = new Component();
    comp->set_type(Ow_tel_num);
    Telephone *owt = ot_model->actotlist.at(index.row());
    comp->set_index_data(owt, 0);
    int st = comp->exec();
               comp->setFocus();
    switch(st)
    {
        case 1:
        owt->oldnum = comp->content->Oldnum;
        owt->internum = comp->content->Internum;
        owt->num = comp->content->tel_num;

        if(owt->state != IsNewing)
            owt->state = IsChanged;

        //ot_model->setOTList(new_cr->owt());
        ui->tableView->setModel(ot_model);

        QString temp = "Номера телефонов("+QString::number(this->editablePerson->telephones()->size())+")";
        ui->label_tels->setText(temp);
        break;
    }
    delete comp;
}

void EditPerson::on_pb_add_line_telephone_clicked()
{
        Component *comp = new Component();
        comp->set_type(new_ow_tel);
        Telephone *owt = new Telephone();
        owt->internum = false;
        owt->id = ot_model->otlist->count();
        owt->person_id = "0";
        owt->state = IsNewing;
        owt->oldnum = false;

        comp->set_index_data(owt);
        int st = comp->exec();
                   comp->setFocus();
        switch(st)
        {
            case 1:
                owt->oldnum = comp->content->Oldnum;
                owt->internum = comp->content->Internum;
                owt->num = comp->content->tel_num;
                ot_model->addRow_owner_tel(owt);
                //owt->state = IsChanged;
                //ot_model->setOTList(new_cr->owt());
                ui->tableView->setModel(ot_model);

                QString temp = "Номера телефонов("+QString::number(this->editablePerson->telephones()->size())+")";
                ui->label_tels->setText(temp);
            break;
        }
        delete comp;
}

void EditPerson::on_pb_del_line_telephone_clicked()
{
    QModelIndex ind = ui->tableView->currentIndex();
    if( ind.isValid())
        {
        QMessageBox msg;
        msg.setWindowTitle("Подтверждение");
        msg.setText("Вы действительно хотите удалить выбранный телефон?");
        msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

        int a = msg.exec();
        switch (a)
        {
            case QMessageBox::Ok :
            contacts_model->delBindedContacts(this->editablePerson->telephones()->at(ind.row())->id);
            ot_model->delRow_owner_tel(ind);
//            if(imprt_t == Update_import_data) //взаимодействие с моделью
//            {
//                Owners_tel *temp_owt  = new_cr->owt()->at(ind.row());
//                 delete temp_owt;
//                 temp_owt = 0;
//                 new_cr->owt()->removeAt(ind.row());
//            }
//            if (new_cr->owt()->count() == 0)
//            {
//                Owners_tel *owt = new Owners_tel(ot_model->otlist->count(),0,false,IsNewing);
//                owt->oldnum = false;
//                ot_model->addRow_owner_tel(owt);
//                ot_model->reset_OTModel();

//            }
            ui->tableView->setModel(ot_model);
            //QString temp1 = "Номера телефонов("+QString::number(new_cr->owt()->size())+")";
            ui->label_tels->setText("Номера телефонов("+QString::number(this->editablePerson->telephones()->size())+")");
            contacts_model->reset_ContactModel();
            ui->label_cont->setText("Номера контактов");
            if(ui->tableView->model()->rowCount() > 0 ) setFocusOnTab1();
            else set_tab_orders();
        return;
        case QMessageBox::Cancel :
            if(ui->tableView->model()->rowCount() > 0 ) setFocusOnTab1();
            else set_tab_orders();
        return;
        }
    }
}

void EditPerson::on_tableView_2_clicked(const QModelIndex &index)
{
    QModelIndex indexOT = ui->tableView->currentIndex();
}

void EditPerson::on_tableView_2_doubleClicked(const QModelIndex &index)
{
        Component *comp = new Component();
        comp->set_type(Contact_num);
        Contact *cnt = contacts_model->actlist.at(index.row());
        comp->set_index_data(0,cnt);
        int st = comp->exec();
                   comp->setFocus();
        switch(st)
        {
            case 1:
            cnt->oldnum = comp->content->Oldnum;
            cnt->internum = comp->content->Internum;
            cnt->number = comp->content->tel_num;
            cnt->alias = comp->content->mark;

            if(cnt->state != IsNewing)
                cnt->state = IsChanged;

            //ot_model->setOTList(new_cr->owt());
            ui->tableView_2->setModel(contacts_model);
            ui->tableView_2->resizeColumnToContents(2);
            QString temp = "Номера контактов("+QString::number(contacts_model->actlist.count())+")";
            ui->label_cont->setText(temp);
            break;
        }
        delete comp;
}

void EditPerson::on_pb_add_contact_line_clicked()
{
    QModelIndex index = ui->tableView->currentIndex();
       if(index.isValid())
       {
           Component *comp = new Component();
           comp->set_type(new_cont_tel);

           Contact *cnt = new Contact();
           cnt->telephone_id = this->editablePerson->telephones()->at(index.row())->id;

           cnt->oldnum = false;
           comp->set_index_data(0,cnt);
           int st = comp->exec();
           comp->setFocus();
           switch(st)
           {
           case 1:
               cnt->oldnum = comp->content->Oldnum;
               cnt->internum = comp->content->Internum;
               cnt->number= comp->content->tel_num;
               cnt->alias = comp->content->mark;
               contacts_model->addRow_contact(cnt);

               ui->tableView_2->setModel(contacts_model);

               QString temp = "Номера контактов("+QString::number(contacts_model->actlist.count())+")";
               ui->label_cont->setText(temp);

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

void EditPerson::on_pb_del_contact_line_clicked()
{
    QModelIndex ind = ui->tableView_2->currentIndex();
    if( ind.isValid())
    {
        QMessageBox msg;
        msg.setWindowTitle("Подтверждение");
        msg.setText("Вы действительно хотите удалить выбранный телефон?");
        msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

        int a = msg.exec();
        switch (a)
        {
            case QMessageBox::Ok :

        contacts_model->delRow_contact(ind);
        ui->label_cont->setText("Номера контактов("+QString::number(contacts_model->actlist.count())+")");
            if(ui->tableView_2->model()->rowCount() > 0)setFocusOnTab2();
            else if(ui->tableView->model()->rowCount() > 0 ) setFocusOnTab1();
            else set_tab_orders();
            return ;
            case QMessageBox::Cancel :
            if(ui->tableView_2->model()->rowCount() > 0)setFocusOnTab2();
            else if(ui->tableView->model()->rowCount() > 0 ) setFocusOnTab1();
            else set_tab_orders();
            return ;
        }
    }
}

void EditPerson::setFocusOnTab1()
{
    QItemSelectionModel::SelectionFlags flags = QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows;
    QModelIndex tempIndex = ui->tableView->model()->index(0, 0);
    ui->tableView->selectionModel()->select(tempIndex, flags);
    ui->tableView->setFocus();
}

void EditPerson::setFocusOnTab2()
{
    QItemSelectionModel::SelectionFlags flags = QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows;
    QModelIndex tempIndex = ui->tableView_2->model()->index(0, 0);
    ui->tableView_2->selectionModel()->select(tempIndex, flags);
    ui->tableView_2->setFocus();
}

void EditPerson::set_tab_orders()
{

    ui->le_last_name->setFocus();
    setTabOrder( ui->le_last_name, ui->le_name);
    setTabOrder( ui->le_name, ui->le_mid_name);

//    setTabOrder(ui->le_mid_name, bd->day);
//    setTabOrder(bd->day, bd->month);
//    setTabOrder(bd->month, bd->year);

//    setTabOrder( bd->year, ui->le_check_for);
//    setTabOrder(ui->le_check_for, ui->cb_adres);
//    setTabOrder(ui->le_check_for, ui->le_nickname);

//    setTabOrder(ui->le_nickname,  ui->cb_adres);

//    setTabOrder( ui->cb_adres, ui->le_reg_city);

//    setTabOrder(ui->le_reg_city, ui->le_reg_street);
//    setTabOrder(ui->le_reg_street, ui->le_reg_house);
//    setTabOrder(ui->le_reg_house, ui->le_reg_corp);
//    setTabOrder(ui->le_reg_corp, ui->le_reg_flat);

//    setTabOrder(ui->le_reg_flat, ui->le_liv_city);

//    setTabOrder(ui->le_liv_city, ui->le_liv_street);
//    setTabOrder(ui->le_liv_street, ui->le_liv_house);
//    setTabOrder(ui->le_liv_house, ui->le_liv_corp);
//    setTabOrder(ui->le_liv_corp, ui->le_liv_flat);

//    setTabOrder(ui->pb_Back_to_Main, p_b);
}

void EditPerson::setFocus()
{
    ui->le_last_name->setFocus();
}


void EditPerson::ShowPopUp()
{
    PopUp::instance()->setPopupText("<h2 align=\"middle\">Навигация в окне редактирования</h2>"
                                    "<p><b>\"CTRL\"+\"PGUP\" и \"CTRL\"+\"PGDN\"</b> для переключения вкладок связанных объектов</p>"
                                    "<p><b>\"ENTER\"</b> для сохранения изменений</p>"
                                    "<p><b>\"ESC\"</b> для закрытия окна редактирования без сохранения</p>"
                                    "<h2 align=\"middle\">Навигация внутри таблиц</h2>"
                                    "<p>&rarr; для раскрытия контактов фокуса</p>"
                                    "<p>&uarr; и &darr; для перемещения фокуса вверх и вниз в пределах таблицы</p>"
                                    "<p>&larr; для возвращения фокуса назад</p>"
                                    "<p><b>\"D\"</b> для удаления телефона под фокусом</p>"
                                    "<p><b>\"A\"</b> для добавления телефона</p>"
                                    "<p><b>\"S\"</b> для добавления контакта</p>"
                                    "<p><b>\"ENTER\"</b> для редактирования телефона под фокусом</p>"
                                    "<p><b>\"ESC\"</b> для перемещения фокуса обратно на форму</p>"
                                    , leftMenu);
}

void EditPerson::fillFields()
{
    ui->le_name->setText(this->editablePerson->name);
    ui->le_alias->setText(this->editablePerson->alias);
    ui->le_mid_name->setText(this->editablePerson->midname);
    ui->le_last_name->setText(this->editablePerson->lastname);

    if(     this->state == updatePerson &&
            this->editablePerson->telephones()->isEmpty()){
        Telephone::selectTelephone(this->editablePerson->telephones(), this->editablePerson->id);
    }
    this->ot_model->setOTList(this->editablePerson->telephones());
    ui->tableView->setModel(this->ot_model);
    ui->tableView->resizeColumnToContents(0);
    ui->tableView->resizeColumnToContents(1);
    ui->tableView->resizeColumnToContents(2);
    ui->tableView->resizeColumnToContents(3);
    ui->tableView->setWordWrap(false);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
}

void EditPerson::on_pb_save_clicked()
{
    ///Исключаю пропуски
    for (int a=0; a < ot_model->actotlist.size(); a++)
    {
        if((ot_model->actotlist.at(a)->num == "") && (ot_model->actotlist.at(a)->state == IsNewing))
        {
            ot_model->actotlist.removeAt(a);
            ot_model->otlist->removeAt(a);
            a--;
        }
    }

    for (int a=0; a < contacts_model->actlist.size(); a++)
    {
        if((contacts_model->actlist.at(a)->number == "") &&
                (contacts_model->actlist.at(a)->state == IsNewing))
        {
            contacts_model->actlist.removeAt(a);
            contacts_model->clist->removeAt(a);
            a--;
        }
    }

    this->editablePerson->name = ui->le_name->text();
    this->editablePerson->alias = ui->le_alias->text();
    this->editablePerson->midname = ui->le_mid_name->text();
    this->editablePerson->lastname = ui->le_last_name->text();

    msgbx.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    if(this->state==updatePerson)
        msgbx.setText("<font size = '8'>Подтверждение</font> <br> <font size = '5'>"
                  "Вы готовы завершить редактирование фигуранта уголовного дела?</font>");

    else if(this->state==addPerson)
        msgbx.setText("<font size = '8'>Подтверждение</font> <br> <font size = '5'>"
              "Вы готовы завершить добавление фигуранта уголовного дела?</font>");

    msgbx.setWindowTitle("Выберите действие");

    int ret = msgbx.exec();

    switch (ret) {
    case QMessageBox::Cancel:
        break;
    case QMessageBox::Ok:

        if(this->state == updatePerson){
            bool state = Person::updatePerson(this->editablePerson);
            if(state){
                msgbx.setText("<font size = '5'>Данные усешно сохранены.</font>");
                msgbx.setWindowTitle("Успех");
                msgbx.setStandardButtons(QMessageBox::Ok);
                msgbx.exec();
                this->closeWidget();

            } else {
                msgbx.setText("<font size = '5'>Во время сохранения данных возникла следующая ошибка:"+
                              db_connection::instance()->lastError+"</font>");
                msgbx.setWindowTitle("Ошибка");
                msgbx.setStandardButtons(QMessageBox::Ok);
                msgbx.exec();
            }
        }else if(this->state == addPerson){
            QStringList list = this->editablePerson->id.split("raw");
            this->editablePerson->id = "new" + list[1];
//            this->parent->clearLayout();
            emit personIsAdded(this);
            this->closeWidget();
        }



        break;
    }
    ///ПРОВЕРКА НОВЫХ НОМЕРОВ
//        switch (imprt_t) {
//        case Update_import_data:
//            break;
//        case Add_import_data:
//            break;

//        case Update_pg_data:            //Проверку номеров с базой
//          if(new_cr->zk_id == main_cr->zk_id)
//          {
//              if (!compare_tel_num())     //Делаем только в случае редактирования
//              {
//                  return;
//              }
//                  break;
//          }
//                break;

//        case Update_pg_data_import:
//            break;
//             }




//    this->editablePerson->check_for = ui->le_check_for->text();
//    this->editablePerson->dop_info = ui->le_dop_info->toPlainText();
//    this->editablePerson->reg_city = ui->le_reg_city->text();
//    this->editablePerson->reg_street = ui->le_reg_street->text();
//    this->editablePerson->reg_home = ui->le_reg_house->text();
//    this->editablePerson->reg_corp = ui->le_reg_corp ->text();
//    this->editablePerson->reg_flat = ui->le_reg_flat->text();
//    this->editablePerson->nickname = ui->le_nickname->text();
//    if (ui->cb_adres->checkState() == Qt::Checked)
//    {
//        this->editablePerson->liv_city = new_cr->reg_city;
//        this->editablePerson->liv_street = this->editablePerson->reg_street;
//        this->editablePerson->liv_home = this->editablePerson->reg_home;
//        this->editablePerson->liv_corp = this->editablePerson->reg_corp;
//        this->editablePerson->liv_flat = this->editablePerson->reg_flat;
//    }
//    else
//    {
//        this->editablePerson->liv_city = ui->le_liv_city->text();
//        this->editablePerson->liv_street = ui->le_liv_street->text();
//        new_cr->liv_home = ui->le_liv_house->text();
//        this->editablePerson->liv_corp = ui->le_liv_corp->text();
//        this->editablePerson->liv_flat = ui->le_liv_flat->text();
//    }

//        if(frm_t == Confluence_form)
//        {
//            if(new_cr->zk_id == 0)
//            {
//                Add_zk_into_base();
//                return;
//            }else
//                Crud::del_zk(added_cr->zk_id);
//        }
//        if( new_cr->update_zk(linked_crud_id) )
//        {
//            if(Crud::save_all_crud(new_cr)) /// Если сохранили телефоны
//            {
//                if(list_for_destroy != 0)
//                {
//                    Crud *m_cr = list_for_destroy->at(0);

//                    for(int a = 1; a<list_for_destroy->size(); a++)
//                    {
//                        for(int b = 0 ; b< linked_crud_id->size(); b++)
//                        {
//                            if( list_for_destroy->at(a)->zk_id == linked_crud_id->at(b))
//                            {
//                                linked_crud_id->removeAt(b);
//                                break;
//                            }
//                        }
//                        destroy_link(m_cr,list_for_destroy->at(a));
//                    }
//                }

//                QMessageBox::information(this,QObject::tr("Успех"),QObject::tr("Данные сохранены в БД!")); ///Хвалимся
//               clear_ALL();
//            }
//            else
//            {        /// Если не удалось добавить телефоны
//         QMessageBox::critical(this,QObject::tr("Ошибка"),QObject::tr("Не удалось выполнить обновление данных!"));
//                 clea r_ALL();
//            }
//        }
//        else {
//            QMessageBox::critical(this,QObject::tr("Ошибка"),QObject::tr("Не удалось выполнить обновление данных!"));
//                  clear_ALL();
//             }
//             return;

//    }

}

void EditPerson::on_pb_cancel_clicked()
{

    msgbx.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

    msgbx.setText("<font size = '8'>Подтверждение</font> <br> <font size = '5'>"
                  "Вы готовы выйти без сохранения?</font>");

    msgbx.setWindowTitle("Выберите действие");

    int ret = msgbx.exec();

    switch (ret) {
    case QMessageBox::Cancel:
        break;
    case QMessageBox::Ok:
        this->closeWidget();
        break;
    }
}

void EditPerson::closeWidget()
{
    emit closeThis(this);
}
