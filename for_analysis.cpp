#include "for_analysis.h"

For_analysis::For_analysis()
{

}

void For_analysis::short_face_analysis_all_db(int id)
{
    analysis_res.clear();
        ///////////////////////////////////////
    querry.prepare("SELECT \"contacts\".\"FK_Cl_telephone\""
                   "FROM \"contacts\", \"owners_tel\""
                   "WHERE \"owners_tel\".\"FK_Telephone_Zk\" = (:id) AND"
                    "\"owners_tel\".\"Telephone_num\" = \"contacts\".\"cl_telephone\"");
    querry.bindValue(":id", id);

    if (!querry.exec())
        qDebug() << querry.lastError();

        ///////////////////////////////////////
    while (querry.next())
    {
        ///////////////////////////////////////
    temp.prepare("SELECT \"owners_tel\".\"FK_Telephone_Zk\""
        "FROM \"owners_tel\""
       " WHERE "
       " \"owners_tel\".\"Telephone_id\" = (:fk_id)");
        temp.bindValue(":fk_id",querry.value(0).toInt());
        if (!temp.exec())
            qDebug() << temp.lastError();

        ///////////////////////////////////////
       while (temp.next())
            {
        ///////////////////////////////////////
        temp_2.prepare("SELECT "
                       "regexp_replace(\"zk\".\"Lastname\", '\\s+$', ''), "
                       "regexp_replace(\"zk\".\"Name\", '\\s+$', ''), "
                       "regexp_replace(\"zk\".\"Mid_name\", '\\s+$', ''),"
                       "regexp_replace(\"zk\".\"Birth_date\", '\\s+$', ''),"
                       "regexp_replace(\"zk\".\"Date_add\", '\\s+$', ''),"
                       "regexp_replace(\"zk\".\"Check_for\", '\\s+$', '')"
                      " FROM \"zk\" "
                      " WHERE "
                       "\"zk\".\"Zk_id\" = (:zk_id)");
        temp_2.bindValue(":zk_id",temp.value(0).toInt());

        match_counter.append(temp.value(0).toInt());

        if (!temp_2.exec())
            qDebug() << temp_2.lastError();

        while (temp_2.next())
                {
            analysis_res += temp_2.value(0).toString()+" "+
                    temp_2.value(1).toString()+" "+
                    temp_2.value(2).toString()+", "+
                    temp_2.value(3).toString()+", окраска: ???, записная книжка сформирована "+
                    temp_2.value(4).toString()+" в интересах: "+
                    temp_2.value(5).toString()+"  \r\n \r\n";
                }

            }
    }
 if(!analysis_res.isEmpty())
  {
     QString temp_for_add;
     temp_3.prepare("SELECT COUNT( DISTINCT \"contacts\".\"FK_Cl_telephone\")"
                    "FROM \"contacts\", \"owners_tel\""
                    "WHERE \"owners_tel\".\"FK_Telephone_Zk\" = (:id) AND"
                     "\"owners_tel\".\"Telephone_num\" = \"contacts\".\"cl_telephone\"");
     temp_3.bindValue(":id", id);
     if (!temp_3.exec())
         qDebug() << temp_3.lastError();
     while (temp_3.next())
             {
         For_analysis::get_L_N_M(id);

         temp_for_add = "\t ФОРМИРОВАНИЕ КРАТКОЙ СПРАВКИ С ПРИВЯЗКОЙ К ЛИЦАМ \r\n \r\n"
                        "Владелец записной книжки ";
         temp_for_add += Lastname+" "+Name+" "+Midname+" "
                         " возможно знаком со ";

        if (temp_3.value(0).toInt() == 1)
            temp_for_add += " следующим лицом: \r\n \r\n";
        else
            temp_for_add += "следующими лицами: \r\n \r\n";
           }
     temp_3.clear();

     analysis_res.insert(0,temp_for_add);
  }
}

void For_analysis::short_tel_analysis_all_db(int id)
{
   analysis_res.clear();
    For_analysis::get_L_N_M(id);
    ///////////////////ВЫТАЩИЛ НОМЕРА ТЕЛЕФОНА////////////////////
 querry.prepare("SELECT DISTINCT \"owners_tel\".\"Telephone_num\""
                "FROM \"owners_tel\", \"contacts\""
                "WHERE \"owners_tel\".\"FK_Telephone_Zk\" = (:id)"
               " AND "
               " \"owners_tel\".\"Telephone_num\" = \"contacts\".\"cl_telephone\"");
 querry.bindValue(":id",id);
 if (!querry.exec())
     qDebug() << querry.lastError();

///////////////////QUERRY - НОМЕР АНАЛИЗИРУЕМОГО ТЕЛЕФОНА////////////////////
 while (querry.next())
         {

     Tel_num = querry.value(0).toString();
      analysis_res +="Номер телефона "+ Tel_num+", принадлежащий владельцу запиной книжки: ";
     ///////////////////ЗАПИСАЛ ФИО ВЛАДЕЛЬЦА////////////////////


      analysis_res += Lastname+" "+Name+" "+Midname+" обнаружен в ";

      ///////////////////ОПРЕДЕЛЯЮ КОЛ-ВО ЗАПИСЕЙ////////////////////
      temp.prepare("SELECT COUNT( DISTINCT \"contacts\".\"FK_Cl_telephone\")"
                   "FROM \"owners_tel\", \"contacts\" "
                   "WHERE \"owners_tel\".\"FK_Telephone_Zk\" = (:id)"
                  " AND "
                   "\"contacts\".\"cl_telephone\" = (:tel_num) ");
      temp.bindValue(":id",id);
      temp.bindValue(":tel_num",Tel_num);

      if (!temp.exec())
          qDebug() << temp.lastError();

      while (temp.next())
      {
          if(temp.value(0).toInt() == 1)
              analysis_res+="записной книжке: \r\n \r\n";
          else
              analysis_res+="записных книжках: \r\n \r\n";
      }
      temp.clear();
      ///////////////////ДОСТАЮ ИЗ ЗК НУЖНЫЕ ПОЛЯ////////////////////
      temp.prepare("SELECT \"contacts\".\"FK_Cl_telephone\", \"contacts\".\"cl_info\""
                   "FROM \"owners_tel\", \"contacts\""
                  " WHERE \"owners_tel\".\"FK_Telephone_Zk\" = (:id)"
                  " AND"
                  " \"owners_tel\".\"Telephone_num\" = \"contacts\".\"cl_telephone\""
                   "AND"
                   "\"contacts\".\"cl_telephone\" = (:tel_num)");
      temp.bindValue(":id",id);
      temp.bindValue(":tel_num",Tel_num);
      if (!temp.exec())
          qDebug() << temp.lastError();

        ///////////////////TEMP - ////////////////////
      while (temp.next())
      {
         temp_2.prepare("SELECT \"owners_tel\".\"FK_Telephone_Zk\""
                        "FROM \"owners_tel\""
                       " WHERE \"owners_tel\".\"Telephone_id\" = (:temp_val)");
         temp_2.bindValue(":temp_val",temp.value(0).toInt());
         if (!temp_2.exec())
             qDebug() << temp_2.lastError();

       ///////////////////TEMP_2 - НОМЕР ЗК С НОМЕРОМ ТЕЛЕФОНА////////////////////
         while (temp_2.next())
            {
                temp_3.prepare("SELECT "
                               "regexp_replace(\"zk\".\"Date_add\", '\\s+$', ''),"
                               "regexp_replace(\"zk\".\"Lastname\", '\\s+$', ''),"
                               "regexp_replace(\"zk\".\"Name\", '\\s+$', ''),"
                               "regexp_replace(\"zk\".\"Mid_name\", '\\s+$', ''),"
                               "regexp_replace(\"zk\".\"Birth_date\", '\\s+$', ''),"
                               "regexp_replace(\"zk\".\"Check_for\", '\\s+$', '')"
                               "FROM \"zk\""
                              " WHERE \"zk\".\"Zk_id\" = (:temp_2_val)");
                temp_3.bindValue(":temp_2_val",temp_2.value(0).toInt());
                if (!temp_3.exec())
                    qDebug() << temp_3.lastError();
                while (temp_3.next())
                {
                    analysis_res+="сформированной "+temp_3.value(0).toString()+
                            " в отношении: "+temp_3.value(1).toString()+" "+
                            temp_3.value(2).toString()+" "+
                            temp_3.value(3).toString()+", "+temp_3.value(4).toString()+" по окраске: ???, в интересах "+
                            temp_3.value(5).toString()+ " ";
                    if (!temp.value(1).toString().isEmpty())
                        analysis_res += "с пометкой "+temp.value(1).toString()+"\r\n \r\n";
                    else
                        analysis_res +="\r\n \r\n";
                }
                temp_3.clear();
            }
         }
 }

 if(!analysis_res.isEmpty())
   analysis_res.insert(0,"\t ФОРМИРОВАНИЕ КРАТКОЙ СПРАВКИ С ПРИВЯЗКОЙ К ТЕЛЕФОННЫМ НОМЕРАМ \r\n \r\n");
}

