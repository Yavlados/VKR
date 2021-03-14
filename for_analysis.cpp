#include "for_analysis.h"

For_analysis::For_analysis()
{
    list = new List_master(Analysis);
    this->v1Result = 0;
    this->v2Result = 0;
    this->v3Result = 0;
}

QList<Crud *> *For_analysis::get_crud(Crud *cr, QString added_SQL)
{

}

QList<Crud *> *For_analysis::get_1_var(Crud *cr, QList<Crud *> *crudlist, QString added_SQL)
{
//    db_connection *db = db_connection::instance();
//    QSqlQuery temp(db->db());
//    QSqlQuery querry(db->db());
//    analysis_res.clear();

//    ///1 вариант - собираю людей, у которых в КОНТАКТАХ лежит номер анализируемого человека


//        QString tempSQL =" SELECT zk.zk_id, S.TEL_NUM, S.CONTACT_MARK,"
//                " S.OW_TEL_NUM "
//                " FROM zk "
//                " INNER JOIN (SELECT "
//                " owners_tel.telephone_num AS TEL_NUM,"
//                " Q.CONTACT_MARK AS CONTACT_MARK,"
//                " owners_tel.fk_telephone_zk AS TEL_ID, Q.TEL_NUM AS OW_TEL_NUM"
//                " FROM OWners_tel"
//                " INNER join(SELECT contacts.cl_telephone AS CONTACT_NUM,"
//                " contacts.cl_info AS CONTACT_MARK,"
//                " contacts.fk_cl_telephone AS FK, "
//                " OW.TEL_NUM AS TEL_NUM"
//                " FROM contacts"
//                " INNER JOIN (SELECT owners_tel.telephone_num AS TEL_NUM"
//                " FROM owners_tel"
//                " where"+temp_str_for_num+") AS OW"
//                " ON OW.TEL_NUM = contacts.cl_telephone) AS Q"
//                " ON Q.FK = owners_tel.telephone_id) AS S"
//                " ON S.TEL_ID = zk.zk_id "
//                " WHERE zk.zk_id >0 ";

//        if(!added_SQL.isEmpty()) //Дополнительные условия запроса (ограничения по дате и ЗК)
//            tempSQL+=added_SQL;

//        querry.prepare(tempSQL);

//        if (!querry.exec())
//            qDebug() << "For_analysis::get_1_var" << querry.lastError();

//        /// 0 - id
//        /// 1- номер к которому привязан
//        /// 2- пометка
//        /// 3- номер анализируемого, который был обнаружен

//        while (querry.next())
//        {
//            bool add_is_ready = false;//флаг апдейта
//            if(crudlist->isEmpty())//пустой лист всегда получит запись
//                goto metka;
//            for (int a = 0 ;a<crudlist->size();a++)
//            {
//                if(crudlist->at(a)->zk_id == querry.value(0).toInt())    //Если запись совпадает
//                {
//                    Owners_tel *temp_ot = 0;
//                   ///Просто добавляю новый телефон в старую запись
//                    if(querry.value(1).toString().startsWith("499") || querry.value(1).toString().startsWith("495") )
//                        temp_ot = new Owners_tel(querry.value(1).toString(),1,crudlist->at(a)->zk_id, false, true);
//                    else
//                        temp_ot = new Owners_tel(querry.value(1).toString(),1,crudlist->at(a)->zk_id, false, false);

//                    Contacts *temp_cont = 0;
//                    if(querry.value(3).toString().startsWith("499") || querry.value(3).toString().startsWith("495") )
//                        temp_cont = new Contacts(1, querry.value(3).toString(), querry.value(2).toString(),temp_ot->tel_id, false, true);
//                    else
//                        temp_cont = new Contacts(1, querry.value(3).toString(), querry.value(2).toString(),temp_ot->tel_id, false, false);
//                    ///Собираем воедино
//                    temp_ot->cont()->append(temp_cont);//Засунул контакт в телефон
//                    crudlist->at(a)->owt()->append(temp_ot);
//                    add_is_ready=true;
//                    break;
//                }
//            }
//            if(add_is_ready==false) //если флаг апдейта не изменился
//            {
//                metka: //То добавляю новую запись
//                Crud *temp_crud = Crud::id_zk_search(querry.value(0).toInt());//Собираю информацию о владельце ЗК

//                Owners_tel *temp_ot = 0;
//                ///Беру его телефон с id = 1 (Значит телефон анализируемого обнаружен в контактах этого человека)

//                if(querry.value(1).toString().startsWith("499") || querry.value(1).toString().startsWith("495") )
//                    temp_ot = new Owners_tel(querry.value(1).toString(),1,temp_crud->zk_id, false, true);
//                else
//                    temp_ot = new Owners_tel(querry.value(1).toString(),1,temp_crud->zk_id, false, false);

//                Contacts *temp_cont = 0;
//                if( querry.value(3).toString().startsWith("499") || querry.value(3).toString().startsWith("495") )
//                    temp_cont = new Contacts(1, querry.value(3).toString(), querry.value(2).toString(),temp_ot->tel_id,false, true);
//                else
//                    temp_cont = new Contacts(1, querry.value(3).toString(), querry.value(2).toString(),temp_ot->tel_id, false, false);

//                ///Собираем воедино
//                temp_ot->cont()->append(temp_cont);
//                temp_crud->owt()->append(temp_ot);
//                crudlist->append(temp_crud);
//            }
//        }
//        return crudlist;
}

QList<Crud *> *For_analysis::get_2_var(Crud *cr, QList<Crud *> *crudlist, QString added_SQL)
{
//    db_connection *db = db_connection::instance();
//    QSqlQuery temp(db->db());
//    QSqlQuery querry(db->db());
//    QString tempSQL;
//    /// 2 вариант - в КОНТАКТАХ анализируемого человека обнаружен этот человек
//          tempSQL += " SELECT DISTINCT zk.zk_id, OWT.OWNER_TEL,"
//                 " OWT.TEL_NUM, OWT.CONT_MARK"
//                 " FROM zk"
//                 " INNER JOIN (SELECT DISTINCT owners_tel.fk_telephone_zk AS FK_ZK,"
//                 " owners_tel.telephone_num AS TEL_NUM,"
//                 " CONT.CONT_MARK AS CONT_MARK,"
//                 " CONT.OWNER_TEL AS OWNER_TEL"
//                 " FROM owners_tel"
//                 " INNER JOIN (SELECT DISTINCT contacts.cl_telephone AS CONT_TEL,"
//                 " contacts.cl_info AS CONT_MARK,"
//                 " tels.OWNER_TEL AS OWNER_TEL"
//                 " FROM contacts,(SELECT DISTINCT owners_tel.telephone_id as tel_id,"
//                 " owners_tel.telephone_num AS OWNER_TEL"
//                 " FROM owners_tel"
//                 " WHERE "+temp_str_for_num+") as tels"
//                 " WHERE ("+temp_str_for_cont_num+") AND tels.tel_id = contacts.fk_cl_telephone) AS CONT"
//                 " ON CONT.CONT_TEL = owners_tel.telephone_num) AS OWT"
//                 " ON OWT.FK_ZK = zk.zk_id"
//                 " WHERE ZK.zk_id>0 ";

//         if(!added_SQL.isEmpty()) //Дополнительные условия запроса (ограничения по дате и ЗК)
//             tempSQL+=added_SQL;

//              querry.prepare(tempSQL);
//              querry.bindValue(":zk_id", cr->zk_id);

//              if (!querry.exec())
//                  qDebug() << "For_analysis::get_2_var" << querry.lastError();

//              /// 0 - id
//              /// 1- номер анализируемого к которому привязан контакт
//              /// 2- номер обнаруженного контакта
//              /// 3- пометка

//              while (querry.next())
//              {
//                    bool add_is_ready = false;//флаг апдейта
//                      if(crudlist->isEmpty())//пустой лист всегда получит запись
//                          goto metka;
//                      for (int a = 0 ;a<crudlist->size();a++)
//                      {
//                          if(crudlist->at(a)->zk_id == querry.value(0).toInt())    //Если запись совпадает
//                          {
//                              Owners_tel *temp_ot = 0;
//                              Contacts *temp_cont = 0;

//                                if(querry.value(1).toString().startsWith("499") || querry.value(1).toString().startsWith("495") )
//                               temp_ot = new Owners_tel(querry.value(1).toString(),2,crudlist->at(a)->zk_id, false, true);
//                                else
//                               temp_ot = new Owners_tel(querry.value(1).toString(),2,crudlist->at(a)->zk_id, false, false);


//                                if(querry.value(2).toString().startsWith("499") || querry.value(2).toString().startsWith("495") )
//                                    temp_cont = new Contacts(2, querry.value(2).toString(), querry.value(3).toString(),temp_ot->tel_id, false, true);
//                                else
//                                    temp_cont = new Contacts(2, querry.value(2).toString(), querry.value(3).toString(),temp_ot->tel_id, false, false);

//                               ///Собираем воедино
//                               temp_ot->cont()->append(temp_cont);//Засунул контакт в телефон
//                               crudlist->at(a)->owt()->append(temp_ot);
//                              add_is_ready=true;
//                              break;

//                          }
//                      }
//                      if(add_is_ready==false) //если флаг апдейта не изменился
//                      {
//                          metka: //То добавляю новую запись
//                          Crud *temp_crud = Crud::id_zk_search(querry.value(0).toInt());//Собираю информацию о владельце ЗК
//                          Owners_tel *temp_ot = 0;
//                          Contacts *temp_cont = 0;

//                          ///Беру его телефон с id = 2 (ЗНАЧИТ ТЕЛЕФОН - НОМЕР АНАЛИЗИРУЕМОГО, В КОТОРОМ ОБНАРУЖЕН КОНТАКТ)
//                          if(querry.value(1).toString().startsWith("499") || querry.value(1).toString().startsWith("495") )
//                            temp_ot = new Owners_tel(querry.value(1).toString(),2,temp_crud->zk_id, false, true);
//                          else
//                            temp_ot = new Owners_tel(querry.value(1).toString(),2,temp_crud->zk_id, false, false);


//                          if(querry.value(2).toString().startsWith("499") || querry.value(2).toString().startsWith("495") )
//                              temp_cont = new Contacts(2, querry.value(2).toString(), querry.value(3).toString(), temp_ot->tel_id, false, true);
//                          else
//                              temp_cont = new Contacts(2, querry.value(2).toString(), querry.value(3).toString(), temp_ot->tel_id, false, true);

//                          ///Собираем воедино
//                          temp_ot->cont()->append(temp_cont);
//                          temp_crud->owt()->append(temp_ot);
//                          crudlist->append(temp_crud);
//                      }
//                }
//              return crudlist;
}

