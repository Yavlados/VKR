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
    set_settings_from_ini();
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
    QString file = QCoreApplication::applicationDirPath()+"/testing.ini";

    QSettings *settings = new QSettings(file,QSettings::IniFormat);;
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

void Settings_Form::on_pushButton_clicked()
{
    QCloseEvent *event = nullptr;
    closeEvent(event);
}

void Settings_Form::set_settings_from_ini()
{
    if(Settings_connection::instance()->showing_count != 0)
    {
        ui->lineEdit_2->setText(QString::number(Settings_connection::instance()->showing_count));
    }
}

void Settings_Form::on_groupBox_3_toggled(bool arg1)
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

void Settings_Form::on_groupBox_4_toggled(bool arg1)
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
