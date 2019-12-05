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

Settings_connection *Settings_connection::instance()
{
    if( _instance == nullptr)
        _instance = new Settings_connection();

    return _instance;
}
