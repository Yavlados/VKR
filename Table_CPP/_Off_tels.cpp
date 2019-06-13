#include "_Off_tels.h"

Off_tels::Off_tels()
{

}

Off_tels::Off_tels(int id, QString num, QString name)
{
     of_t_id = id;
     service_name = name;
     tel_num = num;
}

bool Off_tels::selectOffTel(QList<Off_tels *> *list)
{
    if(list==nullptr)
        return false;

    qDeleteAll(*list);
    list->clear();

    if( !db_connection::instance()->db_connect() )
        return false;

    QSqlQuery temp(db_connection::instance()->db());
    temp.prepare("SELECT Official_tel.of_t_id,"
                 " Official_tel.service_name, "
                  "Official_tel.tel_num"
                " FROM Official_tel");
    if (!temp.exec())
    {
        qDebug() << temp.lastError();
        return false;
    }

    while (temp.next())
    {
        Off_tels *cnt = new Off_tels(temp.value(0).toInt(), temp.value(1).toString(), temp.value(2).toString());
        list->append(cnt);
    }

    return true;
}
