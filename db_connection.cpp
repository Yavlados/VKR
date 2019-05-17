#include "db_connection.h"

db_connection *db_connection::_instance = nullptr;

db_connection::db_connection()
{
    db = QSqlDatabase::addDatabase("QPSQL");
    ok = db.isValid();
}

db_connection::~db_connection()
{
    if( db.isOpen())
        db.close();
}

db_connection *db_connection::instance()
{
    if( _instance == nullptr)
        _instance = new db_connection();

    return _instance;
}

bool db_connection::db_connect()
{
    lastError.clear();

    if( db.isOpen() )
        return true;

    db.setHostName("localhost");
    db.setDatabaseName("postgres");
    //db.setDatabaseName("zk");
    db.setUserName("postgres");
    db.setPassword("23Xeromant23");
    //db.setPassword("ipqo");
    db.setPort(5432);
    //db.setPort(5433);
    ok = db.open();
    if (ok == true)
        qDebug() <<"ok";

    return ok;
}
