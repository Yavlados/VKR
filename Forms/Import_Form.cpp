#include "Import_Form.h"
#include "ui_import_form.h"
#include "an_result_Form.h"

Import_Form::Import_Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Import_Form)
{
    ui->setupUi(this);

}

Import_Form::~Import_Form()
{

}