void For_analysis::long_face_analysis_all_db(int id)
{
    For_analysis::get_L_N_M(id);
    analysis_res.clear();
        ///////////////////////////////////////
    querry.prepare("SELECT \"contacts\".\"FK_Cl_telephone\", \"contacts\".\"cl_info\""
                   "FROM \"contacts\", \"owners_tel\""
                   "WHERE \"owners_tel\".\"FK_Telephone_Zk\" = (:id) AND"
                    "\"owners_tel\".\"Telephone_num\" = \"contacts\".\"cl_telephone\"");
    querry.bindValue(":id", id);

    if (!querry.exec())
        qDebug() << querry.lastError();

        ///////////////////////////////////////
    while (querry.next())
    {
        ///////////////////////////////////////
    temp.prepare("SELECT \"owners_tel\".\"FK_Telephone_Zk\",\"owners_tel\".\"Telephone_num\" "
        "FROM \"owners_tel\""
       " WHERE "
       " \"owners_tel\".\"Telephone_id\" = (:fk_id)");
        temp.bindValue(":fk_id",querry.value(0).toInt());
        if (!temp.exec())
            qDebug() << temp.lastError();

        ///////////////////////////////////////
       while (temp.next())
            {
        ///////////////////////////////////////
        temp_2.prepare("SELECT "
        /*0*/          "regexp_replace(\"zk\".\"Lastname\", '\\s+$', ''), "
                       "regexp_replace(\"zk\".\"Name\", '\\s+$', ''), "
                       "regexp_replace(\"zk\".\"Mid_name\", '\\s+$', ''),"
                       "regexp_replace(\"zk\".\"Birth_date\", '\\s+$', ''),"
                       ""
        /*4*/          "regexp_replace(\"zk\".\"Reg_city\", '\\s+$', ''),"
                       "regexp_replace(\"zk\".\"Reg_street\", '\\s+$', ''),"
                       "regexp_replace(\"zk\".\"Reg_home\", '\\s+$', ''),"
                       "regexp_replace(\"zk\".\"Reg_corp\", '\\s+$', ''),"
                       "regexp_replace(\"zk\".\"Reg_flat\", '\\s+$', ''),"
                       ""
        /*9*/          "regexp_replace(\"zk\".\"Dop_info\", '\\s+$', ''),"
                       "regexp_replace(\"zk\".\"Date_add\", '\\s+$', ''),"
                       "regexp_replace(\"zk\".\"Check_for\", '\\s+$', '')"
                      " FROM \"zk\" "
                      " WHERE "
                       "\"zk\".\"Zk_id\" = (:zk_id)");
        temp_2.bindValue(":zk_id",temp.value(0).toInt());
        if (!temp_2.exec())
            qDebug() << temp_2.lastError();

        while (temp_2.next())
                {
            analysis_res += temp_2.value(0).toString()+" "+
                    temp_2.value(1).toString()+" "+
                    temp_2.value(2).toString()+", "+
                    temp_2.value(3).toString()+", адрес регистрации: "+
                    temp_2.value(4).toString()+" "+temp_2.value(5).toString()+" "+
                    temp_2.value(6).toString()+" "+temp_2.value(7).toString()+" "+
                    temp_2.value(8).toString()+", "+" телефон: "+
                    temp.value(1).toString()+ ", окраска: ???, дополнительная информация: "+
                    temp_2.value(9).toString()+", записная книжка сформирована "+
                    temp_2.value(10).toString()+" в интересах: "+
                    temp_2.value(11).toString()+"  \r\n \t Вывод о знакомстве сделан на основании того, что: \r\n";

            temp_3.prepare("SELECT DISTINCT \"owners_tel\".\"Telephone_num\""
                                            "FROM \"owners_tel\", \"contacts\""
                                            "WHERE \"owners_tel\".\"FK_Telephone_Zk\" = (:id)"
                                           "AND "
                                           "\"owners_tel\".\"Telephone_num\" = \"contacts\".\"cl_telephone\""
                                            "AND"
                                            "\"contacts\".\"FK_Cl_telephone\" = (:fk)");
                             temp_3.bindValue(":id",id);
                             temp_3.bindValue(":fk",querry.value(0));
                             if (!temp_3.exec())
                                 qDebug() << temp_3.lastError();

                            ///////////////////temp_3 - НОМЕР АНАЛИЗИРУЕМОГО ТЕЛЕФОНА////////////////////
                             while (temp_3.next())
                                     {
                                 Tel_num = temp_3.value(0).toString();
                                     }
                             temp_3.clear();

                             analysis_res+="Номер телефона "+Tel_num+", принадлежащий "+Lastname+" "+Name+" "+Midname+" "+
                                     ", обнаружен в записной книжке, владельцем которой является "+
                    temp_2.value(0).toString()+" "+temp_2.value(1).toString()+" "+ temp_2.value(2).toString()+" ";
                             if (!querry.value(1).toString().isEmpty())
                                 analysis_res += "с пометкой "+querry.value(1).toString()+"\r\n \r\n";
                             else
                                 analysis_res +="\r\n \r\n";
                }

            }
    }
    //////////////-------ВТОРОЙ ВАРИАНТ ЗНАКОМСТВА-------/////////////////
    temp.clear();
    temp_2.clear();
    querry.clear();
    querry.prepare("SELECT"
                   "\"owners_tel\".\"Telephone_id\" "
                  " FROM"
                  " \"owners_tel\" "
                  " WHERE"
                  " \"owners_tel\".\"FK_Telephone_Zk\" = (:id)");
    querry.bindValue(":id", id);

    if (!querry.exec())
        qDebug() << querry.lastError();
    while (querry.next())
    {
        temp.prepare("SELECT"
                     "\"owners_tel\".\"FK_Telephone_Zk\", "
                     "\"owners_tel\".\"Telephone_num\", "
                     "regexp_replace(\"contacts\".\"cl_info\", '\\s+$', '')"
                    " FROM"
                   " \"contacts\",\"owners_tel\""
                   "  WHERE "
                   "  \"contacts\".\"cl_telephone\" = \"owners_tel\".\"Telephone_num\" "
                   "  AND "
                   "  \"contacts\".\"FK_Cl_telephone\" = (:fk)");
        temp.bindValue(":fk", querry.value(0));
        if (!temp.exec())
            qDebug() << temp.lastError();
        while (temp.next())
        {
            temp_2.prepare("SELECT "
            /*0*/          "regexp_replace(\"zk\".\"Lastname\", '\\s+$', ''), "
                           "regexp_replace(\"zk\".\"Name\", '\\s+$', ''), "
                           "regexp_replace(\"zk\".\"Mid_name\", '\\s+$', ''),"
                           "regexp_replace(\"zk\".\"Birth_date\", '\\s+$', ''),"
                           ""
            /*4*/          "regexp_replace(\"zk\".\"Reg_city\", '\\s+$', ''),"
                           "regexp_replace(\"zk\".\"Reg_street\", '\\s+$', ''),"
                           "regexp_replace(\"zk\".\"Reg_home\", '\\s+$', ''),"
                           "regexp_replace(\"zk\".\"Reg_corp\", '\\s+$', ''),"
                           "regexp_replace(\"zk\".\"Reg_flat\", '\\s+$', ''),"
                           ""
            /*9*/          "regexp_replace(\"zk\".\"Dop_info\", '\\s+$', ''),"
                           "regexp_replace(\"zk\".\"Date_add\", '\\s+$', ''),"
                           "regexp_replace(\"zk\".\"Check_for\", '\\s+$', '')"
                          " FROM \"zk\" "
                          " WHERE "
                           "\"zk\".\"Zk_id\" = (:zk_id)");
            temp_2.bindValue(":zk_id",temp.value(0).toInt());
            if (!temp_2.exec())
                qDebug() << temp_2.lastError();

            while (temp_2.next())
                    {
                analysis_res += temp_2.value(0).toString()+" "+
                        temp_2.value(1).toString()+" "+
                        temp_2.value(2).toString()+", "+
                        temp_2.value(3).toString()+", адрес регистрации: "+
                        temp_2.value(4).toString()+" "+temp_2.value(5).toString()+" "+
                        temp_2.value(6).toString()+" "+temp_2.value(7).toString()+" "+
                        temp_2.value(8).toString()+", "+" телефон: "+
                        temp.value(1).toString()+ ", окраска: ???, дополнительная информация: "+
                        temp_2.value(9).toString()+", записная книжка сформирована "+
                        temp_2.value(10).toString()+" в интересах: "+
                        temp_2.value(11).toString()+"  \r\n \t Вывод о знакомстве сделан на основании того, что: \r\n"+
                    "Номер телефона "+temp.value(1).toString()+", ";

                if(!temp.value(2).toString().isEmpty())
                    analysis_res+=" с пометкой "+temp.value(2).toString() + " ";
                analysis_res+=" в записной книжке, владельцем которой является "
                        +Lastname+" "+Name+" "+Midname+" обнаружен, как принадлежащий владельцу записной книжки: "+
                        temp_2.value(0).toString()+" "+
                        temp_2.value(1).toString()+" "+
                        temp_2.value(2).toString()+" \r\n";
                    }
        }
    }
    if(!analysis_res.isEmpty())
    {
        For_analysis::get_L_N_M(id);
        QString temp_for_add;
        temp_for_add = "\t ФОРМИРОВАНИЕ ПОЛНОЙ СПРАВКИ С ПРИВЯЗКОЙ К ЛИЦАМ \r\n \r\n"
                       "Владелец записной книжки ";
            ///////////////////////////////////////

            temp_for_add += Lastname+" "+Name+" "+Midname+" "
                            " возможно знаком со ";
            For_analysis::get_L_N_M(id);
            ///////////////////////////////////////
            temp_3.prepare("SELECT COUNT(DISTINCT \"contacts\".\"FK_Cl_telephone\")"
                           "FROM \"contacts\", \"owners_tel\""
                           "WHERE \"owners_tel\".\"FK_Telephone_Zk\" = (:id) AND"
                            "\"owners_tel\".\"Telephone_num\" = \"contacts\".\"cl_telephone\"");
            temp_3.bindValue(":id", id);
            if (!temp_3.exec())
                qDebug() << temp_3.lastError();
            while (temp_3.next())
                    {
               if (temp_3.value(0).toInt() == 1)
                   temp_for_add += " следующим лицом: \r\n \r\n";
               else
                   temp_for_add += "следующими лицами: \r\n \r\n";
                    }
            temp_3.clear();

            analysis_res.insert(0,temp_for_add);
    }
}

