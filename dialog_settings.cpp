#include "dialog_settings.h"
#include "ui_dialog_settings.h"


#include <QSettings>

Dialog_settings::Dialog_settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_settings)
{
    ui->setupUi(this);
    QSize size = this->size();
    setFixedSize(size);
    setModal(true);
    ui->label_2->setVisible(false);
    ui->comboBox->setVisible(false);
}

Dialog_settings::~Dialog_settings()
{
    delete ui;
}


void Dialog_settings::on_groupBox_3_toggled(bool arg1)
{
    if(!arg1)
    {
        foreach(QCheckBox *cb, ui->groupBox_3->findChildren<QCheckBox*>())
        {
            cb->setCheckState(Qt::Unchecked);
        }
    } else
    {
        foreach(QCheckBox *cb, ui->groupBox_3->findChildren<QCheckBox*>())
        {
            cb->setCheckState(Qt::Checked);
        }
    }
}

void Dialog_settings::on_groupBox_4_toggled(bool arg1)
{
    if(!arg1)
    {
        foreach(QCheckBox *cb, ui->groupBox_4->findChildren<QCheckBox*>())
        {
            cb->setCheckState(Qt::Unchecked);
        }
    } else
    {
        foreach(QCheckBox *cb, ui->groupBox_4->findChildren<QCheckBox*>())
        {
            cb->setCheckState(Qt::Checked);
        }
    }
}

void Dialog_settings::on_pushButton_clicked()
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
    foreach (QCheckBox *cb , ui->groupBox_3->findChildren<QCheckBox*>("LIV_CITY"))
    {
        if(cb->isChecked())
        {
            settings->setArrayIndex(i);
            settings->setValue("ColumnName",cb->objectName());
            settings->setValue("ColumnIndex", i);
            i++;
        }
    }
    foreach (QCheckBox *cb , ui->groupBox_3->findChildren<QCheckBox*>("LIV_STREET"))
    {
        if(cb->isChecked())
        {
            settings->setArrayIndex(i);
            settings->setValue("ColumnName",cb->objectName());
            settings->setValue("ColumnIndex", i);
            i++;
        }
    }
    foreach (QCheckBox *cb , ui->groupBox_3->findChildren<QCheckBox*>("LIV_HOME"))
    {
        if(cb->isChecked())
        {
            settings->setArrayIndex(i);
            settings->setValue("ColumnName",cb->objectName());
            settings->setValue("ColumnIndex", i);
            i++;
        }
    }
    foreach (QCheckBox *cb , ui->groupBox_3->findChildren<QCheckBox*>("LIV_CORP"))
    {
        if(cb->isChecked())
        {
            settings->setArrayIndex(i);
            settings->setValue("ColumnName",cb->objectName());
            settings->setValue("ColumnIndex", i);
            i++;
        }
    }
    foreach (QCheckBox *cb , ui->groupBox_3->findChildren<QCheckBox*>("LIV_FLAT"))
    {
        if(cb->isChecked())
        {
            settings->setArrayIndex(i);
            settings->setValue("ColumnName",cb->objectName());
            settings->setValue("ColumnIndex", i);
            i++;
        }
    }
    foreach (QCheckBox *cb , ui->groupBox_4->findChildren<QCheckBox*>("REG_CITY"))
    {
        if(cb->isChecked())
        {
            settings->setArrayIndex(i);
            settings->setValue("ColumnName",cb->objectName());
            settings->setValue("ColumnIndex", i);
            i++;
        }
    }
    foreach (QCheckBox *cb , ui->groupBox_4->findChildren<QCheckBox*>("REG_STREET"))
    {
        if(cb->isChecked())
        {
            settings->setArrayIndex(i);
            settings->setValue("ColumnName",cb->objectName());
            settings->setValue("ColumnIndex", i);
            i++;
        }
    }
    foreach (QCheckBox *cb , ui->groupBox_4->findChildren<QCheckBox*>("REG_HOME"))
    {
        if(cb->isChecked())
        {
            settings->setArrayIndex(i);
            settings->setValue("ColumnName",cb->objectName());
            settings->setValue("ColumnIndex", i);
            i++;
        }
    }
    foreach (QCheckBox *cb , ui->groupBox_4->findChildren<QCheckBox*>("REG_CORP"))
    {
        if(cb->isChecked())
        {
            settings->setArrayIndex(i);
            settings->setValue("ColumnName",cb->objectName());
            settings->setValue("ColumnIndex", i);
            i++;
        }
    }
    foreach (QCheckBox *cb , ui->groupBox_4->findChildren<QCheckBox*>("REG_FLAT"))
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

   ///Настройки подключения
   settings->beginGroup("HOSTNAME");
   settings->setValue("HOSTNAME", ui->le_host->text());
   settings->endGroup();

   settings->beginGroup("DATABASE");
   settings->setValue("DATABASE", ui->le_db->text());
   settings->endGroup();

   settings->beginGroup("PORT");
   settings->setValue("PORT", ui->le_port->text());
   settings->endGroup();

   settings->beginGroup("USER");
   settings->setValue("USER", ui->le_user->text());
   settings->endGroup();

   settings->beginGroup("PASSWORD");
   settings->setValue("PASSWORD", ui->le_pass->text());
   settings->endGroup();

   settings->sync();

   Settings_connection::instance()->Set_settings();

    emit accept();
}

void Dialog_settings::on_pushButton_2_clicked()
{
    emit reject();
}

void Dialog_settings::closeEvent(QCloseEvent *event)
{
    emit reject();
}
