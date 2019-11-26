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
    db_connection *db = db_connection::instance(); ///Подключение, используется у всех Query

    static bool saveAll_cont(QList<Contacts*> *list, int new_tel_id);
    static bool selectTelContacts(QList<Contacts*> *list, int);
    static bool selectOffTel(QList<Contacts*> *list);
    static bool var2_analysis_for_main(QList<Contacts *> *list, int tel_id);

    bool insert(bool setState=true, int new_tel_id = 0 );
    bool update(bool setState=true);
    bool remove();

    void check() const;
};


#endif // CONTACTS_H
