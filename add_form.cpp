#include "add_Form.h"
#include "ui_add_form.h"
#include "_Crud.h"
#include "_Owners_tel.h"
#include "_Contacts.h"
#include <QSqlRecord>
#include "db_connection.h"

Add_form::Add_form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Add_form)
{
    ui->setupUi(this);

    connect(this, SIGNAL(Add_contact_row(int)),contacts_model,SLOT(addRow_contact(int)));
    connect(ui->pb_add_line_telephone, SIGNAL(clicked()), ot_model, SLOT(addRow_owner_tel()));

    set_validators();
    QModelIndex index = ui->tableView->currentIndex();
    qDebug() << index.column();
}

///////////////////////////////////////////////
void Add_form::on_pb_back_to_main_clicked()
{
    msgbx.setText("<font size = '5'> Подтверждение<br>Вы действительно хотите отменить добавление записной книги?</font>");
    msgbx.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgbx.setButtonText(QMessageBox::Ok,"Да");
    msgbx.setButtonText(QMessageBox::Cancel,"Нет");
    int ret = msgbx.exec();

    switch (ret) {
    case QMessageBox::Cancel:
        break;
    case QMessageBox::Ok:
    Clear();
    break;
    }
}

void Add_form::on_pb_remove_line_telephone_clicked()
{
    QModelIndex ind = ui->tableView->currentIndex();
    if( ind.isValid() && otList->count()>1)
    {
        contacts_model->delBindedContacts(otList->at(ind.row())->tel_id);
        ot_model->delRow_owner_tel(ind);
    }
    contacts_model->reset_ContactModel();
}

void Add_form::on_tableView_clicked(const QModelIndex &index)
{
    qDebug() << otList->at(index.row())->tel_id;

    contacts_model->setContactList(contactList, otList->at(index.row())->tel_id);

    contacts_model->state = Edit_cont;
    ui->tableView_2->setModel(contacts_model);
    ui->tableView_2->setColumnWidth(0,250);
    ui->tableView_2->setColumnWidth(1,250);
        qDebug() << index;
}

void Add_form::set_validators()
{
   ui->le_birth_date_day->setValidator(new QIntValidator(1,31));
   ui->le_birth_date_month->setValidator(new QIntValidator(1,12));
   ui->le_birth_date_year->setValidator(new QIntValidator(1960,2100));
}

void Add_form::on_pb_add_contact_line_clicked()
{
    QModelIndex index = ui->tableView->currentIndex();
    if(index.isValid())
        emit Add_contact_row(otList->at(index.row())->tel_id);
}


void Add_form::on_pb_remove_contact_line_clicked()
{
    QModelIndex ind = ui->tableView_2->currentIndex();
    if( ind.isValid())
    {
        contacts_model->delRow_contact(ind);
    }
}


/////////// Метод, выполняемый после перехода на форму ///////////////////
void Add_form::Fill_table_in_add()
{
    Crud *cr = new Crud();
    cr->get_max_zk();
    if(Owners_tel::selectZkTelForAdd(otList, cr->zk_id+1) && Contacts::selectAll(contactList))
           ot_model->setOTList(otList);
       ot_model->state = Edit_Ot; ///меняю флаги для изменения
       ui->tableView->setModel(ot_model);
       ui->tableView->setColumnWidth(0,250);

}


