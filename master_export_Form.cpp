#include "master_export_Form.h"
#include "ui_master_export_form.h"

master_export_Form::master_export_Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::master_export_Form)
{
    ui->setupUi(this);
    on_cb_zk_clicked();
}

master_export_Form::~master_export_Form()
{
    delete ui;
}

void master_export_Form::on_cb_zk_clicked()
{
    if(ui->cb_zk->isChecked())
     {
        ui->rb_check->setVisible(true);
        ui->rb_check_all->setVisible(true);
    }
    else
    {
        ui->rb_check->setVisible(false);
        ui->rb_check_all->setVisible(false);
    }
}

void master_export_Form::on_rb_check_clicked()
{
  if(!ui->rb_check_all->isChecked())
        emit rb_zk_clicked();
}

void master_export_Form::on_rb_check_all_clicked()
{
    if(!ui->rb_check->isChecked())
        emit rb_check_all();
}

void master_export_Form::on_pb_directory_clicked()
{
    QString filename = QFileDialog::getSaveFileName(
                       this,
                       tr("Save Document"),
                       QDir::currentPath(),
                       tr("Dump database (*.db)") );

    if( !filename.isNull() )
        ui->le_file_path->setText(filename);
}

void master_export_Form::on_pb_Export_clicked()
{
    emit TESTING_export(ui->le_file_path->text(), ui->le_password->text());
}

void master_export_Form::on_pushButton_clicked()
{///Пытаюсь открыть зашифрованную бд
    emit TESTING_open(ui->le_file_path->text(), ui->le_password->text());

}
