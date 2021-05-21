#include "EditPerson.h"
#include "ui_EditPerson.h"

EditPerson::EditPerson(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditPerson)
{
    ui->setupUi(this);

    this->ot_model = new MTM_Telephone();
    this->contacts_model = new MTM_Contact();
    this->comparsion = 0;

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

//    ui->lv_linkedPersons.
    this->setFocus();
    ui->label_editPerson_info->hide();
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

    //Address
    ui->le_liv_country->setText(this->editablePerson->country.liv);
    ui->le_reg_country->setText(this->editablePerson->country.reg);

    ui->le_liv_city->setText(this->editablePerson->city.liv);
    ui->le_reg_city->setText(this->editablePerson->city.reg);

    ui->le_liv_street->setText(this->editablePerson->street.liv);
    ui->le_reg_street->setText(this->editablePerson->street.reg);


    ui->le_liv_house->setText(this->editablePerson->house.liv);
    ui->le_reg_house->setText(this->editablePerson->house.reg);

    ui->le_liv_corp->setText(this->editablePerson->corp.liv);
    ui->le_reg_corp->setText(this->editablePerson->corp.reg);

    ui->le_liv_flat->setText(this->editablePerson->flat.liv);
    ui->le_reg_flat->setText(this->editablePerson->flat.reg);

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

    this->fillLinkedPersons();
}