void For_analysis::long_tel_analysis_all_db(int id)
{
    For_analysis::get_L_N_M(id);
    analysis_res.clear();
    QSqlQuery new_temp;

    querry.prepare("SELECT DISTINCT \"contacts\".\"FK_Cl_telephone\", \"contacts\".\"cl_info\""
                   "FROM \"contacts\", \"owners_tel\""
                   "WHERE \"owners_tel\".\"FK_Telephone_Zk\" = (:id) AND"
                    "\"owners_tel\".\"Telephone_num\" = \"contacts\".\"cl_telephone\"");
    querry.bindValue(":id", id);
    if (!querry.exec())
        qDebug() << querry.lastError();

    while (querry.next())
    {
        qDebug() << querry.value(0).toString();
        new_temp.prepare("SELECT DISTINCT \"owners_tel\".\"Telephone_num\""
                                        "FROM \"owners_tel\", \"contacts\""
                                        "WHERE \"owners_tel\".\"FK_Telephone_Zk\" = (:id)"
                                       "AND "
                                       "\"owners_tel\".\"Telephone_num\" = \"contacts\".\"cl_telephone\""
                                        "AND"
                                        "\"contacts\".\"FK_Cl_telephone\" = (:fk)");
                         new_temp.bindValue(":id",id);
                         new_temp.bindValue(":fk",querry.value(0));
                         if (!new_temp.exec())
                             qDebug() << new_temp.lastError();

                        ///////////////////new_temp - НОМЕР АНАЛИЗИРУЕМОГО ТЕЛЕФОНА////////////////////
                         while (new_temp.next())
                         {
                             if (Tel_num != new_temp.value(0).toString())
                                {
                                    Tel_num =  new_temp.value(0).toString();

                         analysis_res+="Номер телефона "+Tel_num+", принадлежащий владельцу записной книжки: "+
                                 Lastname+" "+Name+" "+Midname+" обнаружен в ";
                         temp_3.prepare("SELECT COUNT(DISTINCT \"contacts\".\"FK_Cl_telephone\")"
                                        "FROM \"contacts\", \"owners_tel\""
                                        "WHERE \"owners_tel\".\"FK_Telephone_Zk\" = (:id) AND"
                                         "\"contacts\".\"cl_telephone\" = (:tel_num)");
                         temp_3.bindValue(":id", id);
                         temp_3.bindValue(":tel_num",Tel_num);
                         if (!temp_3.exec())
                             qDebug() << temp_3.lastError();
                         while (temp_3.next())
                                 {
                            if (temp_3.value(0).toInt() == 1)
                                analysis_res += " записной книжке: \r\n \r\n";
                            else
                                analysis_res += "записных книжках: \r\n \r\n";
                                 }
                         temp_3.clear();
                          ///////////////////////////////////////
                         temp.prepare("SELECT DISTINCT \"contacts\".\"FK_Cl_telephone\", \"contacts\".\"cl_info\""
                                        "FROM \"contacts\", \"owners_tel\""
                                        "WHERE \"owners_tel\".\"FK_Telephone_Zk\" = (:id) AND"
                                         " \"contacts\".\"cl_telephone\" = (:tel_num)");
                         temp.bindValue(":id", id);
                         temp.bindValue(":tel_num",Tel_num);
                         if (!temp.exec())
                             qDebug() << temp.lastError();


                         while (temp.next())
                         {
                         temp_3.prepare("SELECT \"owners_tel\".\"FK_Telephone_Zk\",\"owners_tel\".\"Telephone_num\" "
                             "FROM \"owners_tel\""
                            " WHERE "
                            " \"owners_tel\".\"Telephone_id\" = (:fk_id)");
                             temp_3.bindValue(":fk_id",temp.value(0).toInt());
                             if (!temp_3.exec())
                                 qDebug() << temp_3.lastError();

                            while (temp_3.next())
                                 {
                             temp_2.prepare("SELECT "
                             /*0*/          "regexp_replace(\"zk\".\"Lastname\", '\\s+$', ''), "
                                            "regexp_replace(\"zk\".\"Name\", '\\s+$', ''), "
                                            "regexp_replace(\"zk\".\"Mid_name\", '\\s+$', ''),"
                                            "regexp_replace(\"zk\".\"Birth_date\", '\\s+$', ''),"
                                            ""
                             /*4*/          "regexp_replace(\"zk\".\"Reg_city\", '\\s+$', ''),"
                                            "regexp_replace(\"zk\".\"Reg_street\", '\\s+$', ''),"
                                            "regexp_replace(\"zk\".\"Reg_home\", '\\s+$', ''),"
                                            "regexp_replace(\"zk\".\"Reg_corp\", '\\s+$', ''),"
                                            "regexp_replace(\"zk\".\"Reg_flat\", '\\s+$', ''),"
                                            ""
                             /*9*/          "regexp_replace(\"zk\".\"Dop_info\", '\\s+$', ''),"
                                            "regexp_replace(\"zk\".\"Date_add\", '\\s+$', ''),"
                                            "regexp_replace(\"zk\".\"Check_for\", '\\s+$', '')"
                                           " FROM \"zk\" "
                                           " WHERE "
                                            "\"zk\".\"Zk_id\" = (:zk_id)");
                             temp_2.bindValue(":zk_id",temp_3.value(0).toInt());
                             if (!temp_2.exec())
                                 qDebug() << temp_2.lastError();

                             while (temp_2.next())
                                        {
                                 analysis_res += temp_2.value(0).toString()+" "+
                                         temp_2.value(1).toString()+" "+
                                         temp_2.value(2).toString()+", "+
                                         temp_2.value(3).toString()+", адрес регистрации: "+
                                         temp_2.value(4).toString()+" "+temp_2.value(5).toString()+" "+
                                         temp_2.value(6).toString()+" "+temp_2.value(7).toString()+" "+
                                         temp_2.value(8).toString()+", "+" телефон: "+
                                         temp_3.value(1).toString()+ ", окраска: ???, дополнительная информация: "+
                                         temp_2.value(9).toString()+", записная книжка сформирована "+
                                         temp_2.value(10).toString()+" в интересах: "+
                                         temp_2.value(11).toString();
                                 if(!temp.value(1).toString().isEmpty())
                                     analysis_res+=" c пометкой "+ temp.value(1).toString();
                                 analysis_res+="\r\n \r\n";
                                        }
                                }
                             }
                             }
                         }
    }

    if (!analysis_res.isEmpty())
    {
         analysis_res.insert(0,"\t ФОРМИРОВАНИЕ ПОЛНОЙ СПРАВКИ С ПРИВЯЗКОЙ К НОМЕРАМ \r\n \r\n");
    }
}

void For_analysis::short_face_analysis_all_db(QVector<int> vector, int id)
{
    analysis_res.clear();
    int counter = 0;
   querry.prepare("SELECT DISTINCT \"owners_tel\".\"Telephone_num\""
                  "FROM  \"owners_tel\""
                  "WHERE \"owners_tel\".\"FK_Telephone_Zk\" = (:id)");
   querry.bindValue(":id",id);
   if (!querry.exec())
       qDebug() << querry.lastError();

   while (querry.next())
   {
       qDebug() <<"querry   "+ querry.value(0).toString();
     for (int a =0; a<vector.size(); a++)
       {
         qDebug() << vector.at(a);

           temp.prepare("SELECT DISTINCT \"owners_tel\".\"Telephone_id\""
                          "FROM  \"owners_tel\""
                          "WHERE \"owners_tel\".\"FK_Telephone_Zk\" = (:id)");
           temp.bindValue(":id", vector.at(a));
           if (!temp.exec())
               qDebug() << temp.lastError();

           while (temp.next())
           {
               qDebug() << "temp  " + temp.value(0).toString();

    /**/           temp_2.prepare("SELECT DISTINCT \"contacts\".\"cl_info\", \"contacts\".\"cl_telephone\" "
                           "FROM \"contacts\", \"owners_tel\""
                           "WHERE \"contacts\".\"cl_telephone\" = (:an_tel)"
                           "AND \"contacts\".\"FK_Cl_telephone\" = (:id)");

               temp_2.bindValue(":an_tel",querry.value(0).toString());
               qDebug() <<querry.value(0).toString();

               temp_2.bindValue(":id",temp.value(0).toInt());
               qDebug() <<temp.value(0).toInt();

               if (!temp_2.exec())
                   qDebug() << temp_2.lastError();

               while (temp_2.next())
               {
                  counter++;
                        temp_3.prepare("SELECT "
                                      "regexp_replace(\"zk\".\"Lastname\", '\\s+$', ''), "
                                      "regexp_replace(\"zk\".\"Name\", '\\s+$', ''), "
                                      "regexp_replace(\"zk\".\"Mid_name\", '\\s+$', ''),"
                                      "regexp_replace(\"zk\".\"Birth_date\", '\\s+$', ''),"
                                      "regexp_replace(\"zk\".\"Date_add\", '\\s+$', ''),"
                                      "regexp_replace(\"zk\".\"Check_for\", '\\s+$', '')"
                                     " FROM \"zk\" "
                                     " WHERE "
                                      "\"zk\".\"Zk_id\" = (:zk_id)");
                       temp_3.bindValue(":zk_id",vector.at(a));

                       if (!temp_3.exec())
                           qDebug() << temp_3.lastError();
                       while (temp_3.next())
                               {
                           analysis_res += temp_3.value(0).toString()+" "+
                                   temp_3.value(1).toString()+" "+
                                   temp_3.value(2).toString()+", "+
                                   temp_3.value(3).toString()+", окраска: ???, записная книжка сформирована "+
                                   temp_3.value(4).toString()+" в интересах: "+
                                   temp_3.value(5).toString()+"  \r\n \r\n";
                               }
                       temp_3.clear();
               }

           }
       }
   }
   if(counter > 0)
   {
       QString for_insert;
       For_analysis::get_L_N_M(id);

                              for_insert = "\t ФОРМИРОВАНИЕ КРАТКОЙ СПРАВКИ С ПРИВЯЗКОЙ К ЛИЦАМ \r\n \r\n"
                                             "Владелец записной книжки ";
                              for_insert += Lastname+" "+Name+" "+Midname+" "
                                              " возможно знаком со ";
                              if(counter==1)
                                  for_insert += " следующим лицом: \r\n \r\n";
                              else
                                  for_insert += "следующими лицами: \r\n \r\n";
     analysis_res.insert(0,for_insert);
   }
}

