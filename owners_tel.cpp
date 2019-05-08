#include "owners_tel.h"

void Owners_tel::append_telephones(QString tel)
{
    telephones.append(tel);
}

void Owners_tel::return_values() const
{
    if (telephones.size() > 0)
    {
        for (int i=0; i < telephones.size(); i++)
        {
            qDebug() << telephones.value(i);
        }
    }
    else {
        qDebug() << "list is empty";
    }
}

void Owners_tel::add_numbers(int zk_id)
{
    for (int i =0; i< telephones.size(); i++)
    {
        querry.prepare("INSERT INTO \"owners_tel\""
                       "(\"Telephone_num\", \"FK_Telephone_Zk\")"
                       "VALUES((:telephone),(:zk_id))");
        querry.bindValue(":telephone", telephones.value(i));
        querry.bindValue(":zk_id", zk_id);
        if (!querry.exec())
        {
                qDebug() << "querry.lastError()";
        }
        querry.clear();
    }
}

void Owners_tel::recieve_contacts(int tel_id)
{
    querry.prepare("SELECT \"contacts\".\"cl_telephone\",\"contacts\".\"cl_info\""
                   "FROM \"contacts\""
                   "WHERE \"contacts\".\"FK_Cl_telephone\" = (:id) ");
    querry.bindValue(":id", tel_id);
        if (!querry.exec())
            qDebug() << querry.lastError();
    model->setQuery(querry);
    querry.clear();
}

//Возвращает случайное число из диапазона int если нет записи

void Owners_tel::recieve_tel_id(QString telephone_num)
{
    querry.prepare("SELECT \"owners_tel\".\"Telephone_id\""
                   "FROM \"owners_tel\""
                   "WHERE \"owners_tel\".\"Telephone_num\" = (:tel)");

    querry.bindValue(":tel", telephone_num);
    if (!querry.exec())
        qDebug() << querry.lastError();

       if (querry.next())
        {
            qDebug() << querry.value(0);
           tel_id = querry.value(0).toInt();

        qDebug() << querry.executedQuery();
        qDebug() << tel_id;
        }
     else {
         tel_id = 0;
     }
}

void Owners_tel::del_tel()
{
    querry.prepare("DELETE FROM \"owners_tel\""
    "WHERE \"owners_tel\".\"Telephone_id\" = (:tel_id)");
    querry.bindValue(":tel_id",tel_id);
    if (!querry.exec())
        qDebug() << querry.lastError();
}

void Owners_tel::add_telephones()
{
    querry.prepare("SELECT \"owners_tel\".\"Telephone_id\""
                  "FROM \"owners_tel\""
                  "WHERE \"owners_tel\".\"FK_Telephone_Zk\" IS NULL");
    if (!querry.exec())
        qDebug() << querry.lastError();
    qDebug() << querry.executedQuery();
    while(querry.next())
    {
        temp.prepare("UPDATE \"owners_tel\""
                     "SET \"FK_Telephone_Zk\" = (:id)"
                     "WHERE \"owners_tel\".\"Telephone_id\" = (:tel_id)");
        temp.bindValue(":id", zk_id);
        temp.bindValue(":tel_id", querry.value(0).toInt());
        if (!temp.exec())
            qDebug() << temp.lastError();
    qDebug() << temp.executedQuery();
    }
}

void Owners_tel::get_filter_for_add()
{

    //Беру все новые поля
    querry.prepare("SELECT \"owners_tel\".\"Telephone_id\""
                  "FROM \"owners_tel\""
                  "WHERE \"owners_tel\".\"FK_Telephone_Zk\" IS NULL");
    if (!querry.exec())
        qDebug() << querry.lastError();
    qDebug() << querry.executedQuery();
    while(querry.next())
    {
        //Добавляю к новым полям ФК новой зк
        temp.prepare("UPDATE \"owners_tel\""
                     "SET \"FK_Telephone_Zk\" = (:id)"
                     "WHERE \"owners_tel\".\"Telephone_id\" = (:tel_id)");
        temp.bindValue(":id", new_zk_id);
        temp.bindValue(":tel_id", querry.value(0).toInt());
        if (!temp.exec())
            qDebug() << temp.lastError();
    qDebug() << temp.executedQuery();
    }
}

void Owners_tel::get_new_zk_id() //для фильтра
{
    //Определяю Ид новой записной книги
    querry.prepare("SELECT MAX(\"zk\".\"Zk_id\")"
                   "FROM \"zk\"");
    if (!querry.exec())
        qDebug() << querry.lastError();
    while(querry.next())
    {
        new_zk_id = querry.value(0).toInt();
    }

    querry.clear();

}

void Owners_tel::del_where_fk_null()
{
    querry.prepare("DELETE FROM \"owners_tel\""
    "WHERE \"owners_tel\".\"FK_Telephone_Zk\" IS NULL");
    if (!querry.exec())
        qDebug() << querry.lastError();
}

