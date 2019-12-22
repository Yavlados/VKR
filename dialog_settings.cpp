#include "dialog_settings.h"
#include "ui_dialog_settings.h"


#include <QSettings>
#include <QFile>
#include <qDebug>


Dialog_settings::Dialog_settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_settings)
{
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(0);
    on_tabWidget_currentChanged(0);
    QSize size = this->size();
    setFixedSize(size);
    setModal(true);
    ui->label_2->setVisible(false);
    ui->comboBox->setVisible(false);
    Settings_connection::instance()->Set_settings();
    set_settings_from_ini();
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
    settings = Settings_connection::instance()->ret_settings();
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

    if(ui->checkBox->checkState() == Qt::Checked)
    {
        settings->beginGroup("PASSWORD");
        settings->setValue("PASSWORD", ui->le_pass->text());
        settings->endGroup();
    }

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
    (void)event;
    emit reject();
}

void Dialog_settings::set_settings_from_ini()
{
    if(Settings_connection::instance()->Port != 0)
    {
        ui->le_port->setText(QString::number(Settings_connection::instance()->Port));
    }
    if(!Settings_connection::instance()->HostName.isEmpty())
    {
        ui->le_host->setText(Settings_connection::instance()->HostName);
    }
    if(!Settings_connection::instance()->DatabaseName.isEmpty())
    {
        ui->le_db->setText(Settings_connection::instance()->DatabaseName);
    }
    if(!Settings_connection::instance()->User.isEmpty())
    {
        ui->le_user->setText(Settings_connection::instance()->User);
    }
    if(!Settings_connection::instance()->Password.isEmpty())
    {
        ui->le_pass->setText(Settings_connection::instance()->Password);

    }
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


void Dialog_settings::set_tab_orders_pg_0()
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

void Dialog_settings::set_tab_orders_pg_1()
{
    ui->le_host->setFocus();
    setTabOrder(ui->le_host, ui->le_db);
    setTabOrder(ui->le_db, ui->le_port);
    setTabOrder(ui->le_port, ui->le_user);
    setTabOrder(ui->le_user, ui->le_pass);
   setTabOrder(ui->le_pass, ui->checkBox);

}

void Dialog_settings::on_tabWidget_currentChanged(int index)
{
    if(index == 1)
    {
        set_tab_orders_pg_1();
    }else if(index == 0)
    {
        set_tab_orders_pg_0();
    }
}

void Dialog_settings::keyPressEvent(QKeyEvent *event)
{

        switch(event->key())
     {
      case Qt::Key::Key_Enter:
         on_pushButton_clicked();
         return;
     case Qt::Key::Key_Escape:
            reject();
         return;
     }
}
