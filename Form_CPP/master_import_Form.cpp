#include "master_import_Form.h"
#include "ui_master_import_form.h"

Master_import_form::Master_import_form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Master_import_form)
{
//    parent_win = qobject_cast<parent>();
//    parent_win = parent->qt;
    ui->setupUi(this);
    ui->le_password_2->setEchoMode(QLineEdit::Password);
    actual_size = this->size();
}

Master_import_form::~Master_import_form()
{
    delete ui;
}
void Master_import_form::on_pushButton_clicked()
{///Пытаюсь открыть зашифрованную бд
    emit TESTING_open(ui->le_file_path_2->text(), ui->le_password_2->text(), folder, of_t );
}

void Master_import_form::on_pb_directory_2_clicked()
{
    QMessageBox msgbx;
    msgbx.setWindowTitle("Сделайте выбор");
    msgbx.setText("Выберите, откуда будет производиться импорт");
    msgbx.setStandardButtons(QMessageBox::Yes | QMessageBox::Ok | QMessageBox::Cancel);

    msgbx.setButtonText(QMessageBox::Yes, "Импорт служебных телефонов");
    msgbx.setButtonText(QMessageBox::Ok, "Импорт из старой ЗК");
    msgbx.setButtonText(QMessageBox::Cancel, "Импорт ЗК");

    QString filename;
    int ret = msgbx.exec();
    
    if(ret == QMessageBox::Ok)
    {
                QMessageBox temp;
                temp.setWindowTitle("Сделайте выбор");
                temp.setText("Выберите, откуда будет производиться импорт");
                temp.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
                temp.setButtonText(QMessageBox::Ok, "Импорт из папки");
                temp.setButtonText(QMessageBox::Cancel, "Импорт выбранных файлов");
                int s = temp.exec();
                switch(s)
                {
                //Ипорт из ПАПКИ!!!
                    case QMessageBox::Ok :
                    if(file_path == nullptr)
                    {
                        filename = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                                       "/home",
                                                                       QFileDialog::ShowDirsOnly
                                                                       | QFileDialog::DontResolveSymlinks);
                        file_path = filename;
                        if( !filename.isNull() )
                        {
                            folder = true;
                            ui->le_file_path_2->setText(filename);
                        }
                        return;
                    }
                        else
                    {
                        filename = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                                     "/home",
                                                                     QFileDialog::ShowDirsOnly
                                                                     | QFileDialog::DontResolveSymlinks);

                        file_path = filename;
                        if( !filename.isNull() )
                        {
                            folder = true;
                            ui->le_file_path_2->setText(filename);
                        }
                        return;
                    }
                case QMessageBox::Cancel:
                    if(file_path == nullptr)
                    {
                        filename = QFileDialog::getOpenFileName(
                                    this,
                                    tr("Save Document"),
                                    QDir::currentPath(),
                                    tr("Dump database (*)"), nullptr, QFileDialog::DontUseNativeDialog  );
                        int x = filename.lastIndexOf("/");
                        file_path = filename.left(x);
                        if( !filename.isNull() )
                            ui->le_file_path_2->setText(filename);
                        return;
                    }
                        else
                    {
                        filename = QFileDialog::getOpenFileName(
                                    this,
                                    tr("Save Document"),
                                    file_path,
                                    tr("Dump database (*)"), nullptr, QFileDialog::DontUseNativeDialog  );
                        int x = filename.lastIndexOf("/");
                        file_path = filename.left(x);

                        if( !filename.isNull() )
                        {
                            ui->le_file_path_2->setText(filename);
                        }
                        return;
                    }
                default:
                    return;
                }

    }
    else
    if(ret == QMessageBox::Cancel || ret == QMessageBox::Yes)
        {
          if(file_path == nullptr)
          {
              filename = QFileDialog::getOpenFileName(
                          this,
                          tr("Save Document"),
                          QDir::currentPath(),
                          tr("Dump database (*.db)"), nullptr, QFileDialog::DontUseNativeDialog  );
              int x = filename.lastIndexOf("/");
              file_path = filename.left(x);
              if( !filename.isNull() )
                  ui->le_file_path_2->setText(filename);
              if(ret == QMessageBox::Yes)
                  of_t = true;
              return;
          }
          else
          {

              filename = QFileDialog::getOpenFileName(
                          this,
                          tr("Save Document"),
                          file_path,
                          tr("Dump database (*.db)"), nullptr, QFileDialog::DontUseNativeDialog  );
              int x = filename.lastIndexOf("/");
              file_path = filename.left(x);
              if( !filename.isNull() )
                  ui->le_file_path_2->setText(filename);
              if(ret == QMessageBox::Yes)
                  of_t = true;
              return;
          }
    }

}