void For_analysis::short_tel_analysis_all_db(QVector<int> vector, int id)
{
    analysis_res.clear();
    For_analysis::get_L_N_M(id);

   querry.prepare("SELECT DISTINCT \"owners_tel\".\"Telephone_num\""
                  "FROM  \"owners_tel\""
                  "WHERE \"owners_tel\".\"FK_Telephone_Zk\" = (:id)");
   querry.bindValue(":id",id);
   if (!querry.exec())
       qDebug() << querry.lastError();

   while (querry.next())
   {
       for (int a =0; a<vector.size(); a++)
       {
         qDebug() << vector.at(a);

           temp.prepare("SELECT DISTINCT \"owners_tel\".\"Telephone_id\""
                          "FROM  \"owners_tel\""
                          "WHERE \"owners_tel\".\"FK_Telephone_Zk\" = (:id)");
           temp.bindValue(":id", vector.at(a));
           if (!temp.exec())
               qDebug() << temp.lastError();

           while (temp.next())
           {
    /**/      temp_2.prepare("SELECT DISTINCT \"contacts\".\"cl_info\", \"contacts\".\"cl_telephone\" "
                           "FROM \"contacts\", \"owners_tel\""
                           "WHERE \"contacts\".\"cl_telephone\" = (:an_tel)"
                           "AND \"contacts\".\"FK_Cl_telephone\" = (:id)");

               temp_2.bindValue(":an_tel",querry.value(0).toString());

               temp_2.bindValue(":id",temp.value(0).toInt());

               if (!temp_2.exec())
                   qDebug() << temp_2.lastError();


               while (temp_2.next())
               {
                   if (Tel_num != querry.value(0).toString())
                {
                    Tel_num =  querry.value(0).toString();

                   analysis_res +="Номер телефона "+ Tel_num+", принадлежащий владельцу запиной книжки: ";

                   analysis_res += Lastname+" "+Name+" "+Midname+" обнаружен в ";
                   ///////////////////ОПРЕДЕЛЯЮ КОЛ-ВО ЗАПИСЕЙ////////////////////
                   temp_3.prepare("SELECT COUNT( DISTINCT \"contacts\".\"FK_Cl_telephone\")"
                                "FROM \"owners_tel\", \"contacts\" "
                                "WHERE \"owners_tel\".\"FK_Telephone_Zk\" = (:id)"
                               " AND "
                                "\"contacts\".\"cl_telephone\" = (:tel_num) ");
                   temp_3.bindValue(":id",id);
                   temp_3.bindValue(":tel_num",Tel_num);

                   if (!temp_3.exec())
                       qDebug() << temp_3.lastError();

                   while (temp_3.next())
                   {
                       if(temp_3.value(0).toInt() == 1)
                           analysis_res+="записной книжке: \r\n \r\n";
                       else
                           analysis_res+="записных книжках: \r\n \r\n";
                   }
                   temp_3.clear();
                 }
                   temp_3.prepare("SELECT "
                                  "regexp_replace(\"zk\".\"Date_add\", '\\s+$', ''),"
                                  "regexp_replace(\"zk\".\"Lastname\", '\\s+$', ''),"
                                  "regexp_replace(\"zk\".\"Name\", '\\s+$', ''),"
                                  "regexp_replace(\"zk\".\"Mid_name\", '\\s+$', ''),"
                                  "regexp_replace(\"zk\".\"Birth_date\", '\\s+$', ''),"
                                  "regexp_replace(\"zk\".\"Check_for\", '\\s+$', '')"
                                  "FROM \"zk\""
                                 " WHERE \"zk\".\"Zk_id\" = (:temp_2_val)");
                   temp_3.bindValue(":temp_2_val",vector.at(a));
                   if (!temp_3.exec())
                       qDebug() << temp_3.lastError();
                   while (temp_3.next())
                   {
                       analysis_res+="сформированной "+temp_3.value(0).toString()+
                               " в отношении: "+temp_3.value(1).toString()+" "+
                               temp_3.value(2).toString()+" "+
                               temp_3.value(3).toString()+", "+temp_3.value(4).toString()+" по окраске: ???, в интересах "+
                               temp_3.value(5).toString()+ " ";
                       if (!temp_2.value(0).toString().isEmpty())
                           analysis_res += "с пометкой "+temp_2.value(0).toString()+"\r\n \r\n";
                       else
                           analysis_res +="\r\n \r\n";
                   }
                   temp_3.clear();
               }

           }
       }

   }
   if(!analysis_res.isEmpty())
   {
       analysis_res.insert(0,"\t ФОРМИРОВАНИЕ КРАТКОЙ СПРАВКИ С ПРИВЯЗКОЙ К НОМЕРАМ \r\n \r\n");
   }
}