void Add_form::on_pb_add_zk_final_clicked()
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

        Crud *cr = new Crud();
        cr->lastname = ui->le_last_name->text();
        cr->name=ui->le_name->text();
        cr->mid_name = ui->le_mid_name->text();
        cr->check_for = ui->le_check_for->text();
        cr->dop_info = ui->le_dop_info->toPlainText();
        cr->reg_city = ui->le_reg_city->text();
        cr->reg_street = ui->le_reg_street->text();
        cr->reg_home = ui->le_reg_house->text();
        cr->reg_corp = ui->le_reg_corp ->text();
        cr->reg_flat = ui->le_reg_flat->text();
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
            cr->birth_date = day+"."+month+"."+year;
        }

        if (ui->checkBox->checkState() == Qt::Checked)
        {
            cr->liv_city = cr->reg_city;
            cr->liv_street = cr->reg_street;
            cr->reg_home = cr->liv_home;
            cr->reg_corp = cr->liv_corp;
            cr->reg_flat = cr->liv_flat;
        }
        else
        {
            cr->liv_city = ui->le_liv_city->text();
            cr->liv_street = ui->le_liv_street->text();
            cr->liv_home = ui->le_liv_house->text();
            cr->liv_corp = ui->le_liv_corp->text();
            cr->liv_flat = ui->le_liv_flat->text();
        }

        cr->check();
        /// Проверка на уникальность
        Owners_tel *temp = new Owners_tel();

        for (int i=0; i < otList->size(); i++)
        {
            Owners_tel *ow = otList->at(i);
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
                        emit open_update_tab(cr->zk_id);
                        delete cr;
                        return; /// во всех случаях return - мы выходим из функции

                    case QMessageBox::Open:
                        return;

                    case QMessageBox::Cancel:
                     Clear();
                     cr->del_zk(cr->new_zk_id);
                     delete cr;
                        return;
                            }
                }
        }
        delete temp;


        if( cr->add_zk() )
        {
            if( Owners_tel::saveAll(otList)) /// Если сохранили телефоны
            {
                if( Contacts::saveAll(contactList) ) /// Сохраняем контакты
                {
                    QMessageBox::information(this,QObject::tr("Успех"),QObject::tr("Данные сохранены в БД!")); ///Хвалимся
                    Clear();
                    delete cr;
                }
                else
                {   ///Иначе жалуемся на неудачное добавление КОНТАКТОВ
                    QMessageBox::critical(this,QObject::tr("Ошибка"),QObject::tr("Не удалось выполнить запрос: %1").arg(
                                              db_connection::instance()->lastError));
                    Clear();
                    delete cr;
                }
            }
        }
        else {
            QMessageBox::critical(this,QObject::tr("Ошибка"),QObject::tr("Не удалось выполнить обновление данных!"));
             Clear();
             }
             break;
         }
}

void Add_form::Clear()
{
    foreach (QLineEdit *l, this->findChildren<QLineEdit*>())
    {
        l->clear();
    }
    if (!otList->isEmpty())
        otList->clear();
    ot_model->reset_OTModel();
     if (!contactList->isEmpty())
         contactList->clear();
     contacts_model->reset_ContactModel();

    emit toMainForm();
}

Add_form::~Add_form()
{
    delete ui;
}

void Add_form::on_pushButton_clicked()
{
//    QModelIndex index = ui->tableView->currentIndex();
//    //QModelIndex index2 = ui->tableView_2->currentIndex();
//    QString str = QString::number(otList->at(index.row())->tel_id);
//    QString str2 = QString::number(otList->at(index.row())->parentZK_id);
//    QString str5 = QString::number(otList->at(index.row())->state);
//    qDebug() << str + " "+ otList->at(index.row())->tel_num +" "+ str2 + " " + str5;
}

void Add_form::on_radioButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void Add_form::on_radioButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Add_form::on_checkBox_clicked()
{
    if (ui->checkBox->isChecked())
    {
        ui->stackedWidget->setCurrentIndex(0);
        ui->radioButton_2->setVisible(false);
        ui->radioButton->setVisible(false);
        QLabel *textlabel = new QLabel("Адрес проживания продублируется");
        textlabel->setText("Адрес проживания продублируется");
        ui->vl_for_label->addWidget(textlabel);

        ui->le_liv_city->setText(ui->le_reg_city->text());
        ui->le_liv_street->setText(ui->le_reg_street->text());
        ui->le_liv_house->setText( ui->le_reg_house->text());
        ui->le_liv_corp->setText(ui->le_reg_corp->text());
        ui->le_liv_flat->setText(ui->le_reg_flat->text());
    }
    else {
        ui->radioButton_2->setVisible(true);
        ui->radioButton->setVisible(true);
        QLayoutItem *item = ui->vl_for_label->takeAt(0);
        delete item->widget();

        ui->le_liv_city->clear();
        ui->le_liv_street->clear();
        ui->le_liv_house->clear();
        ui->le_liv_corp->clear();
        ui->le_liv_flat->clear();
    }
}
