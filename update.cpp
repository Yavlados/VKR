#include "update.h"
#include "ui_update.h"
#include "crud.h"
#include "owners_tel.h"
#include "contacts.h"
#include "telephones_upd.h"

Update::Update(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Update)
{
    ui->setupUi(this);

   // connect(ui->rb_adres_liv, SIGNAL(clicked()),this, SLOT(radio_button2_checked()));
}

Update::~Update()
{
    delete ui;
}

void Update::Recieve_data(int id)
{

     Crud *cr = new Crud(id);
     cr->call_update_list();
     ui->le_last_name->setText(cr->lastname);
     ui->le_name->setText(cr->name);
     ui->le_mid_name->setText(cr->mid_name);
     ui->lineEdit_4->setText(cr->birth_date);
     ui->le_check_for->setText(cr->check_for);
     ui->le_dop_info->setText(cr->dop_info);

     if(cr->adres_reg == QString("X"))
         ui->rb_adres_reg->toggled(true);

     if(cr->adres_liv == QString("X"))
         ui->rb_adres_liv->toggled(true);

     ui->le_reg_city->setText(cr->reg_city);
     ui->le_reg_street->setText(cr->reg_street);
     ui->le_reg_house->setText(cr->reg_home);
     ui->le_reg_corp->setText(cr->reg_corp);
     ui->le_reg_flat->setText(cr->reg_flat);

//     cr->select_telephone();
//     ui->tableView->setModel(cr->model);
        zk_id = id;
//     cr->check();

     model = new QSqlTableModel(this);
     model->setEditStrategy(QSqlTableModel::OnManualSubmit); //при изменении клетки надо добавить в
     model->setTable("owners_tel");                      //в поле фк соответствующий ключ
     QString str = QString::number(id);

     model->setFilter("\"FK_Telephone_Zk\" IS NULL OR \"FK_Telephone_Zk\" = " + str);

     qDebug() << str;
     model->select();

    ui->tableView->setModel(model);
    model->setHeaderData(2,Qt::Horizontal, QObject::tr("Номер телефона"));
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->setColumnHidden(1,true);
    ui->tableView->resizeColumnsToContents();
     delete cr;
}

void Update::on_pb_Update_clicked()
{
    msgbx.setText("Подтверждение");
    msgbx.setInformativeText("Вы готовы завершить редактирование записной книги?");
    msgbx.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    int ret = msgbx.exec();

    switch (ret) {
    case QMessageBox::Cancel:
        break;
    case QMessageBox::Ok:
    qDebug() << zk_id;
    Crud *cr = new Crud(zk_id,ui->le_last_name->text(),ui->le_name->text(),
                        ui->le_mid_name->text(), ui->le_check_for->text(),ui->le_dop_info->text(),
                        adres_reg, adres_liv,
                        ui->le_reg_city->text(),ui->le_reg_street->text(),ui->le_reg_house->text(),
                        ui->le_reg_corp ->text(),ui->le_reg_flat->text());

    Owners_tel *o_t = new Owners_tel();

    cr->check();
    cr->update_zk();
    adres_reg = "";
    adres_liv = "";

    o_t->new_zk_id = zk_id;
    o_t->get_filter_for_add();

    emit Ready_for_update();
    delete cr;
    delete o_t;
    }
}

void Update::on_rb_adres_reg_clicked()
{
    if (adres_reg.isEmpty())
    {
        adres_liv.clear();
        adres_reg = "X";
    }
    qDebug() << adres_reg;
    qDebug() << adres_liv;
}

void Update::on_rb_adres_liv_clicked()
{
    if (adres_liv.isEmpty())
    {
        adres_reg.clear();
        adres_liv = "X";
    }
    qDebug() << adres_liv;
    qDebug() << adres_reg;
}

void Update::on_pb_add_line_telephone_clicked()
{
    qDebug() << "inserting row" << model->insertRow(model->rowCount());
}

void Update::on_pb_del_line_telephone_clicked()
{
    int selectedRow = ui->tableView->currentIndex().row();
    if (selectedRow >= 0)
    {
        if(ui->tableView->currentIndex().data() != "")
        {
            msgbx.setText("Подтверждение");
            msgbx.setInformativeText("Вы действительно хотите удалить из записной книги номер " +tel_num);
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

void Update::on_pb_Back_to_Main_clicked()
{
     emit Ready_for_update();
}

void Update::on_tableView_clicked(const QModelIndex &index)
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

void Update::on_pb_add_contact_line_clicked()
{
    qDebug() << "inserting row" << model_2->insertRow(model_2->rowCount());

}

void Update::on_pb_add_contact_clicked()
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

void Update::on_pb_remove_contact_line_clicked()
{
    int selectedRow = ui->tableView_2->currentIndex().row();
    qDebug() << selectedRow;
    if (selectedRow >= 0)
    {
        if(ui->tableView_2->currentIndex().data() != "")
        {
            msgbx.setText("Подтверждение");
            msgbx.setInformativeText("Вы действительно хотите удалить выбранный номер из списка контактов?");
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
        else {
            qDebug() << "deleting row" << model_2->removeRow(selectedRow);
        }
    }
    else {
        qDebug() << "No rows selected";
    }
}

void Update::on_pb_add_telephone_clicked()
{
      model->submitAll();
}
