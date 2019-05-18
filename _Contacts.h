#ifndef CONTACTS_H
#define CONTACTS_H

#include "_Owners_tel.h"

#include <QList>
#include <QString>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>

class Contacts: public Owners_tel
{
private:

public:

    Contacts();                                                     /// Пустой конструктор класса
    Contacts(int i, QString tel, QString m, int ot_id, DbState st = IsReaded); /// Конструктор класса с тремя переменными
    ~Contacts();

    int owner_tel_id;
    //int contact_id;

    int parent_OT_id;      ///
    int contact_id;     /// ID записи в таблице, соотвествует DbState
    QString contact_tel_num;    /// Номер Телефона
    QString mark;       /// Пометка контакта
    DbState cont_state;

    static bool selectAll(QList<Contacts*> *list);
    static bool saveAll(QList<Contacts*> *list);
    static bool selectTelContacts(QList<Contacts*> *list, int);
    static bool selectContactsforEdit(QList<Contacts*> *list, int);

    bool insert(bool setState=true);
    bool update(bool setState=true);
    bool remove();

    void check() const;
};


#endif // CONTACTS_H
