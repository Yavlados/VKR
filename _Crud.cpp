#include "_Crud.h"
#include <QMessageBox>
#include <QDialogButtonBox>

#include <QMetaObject>

Crud::Crud(QString t_n): telephone_num(t_n)
{
   checkState_ = Unchecked;
}

Crud::Crud(int id): zk_id(id)
{
   checkState_ = Unchecked;
}

Crud::Crud()
{
    checkState_ = Unchecked;
}

bool Crud::selectAll(QList<Crud *> *list)
{
    if(list==nullptr)
        return false;

    qDeleteAll(*list);
    list->clear();

    if( !db_connection::instance()->db_connect() )
        return false;

    QSqlQuery temp(db_connection::instance()->db());
    temp.prepare("SELECT "
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
                 " WHERE zk.zk_id != 102" /// 102 - номер записи для служебных телефонов
                 " ORDER BY zk.zk_id");
    if (!temp.exec())
    {
        qDebug() << temp.lastError();
        return false;
    }

    while (temp.next())
    {
        Crud *cr = new Crud();
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
        cr->state = IsReaded;
        list->append(cr);
    }

    return true;
}

bool Crud::select_search(QList<Crud*> *list, QString Query)
{
    if(list==nullptr)
        return false;

    qDeleteAll(*list);
    list->clear();

    if( !db_connection::instance()->db_connect() )
        return false;

    QSqlQuery temp(db_connection::instance()->db());
    temp.prepare(Query);
    if (!temp.exec())
    {
        qDebug() << temp.lastError();
        qDebug() << "select_search";
        qDebug() << temp.executedQuery();
        return false;
    }

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
        cr->state = IsReaded;
        list->append(cr);
    }

    return true;
}

void Crud::select_all()
{   ///Под снос
    QSqlQuery querry(db_connection::instance()->db());
    querry.prepare("SELECT "
                   "\"zk\".\"Zk_id\","
                   "regexp_replace(\"zk\".\"Lastname\", '\\s+$', ''),"
                   "regexp_replace(\"zk\".\"Name\", '\\s+$', ''),"
                   "regexp_replace(\"zk\".\"Mid_name\", '\\s+$', ''),"
                   "regexp_replace(\"zk\".\"Birth_date\", '\\s+$', ''),"
                   "regexp_replace(\"zk\".\"Reg_city\", '\\s+$', ''),"
                   "regexp_replace(\"zk\".\"Reg_street\", '\\s+$', ''),"
                   "regexp_replace(\"zk\".\"Reg_home\", '\\s+$', ''),"
                   "regexp_replace(\"zk\".\"Reg_corp\", '\\s+$', ''),"
                   "regexp_replace(\"zk\".\"Reg_flat\", '\\s+$', ''),"
                   "regexp_replace(\"zk\".\"Liv_city\", '\\s+$', ''),"
                   "regexp_replace(\"zk\".\"Liv_street\", '\\s+$', ''),"
                   "regexp_replace(\"zk\".\"Liv_home\", '\\s+$', ''),"
     /*12*/        "regexp_replace(\"zk\".\"Liv_corp\", '\\s+$', ''),"
                   "regexp_replace(\"zk\".\"Liv_flat\", '\\s+$', ''),"
                   ""
                   "regexp_replace(\"zk\".\"Check_for\", '\\s+$', ''),"
                   "regexp_replace(\"zk\".\"Dop_info\", '\\s+$', ''),"
                   "regexp_replace(\"zk\".\"Date_add\", '\\s+$', ''),"
                   "regexp_replace(\"zk\".\"Time_add\", '\\s+$', '')"
                   "FROM "
                   "\"zk\""
                   "WHERE \"zk\".\"Zk_id\" != 102" /// 102 - номер записи для служебных телефонов
                   " ORDER BY \"zk\".\"Zk_id\"");
    if (!querry.exec())
        qDebug() << querry.lastError();
    model->setQuery(querry);
    model->setHeaderData(0,Qt::Horizontal, QObject::tr("Номер ЗК"));
    model->setHeaderData(1,Qt::Horizontal, QObject::tr("Фамилия"));
    model->setHeaderData(2,Qt::Horizontal, QObject::tr("Имя"));
    model->setHeaderData(3,Qt::Horizontal, QObject::tr("Отчество"));
    model->setHeaderData(4,Qt::Horizontal, QObject::tr("Дата рождения"));

    model->setHeaderData(5,Qt::Horizontal, QObject::tr("Город регистрации"));
    model->setHeaderData(6,Qt::Horizontal, QObject::tr("Улица регистрации"));
    model->setHeaderData(7,Qt::Horizontal, QObject::tr("Дом регистрации"));
    model->setHeaderData(8,Qt::Horizontal, QObject::tr("Корпус регистрации"));
    model->setHeaderData(9,Qt::Horizontal, QObject::tr("Квартира регистрации"));

    model->setHeaderData(10,Qt::Horizontal, QObject::tr("Город проживания"));
    model->setHeaderData(11,Qt::Horizontal, QObject::tr("Улица проживания"));
    model->setHeaderData(12,Qt::Horizontal, QObject::tr("Дом проживания"));
    model->setHeaderData(13,Qt::Horizontal, QObject::tr("Корпус проживания"));
    model->setHeaderData(14,Qt::Horizontal, QObject::tr("Квартира проживания"));

    model->setHeaderData(15,Qt::Horizontal, QObject::tr("Проверяется в интересах"));
    model->setHeaderData(16,Qt::Horizontal, QObject::tr("Дополнительная информация"));
    model->setHeaderData(17,Qt::Horizontal, QObject::tr("День добавления"));
    model->setHeaderData(18,Qt::Horizontal, QObject::tr("Время добавления"));

    querry.clear();
}

