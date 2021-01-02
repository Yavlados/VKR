#include "_Person.h"

Person::Person()
{

}

bool Person::selectByEventId(QList<Person *> *personsList, QString eventId)
{
    if(personsList==0)
        return false;

    if( !db_connection::instance()->db_connect() )
        return false;

//    qDeleteAll(*personsList);
    personsList->clear();

    QString cname = db_connection::instance()->db().connectionName();
    bool isOk = db_connection::instance()->db().database(cname).transaction();

    QSqlQuery temp(db_connection::instance()->db());
    temp.prepare("SELECT "
                 "p.name, "
                 "p.midname,"
                 "p.lastname,"
                 "p.alias, "
                 "p.id "
                 "FROM notebook2.person as p, "
                 " notebook2.event_person "
                 " WHERE event_person.person_id = p.id "
                 " AND event_person.event_id = (:event_id)");
     temp.bindValue(":event_id", eventId);
     if (!temp.exec())
     {
         qDebug()  << "Person::selectByEventId"<< temp.lastError() << temp.executedQuery();
         isOk = false;
     }

     while (temp.next())
     {
         Person *person = new Person();
         person->name = temp.value(0).toString();
         person->midname = temp.value(1).toString();
         person->lastname = temp.value(2).toString();
         person->alias = temp.value(3).toString();
         person->id = temp.value(4).toString();

         personsList->append(person);
     }

     if(personsList->isEmpty())
         return false;

     return true;
}