void For_analysis::long_face_analysis_all_db(QVector<int> vector, int id)
{
    For_analysis::get_L_N_M(id);
    analysis_res.clear();
    int counter = 0;
   querry.prepare("SELECT DISTINCT \"owners_tel\".\"Telephone_num\""
                  "FROM  \"owners_tel\""
                  "WHERE \"owners_tel\".\"FK_Telephone_Zk\" = (:id)");
   querry.bindValue(":id",id);
   if (!querry.exec())
       qDebug() << querry.lastError();

   while (querry.next())
   {
       qDebug() <<"querry   "+ querry.value(0).toString();
     for (int a =0; a<vector.size(); a++)
       {
         qDebug() << vector.at(a);

           temp.prepare("SELECT DISTINCT \"owners_tel\".\"Telephone_id\",\"owners_tel\".\"Telephone_num\""
                          "FROM  \"owners_tel\""
                          "WHERE \"owners_tel\".\"FK_Telephone_Zk\" = (:id)");
           temp.bindValue(":id", vector.at(a));
           if (!temp.exec())
               qDebug() << temp.lastError();

           while (temp.next())
           {
               temp_2.prepare("SELECT DISTINCT \"contacts\".\"cl_info\", \"contacts\".\"cl_telephone\" "
                                         "FROM \"contacts\", \"owners_tel\""
                                         "WHERE \"contacts\".\"cl_telephone\" = (:an_tel)"
                                         "AND \"contacts\".\"FK_Cl_telephone\" = (:id)");

                             temp_2.bindValue(":an_tel",querry.value(0).toString());
                             qDebug() <<querry.value(0).toString();

                             temp_2.bindValue(":id",temp.value(0).toInt());
                             qDebug() <<temp.value(0).toInt();

                             if (!temp_2.exec())
                                 qDebug() << temp_2.lastError();

                             while (temp_2.next())
                             {
                                 counter++;
                                       temp_3.prepare("SELECT "
          /*0*/          "regexp_replace(\"zk\".\"Lastname\", '\\s+$', ''), "
                         "regexp_replace(\"zk\".\"Name\", '\\s+$', ''), "
                         "regexp_replace(\"zk\".\"Mid_name\", '\\s+$', ''),"
                         "regexp_replace(\"zk\".\"Birth_date\", '\\s+$', ''),"
                         ""
          /*4*/          "regexp_replace(\"zk\".\"Reg_city\", '\\s+$', ''),"
                         "regexp_replace(\"zk\".\"Reg_street\", '\\s+$', ''),"
                         "regexp_replace(\"zk\".\"Reg_home\", '\\s+$', ''),"
                         "regexp_replace(\"zk\".\"Reg_corp\", '\\s+$', ''),"
                         "regexp_replace(\"zk\".\"Reg_flat\", '\\s+$', ''),"
                          ""
          /*9*/          "regexp_replace(\"zk\".\"Dop_info\", '\\s+$', ''),"
                         "regexp_replace(\"zk\".\"Date_add\", '\\s+$', ''),"
                         "regexp_replace(\"zk\".\"Check_for\", '\\s+$', '')"
                         " FROM \"zk\" "
                         " WHERE "
                         "\"zk\".\"Zk_id\" = (:zk_id)");
                                      temp_3.bindValue(":zk_id",vector.at(a));
                                    QString str = QString::number(vector.at(a));
                                      if (!temp_3.exec())
                                          qDebug() << temp_3.lastError();
                                      while (temp_3.next())
                                              {
                                          analysis_res += "Номер ЗК - "+str+" "+temp_3.value(0).toString()+" "+
                                                  temp_3.value(1).toString()+" "+
                                                  temp_3.value(2).toString()+", "+
                                                  temp_3.value(3).toString()+", адрес регистрации: "+
                                                  temp_3.value(4).toString()+" "+temp_3.value(5).toString()+" "+
                                                  temp_3.value(6).toString()+" "+temp_3.value(7).toString()+" "+
                                                  temp_3.value(8).toString()+", "+" телефон: "+
                                                  temp.value(1).toString()+ ", окраска: ???, дополнительная информация: "+
                                                  temp_3.value(9).toString()+", записная книжка сформирована "+
                                                  temp_3.value(10).toString()+" в интересах: "+
                                                  temp_3.value(11).toString()+"  \r\n \t Вывод о знакомстве сделан на основании того, что: \r\n";

                                                  analysis_res+="Номер телефона "+querry.value(0).toString()+", принадлежащий "+Lastname+" "+Name+" "+Midname+" "+
                                                    ", обнаружен в записной книжке, владельцем которой является "+
                                                  temp_3.value(0).toString()+" "+temp_3.value(1).toString()+" "+ temp_3.value(2).toString()+" ";
                                                           if (!temp_2.value(0).toString().isEmpty())
                                                               analysis_res += "с пометкой "+temp_2.value(0).toString()+"\r\n \r\n";
                                                           else
                                                               analysis_res +="\r\n \r\n";
                                               }
                              temp_3.clear();
                             }
           }
     }
   }

   //////////////-------ВТОРОЙ ВАРИАНТ ЗНАКОМСТВА-------/////////////////
   temp.clear();
   temp_2.clear();
   querry.clear();
   querry.prepare("SELECT"
                  "\"owners_tel\".\"Telephone_id\" "
                 " FROM"
                 " \"owners_tel\" "
                 " WHERE"
                 " \"owners_tel\".\"FK_Telephone_Zk\" = (:id)");
   querry.bindValue(":id", id);

   if (!querry.exec())
       qDebug() << querry.lastError();
   while (querry.next())
   {
       temp.prepare("SELECT"
                    "\"owners_tel\".\"FK_Telephone_Zk\", "
                    "\"owners_tel\".\"Telephone_num\", "
                    "regexp_replace(\"contacts\".\"cl_info\", '\\s+$', '')"
                   " FROM"
                  " \"contacts\",\"owners_tel\""
                  "  WHERE "
                  "  \"contacts\".\"cl_telephone\" = \"owners_tel\".\"Telephone_num\" "
                  "  AND "
                  "  \"contacts\".\"FK_Cl_telephone\" = (:fk)");
       temp.bindValue(":fk", querry.value(0));
       if (!temp.exec())
           qDebug() << temp.lastError();
       while (temp.next())
       {
           for (int a = 0; a < vector.size(); a++)
           {
            if(vector.at(a) == temp.value(0).toInt())
            {
                temp_2.prepare("SELECT "
                /*0*/          "regexp_replace(\"zk\".\"Lastname\", '\\s+$', ''), "
                               "regexp_replace(\"zk\".\"Name\", '\\s+$', ''), "
                               "regexp_replace(\"zk\".\"Mid_name\", '\\s+$', ''),"
                               "regexp_replace(\"zk\".\"Birth_date\", '\\s+$', ''),"
                               ""
                /*4*/          "regexp_replace(\"zk\".\"Reg_city\", '\\s+$', ''),"
                               "regexp_replace(\"zk\".\"Reg_street\", '\\s+$', ''),"
                               "regexp_replace(\"zk\".\"Reg_home\", '\\s+$', ''),"
                               "regexp_replace(\"zk\".\"Reg_corp\", '\\s+$', ''),"
                               "regexp_replace(\"zk\".\"Reg_flat\", '\\s+$', ''),"
                               ""
                /*9*/          "regexp_replace(\"zk\".\"Dop_info\", '\\s+$', ''),"
                               "regexp_replace(\"zk\".\"Date_add\", '\\s+$', ''),"
                               "regexp_replace(\"zk\".\"Check_for\", '\\s+$', '')"
                              " FROM \"zk\" "
                              " WHERE "
                               "\"zk\".\"Zk_id\" = (:zk_id)");
                temp_2.bindValue(":zk_id",temp.value(0).toInt());
                if (!temp_2.exec())
                    qDebug() << temp_2.lastError();

                while (temp_2.next())
                        {
                    analysis_res += temp_2.value(0).toString()+" "+
                            temp_2.value(1).toString()+" "+
                            temp_2.value(2).toString()+", "+
                            temp_2.value(3).toString()+", адрес регистрации: "+
                            temp_2.value(4).toString()+" "+temp_2.value(5).toString()+" "+
                            temp_2.value(6).toString()+" "+temp_2.value(7).toString()+" "+
                            temp_2.value(8).toString()+", "+" телефон: "+
                            temp.value(1).toString()+ ", окраска: ???, дополнительная информация: "+
                            temp_2.value(9).toString()+", записная книжка сформирована "+
                            temp_2.value(10).toString()+" в интересах: "+
                            temp_2.value(11).toString()+"  \r\n \t Вывод о знакомстве сделан на основании того, что: \r\n"+
                        "Номер телефона "+temp.value(1).toString()+", ";

                    if(!temp.value(2).toString().isEmpty())
                        analysis_res+=" с пометкой "+temp.value(2).toString() + " ";
                    analysis_res+=" в записной книжке, владельцем которой является "
                            +Lastname+" "+Name+" "+Midname+" обнаружен, как принадлежащий владельцу записной книжки: "+
                            temp_2.value(0).toString()+" "+
                            temp_2.value(1).toString()+" "+
                            temp_2.value(2).toString()+" \r\n";
                        }
           }
         }
      }
   }
   if(!analysis_res.isEmpty())
   {
       QString temp_for_add;
       temp_for_add = "\t ФОРМИРОВАНИЕ ПОЛНОЙ СПРАВКИ С ПРИВЯЗКОЙ К ЛИЦАМ \r\n \r\n"
                      "Владелец записной книжки ";
           ///////////////////////////////////////

           temp_for_add += Lastname+" "+Name+" "+Midname+" "
                           " возможно знаком со ";
           For_analysis::get_L_N_M(id);
           ///////////////////////////////////////
           temp_3.prepare("SELECT COUNT(DISTINCT \"contacts\".\"FK_Cl_telephone\")"
                          "FROM \"contacts\", \"owners_tel\""
                          "WHERE \"owners_tel\".\"FK_Telephone_Zk\" = (:id) AND"
                           "\"owners_tel\".\"Telephone_num\" = \"contacts\".\"cl_telephone\"");
           temp_3.bindValue(":id", id);
           if (!temp_3.exec())
               qDebug() << temp_3.lastError();
           while (temp_3.next())
                   {
              if (temp_3.value(0).toInt() == 1)
                  temp_for_add += " следующим лицом: \r\n \r\n";
              else
                  temp_for_add += "следующими лицами: \r\n \r\n";
                   }
           temp_3.clear();

           analysis_res.insert(0,temp_for_add);
   }
}

void For_analysis::long_tel_analysis_all_db(QVector<int> vector, int id)
{
    analysis_res.clear();
    For_analysis::get_L_N_M(id);

   querry.prepare("SELECT DISTINCT \"owners_tel\".\"Telephone_num\""
                  "FROM  \"owners_tel\""
                  "WHERE \"owners_tel\".\"FK_Telephone_Zk\" = (:id)");
   querry.bindValue(":id",id);
   if (!querry.exec())
       qDebug() << querry.lastError();

   while (querry.next())
   {
       for (int a =0; a<vector.size(); a++)
       {
         qDebug() << vector.at(a);

           temp.prepare("SELECT DISTINCT \"owners_tel\".\"Telephone_id\""
                          "FROM  \"owners_tel\""
                          "WHERE \"owners_tel\".\"FK_Telephone_Zk\" = (:id)");
           temp.bindValue(":id", vector.at(a));
           if (!temp.exec())
               qDebug() << temp.lastError();

           while (temp.next())
           {
    /**/      temp_2.prepare("SELECT DISTINCT \"contacts\".\"cl_info\", \"contacts\".\"cl_telephone\" "
                           "FROM \"contacts\", \"owners_tel\""
                           "WHERE \"contacts\".\"cl_telephone\" = (:an_tel)"
                           "AND \"contacts\".\"FK_Cl_telephone\" = (:id)");

               temp_2.bindValue(":an_tel",querry.value(0).toString());

               temp_2.bindValue(":id",temp.value(0).toInt());

               if (!temp_2.exec())
                   qDebug() << temp_2.lastError();


               while (temp_2.next())
               {
                   if (Tel_num != querry.value(0).toString())
                {
                    Tel_num =  querry.value(0).toString();

                   analysis_res +="Номер телефона "+ Tel_num+", принадлежащий владельцу запиной книжки: ";

                   analysis_res += Lastname+" "+Name+" "+Midname+" обнаружен в ";
                   ///////////////////ОПРЕДЕЛЯЮ КОЛ-ВО ЗАПИСЕЙ////////////////////
                   temp_3.prepare("SELECT COUNT( DISTINCT \"contacts\".\"FK_Cl_telephone\")"
                                "FROM \"owners_tel\", \"contacts\" "
                                "WHERE \"owners_tel\".\"FK_Telephone_Zk\" = (:id)"
                               " AND "
                                "\"contacts\".\"cl_telephone\" = (:tel_num) ");
                   temp_3.bindValue(":id",id);
                   temp_3.bindValue(":tel_num",Tel_num);

                   if (!temp_3.exec())
                       qDebug() << temp_3.lastError();

                   while (temp_3.next())
                   {
                       if(temp_3.value(0).toInt() == 1)
                           analysis_res+="записной книжке: \r\n \r\n";
                       else
                           analysis_res+="записных книжках: \r\n \r\n";
                   }
                   temp_3.clear();
                 }
                   temp_3.prepare("SELECT "
 /*0*/          "regexp_replace(\"zk\".\"Lastname\", '\\s+$', ''), "
                "regexp_replace(\"zk\".\"Name\", '\\s+$', ''), "
                "regexp_replace(\"zk\".\"Mid_name\", '\\s+$', ''),"
                "regexp_replace(\"zk\".\"Birth_date\", '\\s+$', ''),"
                ""
 /*4*/          "regexp_replace(\"zk\".\"Reg_city\", '\\s+$', ''),"
                "regexp_replace(\"zk\".\"Reg_street\", '\\s+$', ''),"
                "regexp_replace(\"zk\".\"Reg_home\", '\\s+$', ''),"
                "regexp_replace(\"zk\".\"Reg_corp\", '\\s+$', ''),"
                "regexp_replace(\"zk\".\"Reg_flat\", '\\s+$', ''),"
                ""
/*9*/           "regexp_replace(\"zk\".\"Dop_info\", '\\s+$', ''),"
                "regexp_replace(\"zk\".\"Date_add\", '\\s+$', ''),"
                "regexp_replace(\"zk\".\"Check_for\", '\\s+$', '')"
                " FROM \"zk\" "
                " WHERE "
                "\"zk\".\"Zk_id\" = (:zk_id)");
                   temp_3.bindValue(":zk_id",vector.at(a));
                   if (!temp_3.exec())
                       qDebug() << temp_3.lastError();
                   while (temp_3.next())
                   {
            analysis_res += temp_3.value(0).toString()+" "+
                    temp_3.value(1).toString()+" "+
                    temp_3.value(2).toString()+", "+
                    temp_3.value(3).toString()+", адрес регистрации: "+
                    temp_3.value(4).toString()+" "+temp_3.value(5).toString()+" "+
                    temp_3.value(6).toString()+" "+temp_3.value(7).toString()+" "+
                    temp_3.value(8).toString()+", "+" телефон: "+
                    querry.value(0).toString()+ ", окраска: ???, дополнительная информация: "+
                    temp_3.value(9).toString()+", записная книжка сформирована "+
                    temp_3.value(10).toString()+" в интересах: "+
                    temp_3.value(11).toString();
            if(!temp_2.value(0).toString().isEmpty())
                analysis_res+=" c пометкой "+ temp_2.value(0).toString();
            analysis_res+="\r\n \r\n";
                   }
                   temp_3.clear();
               }

           }
       }
   }
   if(!analysis_res.isEmpty())
   {
       analysis_res.insert(0,"\t ФОРМИРОВАНИЕ ПОЛНОЙ СПРАВКИ С ПРИВЯЗКОЙ К НОМЕРАМ \r\n \r\n");
   }
}

