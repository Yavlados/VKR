#include "db_connection.h"

void Connection::db_connect()
{
    QSqlQuery query;
    db.setHostName("localhost");
    db.setDatabaseName("postgres");
    db.setUserName("postgres");
    db.setPassword("23Xeromant23");
    db.setPort(5432);
    ok = db.open();
    if (ok == true)
        qDebug() <<"ok";
}
