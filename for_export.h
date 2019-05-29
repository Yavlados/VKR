#ifndef FOR_EXPORT_H
#define FOR_EXPORT_H
#include "_Crud.h"/// ->_Owners_tel.h->_Contacts.h->db_connrction
#include "_Off_tels.h"
#include "Import_Form.h"

#include <QString>
#include <QFile>
#include <QMessageBox>

enum Import_state { Password_incorrect = 0,
                    Import_succesful = 1
};

class For_export
{
public:
    Import_Form *import = new Import_Form;

    For_export();
    void fill_crud_list(QList<Crud*> *crud, int crud_id, SqlType);
    void fill_owners_tel_list(QList<Owners_tel*> *owner_telLIST, int zk_id, int new_zk, int new_tel, SqlType);
    void fill_contacts_list(QList<Contacts*> *contactLIST, int tel_id, int new_tel_id, SqlType);

    void fill_off_tels( QList<Off_tels*>  *offtel, SqlType);

    bool Do_export(QString filename, QList<Crud*> *crud, QString password , bool cb_checked, bool cb_set_password, QList<Off_tels*> *offtel);
    Import_state Testing_open_db(QString filename, QString password);
    bool fill_all_crud_list(QList<Crud*> *crud, SqlType);

    QList<Crud*>        *crud; ///Локальный список
    QList<Off_tels*>  *offtel;

signals:
    void send_list(QList<Crud*> crudlist);

private:
     int counter_crud; ///будем передавать в качестве id
     int counter_tel;
    QFile db_file;
};

#endif // FOR_EXPORT_H