void EditPerson::fillLinkedPersons()
{
   if(this->editablePerson->linked_persons == 0 ||
           this->editablePerson->linked_persons->size() == 0) {
       ui->label_linked_persons->hide();
       while(ui->vl_linked_info->count() != 0) {
           QLayoutItem *item = ui->vl_linked_info->takeAt(0);
           delete item->widget();
       }
       return;
   } else{
       if(ui->vl_linked_person)
           while(ui->vl_linked_person->count() != 0) {
               QLayoutItem *item = ui->vl_linked_person->takeAt(0);
               delete item->widget();
           }
   }



   for (int i=0; i < this->editablePerson->linked_persons->size(); i++) {
       LinkedPerson *card = new LinkedPerson();
       Person *p = this->editablePerson->linked_persons->at(i);
       connect(card, SIGNAL(openPerson(Person*)), this, SLOT(emitOpenLinkedPerson(Person*)));
       connect(card, SIGNAL(deleteLink(Person*)), this, SLOT(destroyLinkBetweenPersons(Person*)));
       card->setPerson(p);
       ui->vl_linked_person->addWidget(card);
   }
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

    // Address
    this->editablePerson->country.liv = ui->le_liv_country->text();
    this->editablePerson->country.reg = ui->le_reg_country->text();

    this->editablePerson->city.liv = ui->le_liv_city->text();
    this->editablePerson->city.reg = ui->le_reg_city->text();

    this->editablePerson->street.liv = ui->le_liv_street->text();
    this->editablePerson->street.reg = ui->le_reg_street->text();


    this->editablePerson->house.liv = ui->le_liv_house->text();
    this->editablePerson->house.reg = ui->le_reg_house->text();

    this->editablePerson->corp.liv = ui->le_liv_corp->text();
    this->editablePerson->corp.reg = ui->le_reg_corp->text();

    this->editablePerson->flat.liv = ui->le_liv_flat->text();
    this->editablePerson->flat.reg = ui->le_reg_flat->text();

    if(ui->gb_adress_liv->isChecked()) this->copyAdresses();

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
        if(this->checkLinksBeforeSave())
            return;

        if(this->state == updatePerson){
            bool state = Person::updatePerson(this->editablePerson);
            if(state){
                this->handleLinks();

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

void EditPerson::on_gb_adress_liv_toggled(bool arg1)
{
    if(!arg1)
        ui->gb_adress_liv->setTitle("Адрес проживания");
    else
    {
        ui->gb_adress_liv->setTitle("Адрес проживания (совпадает с адресом регистрации)");
        this->copyAdresses();
    }

    QList<QLineEdit*> livList = ui->gb_adress_liv->findChildren<QLineEdit*>();
        for (int i =0; i < livList.length(); i++) {
          QLineEdit *le = livList.at(i);
          le->setEnabled(!arg1);
        }
}

void EditPerson::copyAdresses()
{
    this->editablePerson->country.reg   =   ui->le_reg_country->text();
    this->editablePerson->city.reg  =   ui->le_reg_city->text();
    this->editablePerson->street.reg  =   ui->le_reg_street->text();     ;
    this->editablePerson->house.reg  =   ui->le_reg_house->text();     ;
    this->editablePerson->corp.reg  =   ui->le_reg_corp->text();     ;
    this->editablePerson->flat.reg  =   ui->le_reg_flat->text();     ;

    this->editablePerson->country.liv   = this->editablePerson->country.reg;
    this->editablePerson->city.liv      = this->editablePerson->city.reg;
    this->editablePerson->street.liv    = this->editablePerson->street.reg;
    this->editablePerson->house.liv     = this->editablePerson->house.reg;
    this->editablePerson->corp.liv      = this->editablePerson->corp.reg;
    this->editablePerson->flat.liv      = this->editablePerson->flat.reg;

    ui->le_liv_country->setText(this->editablePerson->country.liv);
    ui->le_liv_city->setText(this->editablePerson->city.liv    );
    ui->le_liv_street->setText(this->editablePerson->street.liv  );
    ui->le_liv_house->setText(this->editablePerson->house.liv   );
    ui->le_liv_corp->setText(this->editablePerson->corp.liv    );
    ui->le_liv_flat->setText(this->editablePerson->flat.liv);
}

bool EditPerson::checkLinksBeforeSave()
{
    QList<comparsionResult*> *comparsion = LinksManager::instance()->findLinks(this->editablePerson);
    if(comparsion == 0) return false;
    bool aborted = false;
    for(int i =0; i< comparsion->length(); i++){
        auto comp = comparsion->at(i);
        QMessageBox msg;
        msg.setWindowTitle("Внимание");
        msg.setText(comp->message);
        msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel | QMessageBox::Save);
        msg.setButtonText(QMessageBox::Ok, "Связать фигурантов");
        msg.setButtonText(QMessageBox::Cancel, "Отменить сохранение");
        msg.setButtonText(QMessageBox::Save, "Не связывать");

        int ret = msg.exec();

        switch(ret){
        case QMessageBox::Ok:
            comp->solution = link_solution;
            break;
        case QMessageBox::Cancel:
            aborted = true;
            break;
        case QMessageBox::Save:
            comp->solution = ignore_solution;
            break;
        }
        if(aborted) break;
    }
    if(aborted) return true;
    this->comparsion = comparsion;
    return false;
}

bool EditPerson::handleLinks()
{
    QList<comparsionResult*> *list = new QList<comparsionResult*>();
    if( this->comparsion != 0 )
        for (int i=0; i<this->comparsion->length(); i++ ) {
            auto comp = this->comparsion->at(i);
            switch(comp->solution){
                case link_solution:
                list->append(comp);
                break;
            case ignore_solution:
                break;
            }
        }
    if(list->size() > 0 ){
        LinksManager::instance()->createLinks(this->editablePerson, list);
        Person::getLinkedPersons(this->editablePerson);
    }
   delete list;
    return true;
}

void EditPerson::emitOpenLinkedPerson(Person *person)
{
    QMessageBox msg;
    msg.setWindowTitle("Внимание");
    msg.setText("Вы действительно хотите перейти к редактированию фигуранта <b>"+person->lastname + "</b> <b>"+ person->name+ "</b> <b>" + person->midname+"</b>?");
    msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msg.setButtonText(QMessageBox::Ok, "Перейти к редактированию");
    msg.setButtonText(QMessageBox::Cancel, "Отмена");
    int ret = msg.exec();

    switch(ret){
        case QMessageBox::Ok:
        emit openLinkedPerson(person);
        this->closeWidget();
        break;
    case QMessageBox::Cancel:
        return;
    }

}

void EditPerson::destroyLinkBetweenPersons(Person *person)
{
    QMessageBox msg;
    QMessageBox msg1;
    msg.setWindowTitle("Внимание");
    msg.setText("Вы действительно хотите разорвать связь между <b>"+this->editablePerson->lastname + "</b> <b>"+ this->editablePerson->name+ "</b> <b>" + this->editablePerson->midname+"</b> и <b>"+person->lastname + "</b> <b>"+ person->name+ "</b> <b>" + person->midname+"</b>?");
    msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msg.setButtonText(QMessageBox::Ok, "Да");
    msg.setButtonText(QMessageBox::Cancel, "Отмена");
    int ret = msg.exec();

    switch(ret){
    case QMessageBox::Ok:
        if(Person::unlinkPersons(this->editablePerson, person) && Person::getLinkedPersons(this->editablePerson)){
            this->fillLinkedPersons();
            msg1.setText("Разрыв связи произошел успешно");
        } else{
            msg1.setText("Возникла ошибка при разрыве связи");
            return;
        }
           msg1.exec();
        break;
    case QMessageBox::Cancel:
        return;
    }

}


