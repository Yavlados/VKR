#include "_Owners_tel.h"

Owners_tel::Owners_tel()
{
    parentZK_id = 0;
    tel_id = 0;
    internum = 0;
    oldnum  = 0;
    _cont = nullptr;
}

Owners_tel::Owners_tel(int t_d): tel_id(t_d)
{
    _cont = nullptr;
}

Owners_tel::Owners_tel(int id, int zk, bool i_m, DbState st)
{//////// Конструктор для формы добавления////////
    _cont = nullptr;
    internum = i_m;
    tel_id = id;
    parentZK_id = zk;
    state = st;
}

Owners_tel::Owners_tel(QString t_n, int t_id, int zk, bool i_m, bool o_n, DbState st):
    tel_id (t_id), tel_num(t_n), parentZK_id(zk), internum(i_m), oldnum(o_n)
{
    _cont = nullptr;
    state = st;
}

Owners_tel::~Owners_tel()
{
    Contacts::delete_all(_cont);
    qDebug()<<"delete telephone"<<tel_id;
}

QList<Contacts *> *Owners_tel::cont()
{
    if (_cont == nullptr)
    {
        _cont = new QList<Contacts*>;
    }
    return _cont;
}

bool Owners_tel::saveAll(QList<Owners_tel *> *list)
{
    if(list == nullptr || !db_connection::instance()->db_connect())
        return false;

    QString cname = db_connection::instance()->db().connectionName();

    bool isOk = db_connection::instance()->db().database(cname).transaction();

    for(int i=0;i<list->size();i++)
    {
        if ( !isOk )
            break;

    if ( ! list->at(i)->tel_num.isEmpty())
        switch(list->at(i)->state)
        {
        case IsNewing:
                isOk = list->at(i)->insert_tel(false);
            break;

        case IsChanged:
            isOk = list->at(i)->update_tel(false);
            break;
        case IsRemoved:
            isOk = list->at(i)->remove_tel();
            break;
        }
    }

    if(!isOk)
    {
        db_connection::instance()->db().database(cname).rollback();
        qDebug() << "отсюда";
        return false;
    }
    db_connection::instance()->db().database(cname).commit();

    for(int i=list->size()-1;i>=0;i--)
    {
        if( list->at(i)->state==IsRemoved)
        {
            delete list->at(i);
            list->removeAt(i);
        }
        else
            list->at(i)->state = IsReaded;
    }

    return true;
}

//////// Вместо вызова списка, создаю пустой для формы добавления ///////////
bool Owners_tel::selectZkTelForAdd(QList<Owners_tel *> *list, int zk)
{
    if(list==nullptr)
        return false;

    qDeleteAll(*list);
    list->clear();

    if(zk!=0)
    {
        if( !db_connection::instance()->db_connect() )
            return false;

        QSqlQuery temp(db_connection::instance()->db());
        temp.prepare("SELECT owners_tel.Telephone_num, owners_tel.Telephone_id, owners_tel.FK_Telephone_Zk, "
                     "owners_tel.internum, owners_tel.oldnum "
                     " FROM  owners_tel"
                     " WHERE owners_tel.FK_Telephone_Zk = (:id)");
        temp.bindValue(":id", zk);
        if (!temp.exec())
        {
            qDebug() << temp.lastError();
            qDebug() <<"selectZkTelForAdd";
            return false;
        }

        while (temp.next())
        {
            Owners_tel *ot = new Owners_tel(temp.value(0).toString(), temp.value(1).toInt(), temp.value(2).toInt(),temp.value(3).toBool(),temp.value(4).toBool(),IsReaded);
            list->append(ot);
        }
        /// Если у зк нет телефонов при редактировании
    }
    else
    {  //////создаю модель с двумя - id и FK///////
           Owners_tel *ot = new Owners_tel(nullptr, zk ,IsNewing);
           list->append(ot);
           return true;
    }
    //Костыль для пустого списка(корректное отображение в главном меню)
    if(list->isEmpty())
    {
                //////создаю модель с двумя - id и FK///////
                Owners_tel *ot = new Owners_tel(0 , 0,false,IsNewing);
                ot->oldnum = false;
                list->append(ot);
               return true;
    }
    return true;
}

