#include "linkedperson.h"
#include "ui_linkedperson.h"

LinkedPerson::LinkedPerson(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LinkedPerson)
{
    ui->setupUi(this);
}

LinkedPerson::~LinkedPerson()
{
    delete ui;
}

void LinkedPerson::setPerson(Person *p)
{
    this->person = p;
    ui->label_name->setText(this->person->name);
    ui->label_lastname->setText(this->person->lastname);
    ui->label_midname->setText(this->person->midname);
}

void LinkedPerson::on_pb_openView_clicked()
{
    emit openPerson(this->person);
}

void LinkedPerson::on_pb_unlink_clicked()
{
    emit deleteLink(this->person);
}