void For_analysis::short_face_analysis_all_db(QDate dateFROM, QDate dateTO, int id)
{
    analysis_res.clear();
    int counter = 0;
    querry.prepare("SELECT \"zk\".\"Date_add\", \"zk\".\"Zk_id\""
                   "From \"zk\""
                   "WHERE \"zk\".\"Date_add\" IS NOT NULL");

    if (!querry.exec())
        qDebug() << querry.lastError();
    while (querry.next())
    {
        QDate date;
        auto list =  querry.value(0).toString().split('-');
        date.setDate(list.at(0).toInt(),list.at(1).toInt(),list.at(2).toInt());

        if(date <= dateTO && date >= dateFROM)
        {
            temp.prepare("SELECT DISTINCT \"owners_tel\".\"Telephone_num\""
                           "FROM  \"owners_tel\""
                           "WHERE \"owners_tel\".\"FK_Telephone_Zk\" = (:id)");
            temp.bindValue(":id",id);
            if (!temp.exec())
                qDebug() << temp.lastError();

            while (temp.next())
            {
                temp_2.prepare("SELECT DISTINCT \"owners_tel\".\"Telephone_id\""
                               "FROM  \"owners_tel\""
                               "WHERE \"owners_tel\".\"FK_Telephone_Zk\" = (:id)");
                temp_2.bindValue(":id", querry.value(1));
                if (!temp_2.exec())
                    qDebug() << temp_2.lastError();

                while (temp_2.next())
                {
                    temp_3.prepare("SELECT DISTINCT \"contacts\".\"cl_info\", \"contacts\".\"cl_telephone\" "
                                              "FROM \"contacts\", \"owners_tel\""
                                              "WHERE \"contacts\".\"cl_telephone\" = (:an_tel)"
                                              "AND \"contacts\".\"FK_Cl_telephone\" = (:id)");

                                  temp_3.bindValue(":an_tel",temp.value(0).toString());

                                  temp_3.bindValue(":id",temp_2.value(0).toInt());

                                  if (!temp_3.exec())
                                      qDebug() << temp_3.lastError();

                                  while (temp_3.next())
                                  {
                                     QSqlQuery temp_4;
                                     counter++;
                                     temp_4.prepare("SELECT "
                                                   "regexp_replace(\"zk\".\"Lastname\", '\\s+$', ''), "
                                                   "regexp_replace(\"zk\".\"Name\", '\\s+$', ''), "
                                                   "regexp_replace(\"zk\".\"Mid_name\", '\\s+$', ''),"
                                                   "regexp_replace(\"zk\".\"Birth_date\", '\\s+$', ''),"
                                                   "regexp_replace(\"zk\".\"Date_add\", '\\s+$', ''),"
                                                   "regexp_replace(\"zk\".\"Check_for\", '\\s+$', '')"
                                                  " FROM \"zk\" "
                                                  " WHERE "
                                                   "\"zk\".\"Zk_id\" = (:zk_id)");
                                    temp_4.bindValue(":zk_id",querry.value(1));

                                    if (!temp_4.exec())
                                        qDebug() << temp_4.lastError();
                                    while (temp_4.next())
                                            {
                                        analysis_res += temp_4.value(0).toString()+" "+
                                                temp_4.value(1).toString()+" "+
                                                temp_4.value(2).toString()+", "+
                                                temp_4.value(3).toString()+", окраска: ???, записная книжка сформирована "+
                                                temp_4.value(4).toString()+" в интересах: "+
                                                temp_4.value(5).toString()+"  \r\n \r\n";
                                            }
                                    temp_4.clear();
                                  }
                }
            }
        }
    }
if(counter > 0)
 {
   QString for_insert;
   For_analysis::get_L_N_M(id);
        for_insert = "\t ФОРМИРОВАНИЕ КРАТКОЙ СПРАВКИ С ПРИВЯЗКОЙ К ЛИЦАМ \r\n \r\n"
                                              "Владелец записной книжки ";
                               for_insert += Lastname+" "+Name+" "+Midname+" "
                                               " возможно знаком со ";
                               if(counter==1)
                                   for_insert += " следующим лицом: \r\n \r\n";
                               else
                                   for_insert += "следующими лицами: \r\n \r\n";
      analysis_res.insert(0,for_insert);
}
}

void For_analysis::short_tel_analysis_all_db(QDate dateFROM, QDate dateTO, int id)
{
    analysis_res.clear();
    For_analysis::get_L_N_M(id);

    querry.prepare("SELECT \"zk\".\"Date_add\", \"zk\".\"Zk_id\""
                   "From \"zk\""
                   "WHERE \"zk\".\"Date_add\" IS NOT NULL");

    if (!querry.exec())
        qDebug() << querry.lastError();
    while (querry.next())
    {
        QDate date;
        auto list =  querry.value(0).toString().split('-');
        date.setDate(list.at(0).toInt(),list.at(1).toInt(),list.at(2).toInt());

        if(date <= dateTO && date >= dateFROM)
        {
            temp.prepare("SELECT DISTINCT \"owners_tel\".\"Telephone_num\""
                           "FROM  \"owners_tel\""
                           "WHERE \"owners_tel\".\"FK_Telephone_Zk\" = (:id)");
            temp.bindValue(":id",id);
            if (!temp.exec())
                qDebug() << temp.lastError();

            while (temp.next())
            {
                temp_2.prepare("SELECT DISTINCT \"owners_tel\".\"Telephone_id\""
                               "FROM  \"owners_tel\""
                               "WHERE \"owners_tel\".\"FK_Telephone_Zk\" = (:id)");
                temp_2.bindValue(":id", querry.value(1));
                if (!temp_2.exec())
                    qDebug() << temp_2.lastError();

                while (temp_2.next())
                {
                    temp_3.prepare("SELECT DISTINCT \"contacts\".\"cl_info\", \"contacts\".\"cl_telephone\" "
                                               "FROM \"contacts\", \"owners_tel\""
                                               "WHERE \"contacts\".\"cl_telephone\" = (:an_tel)"
                                               "AND \"contacts\".\"FK_Cl_telephone\" = (:id)");

                                   temp_3.bindValue(":an_tel",temp.value(0).toString());

                                   temp_3.bindValue(":id",temp_2.value(0).toInt());

                                   if (!temp_3.exec())
                                       qDebug() << temp_3.lastError();


                                   while (temp_3.next())
                                   {
                                       QSqlQuery temp_4;
                                       if (Tel_num != temp.value(0).toString())
                                    {
                                        Tel_num =  temp.value(0).toString();

                                       analysis_res +="Номер телефона "+ Tel_num+", принадлежащий владельцу запиной книжки: ";

                                       analysis_res += Lastname+" "+Name+" "+Midname+" обнаружен в ";
                                       ///////////////////ОПРЕДЕЛЯЮ КОЛ-ВО ЗАПИСЕЙ////////////////////
                                       temp_4.prepare("SELECT COUNT( DISTINCT \"contacts\".\"FK_Cl_telephone\")"
                                                    "FROM \"owners_tel\", \"contacts\" "
                                                    "WHERE \"owners_tel\".\"FK_Telephone_Zk\" = (:id)"
                                                   " AND "
                                                    "\"contacts\".\"cl_telephone\" = (:tel_num) ");
                                       temp_4.bindValue(":id",id);
                                       temp_4.bindValue(":tel_num",Tel_num);

                                       if (!temp_4.exec())
                                           qDebug() << temp_4.lastError();

                                       while (temp_4.next())
                                       {
                                           if(temp_4.value(0).toInt() == 1)
                                               analysis_res+="записной книжке: \r\n \r\n";
                                           else
                                               analysis_res+="записных книжках: \r\n \r\n";
                                       }
                                       temp_4.clear();
                                     }
                                       temp_4.prepare("SELECT "
                                                      "regexp_replace(\"zk\".\"Date_add\", '\\s+$', ''),"
                                                      "regexp_replace(\"zk\".\"Lastname\", '\\s+$', ''),"
                                                      "regexp_replace(\"zk\".\"Name\", '\\s+$', ''),"
                                                      "regexp_replace(\"zk\".\"Mid_name\", '\\s+$', ''),"
                                                      "regexp_replace(\"zk\".\"Birth_date\", '\\s+$', ''),"
                                                      "regexp_replace(\"zk\".\"Check_for\", '\\s+$', '')"
                                                      "FROM \"zk\""
                                                     " WHERE \"zk\".\"Zk_id\" = (:temp_2_val)");
                                       temp_4.bindValue(":temp_2_val",querry.value(1));
                                       if (!temp_4.exec())
                                           qDebug() << temp_4.lastError();
                                       while (temp_4.next())
                                       {
                                           analysis_res+="сформированной "+temp_4.value(0).toString()+
                                                   " в отношении: "+temp_4.value(1).toString()+" "+
                                                   temp_4.value(2).toString()+" "+
                                                   temp_4.value(3).toString()+", "+temp_4.value(4).toString()+" по окраске: ???, в интересах "+
                                                   temp_4.value(5).toString()+ " ";
                                           if (!temp_3.value(0).toString().isEmpty())
                                               analysis_res += "с пометкой "+temp_3.value(0).toString()+"\r\n \r\n";
                                           else
                                               analysis_res +="\r\n \r\n";
                                       }
                                       temp_4.clear();
                }
            }
        }
    }
    }
}