bool Crud::selectAllDb(QList<Crud *> *list, QList<Owners_tel *> *otlist, QList<Contacts *> *contlist)
{
    list->clear();

    QSqlQuery query(db_connection::instance()->db());
    query.prepare("SELECT zk.zk_id, zk.lastname"
                  " From zk");
    if (!query.exec())
        qDebug() << "oops";
    while(query.next())
    {
        Crud *cr = new Crud;
        cr->zk_id = query.value(0).toInt();
        cr->lastname = query.value(1).toString();

        QSqlQuery temp1(db_connection::instance()->db());
        temp1.prepare("SELECT owners_tel.telephone_id, owners_tel.telephone_num "
                      " From owners_tel"
                       " WHERE owners_tel.fk_telephone_zk = (:id)");
        temp1.bindValue(":id", cr->zk_id);
        if(!temp1.exec())
            qDebug() << "temp1";
        while(temp1.next())
        {
            Owners_tel *ow = new Owners_tel;
            ow->tel_id = temp1.value(0).toInt();
            ow->tel_num = temp1.value(1).toString();
            QSqlQuery temp2(db_connection::instance()->db());

            temp2.prepare("SELECT contacts.contact_list_id, contacts.cl_telephone, contacts.cl_info"
                          " From contacts"
                          " WHERE contacts.fk_cl_telephone = (:id)");
            temp2.bindValue(":id", ow->tel_id);
            if(!temp2.exec())
                qDebug() << "temp2";

            while (temp2.next())
            {
                Contacts *con = new Contacts;
                con->contact_id = temp2.value(0).toInt();
                con->contact_tel_num = temp2.value(1).toString();
                con->mark  = temp2.value(2).toString();

                ow->cont->append(con);
            }
            otlist->append(ow);
        }
        list->append(cr);
    }
        return true;
}

void Crud::check() const
{
    qDebug() << zk_id;
    qDebug() << lastname;
    qDebug() << name;
    qDebug() << mid_name;
    qDebug() << birth_date;

    qDebug() << reg_city;
    qDebug() << reg_street;
    qDebug() << reg_home;
    qDebug() << reg_corp;
    qDebug() << reg_flat;

    qDebug() << liv_city;
    qDebug() << liv_street;
    qDebug() << liv_home;
    qDebug() << liv_corp;
    qDebug() << liv_flat;

    qDebug() << check_for;
    qDebug() << dop_info;
    qDebug() << date_add;
    qDebug() << time_add;

}