QList<Crud *> *For_analysis::get_3_var(Crud *cr, QList<Crud *> *crudlist, QString added_SQL)
{
//    db_connection *db = db_connection::instance();
//    QSqlQuery temp(db->db());
//    QSqlQuery querry(db->db());
//    QString tempSQL;


//   tempSQL +=" SELECT tab.s_fk, tab.telephone_num, tab.cont_num, tab.cont_mark"
//   " FROM (SELECT NOT_SORTED_CONTS.CONT_MARK, NOT_SORTED_CONTS.CONT_NUM, NOT_SORTED_CONTS.FK, owners_tel.telephone_num, owners_tel.fk_telephone_zk AS S_FK"
//   " FROM owners_tel"
//   " INNER JOIN(SELECT contacts.contact_list_id, contacts.cl_info AS CONT_MARK, contacts.cl_telephone AS CONT_NUM, contacts.fk_cl_telephone AS FK"
//   " FROM contacts,(SELECT DISTINCT contacts.cl_telephone AS CONT_TEL, contacts.cl_info AS CONT_MARK, "
//   " tels.OWNER_TEL AS OWNER_TEL,"
//   " tels.FK AS TELS_FK"
//   " FROM contacts, "
//   " (SELECT DISTINCT owners_tel.telephone_id as tel_id, owners_tel.telephone_num AS OWNER_TEL,"
//   " owners_tel.fk_telephone_zk AS FK"
//   " FROM owners_tel WHERE  "+temp_str_for_num+") as tels "
//   " WHERE ("+temp_str_for_cont_num+")"
//   " AND tels.tel_id = contacts.fk_cl_telephone) as CONTS"
//   " WHERE CONTS.CONT_TEL =  contacts.cl_telephone) AS NOT_SORTED_CONTS"
//   " ON NOT_SORTED_CONTS.FK = owners_tel.telephone_id) as  tab"
//   " WHERE tab.S_FK!= "+QString::number(cr->zk_id);


//   if(!added_SQL.isEmpty()) //Дополнительные условия запроса (ограничения по дате и ЗК)
//       tempSQL+=added_SQL;

//        querry.prepare(tempSQL);
//        querry.bindValue(":zk_id", cr->zk_id);

//        if (!querry.exec())
//            qDebug() << "For_analysis::get_3_var" << querry.lastError();

//        ///Возвращаю 0 - id
//        /// 1 - номер телефона с контактом
//        /// 2 - номер контакта
//        /// 3 - метку контакта
//        while (querry.next())
//        {
//            bool add_is_ready = false;//флаг апдейта
//              if(crudlist->isEmpty())//пустой лист всегда получит запись
//                  goto metka;
//              for (int a = 0 ;a<crudlist->size();a++)
//              {
//                  if(crudlist->at(a)->zk_id == querry.value(0).toInt())    //Если запись совпадает
//                  {
//                      Owners_tel *temp_ot = 0;
//                      Contacts *temp_cont = 0;
//                      ///Просто добавляю новый телефон в старую запись
//                      if(querry.value(1).toString().startsWith("499") || querry.value(1).toString().startsWith("495") )
//                        temp_ot = new Owners_tel(querry.value(1).toString(),3,crudlist->at(a)->zk_id,false, true);
//                      else
//                        temp_ot = new Owners_tel(querry.value(1).toString(),3,crudlist->at(a)->zk_id, false, false);

//                      if(querry.value(2).toString().startsWith("499") || querry.value(2).toString().startsWith("495") )
//                        temp_cont = new Contacts(3, querry.value(2).toString(), querry.value(3).toString(),temp_ot->tel_id, false, true);
//                      else
//                        temp_cont = new Contacts(3, querry.value(2).toString(), querry.value(3).toString(),temp_ot->tel_id,  false, false);

//                      ///Собираем воедино
//                       temp_ot->cont()->append(temp_cont);//Засунул контакт в телефон
//                       crudlist->at(a)->owt()->append(temp_ot);
//                      add_is_ready=true;
//                      break;
//                  }
//              }
//              if(add_is_ready==false) //если флаг апдейта не изменился
//              {
//                  metka: //То добавляю новую запись
//                  Owners_tel *temp_ot = 0;
//                  Contacts *temp_cont = 0;
//                  Crud *temp_crud = Crud::id_zk_search(querry.value(0).toInt());//Собираю информацию о владельце ЗК

//                  ///Беру его телефон с id = 2 (ЗНАЧИТ ТЕЛЕФОН - НОМЕР АНАЛИЗИРУЕМОГО, В КОТОРОМ ОБНАРУЖЕН КОНТАКТ)
//                  if(querry.value(1).toString().startsWith("499") || querry.value(1).toString().startsWith("495") )
//                    temp_ot  = new Owners_tel(querry.value(1).toString(),3,temp_crud->zk_id, false, true);
//                  else
//                     temp_ot  = new Owners_tel(querry.value(1).toString(),3,temp_crud->zk_id, false, false);

//                  if(querry.value(2).toString().startsWith("499") || querry.value(2).toString().startsWith("495") )
//                    temp_cont = new Contacts(3, querry.value(2).toString(), querry.value(3).toString(), temp_ot->tel_id, false, true);
//                  else
//                    temp_cont = new Contacts(3, querry.value(2).toString(), querry.value(3).toString(), temp_ot->tel_id, false, false);

//                  ///Собираем воедино
//                  temp_ot->cont()->append(temp_cont);
//                  temp_crud->owt()->append(temp_ot);
//                  crudlist->append(temp_crud);
//              }
//        }
//        return crudlist;
}

