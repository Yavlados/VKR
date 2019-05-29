#include "Import_Form.h"
#include "ui_import_form.h"

Import_Form::Import_Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Import_Form)
{
    ui->setupUi(this);
}

Import_Form::~Import_Form()
{
    delete ui;
}

void Import_Form::recieve_all(QList<Crud *> *crudlist)
{
    crud_model->setCrudlist(crudlist);
    ui->tableView_zk->setModel(crud_model);
}
