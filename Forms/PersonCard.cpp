#include "PersonCard.h"
#include "ui_PersonCard.h"
#include <QMessageBox>

/// REFACTORED

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
    ui->label_name->setText(this->localPerson->name);
    ui->label_lastname->setText(this->localPerson->lastname);
    ui->label_midname->setText(this->localPerson->midname);
}

void PersonCard::setState(personCardShowingState t)
{
    this->state = t;
    switch(this->state){
    case mainWindow:
        ui->pb_button->hide();
        ui->editButton->hide();
        break;
    case editEventWindow:
        break;
    }
}

void PersonCard::on_editButton_clicked()
{
    emit openEditWindow(this->localPerson);
}

void PersonCard::on_pb_button_clicked()
{
    QMessageBox msg;
    msg.setWindowTitle("Подтверждение");
    msg.setText("Вы действительно хотите удалить выбранного фигуранта?");
    msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

    int a = msg.exec();

    switch (a) {
        case QMessageBox::Ok :
        emit removePerson(this->localPerson);
        return;
    case QMessageBox::Cancel :

        return;
    }
}
