#include "officialtelephones_Form.h"
#include "ui_officialtelephones.h"

OfficialTelephones::OfficialTelephones(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OfficialTelephones)
{
    ui->setupUi(this);

}

OfficialTelephones::~OfficialTelephones()
{
    delete ui;
    delete this;
}

void OfficialTelephones::Fill_table()
{
    if(Off_tels::selectOffTel(ofTlist))
           of_model->setOffTList(ofTlist);

       ui->tableView->setModel(of_model);
       ui->tableView->resizeColumnToContents(1);
       ui->tableView->resizeColumnToContents(0);
}