void For_analysis::long_face_analysis_all_db(QDate dateFROM, QDate dateTO, int id)
{
    int counter = 0;
    analysis_res.clear();
    For_analysis::get_L_N_M(id);

    querry.prepare("SELECT \"zk\".\"Date_add\", \"zk\".\"Zk_id\""
                   "From \"zk\""
                   "WHERE \"zk\".\"Date_add\" IS NOT NULL");

    if (!querry.exec())
        qDebug() << querry.lastError();
    while (querry.next())
    {
        QDate date;
        auto list =  querry.value(0).toString().split('-');
        date.setDate(list.at(0).toInt(),list.at(1).toInt(),list.at(2).toInt());

        if(date <= dateTO && date >= dateFROM)
        {
            temp.prepare("SELECT DISTINCT \"owners_tel\".\"Telephone_num\""
                           "FROM  \"owners_tel\""
                           "WHERE \"owners_tel\".\"FK_Telephone_Zk\" = (:id)");
            temp.bindValue(":id",id);
            if (!temp.exec())
                qDebug() << temp.lastError();

            while (temp.next())
            {
                temp_2.prepare("SELECT DISTINCT \"owners_tel\".\"Telephone_id\",\"owners_tel\".\"Telephone_num\""
                               "FROM  \"owners_tel\""
                               "WHERE \"owners_tel\".\"FK_Telephone_Zk\" = (:id)");
                temp_2.bindValue(":id", querry.value(1));
                if (!temp_2.exec())
                    qDebug() << temp_2.lastError();

                while (temp_2.next())
                {
                    temp_3.prepare("SELECT DISTINCT \"contacts\".\"cl_info\", \"contacts\".\"cl_telephone\" "
                                              "FROM \"contacts\", \"owners_tel\""
                                              "WHERE \"contacts\".\"cl_telephone\" = (:an_tel)"
                                              "AND \"contacts\".\"FK_Cl_telephone\" = (:id)");

                                  temp_3.bindValue(":an_tel",temp.value(0).toString());

                                  temp_3.bindValue(":id",temp_2.value(0).toInt());

                                  if (!temp_3.exec())
                                      qDebug() << temp_3.lastError();

                                  while (temp_3.next())
                                  {
                                      QSqlQuery temp_4;
                                      counter++;
                                            temp_4.prepare("SELECT "
               /*0*/          "regexp_replace(\"zk\".\"Lastname\", '\\s+$', ''), "
                              "regexp_replace(\"zk\".\"Name\", '\\s+$', ''), "
                              "regexp_replace(\"zk\".\"Mid_name\", '\\s+$', ''),"
                              "regexp_replace(\"zk\".\"Birth_date\", '\\s+$', ''),"
                              ""
               /*4*/          "regexp_replace(\"zk\".\"Reg_city\", '\\s+$', ''),"
                              "regexp_replace(\"zk\".\"Reg_street\", '\\s+$', ''),"
                              "regexp_replace(\"zk\".\"Reg_home\", '\\s+$', ''),"
                              "regexp_replace(\"zk\".\"Reg_corp\", '\\s+$', ''),"
                              "regexp_replace(\"zk\".\"Reg_flat\", '\\s+$', ''),"
                               ""
               /*9*/          "regexp_replace(\"zk\".\"Dop_info\", '\\s+$', ''),"
                              "regexp_replace(\"zk\".\"Date_add\", '\\s+$', ''),"
                              "regexp_replace(\"zk\".\"Check_for\", '\\s+$', '')"
                              " FROM \"zk\" "
                              " WHERE "
                              "\"zk\".\"Zk_id\" = (:zk_id)");
                                           temp_4.bindValue(":zk_id",querry.value(1));
                                         QString str = QString::number(querry.value(1).toInt());
                                           if (!temp_4.exec())
                                               qDebug() << temp_4.lastError();
                                           while (temp_4.next())
                                                   {
                                               analysis_res += "Номер ЗК - "+str+" "+temp_4.value(0).toString()+" "+
                                                       temp_4.value(1).toString()+" "+
                                                       temp_4.value(2).toString()+", "+
                                                       temp_4.value(3).toString()+", адрес регистрации: "+
                                                       temp_4.value(4).toString()+" "+temp_4.value(5).toString()+" "+
                                                       temp_4.value(6).toString()+" "+temp_4.value(7).toString()+" "+
                                                       temp_4.value(8).toString()+", "+" телефон: "+
                                                       temp_2.value(1).toString()+ ", окраска: ???, дополнительная информация: "+
                                                       temp_4.value(9).toString()+", записная книжка сформирована "+
                                                       temp_4.value(10).toString()+" в интересах: "+
                                                       temp_4.value(11).toString()+"  \r\n \t Вывод о знакомстве сделан на основании того, что: \r\n";

                                                       analysis_res+="Номер телефона "+temp.value(0).toString()+", принадлежащий "+Lastname+" "+Name+" "+Midname+" "+
                                                         ", обнаружен в записной книжке, владельцем которой является "+
                                                       temp_4.value(0).toString()+" "+temp_4.value(1).toString()+" "+ temp_4.value(2).toString()+" ";
                                                                if (!temp_3.value(0).toString().isEmpty())
                                                                    analysis_res += "с пометкой "+temp_3.value(0).toString()+"\r\n \r\n";
                                                                else
                                                                    analysis_res +="\r\n \r\n";
                                                    }
                                   temp_4.clear();
                                  }
                }
            }
        }
    }
         //////////////-------ВТОРОЙ ВАРИАНТ ЗНАКОМСТВА-------/////////////////
        querry.clear();
        temp.clear();
        temp_2.clear();
        temp_3.clear();

        querry.prepare("SELECT \"zk\".\"Date_add\", \"zk\".\"Zk_id\""
                       "From \"zk\""
                       "WHERE \"zk\".\"Date_add\" IS NOT NULL");

        if (!querry.exec())
            qDebug() << querry.lastError();
        while (querry.next())
        {
            temp.prepare("SELECT"
                           "\"owners_tel\".\"Telephone_id\" "
                          " FROM"
                          " \"owners_tel\" "
                          " WHERE"
                          " \"owners_tel\".\"FK_Telephone_Zk\" = (:id)");
            temp.bindValue(":id", id);

            if (!temp.exec())
                qDebug() << temp.lastError();
            while (temp.next())
            {
                temp_2.prepare("SELECT"
                             "\"owners_tel\".\"FK_Telephone_Zk\", "
                             "\"owners_tel\".\"Telephone_num\", "
                             "regexp_replace(\"contacts\".\"cl_info\", '\\s+$', '')"
                            " FROM"
                           " \"contacts\",\"owners_tel\""
                           "  WHERE "
                           "  \"contacts\".\"cl_telephone\" = \"owners_tel\".\"Telephone_num\" "
                           "  AND "
                           "  \"contacts\".\"FK_Cl_telephone\" = (:fk)");
                temp_2.bindValue(":fk", temp.value(0));
                if (!temp_2.exec())
                    qDebug() << temp_2.lastError();
                while (temp_2.next())
                {
                    if(querry.value(1) == temp_2.value(0).toInt())
                    {
                        temp_3.prepare("SELECT "
                        /*0*/          "regexp_replace(\"zk\".\"Lastname\", '\\s+$', ''), "
                                       "regexp_replace(\"zk\".\"Name\", '\\s+$', ''), "
                                       "regexp_replace(\"zk\".\"Mid_name\", '\\s+$', ''),"
                                       "regexp_replace(\"zk\".\"Birth_date\", '\\s+$', ''),"
                                       ""
                        /*4*/          "regexp_replace(\"zk\".\"Reg_city\", '\\s+$', ''),"
                                       "regexp_replace(\"zk\".\"Reg_street\", '\\s+$', ''),"
                                       "regexp_replace(\"zk\".\"Reg_home\", '\\s+$', ''),"
                                       "regexp_replace(\"zk\".\"Reg_corp\", '\\s+$', ''),"
                                       "regexp_replace(\"zk\".\"Reg_flat\", '\\s+$', ''),"
                                       ""
                        /*9*/          "regexp_replace(\"zk\".\"Dop_info\", '\\s+$', ''),"
                                       "regexp_replace(\"zk\".\"Date_add\", '\\s+$', ''),"
                                       "regexp_replace(\"zk\".\"Check_for\", '\\s+$', '')"
                                      " FROM \"zk\" "
                                      " WHERE "
                                       "\"zk\".\"Zk_id\" = (:zk_id)");
                        temp_3.bindValue(":zk_id",temp_2.value(0).toInt());
                        if (!temp_3.exec())
                            qDebug() << temp_3.lastError();

                        while (temp_3.next())
                                {
                            analysis_res += temp_3.value(0).toString()+" "+
                                    temp_3.value(1).toString()+" "+
                                    temp_3.value(2).toString()+", "+
                                    temp_3.value(3).toString()+", адрес регистрации: "+
                                    temp_3.value(4).toString()+" "+temp_3.value(5).toString()+" "+
                                    temp_3.value(6).toString()+" "+temp_3.value(7).toString()+" "+
                                    temp_3.value(8).toString()+", "+" телефон: "+
                                    temp_2.value(1).toString()+ ", окраска: ???, дополнительная информация: "+
                                    temp_3.value(9).toString()+", записная книжка сформирована "+
                                    temp_3.value(10).toString()+" в интересах: "+
                                    temp_3.value(11).toString()+"  \r\n \t Вывод о знакомстве сделан на основании того, что: \r\n"+
                                "Номер телефона "+temp_2.value(1).toString()+", ";

                            if(!temp_2.value(2).toString().isEmpty())
                                analysis_res+=" с пометкой "+temp_2.value(2).toString() + " ";
                            analysis_res+=" в записной книжке, владельцем которой является "
                                    +Lastname+" "+Name+" "+Midname+" обнаружен, как принадлежащий владельцу записной книжки: "+
                                    temp_3.value(0).toString()+" "+
                                    temp_3.value(1).toString()+" "+
                                    temp_3.value(2).toString()+" \r\n";
                                }
                   }
                }
            }
        }
    if(!analysis_res.isEmpty())
    {
        QString temp_for_add;
        temp_for_add = "\t ФОРМИРОВАНИЕ ПОЛНОЙ СПРАВКИ С ПРИВЯЗКОЙ К ЛИЦАМ \r\n \r\n"
                       "Владелец записной книжки ";
            ///////////////////////////////////////

            temp_for_add += Lastname+" "+Name+" "+Midname+" "
                            " возможно знаком со ";
            For_analysis::get_L_N_M(id);
            ///////////////////////////////////////
            temp_3.prepare("SELECT COUNT(DISTINCT \"contacts\".\"FK_Cl_telephone\")"
                           "FROM \"contacts\", \"owners_tel\""
                           "WHERE \"owners_tel\".\"FK_Telephone_Zk\" = (:id) AND"
                            "\"owners_tel\".\"Telephone_num\" = \"contacts\".\"cl_telephone\"");
            temp_3.bindValue(":id", id);
            if (!temp_3.exec())
                qDebug() << temp_3.lastError();
            while (temp_3.next())
                    {
               if (temp_3.value(0).toInt() == 1)
                   temp_for_add += " следующим лицом: \r\n \r\n";
               else
                   temp_for_add += "следующими лицами: \r\n \r\n";
                    }
            temp_3.clear();

            analysis_res.insert(0,temp_for_add);
    }
}

