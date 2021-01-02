#include "_Event.h"

Event::Event()
{
    _persons = 0;
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
                 " FROM notebook2.event;");
    if (!temp.exec())
    {
        qDebug() << "Event::selectAll" << temp.lastError();
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
