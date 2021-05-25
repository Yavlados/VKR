#ifndef PERSON_H
#define PERSON_H
#include <QString>
#include <QList>
#include "db_connection.h"
#include "_Telephone.h"

struct Adress {
    QString liv;
    QString reg;
    Adress();
    void setData(QString sqlRow);
};

enum mergeType{
    firstIsParent,
    secondIsParent
};

enum personType{
    dbPerson,
    importPerson
};

class Person
{
public:
    Person();
    ~Person();
    QString lastname;
    QString name;
    QString midname;
    QString alias;
    QString id;
    DbState state;
    personType type;
    QList<Telephone*> *_telephones;

    // Adress
    Adress country;
    Adress city;
    Adress street;
    Adress house;
    Adress corp;
    Adress flat;

    // Links
    QString hash;
    QList<Person*> *linked_persons;
    QSet<QString> *_linked_events;

    QList<Telephone *> *telephones();
    QSet<QString> *linked_events();
    void setTelephones(QList<Telephone *>  *newTelephones);
    static bool selectByEventId(QList<Person*> *personsList, QString eventId);
    static bool updatePerson(Person *person);
    static bool createPerson(Person *person, QString eventId);
    static bool deletePerson(Person *person);

    static bool handleTelephones(QList<Telephone*> *telephones, QString personId);

    static bool linkEventPerson(QString eventId, QString personId);
    static bool addHashRowToLinksTable(Person *person);

    static bool getLinkedPersons(Person *person);
    static bool unlinkPersonOnDelete(Person *person);
    static bool unlinkPersons(Person *person1, Person *person2);

    static QString updateHashArray(QString hashes, QString hashToExclude);

    static Person *mergePersons(Person *person1, Person *person2, mergeType type);

    static bool handleListOfPersons(QList<Person*> *list);

    static bool getLinkedEvents(Person *p,  QSet<QString> *resList = 0);
    static bool dropLinksToEvents(Person *p);
    static bool setLinkedEvents(Person *p);


    static bool setLinksForPerson(Person *p);
    static bool concatEventIds(Person *p1, Person *p2, Person *res);
};

#endif // PERSON_H
