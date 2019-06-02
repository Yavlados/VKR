#ifndef LIST_MASTER_H
#define LIST_MASTER_H
#include "_Crud.h"/// ->_Owners_tel.h->_Contacts.h->db_connrction
#include "_Off_tels.h"

#include <QString>

enum Form_state { Export = 0,
                  Import = 1,
                  Analysis = 2};

class List_master
{
public:
    List_master();
    List_master(Form_state st);
    ///
    /// Методы переехали из экспорта
    ///
    void fill_crud_list(QList<Crud*> *crud, int crud_id, SqlType);
    void fill_owners_tel_list(QList<Owners_tel*> *owner_telLIST, int zk_id, int new_zk, SqlType);
    void fill_contacts_list(QList<Contacts*> *contactLIST, int tel_id, int new_tel_id, SqlType);
    bool fill_all_crud_list(QList<Crud*> *crud, SqlType);
    void set_counters();
    Crud* get_crud(int id);
    void fill_off_tels(QList<Off_tels *> *offtel, SqlType sqlt);


private:
    Form_state         frm_st;
    int counter_crud; ///будем передавать в качестве id
    int counter_tel;
};

#endif // LIST_MASTER_H
