#include "settings_connection.h"
#include <QDebug>
#include <QCoreApplication>
#include <QFile>

Settings_connection *Settings_connection::_instance = nullptr;

Settings_connection::Settings_connection()
{
    //    Set_settings();
}

QSettings *Settings_connection::ret_settings()
{
    return settings;
}

void Settings_connection::Set_settings()
{
    if (!showing_cols.isEmpty())
        showing_cols.clear();

    QString file = QCoreApplication::applicationDirPath()+"/testing.ini";

   if (!QFile(file).exists())
        settings = new QSettings(file,QSettings::IniFormat);
   else
   {
       settings = new QSettings(file,QSettings::IniFormat);


       settings->beginReadArray("COLUMNS_ARRAY");
       columns_count = settings->value("size").toInt();
       for (int i = 0; i < columns_count; i++)
       {
           settings->setArrayIndex(i);
           Columns *col = new Columns;
           col->number = settings->value("ColumnIndex").toInt()+1;
           col->column_name = settings->value("ColumnName").toString();
           set_content(col->column_name);
           showing_cols.append(col);
       }
       columns_count += 1;
       settings->endArray();
       settings->beginGroup("PAGE_COUNT");
       showing_count = settings->value("PAGE_COUNT").toInt();
       settings->endGroup();

       settings->beginGroup("HOSTNAME");
       HostName = settings->value("HOSTNAME").toString();
       settings->endGroup();

       settings->beginGroup("DATABASE");
       DatabaseName = settings->value("DATABASE").toString();
       settings->endGroup();

       settings->beginGroup("USER");
       User = settings->value("USER").toString();
       settings->endGroup();

       settings->beginGroup("PASSWORD");
       Password = settings->value("PASSWORD").toString();
       settings->endGroup();

       settings->beginGroup("PORT");
       Port = settings->value("PORT").toInt();
       settings->endGroup();
   }

}

void Settings_connection::up_checkboxes(QCheckBox *cb)
{
    if(cb->objectName() == "ZK_ID")
        cb->setChecked(content.zk_id);
    else if (cb->objectName() == "LASTNAME")
        cb->setChecked(content.lastname);
    else if (cb->objectName() == "NAME")
        cb->setChecked(content.name);
    else if (cb->objectName() == "MID_NAME")
        cb->setChecked(content.mid_name);
    else if (cb->objectName() == "BIRTH_DATE")
        cb->setChecked(content.birth_date);
    else if (cb->objectName() == "CHECK_FOR")
        cb->setChecked(content.check_for);
    else if (cb->objectName() == "DOP_INFO")
        cb->setChecked(content.dop_info);
    else if (cb->objectName() == "REG_CITY")
        cb->setChecked(content.reg_city);
    else if (cb->objectName() == "REG_STREET")
        cb->setChecked(content.reg_street);
    else if (cb->objectName() == "REG_HOME")
        cb->setChecked(content.reg_home);
    else if (cb->objectName() == "REG_CORP")
        cb->setChecked(content.reg_corp);
    else if (cb->objectName() == "REG_FLAT")
        cb->setChecked(content.reg_flat);
    else if (cb->objectName() == "LIV_CITY")
        cb->setChecked(content.liv_city);
    else if (cb->objectName() == "LIV_STREET")
        cb->setChecked(content.liv_street);
    else if (cb->objectName() == "LIV_HOME")
        cb->setChecked(content.liv_home);
    else if (cb->objectName() == "LIV_CORP")
        cb->setChecked(content.liv_corp);
    else if (cb->objectName() == "LIV_FLAT")
        cb->setChecked(content.liv_flat);
    else if (cb->objectName() == "ADD_DATE")
        cb->setChecked(content.date_add);
    else if (cb->objectName() == "ADD_TIME")
        cb->setChecked(content.time_add);
}

void Settings_connection::set_content(QString col_name)
{
    if(col_name == "ZK_ID")
        content.zk_id = true;
    else if (col_name == "LASTNAME")
        content.lastname = true;
    else if (col_name == "NAME")
        content.name = true;
    else if (col_name == "MID_NAME")
        content.mid_name = true;
    else if (col_name == "BIRTH_DATE")
        content.birth_date = true;
    else if (col_name == "CHECK_FOR")
        content.check_for = true;
    else if (col_name == "DOP_INFO")
        content.dop_info = true;
    else if (col_name == "REG_CITY")
        content.reg_city = true;
    else if (col_name == "REG_STREET")
        content.reg_street = true;
    else if (col_name == "REG_HOME")
        content.reg_home = true;
    else if (col_name == "REG_CORP")
        content.reg_corp = true;
    else if (col_name == "REG_FLAT")
        content.reg_flat = true;
    else if (col_name == "LIV_CITY")
        content.liv_city = true;
    else if (col_name == "LIV_STREET")
        content.liv_street = true;
    else if (col_name == "LIV_HOME")
        content.liv_home = true;
    else if (col_name == "LIV_CORP")
        content.liv_corp = true;
    else if (col_name == "LIV_FLAT")
        content.liv_flat = true;
    else if (col_name == "ADD_DATE")
        content.date_add = true;
    else if (col_name == "ADD_TIME")
        content.time_add = true;
}

Settings_connection *Settings_connection::instance()
{
    if( _instance == nullptr)
        _instance = new Settings_connection();

    return _instance;
}
