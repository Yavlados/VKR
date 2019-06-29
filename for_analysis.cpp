#include "for_analysis.h"

For_analysis::For_analysis()
{
    list = new List_master(Analysis);
}

QList<Crud *> *For_analysis::get_crud(Crud *cr, QString added_SQL)
{
    analysis_res.clear();

    QList<int> temp_zk_id;//лист для избежания повторов

    QList<Crud*> *crudlist = new QList <Crud*>; //Список для результатов анализа

    ///Делаю 3 запроса, согласно с вариантами знакомства
   crudlist = get_1_var(cr, crudlist, added_SQL);
   crudlist = get_2_var(cr, crudlist, added_SQL);
   crudlist = get_3_var(cr, crudlist, added_SQL);
           ///3 вариант - общий контакт
                return crudlist;
}

QList<Crud *> *For_analysis::get_1_var(Crud *cr, QList<Crud *> *crudlist, QString added_SQL)
{
    db_connection *db = db_connection::instance();
    QSqlQuery temp(db->db());
    QSqlQuery querry(db->db());
    analysis_res.clear();

    ///1 вариант - собираю людей, у которых в КОНТАКТАХ лежит номер анализируемого человека

        QString tempSQL =" SELECT zk.zk_id, S.TEL_NUM, S.CONTACT_MARK,"
                " S.OW_TEL_NUM "
                " FROM zk "
                " INNER JOIN (SELECT "
                " owners_tel.telephone_num AS TEL_NUM,"
                " Q.CONTACT_MARK AS CONTACT_MARK,"
                " owners_tel.telephone_id AS TEL_ID, Q.TEL_NUM AS OW_TEL_NUM"
                " FROM OWners_tel"
                " INNER join(SELECT contacts.cl_telephone AS CONTACT_NUM,"
                " contacts.cl_info AS CONTACT_MARK,"
                " contacts.fk_cl_telephone AS FK, "
                " OW.TEL_NUM AS TEL_NUM"
                " FROM contacts"
                " INNER JOIN (SELECT owners_tel.telephone_num AS TEL_NUM"
                " FROM owners_tel"
                " where owners_tel.fk_telephone_zk = (:zk_id)) AS OW"
                " ON OW.TEL_NUM = contacts.cl_telephone) AS Q"
                " ON Q.FK = owners_tel.telephone_id) AS S"
                " ON S.TEL_ID = zk.zk_id "
                " WHERE zk.zk_id >0 ";

        if(!added_SQL.isEmpty()) //Дополнительные условия запроса (ограничения по дате и ЗК)
            tempSQL+=added_SQL;

        querry.prepare(tempSQL);
        querry.bindValue(":zk_id", cr->zk_id);

        if (!querry.exec())
            qDebug() << querry.lastError();

        /// 0 - id
        /// 1- номер к которому привязан
        /// 2- пометка
        /// 3- номер анализируемого, который был обнаружен

        while (querry.next())
        {
            bool add_is_ready = false;//флаг апдейта
            if(crudlist->isEmpty())//пустой лист всегда получит запись
                goto metka;
            for (int a = 0 ;a<crudlist->size();a++)
            {
                if(crudlist->at(a)->zk_id == querry.value(0).toInt())    //Если запись совпадает
                {
                   ///Просто добавляю новый телефон в старую запись
                    Owners_tel *temp_ot = new Owners_tel(querry.value(1).toString(),1,crudlist->at(a)->zk_id);

                    Contacts *temp_cont = new Contacts(1, querry.value(3).toString(), querry.value(2).toString(),temp_ot->tel_id);
                    ///Собираем воедино
                    temp_ot->cont()->append(temp_cont);//Засунул контакт в телефон
                    crudlist->at(a)->owt()->append(temp_ot);
                    add_is_ready=true;
                    break;
                }
            }
            if(add_is_ready==false) //если флаг апдейта не изменился
            {
                metka: //То добавляю новую запись
                Crud *temp_crud = Crud::id_zk_search(querry.value(0).toInt());//Собираю информацию о владельце ЗК

                ///Беру его телефон с id = 1 (Значит телефон анализируемого обнаружен в контактах этого человека)
                Owners_tel *temp_ot = new Owners_tel(querry.value(1).toString(),1,temp_crud->zk_id);

                Contacts *temp_cont = new Contacts(1, querry.value(3).toString(), querry.value(2).toString(),temp_ot->tel_id);

                ///Собираем воедино
                temp_ot->cont()->append(temp_cont);
                temp_crud->owt()->append(temp_ot);
                crudlist->append(temp_crud);
            }
        }
        return crudlist;
}

