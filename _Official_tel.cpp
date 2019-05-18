#include "_Official_tel.h"

Official_tel::Official_tel()
{
    state = IsNewing;
}

Official_tel::Official_tel(int id, QString t_n, QString s_n, DbState st)
{
    of_t_id = id;
    tel_num = t_n;
    service_name = s_n;
    state = st;
}

bool Official_tel::selectAll(QList<Official_tel *> *list)
{
    if(list==nullptr)
        return false;

    qDeleteAll(*list);
    list->clear();

    if( !db_connection::instance()->db_connect() )
        return false;

    QSqlQuery temp(db_connection::instance()->db);
    temp.prepare("SELECT "
                 "\"official_tel\".\"of_t_id\","
                  "\"official_tel\".\"tel_num\","
                 "\"official_tel\".\"service_name\" "
                 ""
                 "FROM \"official_tel\"");
    if (!temp.exec())
    {
        qDebug() << temp.lastError();
        return false;
    }

    while (temp.next())
    {
        Official_tel *cnt = new Official_tel(temp.value(0).toInt(), temp.value(1).toString(), temp.value(2).toString(), IsReaded);
        list->append(cnt);
    }

    return true;
}