void For_analysis::short_face_analysis(Crud *cr, QList<Crud*> *crudlist)
{

//    if(!crudlist->isEmpty())
//    {
//     analysis_res +="\t КРАТКАЯ СПРАВКА ПРИВЯЗКА К ЛИЦАМ ЗК #"+QString::number(cr->zk_id)+" \r\n \r\n";
//     analysis_res +=  "Владелец записной книжки " +
//     cr->lastname+" "+cr->name+" "+cr->mid_name+" возможно знаком ";
//        if(crudlist->size()>1)
//            analysis_res+="со следующими лицами: \r\n \r\n";
//        else
//            analysis_res += "со следующим лицом: \r\n \r\n";
//        for (int i=0; i<crudlist->size(); i++)
//        {
//            analysis_res += crudlist->at(i)->lastname +" "+crudlist->at(i)->name +" "+
//                    crudlist->at(i)->mid_name +", "+crudlist->at(i)->birth_date + ", окраска ???, записная книжка сформирована: "+
//                    crudlist->at(i)->date_add+"  в интересах: "+crudlist->at(i)->check_for+"; \r\n \r\n";
//        }
//    }
}

void For_analysis::short_tel_analysis(Crud *cr, QList<Crud*> *crudlist)
{
// if(!crudlist->isEmpty())
//    {
//      analysis_res +="\t КРАТКАЯ СПРАВКА ПРИВЯЗКА К ТЕЛЕФОНАМ ЗК #"+QString::number(cr->zk_id)+" \r\n \r\n";

//      QList<Crud*> *temp_crudlist1 = new QList<Crud*>;
//      QList<Crud*> *temp_crudlist2 = new QList<Crud*>;
//      QList<Crud*> *temp_crudlist3 = new QList<Crud*>;

//    ///Новый вариант
/////Раннее работало неправильно - буду трижды писать тот же код
//       for(int a = 0; a < crudlist->size(); a++)
//       {
//           if(crudlist->at(a)->owt()->at(0)->tel_id == 1)
//           {
//               temp_crudlist1->append(crudlist->at(a));
//               if(a<crudlist->size()-1 && crudlist->at(a)->owt()->at(0)->cont()->at(0)->contact_tel_num == crudlist->at(a+1)->owt()->at(0)->cont()->at(0)->contact_tel_num &&
//                  crudlist->at(a)->owt()->at(0)->tel_id == 1 )
//                 {
//                    continue;
//                 }
//           }
//          else if(crudlist->at(a)->owt()->at(0)->tel_id == 2)
//           {
//               temp_crudlist2->append(crudlist->at(a));
//               if(a<crudlist->size()-1 && crudlist->at(a)->owt()->at(0)->cont()->at(0)->contact_tel_num == crudlist->at(a+1)->owt()->at(0)->cont()->at(0)->contact_tel_num &&
//                  crudlist->at(a)->owt()->at(0)->tel_id == 2 )
//                {
//                    continue;
//                }
//           }
//            else if(crudlist->at(a)->owt()->at(0)->tel_id == 3)
//           {
//               temp_crudlist3->append(crudlist->at(a));
//    if(a<crudlist->size()-1 && crudlist->at(a)->owt()->at(0)->cont()->at(0)->contact_tel_num == crudlist->at(a+1)->owt()->at(0)->cont()->at(0)->contact_tel_num &&
//       crudlist->at(a)->owt()->at(0)->tel_id == 3 )
//              {
//                continue;
//              }
//           }
//       }

//   ///Сгруппировали массив

//if(!temp_crudlist1->isEmpty())
//{
//    analysis_res +="1 ВАРИАНТ \r\n";
//    analysis_res +="Номер телефона ";
//    if (temp_crudlist1->at(0)->owt()->at(0)->cont()->at(0)->oldnum == true)
//        analysis_res += temp_crudlist1->at(0)->owt()->at(0)->cont()->at(0)->contact_tel_num + "(старый)";
//    else
//       analysis_res +=temp_crudlist1->at(0)->owt()->at(0)->cont()->at(0)->contact_tel_num ;
//    analysis_res += ", принадлежащий владельцу записной книжки: "+
//            cr->lastname+" "+cr->name+" "+cr->mid_name+" обнаружен в ";

//    if (temp_crudlist1->count()>1)
//        analysis_res += "записных книжках \r\n \r\n";
//    else
//        analysis_res += "записной книжке: \r\n \r\n";
//    for (int i = 0; i< temp_crudlist1->size(); i++ )
//    {
//        analysis_res += "Cформированной "+ temp_crudlist1->at(i)->date_add+" в отношении: "+
//                temp_crudlist1->at(i)->lastname +" "+temp_crudlist1->at(i)->name +" " +temp_crudlist1->at(i)->mid_name+
//                ", по ораске ???, в интересах "+temp_crudlist1->at(i)->check_for;

//        if (!temp_crudlist1->at(i)->owt()->at(0)->cont()->at(0)->mark.isEmpty())
//            analysis_res += " с пометкой "+temp_crudlist1->at(i)->owt()->at(0)->cont()->at(0)->mark + "\r\n \r\n";
//        else
//            analysis_res +="\r\n \r\n";
//    }
//        temp_crudlist1->clear();
//        delete temp_crudlist1;

//}
//if(!temp_crudlist2->isEmpty())
//{
//        analysis_res +="2 ВАРИАНТ \r\n";
//    analysis_res +="Номер телефона ";
//    if (temp_crudlist2->at(0)->owt()->at(0)->cont()->at(0)->oldnum == true)
//        analysis_res += temp_crudlist2->at(0)->owt()->at(0)->cont()->at(0)->contact_tel_num + "(старый)";
//    else
//       analysis_res +=temp_crudlist2->at(0)->owt()->at(0)->cont()->at(0)->contact_tel_num ;
//    analysis_res += ", принадлежащий владельцу записной книжки: "+
//            cr->lastname+" "+cr->name+" "+cr->mid_name+" обнаружен в ";

//    if (temp_crudlist2->count()>1)
//        analysis_res += "записных книжках \r\n \r\n";
//    else
//        analysis_res += "записной книжке: \r\n \r\n";
//    for (int i = 0; i< temp_crudlist2->size(); i++ )
//    {
//        analysis_res += "Cформированной "+ temp_crudlist2->at(i)->date_add+" в отношении: "+
//                temp_crudlist2->at(i)->lastname +" "+temp_crudlist2->at(i)->name +" " +temp_crudlist2->at(i)->mid_name+
//                ", по ораске ???, в интересах "+temp_crudlist2->at(i)->check_for;

//        if (!temp_crudlist2->at(i)->owt()->at(0)->cont()->at(0)->mark.isEmpty())
//            analysis_res += " с пометкой "+temp_crudlist2->at(i)->owt()->at(0)->cont()->at(0)->mark + "\r\n \r\n";
//        else
//            analysis_res +="\r\n \r\n";
//    }
//        temp_crudlist2->clear();
//        delete temp_crudlist2;

//}
//if(!temp_crudlist3->isEmpty())
//{
//    analysis_res +="3 ВАРИАНТ \r\n";
//    analysis_res +="Номер телефона ";
//    if (temp_crudlist3->at(0)->owt()->at(0)->cont()->at(0)->oldnum == true)
//        analysis_res += temp_crudlist3->at(0)->owt()->at(0)->cont()->at(0)->contact_tel_num + "(старый)";
//    else
//       analysis_res +=temp_crudlist3->at(0)->owt()->at(0)->cont()->at(0)->contact_tel_num ;
//    analysis_res += ", принадлежащий владельцу записной книжки: "+
//            cr->lastname+" "+cr->name+" "+cr->mid_name+" обнаружен в ";

//    if (temp_crudlist3->count()>1)
//        analysis_res += "записных книжках \r\n \r\n";
//    else
//        analysis_res += "записной книжке: \r\n \r\n";
//    for (int i = 0; i< temp_crudlist3->size(); i++ )
//    {
//        analysis_res += "Cформированной "+ temp_crudlist3->at(i)->date_add+" в отношении: "+
//                temp_crudlist3->at(i)->lastname +" "+temp_crudlist3->at(i)->name +" " +temp_crudlist3->at(i)->mid_name+
//                ", по ораске ???, в интересах "+temp_crudlist3->at(i)->check_for;

//        if (!temp_crudlist3->at(i)->owt()->at(0)->cont()->at(0)->mark.isEmpty())
//            analysis_res += " с пометкой "+temp_crudlist3->at(i)->owt()->at(0)->cont()->at(0)->mark + "\r\n \r\n";
//        else
//            analysis_res +="\r\n \r\n";
//    }
//        temp_crudlist3->clear();
//        delete temp_crudlist3;

//}
// }
}