QList<Crud *> *For_analysis::get_2_var(Crud *cr, QList<Crud *> *crudlist, QString added_SQL)
{
    db_connection *db = db_connection::instance();
    QSqlQuery temp(db->db());
    QSqlQuery querry(db->db());
    QString tempSQL;
    /// 2 вариант - в КОНТАКТАХ анализируемого человека обнаружен этот человек
          tempSQL +=" SELECT zk.zk_id, OWT.OWNER_TEL,"
                    " OWT.TEL_NUM, OWT.CONT_MARK"
                    " FROM zk"
                    " INNER JOIN (SELECT owners_tel.fk_telephone_zk AS FK_ZK,"
                    " owners_tel.telephone_num AS TEL_NUM,"
                    " CONT.CONT_MARK AS CONT_MARK,"
                    " CONT.OWNER_TEL AS OWNER_TEL"
                    " FROM owners_tel"
                    " INNER JOIN (SELECT contacts.cl_telephone AS CONT_TEL,"
                    " contacts.cl_info AS CONT_MARK,"
                    " tels.OWNER_TEL AS OWNER_TEL"
                    " FROM contacts "
                    " INNER JOIN(SELECT  DISTINCT owners_tel.telephone_id as tel_id,"
                    " owners_tel.telephone_num AS OWNER_TEL"
                    " FROM owners_tel"
                    " WHERE owners_tel.fk_telephone_zk = (:zk_id)) as tels"
                    " ON tels.tel_id = contacts.fk_cl_telephone) AS CONT"
                    " ON CONT.CONT_TEL = owners_tel.telephone_num) AS OWT"
                    " ON OWT.FK_ZK = zk.zk_id"
                    " WHERE ZK.zk_id>0 ";

         if(!added_SQL.isEmpty()) //Дополнительные условия запроса (ограничения по дате и ЗК)
             tempSQL+=added_SQL;

              querry.prepare(tempSQL);
              querry.bindValue(":zk_id", cr->zk_id);

              if (!querry.exec())
                  qDebug() << querry.lastError();

              /// 0 - id
              /// 1- номер анализируемого к которому привязан контакт
              /// 2- номер обнаруженного контакта
              /// 3- пометка

              while (querry.next())
              {
                    bool add_is_ready = false;//флаг апдейта
                      if(crudlist->isEmpty())//пустой лист всегда получит запись
                          goto metka;
                      for (int a = 0 ;a<crudlist->size();a++)
                      {
                          if(crudlist->at(a)->zk_id == querry.value(0).toInt())    //Если запись совпадает
                          {
                              ///Просто добавляю новый телефон в старую запись
                               Owners_tel *temp_ot = new Owners_tel(querry.value(1).toString(),2,crudlist->at(a)->zk_id);

                               Contacts *temp_cont = new Contacts(2, querry.value(2).toString(), querry.value(3).toString(),temp_ot->tel_id);
                               ///Собираем воедино
                               temp_ot->cont()->append(temp_cont);//Засунул контакт в телефон
                               crudlist->at(a)->owt()->append(temp_ot);
                              add_is_ready=true;
                              break;
                          }
                      }
                      if(add_is_ready==false) //если флаг апдейта не изменился
                      {
                          metka: //То добавляю новую запись
                          Crud *temp_crud = Crud::id_zk_search(querry.value(0).toInt());//Собираю информацию о владельце ЗК

                          ///Беру его телефон с id = 2 (ЗНАЧИТ ТЕЛЕФОН - НОМЕР АНАЛИЗИРУЕМОГО, В КОТОРОМ ОБНАРУЖЕН КОНТАКТ)
                          Owners_tel *temp_ot = new Owners_tel(querry.value(1).toString(),2,temp_crud->zk_id);

                          Contacts *temp_cont = new Contacts(2, querry.value(2).toString(), querry.value(3).toString(), temp_ot->tel_id);

                          ///Собираем воедино
                          temp_ot->cont()->append(temp_cont);
                          temp_crud->owt()->append(temp_ot);
                          crudlist->append(temp_crud);
                      }
                }
              return crudlist;
}

