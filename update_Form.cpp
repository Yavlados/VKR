#include "update_Form.h"
#include "ui_update.h"
#include "_Crud.h"
#include "_Owners_tel.h"
#include "_Contacts.h"

#include <QSqlRecord>
#include <QStringRef>

Update::Update(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Update)
{
    ui->setupUi(this);
    connect(ui->pb_add_line_telephone, SIGNAL(clicked()), ot_model, SLOT(addRow_owner_tel()));
    connect(this, SIGNAL(Add_contact_row(int)),contacts_model,SLOT(addRow_contact(int)));
    set_validators();
}

Update::~Update()
{
    delete ui;
}

void Update::Recieve_data(Crud *cr)
{
     new_cr = cr;
     ui->le_last_name->setText(new_cr->lastname);
     ui->le_name->setText(new_cr->name);
     ui->le_mid_name->setText(new_cr->mid_name);

     QString day = new_cr->birth_date.left(2);
     QString month = new_cr->birth_date.mid(3,2);
     QString year = new_cr->birth_date.right(4);

     ui->le_birth_date_day->setText(day);
     ui->le_birth_date_month->setText(month);
     ui->le_birth_date_year->setText(year);

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

     if(cr->owt()->isEmpty())
         Owners_tel::selectZkTelForAdd(new_cr->owt(),new_cr->zk_id);
       ot_model->setOTList(new_cr->owt());

     ot_model->state = Edit_Ot; ///меняю флаги для изменения
     ui->tableView->setModel(ot_model);
     ui->tableView->setColumnWidth(0,250);
     contacts_model->reset_ContactModel();
     zk_id = cr->zk_id;

        clear_Vl();

     QLabel *lb = new QLabel("<font size = 10> <h1> <div align=\"center\"> Редактирование </div> </h1> </font>");
     ui->vl_for_label->addWidget(lb);

     QPushButton *p_b = new QPushButton;
     p_b->setText("Редактировать");

     ui->vl_for_button->addWidget(p_b);

     connect(p_b, SIGNAL(clicked()), this ,SLOT(on_pb_Update_clicked()));
}

void Update::on_pb_Update_clicked()
{
    msgbx.setGeometry(960,510, 180,210);
    msgbx.setText("<font size = '8'>Подтверждение</font> <br> <font size = '5'>Вы готовы завершить редактирование записной книги?</font>");
    msgbx.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    int ret = msgbx.exec();

    switch (ret) {
    case QMessageBox::Cancel:
        break;
    case QMessageBox::Ok:

    new_cr->zk_id = zk_id;
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
                return;
            }
        }
        new_cr->birth_date = day+"."+month+"."+year;
    }

    new_cr->liv_city = ui->le_liv_city->text();
    new_cr->liv_street = ui->le_liv_street->text();
    new_cr->liv_home = ui->le_liv_house->text();
    new_cr->liv_corp = ui->le_liv_corp->text();
    new_cr->liv_flat = ui->le_liv_flat->text();

   if( new_cr->update_zk() )
   {
       if(Crud::save_all_crud(new_cr)) /// Если сохранили телефоны
       {
           QMessageBox::information(this,QObject::tr("Успех"),QObject::tr("Данные сохранены в БД!")); ///Хвалимся
          clear_ALL();
       }
       else
       {        /// Если не удалось добавить телефоны
           if(db_connection::instance()->error.number()==23505) //ошибка уникальности поля
           {
               msgbx.setText("<font size = '5'> ВНИМАНИЕ: введенный телефонный номер " +db_connection::instance()->error_tel_num+" "
                                  "обнаружен принадлежим владельцу записной книжки № "+QString::number(new_cr->get_id_from_tel(db_connection::instance()->error_tel_num))+"</font>");
               msgbx.setStandardButtons(QMessageBox::Ok | QMessageBox::Open | QMessageBox::Cancel);
               msgbx.setButtonText(QMessageBox::Ok,"Перейти к записной книжке № "+ QString::number(new_cr->zk_id));
               msgbx.setButtonText(QMessageBox::Open,"Редактировать телефонный номер");
               msgbx.setButtonText(QMessageBox::Cancel,"Закрыть карточку без сохранения");

               int ret = msgbx.exec();

               switch (ret) {
               case QMessageBox::Ok:
                   //Recieve_data(cr->zk_id);

                   break;

               case QMessageBox::Open:
                   break;

               case QMessageBox::Cancel:
                clear_ALL();
                   break;
                       }
           }
           else ///Или ошибка НЕ в нарушении уникальности
           {
               QMessageBox::critical(this,QObject::tr("Ошибка"),QObject::tr("Не удалось выполнить запрос: %1").arg(
                                         db_connection::instance()->lastError));
             clear_ALL();
           }
       }
   }
   else {
       QMessageBox::critical(this,QObject::tr("Ошибка"),QObject::tr("Не удалось выполнить обновление данных!"));
             clear_ALL();
        }
        break;
    }
}

