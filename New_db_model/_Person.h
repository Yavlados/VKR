#ifndef PERSON_H
#define PERSON_H
#include <QString>
#include <QList>
#include "db_connection.h"

class Person
{
public:
    Person();
    QString lastname;
    QString name;
    QString midname;
    QString alias;
    QString id;
    static bool selectByEventId(QList<Person*> *personsList, QString eventId);
};

#endif // PERSON_H