QList<Crud *> *For_analysis::get_3_var(Crud *cr, QList<Crud *> *crudlist, QString added_SQL)
{
    db_connection *db = db_connection::instance();
    QSqlQuery temp(db->db());
    QSqlQuery querry(db->db());
    QString tempSQL;

    tempSQL += " SELECT zk.zk_id, OW.TEL_NUM, OW.CONTACT_TEL, OW.CONTACT_MARK "
           " FROM zk"
           " INNER JOIN(SELECT  DISTINCT owners_tel.fk_telephone_zk as ID,"
           " owners_tel.telephone_num as TEL_NUM,"
           " Q.CONT_MARK AS CONTACT_MARK, "
           " Q.CONT_TEL AS CONTACT_TEL"
           " FROM owners_tel"
           " INNER JOIN"
           " (SELECT DISTINCT contacts.cl_telephone as CONT_TEL,"
           " contacts.contact_list_id as CONT_ID,"
           " contacts.fk_cl_telephone as CONT_FK,"
           " contacts.cl_info as CONT_MARK "
           " FROM contacts"
           " INNER JOIN (SELECT DISTINCT contacts.cl_telephone as TELEPHONE"
           " , contacts.contact_list_id as ID "
           " FROM contacts, owners_tel"
           " WHERE owners_tel.fk_telephone_zk = (:zk_id)) AS S"
           " ON S.TELEPHONE = contacts.cl_telephone"
           " WHERE S.ID != contacts.contact_list_id ) AS Q"
           " ON Q.CONT_FK = owners_tel.telephone_id"
           " WHERE owners_tel.fk_telephone_zk != (:zk_id)) AS OW"
           " ON OW.ID = zk.zk_id"
           " WHERE zk_id>0 ";

   if(!added_SQL.isEmpty()) //Дополнительные условия запроса (ограничения по дате и ЗК)
       tempSQL+=added_SQL;

        querry.prepare(tempSQL);
        querry.bindValue(":zk_id", cr->zk_id);

        if (!querry.exec())
            qDebug() << querry.lastError();

        ///Возвращаю 0 - id
        /// 1 - номер телефона с контактом
        /// 2 - номер контакта
        /// 3 - метку контакта
        while (querry.next())
        {
            bool add_is_ready = false;//флаг апдейта
              if(crudlist->isEmpty())//пустой лист всегда получит запись
                  goto metka;
              for (int a = 0 ;a<crudlist->size();a++)
              {
                  if(crudlist->at(a)->zk_id == querry.value(0).toInt())    //Если запись совпадает
                  {
                      ///Просто добавляю новый телефон в старую запись
                       Owners_tel *temp_ot = new Owners_tel(querry.value(1).toString(),3,crudlist->at(a)->zk_id);

                       Contacts *temp_cont = new Contacts(3, querry.value(2).toString(), querry.value(3).toString(),temp_ot->tel_id);
                       ///Собираем воедино
                       temp_ot->cont()->append(temp_cont);//Засунул контакт в телефон
                       crudlist->at(a)->owt()->append(temp_ot);
                      add_is_ready=true;
                      break;
                  }
              }
              if(add_is_ready==false) //если флаг апдейта не изменился
              {
                  metka: //То добавляю новую запись
                  Crud *temp_crud = Crud::id_zk_search(querry.value(0).toInt());//Собираю информацию о владельце ЗК

                  ///Беру его телефон с id = 2 (ЗНАЧИТ ТЕЛЕФОН - НОМЕР АНАЛИЗИРУЕМОГО, В КОТОРОМ ОБНАРУЖЕН КОНТАКТ)
                  Owners_tel *temp_ot = new Owners_tel(querry.value(1).toString(),3,temp_crud->zk_id);

                  Contacts *temp_cont = new Contacts(3, querry.value(2).toString(), querry.value(3).toString(), temp_ot->tel_id);

                  ///Собираем воедино
                  temp_ot->cont()->append(temp_cont);
                  temp_crud->owt()->append(temp_ot);
                  crudlist->append(temp_crud);
              }
        }
        return crudlist;
}

