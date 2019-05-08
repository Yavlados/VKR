#ifndef CONTACTS_H
#define CONTACTS_H

#include <QList>
#include <QString>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QVector>
#include "owners_tel.h"

class Contacts: public Owners_tel
{
private:

public:
    int owner_tel_id;
    int contact_id;
    QVector<QString> contacts_tel;
    QVector<QString> contacts_info;

    QVector<QVector <QString> > vector;

   void append_contact(QString);

   void append_info(QString);

   void add_contact_to_db();

   void temp_change_cont();

   void recieve_cont_id(QString, QString);

   void del_contact(int);

   void add_fk_contact(int);

Contacts()
{
}
};

#endif // CONTACTS_H
