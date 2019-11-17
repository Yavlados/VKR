#include "dialog_conflict.h"
#include "ui_dialog_conflict.h"

Dialog_conflict::Dialog_conflict(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_conflict)
{
    //setFixedWidth()
    setModal(true);
    ui->setupUi(this);
    ui->label->setWordWrap(true);
}

Dialog_conflict::~Dialog_conflict()
{
    delete ui;
}

void Dialog_conflict::setText(QString str)
{
    ui->label->setText(str);
}

void Dialog_conflict::setButtonText(QString str)
{
    ui->pb_go_to->setText(str);
}

void Dialog_conflict::on_pb_save_link_clicked()
{
    state = 1;
            close();
}

void Dialog_conflict::on_pb_merge_clicked()
{
        state = 2;
        close();
}

void Dialog_conflict::on_pb_edit_tel_clicked()
{
        state = 3;
                close();
}

void Dialog_conflict::on_pb_go_to_clicked()
{
        state = 5;
                close();
}

void Dialog_conflict::on_pb_close_clicked()
{
        state = 4;
                close();
}
