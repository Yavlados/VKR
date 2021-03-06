#include "_Off_tels.h"

Off_tels::Off_tels()
{
    this->state = Newing;
}

Off_tels::Off_tels(int id, QString num, QString name)
{
     of_t_id = id;
     tel_num = num;
     service_name = name;
}

bool Off_tels::search(QList<Off_tels *> *list, QString tel_num, QString name)
{
        if(list==0)
        return false;

    qDeleteAll(*list);
    list->clear();

    QSqlQuery temp(db_connection::instance()->db());
    QString temp_query = "SELECT id,"
                         " name, "
                         " number"
                         " FROM notebook2.official_telephones";
    QString where_condition;
    if(!tel_num.isEmpty())
    {
        if(where_condition.isEmpty())
            where_condition += " WHERE official_telephones.number LIKE ('"+tel_num+"')";
        else
            where_condition += "AND official_telephones.number LIKE ('"+tel_num+"')";
    }
    if(!name.isEmpty())
    {
        if(where_condition.isEmpty())
            where_condition += " WHERE LOWER(official_telephones.name) LIKE LOWER('"+name+"')";
        else
            where_condition += "AND LOWER(official_telephones.name) LIKE LOWER('"+name+"')";
    }
    temp_query+= where_condition;
    temp.prepare(temp_query);
    if (!temp.exec())
    {
        qDebug() << "Off_tels::search" << temp.lastError();
        return false;
    }

    while (temp.next())
    {
        Off_tels *cnt = new Off_tels();
        cnt->of_t_id = temp.value(0).toInt();
        cnt->service_name = temp.value(1).toString();
        cnt->tel_num = temp.value(2).toString();
        list->append(cnt);
    }
}

bool Off_tels::selectOffTel(QList<Off_tels *> *list)
{
    if(list==0)
        return false;

    qDeleteAll(*list);
    list->clear();

    if( !db_connection::instance()->db_connect() )
        return false;

    QSqlQuery temp(db_connection::instance()->db());
    temp.prepare( "SELECT id,"
                  " name, "
                  " number "
                  " FROM notebook2.official_telephones");
    if (!temp.exec())
    {
        qDebug() << "Off_tels::selectOffTel" << temp.lastError();
        return false;
    }

    while (temp.next())
    {
        Off_tels *cnt = new Off_tels();
        cnt->of_t_id = temp.value(0).toInt();
        cnt->service_name = temp.value(1).toString();
        cnt->tel_num = temp.value(2).toString();
        cnt->state = Readed;
        list->append(cnt);
    }

    return true;
}

bool Off_tels::add_off_tel (Off_tels *of_t)
{
    QSqlQuery querry(db_connection::instance()->db());
    querry.prepare(" INSERT INTO notebook2.official_telephones "
                   " (number, name)"
                   " VALUES"
                   " ((:t_n), (:s_n))"
                   " RETURNING official_telephones.id");
    querry.bindValue(":t_n",of_t->tel_num);
    querry.bindValue(":s_n",of_t->service_name);
    if (!querry.exec())
    {
        qDebug() << "Off_tels::add_off_tel" << querry.lastError();
        return false;
    }
    while (querry.next())
    {
        of_t->of_t_id = querry.value(0).toInt();
    }
    return true;
}

bool Off_tels::del_off_tel(Off_tels *of_t)
{
    QSqlQuery querry(db_connection::instance()->db());
    querry.prepare("DELETE FROM notebook2.official_telephones "
                   " WHERE official_telephones.id = (:id)");

    querry.bindValue(":id",of_t->of_t_id);

    if (!querry.exec())
    {
        qDebug() << "Off_tels::del_off_tel" << querry.lastError();
        return false;
    }

    return true;
}

bool Off_tels::update(QList<Off_tels *> *list)
{
    if( !db_connection::instance()->db_connect() )
        return false;

    QString cname = db_connection::instance()->db().connectionName();

    bool isOk = db_connection::instance()->db().database(cname).transaction();

    QSqlQuery temp(db_connection::instance()->db());
    for (int a = 0 ; a < list->size(); a++)
    {
        temp.prepare("UPDATE notebook2.official_telephones SET number = (:tel_num), "
                                " name = (:s_n)"
                                " WHERE "
                                " id = (:id)");
          temp.bindValue(":tel_num", list->at(a)->tel_num);
          temp.bindValue(":s_n", list->at(a)->service_name);
          temp.bindValue(":id", list->at(a)->of_t_id);


        if (!temp.exec())
        {
            db_connection::instance()->lastError = temp.lastError().text();
            isOk =  false;
            break;
        }
    }
    if(!isOk)
    {
        db_connection::instance()->db().database(cname).rollback();
        qDebug() << "Off_tels::update ROLLBACK";
        return false;
    }
    db_connection::instance()->db().database(cname).commit();

    return true;
}

QList<Off_tels *> *Off_tels::compare_with_base(QString query)
{
    QList<Off_tels *> *list = 0;
    QSqlQuery querry(db_connection::instance()->db());
    querry.prepare("SELECT number, name, id from notebook2.official_telephones "
                   "WHERE "+ query);

    if (!querry.exec())
    {
        qDebug() << "Off_tels::compare_with_base" << querry.lastError();
        return list;
    }
    else {
        list = new QList<Off_tels *>;
    }
    while (querry.next())
    {
        Off_tels *of_t = new Off_tels;
        of_t->state = Readed;
        of_t->tel_num = querry.value(0).toString();
        of_t->service_name = querry.value(1).toString();
        of_t->of_t_id = querry.value(2).toInt();

        list->append(of_t);
    }
    return list;
}

void Off_tels::clear_list(QList<Off_tels *> *list)
{

    if(list != 0)
    {
        if(!list->isEmpty())
        {
            for(QList<Off_tels *>::const_iterator it = list->begin(); it != list->end(); it++)
            {
                delete *it;

            }

        }
    }
}

bool Off_tels::del_off_tel_by_id(int id)
{

    QSqlQuery querry(db_connection::instance()->db());
    querry.prepare("DELETE FROM notebook2.official_telephones "
                   " WHERE official_telephones.id = (:id)");

    querry.bindValue(":id",id);

    if (!querry.exec())
    {
        qDebug() << "Off_tels::del_off_tel_by_id" << querry.lastError();
        return false;
    }

    return true;
}
