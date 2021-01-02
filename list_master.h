#ifndef LIST_MASTER_H
#define LIST_MASTER_H
#include "_Crud.h"/// ->_Owners_tel.h->_Contacts.h->db_connrction
#include "_Off_tels.h"
#include "_Zk_links.h"

#include <QString>
#include <QVector>

enum Form_state { Export = 0,
                  Import = 1,
                  Analysis = 2,
                  Main_window_for_Update = 3};

enum export_state { Password_abort,
                    Connection_trouble,
                    Success
                    };

/**
* \file list_master.h
* \brief Класс работы со списками
*/

class List_master           ///Класс работы со списками
{
public:
    List_master();
    List_master(Form_state st);

    ///Достает только список всех ЗК
    void fill_crud_list(QList<Crud*> *crud, int crud_id, SqlType);

    ///Достает одну ЗК
     Crud* get_crud(int id);

     ///Достает список всех телефонов, привязанных к ЗК
     void fill_owners_tel_list(QList<Owners_tel*> *owner_telLIST, int zk_id, int new_zk, SqlType);

     ///Достает список всех контактов, привязанных к телефону
     void fill_contacts_list(QList<Contacts*> *contactLIST, int tel_id, int new_tel_id, SqlType);

    /// Достает список ЗК, а также связанных с ними объектов (телефоны и контакты)
    export_state fill_all_crud_list(QList<Crud*> *crud, SqlType sqltype, QString password = 0, QString filename = 0);

    ///Достает список всех служебных телефонов
    export_state fill_off_tels(QList<Off_tels *> *offtel, SqlType sqlt, QString password = 0, QString filename = 0);

    ///Добавляет в базу список ЗК
    bool insert_crud_in_db(QList<Crud*>*crud , QList<int> *list_id = 0, QVector<QVector<int> > *vector = 0,QVector<QVector<QString>> *vector_str = 0, bool old_db = false);

    ///Добавляет в базу список служебных телефонов
    bool insert_off_tel_in_db(QList<Off_tels *> *offtel);

    ///Удаляет ЗК из базы
    bool del_zk_from_pg(QList<int> del_list);

    ///Удаляет список список служебных телефонов из базы
    bool del_offt_from_pg(QList<int> del_list);

    ///Выполняет поиск по условиям
    static QList<Crud*>* search(QString search_query);

    void fill_links(QVector<QVector <QString> > *vector);
    void set_counters();
private:
    Form_state         frm_st;
    int counter_crud; ///будем передавать в качестве id
    int counter_tel;
};

#endif // LIST_MASTER_H