void Crud::zk_search()
{
    if( !db_connection::instance()->db_connect() )
        return;

    search_is_ready = false;
    model_is_ready = false; //Зануляем флаги

    msgbx.setText("Результат поиска");
    msgbx.setGeometry(960,510, 180,210);

    QString qry;
    if(zk_id != 0)
    {
        QString temp = QString::number(zk_id);
        qry += " AND zk.Zk_id = "+temp+" ";
    }

    if (!tel_num.isEmpty())
    {
       qry += " AND owners_tel.Telephone_num = ('"+tel_num+"')"
               "AND owners_tel.FK_Telephone_Zk = zk.Zk_id ";
    }

    if (!lastname.isEmpty())
    {
        qry += " AND LOWER(zk.Lastname) LIKE LOWER('"+lastname+"%') ";
    }
    if (!name.isEmpty())
    {
        qry += " AND LOWER(zk.Name) LIKE LOWER('"+name+"%')";
    }
    if (!mid_name.isEmpty())
    {
        qry += " AND LOWER(zk.Mid_name) LIKE LOWER('"+mid_name+"%')";
    }

    if(birth_date.count() > 2)
    {
        if (birth_date.at(0)=="." && birth_date.at(1) == ".")
        {
            birth_date.remove(0,2); /// Убираю точки
            qry += " AND zk.Birth_date LIKE ('______"+birth_date+"%')" ;
        }
            else
        {
            if (birth_date.at(0) == ".")
            {
                qry += " AND zk.Birth_date LIKE ('__"+birth_date+"%')" ;
            }
            else {
                birth_date.chop(1); ///убираю одну точку
                qry += " AND zk.Birth_date LIKE ('"+birth_date+"%')" ;
            }
        }
    }

    if (!reg_city.isEmpty())
    {
        qry += " AND LOWER(zk.Reg_city) LIKE LOWER('"+reg_city+"%')";
    }
    if (!reg_street.isEmpty())
    {
        qry += " AND LOWER(zk.Reg_street ) LIKE LOWER('"+reg_street+"%')";
    }

    if (!reg_home.isEmpty())
    {
        qry += " AND zk.Reg_home =('"+reg_home+"')";
    }

    if (!reg_corp.isEmpty())
    {
        qry += " AND zk.Reg_corp = ('"+reg_corp+"')";
    }
    if (!reg_flat.isEmpty())
    {
        qry += " AND zk.Reg_flat = ('"+reg_flat+"')";
    }

    if(date_add.count()>2)
    {
        if (date_add.at(0)=="-" && date_add.at(1) == "-")
        {
            date_add.remove(0,2); /// Убираю точки
            qry += " AND zk.Date_add LIKE ('______"+date_add+"%')" ;
        }
        else
        {
            if (date_add.at(0) == "-")
            {
                qry += " AND zk.Date_add LIKE ('__"+date_add+"%')" ;
            }
            else {
                date_add.chop(1); ///убираю одну точку
                qry += " AND zk. LIKE ('"+date_add+"%')" ;
            }
        }
    }

    if( Date_From.isValid() || Date_To.isValid() )
    {
        QString tempp;
            QSqlQuery querry(db_connection::instance()->db());
        querry.prepare("SELECT zk.Date_add, zk.Zk_id"
                       " FROM zk"
                       " WHERE zk.Date_add IS NOT NULL");

        if (!querry.exec())
            qDebug() << querry.lastError();

        while (querry.next())
        {
            QDate date;
            auto list =  querry.value(0).toString().split('-');
            date.setDate(list.at(0).toInt(),list.at(1).toInt(),list.at(2).toInt());

            if( (Date_From.isValid() && Date_To.isValid() && Date_From < Date_To) &&
                  (  date <= Date_To && date >= Date_From )) /// Если две даты
            {
                if(tempp.isEmpty())
                {
                    tempp+=" zk.Zk_id = "+querry.value(1).toString();
                }
                else
                {
                  tempp+=" OR  zk.Zk_id = "+querry.value(1).toString();
                }
            }
            else
            { /// Если есть дата С
                if( (Date_From.isValid() && !Date_To.isValid()) && date >= Date_From)
                {
                    if(tempp.isEmpty())
                    {
                        tempp+=" zk.Zk_id = "+querry.value(1).toString();
                    }
                    else
                    {
                      tempp+=" OR  zk.Zk_id = "+querry.value(1).toString();
                    }
                }
                else
                {
                    if( ( Date_To.isValid() && !Date_From.isValid() ) && date <= Date_To )
                    {/// Если есть дата ПО
                        if(tempp.isEmpty())
                        {
                            tempp+=" zk.Zk_id= "+querry.value(1).toString();
                        }
                        else
                        {
                          tempp+=" OR  zk.Zk_id = "+querry.value(1).toString();
                        }
                    }
//                    else /// Если мы вошли в это условие и не прошли ни по одному параметру
//                    {///месседжбокс с ошибкой
//                        msgbx.setText("<font size = '8'>Дата введена некорректно!</font>");
//                        msgbx.setStandardButtons(QMessageBox::Cancel);
//                        msgbx.setButtonText(QMessageBox::Cancel,"Закрыть");
//                        int ret = msgbx.exec();
//                        switch (ret) {
//                        case QMessageBox::Cancel:
//                            return;
//                       }
//                    }
                }
            }
        }
        if (!tempp.isEmpty())
        {
            qry+=" AND ("+tempp+" ) ";
        }

    }

if (!qry.isEmpty())
  {
   QSqlQuery querry(db_connection::instance()->db());
    querry.prepare("SELECT "
                   " COUNT(DISTINCT zk.Zk_id)"
                   " FROM "
                   " zk,owners_tel "
                   " WHERE "
                   " zk.Zk_id>0 " + qry);//qry - набор данных для поиска
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
         int ret = msgbx.exec();
         switch (ret) {
         case QMessageBox::Cancel:
             break;
                 }
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
        zk_search_model(qry);
        break;

    case QMessageBox::Open:
        zk_search_report(qry);
        break;

    case QMessageBox::Cancel:
        break;
            }
        }
        else
    {
    //Crud::select_all();
}
}

