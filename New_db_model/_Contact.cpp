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
