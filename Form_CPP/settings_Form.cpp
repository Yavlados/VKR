#include "settings_Form.h"
#include "ui_settings_form.h"

#include <QtDebug>
#include <QDrag>

Settings_Form::Settings_Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Settings_Form)
{
    setWindowFlags(windowFlags() | Qt::Dialog);
    setWindowModality(Qt::WindowModal);

    //up_check_boxes();

    ui->setupUi(this);
}

Settings_Form::~Settings_Form()
{
    delete ui;
}

void Settings_Form::dragEnterEvent(QDragEnterEvent *e)
{
}

void Settings_Form::up_check_boxes()
{

}

void Settings_Form::closeEvent(QCloseEvent *event)
{
    (void)event;
    delete this;
}

void Settings_Form::on_pb_save_settings_clicked()
{
    QSettings *settings = new QSettings("testing.ini",QSettings::IniFormat);;
    int i = 0;
    settings->beginWriteArray("COLUMNS_ARRAY");
    settings->remove("");
    settings->sync();
    settings->endArray();
    settings->beginWriteArray("COLUMNS_ARRAY");
    foreach (QCheckBox *cb , ui->groupBox->findChildren<QCheckBox*>("ZK_ID"))
    {
        if(cb->isChecked())
        {
            settings->setArrayIndex(i);
            settings->setValue("ColumnName",cb->objectName());
            settings->setValue("ColumnIndex", i);
            i++;
        }
    }
    foreach (QCheckBox *cb , ui->groupBox->findChildren<QCheckBox*>("LASTNAME"))
    {
        if(cb->isChecked())
        {
            settings->setArrayIndex(i);
            settings->setValue("ColumnName",cb->objectName());
            settings->setValue("ColumnIndex", i);
            i++;
        }
    }
    foreach (QCheckBox *cb , ui->groupBox->findChildren<QCheckBox*>("NAME"))
    {
        if(cb->isChecked())
        {
            settings->setArrayIndex(i);
            settings->setValue("ColumnName",cb->objectName());
            settings->setValue("ColumnIndex", i);
            i++;
        }
    }
    foreach (QCheckBox *cb , ui->groupBox->findChildren<QCheckBox*>("MID_NAME"))
    {
        if(cb->isChecked())
        {
            settings->setArrayIndex(i);
            settings->setValue("ColumnName",cb->objectName());
            settings->setValue("ColumnIndex", i);
            i++;
        }
    }
    foreach (QCheckBox *cb , ui->groupBox->findChildren<QCheckBox*>("BIRTH_DATE"))
    {
        if(cb->isChecked())
        {
            settings->setArrayIndex(i);
            settings->setValue("ColumnName",cb->objectName());
            settings->setValue("ColumnIndex", i);
            i++;
        }
    }
    foreach (QCheckBox *cb , ui->groupBox->findChildren<QCheckBox*>("ADD_DATE"))
    {
        if(cb->isChecked())
        {
            settings->setArrayIndex(i);
            settings->setValue("ColumnName",cb->objectName());
            settings->setValue("ColumnIndex", i);
            i++;
        }
    }
    foreach (QCheckBox *cb , ui->groupBox->findChildren<QCheckBox*>("ADD_TIME"))
    {
        if(cb->isChecked())
        {
            settings->setArrayIndex(i);
            settings->setValue("ColumnName",cb->objectName());
            settings->setValue("ColumnIndex", i);
            i++;
        }
    }
    foreach (QCheckBox *cb , ui->groupBox->findChildren<QCheckBox*>("CHECK_FOR"))
    {
        if(cb->isChecked())
        {
            settings->setArrayIndex(i);
            settings->setValue("ColumnName",cb->objectName());
            settings->setValue("ColumnIndex", i);
            i++;
        }
    }
    foreach (QCheckBox *cb , ui->groupBox->findChildren<QCheckBox*>("DOP_INFO"))
    {
        if(cb->isChecked())
        {
            settings->setArrayIndex(i);
            settings->setValue("ColumnName",cb->objectName());
            settings->setValue("ColumnIndex", i);
            i++;
        }
    }
    foreach (QCheckBox *cb , ui->groupBox->findChildren<QCheckBox*>("LIV_CITY"))
    {
        if(cb->isChecked())
        {
            settings->setArrayIndex(i);
            settings->setValue("ColumnName",cb->objectName());
            settings->setValue("ColumnIndex", i);
            i++;
        }
    }
    foreach (QCheckBox *cb , ui->groupBox->findChildren<QCheckBox*>("LIV_STREET"))
    {
        if(cb->isChecked())
        {
            settings->setArrayIndex(i);
            settings->setValue("ColumnName",cb->objectName());
            settings->setValue("ColumnIndex", i);
            i++;
        }
    }
    foreach (QCheckBox *cb , ui->groupBox->findChildren<QCheckBox*>("LIV_HOME"))
    {
        if(cb->isChecked())
        {
            settings->setArrayIndex(i);
            settings->setValue("ColumnName",cb->objectName());
            settings->setValue("ColumnIndex", i);
            i++;
        }
    }
    foreach (QCheckBox *cb , ui->groupBox->findChildren<QCheckBox*>("LIV_CORP"))
    {
        if(cb->isChecked())
        {
            settings->setArrayIndex(i);
            settings->setValue("ColumnName",cb->objectName());
            settings->setValue("ColumnIndex", i);
            i++;
        }
    }
    foreach (QCheckBox *cb , ui->groupBox->findChildren<QCheckBox*>("LIV_FLAT"))
    {
        if(cb->isChecked())
        {
            settings->setArrayIndex(i);
            settings->setValue("ColumnName",cb->objectName());
            settings->setValue("ColumnIndex", i);
            i++;
        }
    }
    foreach (QCheckBox *cb , ui->groupBox->findChildren<QCheckBox*>("REG_CITY"))
    {
        if(cb->isChecked())
        {
            settings->setArrayIndex(i);
            settings->setValue("ColumnName",cb->objectName());
            settings->setValue("ColumnIndex", i);
            i++;
        }
    }
    foreach (QCheckBox *cb , ui->groupBox->findChildren<QCheckBox*>("REG_STREET"))
    {
        if(cb->isChecked())
        {
            settings->setArrayIndex(i);
            settings->setValue("ColumnName",cb->objectName());
            settings->setValue("ColumnIndex", i);
            i++;
        }
    }
    foreach (QCheckBox *cb , ui->groupBox->findChildren<QCheckBox*>("REG_HOME"))
    {
        if(cb->isChecked())
        {
            settings->setArrayIndex(i);
            settings->setValue("ColumnName",cb->objectName());
            settings->setValue("ColumnIndex", i);
            i++;
        }
    }
    foreach (QCheckBox *cb , ui->groupBox->findChildren<QCheckBox*>("REG_CORP"))
    {
        if(cb->isChecked())
        {
            settings->setArrayIndex(i);
            settings->setValue("ColumnName",cb->objectName());
            settings->setValue("ColumnIndex", i);
            i++;
        }
    }
    foreach (QCheckBox *cb , ui->groupBox->findChildren<QCheckBox*>("REG_FLAT"))
    {
        if(cb->isChecked())
        {
            settings->setArrayIndex(i);
            settings->setValue("ColumnName",cb->objectName());
            settings->setValue("ColumnIndex", i);
            i++;
        }
    }

   settings->endArray();
   settings->beginGroup("PAGE_COUNT");
   settings->setValue("PAGE_COUNT", ui->lineEdit_2->text().toInt());
   settings->endGroup();
   settings->sync();


   Settings_connection::instance()->Set_settings();
   emit Update_main();
   delete this;
}

void Settings_Form::on_groupBox_clicked()
{
        QGroupBox *groupbox = ui->groupBox;
        groupbox->setChecked(true);
}
