#include "for_analysis.h"

For_analysis::For_analysis()
{
    list = new List_master(Analysis);
}

QList<Crud *> *For_analysis::get_crud(Crud *cr, QString added_SQL)
{
    db_connection *db = db_connection::instance();
    QSqlQuery temp(db->db());
    QSqlQuery querry(db->db());
    analysis_res.clear();

    QList<Crud*> *crudlist = new QList <Crud*>; //Список для результатов анализа

    ///Пробегаюсь по телефонным номерам
    for (int a =0; a<cr->owt()->size(); a++)
    {
        QString tempSQL = "SELECT  DISTINCT owners_tel.fk_telephone_zk, contacts.cl_info, owners_tel.telephone_num "
                          " FROM  contacts, owners_tel, zk "
                          " WHERE contacts.cl_telephone = (:tel_num) "
                          " AND contacts.fk_cl_telephone = owners_tel.telephone_id"
                          " AND owners_tel.fk_telephone_zk = zk.zk_id";

        if(!added_SQL.isEmpty()) //Дополнительные условия запроса (ограничения по дате и ЗК)
            tempSQL+=added_SQL;

        querry.prepare(tempSQL);
        querry.bindValue(":tel_num", cr->owt()->at(a)->tel_num);

        if (!querry.exec())
            qDebug() << querry.lastError();

        ///Возвращаю id ЗК
        while (querry.next())
        {
            Crud *temp_crud = Crud::id_zk_search(querry.value(0).toInt());//Собираю информацию о владельце ЗК
 ///Беру его телефон с id = 1 (Значит телефон анализируемого обнаружен в контактах этого человека)
            Owners_tel *temp_ot = new Owners_tel(querry.value(2).toString(),1,temp_crud->zk_id);
            Contacts *temp_cont = new Contacts(1,cr->owt()->at(a)->tel_num,querry.value(1).toString(),temp_ot->tel_id);
            ///Собираем воедино
            temp_ot->cont()->append(temp_cont);
            temp_crud->owt()->append(temp_ot);
            crudlist->append(temp_crud);
        }
    ///Таким образом собрал людей, у которых в КОНТАКТАХ лежит номер анализируемого человека

        ////////ИЗМЕНИТЬ
        tempSQL.clear();
       tempSQL += " SELECT  DISTINCT  owners_tel.fk_telephone_zk, owners_tel.telephone_num, contacts.cl_info "
                  " FROM  contacts, owners_tel"
                  " WHERE contacts.fk_cl_telephone = (:tel_id)"
                  " AND  contacts.cl_telephone = owners_tel.telephone_num";

       if(!added_SQL.isEmpty()) //Дополнительные условия запроса (ограничения по дате и ЗК)
           tempSQL+=added_SQL;


            querry.prepare(tempSQL);
            querry.bindValue(":tel_id", cr->owt()->at(a)->tel_id);

            if (!querry.exec())
                qDebug() << querry.lastError();

            ///Возвращаю id ЗК
            while (querry.next())
            {
              Crud *temp_crud = Crud::id_zk_search(querry.value(0).toInt());//Собираю информацию о владельце ЗК
 ///Беру его телефон с id = 2 (Значит что в контактах анализируемого обнаружен этот человек)
            Owners_tel *temp_ot = new Owners_tel(cr->owt()->at(a)->tel_num, 2, temp_crud->zk_id);
            Contacts *temp_cont = new Contacts( 2, querry.value(1).toString(), querry.value(2).toString(), temp_ot->tel_id);
            ///Собираем воедино
            temp_ot->cont()->append(temp_cont);
            temp_crud->owt()->append(temp_ot);
            crudlist->append(temp_crud);
            }
    }
///Имею список crudlist - с людьми, телефонами и контактами. если номер с id 1 - первый вариант
/// знакомства. Если номер с id 2 - второй вариант знакомства (в таком случае телефон
/// при владельце = телефону в контактах, мб это не правильно). Возможно, придется удалять одинаковые
/// записи в листе
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
        if(crudlist->at(i)->owt()->at(0)->tel_id == 1)
        {
            analysis_res+="Номер телефона "+crudlist->at(i)->owt()->at(0)->cont()->at(0)->contact_tel_num+", принадлежащий " +
                    cr->lastname+" "+cr->name+" "+cr->mid_name+", обнаружен в записной книжке, владельцем которой является "+
                    crudlist->at(i)->lastname +" "+crudlist->at(i)->name +" "+crudlist->at(i)->mid_name +" ";
            if(!crudlist->at(i)->owt()->at(0)->cont()->at(0)->mark.isEmpty())
                analysis_res +="с пометкой "+crudlist->at(i)->owt()->at(0)->cont()->at(0)->mark + "\r\n \r\n";
            else
                analysis_res+="\r\n \r\n";
        }
        if(crudlist->at(i)->owt()->at(0)->tel_id == 2)
        {
          analysis_res+="Номер телефона "+crudlist->at(i)->owt()->at(0)->cont()->at(0)->contact_tel_num;
            if(!crudlist->at(i)->owt()->at(0)->cont()->at(0)->mark.isEmpty())
                analysis_res +=" с пометкой "+crudlist->at(i)->owt()->at(0)->cont()->at(0)->mark + " ";
            else
                analysis_res+=" ";
            analysis_res+=" в записной книжке, владельцем которой является "+
            cr->lastname+" "+cr->name+" "+cr->mid_name+", обнаружен, как принадлежащий владельцу записной книжки: "+
            crudlist->at(i)->lastname +" "+crudlist->at(i)->name +" "+crudlist->at(i)->mid_name+"\r\n \r\n";
        }
     }
    }
}

void For_analysis::long_tel_analysis(Crud *cr, QList<Crud*> *crudlist)
{
 if(!crudlist->isEmpty())
    {
      QList<Crud*> *temp_crudlist = new QList<Crud*>;
      analysis_res +="\t ФОРМИРОВАНИЕ ПОЛНОЙ СПРАВКИ С ПРИВЯЗКОЙ К ТЕЛЕФОНАМ \r\n \r\n";

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