void For_analysis::short_face_analysis(Crud *cr, QList<Crud*> *crudlist)
{
 if(!crudlist->isEmpty())
    {
     analysis_res +="\t ФОРМИРОВАНИЕ КРАТКОЙ СПРАВКИ С ПРИВЯЗКОЙ К ЛИЦАМ \r\n \r\n";
     analysis_res +=  "Владелец записной книжки " +
     cr->lastname+" "+cr->name+" "+cr->mid_name+" возможно знаком ";
        if(crudlist->size()>1)
            analysis_res+="со следующими лицами: \r\n \r\n";
        else
            analysis_res += "со следующим лицом: \r\n \r\n";
        for (int i=0; i<crudlist->size(); i++)
        {
            analysis_res += crudlist->at(i)->lastname +" "+crudlist->at(i)->name +" "+
                    crudlist->at(i)->mid_name +", "+crudlist->at(i)->birth_date + ", окраска ???, записная книжка сформирована: "+
                    crudlist->at(i)->date_add+"  в интересах: "+crudlist->at(i)->check_for+"; \r\n \r\n";
        }
    }
}

void For_analysis::short_tel_analysis(Crud *cr, QList<Crud*> *crudlist)
{
 if(!crudlist->isEmpty())
    {
      QList<Crud*> *temp_crudlist = new QList<Crud*>;
      analysis_res +="\t ФОРМИРОВАНИЕ КРАТКОЙ СПРАВКИ С ПРИВЯЗКОЙ К ТЕЛЕФОНАМ \r\n \r\n";

      int a =0; //Итератор

metka:
    if(crudlist->at(a)->owt()->at(0)->tel_id == 1)
        temp_crudlist->append(crudlist->at(a));
    else
        return;

    if(crudlist->at(a)->owt()->at(0)->cont()->at(0)->contact_tel_num == crudlist->at(a+1)->owt()->at(0)->cont()->at(0)->contact_tel_num &&
      crudlist->at(a)->owt()->at(0)->tel_id == 1 && a<crudlist->size()-1)
      {
          a++;
          goto metka;
      }
   ///Сгруппировали массив

if(!temp_crudlist->isEmpty())
{
    analysis_res +="Номер телефона ";
    analysis_res += temp_crudlist->at(0)->owt()->at(0)->cont()->at(0)->contact_tel_num + ", принадлежащий владельцу записной книжки: "+
            cr->lastname+" "+cr->name+" "+cr->mid_name+" обнаружен в ";

    if (temp_crudlist->count()>1)
        analysis_res += "записных книжках \r\n \r\n";
    else
        analysis_res += "записной книжке: \r\n \r\n";
    for (int i = 0; i< temp_crudlist->size(); i++ )
    {
        analysis_res += "Cформированной "+ temp_crudlist->at(i)->date_add+" в отношении: "+
                temp_crudlist->at(i)->lastname +" "+temp_crudlist->at(i)->name +" " +temp_crudlist->at(i)->mid_name+
                ", по ораске ???, в интересах "+temp_crudlist->at(i)->check_for;

        if (!temp_crudlist->at(i)->owt()->at(0)->cont()->at(0)->mark.isEmpty())
            analysis_res += " с пометкой "+temp_crudlist->at(i)->owt()->at(0)->cont()->at(0)->mark + "\r\n \r\n";
        else
            analysis_res +="\r\n \r\n";
    }
    if(a<crudlist->size()-1)
    {
        temp_crudlist->clear();
        a++ ;
        goto metka;
    }
}
else
    return;
    }
}

