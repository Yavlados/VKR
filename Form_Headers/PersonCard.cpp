#include "PersonCard.h"
#include "ui_PersonCard.h"

PersonCard::PersonCard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PersonCard)
{
    ui->setupUi(this);
}

PersonCard::~PersonCard()
{
    delete ui;
}

void PersonCard::setPerson(Person *person)
{
    this->localPerson = person;
    ui->personFio->setText(this->localPerson->name + " " +
                           this->localPerson->lastname + " " +
                           this->localPerson->midname);
}

//void PersonCard::close()
//{

//}

void PersonCard::on_editButton_clicked()
{
    emit openEditWindow(this->localPerson);
}

//void PersonCard::keyPressEvent(QKeyEvent *event)
//{
//    switch(event->key())
//    {
//    case Qt::Key_Escape:
//        this->close();
//        return;
//    }
//}
