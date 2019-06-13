#include "_Owners_tel.h"

Owners_tel::Owners_tel()
{
    parentZK_id = 0;
    tel_id = 0;
    _cont = nullptr;
}

Owners_tel::Owners_tel(int t_d): tel_id(t_d)
{
    _cont = nullptr;
}

Owners_tel::Owners_tel(int id, int zk, DbState st)
{//////// Конструктор для формы добавления////////
    _cont = nullptr;
    tel_id = id;
    parentZK_id = zk;
    state = st;
}

Owners_tel::Owners_tel(QString t_n, int t_id, int zk, DbState st):
    tel_id (t_id), tel_num(t_n), parentZK_id(zk)
{
    _cont = nullptr;
    state = st;
}

Owners_tel::~Owners_tel()
{
    qDebug()<<"delete telephone"<<tel_id;
}

QList<Contacts *> *Owners_tel::cont()
{
    if (_cont == nullptr)
    {
        _cont = new QList<Contacts*>;
        //Contacts::selectTelContacts(_cont, tel_id);
    }
    return _cont;
}

bool Owners_tel::selectAll(QList<Owners_tel *> *list)
{
    if(list==nullptr)
        return false;

    qDeleteAll(*list);
    list->clear();

    if( !db_connection::instance()->db_connect() )
        return false;

    QSqlQuery temp(db_connection::instance()->db());
    temp.prepare("SELECT owners_tel.Telephone_num "
                           "FROM  owners_tel");
    if (!temp.exec())
    {
        qDebug() << temp.lastError();
        return false;
    }

    while (temp.next())
    {
        //Owners_tel *ot = new Owners_tel(temp.value(0).toString(),IsReaded);
        //list->append(ot);
    }

    for (int i = 0; i<list->size(); i++)
    {
        //QString str = QString::number(list->at(i)->contact_id);
        qDebug() << list->at(i)->tel_num;
    }

    return true;
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
        temp.prepare("SELECT owners_tel.Telephone_num, owners_tel.Telephone_id, owners_tel.FK_Telephone_Zk "
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
            Owners_tel *ot = new Owners_tel(temp.value(0).toString(), temp.value(1).toInt(), temp.value(2).toInt(),IsReaded);
            list->append(ot);
        }
        /// Если у зк нет телефонов при редактировании
    }
    else
    {  //////создаю модель с двумя - id и FK///////
           Owners_tel *ot = new Owners_tel(0, zk ,IsNewing);
           list->append(ot);
           return true;
    }
    if(list->isEmpty())
    {
        QSqlQuery temp(db_connection::instance()->db());

        temp.prepare("SELECT MAX( owners_tel.Telephone_id) "
                             " FROM owners_tel");
                if (!temp.exec())
                {
                    qDebug() << temp.lastError();
                    qDebug() << "selectZkTelForAdd";
                    return false;
                }
                while (temp.next())
                {
                //////создаю модель с двумя - id и FK///////
                Owners_tel *ot = new Owners_tel(temp.value(0).toInt() +1 , zk,IsNewing);
                list->append(ot);
               return true;
                }
    }
    return true;
}

QList<Owners_tel *> *Owners_tel::get_ow_list(int zk_id)
{
    QList<Owners_tel *> *list = new QList<Owners_tel*>;

    if( !db_connection::instance()->db_connect() )
        return list;

    QSqlQuery temp(db_connection::instance()->db());
    temp.prepare("SELECT owners_tel.Telephone_num, owners_tel.Telephone_id, owners_tel.FK_Telephone_Zk "
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
        Owners_tel *ot = new Owners_tel(temp.value(0).toString(), temp.value(1).toInt(), temp.value(2).toInt(),IsReaded);
        list->append(ot);
    }
    return list;
}

int Owners_tel::insert_tel(bool setState, int zk_id)
{
    if( !db_connection::instance()->db_connect() && tel_num.isEmpty())
        return false;

    QSqlQuery temp(db_connection::instance()->db());
    temp.prepare("INSERT INTO owners_tel( Telephone_num, FK_Telephone_Zk) "
                 " VALUES ( (:tel_num),(:fk_id)) RETURNING Telephone_id");
    temp.bindValue(":tel_num",tel_num);
    temp.bindValue(":fk_id",zk_id);

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
    temp.prepare("UPDATE owners_tel SET Telephone_num = (:tel_num)"
                            " WHERE Telephone_id = (:tel_id)");
    temp.bindValue(":tel_id", tel_id);
    temp.bindValue(":tel_num", tel_num);

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

bool Owners_tel::compare_with_base(QString tel_num)
{
    if( !db_connection::instance()->db_connect() )
        return false;

    QSqlQuery temp(db_connection::instance()->db());

   temp.prepare("SELECT owners_tel.FK_Telephone_Zk "
                " FROM owners_tel "
                " WHERE owners_tel.Telephone_num = (:num)");
   temp.bindValue(":num",tel_num);

    if (!temp.exec())
    {
        db_connection::instance()->lastError = temp.lastError().text();
        qDebug() << temp.lastError();
        return false;
    }
    while(temp.next())
    {
        parentZK_id = temp.value(0).toInt();
        return false;
    }
    return true;
}
//////////////////////////////////////////////////////////////////////
