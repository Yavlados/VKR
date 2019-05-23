#include "db_connection.h"

db_connection *db_connection::_instance = nullptr;

db_connection::db_connection()
{
    type =PSQLtype;
    _dbpg = QSqlDatabase::addDatabase("QPSQL","conPQ");
    ok = _dbpg.isValid();
    _dbsql = QSqlDatabase::addDatabase("SQLITECIPHER","conql");

}

db_connection::~db_connection()
{
    if( db().isOpen())
        db().close();
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
   // bool ok;

    qDebug()<<"db_con" << db().connectionName();
    if( db().isOpen() )
        return true;
    switch (type)
    {
        case (PSQLtype):
        _dbpg.setHostName("localhost");
        _dbpg.setDatabaseName("postgres");
        //db().setDatabaseName("zk");
       _dbpg.setUserName("postgres");
        _dbpg.setPassword("23Xeromant23");
        //db().setPassword("ipqo");
        _dbpg.setPort(5432);
        //db().setPort(5433);
        ok = _dbpg.open();
        if (ok == true)
            qDebug() <<"ok";
        else
            qDebug() << _dbpg.lastError().text();
        break;
    case (SQLliteType):
        //_dbsql.addDatabase("QSQLITE","conql");
        //_dbsql.setDatabaseName("E:\\t1.db");
        ok = _dbsql.open();
        if (ok == true)
            qDebug() <<"ok";
        else
            qDebug() << _dbsql.lastError().text();
        break;
    }

    return ok;
}

QSqlDatabase &db_connection::db()
{
    switch (type)
    {
        case (PSQLtype):
         return _dbpg;
    case (SQLliteType):
        return _dbsql;
    }
}

void db_connection::set_Sql_type(SqlType t)
{
   type = t;
}
