#ifndef TELEPHONE_H
#define TELEPHONE_H

#include "enums.h"
#include <QString>
#include "_Contact.h"
#include <QList>
#include "db_connection.h"

class Telephone
{
public:

    QString id;
    QString num;
    QString person_id;
    bool internum;
    bool oldnum;
    DbState state;

    Telephone();

    QList<Contact *> *cont();

    QList<Contact*> *_cont; /// У телефона - список контактов
    /// CRUD
    static bool selectTelephone(QList<Telephone *> *list, QString personId );
    static bool createTelephone(Telephone *telephone, QString personId);
    static bool updateTelephone(Telephone *telephone);
    static bool deleteTelephone(Telephone *telephone);

};

#endif // TELEPHONE_H
