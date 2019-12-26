#include "for_export.h"
For_export::For_export()
{
    list = new List_master(Export);
}

bool For_export::Do_export(QString filename, QList<Crud *> *crud, QString password,
                           bool cb_checked, bool cb_set_password, QList<Off_tels*>  *offtel, zk_links *linklist)
{
    db_file.setFileName(filename); //Установка имени файлу дб

    QString newStr = Create_report_filename(filename);
    export_report.setFileName(newStr);

    qDebug() << db_file.fileName() << endl << export_report.fileName();
    db_connection *db = db_connection::instance();

    if (cb_set_password)
    {
        db->set_Sql_type(SQLlitechipher);
        db->db().setPassword(password);
    }
    else
        db->set_Sql_type(SQLliteType);


    ///Создание таблиц
 if (db_file.open(QIODevice::WriteOnly) && export_report.open(QIODevice::WriteOnly))
     {
         db->db().setDatabaseName(db_file.fileName());

         QTextStream in(&export_report);
         QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
         if(codec != nullptr)
           in.setCodec(codec);

         in << info_text;

         qDebug() << db->db_connect()<<db->db().lastError();
         QSqlQuery query(db->db());

         if(!crud->isEmpty())
         {
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
                        " date_upd character varying(20),"
                        " row_id uuid UNIQUE,"
                        " CONSTRAINT PK_Zk_id PRIMARY KEY (zk_id),"
                        " CONSTRAINT Zk_Zk_id_key UNIQUE (zk_id)"
                        " );"))
             qDebug() << query.lastError();



         if(!query.exec(" CREATE TABLE zk_links"
        " ("
          " row_id1 uuid,"
          " row_id2 uuid,"
          " CONSTRAINT row_id_1_zk FOREIGN KEY (row_id1)"
             "  REFERENCES zk (row_id) MATCH SIMPLE"
              " ON UPDATE CASCADE ON DELETE CASCADE,"
          " CONSTRAINT row_id_2_zk FOREIGN KEY (row_id2)"
              " REFERENCES zk (row_id) MATCH SIMPLE"
              " ON UPDATE CASCADE ON DELETE CASCADE"
         ")"))
                 qDebug() << query.lastError();

         if(!query.exec(" CREATE TABLE owners_tel"
                        " ( telephone_id integer NOT NULL, "
                        " fk_telephone_zk integer, "
                        " telephone_num character varying(11) NOT NULL, "
                        " internum boolean,"
                        " oldnum boolean,"
                        " CONSTRAINT PK_telephone_id PRIMARY KEY (telephone_id),"
                        " CONSTRAINT FK_Telephone_ZK FOREIGN KEY (fk_telephone_zk)"
                        "  REFERENCES zk (zk_id) MATCH SIMPLE"
                        " ON UPDATE CASCADE ON DELETE CASCADE,"
                        " CONSTRAINT Owners_tel_Telephone_id_key UNIQUE (telephone_id)"
                        " ); "))
             qDebug() << query.lastError();

         if(!query.exec(" CREATE TABLE contacts"
                        " ("
                        " contact_list_id integer NOT NULL, "
                        " cl_telephone character varying(11), "
                        " cl_info character varying(50), "
                        " fk_cl_telephone integer, "
                        " internum boolean,"
                        " oldnum boolean,"
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
                           "(Lastname, Name, Mid_name, Birth_date,"
                           "Reg_city,Reg_street,Reg_home,Reg_corp,"
                           "Reg_flat,"
                                "Liv_city,Liv_street,Liv_home,Liv_corp,"
                                "Liv_flat,"
                                "Check_for, Dop_info,"
                                "Date_add, Time_add, date_upd, row_id) "
                                " VALUES ((:lastname),(:name),(:mid_name), (:b_d),"
                                "(:r_c),(:r_s),(:r_h),(:r_corp),(:r_f),"
                                "(:l_c),(:l_s),(:l_h),(:l_corp),(:l_f),"
                                "(:c_f),(:d_i),"
                                "(:d_a), (:t_a), (:d_u), (:r_i))");
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
            query.bindValue(":d_u",crud->at(i)->date_upd);

            query.bindValue(":r_i",crud->at(i)->row_id);

            in << "\r\n";
            in << crud->at(i)->row_id;
            in <<"\t";
            in << crud->at(i)->lastname;
            in << "\t";
            in << crud->at(i)->name;
            in << "\t";
            in << crud->at(i)->mid_name;
            if(!query.exec())
             {
             qDebug() << query.lastError();
             }
        else
        {
          query.clear();

            for (int a = 0; a < crud->at(i)->owt()->size(); a++)
            {
                query.prepare("INSERT INTO owners_tel( Telephone_num, FK_Telephone_Zk, internum, oldnum) "
                              " VALUES ((:tel_num), (:fk_id), (:i_n), (:o_n))");
                query.bindValue(":tel_num",crud->at(i)->owt()->at(a)->tel_num);
                query.bindValue(":fk_id",crud->at(i)->owt()->at(a)->parentZK_id);
                query.bindValue(":i_n",crud->at(i)->owt()->at(a)->internum);
                query.bindValue(":o_n",crud->at(i)->owt()->at(a)->oldnum);

                if (!query.exec())
                {
                    qDebug() << query.lastError() << query.executedQuery() << crud->at(i)->owt()->at(a)->tel_num << crud->at(i)->owt()->at(a)->parentZK_id << crud->at(i)->owt()->at(a)->internum << crud->at(i)->owt()->at(a)->oldnum;
                }
                else
                {
                    query.clear();
                    qDebug()<< crud->at(i)->owt()->at(a)->cont()->size();
                    for (int b=0; b<crud->at(i)->owt()->at(a)->cont()->size();b++)
                    {
                        query.prepare("INSERT INTO contacts (cl_telephone, cl_info, FK_Cl_telephone, internum, oldnum) VALUES ( (:tel_num), (:mark), (:fk_id), (:i_n), (:o_n))");
                        query.bindValue(":tel_num",crud->at(i)->owt()->at(a)->cont()->at(b)->contact_tel_num);
                        query.bindValue(":mark",crud->at(i)->owt()->at(a)->cont()->at(b)->mark);
                        query.bindValue(":fk_id",crud->at(i)->owt()->at(a)->cont()->at(b)->parent_OT_id);
                        query.bindValue(":i_n",crud->at(i)->owt()->at(a)->cont()->at(b)->internum);
                        query.bindValue(":o_n",crud->at(i)->owt()->at(a)->cont()->at(b)->oldnum);
                        if (!query.exec())
                        {
                            qDebug() << query.lastError();
                        }

                    }
               }
            }
        }
            query.clear();
       }
         if (!linklist->links->isEmpty())
             for (int a = 0; a < linklist->links->size();a++)
             {
                 query.prepare("INSERT INTO zk_links (row_id1, row_id2) VALUES ( (:r_id1), (:r_id2))");
                 query.bindValue(":r_id1",linklist->links->at(a)->uuid1);
                 query.bindValue(":r_id2",linklist->links->at(a)->uuid2);
                 if (!query.exec())
                 {
                     qDebug() << query.lastError();
                 }

             }
    }
         ///Если захотели служебные телефоны
         if (!offtel->isEmpty())
         {
             in << "\r\n Следующие служебные телефоны:";
           if (!query.exec("CREATE TABLE official_tel"
                       "("
                        " of_t_id integer NOT NULL,"
                        " tel_num character(20),"
                       "  service_name character(25),"
                       "  CONSTRAINT PK_of_t_id PRIMARY KEY (of_t_id)"
                       ")"))
               qDebug()<< query.lastError();
           query.clear();
            //fill_off_tels(offtel, SQLliteType); //Заполнил список служебными телефонами
            for (int y = 0;y < offtel->size(); y++)
            {
                query.prepare("INSERT INTO official_tel (tel_num,"
                              " service_name) VALUES ((:t_n), (:s_n))");
                query.bindValue(":t_n", offtel->at(y)->tel_num);
                query.bindValue(":s_n", offtel->at(y)->service_name);
                if(!query.exec())
                   qDebug() << query.lastError();
                else
                    in << "\r\n "+offtel->at(y)->service_name;
            }
         }

         export_report.close();
     }
     db_file.close();
     db->db().close();
     db->set_Sql_type(PSQLtype); /// Перевожу обратно на PSQL, тк работаю в основном с ним
     qDebug() << db->db_connect()<<db->db().lastError();
     return true;
}

QString For_export::Create_report_filename(QString filename)
{
    QString date = QDate::currentDate().toString(Qt::ISODate);
    QString time = QTime::currentTime().toString(Qt::ISODate);
    QFileInfo fileInfo(db_file.fileName());
    QString filename_db(fileInfo.fileName());
    info_text = date + " в " + time + " была осуществлена выгрузка в файл "+filename_db + " следующих ЗК: ";

    time.replace(":","-");

    QString newStr = filename;

    newStr.remove(filename_db);
    newStr += "("+filename_db+")"+date + "_" + time+".txt";


    return newStr;
}
