#include "contacts.h"


void Contacts::append_contact(QString contact_tel)
{
    contacts_tel.append(contact_tel);
}


void Contacts::append_info(QString contact_info)
{
  contacts_info.append(contact_info);

}

void Contacts::add_contact_to_db()
{
    vector.push_back(contacts_tel);
    vector.push_back(contacts_info);

    for (int i =0; i< contacts_info.size(); i++)
    {
        qDebug() << vector.value(0).value(i) + "       "+vector.value(1).value(i);

        querry.prepare("INSERT INTO \"contacts\""
                       "(\"cl_telephone\", \"cl_info\",\"FK_Cl_telephone\")"
                       "VALUES((:telephone),(:info),(:zk_id))");
        querry.bindValue(":telephone", vector.value(0).value(i));
        querry.bindValue(":info", vector.value(1).value(i));
        querry.bindValue(":zk_id", owner_tel_id);
        if (!querry.exec())
           qDebug() << querry.lastError();
        querry.clear();
    }
}

void Contacts::temp_change_cont()
{
    vector.push_back(contacts_tel);
    vector.push_back(contacts_info);
    if (vector.size() > 0)
    {
for (int i=0; i < contacts_info.size(); i++)
{
      }

    }
    else {
        qDebug() << "list is empty";
    }
}

void Contacts::recieve_cont_id(QString cl_tel_num, QString cl_tel_info)
{
    querry.prepare("SELECT \"contacts\".\"Contact_list_id\""
                   "FROM \"contacts\""
                   "WHERE \"contacts\".\"cl_telephone\" = (:tel)"
                   "AND "
                   "\"contacts\".\"cl_info\" = (:info)");
    querry.bindValue(":tel", cl_tel_num);
    querry.bindValue(":info", cl_tel_info);
    if (!querry.exec())
        qDebug() << querry.lastError();
    while (querry.next())
       contact_id = querry.value(0).toInt();
    qDebug() << contact_id;

}

void Contacts::del_contact(int c_id)
{
    querry.prepare("DELETE FROM \"contacts\""
    "WHERE \"contacts\".\"Contact_list_id\" = (:con_id)");
    querry.bindValue(":con_id",c_id);
    if (!querry.exec())
        qDebug() << querry.lastError();
}

void Contacts::add_fk_contact(int tel_id)
{
    querry.prepare("UPDATE \"contacts\""
                   "SET \"FK_Cl_telephone\" = (:tel_id)"
                   "WHERE \"FK_Cl_telephone\" IS NULL");
    querry.bindValue(":tel_id",tel_id);
    if (!querry.exec())
        qDebug() << querry.lastError();
    querry.clear();
}
