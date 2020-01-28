#include "_Crud.h"
#include <QMessageBox>
#include <QDialogButtonBox>

#include <QMetaObject>
#include <QFile>

QList<Owners_tel *> *Crud::owt()
{
///идея хорошая, но мне так не нужно
    if (_owt == nullptr)
    {
        state = IsNewing;
        _owt = new QList<Owners_tel*>;
    }
    checkState_ = Unchecked;
    return _owt;
}


Crud::Crud(int id): zk_id(id)
{
    _owt = nullptr;
    checkState_ = Unchecked;
}

Crud::~Crud()
{
    Owners_tel::delete_all(_owt);
}

Crud::Crud()
{
        _owt = nullptr;
    checkState_ = Unchecked;
}
//-----------------------------------------------------------------------------------//
bool Crud:: selectAll(QList<Crud *> *list)
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
                 "zk.time_add, "
                 "zk.date_upd,"
                 "zk.nickname "
                 " FROM "
                 " zk "
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
        cr->date_upd = temp.value(19).toString();
        cr->nickname = temp.value(20).toString();
        cr->state = IsReaded;
        list->append(cr);
    }

    if(list->isEmpty())
        return false;

    return true;
}
//-----------------------------------------------------------------------------------//
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
//-----------------------------------------------------------------------------------//
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
                   ""
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
//-----------------------------------------------------------------------------------//
bool Crud::update_zk(QList<int> *list_id)
{
    date_upd = QDate::currentDate().toString(Qt::ISODate)+" "+QTime::currentTime().toString();
        QSqlQuery querry(db_connection::instance()->db());
        QSqlQuery query1(db_connection::instance()->db());
        QSqlQuery query2(db_connection::instance()->db());

        QString cname = db_connection::instance()->db().connectionName();

        bool isOk = db_connection::instance()->db().database(cname).transaction();
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
                   "Dop_info = (:d_i),"
                   "Date_upd = (:d_u),"
                   "Nickname = (:n_n)"
                   " WHERE zk.Zk_id = (:id) "
                   " RETURNING zk.row_id");

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

    querry.bindValue(":d_u",date_upd);
    querry.bindValue(":n_n", nickname);

    querry.bindValue(":id", zk_id);
    if (!querry.exec())
    {
        qDebug() << birth_date;
        qDebug() << querry.lastError();
        qDebug() << querry.executedQuery();
        isOk = false;
    }else
    {
        //вызвать метод добавления в линкед намс

        if(list_id != nullptr && isOk)
            while(querry.next())
            {
                //Сначла проверка
                for (int i = 0; i < list_id->size(); i++)
                {
                    if(list_id->at(i) != zk_id)
                    {
                        query2.prepare("SELECT *"
                                       " FROM zk_links"
                                       " WHERE (row_id2 = '"+querry.value(0).toString()+ "' AND row_id1 = (SELECT row_id FROM zk WHERE zk_id = "+QString::number(list_id->at(i))+"))"
                                        " OR"
                                        " (row_id1 = '"+querry.value(0).toString()+ "' AND row_id2 = (SELECT row_id FROM zk WHERE zk_id = "+QString::number(list_id->at(i))+"))");

                        if(query2.exec())
                        {
                            if(!query2.next())
                            {
                                query1.prepare("INSERT INTO zk_links (row_id1,row_id2) "
                                               " VALUES ('"+querry.value(0).toString()+"',(SELECT row_id"
                                                                                       " FROM zk"
                                                                                       " WHERE zk.zk_id = "+QString::number(list_id->at(i))+"))");
                                if(!query1.exec())
                                {
                                    qDebug() << query1.lastError();
                                    qDebug() << query1.executedQuery();
                                    isOk = false;
                                    break;
                                }
                                else {
                                    qDebug() << query1.executedQuery();
                                }
                            }
                        }
                        else
                            qDebug() << query2.lastError() << query2.executedQuery();
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
}
//-----------------------------------------------------------------------------------//
bool Crud::add_zk()
{
     date_add = QDate::currentDate().toString(Qt::ISODate);
     time_add = QTime::currentTime().toString();
    QSqlQuery querry(db_connection::instance()->db());
    querry.prepare("INSERT INTO zk "
                   "(Lastname, Name, Mid_name, Birth_date,"
                   "Reg_city, Reg_street, Reg_home, Reg_corp,"
                   "Reg_flat,"
                   "Liv_city, Liv_street, Liv_home, Liv_corp,"
                   "Liv_flat,"
                   "Check_for, Dop_info,"
                   "Date_add, Time_add, Nickname) "
                   " VALUES ((:lastname),(:name),(:mid_name), (:b_d),"
                   "(:r_c),(:r_s),(:r_h),(:r_corp),(:r_f),"
                   "(:l_c),(:l_s),(:l_h),(:l_corp),(:l_f),"
                   "(:c_f),(:d_i),"
                   "(:d_a), (:t_a), (:n_n)) "
                   " RETURNING zk.Zk_id, zk.row_id");

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
    querry.bindValue(":n_n", nickname);

    if (!querry.exec())
    {
        qDebug() << birth_date;
        qDebug() << querry.lastError() << querry.value(1).toString();
        return false;
    }

    while (querry.next())
    {
        zk_id = querry.value(0).toInt();
    }

    qDebug() << querry.executedQuery();
    return true;
}

//-----------------------------------------------------------------------------------//
void Crud::del_zk(int del_id)
{
    QString cname = db_connection::instance()->db().connectionName();

    bool isOk = db_connection::instance()->db().database(cname).transaction();

    QSqlQuery querry(db_connection::instance()->db());
    querry.prepare("DELETE FROM zk "
                 " WHERE zk.Zk_id = (:id)");
    querry.bindValue(":id", del_id);
    if (!querry.exec())
    {
        isOk = false;
        qDebug() << querry.lastError();
    }else
        qDebug() << querry.executedQuery();

    if(!isOk)
    {
        db_connection::instance()->db().database(cname).rollback();
    }else
    {
        db_connection::instance()->db().database(cname).commit();
    }
}
//-----------------------------------------------------------------------------------//
Crud* Crud::id_zk_search(int zk_id)
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
                   "zk.time_add, "
                   "zk.date_upd, "
                   "zk.row_id"
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
        cr->row_id = querry.value(20).toString();
        cr->state = IsReaded;
        return cr;
    }
    else {
        return nullptr;
    }
}
//-----------------------------------------------------------------------------------//
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
//-----------------------------------------------------------------------------------//
bool Crud::save_all_crud(Crud *cr)
{
        if (cr->owt()->isEmpty())
            return true;
    if(!db_connection::instance()->db_connect())
        return false;

    QString cname = db_connection::instance()->db().connectionName();

    bool isOk = db_connection::instance()->db().database(cname).transaction();


    for (int i = 0; i< cr->owt()->size(); i++)
    {
        if ( !isOk )
            break;

        if(!cr->owt()->at(i)->tel_num.isEmpty())
        {
            switch (cr->owt()->at(i)->state)
            {
            case IsNewing:
                int a; //локальная переменная
                a = cr->owt()->at(i)->insert_tel(false, cr->zk_id);
                if (a != -1) //если успешно добавил телефон
                   isOk = Contacts::saveAll_cont(cr->owt()->at(i)->cont(), a); //сохраняю контакты с новым айди
                break;
            case IsChanged:
                if  (cr->owt()->at(i)->update_tel(false))
                {
                    int a = cr->owt()->at(i)->tel_id;
                    isOk = Contacts::saveAll_cont(cr->owt()->at(i)->cont(),a);
                } else
                    isOk = false;
                break;
            case IsRemoved:
                cr->owt()->at(i)->remove_tel();
                break;
            case IsReaded: //Не трогал телефоны, но вдруг менял контакты
                if (cr->owt()->at(i)->state == IsReaded)
                {//Повторное условие для того чтоб сделать переменную локальной
                    int a = cr->owt()->at(i)->tel_id;
                    isOk = Contacts::saveAll_cont(cr->owt()->at(i)->cont(),a);
                }
                    break;
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
//-----------------------------------------------------------------------------------//
Crud* Crud::operator+ (Crud *old_crud)
{
     zk_id = old_crud->zk_id;
     lastname = old_crud->lastname;
     name = old_crud->name;
     mid_name = old_crud->mid_name;
     birth_date = old_crud->birth_date;
     check_for = old_crud->check_for;
     dop_info = old_crud->dop_info;

    //Адрес регистрации
     reg_city = old_crud->reg_city;
     reg_street = old_crud->reg_street;
     reg_home= old_crud->reg_home;
     reg_corp= old_crud->reg_corp;
     reg_flat= old_crud->reg_flat;

    //Адрес проживания
     liv_city= old_crud->liv_city;
     liv_street= old_crud->liv_street;
     liv_home= old_crud->liv_home;
     liv_corp= old_crud->liv_corp;
     liv_flat= old_crud->liv_flat;
     return this;
}
//-----------------------------------------------------------------------------------//
QList<CompareResult>* Crud::compare_cruds(Crud *cmp_cr)
{
    if(cmp_cr != nullptr)
    {
        QList<CompareResult> *list = new QList<CompareResult>;

        if(lastname == cmp_cr->lastname)
            list->append(lastname_CR);

        if(name == cmp_cr->name)
            list->append(name_CR);

        if(mid_name == cmp_cr->mid_name)
            list->append(mid_name_CR);

        if(birth_date == cmp_cr->birth_date)
            list->append(birth_date_CR);

        if(check_for == cmp_cr->check_for)
            list->append(check_for_CR);

        if(dop_info == cmp_cr->dop_info)
            list->append(dop_info_CR);

        if(reg_city == cmp_cr->reg_city)
            list->append(reg_city_CR);

        if(reg_street == cmp_cr->reg_street)
            list->append(reg_street_CR);

        if(reg_home == cmp_cr->reg_home)
            list->append(reg_home_CR);

        if(reg_corp == cmp_cr->reg_corp)
            list->append(reg_corp_CR);

        if(reg_flat == cmp_cr->reg_flat)
            list->append(reg_flat_CR);

        if(liv_city == cmp_cr->liv_city)
            list->append(liv_city_CR);

        if(liv_street == cmp_cr->liv_street)
            list->append(liv_street_CR);

        if(liv_home == cmp_cr->liv_home)
            list->append(liv_home_CR);

        if(liv_corp == cmp_cr->liv_corp)
            list->append(liv_corp_CR);

        if(liv_flat == cmp_cr->liv_flat)
            list->append(liv_flat_CR);
        return list;
    }
}
//-----------------------------------------------------------------------------------//
void Crud::Change_linked_in_db(bool state, int id, QString linked)
{
}
//-----------------------------------------------------------------------------------//
bool Crud::compare_with_base(QString query_tel_num, QString query_fio, int id, QList<int> *id_list)
{//id необходим для сортировки запроса
    if( !db_connection::instance()->db_connect() )
        return false;

//    if (!linked_nums.isEmpty())
//        linked_nums.clear();
    if (id_list == nullptr)
        id_list = new QList<int>;

    QSqlQuery temp(db_connection::instance()->db());
    QString Query;

    ///Собираем строку для запроса
    //Если не ищем номера, то ставим некорректное условие для запроса
    if (query_tel_num.isEmpty())
        query_tel_num = " owners_tel.FK_Telephone_Zk = 0 ";

    Query += "SELECT owners_tel.FK_Telephone_Zk AS fk, owners_tel.telephone_num AS tel_num"
            " FROM owners_tel "
            " WHERE ("+query_tel_num+")";

    if (id != 0) //в случае редактирования имеющейся записи
    {            //исключаем редактируемую запись
      Query +=  " AND owners_tel.FK_Telephone_Zk != "+QString::number(id);
    }

    if(!id_list->isEmpty())
    {
        for (int i = 0;i<id_list->size();i++)
        {
         Query+=" AND owners_tel.FK_Telephone_Zk != "+QString::number(id_list->at(i));
        }
    }
    Query+= " UNION"
            " SELECT zk.zk_id, 'NULL'"
            " FROM zk"
            " WHERE "+query_fio;

    if (id != 0) //в случае редактирования имеющейся записи
    {            //исключаем редактируемую запись
      Query +=  " AND zk.zk_id !=  "+QString::number(id);
    }

    //Доп исключения из списка линкедайди
    if(!id_list->isEmpty())
    {
        for (int i = 0;i<id_list->size();i++)
        {
         Query+=" AND zk.zk_id != "+QString::number(id_list->at(i));
        }
    }

    temp.prepare(Query);
        if (!temp.exec())
        {
            db_connection::instance()->lastError = temp.lastError().text();
            qDebug() << temp.lastError() << Query;
            return false;
        }else
        {
            qDebug() << temp.executedQuery();
            compare_result = new QList<Cl_in_db_struct>;
        }
        QList<int> *temp_for_contains = new QList<int>;
         while(temp.next())
        {
             if(!temp_for_contains->contains(temp.value(0).toInt()))
             {
                 temp_for_contains->append(temp.value(0).toInt());
                 Cl_in_db_struct cl;
                 cl.id = temp.value(0).toInt();
                 cl.Tel_num = temp.value(1).toString();
                 compare_result->append(cl);
             }

             /*//необходимо проверить появились ли новые совпадения
            zk_id = temp.value(0).toInt();

            //сбор локального списка с совпавшими ид
            if(!for_numbers.contains(zk_id) && zk_id != 0)
                for_numbers.append(zk_id);

            if (temp.value(1).toInt() != 0 && !for_numbers.contains(temp.value(1).toInt()))
                for_numbers.append(temp.value(1).toInt());

            //Проверка на содержимое старого списка для линкед намс (позже удалить)

          if(id_list != nullptr && !id_list->isEmpty())
            if(zk_id != 0  &&  !id_list->contains(zk_id))
                linked_nums += ","+QString::number(zk_id);

           if(id_list != nullptr && !id_list->isEmpty())
            if (temp.value(1).toInt() != 0 && !id_list->contains(zk_id))
                linked_nums += ","+QString::number(temp.value(1).toInt());


            Owners_tel *ow = new Owners_tel();
            owt()->append(ow);
            owt()->at(0)->parentZK_id = temp.value(1).toInt();
            owt()->at(0)->tel_num = temp.value(2).toString();*/
        }

         if (compare_result == nullptr || compare_result->isEmpty())
         {

             delete temp_for_contains;
             return true;
         }
        else
        {
            delete temp_for_contains;
            return false;
        }
}
//-----------------------------------------------------------------------------------//
QList<int> Crud::string_parsing(QString linked_nums_string)
{
    //парсинг строки
        int t = 0;
        QString a;
        QList<int> temp;
        while (t  < linked_nums_string.size() )
        {
           if (linked_nums_string.at(t) != ',')
           {
              a.append(linked_nums_string.at(t));
              t++;
           }else
           {
            if(!a.isEmpty())
                temp.append(a.toInt());
            a.clear();
            t++;
           }
        }
        temp.append(a.toInt());
        return temp;
}

//-----------------------------------------------------------------------------------//
QList<int> *Crud::take_links(QString row_id, SqlType sqltype, QString filename, QString password)
{
    db_connection *db = db_connection::instance();

    db_connection::instance()->set_Sql_type(sqltype);
    QFile db_file;

    if(filename != nullptr && !filename.isEmpty())
    {

        if(sqltype == SQLliteType)
        {
            db_file.setFileName(filename); //Установка имени файлу дб
            db->db().setDatabaseName(db_file.fileName());
        }
        else if(sqltype == SQLlitechipher)
        {
            db_file.setFileName(filename);
            if (db->db().open("user",password) )
            {
                db->db().setDatabaseName(db_file.fileName());
            }
        }
    }

    QSqlQuery querry(db_connection::instance()->db());
    qDebug() << db->db_connect()<<db->db().lastError();

    QList<int> *temp = nullptr ;
    querry.prepare(" Select zk.zk_id  FROM "
                   " (SELECT row_id1 FROM zk_links  "
                   " where row_id2 = '"+row_id+"' "
                   " UNION "
                   " SELECT row_id2 FROM zk_links  "
                   " where row_id1 = '"+row_id+"') as t "
                   " LEFT JOIN zk"
                   " ON zk.row_id = t.row_id1");
    if(!querry.exec())
    {
        qDebug() << querry.lastError() << querry.executedQuery();
        return temp;
    }else
    {
        temp = new QList<int>;
        while (querry.next())
        {
            temp->append(querry.value(0).toInt());
        }
       if(sqltype == SQLliteType)
       {
           db_file.close();
           db->db().close();
           db->set_Sql_type(PSQLtype); /// Перевожу обратно на PSQL, тк работаю в основном с ним
           qDebug() << db->db_connect()<<db->db().lastError();
       }
        return temp;
    }
}

bool Crud::delete_all(QList<Crud*> *list)
{
    if(!list->isEmpty())
        {
            for(QList<Crud*>::const_iterator it = list->begin(); it != list->end(); it++)
            {
                delete *it;
            }

        }
}
