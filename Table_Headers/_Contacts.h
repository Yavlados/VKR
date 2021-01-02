#ifndef CONTACTS_H
#define CONTACTS_H
#include "db_connection.h"

#include <QList>
#include <QString>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include "enums.h"
/**
 * \file _Contacts.h
 * \brief Класс сущности "Контакты"
*/

class Contacts          ///Класс сущности "Контакты"
{
private:

public:


    Contacts();                                 /// Пустой конструктор класса
    Contacts(int cont_id, QString tel, QString mark, int ot_id, bool i_n = false, bool o_n = false, DbState st = IsReaded, int linked_id = 0); /// Конструктор класса с тремя переменными
    ~Contacts();
    int parent_OT_id;      ///
    int contact_id;     /// ID записи в таблице, соотвествует DbState
    QString contact_tel_num;    /// Номер Телефона
    QString mark;       /// Пометка контакта
    bool internum;
    bool oldnum;    
    int linked_id;
    DbState cont_state;
    db_connection *db; ///Подключение, используется у всех Query

    static bool saveAll_cont(QList<Contacts*> *list, int new_tel_id);
    static bool selectTelContacts(QList<Contacts*> *list, int);
    static bool selectOffTel(QList<Contacts*> *list);
    static bool var2_analysis_for_main(QList<Contacts *> *list, int tel_id);

    bool insert(bool setState=true, int new_tel_id = 0 );
    bool update(bool setState=true);
    bool remove();

    void check() const;

    static bool delete_all(QList<Contacts*> *list);
};


#endif // CONTACTS_H