void For_analysis::long_face_analysis(Crud *cr, QList<Crud*> *crudlist)
{
 if(!crudlist->isEmpty())
    {
     analysis_res +="\t ФОРМИРОВАНИЕ ПОЛНОЙ СПРАВКИ С ПРИВЯЗКОЙ К ЛИЦАМ \r\n \r\n";
     analysis_res +=  "Владелец записной книжки " +
     cr->lastname+" "+cr->name+" "+cr->mid_name+" возможно знаком ";
     if(crudlist->size()>1)
         analysis_res+="со следующими лицами: \r\n \r\n";
     else
         analysis_res += "со следующим лицом: \r\n \r\n";
     for (int i=0; i<crudlist->size(); i++)
     {
         analysis_res += crudlist->at(i)->lastname +" "+crudlist->at(i)->name +" "+
                 crudlist->at(i)->mid_name +", "+crudlist->at(i)->birth_date + ", адрес регистрации:" +
                 crudlist->at(i)->reg_city+", "+crudlist->at(i)->reg_street+", "+ crudlist->at(i)->reg_home +"/"+
                 crudlist->at(i)->reg_corp+ ", " + crudlist->at(i)->reg_home + ", адрес проживания: " +
                 crudlist->at(i)->liv_city+", "+crudlist->at(i)->liv_street+", "+ crudlist->at(i)->liv_home +"/"+
                 crudlist->at(i)->liv_corp+ ", " + crudlist->at(i)->liv_home ;
         QList<Owners_tel*> *list = Owners_tel::get_ow_list(crudlist->at(i)->zk_id);
         if (!list->isEmpty())
         {
             analysis_res +=" телефон: ";
             for(int a = 0 ; a < list->size(); a++)
             {
                 analysis_res += list->at(a)->tel_num+", ";
             }
         }
         analysis_res +=", окраска ???, дополнительная информация: "+crudlist->at(i)->dop_info +
         ", Дата формирования " + crudlist->at(i)->date_add + " в интересах: "+crudlist->at(i)->check_for+"\r\n";
         analysis_res+="Вывод о знакомстве сделан на основании того, что:\r\n";
         for (int a =0; a<crudlist->at(i)->owt()->size(); a++)
         {
             if(crudlist->at(i)->owt()->at(a)->tel_id == 1)
             {
                 analysis_res+="1 ВАРИАНТ Номер телефона "+crudlist->at(i)->owt()->at(a)->cont()->at(0)->contact_tel_num+", принадлежащий " +
                         cr->lastname+" "+cr->name+" "+cr->mid_name+", обнаружен в записной книжке, владельцем которой является "+
                         crudlist->at(i)->lastname +" "+crudlist->at(i)->name +" "+crudlist->at(i)->mid_name +" ";
                 if(!crudlist->at(i)->owt()->at(a)->cont()->at(0)->mark.isEmpty())
                     analysis_res +="с пометкой "+crudlist->at(i)->owt()->at(a)->cont()->at(0)->mark + "\r\n";
                 else
                     analysis_res+=" \r\n";
             }
             if(crudlist->at(i)->owt()->at(a)->tel_id == 2)
             {
                 analysis_res+="2 ВАРИАНТ Номер телефона "+crudlist->at(i)->owt()->at(a)->cont()->at(0)->contact_tel_num;
                 if(!crudlist->at(i)->owt()->at(a)->cont()->at(0)->mark.isEmpty())
                     analysis_res +=" с пометкой "+crudlist->at(i)->owt()->at(a)->cont()->at(0)->mark + " ";
                 else
                     analysis_res+=" ";
                 analysis_res+=" в записной книжке, владельцем которой является "+
                         cr->lastname+" "+cr->name+" "+cr->mid_name+", обнаружен, как принадлежащий владельцу записной книжки: "+
                         crudlist->at(i)->lastname +" "+crudlist->at(i)->name +" "+crudlist->at(i)->mid_name+" \r\n";

             }
             if(crudlist->at(i)->owt()->at(a)->tel_id == 3)
             {
                 analysis_res+="3 ВАРИАНТ Номер телефона "+crudlist->at(i)->owt()->at(a)->cont()->at(0)->contact_tel_num;
                 if(!crudlist->at(i)->owt()->at(a)->cont()->at(0)->mark.isEmpty())
                     analysis_res +=" с пометкой "+crudlist->at(i)->owt()->at(a)->cont()->at(0)->mark + " ";
                 else
                     analysis_res+=" ";
                 analysis_res+="был обнаружен в ЗК, принадлежащей "+
                         crudlist->at(i)->lastname +" "+crudlist->at(i)->name +" "+crudlist->at(i)->mid_name+
                         ", так же как и в ЗК, принадлежащей "+ cr->lastname+" "+cr->name+" "+cr->mid_name+"\r\n";
             }
         }
         analysis_res +="____________________\r\n \r\n";
     }
    }
}

