#include "telephones_upd.h"
#include "ui_telephones_upd.h"
#include "crud.h"
#include "owners_tel.h"
#include "contacts.h"

Telephones_upd::Telephones_upd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Telephones_upd)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);
}

Telephones_upd::~Telephones_upd()
{
    delete ui;
}

void Telephones_upd::on_pb_back_to_page1_clicked()
{
    delete this;
}

void Telephones_upd::on_pb_add_contact_line_clicked()
{
    QLineEdit* line = new QLineEdit(this);
    ui->vl_contact_num->addWidget(line);
    line->setObjectName("Contact_line");
    line->show();
    linelist_tel_contact.append(line);

    QLineEdit* line_info = new QLineEdit(this);
    ui->vl_contact_info->addWidget(line_info);
    line_info->setObjectName("Contact_info");
    line_info->show();
    linelist_tel_info.append(line_info);
}

void Telephones_upd::on_pb_add_contact_clicked()
{
    Contacts *c = new Contacts;
    c->owner_tel_id = tel_id;
    foreach (QLineEdit *l , this->findChildren<QLineEdit*>("Contact_line"))
    {
       if (l->text() != QString(""))
           c->append_contact(l->text());
        l->clear();
    }

    foreach (QLineEdit *a , this->findChildren<QLineEdit*>("Contact_info"))
    {
       if (a->text() != QString(""))
             c->append_info(a->text());
      a->clear();
    }
    c->add_contact_to_db();
  delete c;

     Owners_tel *ow = new Owners_tel();
     ow->recieve_contacts(tel_id);
     ui->tableView_2->setModel(ow->model);
      delete ow;
}

void Telephones_upd::on_pb_add_line_telephone_clicked()
{
    QLineEdit* line = new QLineEdit(this);
    ui->Layout_with_new_numbers->addWidget(line);
    line->setObjectName("Telephone_line");
    line->show();
    linelist_telephone.append(line);
}

void Telephones_upd::on_pb_del_line_telephone_clicked()
{
    QLayout *layout =  ui->Layout_with_new_numbers;
      if(layout->count()>0)
      {
          QLayoutItem *item = layout->takeAt(layout->count()-1);
          delete item->widget();
          linelist_telephone.removeAt(linelist_telephone.size()-1);
      }
}

void Telephones_upd::on_pb_add_telephone_clicked()
{
    Owners_tel *o_t = new Owners_tel();
    foreach (QLineEdit *l , this->findChildren<QLineEdit*>("Telephone_line"))
    {
        if (l->text() != QString(""))
           o_t->append_telephones(l->text());
        l->clear();
    }
    o_t->add_numbers(zk_id);
    Crud *cr = new Crud(zk_id);
    cr->select_telephone();
    ui->tableView->setModel(cr->model);
    delete cr;
    delete o_t;
}

void Telephones_upd::on_pb_del_telephone_clicked()
{
    Owners_tel *o_t = new Owners_tel(tel_id);
    Crud *cr = new Crud(zk_id);

    o_t->del_tel();
    cr->select_telephone();
    ui->tableView->setModel(cr->model);
    delete cr;
    delete o_t;
}

void Telephones_upd::on_pb_remove_contact_line_clicked()
{
    QLayout *layout_1 =  ui->vl_contact_num;
    if(layout_1->count()>0)
    {
        QLayoutItem *item = layout_1->takeAt(layout_1->count()-1);
        delete item->widget();
        linelist_tel_contact.removeAt(linelist_tel_contact.size()-1);
    }

    QLayout *layout_2 =  ui->vl_contact_info;
     if(layout_2->count()>0)
     {
         QLayoutItem *item1 = layout_2->takeAt(layout_2->count()-1);
         delete item1->widget();
         linelist_tel_info.removeAt(linelist_tel_info.size()-1);
     }
}



void Telephones_upd::Recieve_id_from_upd(int id)
{
    zk_id = id;
    Crud *cr = new Crud(zk_id);
    cr->select_telephone();
    ui->tableView->setModel(cr->model);
    cr->check();
    delete cr;
}

void Telephones_upd::Recieve_id_from_add(int id)
{
    zk_id = id;
    Crud *cr = new Crud(zk_id);
    cr->select_telephone();
    ui->tableView->setModel(cr->model);
    cr->check();
    delete cr;

    QPushButton *p_b = new QPushButton;
       p_b->setText("Отменить добавление");
       ui->vl_for_button->addWidget(p_b);

       connect(p_b, SIGNAL(clicked()), this ,SLOT(Add_abort()));
}

void Telephones_upd::Add_abort()
{
    Crud *cr = new Crud(zk_id);
    cr->del_zk();
    delete cr;
    emit Ready_to_close();
    delete this;
    qDebug() << "qq";
}

void Telephones_upd::on_tableView_clicked(const QModelIndex &index)
{
    (void)index;
    Owners_tel *ow = new Owners_tel();
    ow->recieve_tel_id(index.data().toString());
    tel_id = ow->tel_id;
    ow->recieve_contacts(tel_id);
    ui->tableView_2->setModel(ow->model);
    delete ow;
}

void Telephones_upd::on_tableView_2_clicked(const QModelIndex &index)
{
    (void)index;
    Contacts *c = new Contacts();
    QModelIndexList *indexes = new QModelIndexList;
    *indexes = ui->tableView_2->selectionModel()->selection().indexes();
c->recieve_cont_id(indexes->value(0).data().toString(),indexes->value(1).data().toString());
contact_id = c->contact_id;
delete c;
}

void Telephones_upd::on_pb_del_contact_clicked()
{
    Owners_tel *ow = new Owners_tel();
    Contacts *c = new Contacts();
    c->del_contact(contact_id);
    ow->recieve_contacts(tel_id);
    ui->tableView_2->setModel(ow->model);
    delete ow;
    delete c;
}