void For_analysis::long_face_analysis(Crud *cr, QList<Crud*> *crudlist)
{
// if(!crudlist->isEmpty())
//    {
//     analysis_res +="\t ПОЛНАЯ СПРАВКА ПРИВЯЗКА К ЛИЦАМ ДЛЯ ЗК #"+QString::number(cr->zk_id)+" \r\n \r\n";
//     analysis_res +=  "Владелец записной книжки " +
//     cr->lastname+" "+cr->name+" "+cr->mid_name+" возможно знаком ";
//     if(crudlist->size()>1)
//         analysis_res+="со следующими лицами: \r\n \r\n";
//     else
//         analysis_res += "со следующим лицом: \r\n \r\n";
//     for (int i=0; i<crudlist->size(); i++)
//     {
//         analysis_res += crudlist->at(i)->lastname +" "+crudlist->at(i)->name +" "+
//                 crudlist->at(i)->mid_name +", "+crudlist->at(i)->birth_date + ", адрес регистрации:" +
//                 crudlist->at(i)->reg_city+", "+crudlist->at(i)->reg_street+", "+ crudlist->at(i)->reg_home +"/"+
//                 crudlist->at(i)->reg_corp+ ", " + crudlist->at(i)->reg_home + ", адрес проживания: " +
//                 crudlist->at(i)->liv_city+", "+crudlist->at(i)->liv_street+", "+ crudlist->at(i)->liv_home +"/"+
//                 crudlist->at(i)->liv_corp+ ", " + crudlist->at(i)->liv_home ;
//         QList<Owners_tel*> *list = Owners_tel::get_ow_list(crudlist->at(i)->zk_id);
//         if (!list->isEmpty())
//         {
//             analysis_res +=" телефон: ";
//             for(int a = 0 ; a < list->size(); a++)
//             {
//                 if (list->at(a)->oldnum == true)
//                    analysis_res += list->at(a)->tel_num+"(старый), ";
//                 else
//                     analysis_res += list->at(a)->tel_num+", ";
//             }
//         }
//         analysis_res +=", окраска ???, дополнительная информация: "+crudlist->at(i)->dop_info +
//         ", Дата формирования " + crudlist->at(i)->date_add + " в интересах: "+crudlist->at(i)->check_for+"\r\n";
//         analysis_res+="Вывод о знакомстве сделан на основании того, что:\r\n";
//         for (int a =0; a<crudlist->at(i)->owt()->size(); a++)
//         {
//             if(crudlist->at(i)->owt()->at(a)->tel_id == 1)
//             {
//                 analysis_res+="1 ВАРИАНТ Номер телефона ";
//                 if (crudlist->at(i)->owt()->at(a)->cont()->at(0)->oldnum == true)
//                    analysis_res+=crudlist->at(i)->owt()->at(a)->cont()->at(0)->contact_tel_num+"(старый), ";
//                 else
//                     analysis_res += crudlist->at(i)->owt()->at(a)->cont()->at(0)->contact_tel_num+", ";
//                  analysis_res += "принадлежащий " +
//                         cr->lastname+" "+cr->name+" "+cr->mid_name+", обнаружен в записной книжке, владельцем которой является "+
//                         crudlist->at(i)->lastname +" "+crudlist->at(i)->name +" "+crudlist->at(i)->mid_name +" ";
//                 if(!crudlist->at(i)->owt()->at(a)->cont()->at(0)->mark.isEmpty())
//                     analysis_res +="с пометкой "+crudlist->at(i)->owt()->at(a)->cont()->at(0)->mark + "\r\n";
//                 else
//                     analysis_res+=" \r\n";
//             }
//             if(crudlist->at(i)->owt()->at(a)->tel_id == 2)
//             {
//                 analysis_res+="2 ВАРИАНТ Номер телефона  ";
//                 if (crudlist->at(i)->owt()->at(a)->cont()->at(0)->oldnum == true)
//                    analysis_res+=crudlist->at(i)->owt()->at(a)->cont()->at(0)->contact_tel_num+"(старый), ";
//                 else
//                     analysis_res += crudlist->at(i)->owt()->at(a)->cont()->at(0)->contact_tel_num+", ";

//                 if(!crudlist->at(i)->owt()->at(a)->cont()->at(0)->mark.isEmpty())
//                     analysis_res +=" с пометкой "+crudlist->at(i)->owt()->at(a)->cont()->at(0)->mark + " ";
//                 else
//                     analysis_res+=" ";
//                 analysis_res+=" в записной книжке, владельцем которой является "+
//                         cr->lastname+" "+cr->name+" "+cr->mid_name+", обнаружен, как принадлежащий владельцу записной книжки: "+
//                         crudlist->at(i)->lastname +" "+crudlist->at(i)->name +" "+crudlist->at(i)->mid_name+" \r\n";

//             }
//             if(crudlist->at(i)->owt()->at(a)->tel_id == 3)
//             {
//                 analysis_res+="3 ВАРИАНТ Номер телефона ";
//                 if (crudlist->at(i)->owt()->at(a)->cont()->at(0)->oldnum == true)
//                    analysis_res+=crudlist->at(i)->owt()->at(a)->cont()->at(0)->contact_tel_num+"(старый), ";
//                 else
//                     analysis_res += crudlist->at(i)->owt()->at(a)->cont()->at(0)->contact_tel_num+", ";

//                 if(!crudlist->at(i)->owt()->at(a)->cont()->at(0)->mark.isEmpty())
//                     analysis_res +=" с пометкой "+crudlist->at(i)->owt()->at(a)->cont()->at(0)->mark + " ";
//                 else
//                     analysis_res+=" ";
//                 analysis_res+="был обнаружен в ЗК, принадлежащей "+
//                         crudlist->at(i)->lastname +" "+crudlist->at(i)->name +" "+crudlist->at(i)->mid_name+
//                         ", так же как и в ЗК, принадлежащей "+ cr->lastname+" "+cr->name+" "+cr->mid_name+"\r\n";
//             }
//         }
//         analysis_res +="____________________\r\n \r\n";
//     }
//    }
}

