#include "_Contact.h"

Contact::Contact()
{

}

bool Contact::selectContacts(QList<Contact *> *list, QString tel_id)
{
    if(list==0)
        return false;

    qDeleteAll(*list);
    list->clear();

    if( !db_connection::instance()->db_connect() )
        return false;

    QSqlQuery temp(db_connection::instance()->db());

    temp.prepare("SELECT "
                 "number,"
                 "alias,"
                 "id,"
                 "oldnum,"
                 "internum,"
                 "telephone_id "
                 "FROM notebook2.contact"
                 " WHERE telephone_id = (:id)");
    temp.bindValue(":id",tel_id);
    if (!temp.exec())
    {
        qDebug() << "Contact::selectContacts" <<  temp.lastError() << temp.executedQuery();
        return false;
    }

    while (temp.next())
    {
        Contact *cnt = new Contact;
        cnt->number = temp.value(0).toString();
        cnt->alias = temp.value(1).toString();
        cnt->id = temp.value(2).toString();
        cnt->oldnum = temp.value(3).toBool();
        cnt->internum = temp.value(4).toBool();
        cnt->telephone_id = temp.value(5).toString();
        cnt->state = IsReaded;
        list->append(cnt);
    }
    return true;

}

bool Contact::createContact(Contact *contact, QString telephoneId)
{
    if( !db_connection::instance()->db_connect() )
        return false;

    QString cname = db_connection::instance()->db().connectionName();

    bool isOk = db_connection::instance()->db().database(cname).transaction();
    QSqlQuery temp(db_connection::instance()->db());

    temp.prepare("INSERT INTO notebook2.contact ( "
                     "number,"
                     "alias,"
                     "internum, "
                     "oldnum,"
                     " telephone_id)"
                 " VALUES ( (:number), (:alias), (:internum), (:oldnum), (:telephone_id))" );
    temp.bindValue(":number",contact->number);
    temp.bindValue(":alias", contact->alias);
    temp.bindValue(":internum", contact->internum);
    temp.bindValue(":oldnum", contact->oldnum);
    temp.bindValue(":telephone_id", telephoneId.toInt());

    if (!temp.exec())
    {
        db_connection::instance()->lastError = temp.lastError().text();
        qDebug() << "Contact::createContact" << temp.lastError();
        db_connection::instance()->db().database(cname).rollback();

        return false;
    }
    db_connection::instance()->db().database(cname).commit();
    return true;

}

bool Contact::updateContact(Contact *contact)
{
    if( !db_connection::instance()->db_connect() )
        return false;

    QString cname = db_connection::instance()->db().connectionName();

    bool isOk = db_connection::instance()->db().database(cname).transaction();
    QSqlQuery temp(db_connection::instance()->db());

    temp.prepare("UPDATE notebook2.contact "
                 " SET 	number = (:number),"
                         " alias = (:alias),"
                         " internum = (:internum),"
                         " oldnum = (:oldnum),"
                         "telephone_id = (:telephone_id)"
                 " WHERE id = (:id)");
    temp.bindValue(":number",contact->number);
    temp.bindValue(":alias", contact->alias);
    temp.bindValue(":internum", contact->internum);
    temp.bindValue(":oldnum", contact->oldnum);
    temp.bindValue(":telephone_id", contact->telephone_id.toInt());
    temp.bindValue(":id", contact->id.toInt());

    if (!temp.exec())
    {
        db_connection::instance()->lastError = temp.lastError().text();
        qDebug() << "Contact::updateContact" << temp.lastError();
        db_connection::instance()->db().database(cname).rollback();
        return false;
    }
    db_connection::instance()->db().database(cname).commit();
    return true;

}

bool Contact::deleteContact(Contact *contact)
{
    QString cname = db_connection::instance()->db().connectionName();

    bool isOk = db_connection::instance()->db().database(cname).transaction();
    QSqlQuery temp(db_connection::instance()->db());
    temp.prepare("DELETE FROM notebook2.contact WHERE id=(:id)");

    temp.bindValue(":id", contact->id.toInt());

    if (!temp.exec())
    {
        qDebug()  << "Telephone::updateTelephone"<< temp.lastError() << temp.executedQuery();
        db_connection::instance()->db().database(cname).rollback();
        return false ;
    } else{
        db_connection::instance()->db().database(cname).commit();
        return true;
    }
}
