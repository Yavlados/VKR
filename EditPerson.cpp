#include "EditPerson.h"
#include "ui_EditPerson.h"

EditPerson::EditPerson(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditPerson)
{
    ui->setupUi(this);
}

void EditPerson::setPerson(Person *person)
{
    this->person = person;
}

EditPerson::~EditPerson()
{
    delete ui;
}
