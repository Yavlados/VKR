#ifndef CONTACT_H
#define CONTACT_H

#include <QList>
#include <QString>
#include "db_connection.h"
#include "enums.h"

class Contact
{
public:
    Contact();

    DbState state;

    QString number;
    QString alias;
    QString id;
    QString telephone_id;

    bool oldnum;
    bool internum;

    static bool selectContacts(QList<Contact*> *list, QString tel_id);
};

#endif // CONTACT_H