void For_analysis::long_tel_analysis(Crud *cr, QList<Crud*> *crudlist)
{
 if(!crudlist->isEmpty())
    {
      QList<Crud*> *temp_crudlist = new QList<Crud*>;
      analysis_res +="\t ФОРМИРОВАНИЕ ПОЛНОЙ СПРАВКИ С ПРИВЯЗКОЙ К ТЕЛЕФОНАМ \n\r \n\r";

      int a =0; //Итератор

metka:
    if(crudlist->at(a)->owt()->at(0)->tel_id == 1)
        temp_crudlist->append(crudlist->at(a));
    else
        return;

   if(crudlist->at(a)->owt()->at(0)->cont()->at(0)->contact_tel_num == crudlist->at(a+1)->owt()->at(0)->cont()->at(0)->contact_tel_num &&
              crudlist->at(a)->owt()->at(0)->tel_id == 1 && a<crudlist->size()-1)
      {
          a++;
          goto metka;
      }
   ///Сгруппировали массив

if(!temp_crudlist->isEmpty())
{
    analysis_res +="Номер телефона "+temp_crudlist->at(0)->owt()->at(0)->cont()->at(0)->contact_tel_num;

    analysis_res+=", принадлежащий владельцу записной книжки: ";
    analysis_res += cr->lastname+" "+cr->name+" "+cr->mid_name;
    analysis_res +=" обнаружен, в ";
    if (temp_crudlist->count()>1)
        analysis_res += " записных книжках: \r\n \r\n";
    else
        analysis_res += " записной книжке: \r\n \r\n";

    for (int i = 0; i< temp_crudlist->size(); i++ )
    {
        analysis_res += temp_crudlist->at(i)->lastname +" "+temp_crudlist->at(i)->name +" " +temp_crudlist->at(i)->mid_name+", "+
       temp_crudlist->at(i)->birth_date+", адрес регистрации "+
       temp_crudlist->at(i)->reg_city+", "+temp_crudlist->at(i)->reg_street+", "+ temp_crudlist->at(i)->reg_home +"/"+
       temp_crudlist->at(i)->reg_corp+ ", " + temp_crudlist->at(i)->reg_home + ", адрес проживания: " +
       temp_crudlist->at(i)->liv_city+", "+temp_crudlist->at(i)->liv_street+", "+ temp_crudlist->at(i)->liv_home +"/"+
       temp_crudlist->at(i)->liv_corp+ ", " + temp_crudlist->at(i)->liv_home;


        analysis_res +=" телефон: "+temp_crudlist->at(i)->owt()->at(0)->tel_num;
        analysis_res +=", окраска ???, дополнительная информация: "+temp_crudlist->at(i)->dop_info +
        ", Дата формирования " + temp_crudlist->at(i)->date_add + " в интересах: "+temp_crudlist->at(i)->check_for;

        if (!temp_crudlist->at(i)->owt()->at(0)->cont()->at(0)->mark.isEmpty())
            analysis_res += " с пометкой "+temp_crudlist->at(i)->owt()->at(0)->cont()->at(0)->mark + "\r\n \r\n";
        else
            analysis_res +="\r\n \r\n";
    }
    if(a<crudlist->size()-1)
    {
        temp_crudlist->clear();
        a++ ;
        goto metka;
    }
}
else
    return;
    }
}