void Owners_tel::check_for_null()
{
//    querry.prepare("SELECT \"owners_tel\".\"Telephone_id\""
//                   "FROM \"owners_tel\""
//                   "WHERE \"Telephone_num\" = (:tel)");
//    querry.bindValue(":tel",);
//    if (!querry.exec())
//        qDebug() << querry.lastError();
//    while(querry.next())
//    {
//        null_counter = querry.value(0).toInt();
//    }
//    querry.clear();
}

void Owners_tel::zk_search()
{
    search_is_ready = false;
    model_is_ready = false; //Зануляем флаги

    msgbx.setText("Результат поиска");
    msgbx.setGeometry(960,510, 180,210);

    QString qry;
    if(zk_id != 0)
    {
        QString temp = QString::number(zk_id);
        qry += " AND \"zk\".\"Zk_id\" = "+temp+" ";
    }

    if (!tel_num.isEmpty())
    {
       qry += "AND \"owners_tel\".\"Telephone_num\" = ('"+tel_num+"')"
               "AND \"owners_tel\".\"FK_Telephone_Zk\" = \"zk\".\"Zk_id\" ";
    }

    if (!lastname.isEmpty())
    {
        qry += " AND \"zk\".\"Lastname\" = ('"+lastname+"') ";
    }
    if (!name.isEmpty())
    {
        qry += " AND \"zk\".\"Name\" = ('"+name+"')";
    }
    if (!mid_name.isEmpty())
    {
        qry += " AND \"zk\".\"Mid_name\" = ('"+mid_name+"')";
    }

    if(!birth_date.isEmpty() && birth_date!="..")
    {
      qry += "AND \"zk\".\"Birth_date\" = ('"+birth_date+"')" ;
    }

    if (!reg_city.isEmpty())
    {
        qry += " AND \"zk\".\"Reg_city\" = ('"+reg_city+"')";
    }
    if (!reg_street.isEmpty())
    {
        qry += " AND \"zk\".\"Reg_street\" =('"+reg_street+"')";
    }

    if (!reg_home.isEmpty())
    {
        qry += " AND \"zk\".\"Reg_home\" =('"+reg_home+"')";
    }

    if (!reg_corp.isEmpty())
    {
        qry += "AND \"zk\".\"Reg_corp\" = ('"+reg_corp+"')";
    }
    if (!reg_flat.isEmpty())
    {
        qry += " AND \"zk\".\"Reg_flat\" = ('"+reg_flat+"')";
    }

    if(!date_add.isEmpty() && date_add != "--")
    {
        qry += " AND \"zk\".\"Date_add\" = ('"+date_add+"')";
    }

    if(!period_from.isEmpty() && !period_to.isEmpty() && Date_From < Date_To)
    {
        QString tempp;
        querry.prepare("SELECT \"zk\".\"Date_add\", \"zk\".\"Zk_id\""
                       "FROM \"zk\""
                       "WHERE \"zk\".\"Date_add\" IS NOT NULL");

        if (!querry.exec())
            qDebug() << querry.lastError();

        while (querry.next())
        {
            QDate date;
            auto list =  querry.value(0).toString().split('-');
            date.setDate(list.at(0).toInt(),list.at(1).toInt(),list.at(2).toInt());
            if(date <= Date_To && date >= Date_From)
            {
                if(tempp.isEmpty())
                {
                    tempp+=" \"zk\".\"Zk_id\" = "+querry.value(1).toString();
                }
                else
                {
                  tempp+=" OR  \"zk\".\"Zk_id\" = "+querry.value(1).toString();
                }
            }
        }
        if (!tempp.isEmpty())
        {
            qry+="AND ("+tempp+" ) ";
        }
    }

if (!qry.isEmpty())
  {
    querry.prepare("SELECT "
                   "COUNT(DISTINCT \"zk\".\"Zk_id\")"
                   "FROM "
                   " \"zk\",\"owners_tel\" "
                   "WHERE "
                   "\"zk\".\"Zk_id\">0 " + qry);//qry - набор данных для поиска
    if (!querry.exec())
        qDebug() << querry.lastError();
    qDebug() << querry.executedQuery();
    while (querry.next())
    {
        search_result = querry.value(0).toString(); //определил кол-во результатов
    }
    querry.clear();


    if (search_result.toInt() == 0)
    {
         msgbx.setText("<font size = '8'>Совпадений не найдено</font>");
         msgbx.setStandardButtons(QMessageBox::Cancel);
         msgbx.setButtonText(QMessageBox::Cancel,"Закрыть");
    }
    else {
        msgbx.setText("<font size = '8'> Найдено " + search_result + " совпадений </font>");
        msgbx.setStandardButtons(QMessageBox::Ok | QMessageBox::Open | QMessageBox::Cancel);
        msgbx.setButtonText(QMessageBox::Ok,"ПРОСМОТР");
        msgbx.setButtonText(QMessageBox::Open,"СФОРМИРОВАТЬ СПРАВКУ");
        msgbx.setButtonText(QMessageBox::Cancel,"ОТМЕНА");
    }

    int ret = msgbx.exec();

    switch (ret) {
    case QMessageBox::Ok:
        Owners_tel::zk_search_model(qry);
        break;

    case QMessageBox::Open:
        Owners_tel::zk_search_report(qry);
        break;

    case QMessageBox::Cancel:
        break;
            }
        }
        else
    {
    Crud::select_all();
}
}

