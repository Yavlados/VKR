#include "custommsgbox.h"
#include "ui_custommsgbox.h"

CustomMsgBox::CustomMsgBox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CustomMsgBox)
{
    ui->setupUi(this);
}

CustomMsgBox::~CustomMsgBox()
{
    delete ui;
}

void CustomMsgBox::setMessage(QString text)
{
    ui->label_message->setText(text);
}

void CustomMsgBox::setText(customMsgButtons type, QString text)
{
    switch(type){
    case    linkPersons:
        ui->pb_createLink->setText(text);
        break;
    case    abortSaving:
         ui->pb_abortSaving->setText(text);
        break;
    case     doNotLink:
         ui->pb_dontLink->setText(text);
        break;
    case    mergeFirstIsParent:
         ui->pb_mergeFirst->setText(text);
        break;
    case    mergeSecondIsParent:
         ui->pb_mergeSecond->setText(text);
        break;
    }
}

void CustomMsgBox::closeEvent(QCloseEvent *e)
{
        this->done(doNotLink);
}

void CustomMsgBox::on_pb_createLink_clicked()
{
    this->done(linkPersons);
}

void CustomMsgBox::on_pb_abortSaving_clicked()
{
    this->done(abortSaving);
}

void CustomMsgBox::on_pb_dontLink_clicked()
{
    this->done(doNotLink);
}

void CustomMsgBox::on_pb_mergeFirst_clicked()
{
    this->done(mergeFirstIsParent);
}

void CustomMsgBox::on_pb_mergeSecond_clicked()
{
    this->done(mergeSecondIsParent);
}