void Crud::zk_search_model(QString qry)
{
    Query = "SELECT  DISTINCT "        //стринг для модели
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
            "zk.Time_add"
            " FROM "
            "zk,owners_tel"
            " WHERE"
            " zk.Zk_id>0 " + qry + " ORDER BY zk.Zk_id";
    model_is_ready =true;

}

void Crud::zk_search_report(QString qry)
{
     QSqlQuery querry(db_connection::instance()->db());
    search_res.clear();

    querry.prepare("SELECT  DISTINCT "
                   "zk.Zk_id,"
                   "zk.Lastname, "
                   "zk.Name, "
                   "zk.Mid_name,"
                   "zk.Birth_date,"
                   "zk.Reg_city,"
                   "zk.Reg_street,"
                   "zk.Reg_home,"
                   "zk.Reg_corp,"
                   "zk.Reg_flat,"
                   "zk.Check_for,"
                   "zk.Dop_info,"
                   "zk.Date_add,"
                   "zk.Time_add"
                   " FROM "
                   "zk ,owners_tel"
                   " WHERE"
                   " zk.Zk_id>0 " + qry + " ORDER BY zk.Zk_id");
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
        QSqlQuery temp(db_connection::instance()->db());
        temp.prepare("SELECT owners_tel.Telephone_num"
                      " FROM owners_tel"
                      " WHERE owners_tel.FK_Telephone_Zk = (:id)");
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

/////////////////////////////////////////////////////
void Crud::call_update_list()
{
    if( !db_connection::instance()->db_connect() )
        return;
        QSqlQuery querry(db_connection::instance()->db());
    querry.prepare("SELECT "
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
     /*12*/        "zk.Liv_corp,"
                   "zk.Liv_flat,"
                   ""
                   "zk.Check_for,"
                   "zk.Dop_info "
                   "FROM "
                   " zk "
                   " WHERE zk.Zk_id = (:id)");
    querry.bindValue(":id", zk_id);
   if (!querry.exec())
      qDebug() << querry.lastError();
    while (querry.next())
    {
        lastname = querry.value(0).toString();
        name = querry.value(1).toString();
        mid_name = querry.value(2).toString();
        birth_date = querry.value(3).toString();

        reg_city = querry.value(4).toString();
        reg_street = querry.value(5).toString();
        reg_home = querry.value(6).toString();
        reg_corp = querry.value(7).toString();
        reg_flat = querry.value(8).toString();

        liv_city = querry.value(9).toString();
        liv_street = querry.value(10).toString();
        liv_home = querry.value(11).toString();
        liv_corp = querry.value(12).toString();
        liv_flat = querry.value(13).toString();

        check_for = querry.value(14).toString();
        dop_info = querry.value(15).toString();
    }
         qDebug() << querry.executedQuery();
    querry.clear();
}

bool Crud::update_zk()
{
        QSqlQuery querry(db_connection::instance()->db());
    querry.prepare("UPDATE zk "
                   " SET Lastname = (:lastname),"
                   "Name = (:name),"
                   "Mid_name = (:mid_name),"
                   ""
                   "Birth_date = (:b_d),"
                   ""
                   "Reg_city = (:r_c),"
                   "Reg_street = (:r_s),"
                   "Reg_home = (:r_h),"
                   "Reg_corp = (:r_corp),"
                   "Reg_flat = (:r_f),"
                   ""
                   "Liv_city = (:l_c),"
                   "Liv_street = (:l_s),"
                   "Liv_home = (:l_h),"
                   "Liv_corp = (:l_corp),"
                   "Liv_flat = (:l_f),"
                   ""
                   "Check_for = (:c_f),"
                   "Dop_info = (:d_i) "
                   " WHERE zk.Zk_id = (:id)");

    querry.bindValue(":lastname",lastname);
    querry.bindValue(":name",name);
    querry.bindValue(":mid_name",mid_name);
    querry.bindValue(":b_d",birth_date);

    querry.bindValue(":r_c",reg_city);
    querry.bindValue(":r_s",reg_street);
    querry.bindValue(":r_h",reg_home);
    querry.bindValue(":r_corp",reg_corp);
    querry.bindValue(":r_f",reg_flat);

    querry.bindValue(":l_c",liv_city);
    querry.bindValue(":l_s",liv_street);
    querry.bindValue(":l_h",liv_home);
    querry.bindValue(":l_corp",liv_corp);
    querry.bindValue(":l_f",liv_flat);

    querry.bindValue(":c_f",check_for);
    querry.bindValue(":d_i",dop_info);

    querry.bindValue(":id", zk_id);
    if (!querry.exec())
    {
        qDebug() << querry.lastError();
        return false;
    }
    qDebug() << querry.executedQuery();
    querry.clear();
    return true;
}

//ДОДЕЛАЙ
bool Crud::add_zk()
{
        QSqlQuery querry(db_connection::instance()->db());
    querry.prepare("INSERT INTO zk "
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
                   "(:d_a), (:t_a)) "
                   " RETURNING zk.Zk_id");

    querry.bindValue(":lastname",lastname);
    querry.bindValue(":name",name);
    querry.bindValue(":mid_name",mid_name);
    querry.bindValue(":b_d", birth_date);

    querry.bindValue(":r_c",reg_city);
    querry.bindValue(":r_s",reg_street);
    querry.bindValue(":r_h",reg_home);
    querry.bindValue(":r_corp",reg_corp);
    querry.bindValue(":r_f",reg_flat);

    querry.bindValue(":l_c",liv_city);
    querry.bindValue(":l_s",liv_street);
    querry.bindValue(":l_h",liv_home);
    querry.bindValue(":l_corp",liv_corp);
    querry.bindValue(":l_f",liv_flat);

    querry.bindValue(":c_f",check_for);
    querry.bindValue(":d_i",dop_info);

    querry.bindValue(":d_a",date_add);
    querry.bindValue(":t_a",time_add);

    if (!querry.exec())
    {
        qDebug() << querry.lastError();
        return false;
    }

    while (querry.next())
    {
        new_zk_id = querry.value(0).toInt();
    }

    qDebug() << querry.executedQuery();
    querry.clear();
    return true;
}

void Crud::del_zk(int del_id)
{    QSqlQuery querry(db_connection::instance()->db());
    querry.prepare("DELETE FROM zk "
                 " WHERE zk.Zk_id = (:id)");
    querry.bindValue(":id", del_id);
    if (!querry.exec())
       qDebug() << querry.lastError();
    querry.clear();
}

void Crud::id_zk_search()
{/// Под снос
        QSqlQuery querry(db_connection::instance()->db());
    querry.prepare("SELECT "
                   "zk.\"Zk_id\","
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
                   "\"zk\""
                   "WHERE "
                   "\"zk\".\"Zk_id\" = (:id)"
                   " ORDER BY \"zk\".\"Zk_id\"");
    querry.bindValue(":id", zk_id);
    if (!querry.exec())
        qDebug() << querry.lastError();
    qDebug() << querry.executedQuery();
    model->setQuery(querry);

    querry.clear();
}

void Crud::get_max_zk()
{
    QSqlQuery querry(db_connection::instance()->db());
    querry.prepare("SELECT MAX(zk.Zk_id)"
                   " FROM zk");
    if (!querry.exec())
       qDebug() << querry.lastError();
    while (querry.next())
    {
        zk_id = querry.value(0).toInt();
    }
    qDebug() << querry.executedQuery();
    querry.clear();
}

void Crud::get_min_zk()
{
    QSqlQuery querry(db_connection::instance()->db());
    querry.prepare("SELECT MIN(zk.Zk_id)"
                   " FROM zk");
    if (!querry.exec())
       qDebug() << querry.lastError();
    while (querry.next())
    {
        zk_id = querry.value(0).toInt();
    }
    qDebug() << querry.executedQuery();
    querry.clear();
}

int Crud::get_id_from_tel(QString t_n)
{
    QSqlQuery querry(db_connection::instance()->db());
    querry.prepare("SELECT owners_tel.FK_Telephone_Zk"
                   " FROM  owners_tel"
                   " WHERE "
                   "owners_tel.Telephone_num = ('"+t_n+"')");
    if (!querry.exec())
       qDebug() << querry.lastError();
    while (querry.next())
    {
      zk_id = querry.value(0).toInt();
      return zk_id;
    }
}