void Update::on_pb_Back_to_Main_clicked()
{
    clear_ALL();
}

void Update::on_tableView_clicked(const QModelIndex &index)
{
    qDebug() << new_cr->zk_id << new_cr->owt()->at(index.row())->tel_id << new_cr->owt()->at(index.row())->state;
    if(new_cr->owt()->at(index.row())->cont()->isEmpty())
        Contacts::selectTelContacts(new_cr->owt()->at(index.row())->cont(),new_cr->owt()->at(index.row())->tel_id);
    contacts_model->setContactList(new_cr->owt()->at(index.row())->cont(), new_cr->owt()->at(index.row())->tel_id);

    contacts_model->state = Edit_cont;
    ui->tableView_2->setModel(contacts_model);
    ui->tableView_2->setColumnWidth(0,250);
    ui->tableView_2->setColumnWidth(1,250);
    qDebug() << new_cr->zk_id << new_cr->owt()->at(index.row())->tel_id << new_cr->owt()->at(index.row())->state;
}

void Update::on_pb_del_line_telephone_clicked()
{
    QModelIndex ind = ui->tableView->currentIndex();
    if( ind.isValid() && new_cr->owt()->count()>1)
    {
        contacts_model->delBindedContacts(new_cr->owt()->at(ind.row())->tel_id);
        ot_model->delRow_owner_tel(ind);
    }
    contacts_model->reset_ContactModel();
}

void Update::on_pb_del_contact_line_clicked()
{
    QModelIndex ind = ui->tableView_2->currentIndex();
    if( ind.isValid())
    {
        contacts_model->delRow_contact(ind);
    }
}

void Update::on_pb_add_contact_line_clicked()
{
    QModelIndex index = ui->tableView->currentIndex();
    if(index.isValid())
        emit Add_contact_row(new_cr->owt()->at(index.row())->tel_id);
}

void Update::clear_ALL()
{
    foreach (QLineEdit *l, this->findChildren<QLineEdit*>())
    {
        l->clear();
    }

   clear_Vl();

   contacts_model->reset_ContactModel();
   ot_model->reset_OTModel();
   emit Ready_for_update();
}

void Update::clear_Vl()
{
    while (ui->vl_for_label->count() > 0)
        delete ui->vl_for_label->takeAt(0);

    while (ui->vl_for_button->count() > 0)
        delete ui->vl_for_button->takeAt(0);

    while (ui->vl_for_cb->count() > 0)
        delete ui->vl_for_cb->takeAt(0);
}

void Update::set_validators()
{
    ui->le_birth_date_day->setValidator(new QIntValidator(1,31));
    ui->le_birth_date_month->setValidator(new QIntValidator(1,12));
    ui->le_birth_date_year->setValidator(new QIntValidator(1960,2100));
}

void Update::on_tableView_2_clicked(const QModelIndex &index)
{
    QModelIndex indexOT = ui->tableView->currentIndex();
    qDebug() << new_cr->owt()->at(indexOT.row())->cont()->at(index.row())->contact_id
             << new_cr->owt()->at(indexOT.row())->cont()->at(index.row())->parent_OT_id ;
}

void Update::Fill_table_in_add()
{
    new_cr = new Crud();

    if(Owners_tel::selectZkTelForAdd(new_cr->owt(), new_cr->zk_id))
           ot_model->setOTList(new_cr->owt());
       ot_model->state = Edit_Ot; ///меняю флаги для изменения
       ui->tableView->setModel(ot_model);
       ui->tableView->setColumnWidth(0,250);

            clear_Vl();

       QLabel *lb = new QLabel("<font size = 10> <h1> <div align=\"center\"> Добавление новой записной книжки  </div></h1> </font>");
       ui->vl_for_label->addWidget(lb);

       QCheckBox *cb = new QCheckBox("<h3> <b> Адреса совпадают </b></h3>");
       cb->setText("Адреса совпадают");
       ui->vl_for_cb->addWidget(cb);

       QPushButton *p_b = new QPushButton;
       p_b->setText("Добавить новую записную книжку");
       ui->vl_for_button->addWidget(p_b);

       connect(cb, SIGNAL(clicked()), this, SLOT(cb_clicked()));
       connect(p_b, SIGNAL(clicked()), this ,SLOT(Add_zk()));
}

