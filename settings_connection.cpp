#include "settings_connection.h"
#include <QDebug>

Settings_connection *Settings_connection::_instance = nullptr;

Settings_connection::Settings_connection()
{
    Set_settings();
    Set_pg_settings();
}

void Settings_connection::Set_settings()
{
    if (!showing_cols.isEmpty())
        showing_cols.clear();

   settings = new QSettings("testing.ini",QSettings::IniFormat);
   settings->beginReadArray("COLUMNS_ARRAY");
   columns_count = settings->value("size").toInt();
   for (int i = 0; i < columns_count; i++)
   {
       settings->setArrayIndex(i);
       Columns *col = new Columns;
       col->number = settings->value("ColumnIndex").toInt()+1;
       col->column_name = settings->value("ColumnName").toString();
       showing_cols.append(col);
   }
   columns_count += 1;
   settings->endArray();
   settings->beginGroup("PAGE_COUNT");
   showing_count = settings->value("PAGE_COUNT").toInt();
   settings->endGroup();

}

void Settings_connection::Set_pg_settings()
{
    settings = new QSettings("testing.ini",QSettings::IniFormat);
    settings->beginReadArray("PG_ARRAY");

                HostName  = settings->value("HostName").toString();
                DatabaseName  = settings->value("DatabaseName").toString();
                setUserName  = settings->value("setUserName").toString();
                setPassword  = settings->value("setPassword").toString();
                Port  = settings->value("Port").toInt();
    settings->endArray();
}

Settings_connection *Settings_connection::instance()
{
    if( _instance == nullptr)
        _instance = new Settings_connection();

    return _instance;
}
