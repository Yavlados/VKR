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
                   "zk.time_add,"
                   "zk.date_upd "
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
        cr->date_upd = query.value(19).toString();
        cr->state = IsReaded;
        switch (sqlt)
        {
            case PSQLtype:
            if(frm_st!=Main_window_for_Update)
                fill_owners_tel_list(cr->owt(), crud_id, counter_crud, PSQLtype);
                break;
            case SQLliteType:
                fill_owners_tel_list(cr->owt(), crud_id, counter_crud, SQLliteType);
                break;
            case SQLlitechipher:
                fill_owners_tel_list(cr->owt(), crud_id, counter_crud, SQLlitechipher);
                break;
        }
        switch (frm_st)
        {
        case Export:
                    cr->zk_id = counter_crud;
            break;
        case Import:
                    cr->zk_id = counter_crud;
            break;
        case Analysis:
                    cr->zk_id = counter_crud;
            break;
        case Main_window_for_Update:
                    cr->zk_id = query.value(0).toInt();
                    fill_owners_tel_list(cr->owt(), cr->zk_id, counter_crud, PSQLtype);
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
    query.prepare(" SELECT owners_tel.Telephone_num, owners_tel.Telephone_id, owners_tel.FK_Telephone_Zk, owners_tel.internum, owners_tel.oldnum "
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
            ot = new Owners_tel(query.value(0).toString(), counter_tel, new_zk, query.value(3).toBool(), query.value(4).toBool(), IsReaded);
            ///Экспорт: новый номер, новый айди, новый парентАйди
            break;
        case Import:
            ot = new Owners_tel(query.value(0).toString(), query.value(1).toInt(), query.value(2).toInt() , query.value(3).toBool(), query.value(4).toBool(), IsReaded);
            ///Импорт: Достаю данные как в базе
            break;
        case Analysis:
            ot =new Owners_tel(query.value(0).toString(), query.value(1).toInt(), query.value(2).toInt() , query.value(3).toBool(), query.value(4).toBool(),  IsReaded);
            ///Анализ: Достаю данные как в базе
            break;
        case Main_window_for_Update:
            ot = new Owners_tel(query.value(0).toString(), query.value(1).toInt(), query.value(2).toInt() , query.value(3).toBool(), query.value(4).toBool(),  IsReaded);
            ///Импорт: Достаю данные как в базе
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
                 "contacts.FK_Cl_telephone,"
                  " contacts.internum,"
                  " contacts.oldnum"
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
            cnt = new Contacts(query.value(0).toInt(), query.value(1).toString(), query.value(2).toString(),new_tel_id, query.value(4).toBool(), query.value(5).toBool(), IsReaded);
            ///Экспорт: новый номер, новый айди, новый парентАйди
            break;
        case Import:
           cnt = new Contacts(query.value(0).toInt(), query.value(1).toString(), query.value(2).toString(), query.value(3).toInt(), query.value(4).toBool(), query.value(5).toBool(), IsReaded);
            ///Импорт: Достаю данные как в базе
            break;
        case Analysis:
            cnt = new Contacts(query.value(0).toInt(), query.value(1).toString(), query.value(2).toString(), query.value(3).toInt(), query.value(4).toBool(), query.value(5).toBool(), IsReaded);
            ///Анализ: Достаю данные как в базе
            break;
        case Main_window_for_Update:
           cnt = new Contacts(query.value(0).toInt(), query.value(1).toString(), query.value(2).toString(), query.value(3).toInt(), query.value(4).toBool(), query.value(5).toBool(), IsReaded);
            ///Импорт: Достаю данные как в базе
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
                   "zk.time_add,"
                   "zk.date_upd "
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
        cr->date_upd = query.value(19).toString();
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
               "zk.time_add,"
               "zk.date_upd "
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
    cr->date_upd = querry.value(19).toString();
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

bool List_master::insert_crud_in_db(QList<Crud *> *crud)
{
    if(crud == nullptr || !db_connection::instance()->db_connect())
        return false;

    QString cname = db_connection::instance()->db().connectionName();

    bool isOk = db_connection::instance()->db().database(cname).transaction();

    QSqlQuery query(db_connection::instance()->db());
    QSqlQuery query1(db_connection::instance()->db());
    QSqlQuery query2(db_connection::instance()->db());
    ///Заполнение созданных таблиц
    for(int i = 0; i < crud->size(); i++)
    {
        if ( !isOk )
            break;

       query.prepare("INSERT INTO zk "
                      "(Lastname, Name, Mid_name, Birth_date,"
                      "Reg_city,Reg_street,Reg_home,Reg_corp,"
                      "Reg_flat,"
                           "Liv_city,Liv_street,Liv_home,Liv_corp,"
                           "Liv_flat,"
                           "Check_for, Dop_info,"
                           "Date_add, Time_add) "
                           " VALUES ((:lastname),(:name),(:mid_name), (:b_d),"
                           "(:r_c),(:r_s),(:r_h),(:r_corp),(:r_f),"
                           "(:l_c),(:l_s),(:l_h),(:l_corp),(:l_f),"
                           "(:c_f),(:d_i),"
                           "(:d_a), (:t_a)) RETURNING zk_id");
       query.bindValue(":lastname",crud->at(i)->lastname);
       query.bindValue(":name",crud->at(i)->name);
       query.bindValue(":mid_name",crud->at(i)->mid_name);
       query.bindValue(":b_d", crud->at(i)->birth_date);

       query.bindValue(":r_c",crud->at(i)->reg_city);
       query.bindValue(":r_s",crud->at(i)->reg_street);
       query.bindValue(":r_h",crud->at(i)->reg_home);
       query.bindValue(":r_corp",crud->at(i)->reg_corp);
       query.bindValue(":r_f",crud->at(i)->reg_flat);

       query.bindValue(":l_c",crud->at(i)->liv_city);
       query.bindValue(":l_s",crud->at(i)->liv_street);
       query.bindValue(":l_h",crud->at(i)->liv_home);
       query.bindValue(":l_corp",crud->at(i)->liv_corp);
       query.bindValue(":l_f",crud->at(i)->liv_flat);

       query.bindValue(":c_f",crud->at(i)->check_for);
       query.bindValue(":d_i",crud->at(i)->dop_info);

       query.bindValue(":d_a",crud->at(i)->date_add);
       query.bindValue(":t_a",crud->at(i)->time_add);

       if(!query.exec())
        {
        qDebug() << query.lastError();
            isOk = false;
        }
   while (query.next())
   {
       for (int a = 0; a < crud->at(i)->owt()->size(); a++)
       {
           if (!crud->at(i)->owt()->at(a)->tel_num.isEmpty())
           {
               query1.prepare("INSERT INTO owners_tel( Telephone_num, FK_Telephone_Zk, internum, oldnum) "
                              " VALUES ((:tel_num), (:fk_id), (:i_n), (:o_n)) RETURNING telephone_id");
               query1.bindValue(":tel_num",crud->at(i)->owt()->at(a)->tel_num);
               query1.bindValue(":fk_id",query.value(0).toInt());
               query1.bindValue(":i_n",crud->at(i)->owt()->at(a)->internum);
               query1.bindValue(":o_n",crud->at(i)->owt()->at(a)->oldnum);

               if (!query1.exec())
               {
                   qDebug() << query1.lastError();
                   isOk = false;
               }
               while (query1.next())
               {
                   qDebug()<< crud->at(i)->owt()->at(a)->cont()->size();
                   for (int b=0; b<crud->at(i)->owt()->at(a)->cont()->size();b++)
                   {
                    if(!crud->at(i)->owt()->at(a)->cont()->at(b)->contact_tel_num.isEmpty())
                       {
                           query2.prepare("INSERT INTO contacts (cl_telephone, cl_info, FK_Cl_telephone,internum, oldnum ) VALUES ( (:tel_num), (:mark), (:fk_id), (:i_n), (:o_n))");
                           query2.bindValue(":tel_num",crud->at(i)->owt()->at(a)->cont()->at(b)->contact_tel_num);
                           query2.bindValue(":mark",crud->at(i)->owt()->at(a)->cont()->at(b)->mark);
                           query2.bindValue(":fk_id",query1.value(0).toInt());
                           query2.bindValue(":i_n",crud->at(i)->owt()->at(a)->cont()->at(b)->internum);
                           query2.bindValue(":o_n",crud->at(i)->owt()->at(a)->cont()->at(b)->oldnum);

                           if (!query2.exec())
                           {
                               qDebug() << query2.lastError();
                               isOk = false;
                           }
                       }

                   }
               }
           }
       }
   }
  }
    if(!isOk)
    {
        db_connection::instance()->db().database(cname).rollback();
        qDebug() << "отсюда";
        return false;
    }
    db_connection::instance()->db().database(cname).commit();

    return true;
}

bool List_master::del_zk_from_pg(QList<int> del_list)
{
    if(del_list.isEmpty() || !db_connection::instance()->db_connect())
        return false;

    for (int i =0; i < del_list.size(); i++)
    {
        Crud::del_zk(del_list.at(i));
    }
    return true;
}

QList<Crud *> *List_master::search(QString search_query)
{
    QSqlQuery temp(db_connection::instance()->db());
    temp.prepare("SELECT  DISTINCT "        //стринг для модели
                 "zk.Zk_id,"
                 "zk.Lastname,"
                 "zk.Name,"
                 "zk.Mid_name,"
                 "zk.Birth_date,"
                 ""
                 "zk.Reg_city,"
                 "zk.Reg_street,"
                 "zk.Reg_home,"
                 "zk.Reg_corp,"
                 "zk.Reg_flat,"
                 ""
                 "zk.Liv_city,"
                 "zk.Liv_street,"
                 "zk.Liv_home,"
                 "zk.Liv_corp,"
                 "zk.Liv_flat,"
                 ""
                 "zk.Check_for,"
                 "zk.Dop_info,"
                 "zk.Date_add,"
                 "zk.Time_add,"
                 "zk.date_upd"
                 " FROM "
                 "zk,owners_tel"
                 " WHERE"
                 " zk.Zk_id>0 " + search_query + " ORDER BY zk.Zk_id");
    if (!temp.exec())
    {
        qDebug() << temp.lastError();
        qDebug() << "select_search";
        qDebug() << temp.executedQuery();
        return nullptr;
    }
    qDebug() << temp.executedQuery();
    QList<Crud*> *crudlist = new QList<Crud*>;

    while (temp.next())
    {
        Crud *cr = new Crud();
        cr->checkState_ = Checked;
        cr->zk_id = temp.value(0).toInt();
        cr->lastname = temp.value(1).toString();
        cr->name = temp.value(2).toString();
        cr->mid_name= temp.value(3).toString();
        cr->birth_date = temp.value(4).toString();

        cr->reg_city = temp.value(5).toString();;
        cr->reg_street = temp.value(6).toString();
        cr->reg_home = temp.value(7).toString();
        cr->reg_corp = temp.value(8).toString();
        cr->reg_flat = temp.value(9).toString();

        cr->liv_city = temp.value(10).toString();;
        cr->liv_street = temp.value(11).toString();
        cr->liv_home = temp.value(12).toString();
        cr->liv_corp = temp.value(13).toString();
        cr->liv_flat = temp.value(14).toString();

        cr->check_for = temp.value(15).toString();
        cr->dop_info = temp.value(16).toString();
        cr->date_add = temp.value(17).toString();
        cr->time_add = temp.value(18).toString();
        cr->date_upd = temp.value(19).toString();
        cr->state = IsReaded;
        crudlist->append(cr);
    }
    return crudlist;
}