void For_analysis::long_tel_analysis(Crud *cr, QList<Crud*> *crudlist)
{
// if(!crudlist->isEmpty())
//    {
//      //QList<Crud*> *temp_crudlist = new QList<Crud*>;
//      QList<Crud*> *temp_crudlist1 = new QList<Crud*>;
//      QList<Crud*> *temp_crudlist2 = new QList<Crud*>;
//      QList<Crud*> *temp_crudlist3 = new QList<Crud*>;

//      analysis_res +="\t ПОЛНАЯ СПРАВКА ПРИВЯЗКА К ТЕЛЕФОНАМ ДЛЯ ЗК #"+QString::number(cr->zk_id)+" \n\r \n\r";
//    ///Новый вариант
//    ///Раннее работало неправильно - буду трижды писать тот же код
//       for(int a = 0; a < crudlist->size(); a++)
//       {
//           if(crudlist->at(a)->owt()->at(0)->tel_id == 1)
//           {
//               temp_crudlist1->append(crudlist->at(a));
//               if(a<crudlist->size()-1 && crudlist->at(a)->owt()->at(0)->cont()->at(0)->contact_tel_num == crudlist->at(a+1)->owt()->at(0)->cont()->at(0)->contact_tel_num &&
//                  crudlist->at(a)->owt()->at(0)->tel_id == 1 )
//                 {
//                    continue;
//                 }
//           }
//          else if(crudlist->at(a)->owt()->at(0)->tel_id == 2)
//           {
//               temp_crudlist2->append(crudlist->at(a));
//               if(a<crudlist->size()-1 && crudlist->at(a)->owt()->at(0)->cont()->at(0)->contact_tel_num == crudlist->at(a+1)->owt()->at(0)->cont()->at(0)->contact_tel_num &&
//                  crudlist->at(a)->owt()->at(0)->tel_id == 2 )
//                {
//                    continue;
//                }
//           }
//            else if(crudlist->at(a)->owt()->at(0)->tel_id == 3)
//           {
//               temp_crudlist3->append(crudlist->at(a));
//    if(a<crudlist->size()-1 && crudlist->at(a)->owt()->at(0)->cont()->at(0)->contact_tel_num == crudlist->at(a+1)->owt()->at(0)->cont()->at(0)->contact_tel_num &&
//       crudlist->at(a)->owt()->at(0)->tel_id == 3 )
//              {
//                continue;
//              }
//           }
//       }
//   ///Сгруппировали массив
//   ///
//   ///
//   ///  ///
//   ///
//       if(!temp_crudlist1->isEmpty())
//             {
//                 analysis_res+="1 ВАРИАНТ \r\n";
//                 analysis_res +="Номер телефона ";
//                 if (temp_crudlist1->at(0)->owt()->at(0)->cont()->at(0)->oldnum == true)
//                    analysis_res+=temp_crudlist1->at(0)->owt()->at(0)->cont()->at(0)->contact_tel_num+"(старый), ";
//                 else
//                     analysis_res += temp_crudlist1->at(0)->owt()->at(0)->cont()->at(0)->contact_tel_num+", ";

//                 analysis_res+=" принадлежащий владельцу записной книжки: ";
//                 analysis_res += cr->lastname+" "+cr->name+" "+cr->mid_name;
//                 analysis_res +=" обнаружен, в ";
//                 if (temp_crudlist1->count()>1)
//                     analysis_res += " записных книжках: \r\n \r\n";
//                 else
//                     analysis_res += " записной книжке: \r\n \r\n";

//                 for (int i = 0; i< temp_crudlist1->size(); i++ )
//                 {
//                     analysis_res += "ЗК № " +QString::number(temp_crudlist1->at(i)->zk_id) + " "+ temp_crudlist1->at(i)->lastname +" "+temp_crudlist1->at(i)->name +" " +temp_crudlist1->at(i)->mid_name+", "+
//                    temp_crudlist1->at(i)->birth_date+", адрес регистрации "+
//                    temp_crudlist1->at(i)->reg_city+", "+temp_crudlist1->at(i)->reg_street+", "+ temp_crudlist1->at(i)->reg_home +"/"+
//                    temp_crudlist1->at(i)->reg_corp+ ", " + temp_crudlist1->at(i)->reg_home + ", адрес проживания: " +
//                    temp_crudlist1->at(i)->liv_city+", "+temp_crudlist1->at(i)->liv_street+", "+ temp_crudlist1->at(i)->liv_home +"/"+
//                    temp_crudlist1->at(i)->liv_corp+ ", " + temp_crudlist1->at(i)->liv_home;


//                     analysis_res +=" телефон: ";
//                     if ( temp_crudlist1->at(i)->owt()->at(0)->oldnum == true)
//                         analysis_res += temp_crudlist1->at(i)->owt()->at(0)->tel_num + "(старый), ";
//                     else
//                         analysis_res += temp_crudlist1->at(i)->owt()->at(0)->tel_num + ", ";

//                     analysis_res +=", окраска ???, дополнительная информация: "+temp_crudlist1->at(i)->dop_info +
//                     ", Дата формирования " + temp_crudlist1->at(i)->date_add + " в интересах: "+temp_crudlist1->at(i)->check_for;

//                     if (!temp_crudlist1->at(i)->owt()->at(0)->cont()->at(0)->mark.isEmpty())
//                         analysis_res += " с пометкой "+temp_crudlist1->at(i)->owt()->at(0)->cont()->at(0)->mark + "\r\n \r\n";
//                     else
//                         analysis_res +="\r\n \r\n";
//                 }
//                     temp_crudlist1->clear();
//                     delete temp_crudlist1;
//             }
//       if(!temp_crudlist2->isEmpty())
//       {
//           analysis_res+="2 ВАРИАНТ \r\n";
//           analysis_res +="Номер телефона ";
//           if (temp_crudlist2->at(0)->owt()->at(0)->cont()->at(0)->oldnum == true)
//              analysis_res+=temp_crudlist2->at(0)->owt()->at(0)->cont()->at(0)->contact_tel_num+"(старый), ";
//           else
//               analysis_res += temp_crudlist2->at(0)->owt()->at(0)->cont()->at(0)->contact_tel_num+", ";

//           analysis_res+=" принадлежащий владельцу записной книжки: ";
//           analysis_res += cr->lastname+" "+cr->name+" "+cr->mid_name;
//           analysis_res +=" обнаружен, в ";
//           if (temp_crudlist2->count()>1)
//               analysis_res += " записных книжках: \r\n \r\n";
//           else
//               analysis_res += " записной книжке: \r\n \r\n";

//           for (int i = 0; i< temp_crudlist2->size(); i++ )
//           {
//              analysis_res += "ЗК № " +QString::number(temp_crudlist2->at(i)->zk_id) + " "+temp_crudlist2->at(i)->lastname +" "+temp_crudlist2->at(i)->name +" " +temp_crudlist2->at(i)->mid_name+", "+
//              temp_crudlist2->at(i)->birth_date+", адрес регистрации "+
//              temp_crudlist2->at(i)->reg_city+", "+temp_crudlist2->at(i)->reg_street+", "+ temp_crudlist2->at(i)->reg_home +"/"+
//              temp_crudlist2->at(i)->reg_corp+ ", " + temp_crudlist2->at(i)->reg_home + ", адрес проживания: " +
//              temp_crudlist2->at(i)->liv_city+", "+temp_crudlist2->at(i)->liv_street+", "+ temp_crudlist2->at(i)->liv_home +"/"+
//              temp_crudlist2->at(i)->liv_corp+ ", " + temp_crudlist2->at(i)->liv_home;


//               analysis_res +=" телефон: ";
//               if ( temp_crudlist2->at(i)->owt()->at(0)->oldnum == true)
//                   analysis_res += temp_crudlist2->at(i)->owt()->at(0)->tel_num + "(старый), ";
//               else
//                   analysis_res += temp_crudlist2->at(i)->owt()->at(0)->tel_num + ", ";

//               analysis_res +=", окраска ???, дополнительная информация: "+temp_crudlist2->at(i)->dop_info +
//               ", Дата формирования " + temp_crudlist2->at(i)->date_add + " в интересах: "+temp_crudlist2->at(i)->check_for;

//               if (!temp_crudlist2->at(i)->owt()->at(0)->cont()->at(0)->mark.isEmpty())
//                   analysis_res += " с пометкой "+temp_crudlist2->at(i)->owt()->at(0)->cont()->at(0)->mark + "\r\n \r\n";
//               else
//                   analysis_res +="\r\n \r\n";
//           }
//               temp_crudlist2->clear();
//               delete temp_crudlist2;
//       }
//if(!temp_crudlist3->isEmpty())
//{
//    analysis_res+="3 ВАРИАНТ \r\n";
//    analysis_res +="Номер телефона ";
//    if (temp_crudlist3->at(0)->owt()->at(0)->cont()->at(0)->oldnum == true)
//       analysis_res+=temp_crudlist3->at(0)->owt()->at(0)->cont()->at(0)->contact_tel_num+"(старый), ";
//    else
//        analysis_res += temp_crudlist3->at(0)->owt()->at(0)->cont()->at(0)->contact_tel_num+"";

//     analysis_res += "(с пометкой "+temp_crudlist3->at(0)->owt()->at(0)->cont()->at(0)->mark+"),";

//    analysis_res+=" принадлежащий владельцу записной книжки: ";
//    analysis_res += cr->lastname+" "+cr->name+" "+cr->mid_name;
//    analysis_res +=" обнаружен, в ";

//    if (temp_crudlist3->count()>1)
//        analysis_res += " записных книжках: \r\n \r\n";
//    else
//        analysis_res += " записной книжке: \r\n \r\n";



//    for (int i = 0; i< temp_crudlist3->size(); i++ )
//    {
//       analysis_res += "ЗК № " +QString::number(temp_crudlist3->at(i)->zk_id) + " "+temp_crudlist3->at(i)->lastname +" "+temp_crudlist3->at(i)->name +" " +temp_crudlist3->at(i)->mid_name+", "+
//       temp_crudlist3->at(i)->birth_date+", адрес регистрации "+
//       temp_crudlist3->at(i)->reg_city+", "+temp_crudlist3->at(i)->reg_street+", "+ temp_crudlist3->at(i)->reg_home +"/"+
//       temp_crudlist3->at(i)->reg_corp+ ", " + temp_crudlist3->at(i)->reg_home + ", адрес проживания: " +
//       temp_crudlist3->at(i)->liv_city+", "+temp_crudlist3->at(i)->liv_street+", "+ temp_crudlist3->at(i)->liv_home +"/"+
//       temp_crudlist3->at(i)->liv_corp+ ", " + temp_crudlist3->at(i)->liv_home;


//        analysis_res +=" телефон: ";
//        if ( temp_crudlist3->at(i)->owt()->at(0)->oldnum == true)
//            analysis_res += temp_crudlist3->at(i)->owt()->at(0)->tel_num + "(старый), ";
//        else
//            analysis_res += temp_crudlist3->at(i)->owt()->at(0)->tel_num + ", ";

//        analysis_res +=", окраска ???, дополнительная информация: "+temp_crudlist3->at(i)->dop_info +
//        ", Дата формирования " + temp_crudlist3->at(i)->date_add + " в интересах: "+temp_crudlist3->at(i)->check_for;

//        if (!temp_crudlist3->at(i)->owt()->at(0)->cont()->at(0)->mark.isEmpty())
//            analysis_res += " с пометкой "+temp_crudlist3->at(i)->owt()->at(0)->cont()->at(0)->mark + "\r\n \r\n";
//        else
//            analysis_res +="\r\n \r\n";
//    }
//        temp_crudlist3->clear();
//        delete temp_crudlist3;
//}
//else
//    return;
// }
}

