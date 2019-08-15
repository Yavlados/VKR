#include "_Contacts.h"
//////////////////////////////////////////////////////////////////
Contacts::Contacts()
{
    contact_id=0;
    cont_state = IsNewing;
}

/// Конструктор класса с тремя переменными
Contacts::Contacts(int cont_id, QString tel, QString mark, int ot_id, bool i_n, bool o_n, DbState st, int l_id)
    :contact_id(cont_id), contact_tel_num(tel), mark(mark), internum(i_n), oldnum(o_n), linked_id(l_id)
{
    parent_OT_id = ot_id;
    cont_state = st;
}

Contacts::~Contacts()
{
    qDebug()<<"delete contact"<<contact_id;
}

bool Contacts::saveAll_cont(QList<Contacts*> *list, int new_tel_id)
{
    if(list==nullptr || !db_connection::instance()->db_connect())
        return false;

    QString cname = db_connection::instance()->db().connectionName();

    bool isOk = db_connection::instance()->db().database(cname).transaction();

    for(int i=0;i<list->size();i++)
    {
        if ( !isOk )
            break;

     if (!list->at(i)->contact_tel_num.isEmpty())
        switch(list->at(i)->cont_state)
        {
        case IsNewing:
                isOk = list->at(i)->insert(false, new_tel_id);
            break;
        case IsChanged:
            isOk = list->at(i)->update(false);
            break;
        case IsRemoved:
            isOk = list->at(i)->remove();
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
        if( list->at(i)->cont_state==IsRemoved)
        {
            delete list->at(i);
            list->removeAt(i);
        }
        else
            list->at(i)->cont_state = IsReaded;
    }

    return true;
}

bool Contacts::selectTelContacts(QList<Contacts *> *list, int tel_id)
{
    if(list==nullptr)
        return false;

    qDeleteAll(*list);
    list->clear();

    if( !db_connection::instance()->db_connect() )
        return false;

    QSqlQuery temp(db_connection::instance()->db());

    temp.prepare("SELECT "
                 "contacts.contact_list_id,"
                 "contacts.cl_telephone,"
                 "contacts.cl_info, "
                 "contacts.FK_Cl_telephone, "
                 "contacts.internum, "
                 "contacts.oldnum "
                 " FROM contacts"
                  " WHERE contacts.FK_Cl_telephone = (:id) ");
    temp.bindValue(":id",tel_id);
    if (!temp.exec())
    {
        qDebug() << temp.lastError();
        return false;
    }

    while (temp.next())
    {
            Contacts *cnt = new Contacts(temp.value(0).toInt(), temp.value(1).toString(), temp.value(2).toString(),temp.value(3).toInt(),temp.value(4).toBool(),temp.value(5).toBool(), IsReaded);
            list->append(cnt);
    }

    var2_analysis_for_main(list, tel_id);

}

bool inline Contacts::var2_analysis_for_main(QList<Contacts *> *list, int tel_id)
{
    QString temp_str;
    for (int i = 0 ; i<list->size(); i++)
    {
        if(temp_str.isEmpty())
        {
            temp_str = " contacts.cl_telephone = ('"+list->at(i)->contact_tel_num+"')";
        }else
        {
            temp_str += " OR contacts.cl_telephone = ('"+list->at(i)->contact_tel_num+"')";
        }
        if(list->at(i)->oldnum == true)
            temp_str += " OR contacts.cl_telephone = ('499"+list->at(i)->contact_tel_num+"')"
                                " OR contacts.cl_telephone = ('495"+list->at(i)->contact_tel_num+"')";
    }

    db_connection *db = db_connection::instance();
    QSqlQuery querry(db->db());
    QString tempSQL;
    /// 2 вариант - в КОНТАКТАХ анализируемого человека обнаружен этот человек

    tempSQL += " SELECT DISTINCT zk.zk_id, OWT.OWNER_TEL,"
                 " OWT.TEL_NUM, OWT.CONT_MARK"
                 " FROM zk"
                 " INNER JOIN (SELECT DISTINCT owners_tel.fk_telephone_zk AS FK_ZK,"
                 " owners_tel.telephone_num AS TEL_NUM,"
                 " CONT.CONT_MARK AS CONT_MARK,"
                 " CONT.OWNER_TEL AS OWNER_TEL"
                 " FROM owners_tel"
                 " INNER JOIN (SELECT DISTINCT contacts.cl_telephone AS CONT_TEL,"
                 " contacts.cl_info AS CONT_MARK,"
                 " tels.OWNER_TEL AS OWNER_TEL"
                 " FROM contacts,(SELECT DISTINCT owners_tel.telephone_id as tel_id,"
                 " owners_tel.telephone_num AS OWNER_TEL"
                 " FROM owners_tel"
                 " WHERE owners_tel.telephone_id = "+QString::number(tel_id)+") as tels"
                 " WHERE ("+temp_str+") AND tels.tel_id = contacts.fk_cl_telephone) AS CONT"
                 " ON CONT.CONT_TEL = owners_tel.telephone_num) AS OWT"
                 " ON OWT.FK_ZK = zk.zk_id"
                 " WHERE ZK.zk_id>0 ";

         querry.prepare(tempSQL);

         if (!querry.exec())
             qDebug() << querry.lastError();
         while (querry.next())
         {
             for (int i = 0; i < list->size(); i++)
             {
                 if (list->at(i)->contact_tel_num == querry.value(2).toString())
                 {
                     Contacts *cnt = list->at(i);
                     cnt->linked_id = querry.value(0).toInt();
                 }

             }
         }
}

bool Contacts::insert(bool setState, int new_tel_id)
{
    if( !db_connection::instance()->db_connect() )
        return false;

    QString _temp = contact_tel_num.at(1)+contact_tel_num.mid(3,3)+
            contact_tel_num.mid(7,3)+
            contact_tel_num.mid(11,2)+contact_tel_num.mid(14,2);
contact_tel_num = _temp;

    QSqlQuery temp(db_connection::instance()->db());
    temp.prepare("INSERT INTO contacts( cl_telephone, cl_info, FK_Cl_telephone, internum, oldnum) "
                 " VALUES ( (:tel_num), (:mark), (:fk_id), (:i_n), (:o_n))"
                 " RETURNING Contact_list_id");
    temp.bindValue(":tel_num",contact_tel_num);
    temp.bindValue(":mark",mark);
    temp.bindValue(":fk_id", new_tel_id);
    temp.bindValue(":i_n", internum);
    temp.bindValue(":o_n", oldnum);

    if (!temp.exec())
    {
        db_connection::instance()->lastError = temp.lastError().text();
        qDebug() << temp.lastError();
        return false;
    }
    if (temp.next())
    {
        qDebug()<<temp.executedQuery();
        contact_id = temp.value(0).toInt();
        qDebug() << " contact add" + QString::number(contact_id) << mark;
            if( setState )
                cont_state = IsReaded;
            return true;
    }
        db_connection::instance()->lastError = temp.lastError().text();
        return false;
}

bool Contacts::update(bool setState)
{
    if( !db_connection::instance()->db_connect() )
        return false;

    QString _temp = contact_tel_num.at(1)+contact_tel_num.mid(3,3)+
            contact_tel_num.mid(7,3)+
            contact_tel_num.mid(11,2)+contact_tel_num.mid(14,2);
contact_tel_num = _temp;

    QSqlQuery temp(db_connection::instance()->db());
    temp.prepare("UPDATE contacts SET cl_telephone = (:cl_tel), "
                            "cl_info = (:cl_info),"
                            "internum = (:i_n),"
                            "oldnum = (:o_n) "
                            " WHERE "
                            " Contact_list_id = (:id)");
      temp.bindValue(":cl_tel", contact_tel_num);
      temp.bindValue(":cl_info", mark);
      temp.bindValue(":id", contact_id);
      temp.bindValue(":i_n", internum);
      temp.bindValue(":o_n", oldnum);

    if (!temp.exec())
    {
        db_connection::instance()->lastError = temp.lastError().text();
        qDebug() << temp.lastError();
        return false;
    }

    qDebug() << "update" + QString::number(contact_id);
    if(setState)
        cont_state = IsReaded;
    return true;
}

bool Contacts::remove()
{
    if( !db_connection::instance()->db_connect() )
        return false;

    QSqlQuery temp(db_connection::instance()->db());
   temp.prepare("DELETE FROM contacts WHERE Contact_list_id = (:id)");
   temp.bindValue(":id",contact_id);

    if (!temp.exec())
    {
        db_connection::instance()->lastError = temp.lastError().text();
        qDebug() << temp.lastError();
        return false;
    }

    qDebug() << "delete" + QString::number(contact_id);
    cont_state = IsRemoved;
    return true;
}

void Contacts::check() const
{
    qDebug() << QString::number(contact_id)+" " +contact_tel_num+ " " +mark +" " +QString::number(parent_OT_id);
}