void For_analysis::short_face_analysis_all_db(int id)
{
    Crud *cr = list->get_crud(id);       //Србираю всю информацию об анализируемом
    QList<Crud*> *crudlist = For_analysis::get_crud(cr);

   short_face_analysis(cr, crudlist);
}

void For_analysis::short_tel_analysis_all_db(int id)
{
    Crud *cr = list->get_crud(id);       //Србираю всю информацию об анализируемом
    QList<Crud*> *crudlist = For_analysis::get_crud(cr);

    short_tel_analysis(cr, crudlist);
}

void For_analysis::long_face_analysis_all_db(int id)
{
    Crud *cr = list->get_crud(id);       //Србираю всю информацию об анализируемом
    QList<Crud*> *crudlist = For_analysis::get_crud(cr);

    long_face_analysis(cr, crudlist);
}

void For_analysis::long_tel_analysis_all_db(int id)
{
    Crud *cr = list->get_crud(id);       //Србираю всю информацию об анализируемом
    QList<Crud*> *crudlist = For_analysis::get_crud(cr);

    long_tel_analysis(cr, crudlist);
}

void For_analysis::short_face_analysis_all_db(QVector<int> vector, int id)
{
    Crud *cr = list->get_crud(id);       //Србираю всю информацию об анализируемом
    if(!vector.isEmpty())
    {
        QString tempSQL;
        tempSQL = " AND (";
        for(int i =0; i<vector.size(); i++)
        {
            tempSQL += "zk.zk_id = "+QString::number(vector.at(i));
            if (i < vector.size()-1)
                tempSQL += " OR ";
            else
                tempSQL+=") ";
        }
        QList<Crud*> *crudlist = For_analysis::get_crud(cr, tempSQL);
        short_face_analysis(cr, crudlist);
    }
}

void For_analysis::short_tel_analysis_all_db(QVector<int> vector, int id)
{
    Crud *cr = list->get_crud(id);       //Србираю всю информацию об анализируемом
    if(!vector.isEmpty())
    {
        QString tempSQL;
        tempSQL = " AND (";
        for(int i =0; i<vector.size(); i++)
        {
            tempSQL += "zk.zk_id = "+QString::number(vector.at(i));
            if (i < vector.size()-1)
                tempSQL += " OR ";
            else
                tempSQL+=") ";
        }
        QList<Crud*> *crudlist = For_analysis::get_crud(cr, tempSQL);
    short_tel_analysis(cr, crudlist);
    }
}

void For_analysis::long_face_analysis_all_db(QVector<int> vector, int id)
{    Crud *cr = list->get_crud(id);       //Србираю всю информацию об анализируемом
     if(!vector.isEmpty())
     {
         QString tempSQL;
         tempSQL = " AND (";
         for(int i =0; i<vector.size(); i++)
         {
             tempSQL += "zk.zk_id = "+QString::number(vector.at(i));
             if (i < vector.size()-1)
                 tempSQL += " OR ";
             else
                 tempSQL+=") ";
         }
         QList<Crud*> *crudlist = For_analysis::get_crud(cr, tempSQL);
    long_face_analysis(cr, crudlist);
     }
}

