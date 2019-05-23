#ifndef FOR_EXPORT_H
#define FOR_EXPORT_H
#include "_Crud.h"/// ->_Owners_tel.h->_Contacts.h->db_connrction

#include <QString>
#include <QFile>

class For_export
{
public:
    For_export();
    void fill_crud_list(QList<Crud*> *crud, int crud_id);
    void fill_owners_tel_list(QList<Owners_tel*> *owner_telLIST, int zk_id);
    void fill_contacts_list(QList<Contacts*> *contactLIST, int tel_id);
    bool Do_export(QString filename, QList<Crud*> *crud, QString password);
    bool Testing_open_db(QString filename, QString password);
    QList<Crud*>        *crud; ///Локальный список

private:

    QFile db_file;
};

#endif // FOR_EXPORT_H
