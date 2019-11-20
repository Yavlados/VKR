#include "db_connection.h"

db_connection *db_connection::_instance = nullptr;

db_connection::db_connection()
{
    type =PSQLtype;
    _dbpg = QSqlDatabase::addDatabase("QPSQL","conPQ");
    ok = _dbpg.isValid();
    _dbsql = QSqlDatabase::addDatabase("QSQLITE","conql");
    _dbsqlchipher = QSqlDatabase::addDatabase("SQLITECIPHER","conqlchipher");
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
        //db().setDatabaseName("zk");
        //db().setPassword("ipqo");
        //db().setPort(5433);
        _dbpg.setHostName("localhost");
        _dbpg.setDatabaseName("postgres");
       _dbpg.setUserName("postgres");
        _dbpg.setPassword("23Xeromant23");
        //_dbpg.setPassword("123");
       _dbpg.setPort(5432);
        ok = _dbpg.open();
        if (ok == true)
            qDebug() <<"ok";
        else
            qDebug() << _dbpg.lastError().text();
        break;
    case (SQLliteType):
        ok = _dbsql.open();
        if (ok == true)
            qDebug() <<"ok";
        else
            qDebug() << _dbsql.lastError().text();
        break;
      case (SQLlitechipher):
        ok = _dbsqlchipher.open();
        if (ok == true)
            qDebug() <<"ok";
        else
            qDebug() << _dbsqlchipher.lastError().text();
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
    case (SQLlitechipher):
        return _dbsqlchipher;
    }
}

void db_connection::set_Sql_type(SqlType t)
{
   type = t;
}
