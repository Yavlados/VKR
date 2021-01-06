#ifndef PERSON_H
#define PERSON_H
#include <QString>
#include <QList>
#include "db_connection.h"
#include "_Telephone.h"

class Person
{
public:
    Person();
    QString lastname;
    QString name;
    QString midname;
    QString alias;
    QString id;

    QList<Telephone*> *_telephones;

    QList<Telephone *> *telephones();
    static bool selectByEventId(QList<Person*> *personsList, QString eventId);
    static bool updatePerson(Person *person);
    static bool createPerson(Person *person);
    static bool deletePerson(Person *person);
};

#endif // PERSON_H
