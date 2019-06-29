#ifndef CONTACTS_H
#define CONTACTS_H
#include "db_connection.h"

#include <QList>
#include <QString>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
/**
 * \file _Contacts.h
 * \brief Класс сущности "Контакты"
*/
enum DbState { IsReaded = 1,        ///< считан из БД
               IsNewing = 0,        ///< новый, еще не записан в БД
               IsRemoved = -1 ,     ///< удален в ПО, еще не удален в БД
               IsChanged = 2};       ///< считан из БД и изменен в ПО
class Contacts
{
private:

public:

    Contacts();                                 /// Пустой конструктор класса
    Contacts(int cont_id, QString tel, QString mark, int ot_id, DbState st = IsReaded); /// Конструктор класса с тремя переменными
    ~Contacts();

    int parent_OT_id;      ///
    int contact_id;     /// ID записи в таблице, соотвествует DbState
    QString contact_tel_num;    /// Номер Телефона
    QString mark;       /// Пометка контакта
    DbState cont_state;
    db_connection *db = db_connection::instance(); ///Подключение, используется у всех Query

    static bool selectAll(QList<Contacts*> *list);
    static bool saveAll_cont(QList<Contacts*> *list, int new_tel_id);
    static bool selectTelContacts(QList<Contacts*> *list, int);
    static bool selectContactsforEdit(QList<Contacts*> *list, int);
    static bool selectOffTel(QList<Contacts*> *list);

    bool insert(bool setState=true, int new_tel_id = 0 );
    bool update(bool setState=true);
    bool remove();

    void check() const;
};


#endif // CONTACTS_H
