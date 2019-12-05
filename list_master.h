#ifndef LIST_MASTER_H
#define LIST_MASTER_H
#include "_Crud.h"/// ->_Owners_tel.h->_Contacts.h->db_connrction
#include "_Off_tels.h"
#include "_Zk_links.h"

#include <QString>

enum Form_state { Export = 0,
                  Import = 1,
                  Analysis = 2,
                  Main_window_for_Update = 3};

/**
* \file list_master.h
* \brief Класс работы со списками
*/

class List_master           ///Класс работы со списками
{
public:
    List_master();
    List_master(Form_state st);


    ///
    /// Методы переехали из экспорта
    ///

    void fill_crud_list(QList<Crud*> *crud, int crud_id, SqlType);

     Crud* get_crud(int id);
     void fill_owners_tel_list(QList<Owners_tel*> *owner_telLIST, int zk_id, int new_zk, SqlType);
     void fill_contacts_list(QList<Contacts*> *contactLIST, int tel_id, int new_tel_id, SqlType);

    bool fill_all_crud_list(QList<Crud*> *crud, SqlType);
    void set_counters();
    void fill_off_tels(QList<Off_tels *> *offtel, SqlType sqlt);
    bool insert_crud_in_db(QList<Crud*>*crud , QList<int> *list_id = nullptr, QVector<QVector<int> > *vector = nullptr,QVector<QVector<QString>> *vector_str = nullptr, bool old_db = false);//Лист удобно использовать при добавлении, при импорте - вектор

    bool insert_off_tel_in_db(QList<Off_tels *> *offtel);

    bool del_zk_from_pg(QList<int> del_list);
    bool del_offt_from_pg(QList<int> del_list);

    void fill_links(QVector<QVector <QString> > *vector);

    static QList<Crud*>* search(QString search_query);
private:
    Form_state         frm_st;
    int counter_crud; ///будем передавать в качестве id
    int counter_tel;
};

#endif // LIST_MASTER_H