void For_analysis::long_tel_analysis_all_db(QVector<int> vector, int id)
{   Crud *cr = list->get_crud(id);       //Србираю всю информацию об анализируемом
    if(!vector.isEmpty())
    {
        QString tempSQL;
        tempSQL = " AND (";
        for(int i =0; i<vector.size(); i++)
        {
            tempSQL += "zk.zk_id = "+QString::number(vector.at(i));
            if (i < vector.size()-1)
                tempSQL += " OR ";
            else
                tempSQL+=") ";
        }
        QList<Crud*> *crudlist = For_analysis::get_crud(cr, tempSQL);
    long_tel_analysis(cr, crudlist);
    }

}

void For_analysis::short_face_analysis_all_db(QString DateFrom, QString DateTo, int id)
{
    Crud *cr = list->get_crud(id);       //Србираю всю информацию об анализируемом
    if(!DateFrom.isEmpty() || !DateTo.isEmpty())
    {
        QString tempSQL;
        if(!DateFrom.isEmpty())
        {
            tempSQL += " AND zk.date_add >= ('"+ DateFrom+"')";
        }
        if(!DateTo.isEmpty())
        {
            tempSQL += " AND zk.date_add <= ('"+ DateTo+"')";
        }
        QList<Crud*> *crudlist = For_analysis::get_crud(cr, tempSQL);
        short_face_analysis(cr, crudlist);
    }
}

void For_analysis::short_tel_analysis_all_db(QString DateFrom, QString DateTo, int id)
{
    Crud *cr = list->get_crud(id);       //Србираю всю информацию об анализируемом
    if(!DateFrom.isEmpty() || !DateTo.isEmpty())
    {
        QString tempSQL;
        if(!DateFrom.isEmpty())
        {
            tempSQL += " AND zk.date_add >= ('"+ DateFrom+"')";
        }
        if(!DateTo.isEmpty())
        {
            tempSQL += " AND zk.date_add <= ('"+ DateTo+"')";
        }
        QList<Crud*> *crudlist = For_analysis::get_crud(cr, tempSQL);
    short_tel_analysis(cr, crudlist);
    }
}

void For_analysis::long_face_analysis_all_db(QString DateFrom, QString DateTo, int id)
{
    Crud *cr = list->get_crud(id);       //Србираю всю информацию об анализируемом
    if(!DateFrom.isEmpty() || !DateTo.isEmpty())
    {
        QString tempSQL;
        if(!DateFrom.isEmpty())
        {
            tempSQL += " AND zk.date_add >= ('"+ DateFrom+"')";
        }
        if(!DateTo.isEmpty())
        {
            tempSQL += " AND zk.date_add <= ('"+ DateTo+"')";
        }
        QList<Crud*> *crudlist = For_analysis::get_crud(cr, tempSQL);
    long_face_analysis(cr, crudlist);
    }
}

void For_analysis::long_tel_analysis_all_db(QString DateFrom, QString DateTo, int id)
{
    Crud *cr = list->get_crud(id);       //Србираю всю информацию об анализируемом
    if(!DateFrom.isEmpty() || !DateTo.isEmpty())
    {
        QString tempSQL;
        if(!DateFrom.isEmpty())
        {
            tempSQL += " AND zk.date_add >= ('"+ DateFrom+"')";
        }
        if(!DateTo.isEmpty())
        {
            tempSQL += " AND zk.date_add <= ('"+ DateTo+"')";
        }
        QList<Crud*> *crudlist = For_analysis::get_crud(cr, tempSQL);
    long_tel_analysis(cr, crudlist);
    }
}
