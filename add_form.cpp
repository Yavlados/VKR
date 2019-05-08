#include "add_form.h"
#include "ui_add_form.h"
#include "crud.h"
#include "owners_tel.h"
#include "telephones_upd.h"
#include "contacts.h"
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
    connect(this, SIGNAL(Refresh_tab()), this, SLOT(Fill_table_in_add()));
    connect(this, SIGNAL(Clear_All()),this,SLOT(Clear()));
    connect(this, SIGNAL(Check_for_accept()),this,SLOT(Check_accept()));
}

void Add_form::radio_button1_checked()
{
    if (adres_liv == "X")
        adres_liv = "";

    adres_reg = "X";
    qDebug() << adres_reg;
}

void Add_form::radio_button2_checked()
{
    if (adres_reg == "X")
        adres_reg = "";

    adres_liv = "X";
    qDebug() << adres_liv;
}

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
    emit Clear_All();
    break;
    }
}

void Add_form::on_pb_add_line_telephone_clicked()
{
    qDebug() << "inserting row" << model->insertRow(model->rowCount());
}

void Add_form::on_pb_del_line_telephone_clicked()
{
    int selectedRow = ui->tableView->currentIndex().row();
    if (selectedRow >= 0)
    {
        if(ui->tableView->currentIndex().data() != "")
        {
            msgbx.setText("Подтверждение");
            msgbx.setInformativeText("Вы действительно хотите удалить из записной книги номер "+tel_num);
            msgbx.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            int ret = msgbx.exec();

            switch (ret) {
            case QMessageBox::Cancel:
                break;
            case QMessageBox::Ok:
                qDebug() << "deleting row" << model->removeRow(selectedRow);
                model->submitAll();
            break;
            }
        }
        else {
            qDebug() << "deleting row" << model->removeRow(selectedRow);
        }
    }
    else {
        qDebug() << "No rows selected";
    }
}


void Add_form::on_pb_add_contact_line_clicked()
{
    qDebug() << "inserting row" << model_2->insertRow(model_2->rowCount());
}

void Add_form::on_pb_remove_contact_line_clicked()
{
    int selectedRow = ui->tableView_2->currentIndex().row();
    qDebug() << selectedRow;
    if (selectedRow >= 0)
    {
        if(ui->tableView_2->currentIndex().data() != "")
        {
            msgbx.setText("Подтверждение");
            msgbx.setInformativeText("Вы действительно хотите удалить выбранный контакт");
            msgbx.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            int ret = msgbx.exec();

            switch (ret) {
            case QMessageBox::Cancel:
                break;
            case QMessageBox::Ok:
                qDebug() << "deleting row" << model_2->removeRow(selectedRow);
                model_2->submitAll();
            break;
            }
        }
        qDebug() << "deleting row" << model_2->removeRow(selectedRow);
    }
    else {
        qDebug() << "No rows selected";
    }
}

void Add_form::add_abort()
{
    emit toMainForm();
}

void Add_form::Fill_table_in_add()
{
    model = new QSqlTableModel(this);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setTable("owners_tel");
    Owners_tel *ot = new Owners_tel();
    ot->get_new_zk_id();
    QString str = QString::number(ot->new_zk_id + 1);

    model->setFilter("\"FK_Telephone_Zk\" IS NULL OR \"FK_Telephone_Zk\" =" + str);
    qDebug() << ot->new_zk_id;
    qDebug() << str;
     model->select();

   ui->tableView->setModel(model);
   model->setHeaderData(2,Qt::Horizontal, QObject::tr("Номер телефона"));
   ui->tableView->setColumnHidden(0,true);
   ui->tableView->setColumnHidden(1,true);
   ui->tableView->resizeColumnsToContents();
   delete ot;
}

void Add_form::on_pb_add_telephone_clicked()
{

    qDebug() << model->rowCount();
    QString result;
    Owners_tel *ow = new Owners_tel();
     for (int i=0; i < model->rowCount(); i++)
     {
         result = model->record(i).value("Telephone_num").toString();
         qDebug() << result;
         ow->check_tel_num(result);
     }
     delete ow;
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
        zk_id = cr->zk_id;

         emit Check_for_accept();

         Owners_tel *ow = new Owners_tel();
         ow->get_new_zk_id(); // переопределяю зк ид
         ow->get_filter_for_add();
         qDebug() << ow->new_zk_id;
        delete ow;
        delete cr;
         emit Clear_All();
         emit toMainForm();
        break;
    }
}

void Add_form::on_tableView_clicked(const QModelIndex &index)
{
    Owners_tel *ow = new Owners_tel();
    ow->recieve_tel_id(index.data().toString());
    tel_id = ow->tel_id;
    tel_num = index.data().toString();
    qDebug() << tel_id;

    model_2 = new QSqlTableModel(this);
    model_2->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model_2->setTable("contacts");
    QString str = QString::  number(tel_id);

    model_2->setFilter("\"FK_Cl_telephone\" IS NULL OR \"FK_Cl_telephone\" = " + str);

    qDebug() << str;

    model_2->select();

   ui->tableView_2->setModel(model_2);
   ui->tableView_2->setColumnHidden(0,true);
   model_2->setHeaderData(1,Qt::Horizontal, QObject::tr("Номер телефона"));
   model_2->setHeaderData(2,Qt::Horizontal, QObject::tr("Пометка"));
   ui->tableView_2->setColumnHidden(3,true);
   ui->tableView_2->resizeColumnsToContents();

     delete ow;
}

void Add_form::on_pb_add_contact_clicked()
{
   msgbx.setText("Подтверждение");

   msgbx.setInformativeText("Вы готовы завершить заполнение записной книги для номера: "+ tel_num+" ?");
   msgbx.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
   int ret = msgbx.exec();

   switch (ret) {
   case QMessageBox::Cancel:
       break;
   case QMessageBox::Ok:
       model_2->submitAll();
       Contacts *cnt = new Contacts;
       cnt->add_fk_contact(tel_id);
       delete cnt;
       break;
   }
}

void Add_form::Clear()
{
//    model->clear();
//    model_2->clear();
    foreach (QLineEdit *l, this->findChildren<QLineEdit*>())
    {
        l->clear();
    }
    Owners_tel *ow = new Owners_tel();
    ow->del_where_fk_null();
    delete ow;
    emit toMainForm();
}

void Add_form::Check_accept()
{
    Owners_tel *ow = new Owners_tel();
    ow->check_for_null();
    if (ow->null_counter == 0)
    {
        msgbx.setText("ВНИМАНИЕ");

        msgbx.setInformativeText("Вы не подтвердили добавление номера!");
        msgbx.setStandardButtons(QMessageBox::Ok);
    }
    delete ow;
}

void Add_form::on_tableView_2_clicked(const QModelIndex &index)
{
    (void)index;
    emit Check_for_accept();
}

Add_form::~Add_form()
{
    delete ui;
}