void Update::Add_zk()
{
    /// СНАЧАЛА ПРОВЕРЯЕМ ВВЕДЕННЫЕ НОМЕРА

    msgbx.setText("<font size = '5'><h1> Подтверждение </h1> <br>Вы готовы завершить добавление записной книги?</font>");
    msgbx.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgbx.setButtonText(QMessageBox::Ok,"Подтвердить");
    msgbx.setButtonText(QMessageBox::Cancel,"Отмена");
    int ret = msgbx.exec();

    switch (ret) {
    case QMessageBox::Cancel:
        return;
    case QMessageBox::Ok:
        if (!ui->le_birth_date_day->text().isEmpty() && !ui->le_birth_date_month->text().isEmpty() && !ui->le_birth_date_year->text().isEmpty())
            QString birth_date = ui->le_birth_date_day->text()+"."+ui->le_birth_date_month->text()+"."+ui->le_birth_date_year->text();

        new_cr->lastname = ui->le_last_name->text();
        new_cr->name=ui->le_name->text();
        new_cr->mid_name = ui->le_mid_name->text();
        new_cr->check_for = ui->le_check_for->text();
        new_cr->dop_info = ui->le_dop_info->toPlainText();
        new_cr->reg_city = ui->le_reg_city->text();
        new_cr->reg_street = ui->le_reg_street->text();
        new_cr->reg_home = ui->le_reg_house->text();
        new_cr->reg_corp = ui->le_reg_corp ->text();
        new_cr->reg_flat = ui->le_reg_flat->text();
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
                    return;
                }
            }
            new_cr->birth_date = day+"."+month+"."+year;
        }
foreach (QCheckBox *cb, this->findChildren<QCheckBox*>())

        if (cb->checkState() == Qt::Checked)
        {
            new_cr->liv_city = new_cr->reg_city;
            new_cr->liv_street = new_cr->reg_street;
            new_cr->reg_home = new_cr->liv_home;
            new_cr->reg_corp = new_cr->liv_corp;
            new_cr->reg_flat = new_cr->liv_flat;
        }
        else
        {
            new_cr->liv_city = ui->le_liv_city->text();
            new_cr->liv_street = ui->le_liv_street->text();
            new_cr->liv_home = ui->le_liv_house->text();
            new_cr->liv_corp = ui->le_liv_corp->text();
            new_cr->liv_flat = ui->le_liv_flat->text();
        }

        new_cr->check();
        /// Проверка на уникальность
        Owners_tel *temp = new Owners_tel();

        for (int i=0; i < otList->size(); i++)
        {
            Owners_tel *ow = new_cr->owt()->at(i);
                if(!temp->compare_with_base(ow->tel_num))
                {
                    Crud *cr = new Crud(temp->parentZK_id);
                    msgbx.setText("<font size = '5'> ВНИМАНИЕ: введенный телефонный номер " +ow->tel_num+" "
                                       "обнаружен принадлежим владельцу записной книжки № "+QString::number(temp->parentZK_id)+"</font>");
                    msgbx.setStandardButtons(QMessageBox::Ok | QMessageBox::Open | QMessageBox::Cancel);
                    msgbx.setButtonText(QMessageBox::Ok,"Перейти к записной книжке № "+ QString::number(temp->parentZK_id));
                    msgbx.setButtonText(QMessageBox::Open,"Редактировать телефонный номер");
                    msgbx.setButtonText(QMessageBox::Cancel,"Закрыть карточку без сохранения");
                    int ret = msgbx.exec();

                    switch (ret) {
                    case QMessageBox::Ok:
                        emit open_update_tab(cr);
                        delete cr;
                        return; /// во всех случаях return - мы выходим из функции

                    case QMessageBox::Open:
                        return;

                    case QMessageBox::Cancel:
                     clear_ALL();
                     cr->del_zk(cr->new_zk_id);
                     delete cr;
                        return;
                            }
                }
        }
        delete temp;


        if( new_cr->add_zk() )
        {
            if( Crud::save_all_crud(new_cr)) /// Если сохранили телефоны
            {
             QMessageBox::information(this,QObject::tr("Успех"),QObject::tr("Данные сохранены в БД!")); ///Хвалимся
             clear_ALL();
            }
                else
                {   ///Иначе жалуемся на неудачное добавление КОНТАКТОВ
                    QMessageBox::critical(this,QObject::tr("Ошибка"),QObject::tr("Не удалось выполнить запрос: %1").arg(
                                              db_connection::instance()->lastError));
                    Crud::del_zk(new_cr->zk_id);//удаляю добавленную зк
                    clear_ALL();
                }     
        }
        else {
            QMessageBox::critical(this,QObject::tr("Ошибка"),QObject::tr("Не удалось выполнить обновление данных!"));
             clear_ALL();
             }
             break;
    }
}

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

