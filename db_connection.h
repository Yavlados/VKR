#ifndef DB_CONNECTION_H
#define DB_CONNECTION_H

#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include "settings_connection.h"
#include "dialog_settings.h"

/* в перспективе сделать класс подключения, если появится необходимость
        в диалоговом окне подключения. Пока реализована функция*/

enum SqlType {SQLlitechipher = 0,
              PSQLtype = 1,
              SQLliteType = 2};
/**
 * \file db_connection.h
 * \brief Класс подключения к базе данных
*/
class db_connection     ///Класс подключения к базе данных
{
public:
    /// Доступ к статической переменной класса
    static db_connection *instance();

    ~db_connection(); ///Деструктор класса

    bool db_connect();

    QSqlDatabase &db();
    bool ok;
    void set_Sql_type(SqlType t);
    QString lastError;
    QSqlError error; ///Необходим номер ошибки для определения ошибки уникальности
    QString error_tel_num; ///номер телефона с ошибкой

private:
    /// Скрытый конструктор
    db_connection();  /// Единственный конструктор без аргументов

    /// Скрытая перемнная - единственный экземпляр класса
    static db_connection *_instance;
    QSqlDatabase _dbpg;
    QSqlDatabase _dbsql;
    QSqlDatabase _dbsqlchipher;
    SqlType type;
};



#endif // DB_CONNECTION_H

