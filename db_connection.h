#ifndef DB_CONNECTION_H
#define DB_CONNECTION_H

#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
/* в перспективе сделать класс подключения, если появится необходимость
        в диалоговом окне подключения. Пока реализована функция*/

class db_connection
{
public:
    /// Доступ к статической переменной класса
    static db_connection *instance();

    ~db_connection(); ///Деструктор класса

    bool db_connect();

    QSqlDatabase db;
    bool ok;

    QString lastError;
    QSqlError error; ///Необходим номер ошибки для определения ошибки уникальности
    QString error_tel_num; ///номер телефона с ошибкой

private:
    /// Скрытый конструктор
    db_connection();  /// Единственный конструктор без аргументов

    /// Скрытая перемнная - единственный экземпляр класса
    static db_connection *_instance;
};



#endif // DB_CONNECTION_H

