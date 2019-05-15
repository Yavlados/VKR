#include "add_form.h"
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
    ui->radioButton->click();

    connect(ui->radioButton, SIGNAL(clicked()),this, SLOT(radio_button1_checked()));
    connect(ui->radioButton_2, SIGNAL(clicked()),this, SLOT(radio_button2_checked()));

    connect(this, SIGNAL(Add_contact_row(int)),contacts_model,SLOT(addRow_contact(int)));
    connect(ui->pb_add_line_telephone, SIGNAL(clicked()), ot_model, SLOT(addRow_owner_tel()));

    set_validators();
    QModelIndex index = ui->tableView->currentIndex();
    qDebug() << index.column();
}
///////////////////////////////////////////////
void Add_form::radio_button1_checked()
{
    if (adres_liv == "X")
        adres_liv = "";

    adres_reg = "X";
    qDebug() << adres_reg;
}
///////////////////////////////////////////////
void Add_form::radio_button2_checked()
{
    if (adres_reg == "X")
        adres_reg = "";

    adres_liv = "X";
    qDebug() << adres_liv;
}
///////////////////////////////////////////////
void Add_form::on_pb_back_to_main_clicked()
{
    msgbx.setText("Подтверждение");
    msgbx.setInformativeText("Вы действительно хотите отменить добавление Записной книги?");
    msgbx.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
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
    if( ind.isValid())
    {
        ot_model->delRow_owner_tel(ind);
    }
}

void Add_form::on_tableView_clicked(const QModelIndex &index)
{
    qDebug() << otList->at(index.row())->tel_id;

    contacts_model->setContactList(contactList, otList->at(index.row())->tel_id);

    contacts_model->state = Edit;
    ui->tableView_2->setModel(contacts_model);
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
       ot_model->state = Edit; ///меняю флаги для изменения
       ui->tableView->setModel(ot_model);
}


void Add_form::on_pb_add_zk_final_clicked()
{
    msgbx.setText("Подтверждение");
    msgbx.setInformativeText("Вы готовы завершить добавление записной книги?");
    msgbx.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    int ret = msgbx.exec();

    switch (ret) {
    case QMessageBox::Cancel:
        break;
    case QMessageBox::Ok:
        QString birth_date = ui->le_birth_date_day->text()+"."+ui->le_birth_date_month->text()+"."+ui->le_birth_date_year->text();

        Crud *cr = new Crud(ui->le_last_name->text(),ui->le_name->text(),
                            ui->le_mid_name->text(), birth_date, ui->le_check_for->text(),ui->le_dop_info->toPlainText(),
                            adres_reg, adres_liv,
                            ui->le_reg_city->text(),ui->le_reg_street->text(),ui->le_reg_house->text(),
                            ui->le_reg_corp ->text(),ui->le_reg_flat->text());
        cr->check();
        cr->add_zk();

        if( Owners_tel::saveAll(otList))
        {
            if( Contacts::saveAll(contactList) )
            {
                QMessageBox::information(this,QObject::tr("Успех"),QObject::tr("Данные сохранены в БД!"));
                //reopen();
            }
            else
                QMessageBox::critical(this,QObject::tr("Ошибка"),QObject::tr("Не удалось выполнить запрос: %1").arg(
                                          db_connection::instance()->lastError));
        }
        else
            QMessageBox::critical(this,QObject::tr("Ошибка"),QObject::tr("Не удалось выполнить запрос: %1").arg(
                                      db_connection::instance()->lastError));
        delete cr;
         Clear();
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
    ot_model->reset_model();
     if (!contactList->isEmpty())
         contactList->clear();
     contacts_model->reset_model();

    emit toMainForm();
}

Add_form::~Add_form()
{
    delete ui;
}

void Add_form::on_pushButton_clicked()
{
    QModelIndex index = ui->tableView->currentIndex();
    //QModelIndex index2 = ui->tableView_2->currentIndex();
    QString str = QString::number(otList->at(index.row())->tel_id);
    QString str2 = QString::number(otList->at(index.row())->parentZK_id);
    QString str5 = QString::number(otList->at(index.row())->state);
    qDebug() << str + " "+ otList->at(index.row())->tel_num +" "+ str2 + " " + str5;

}
