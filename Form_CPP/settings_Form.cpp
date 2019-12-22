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
    set_tab_orders();
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
            Settings_connection::instance()->instance()->content.zk_id = true;
            i++;
        }else
            Settings_connection::instance()->instance()->content.zk_id = false;
    }
    foreach (QCheckBox *cb , ui->groupBox->findChildren<QCheckBox*>("LASTNAME"))
    {
        if(cb->isChecked())
        {
            settings->setArrayIndex(i);
            settings->setValue("ColumnName",cb->objectName());
            settings->setValue("ColumnIndex", i);
            Settings_connection::instance()->instance()->content.lastname = true;
            i++;
        }else
            Settings_connection::instance()->instance()->content.lastname = false;
    }
    foreach (QCheckBox *cb , ui->groupBox->findChildren<QCheckBox*>("NAME"))
    {
        if(cb->isChecked())
        {
            settings->setArrayIndex(i);
            settings->setValue("ColumnName",cb->objectName());
            settings->setValue("ColumnIndex", i);
            Settings_connection::instance()->instance()->content.name = true;
            i++;
        }else
            Settings_connection::instance()->instance()->content.name = false;
    }
    foreach (QCheckBox *cb , ui->groupBox->findChildren<QCheckBox*>("MID_NAME"))
    {
        if(cb->isChecked())
        {
            settings->setArrayIndex(i);
            settings->setValue("ColumnName",cb->objectName());
            settings->setValue("ColumnIndex", i);
            Settings_connection::instance()->instance()->content.mid_name = true;
            i++;
        }else
            Settings_connection::instance()->instance()->content.mid_name = false;
    }
    foreach (QCheckBox *cb , ui->groupBox->findChildren<QCheckBox*>("BIRTH_DATE"))
    {
        if(cb->isChecked())
        {
            settings->setArrayIndex(i);
            settings->setValue("ColumnName",cb->objectName());
            settings->setValue("ColumnIndex", i);
            Settings_connection::instance()->instance()->content.birth_date = true;
            i++;
        }else
            Settings_connection::instance()->instance()->content.birth_date = false;
    }
    foreach (QCheckBox *cb , ui->groupBox->findChildren<QCheckBox*>("ADD_DATE"))
    {
        if(cb->isChecked())
        {
            settings->setArrayIndex(i);
            settings->setValue("ColumnName",cb->objectName());
            settings->setValue("ColumnIndex", i);
            Settings_connection::instance()->instance()->content.date_add = true;
            i++;
        }else
            Settings_connection::instance()->instance()->content.date_add = false;
    }
    foreach (QCheckBox *cb , ui->groupBox->findChildren<QCheckBox*>("ADD_TIME"))
    {
        if(cb->isChecked())
        {
            settings->setArrayIndex(i);
            settings->setValue("ColumnName",cb->objectName());
            settings->setValue("ColumnIndex", i);
            Settings_connection::instance()->instance()->content.time_add = true;
            i++;
        }else
            Settings_connection::instance()->instance()->content.time_add = false;
    }
    foreach (QCheckBox *cb , ui->groupBox->findChildren<QCheckBox*>("CHECK_FOR"))
    {
        if(cb->isChecked())
        {
            settings->setArrayIndex(i);
            settings->setValue("ColumnName",cb->objectName());
            settings->setValue("ColumnIndex", i);
            Settings_connection::instance()->instance()->content.check_for = true;
            i++;
        }else
            Settings_connection::instance()->instance()->content.check_for = false;
    }
    foreach (QCheckBox *cb , ui->groupBox->findChildren<QCheckBox*>("DOP_INFO"))
    {
        if(cb->isChecked())
        {
            settings->setArrayIndex(i);
            settings->setValue("ColumnName",cb->objectName());
            settings->setValue("ColumnIndex", i);
            Settings_connection::instance()->instance()->content.dop_info = true;
            i++;
        }else
            Settings_connection::instance()->instance()->content.dop_info = false;
    }
    foreach (QCheckBox *cb , ui->groupBox_3->findChildren<QCheckBox*>("LIV_CITY"))
    {
        if(cb->isChecked())
        {
            settings->setArrayIndex(i);
            settings->setValue("ColumnName",cb->objectName());
            settings->setValue("ColumnIndex", i);
            Settings_connection::instance()->instance()->content.liv_city = true;
            i++;
        }else
            Settings_connection::instance()->instance()->content.liv_city = false;
    }
    foreach (QCheckBox *cb , ui->groupBox_3->findChildren<QCheckBox*>("LIV_STREET"))
    {
        if(cb->isChecked())
        {
            settings->setArrayIndex(i);
            settings->setValue("ColumnName",cb->objectName());
            settings->setValue("ColumnIndex", i);
            Settings_connection::instance()->instance()->content.liv_street = true;
            i++;
        }else
            Settings_connection::instance()->instance()->content.liv_street = false;
    }
    foreach (QCheckBox *cb , ui->groupBox_3->findChildren<QCheckBox*>("LIV_HOME"))
    {
        if(cb->isChecked())
        {
            settings->setArrayIndex(i);
            settings->setValue("ColumnName",cb->objectName());
            settings->setValue("ColumnIndex", i);
            Settings_connection::instance()->instance()->content.liv_home = true;
            i++;
        }else
            Settings_connection::instance()->instance()->content.liv_home = false;
    }
    foreach (QCheckBox *cb , ui->groupBox_3->findChildren<QCheckBox*>("LIV_CORP"))
    {
        if(cb->isChecked())
        {
            settings->setArrayIndex(i);
            settings->setValue("ColumnName",cb->objectName());
            settings->setValue("ColumnIndex", i);
            Settings_connection::instance()->instance()->content.liv_corp = true;
            i++;
        }else
            Settings_connection::instance()->instance()->content.liv_corp = false;
    }
    foreach (QCheckBox *cb , ui->groupBox_3->findChildren<QCheckBox*>("LIV_FLAT"))
    {
        if(cb->isChecked())
        {
            settings->setArrayIndex(i);
            settings->setValue("ColumnName",cb->objectName());
            settings->setValue("ColumnIndex", i);
            Settings_connection::instance()->instance()->content.liv_flat = true;
            i++;
        }else
            Settings_connection::instance()->instance()->content.liv_flat = false;
    }
    foreach (QCheckBox *cb , ui->groupBox_4->findChildren<QCheckBox*>("REG_CITY"))
    {
        if(cb->isChecked())
        {
            settings->setArrayIndex(i);
            settings->setValue("ColumnName",cb->objectName());
            settings->setValue("ColumnIndex", i);
            Settings_connection::instance()->instance()->content.reg_city = true;
            i++;
        }else
            Settings_connection::instance()->instance()->content.reg_city = false;
    }
    foreach (QCheckBox *cb , ui->groupBox_4->findChildren<QCheckBox*>("REG_STREET"))
    {
        if(cb->isChecked())
        {
            settings->setArrayIndex(i);
            settings->setValue("ColumnName",cb->objectName());
            settings->setValue("ColumnIndex", i);
            Settings_connection::instance()->instance()->content.reg_street = true;
            i++;
        }else
            Settings_connection::instance()->instance()->content.reg_street = false;
    }
    foreach (QCheckBox *cb , ui->groupBox_4->findChildren<QCheckBox*>("REG_HOME"))
    {
        if(cb->isChecked())
        {
            settings->setArrayIndex(i);
            settings->setValue("ColumnName",cb->objectName());
            settings->setValue("ColumnIndex", i);
            Settings_connection::instance()->instance()->content.reg_home = true;
            i++;
        }else
            Settings_connection::instance()->instance()->content.reg_home = false;
    }
    foreach (QCheckBox *cb , ui->groupBox_4->findChildren<QCheckBox*>("REG_CORP"))
    {
        if(cb->isChecked())
        {
            settings->setArrayIndex(i);
            settings->setValue("ColumnName",cb->objectName());
            settings->setValue("ColumnIndex", i);
            Settings_connection::instance()->instance()->content.reg_corp = true;
            i++;
        }else
            Settings_connection::instance()->instance()->content.reg_corp = false;
    }
    foreach (QCheckBox *cb , ui->groupBox_4->findChildren<QCheckBox*>("REG_FLAT"))
    {
        if(cb->isChecked())
        {
            settings->setArrayIndex(i);
            settings->setValue("ColumnName",cb->objectName());
            settings->setValue("ColumnIndex", i);
            Settings_connection::instance()->instance()->content.reg_flat = true;
            i++;
        }else
            Settings_connection::instance()->instance()->content.reg_flat = false;
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
    Settings_connection::instance()->up_checkboxes(ui->ZK_ID);
    Settings_connection::instance()->up_checkboxes(ui->LASTNAME);
    Settings_connection::instance()->up_checkboxes(ui->NAME);
    Settings_connection::instance()->up_checkboxes(ui->MID_NAME);
    Settings_connection::instance()->up_checkboxes(ui->BIRTH_DATE);
    Settings_connection::instance()->up_checkboxes(ui->CHECK_FOR);
    Settings_connection::instance()->up_checkboxes(ui->DOP_INFO);
    Settings_connection::instance()->up_checkboxes(ui->REG_CITY);
    Settings_connection::instance()->up_checkboxes(ui->REG_STREET);
    Settings_connection::instance()->up_checkboxes(ui->REG_HOME);
    Settings_connection::instance()->up_checkboxes(ui->REG_CORP);
    Settings_connection::instance()->up_checkboxes(ui->REG_FLAT);
    Settings_connection::instance()->up_checkboxes(ui->LIV_CITY);
    Settings_connection::instance()->up_checkboxes(ui->LIV_STREET);
    Settings_connection::instance()->up_checkboxes(ui->LIV_HOME);
    Settings_connection::instance()->up_checkboxes(ui->LIV_CORP);
    Settings_connection::instance()->up_checkboxes(ui->LIV_FLAT);
    Settings_connection::instance()->up_checkboxes(ui->ADD_DATE);
    Settings_connection::instance()->up_checkboxes(ui->ADD_TIME);

    if(ui->REG_CITY->isChecked() == false && ui->REG_STREET->isChecked() == false &&
            ui->REG_HOME->isChecked() == false && ui->REG_CORP->isChecked() == false &&
            ui->REG_FLAT->isChecked() == false)
    {
        ui->groupBox_4->setChecked(false);
    }
    if(ui->LIV_CITY->isChecked() == false && ui->LIV_STREET->isChecked() == false &&
            ui->LIV_HOME->isChecked() == false && ui->LIV_CORP->isChecked() == false &&
            ui->LIV_FLAT->isChecked() == false)
    {
        ui->groupBox_3->setChecked(false);
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

void Settings_Form::keyPressEvent(QKeyEvent *event)
{
        switch(event->key())
     {
      case Qt::Key::Key_Enter:
         on_pb_save_settings_clicked();
         return;
     case Qt::Key::Key_Escape:
            on_pushButton_clicked();
            return;
     }
}

void Settings_Form::set_tab_orders()
{
    ui->ZK_ID->setFocus();
    setTabOrder(ui->ZK_ID, ui->LASTNAME);
    setTabOrder(ui->LASTNAME, ui->NAME);
    setTabOrder(ui->NAME, ui->MID_NAME);
    setTabOrder(ui->MID_NAME, ui->BIRTH_DATE);
    setTabOrder(ui->BIRTH_DATE, ui->CHECK_FOR);
    setTabOrder(ui->CHECK_FOR, ui->DOP_INFO);
    setTabOrder(ui->DOP_INFO, ui->ADD_DATE);
    setTabOrder(ui->ADD_DATE, ui->ADD_TIME);
    setTabOrder(ui->ADD_TIME, ui->groupBox_3);
     if(ui->groupBox_3->isChecked())
     {
         setTabOrder(ui->groupBox_3, ui->LIV_CITY);
         setTabOrder(ui->LIV_CITY, ui->LIV_STREET);
         setTabOrder(ui->LIV_STREET, ui->LIV_HOME);
         setTabOrder(ui->LIV_HOME, ui->LIV_CORP);
         setTabOrder(ui->LIV_CORP, ui->LIV_FLAT);
         setTabOrder(ui->LIV_CORP, ui->LIV_FLAT);
         setTabOrder(ui->LIV_FLAT, ui->groupBox_4);
     }
     if(ui->groupBox_4->isChecked())
     {
         setTabOrder(ui->groupBox_4, ui->REG_CITY);
         setTabOrder(ui->REG_CITY, ui->REG_STREET);
         setTabOrder(ui->REG_STREET, ui->REG_HOME);
         setTabOrder(ui->REG_HOME, ui->REG_CORP);
         setTabOrder(ui->REG_CORP, ui->REG_FLAT);
         setTabOrder(ui->REG_CORP, ui->REG_FLAT);
     }
}
