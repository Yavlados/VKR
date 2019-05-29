#include "for_export.h"

For_export::For_export()
{
     counter_crud = 0; ///будем передавать в качестве id
     counter_tel = 0;
}

void For_export::fill_crud_list(QList<Crud*> *crud, int crud_id, SqlType sqlt)
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
                   " AND zk . Zk_id =(:id)"
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
                fill_owners_tel_list(cr->owt(), crud_id, counter_crud, counter_tel, PSQLtype);
                break;
            case SQLliteType:
                fill_owners_tel_list(cr->owt(), crud_id, counter_crud, counter_tel, SQLliteType);
                break;
        }
        crud->append(cr);
        counter_crud++;
    }
}

void For_export::fill_owners_tel_list(QList<Owners_tel *> *owner_telLIST, int zk_id,int new_zk, int new_tel,  SqlType sqlt)
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
        Owners_tel *ot = new Owners_tel(query.value(0).toString(), new_tel, new_zk, IsReaded);///Номер, новый айди, новый парентАйди
        switch (sqlt)
        {
            case PSQLtype:
                fill_contacts_list(ot->cont(), query.value(1).toInt(), new_tel, PSQLtype);
                break;
            case SQLliteType:
                fill_contacts_list(ot->cont(), query.value(1).toInt(), new_tel, SQLliteType);
                break;
        }
        owner_telLIST->append(ot);
        new_zk++;
    }
}

void For_export::fill_contacts_list(QList<Contacts *> *contactLIST, int tel_id, int new_tel_id, SqlType sqlt)
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
        Contacts *cnt = new Contacts(query.value(0).toInt(), query.value(1).toString(), query.value(2).toString(),new_tel_id, IsReaded);
        contactLIST->append(cnt);
    }
}

void For_export::fill_off_tels(QList<Off_tels *> *offtel, SqlType sqlt)
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

}

bool For_export::Do_export(QString filename, QList<Crud *> *crud, QString password,
                           bool cb_checked, bool cb_set_password, QList<Off_tels*>  *offtel)
{
     db_file.setFileName(filename);
    db_connection *db = db_connection::instance();

    if (cb_set_password)
    {
        db->set_Sql_type(SQLlitechipher);
        db->db().setPassword(password);
    }
    else
        db->set_Sql_type(SQLliteType);


    ///Создание таблиц
 if (db_file.open(QIODevice::WriteOnly))
     {
         db->db().setDatabaseName(db_file.fileName());

         qDebug() << db->db_connect()<<db->db().lastError();
         QSqlQuery query(db->db());

         if(!query.exec("CREATE TABLE zk"
                        " ( zk_id integer NOT NULL,"
                        " lastname character varying(20),"
                        " name character varying(10),"
                        " mid_name character varying(20),"
                        " birth_date character varying(12),"
                        " date_add character varying(10),"
                        " reg_city character varying(20),"
                        " reg_street character varying(20),"
                        " reg_home character varying(5),"
                        " reg_corp character varying(5),"
                        " reg_flat character varying(5),"
                        " liv_city character varying(20),"
                        " liv_street character varying(20),"
                        " liv_home character varying(5),"
                        " liv_corp character varying(5),"
                        " liv_flat character varying(5),"
                        " time_add character varying(10),"
                        " dop_info character varying(50), "
                        " check_for character varying(25),"
                        " CONSTRAINT PK_Zk_id PRIMARY KEY (zk_id),"
                        " CONSTRAINT Zk_Zk_id_key UNIQUE (zk_id)"
                        " );"))
             qDebug() << query.lastError();

         if(!query.exec(" CREATE TABLE owners_tel"
                        " ( telephone_id integer NOT NULL, "
                        " fk_telephone_zk integer, "
                        " telephone_num character varying(11) NOT NULL, "
                        " CONSTRAINT PK_telephone_id PRIMARY KEY (telephone_id),"
                        " CONSTRAINT FK_Telephone_ZK FOREIGN KEY (fk_telephone_zk)"
                        "  REFERENCES zk (zk_id) MATCH SIMPLE"
                        " ON UPDATE CASCADE ON DELETE CASCADE,"
                        " CONSTRAINT Owners_tel_Telephone_id_key UNIQUE (telephone_id),"
                        " CONSTRAINT Owners_tel_Telephone_num_key UNIQUE (telephone_num)"
                        " ); "))
             qDebug() << query.lastError();

         if(!query.exec(" CREATE TABLE contacts"
                        " ("
                        " contact_list_id integer NOT NULL, "
                        " cl_telephone character varying(11), "
                        " cl_info character varying(50), "
                        " fk_cl_telephone integer, "
                        " CONSTRAINT PK_Contact_list_id PRIMARY KEY (contact_list_id),"
                        " CONSTRAINT FK_Cl_telephone FOREIGN KEY (fk_cl_telephone) "
                        " REFERENCES owners_tel (telephone_id) MATCH SIMPLE"
                        " ON UPDATE CASCADE ON DELETE CASCADE,"
                        " CONSTRAINT Contacts_Contact_list_id_key UNIQUE (contact_list_id)"
                        " );"))
             qDebug() << query.lastError();

         ///Заполнение созданных таблиц
         for(int i = 0; i < crud->size(); i++)
         {
            query.prepare("INSERT INTO zk "
                           "(Lastname, Name,Mid_name, Birth_date,"
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
             }
        if(query.next())
        {
           QSqlQuery query2(db->db());
            for (int a = 0; a < crud->at(i)->owt()->size(); a++)
            {
                query2.prepare("INSERT INTO owners_tel( Telephone_num, FK_Telephone_Zk) "
                              " VALUES ((:tel_num), (:fk_id)) RETURNING telephone_id");
                query2.bindValue(":tel_num",crud->at(i)->owt()->at(a)->tel_num);
                query2.bindValue(":fk_id",query.value(0).toInt());

                if (!query2.exec())
                {
                    qDebug() << query2.lastError();
                }
                if (query2.next())
                {
                    QSqlQuery query3(db->db());
                    for (int b=0; b<crud->at(i)->owt()->at(a)->cont()->size();b++)
                    {
                        query3.prepare("INSERT INTO contacts (cl_telephone, cl_info, FK_Cl_telephone) VALUES ( (:tel_num), (:mark), (:fk_id))");
                        query3.bindValue(":tel_num",crud->at(i)->owt()->at(a)->cont()->at(b)->contact_tel_num);
                        query3.bindValue(":mark",crud->at(i)->owt()->at(a)->cont()->at(b)->mark);
                        query3.bindValue(":fk_id", query2.value(0).toInt());

                        if (!query3.exec())
                        {
                            qDebug() << query3.lastError();
                        }

                    }
               }
            }
        }
            query.clear();
       }
         ///Если захотели служебные телефоны
         if (cb_checked == true)
         {
           if (!query.exec("CREATE TABLE official_tel"
                       "("
                        " tel_num character(20),"
                       "  service_name character(25),"
                        " of_t_id integer NOT NULL,"
                       "  CONSTRAINT PK_of_t_id PRIMARY KEY (of_t_id)"
                       ")"))
               qDebug()<< query.lastError();
           query.clear();
            fill_off_tels(offtel, SQLliteType); //Заполнил список служебными телефонами
            for (int y = 0;y < offtel->size(); y++)
            {
                query.prepare("INSERT INTO official_tel (tel_num,"
                              " service_name) VALUES ((:t_n), (:s_n))");
                query.bindValue(":t_n", offtel->at(y)->tel_num);
                query.bindValue(":s_n", offtel->at(y)->service_name);
                if(!query.exec())
                    qDebug() << query.lastError();
            }
         }
     }
     db_file.close();
     db->set_Sql_type(PSQLtype); /// Перевожу обратно на PSQL, тк работаю в основном с ним
     qDebug() << db->db_connect()<<db->db().lastError();
     return true;
}

