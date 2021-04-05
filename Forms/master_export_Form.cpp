#include "master_export_Form.h"
#include "ui_master_export_form.h"
#include "popup.h"

#include <QLineEdit>

Master_export_Form::Master_export_Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::master_export_Form)
{
    ui->setupUi(this);

    this->sr = new Search;
    this->p_b_counter = 0;
    this->file_path = "";

    ui->le_password->setEchoMode(QLineEdit::Password);
    on_cb_set_password_clicked();
    this->actual_size = this->size();
}

Master_export_Form::~Master_export_Form()
{
    delete ui;
}

void Master_export_Form::on_cb_zk_clicked()
{
    ui->rb_check->setVisible(true);
}
void Master_export_Form::on_cb_off_tel_clicked()
{
    ui->rb_check->setVisible(false);
}

ExportType Master_export_Form::getExportType()
{
    bool isEventsChecked = ui->cb_zk->isChecked();
    bool isOfficialChecked = ui->cb_off_tel->isChecked();
    bool isPasswordChecked = ui->cb_set_password->isChecked();

    if(isPasswordChecked){
        if(isEventsChecked) return eventsPassword;
        else if(isOfficialChecked) return officialPassword;
    } else{
        if(isEventsChecked) return events;
        else if(isOfficialChecked) return official;
    }
}

void Master_export_Form::on_rb_check_clicked()
{
   emit rb_zk_clicked();
}

void Master_export_Form::on_pb_directory_clicked()
{
    QString filename ;
    if(file_path == 0)
        filename = QFileDialog::getSaveFileName(
                           this,
                           tr("Save Document"),
                           QDir::currentPath(),
                           tr("*.nbd; *.nbds; *.nbod; *.nbods") );
    else
        filename = QFileDialog::getSaveFileName(
                           this,
                           tr("Save Document"),
                           file_path,
                           tr("*.nbd; *.nbds; *.nbod; *.nbods") );

    int x = filename.lastIndexOf("/");
    file_path = filename.left(x);
    if( !filename.isNull() )
        ui->le_file_path->setText(filename);
}

void Master_export_Form::on_pb_Export_clicked()
{
    // preparing encryption tool
    QString key;
    if(ui->cb_set_password->checkState())
         key = Util::instance()->convertKey(ui->le_password->text());
    else
         key = "12345";
    SimpleCrypt crypt(key.toLongLong());
    ExportType exportType = this->getExportType();

    emit prepareExport(crypt, exportType, ui->le_file_path->text());
}

void Master_export_Form::on_cb_set_password_clicked()
{
    if(ui->cb_set_password->isChecked())
     {
        ui->label->setVisible(true);
        ui->le_password->setVisible(true);
    }
    else
    {
        ui->label->setVisible(false);
        ui->le_password->setVisible(false);
    }
}

void Master_export_Form::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
 {
  case Qt::Key_Enter:
        on_pb_Export_clicked();
     return;
 case Qt::Key_Escape:
        emit closeThis(this->objectName());
     return;
    case Qt::Key_F1:
      PopUp::instance()->setPopupText("<h2 align=\"middle\">Навигация в окне мастера экспорта</h2>"
                                      "<p><b>\"ENTER\"</b> для начала экспорта</p>"
                                      "<p><b>\"ESC\"</b> для закрытия окна мастера экспорта</p>", rightMenu);
 }
}

void Master_export_Form::focus_on_widget()
{
    ui->pb_directory->setFocus();
    set_tab_orders();
}

void Master_export_Form::set_tab_orders()
{
    ui->hided_le->setFocusProxy(ui->pb_directory);
    setTabOrder(  ui->pb_directory, ui->cb_off_tel);
     setTabOrder(  ui->cb_off_tel, ui->cb_zk);
     setTabOrder(  ui->cb_zk, ui->rb_check);
     setTabOrder(   ui->rb_check, ui->cb_set_password);
     setTabOrder(  ui->cb_set_password, ui->le_password);
     setTabOrder(  ui->le_password, ui->pb_Export);
     setTabOrder(  ui->pb_Export,  ui->hided_le);
}

