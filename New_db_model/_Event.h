#ifndef EVENT_H
#define EVENT_H
#include <QString>
#include "_Person.h"
#include "db_connection.h"
#include "enums.h"

#include <QList>

class Event
{
public:
    Event();
    ~Event();
    QString category;
    QString detention_date;
    QString detention_time;
    QString detention_reason;
    QString detention_by;
    QString keeping_place;
    QString additional;
    QString id;
    CheckStateEvent checkState_;

    DbState state;
    QList<Person *> *persons();
    static bool selectAll(QList<Event*> *eventList);
    static bool updateEvent(Event *event);
    static bool createEvent(Event *event);
    static bool deleteEvent(Event *event);
private:
    QList<Person*> *_persons;
};

#endif // EVENT_H