void Owners_tel::zk_search_model(QString qry)
{
    querry.prepare("SELECT  DISTINCT"        //стринг для модели
                   "\"zk\".\"Zk_id\","
                   "\"zk\".\"Lastname\", "
                   "\"zk\".\"Name\", "
                   "\"zk\".\"Mid_name\", "
                   "\"zk\".\"Birth_date\","
                    "\"zk\".\"Reg_city\","
                   "\"zk\".\"Reg_street\","
                   "\"zk\".\"Reg_home\","
                   "\"zk\".\"Reg_corp\","
                   "\"zk\".\"Reg_flat\","
                   "\"zk\".\"Check_for\", "
                   "\"zk\".\"Dop_info\","
                   "\"zk\".\"Date_add\", "
                   "\"zk\".\"Time_add\""
                   "FROM "
                   "\"zk\" ,\"owners_tel\""
                   "WHERE"
                   "\"zk\".\"Zk_id\">0 " + qry + " ORDER BY \"zk\".\"Zk_id\"");
    if (!querry.exec())
        qDebug() << querry.lastError();
     model->setQuery(querry);

    if(model->rowCount()>0)
        model_is_ready = true;

    querry.clear();
}

void Owners_tel::zk_search_report(QString qry)
{
    search_res.clear();

    querry.prepare("SELECT  DISTINCT"
                   "\"zk\".\"Zk_id\","
                    "regexp_replace(\"zk\".\"Lastname\", '\\s+$', ''), "
                    "regexp_replace(\"zk\".\"Name\", '\\s+$', ''), "
                    "regexp_replace(\"zk\".\"Mid_name\", '\\s+$', ''),"
                    "regexp_replace(\"zk\".\"Birth_date\", '\\s+$', ''),"

                   "regexp_replace(\"zk\".\"Reg_city\", '\\s+$', ''),"
                   "regexp_replace(\"zk\".\"Reg_street\", '\\s+$', ''),"
                   "regexp_replace(\"zk\".\"Reg_home\", '\\s+$', ''),"
                   "regexp_replace(\"zk\".\"Reg_corp\", '\\s+$', ''),"
                   "regexp_replace(\"zk\".\"Reg_flat\", '\\s+$', ''),"

                   "regexp_replace(\"zk\".\"Check_for\", '\\s+$', ''),"
                   "regexp_replace(\"zk\".\"Dop_info\", '\\s+$', ''),"
                   "\"zk\".\"Date_add\", "
                   "\"zk\".\"Time_add\""
                   "FROM "
                   "\"zk\" ,\"owners_tel\""
                   "WHERE"
                   "\"zk\".\"Zk_id\">0 " + qry + " ORDER BY \"zk\".\"Zk_id\"");
    if (!querry.exec())
        qDebug() << querry.lastError();
    while (querry.next())
    {
        search_res+="Записная книжка № "+querry.value(0).toString()+" \r\n Владелец: "+
                querry.value(1).toString()+" "+querry.value(2).toString()+" "+querry.value(3).toString()+
                ", "+querry.value(4).toString()+", адрес регистрации: г."+
                querry.value(5).toString()+" ул. "+querry.value(6).toString()+" д. "+
                querry.value(7).toString()+" к. "+querry.value(8).toString()+" кв. "+
                querry.value(9).toString();
        temp.prepare("SELECT \"owners_tel\".\"Telephone_num\""
                      "FROM \"owners_tel\""
                      "WHERE \"owners_tel\".\"FK_Telephone_Zk\" = (:id)");
        temp.bindValue(":id", querry.value(0).toInt());
        if (!temp.exec())
            qDebug() << temp.lastError();
        else
            search_res +=", телефон:";
        while (temp.next())
        {
         search_res +=" "+temp.value(0).toString()+", ";
        }
       search_res +="окраска: ???, дополнительная информация: "+querry.value(11).toString()+
               ", записная книжка сформирована "+querry.value(12).toString()+
               " в интересах: " +querry.value(10).toString()+"\r\n \r\n";
    }

      if(!search_res.isEmpty())
    {
       search_is_ready = true;
      }
}

void Owners_tel::check_tel_num(QString result)
{
    querry.prepare("SELECT \"owners_tel\".\"FK_Telephone_Zk\""
                   "FROM \"owners_tel\""
                   "WHERE \"owners_tel\".\"Telephone_num\" = ('" + result+"')");
   if(!querry.exec())
       qDebug() << querry.lastError();

    qDebug() << querry.executedQuery();
   while (querry.next())
   {
       msgbx.setText("Предупреждение!");
       msgbx.setText("<font = 8>Введенный вами телефон: "+result+" принадлежит владельцу ЗК № "+querry.value(0).toString()+" </font>");
       msgbx.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
       msgbx.exec();
   }
}

