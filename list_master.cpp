#include "list_master.h"

List_master::List_master()
{
}

List_master::List_master(Form_state st)
{
    frm_st = st;
}

void List_master::fill_crud_list(QList<Crud *> *crud, int crud_id, SqlType sqlt)
{
    db_connection *db = db_connection::instance();
    db->set_Sql_type(sqlt);
    QSqlQuery query(db->db());
    query.prepare("SELECT "
                   "zk.zk_id,"
                   "zk.lastname,"
                   "zk.name,"
                   "zk.mid_name,"
                   "zk.birth_date,"
                   ""
                   "zk.reg_city,"
                   "zk.reg_street,"
                   "zk.reg_home,"
                   "zk.reg_corp,"
                   "zk.reg_flat,"
                   ""
                   "zk.liv_city,"
                   "zk.liv_street,"
                   "zk.liv_home,"
     /*12*/        "zk.liv_corp,"
                   "zk.liv_flat,"
                   ""
                   "zk.check_for,"
                   "zk.dop_info,"
                   "zk.date_add,"
                   "zk.time_add "
                   " FROM "
                   " zk "
                   " WHERE zk . Zk_id =(:id)"
                   " ORDER BY zk.zk_id");
    query.bindValue(":id", crud_id);
    if(!query.exec())
        qDebug() << query.lastError();
    while (query.next())
    {
        Crud *cr = new Crud();
        cr->zk_id = counter_crud;
        cr->lastname = query.value(1).toString();
        cr->name = query.value(2).toString();
        cr->mid_name= query.value(3).toString();
        cr->birth_date = query.value(4).toString();

        cr->reg_city = query.value(5).toString();;
        cr->reg_street = query.value(6).toString();
        cr->reg_home = query.value(7).toString();
        cr->reg_corp = query.value(8).toString();
        cr->reg_flat = query.value(9).toString();

        cr->liv_city = query.value(10).toString();;
        cr->liv_street = query.value(11).toString();
        cr->liv_home = query.value(12).toString();
        cr->liv_corp = query.value(13).toString();
        cr->liv_flat = query.value(14).toString();

        cr->check_for = query.value(15).toString();
        cr->dop_info = query.value(16).toString();
        cr->date_add = query.value(17).toString();
        cr->time_add = query.value(18).toString();
        cr->state = IsReaded;
        switch (sqlt)
        {
            case PSQLtype:
                fill_owners_tel_list(cr->owt(), crud_id, counter_crud, PSQLtype);
                break;
            case SQLliteType:
                fill_owners_tel_list(cr->owt(), crud_id, counter_crud, SQLliteType);
                break;
            case SQLlitechipher:
                fill_owners_tel_list(cr->owt(), crud_id, counter_crud, SQLlitechipher);
                break;
        }
        crud->append(cr);
        counter_crud++;

    }
}

void List_master::fill_owners_tel_list(QList<Owners_tel *> *owner_telLIST, int zk_id, int new_zk, SqlType sqlt)
{

    db_connection *db = db_connection::instance();
    db->set_Sql_type(sqlt);
    QSqlQuery query(db->db());
    query.prepare(" SELECT owners_tel.Telephone_num, owners_tel.Telephone_id, owners_tel.FK_Telephone_Zk "
                 " FROM  owners_tel "
                 " WHERE owners_tel.FK_Telephone_Zk = (:id)");
    query.bindValue(":id", zk_id);
    if (!query.exec())
    {
        qDebug() <<  "   "+ query.executedQuery();
        qDebug() <<"selectZkTel";
    }

    while (query.next())
    {
        Owners_tel *ot;

        switch (frm_st)
        {
        case Export:
            ot = new Owners_tel(query.value(0).toString(), counter_tel, new_zk, IsReaded);
            ///Экспорт: новый номер, новый айди, новый парентАйди
            break;
        case Import:
            ot = new Owners_tel(query.value(0).toString(), query.value(1).toInt(), query.value(2).toInt() , IsReaded);
            ///Импорт: Достаю данные как в базе
            break;
        case Analysis:
            ot =new Owners_tel(query.value(0).toString(), query.value(1).toInt(), query.value(2).toInt() , IsReaded);
            ///Анализ: Достаю данные как в базе
            break;
        }

        switch (sqlt)
        {///Импорт с постгреса делается в случае выявления повторов и корректировки списка импорта
            case PSQLtype: ///Нет необходимости собирать контакты для выявления повторов
            if (frm_st == Import)
                break;
            else
                fill_contacts_list(ot->cont(), query.value(1).toInt(), counter_tel, PSQLtype);
            break;

            case SQLliteType:
                fill_contacts_list(ot->cont(), query.value(1).toInt(), counter_tel, SQLliteType);
                break;

            case SQLlitechipher:
                fill_contacts_list(ot->cont(), query.value(1).toInt(), counter_tel, SQLlitechipher);
                break;
        }
        owner_telLIST->append(ot);
        counter_tel++;
    }
}