void For_analysis::long_tel_analysis_all_db(QDate dateFROM, QDate dateTO, int id)
{
    analysis_res.clear();
    For_analysis::get_L_N_M(id);

    querry.prepare("SELECT \"zk\".\"Date_add\", \"zk\".\"Zk_id\""
                   "From \"zk\""
                   "WHERE \"zk\".\"Date_add\" IS NOT NULL");

    if (!querry.exec())
        qDebug() << querry.lastError();
    while (querry.next())
    {
        QDate date;
        auto list =  querry.value(0).toString().split('-');
        date.setDate(list.at(0).toInt(),list.at(1).toInt(),list.at(2).toInt());

        if(date <= dateTO && date >= dateFROM)
        {
            temp.prepare("SELECT DISTINCT \"owners_tel\".\"Telephone_num\""
                           "FROM  \"owners_tel\""
                           "WHERE \"owners_tel\".\"FK_Telephone_Zk\" = (:id)");
            temp.bindValue(":id",id);
            if (!temp.exec())
                qDebug() << temp.lastError();

            while (temp.next())
            {
                temp_2.prepare("SELECT DISTINCT \"owners_tel\".\"Telephone_id\", \"owners_tel\".\"Telephone_num\""
                               "FROM  \"owners_tel\""
                               "WHERE \"owners_tel\".\"FK_Telephone_Zk\" = (:id)");
                temp_2.bindValue(":id", querry.value(1));
                if (!temp_2.exec())
                    qDebug() << temp_2.lastError();

                while (temp_2.next())
                {
                    temp_3.prepare("SELECT DISTINCT \"contacts\".\"cl_info\", \"contacts\".\"cl_telephone\" "
                                              "FROM \"contacts\", \"owners_tel\""
                                              "WHERE \"contacts\".\"cl_telephone\" = (:an_tel)"
                                              "AND \"contacts\".\"FK_Cl_telephone\" = (:id)");

                                  temp_3.bindValue(":an_tel",temp.value(0).toString());

                                  temp_3.bindValue(":id",temp_2.value(0).toInt());

                                  if (!temp_3.exec())
                                      qDebug() << temp_3.lastError();


                                  while (temp_3.next())
                                  {
                                      QSqlQuery temp_4;
                                      if (Tel_num != temp.value(0).toString())
                                   {
                                       Tel_num =  temp.value(0).toString();

                                      analysis_res +="Номер телефона "+ Tel_num+", принадлежащий владельцу запиной книжки: ";

                                      analysis_res += Lastname+" "+Name+" "+Midname+" обнаружен в ";
                                      ///////////////////ОПРЕДЕЛЯЮ КОЛ-ВО ЗАПИСЕЙ////////////////////
                                      temp_4.prepare("SELECT COUNT( DISTINCT \"contacts\".\"FK_Cl_telephone\")"
                                                   "FROM \"owners_tel\", \"contacts\" "
                                                   "WHERE \"owners_tel\".\"FK_Telephone_Zk\" = (:id)"
                                                  " AND "
                                                   "\"contacts\".\"cl_telephone\" = (:tel_num) ");
                                      temp_4.bindValue(":id",id);
                                      temp_4.bindValue(":tel_num",Tel_num);

                                      if (!temp_4.exec())
                                          qDebug() << temp_4.lastError();

                                      while (temp_4.next())
                                      {
                                          if(temp_4.value(0).toInt() == 1)
                                              analysis_res+="записной книжке: \r\n \r\n";
                                          else
                                              analysis_res+="записных книжках: \r\n \r\n";
                                      }
                                      temp_4.clear();
                                    }
                                      temp_4.prepare("SELECT "
                    /*0*/          "regexp_replace(\"zk\".\"Lastname\", '\\s+$', ''), "
                                   "regexp_replace(\"zk\".\"Name\", '\\s+$', ''), "
                                   "regexp_replace(\"zk\".\"Mid_name\", '\\s+$', ''),"
                                   "regexp_replace(\"zk\".\"Birth_date\", '\\s+$', ''),"
                                   ""
                    /*4*/          "regexp_replace(\"zk\".\"Reg_city\", '\\s+$', ''),"
                                   "regexp_replace(\"zk\".\"Reg_street\", '\\s+$', ''),"
                                   "regexp_replace(\"zk\".\"Reg_home\", '\\s+$', ''),"
                                   "regexp_replace(\"zk\".\"Reg_corp\", '\\s+$', ''),"
                                   "regexp_replace(\"zk\".\"Reg_flat\", '\\s+$', ''),"
                                   ""
                   /*9*/           "regexp_replace(\"zk\".\"Dop_info\", '\\s+$', ''),"
                                   "regexp_replace(\"zk\".\"Date_add\", '\\s+$', ''),"
                                   "regexp_replace(\"zk\".\"Check_for\", '\\s+$', '')"
                                   " FROM \"zk\" "
                                   " WHERE "
                                   "\"zk\".\"Zk_id\" = (:zk_id)");
                                      temp_4.bindValue(":zk_id",querry.value(1));
                                      if (!temp_4.exec())
                                          qDebug() << temp_4.lastError();
                                      while (temp_4.next())
                                      {
                               analysis_res += temp_4.value(0).toString()+" "+
                                       temp_4.value(1).toString()+" "+
                                       temp_4.value(2).toString()+", "+
                                       temp_4.value(3).toString()+", адрес регистрации: "+
                                       temp_4.value(4).toString()+" "+temp_4.value(5).toString()+" "+
                                       temp_4.value(6).toString()+" "+temp_4.value(7).toString()+" "+
                                       temp_4.value(8).toString()+", "+" телефон: "+
                                       temp_2.value(1).toString()+ ", окраска: ???, дополнительная информация: "+
                                       temp_4.value(9).toString()+", записная книжка сформирована "+
                                       temp_4.value(10).toString()+" в интересах: "+
                                       temp_4.value(11).toString();
                               if(!temp_3.value(0).toString().isEmpty())
                                   analysis_res+=" c пометкой "+ temp_3.value(0).toString();
                               analysis_res+="\r\n \r\n";
                                      }
                                      temp_4.clear();
                                  }
                }
            }
        }
    }
    if(!analysis_res.isEmpty())
    {
        analysis_res.insert(0,"\t ФОРМИРОВАНИЕ ПОЛНОЙ СПРАВКИ С ПРИВЯЗКОЙ К НОМЕРАМ \r\n \r\n");
    }
}

void For_analysis::get_L_N_M(int id)
{
    querry.prepare("SELECT "
                   "regexp_replace(\"zk\".\"Lastname\", '\\s+$', ''), "
                   "regexp_replace(\"zk\".\"Name\", '\\s+$', ''), "
                   "regexp_replace(\"zk\".\"Mid_name\", '\\s+$', '')"
                   "FROM \"zk\""
                   "WHERE \"zk\".\"Zk_id\"= (:id)");
    querry.bindValue(":id", id);

    if (!querry.exec())
        qDebug() << querry.lastError();
    while (querry.next())
    {
        Lastname = querry.value(0).toString();
        qDebug() <<Lastname ;
        Name = querry.value(1).toString();
        qDebug() <<Name ;
        Midname = querry.value(2).toString();
        qDebug() <<Midname ;
    }
    querry.clear();
}
