#include "_Event.h"

Event::Event()
{
    _persons = 0;
    this->category          = "";
    this->detention_date    = "";
    this->detention_time    = "";
    this->detention_reason  = "";
    this->detention_by      = "";
    this->keeping_place     = "";
    this->additional        = "";
    this->id                = "";
    this->state             = IsNewing;
    this->checkState_       = Unchecked_;
}

QList<Person *> *Event::persons()
{
    if (_persons == 0)
    {
//        state = IsNewing;
        _persons = new QList<Person*>;
    }
//    checkState_ = Unchecked;
    return _persons;
}

bool Event::selectAll(QList<Event *> *eventList)
{
    if(eventList==0)
        return false;

    qDeleteAll(*eventList);
    eventList->clear();

    if( !db_connection::instance()->db_connect() )
        return false;

    QSqlQuery temp(db_connection::instance()->db());
    temp.prepare("SELECT "
                 "category, "
                 "detention_date, "
                 "detention_time,"
                 "detention_reason,"
                 "detention_by,"
                 "keeping_place,"
                 "additional,"
                 "id"
                 " FROM notebook2.event"
                 " ORDER BY id;");
    if (!temp.exec())
    {
        qDebug() << "Event::selectAll" << temp.lastError();
         db_connection::instance()->lastError = "Event::selectAll " + temp.lastError().text();
         return false;
    }
    while (temp.next())
    {
        Event *e = new Event();
        e->category = temp.value(0).toString();
        e->detention_date = temp.value(1).toString();
        e->detention_time = temp.value(2).toString();
        e->detention_reason = temp.value(3).toString();
        e->detention_by = temp.value(4).toString();
        e->keeping_place = temp.value(5).toString();
        e->additional = temp.value(6).toString();
        e->id = temp.value(7).toString();
        e->state = IsReaded;
        eventList->append(e);
    }
    if(eventList->isEmpty())
        return false;

    return true;
}

bool Event::updateEvent(Event *event)
{
    QString cname = db_connection::instance()->db().connectionName();

    bool isOk = db_connection::instance()->db().database(cname).transaction();
    QSqlQuery temp(db_connection::instance()->db());
    temp.prepare("UPDATE notebook2.event "
                 " SET category =           (:category),"
                       "detention_date =    (:detention_date), "
                       "detention_time =    (:detention_time),  "
                       "detention_reason =  (:detention_reason),"
                       "detention_by =      (:detention_by), "
                      " keeping_place =     (:keeping_place),"
                      " additional =        (:additional)"
                 " WHERE id = (:id) ");
        temp.bindValue(":category", event->category);
        temp.bindValue(":detention_date", event->detention_date);
        temp.bindValue(":detention_time", event->detention_time);
        temp.bindValue(":detention_reason", event->detention_reason);
        temp.bindValue(":detention_by", event->detention_by);
        temp.bindValue(":keeping_place", event->keeping_place);
        temp.bindValue(":additional", event->additional);
        temp.bindValue(":id", event->id.toInt());
     if (!temp.exec())
     {
         qDebug()  << "Event::updateEvent"<< temp.lastError() << temp.executedQuery();
         db_connection::instance()->lastError = "Event::updateEvent " + temp.lastError().text();
         db_connection::instance()->db().database(cname).rollback();

         isOk = false;
     } else {
         for( int i=0; i<event->persons()->size(); i++){
             Person *person = event->persons()->at(i);
             bool a = false;
             switch (person->state) {
                case IsNewing:
                 a = Person::createPerson(person, event->id);
                 break;
             case IsChanged:
                 a = Person::updatePerson(person);
                 break;
             case IsRemoved:
                 a = Person::deletePerson(person);
                 break;
             case IsReaded:
                 a=true;
                 break;
             }

             if(!a){
                 db_connection::instance()->db().database(cname).rollback();
                 isOk = false;
                 return isOk;
             }
         }

         isOk = true;
         db_connection::instance()->db().database(cname).commit();
     }
     return isOk;
}

bool Event::createEvent(Event *event)
{
    if( !db_connection::instance()->db_connect() )
        return false;

    QString cname = db_connection::instance()->db().connectionName();

    bool isOk = db_connection::instance()->db().database(cname).transaction();
    QSqlQuery temp(db_connection::instance()->db());

    temp.prepare("INSERT INTO notebook2.event ( "
                    "category,"
                    "detention_date,"
                    "detention_time,"
                    "detention_reason,"
                    "detention_by,"
                    "keeping_place,"
                    "additional) "
                 " VALUES ( (:category),(:detention_date),(:detention_time),"
                 "(:detention_reason),(:detention_by),(:keeping_place),(:additional) )"
                 " RETURNING id" );
    temp.bindValue(":category", event->category);
    temp.bindValue(":detention_date", event->detention_date);
    temp.bindValue(":detention_time", event->detention_time);
    temp.bindValue(":detention_reason", event->detention_reason);
    temp.bindValue(":detention_by", event->detention_by);
    temp.bindValue(":keeping_place", event->keeping_place);
    temp.bindValue(":additional", event->additional);

    if (!temp.exec())
    {
        db_connection::instance()->lastError = "Event::createEvent " + temp.lastError().text();

        qDebug() << "Event::createEvent" << temp.lastError();
        db_connection::instance()->db().database(cname).rollback();

        return false;
    }
    while(temp.next()){
        event->id = temp.value(0).toString();
        for( int i=0; i<event->persons()->size(); i++){
            Person *person = event->persons()->at(i);
            bool a = false;
            switch (person->state) {
               case IsNewing:
                a = Person::createPerson(person, event->id);
                break;
            case IsChanged:
                a = Person::updatePerson(person);
                break;
            case IsRemoved:
                a = Person::deletePerson(person);
                break;
            case IsReaded:
                a=true;
                break;
            }

            if(!a){
                db_connection::instance()->db().database(cname).rollback();
                isOk = false;
                return isOk;
            }
        }

        isOk = true;
        db_connection::instance()->db().database(cname).commit();
        return true;
    }
    return isOk;

}

bool Event::deleteEvent(Event *event)
{
    QString cname = db_connection::instance()->db().connectionName();

    bool isOk = db_connection::instance()->db().database(cname).transaction();
    QSqlQuery temp(db_connection::instance()->db());
    temp.prepare("DELETE FROM notebook2.event WHERE id=(:id)");

    temp.bindValue(":id", event->id.toInt());

    if (!temp.exec())
    {
        qDebug()  << "Event::deleteEvent"<< temp.lastError() << temp.executedQuery();
        db_connection::instance()->lastError = "Event::deleteEvent " + temp.lastError().text();
        db_connection::instance()->db().database(cname).rollback();
        return false ;
    } else{
        db_connection::instance()->db().database(cname).commit();
        return true;
    }
}