void List_master::fill_contacts_list(QList<Contacts *> *contactLIST, int tel_id, int new_tel_id, SqlType sqlt)
{

    db_connection *db = db_connection::instance();
    db->set_Sql_type(sqlt);
    QSqlQuery query(db->db());

    query.prepare("SELECT "
                 "contacts.contact_list_id,"
                 "contacts.cl_telephone,"
                 "contacts.cl_info, "
                 "contacts.FK_Cl_telephone"
                 " FROM contacts"
                " WHERE contacts.FK_Cl_telephone = (:id) ");
    query.bindValue(":id",tel_id);
    if (!query.exec())
    {
        qDebug() << query.lastError();
        return;
    }

    while (query.next())
    {
        Contacts *cnt;
        switch (frm_st)
        {
        case Export:
            cnt = new Contacts(query.value(0).toInt(), query.value(1).toString(), query.value(2).toString(),new_tel_id, IsReaded);
            ///Экспорт: новый номер, новый айди, новый парентАйди
            break;
        case Import:
           cnt = new Contacts(query.value(0).toInt(), query.value(1).toString(), query.value(2).toString(), query.value(3).toInt(), IsReaded);
            ///Импорт: Достаю данные как в базе
            break;
        case Analysis:
            cnt = new Contacts(query.value(0).toInt(), query.value(1).toString(), query.value(2).toString(), query.value(3).toInt(), IsReaded);
            ///Анализ: Достаю данные как в базе
            break;
        }
        contactLIST->append(cnt);
    }
}

/// В отличии от fill_crud_list собирает список всех ЗК и соответствующих телефонов
bool List_master::fill_all_crud_list(QList<Crud *> *crud, SqlType sqlt)
{
    db_connection *db = db_connection::instance();
      db->set_Sql_type(sqlt);
    QSqlQuery query(db->db());
    qDebug() << db->db_connect()<<db->db().lastError();
    query.prepare("SELECT "
                   "zk.zk_id,"
                   "zk.lastname,"
                   "zk.name,"
                   "zk.mid_name,"
                   "zk.birth_date,"
                   ""
                   "zk.reg_city,"
                   "zk.reg_street,"
                   "zk.reg_home,"
                   "zk.reg_corp,"
                   "zk.reg_flat,"
                   ""
                   "zk.liv_city,"
                   "zk.liv_street,"
                   "zk.liv_home,"
     /*12*/        "zk.liv_corp,"
                   "zk.liv_flat,"
                   ""
                   "zk.check_for,"
                   "zk.dop_info,"
                   "zk.date_add,"
                   "zk.time_add "
                   " FROM "
                   " zk "
                   " ORDER BY zk.zk_id");
    if(!query.exec())
    {
      qDebug() << query.lastError() << query.lastError().text() << query.lastError().number();
        return false;
    }
    //qDebug() << query.executedQuery();
    while (query.next())
    {
        Crud *cr = new Crud();
        cr->zk_id = query.value(0).toInt();
        cr->lastname = query.value(1).toString();
        cr->name = query.value(2).toString();
        cr->mid_name= query.value(3).toString();
        cr->birth_date = query.value(4).toString();

        cr->reg_city = query.value(5).toString();
        cr->reg_street = query.value(6).toString();
        cr->reg_home = query.value(7).toString();
        cr->reg_corp = query.value(8).toString();
        cr->reg_flat = query.value(9).toString();

        cr->liv_city = query.value(10).toString();
        cr->liv_street = query.value(11).toString();
        cr->liv_home = query.value(12).toString();
        cr->liv_corp = query.value(13).toString();
        cr->liv_flat = query.value(14).toString();

        cr->check_for = query.value(15).toString();
        cr->dop_info = query.value(16).toString();
        cr->date_add = query.value(17).toString();
        cr->time_add = query.value(18).toString();
        cr->state = IsReaded;
        switch (sqlt)
        {
            case PSQLtype:
                fill_owners_tel_list(cr->owt(), cr->zk_id, counter_crud, PSQLtype);
                break;
            case SQLliteType:
                fill_owners_tel_list(cr->owt(), cr->zk_id,counter_crud, SQLliteType);
                break;
            case SQLlitechipher:
                fill_owners_tel_list(cr->owt(), cr->zk_id,counter_crud, SQLlitechipher);
                break;
        }
        crud->append(cr);
    }
    return  true;
}

