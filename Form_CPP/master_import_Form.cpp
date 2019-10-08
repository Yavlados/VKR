#include "master_import_Form.h"
#include "ui_master_import_form.h"

Master_import_form::Master_import_form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Master_import_form)
{
    ui->setupUi(this);
    ui->le_password_2->setEchoMode(QLineEdit::Password);

}

Master_import_form::~Master_import_form()
{
    delete ui;
}
void Master_import_form::on_pushButton_clicked()
{///Пытаюсь открыть зашифрованную бд
    emit TESTING_open(ui->le_file_path_2->text(), ui->le_password_2->text());
}

void Master_import_form::on_pb_directory_2_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
                       this,
                       tr("Save Document"),
                       QDir::currentPath(),
                       tr("Dump database (*.db)"), nullptr, QFileDialog::DontUseNativeDialog  );

    if( !filename.isNull() )
        ui->le_file_path_2->setText(filename);
}
