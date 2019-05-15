#include "update.h"
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

void Update::Recieve_data(int id)
{
    clear_ALL();
     Crud *cr = new Crud(id);
     cr->call_update_list();
     ui->le_last_name->setText(cr->lastname);
     ui->le_name->setText(cr->name);
     ui->le_mid_name->setText(cr->mid_name);

     QString day = cr->birth_date.left(2);
     QString month = cr->birth_date.mid(3,2);
     QString year = cr->birth_date.right(4);

     ui->le_birth_date_day->setText(day);
     ui->le_birth_date_month->setText(month);
     ui->le_birth_date_year->setText(year);

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

     if(Owners_tel::selectZkTel(otList, id) && Contacts::selectAll(contactList))
            ot_model->setOTList(otList);
        ot_model->state = Edit; ///меняю флаги для изменения
        ui->tableView->setModel(ot_model);

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

    Crud *cr = new Crud(zk_id,ui->le_last_name->text(),ui->le_name->text(),
                        ui->le_mid_name->text(), ui->le_check_for->text(),ui->le_dop_info->text(),
                        adres_reg, adres_liv,
                        ui->le_reg_city->text(),ui->le_reg_street->text(),ui->le_reg_house->text(),
                        ui->le_reg_corp ->text(),ui->le_reg_flat->text());
    cr->birth_date = ui->le_birth_date_day->text()+"."+ui->le_birth_date_month->text()+"."+ui->le_birth_date_year->text();
    cr->update_zk();
    adres_reg = "";
    adres_liv = "";

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

    emit Ready_for_update();
    delete cr;
    }
}

//////////////////////////////////////////////////
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

//////////////////////////////////////////////////
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


void Update::on_pb_Back_to_Main_clicked()
{
    clear_ALL();
     emit Ready_for_update();
}


void Update::on_tableView_clicked(const QModelIndex &index)
{
    qDebug() << otList->at(index.row())->tel_id;

    contacts_model->setContactList(contactList, otList->at(index.row())->tel_id);

    contacts_model->state = Edit;
    ui->tableView_2->setModel(contacts_model);
}

void Update::on_pb_del_line_telephone_clicked()
{
    QModelIndex ind = ui->tableView->currentIndex();
    if( ind.isValid())
    {
        ot_model->delRow_owner_tel(ind);
    }
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
    emit Add_contact_row(otList->at(index.row())->tel_id);
}

void Update::clear_ALL()
{
  if (!otList->isEmpty())
      otList->clear();
  ot_model->reset_model();
   if (!contactList->isEmpty())
       contactList->clear();
   contacts_model->reset_model();
}

void Update::set_validators()
{
    ui->le_birth_date_day->setValidator(new QIntValidator(1,31));
    ui->le_birth_date_month->setValidator(new QIntValidator(1,12));
    ui->le_birth_date_year->setValidator(new QIntValidator(1960,2100));
}
