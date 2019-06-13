#include "settings_Form.h"
#include "ui_settings_form.h"
#include <QtDebug>
#include <QSettings>
#include <QDrag>

Settings_Form::Settings_Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Settings_Form)
{
    setWindowFlags(windowFlags() | Qt::Dialog);
    setWindowModality(Qt::WindowModal);

    ui->setupUi(this);
}

Settings_Form::~Settings_Form()
{
    delete ui;
}

void Settings_Form::dragEnterEvent(QDragEnterEvent *e)
{
    //ui->checkBox->acceptDrops();
  // ->acceptProposedAction();
}

void Settings_Form::closeEvent(QCloseEvent *event)
{
    (void)event;
    delete this;
}

void Settings_Form::on_pb_save_settings_clicked()
{

    QSettings settings( "C:/Users/Vladya/Documents/qt_projects/untitled9/CONFIG/TEST.ini", QSettings::IniFormat);

   QFont *font = new QFont;
   font->setPointSize(ui->lineEdit->text().toInt());
       settings.beginGroup("FONTS");
       if( !ui->lineEdit->text().isEmpty())
        settings.setValue("SIZE", ui->lineEdit->text().toInt());
       else
        settings.setValue("SIZE", 10);

       settings.endGroup();
       settings.beginGroup("ZK_COLUMNS");
       int i =1;
    foreach (QCheckBox *cb , ui->groupBox->findChildren<QCheckBox*>())
    {
        if(cb->isChecked())
        {
            settings.setValue(QString::number(i),cb->objectName());
            i++;
        }
    }
    settings.setValue("COLUMNS_COUNT", i);
    if( !ui->lineEdit_2->text().isEmpty())
     settings.setValue("PAGE_COUNT", ui->lineEdit_2->text().toInt());
    else
     settings.setValue("PAGE_COUNT", 5);

   settings.sync();
   emit Update_main();
   delete this;
}

void Settings_Form::on_groupBox_clicked()
{
        QGroupBox *groupbox = ui->groupBox;
        groupbox->setChecked(true);
}