void List_master::set_counters()
{
    counter_crud = 1; ///будем передавать в качестве id
    counter_tel = 1; ///в дампе первая зк начинается с 1 а не 0
}

Crud* List_master::get_crud(int zk_id)
{
    QSqlQuery querry(db_connection::instance()->db());
querry.prepare("SELECT "
               "zk.zk_id,"
               "zk.lastname,"
               "zk.name,"
               "zk.mid_name,"
               "zk.birth_date,"
               ""
               "zk.reg_city,"
               "zk.reg_street,"
               "zk.reg_home,"
               "zk.reg_corp,"
               "zk.reg_flat,"
               ""
               "zk.liv_city,"
               "zk.liv_street,"
               "zk.liv_home,"
 /*12*/        "zk.liv_corp,"
               "zk.liv_flat,"
               ""
               "zk.check_for,"
               "zk.dop_info,"
               "zk.date_add,"
               "zk.time_add "
               " FROM "
               " zk "
               " WHERE zk.zk_id = (:id)");
querry.bindValue(":id", zk_id);
if (!querry.exec())
    qDebug() << querry.lastError();
qDebug() << querry.executedQuery();

if (querry.next())
{
    Crud *cr = new Crud();
    cr->checkState_ = Checked;
    cr->zk_id = querry.value(0).toInt();
    cr->lastname = querry.value(1).toString();
    cr->name = querry.value(2).toString();
    cr->mid_name= querry.value(3).toString();
    cr->birth_date = querry.value(4).toString();

    cr->reg_city = querry.value(5).toString();;
    cr->reg_street = querry.value(6).toString();
    cr->reg_home = querry.value(7).toString();
    cr->reg_corp = querry.value(8).toString();
    cr->reg_flat = querry.value(9).toString();

    cr->liv_city = querry.value(10).toString();;
    cr->liv_street = querry.value(11).toString();
    cr->liv_home = querry.value(12).toString();
    cr->liv_corp = querry.value(13).toString();
    cr->liv_flat = querry.value(14).toString();

    cr->check_for = querry.value(15).toString();
    cr->dop_info = querry.value(16).toString();
    cr->date_add = querry.value(17).toString();
    cr->time_add = querry.value(18).toString();
    cr->state = IsReaded;

    fill_owners_tel_list(cr->owt(), cr->zk_id, counter_tel, PSQLtype);

    return cr;
}
else {
    return nullptr;
}

}

void List_master::fill_off_tels(QList<Off_tels *> *offtel, SqlType sqlt)
{
   if (offtel != nullptr)
        offtel->clear();

    db_connection *db = db_connection::instance();
    db->set_Sql_type(sqlt);
    QSqlQuery query(db->db());

    query.prepare("SELECT official_tel.of_t_id,"
                  "official_tel.tel_num,"
                  "official_tel.service_name"
                  " FROM official_tel");
    if (!query.exec())
    {
        qDebug() << query.lastError();
        return;
    }
    while (query.next())
    {
        Off_tels *cnt = new Off_tels(query.value(0).toInt(), query.value(1).toString(), query.value(2).toString());
        offtel->append(cnt);
    }
    db->db().close();
}