void For_analysis::short_face_analysis_all_db(int id)
{
//    Crud *cr = list->get_crud(id);       //Србираю всю информацию об анализируемом
//    QList<Crud*> *crudlist = For_analysis::get_crud(cr);

   short_face_analysis(id);

}

void For_analysis::short_tel_analysis_all_db(int id)
{
//    Crud *cr = list->get_crud(id);       //Србираю всю информацию об анализируемом
//    QList<Crud*> *crudlist = For_analysis::get_crud(cr);

    short_tel_analysis(id);
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

void For_analysis::ClearAll()
{
    temp_str_for_num.clear();

    temp_str_for_cont_num.clear();

    analysis_res.clear();

    match_counter.clear();
}

void For_analysis::getDataFromBaseFace(int eventId, QVector<int> *eventIdList)
{
    this->analysisResult.clear();
    this->v1Result = this->getV1(eventId, eventIdList);
    this->v2Result = this->getV2(eventId, eventIdList);
    this->v3Result = this->getV3(eventId, eventIdList);

    for(int i=0; i< this->v1Result->size(); i++ ){
        Analysis_V1 *v =  this->v1Result->at(i);
        QString fio = v->from_lastname + " " + v->from_name + " " + v->from_midname;
        QList<Analysis_general> b = this->analysisResult[fio];
        Analysis_general *obj = AnalysisModels::instance()->convert(v);
        b.append(*obj);
        this->analysisResult[fio] = b;
    }
    for(int i=0; i< this->v2Result->size(); i++ ){
        Analysis_V2 *v =  this->v2Result->at(i);
        QString fio = v->from_lastname + " " + v->from_name + " " + v->from_midname;
        QList<Analysis_general> b = this->analysisResult[fio];
        Analysis_general *obj = AnalysisModels::instance()->convert(v);
        b.append(*obj);
        this->analysisResult[fio] = b;
    }
    for(int i=0; i< this->v3Result->size(); i++ ){
        Analysis_V3 *v =  this->v3Result->at(i);
        QString fio = v->from_lastname + " " + v->from_name + " " + v->from_midname;
        QList<Analysis_general> b = this->analysisResult[fio];
        Analysis_general *obj = AnalysisModels::instance()->convert(v);
        b.append(*obj);
        this->analysisResult[fio] = b;
    }
}

void For_analysis::getDataFromBaseTelephone(int eventId, QVector<int> *eventIdList)
{
    this->analysisResult.clear();
    this->v1Result = this->getV1(eventId, eventIdList);
    this->v2Result = this->getV2(eventId, eventIdList);
    this->v3Result = this->getV3(eventId, eventIdList);

    for(int i=0; i< this->v1Result->size(); i++ ){
        Analysis_V1 *v =  this->v1Result->at(i);
        QString telephone = v->from_telephone + ";"+v->from_lastname + " " + v->from_name + " " + v->from_midname;
        QList<Analysis_general> b = this->analysisResult[telephone];
        Analysis_general *obj = AnalysisModels::instance()->convert(v);
        b.append(*obj);
        this->analysisResult[telephone] = b;
    }
    for(int i=0; i< this->v2Result->size(); i++ ){
        Analysis_V2 *v =  this->v2Result->at(i);
        QString telephone = v->from_telephone + ";"+v->from_lastname + " " + v->from_name + " " + v->from_midname;
        QList<Analysis_general> b = this->analysisResult[telephone];
        Analysis_general *obj = AnalysisModels::instance()->convert(v);
        b.append(*obj);
        this->analysisResult[telephone] = b;
    }
    for(int i=0; i< this->v3Result->size(); i++ ){
        Analysis_V3 *v =  this->v3Result->at(i);
        QString telephone = v->from_telephone + ";"+v->from_lastname + " " + v->from_name + " " + v->from_midname;
        QList<Analysis_general> b = this->analysisResult[telephone];
        Analysis_general *obj = AnalysisModels::instance()->convert(v);
        b.append(*obj);
        this->analysisResult[telephone] = b;
    }
}

void For_analysis::getDataFromBaseFullFace(int eventId, QVector<int> *eventIdList)
{
    this->analysisResultFull.clear();
    this->v1Result = this->getV1(eventId, eventIdList);
    this->v2Result = this->getV2(eventId, eventIdList);
    this->v3Result = this->getV3(eventId, eventIdList);

    for (int i=0; i< this->v1Result->size(); i++ ) {
        Analysis_V1 *v =  this->v1Result->at(i);
        QString fio = v->from_lastname + " " + v->from_name + " " + v->from_midname;
        Analysis_general *obj = AnalysisModels::instance()->convert(v);
        this->analysisResultFull[fio]["var1"].append(*obj);
    }

    for (int i=0; i< this->v2Result->size(); i++ ) {
        Analysis_V2 *v =  this->v2Result->at(i);
        QString fio = v->from_lastname + " " + v->from_name + " " + v->from_midname;
        Analysis_general *obj = AnalysisModels::instance()->convert(v);
        this->analysisResultFull[fio]["var2"].append(*obj);
    }

    for (int i=0; i< this->v3Result->size(); i++ ) {
        Analysis_V3 *v =  this->v3Result->at(i);
        QString fio = v->from_lastname + " " + v->from_name + " " + v->from_midname;
        Analysis_general *obj = AnalysisModels::instance()->convert(v);
        this->analysisResultFull[fio]["var3"].append(*obj);
    }
}

QList<Analysis_V1*> *For_analysis::getV1(int eventId, QVector<int> *eventIdList)
{
    if( !db_connection::instance()->db_connect() )
        return 0;

    QString cname = db_connection::instance()->db().connectionName();

    bool isOk = db_connection::instance()->db().database(cname).transaction();
    QSqlQuery temp(db_connection::instance()->db());
    if(eventIdList == 0)
        temp.prepare(AnalysisModels::instance()->v1(QString::number(eventId)));
    else
        temp.prepare(AnalysisModels::instance()->v1(QString::number(eventId), eventIdList));

    if (!temp.exec())
    {
        db_connection::instance()->lastError = "For_analysis::getV1 " + temp.lastError().text();

        qDebug() << "For_analysis::getV1" << temp.lastError();
        db_connection::instance()->db().database(cname).rollback();

        return 0;
    }

    QList<Analysis_V1*> *v1Result = new QList<Analysis_V1*>;

    while(temp.next()){
        Analysis_V1 *v1 = new Analysis_V1();
        v1->from_event_id 					= temp.value(0).toString()  ;
        v1->from_event_category 			= temp.value(1).toString()  ;
        v1->from_event_detention_date 		= temp.value(2).toString()  ;
        v1->from_event_detention_time 		= temp.value(3).toString()  ;
        v1->from_event_detention_reason 	= temp.value(4).toString()  ;
        v1->from_event_detention_by 		= temp.value(5).toString()  ;
        v1->from_event_keeping_place 		= temp.value(6).toString()  ;
        v1->from_event_keeping_additional 	= temp.value(7).toString()  ;
        v1->from_name 						= temp.value(8).toString()  ;
        v1->from_lastname 					= temp.value(9).toString()  ;
        v1->from_midname 					= temp.value(10).toString() ;
        v1->from_telephone 					= temp.value(11).toString() ;
        v1->from_telephone_oldnum 			= temp.value(12).toBool()   ;
        v1->from_telephone_internum 		= temp.value(13).toBool()   ;
        v1->to_contact 						= temp.value(14).toString() ;
        v1->to_contact_alias 				= temp.value(15).toString() ;
        v1->to_contact_oldnum 				= temp.value(16).toBool()   ;
        v1->to_contact_internum 			= temp.value(17).toBool()   ;
        v1->to_telephone 					= temp.value(18).toString() ;
        v1->to_telephone_oldnum 			= temp.value(19).toBool()   ;
        v1->to_telephone_internum 			= temp.value(20).toBool()   ;
        v1->to_name 						= temp.value(21).toString() ;
        v1->to_lastname 					= temp.value(22).toString() ;
        v1->to_midname 						= temp.value(23).toString() ;
        v1->to_event_category 				= temp.value(24).toString() ;
        v1->to_event_detention_date 		= temp.value(25).toString() ;
        v1->to_event_detention_time 		= temp.value(26).toString() ;
        v1->to_event_detention_reason 		= temp.value(27).toString() ;
        v1->to_event_detention_by 			= temp.value(28).toString() ;
        v1->to_event_keeping_place 			= temp.value(29).toString() ;
        v1->to_event_keeping_additional 	= temp.value(30).toString() ;
        v1->to_event_id  					= temp.value(31).toString() ;
        v1Result->append(v1);
    }
    db_connection::instance()->db().database(cname).commit();
    return v1Result;
}

QList<Analysis_V2*> *For_analysis::getV2(int eventId, QVector<int> *eventIdList)
{
    if( !db_connection::instance()->db_connect() )
        return 0;

    QString cname = db_connection::instance()->db().connectionName();

    bool isOk = db_connection::instance()->db().database(cname).transaction();
    QSqlQuery temp(db_connection::instance()->db());
    if(eventIdList == 0)
        temp.prepare(AnalysisModels::instance()->v2(QString::number(eventId)));
    else
        temp.prepare(AnalysisModels::instance()->v2(QString::number(eventId), eventIdList));

    if (!temp.exec())
    {
        db_connection::instance()->lastError = "For_analysis::getV2 " + temp.lastError().text();

        qDebug() << "For_analysis::getV2" << temp.lastError();
        db_connection::instance()->db().database(cname).rollback();

        return 0;
    }

    QList<Analysis_V2*> *v2Result = new QList<Analysis_V2*>;

    while(temp.next()){
        Analysis_V2 *v2 = new Analysis_V2();
        v2->from_event_id                   = temp.value(0).toString()  ;
        v2->from_event_category             = temp.value(1).toString()  ;
        v2->from_event_detention_date       = temp.value(2).toString()  ;
        v2->from_event_detention_time       = temp.value(3).toString()  ;
        v2->from_event_detention_reason     = temp.value(4).toString()  ;
        v2->from_event_detention_by         = temp.value(5).toString()  ;
        v2->from_event_keeping_place        = temp.value(6).toString()  ;
        v2->from_event_keeping_additional   = temp.value(7).toString()  ;
        v2->from_name                       = temp.value(8).toString()  ;
        v2->from_lastname                   = temp.value(9).toString()  ;
        v2->from_midname                    = temp.value(10).toString() ;
        v2->from_telephone                  = temp.value(11).toString() ;
        v2->from_telephone_oldnum           = temp.value(12).toBool()   ;
        v2->from_telephone_internum         = temp.value(13).toBool()   ;
        v2->from_contact                    = temp.value(14).toString() ;
        v2->from_contact_alias              = temp.value(15).toString() ;
        v2->from_contact_oldnum             = temp.value(16).toBool()   ;
        v2->from_contact_internum           = temp.value(17).toBool()   ;
        v2->to_telephone                    = temp.value(18).toString() ;
        v2->to_telephone_oldnum             = temp.value(19).toBool()   ;
        v2->to_telephone_internum           = temp.value(20).toBool()   ;
        v2->to_name                         = temp.value(21).toString() ;
        v2->to_lastname                     = temp.value(22).toString() ;
        v2->to_midname                      = temp.value(23).toString() ;
        v2->to_event_category               = temp.value(24).toString() ;
        v2->to_event_detention_date         = temp.value(25).toString() ;
        v2->to_event_detention_time         = temp.value(26).toString() ;
        v2->to_event_detention_reason       = temp.value(27).toString() ;
        v2->to_event_detention_by           = temp.value(28).toString() ;
        v2->to_event_keeping_place          = temp.value(29).toString() ;
        v2->to_event_keeping_additional     = temp.value(30).toString() ;
        v2->to_event_id                     = temp.value(31).toString() ;
        v2Result->append(v2);
    }
    db_connection::instance()->db().database(cname).commit();

    return v2Result;
}

QList<Analysis_V3 *> *For_analysis::getV3(int eventId, QVector<int> *eventIdList)
{
    if( !db_connection::instance()->db_connect() )
        return 0;

    QString cname = db_connection::instance()->db().connectionName();

    bool isOk = db_connection::instance()->db().database(cname).transaction();
    QSqlQuery temp(db_connection::instance()->db());
    if(eventIdList == 0)
        temp.prepare(AnalysisModels::instance()->v3(QString::number(eventId)));
    else
        temp.prepare(AnalysisModels::instance()->v3(QString::number(eventId), eventIdList));

    if (!temp.exec())
    {
        db_connection::instance()->lastError = "For_analysis::getV3 " + temp.lastError().text();

        qDebug() << "For_analysis::getV3" << temp.lastError();
        db_connection::instance()->db().database(cname).rollback();

        return 0;
    }

    QList<Analysis_V3*> *v3Result = new QList<Analysis_V3*>;

    while(temp.next()){
        Analysis_V3 *v3 = new Analysis_V3();
        v3->from_event_id                   = temp.value(0).toString()  ;
        v3->from_event_category             = temp.value(1).toString()  ;
        v3->from_event_detention_date       = temp.value(2).toString()  ;
        v3->from_event_detention_time       = temp.value(3).toString()  ;
        v3->from_event_detention_reason     = temp.value(4).toString()  ;
        v3->from_event_detention_by         = temp.value(5).toString()  ;
        v3->from_event_keeping_place        = temp.value(6).toString()  ;
        v3->from_event_keeping_additional   = temp.value(7).toString()  ;
        v3->from_name                       = temp.value(8).toString()  ;
        v3->from_lastname                   = temp.value(9).toString()  ;
        v3->from_midname                    = temp.value(10).toString() ;
        v3->from_telephone                  = temp.value(11).toString() ;
        v3->from_telephone_oldnum           = temp.value(12).toBool()   ;
        v3->from_telephone_internum         = temp.value(13).toBool()   ;
        v3->from_contact                    = temp.value(14).toString() ;
        v3->from_contact_alias              = temp.value(15).toString() ;
        v3->from_contact_oldnum             = temp.value(16).toBool()   ;
        v3->from_contact_internum           = temp.value(17).toBool()   ;
        v3->to_contact                      = temp.value(18).toString() ;
        v3->to_contact_alias                = temp.value(19).toString() ;
        v3->to_contact_oldnum               = temp.value(20).toBool()   ;
        v3->to_contact_internum             = temp.value(21).toBool()   ;
        v3->to_telephone                    = temp.value(22).toString() ;
        v3->to_telephone_oldnum             = temp.value(23).toBool() ;
        v3->to_telephone_internum           = temp.value(24).toBool() ;
        v3->to_name                         = temp.value(25).toString() ;
        v3->to_lastname                     = temp.value(26).toString() ;
        v3->to_midname                      = temp.value(27).toString() ;
        v3->to_event_category               = temp.value(28).toString() ;
        v3->to_event_detention_date         = temp.value(29).toString() ;
        v3->to_event_detention_time         = temp.value(30).toString() ;
        v3->to_event_detention_reason       = temp.value(31).toString() ;

        v3->to_event_detention_by           = temp.value(32).toString()  ;
        v3->to_event_keeping_place          = temp.value(33).toString()  ;
        v3->to_event_keeping_additional     = temp.value(34).toString()  ;
        v3->to_event_id                     = temp.value(35).toString()  ;
        v3Result->append(v3);
    }
    db_connection::instance()->db().database(cname).commit();

    return v3Result;
}

void For_analysis::short_face_analysis(int eventId)
{
    this->analysis_res +="\t КРАТКАЯ СПРАВКА ПРИВЯЗКА К ЛИЦАМ СОБЫТИЕ #"+QString::number(eventId)+" \r\n";
    for(int i=0; i<this->analysisResult.keys().size(); i++ ){
        QString fio = this->analysisResult.keys().at(i);
        QList<Analysis_general> list = this->analysisResult[fio];
        this->analysis_res +=  "\r\nФигурант события " +fio+" возможно знаком";
                if(list.size()>1)
                    this->analysis_res+=" со следующими лицами: \r\n \r\n";
                else
                    this->analysis_res += "со следующим лицом: \r\n \r\n";
        for(int a=0; a<list.size();a++){
            Analysis_general res = list.at(a);
            this->analysis_res += "\t" + res.to_lastname +" "+res.to_name +" "+ res.to_midname + " "+
                        ", окраска ???, номер события: "+res.to_event_id+", дата добавления: "+
                        res.to_event_detention_date+" "+ res.to_event_detention_time+";\r\n";
        }
    }
}

void For_analysis::short_tel_analysis(int eventId)
{
    this->analysis_res +="\t КРАТКАЯ СПРАВКА ПРИВЯЗКА К ТЕЛЕФОНАМ СОБЫТИЕ #"+QString::number(eventId)+" \r\n";
     for(int i=0; i<this->analysisResult.keys().size(); i++ ){
         QString key = this->analysisResult.keys().at(i);
         auto metaData = key.split(';');
         QList<Analysis_general> list = this->analysisResult[key];
         this->analysis_res +=  "\r\nНомер телефона " +metaData.at(0)+", принадлежащий владельцу записной книжки " +
                 metaData.at(1) + "обнаружен в ";
                 if(list.size()>1)
                     this->analysis_res+=" записных книжках: \r\n \r\n";
                 else
                     this->analysis_res += " записной книжке: \r\n \r\n";
          for(int a=0; a<list.size();a++){
              Analysis_general res = list.at(a);
              this->analysis_res += "\t #"+ res.to_event_id + " сформированной " + res.to_event_detention_date + " " + res.to_event_detention_time+
                      " в отношении: "+ res.to_lastname + " " + res.to_name + " " + res.to_midname + ", по окраске: ???, в интересах "+
                      res.to_event_keeping_place;
              if(res.to_event_keeping_additional != "")
                  this->analysis_res +=" с пометкой " + res.to_event_keeping_additional;

              this->analysis_res +="\r\n";
          }
     }
}

void For_analysis::long_face_analysis(int eventId)
{
    this->analysis_res +="\t ПОЛНАЯ СПРАВКА ПРИВЯЗКА К ЛИЦАМ СОБЫТИЕ #"+QString::number(eventId)+" ";
    for(int i =0; i<this->analysisResultFull.keys().size(); i++){
        QString fio = this->analysisResultFull.keys().at(i);
        QMap<QString, QList<Analysis_general> > map = this->analysisResultFull[fio];
        this->analysis_res +=  "\r\n\r\nФигурант события " +fio+" возможно знаком:";

        QList<Analysis_general> var1List = map["var1"];
        if(var1List.size() > 0){
            this->analysis_res += "\r\n 1 ВАРИАНТ: ";
            for(int a=0; a<var1List.size();a++){
                Analysis_general res = var1List.at(a);

                this->analysis_res += " \r\n \t ЗК#"+res.to_event_id +" "  + "Номер телефона"+
                        res.from_telephone + " ";
                if( res.from_telephone_oldnum || res.from_telephone_internum ){
                    this->analysis_res += "(";

                    if( res.from_telephone_oldnum && res.from_telephone_internum )
                        this->analysis_res += "старый международный";

                    else if( res.from_telephone_oldnum )
                        this->analysis_res += "старый";

                    else if( res.from_telephone_internum )
                        this->analysis_res += "международный";

                    this->analysis_res += ")";
                }

                this->analysis_res += ", принадлежащий "+fio +
                        ", был обнаружен в списке телефонных контактов "+ res.to_lastname +" "+
                        res.to_name +" "+ res.to_midname+" с пометкой "+res.to_contact_alias;
            }
        }
        QList<Analysis_general> var2List = map["var2"];
        if(var2List.size() > 0){
            this->analysis_res += "\r\n 2 ВАРИАНТ: ";

            for(int a=0; a<var2List.size();a++){
              Analysis_general res = var2List.at(a);
              this->analysis_res += " \r\n \t ЗК#" + res.to_event_id +" телефонный номер " +
                      "списка контактов "+
                      res.from_contact +", с пометкой " + res.from_contact_alias + " в записной книжке, "+
                      "владельцем которой является " + fio + " обнаружен, как "+
                      "принадлежащий владельцу записной книжки: "+ res.to_lastname +" "+
                      res.to_name +" "+ res.to_midname + "; \r\n";
            }
        }

        QList<Analysis_general> var3List = map["var3"];
        if(var3List.size() > 0){
            this->analysis_res += "\r\n 3 ВАРИАНТ: ";
            for(int a=0; a<var3List.size();a++){
                Analysis_general res = var3List.at(a);
                this->analysis_res += " \r\n \t ЗК#"+res.to_event_id;
                this->analysis_res += " Номер телефона "+ res.to_contact + " с пометкой "
                        + res.from_contact_alias +", обнаруженный в списке контактов "
                        + fio + " был также обнаружен в списке контактов с пометкой "+
                        res.to_contact_alias + ", владельцем которого является "+
                        res.to_lastname +" "+ res.to_name +" "+ res.to_midname;
            }
        }
    }



//    for(int i=0; i<this->analysisResult.keys().size(); i++ ){
//        QString fio = this->analysisResult.keys().at(i);
//        QList<Analysis_general> list = this->analysisResult[fio];
//        this->analysis_res +=  "\r\nФигурант события " +fio+" возможно знаком";
//                if(list.size()>1)
//                    this->analysis_res+=" со следующими лицами: \r\n \r\n";
//                else
//                    this->analysis_res += "со следующим лицом: \r\n \r\n";
//        for(int a=0; a<list.size();a++){
//            Analysis_general res = list.at(a);
//            this->analysis_res += "\t" + res.to_lastname +" "+res.to_name +" "+ res.to_midname + " "+
//                        ", окраска ???, номер события: "+res.to_event_id+", дата добавления: "+
//                        res.to_event_detention_date+" "+ res.to_event_detention_time+";\r\n";
//        }
//    }
}