Import_state For_export::Testing_open_db(QString filename, QString password)
{
  // QList <Crud*> *crudlist = new QList <Crud*>;
//    QList <Owners_tel*> *OTlist = new QList <Owners_tel*>;
//    QList <Contacts*> *contlist = new QList <Contacts*>;
   // QList<Off_tels*>  *offtel = new  QList<Off_tels*>;

       db_file.setFileName(filename);
       db_connection *db = db_connection::instance();
       db->set_Sql_type(SQLliteType);
       db->db().setDatabaseName(db_file.fileName());

    if (!password.isNull() && !db->db().open("user",password))
        return Password_incorrect;
    else
   {
       qDebug() << db->db_connect()<<db->db().lastError();
       QSqlQuery query(db->db());

       fill_all_crud_list(crud, SQLliteType);

       import->recieve_all(crud);

       db_file.close();
       db->set_Sql_type(PSQLtype); /// Перевожу обратно на PSQL, тк работаю в основном с ним
       qDebug() << db->db_connect()<<db->db().lastError();
       import->show();
       return Import_succesful;
    }
}

/// В отличии от fill_crud_list собирает список всех ЗК и соответствующих
/// телефонов из лайтбд
bool For_export::fill_all_crud_list(QList<Crud *> *crud, SqlType sqlt)
{
    if(crud != nullptr   )
      crud->clear();

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
                     " ORDER BY zk.zk_id");
      if(!query.exec())
          qDebug() << query.lastError();
      while (query.next())
      {
          Crud *cr = new Crud();
          cr->zk_id = query.value(0).toInt();
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
//          switch (sqlt)
//          {
//              case PSQLtype:
//                  fill_owners_tel_list(cr->owt(), cr->zk_id, PSQLtype);
//                  break;
//              case SQLliteType:
//                  fill_owners_tel_list(cr->owt(), cr->zk_id, SQLliteType);
//                  break;
//          }
          crud->append(cr);
      }
}

