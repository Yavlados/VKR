#ifndef DB_CONNECTION_H
#define DB_CONNECTION_H
#include <QSqlQuery>
#include <QDebug>

/* в перспективе сделать класс подключения, если появится необходимость
        в диалоговом окне подключения. Пока реализована функция*/

class Connection
{
public:
    void db_connect();
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
private:
    bool ok;
};


#endif // DB_CONNECTION_H