QList<Owners_tel *> *Owners_tel::get_ow_list(int zk_id)
{
    QList<Owners_tel *> *list = new QList<Owners_tel*>;

    if( !db_connection::instance()->db_connect() )
        return list;

    QSqlQuery temp(db_connection::instance()->db());
    temp.prepare("SELECT owners_tel.Telephone_num, owners_tel.Telephone_id, owners_tel.FK_Telephone_Zk, owners_tel.internum, owners_tel.oldnum "
                 " FROM  owners_tel"
                 " WHERE owners_tel.FK_Telephone_Zk = (:id)");
    temp.bindValue(":id", zk_id);
    if (!temp.exec())
    {
        qDebug() << temp.lastError();
        qDebug() <<"selectZkTelForAdd";
        return list;
    }

    while (temp.next())
    {
        Owners_tel *ot = new Owners_tel(temp.value(0).toString(), temp.value(1).toInt(), temp.value(2).toInt(),temp.value(3).toBool(),temp.value(4).toBool(),IsReaded);
        list->append(ot);
    }
    return list;
}

int Owners_tel::insert_tel(bool setState, int zk_id)
{
    if( !db_connection::instance()->db_connect() && tel_num.isEmpty())
        return false;

    QSqlQuery temp(db_connection::instance()->db());
    temp.prepare("INSERT INTO owners_tel( Telephone_num, FK_Telephone_Zk, internum, oldnum) "
                 " VALUES ( (:tel_num),(:fk_id), (:i_n), (:o_n)) RETURNING Telephone_id");
    temp.bindValue(":tel_num",tel_num);
    temp.bindValue(":fk_id",zk_id);
    temp.bindValue(":i_n",internum);
    temp.bindValue(":o_n", oldnum);
    if (!temp.exec())
    {
        qDebug() << temp.lastError();
        return -1;
    }
    if (temp.next())
    {
        qDebug()<<temp.executedQuery();
        tel_id = temp.value(0).toInt();
        qDebug() << "OT add" + QString::number(tel_id) << tel_num;
            if( setState )
                state = IsReaded;
            return tel_id;
    }
        db_connection::instance()->lastError = temp.lastError().text();
        db_connection::instance()->error = temp.lastError();
        return -1;
}

bool Owners_tel::update_tel(bool setState)
{
    if( !db_connection::instance()->db_connect() )
        return false;

    QSqlQuery temp(db_connection::instance()->db());
    temp.prepare("UPDATE owners_tel SET Telephone_num = (:tel_num),"
                                    "internum = (:i_m),"
                                   " oldnum = (:o_n)"
                            " WHERE Telephone_id = (:tel_id)");
    temp.bindValue(":tel_id", tel_id);
    temp.bindValue(":tel_num", tel_num);
    temp.bindValue(":i_m", internum);
    temp.bindValue(":o_n", oldnum);

    if (!temp.exec())
    {
        db_connection::instance()->lastError = temp.lastError().text();
        qDebug() << temp.lastError();
        return false;
    }

    qDebug() << "update" + QString::number(tel_id);
    if(setState)
        state = IsReaded;
    return true;
}

//////////////////////////////////////////////////////////////////////
bool Owners_tel::remove_tel()
{
    if( !db_connection::instance()->db_connect() )
        return false;

    QSqlQuery temp(db_connection::instance()->db());
   temp.prepare("DELETE FROM owners_tel "
                " WHERE Telephone_id = (:id)");
   temp.bindValue(":id",tel_id);

    if (!temp.exec())
    {
        db_connection::instance()->lastError = temp.lastError().text();
        qDebug() << temp.lastError();
        return false;
    }

    qDebug() << "delete" + QString::number(tel_id);
    state = IsRemoved;
    return true;
}

QList<int> Owners_tel::compare_nums(QList<Owners_tel*> *owt1, QList<Owners_tel*> *owt2)
{
    QList<int> templist;
    for (int i = 0; i < owt1->size(); i++)
        for(int j =0; j <owt2->size(); j++)
        {
            if(owt1->at(i)->tel_num == owt2->at(j)->tel_num)
            {
                templist.append(i);
            }
        }
    return templist;
}

//////////////////////////////////////////////////////////////////////

bool Owners_tel::delete_all(QList<Owners_tel *> list)
{
    if(!list.isEmpty())
        {
            for(QList<Owners_tel*>::const_iterator it = list.begin(); it != list.end(); it++)
            {
                delete *it;
            }

        }
}
//////////////////////////////////////////////////////////////////////

bool Owners_tel::delete_all(QList<Owners_tel *> *list)
{
    if(list != nullptr)
    {
        if(!list->isEmpty())
        {
            for(QList<Owners_tel*>::const_iterator it = list->begin(); it != list->end(); it++)
            {


                delete *it;

            }

        }
        delete list;
        list = nullptr;
    }
}
