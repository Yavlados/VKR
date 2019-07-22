#include "_Crud.h"
#include <QMessageBox>
#include <QDialogButtonBox>

#include <QMetaObject>

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

Crud::Crud()
{
        _owt = nullptr;
    checkState_ = Unchecked;
}

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
                 "zk.date_upd "
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
        cr->state = IsReaded;
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
                   "zk.Time_add,"
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

bool Crud::update_zk()
{
    date_upd = QDate::currentDate().toString(Qt::ISODate)+" "+QTime::currentTime().toString();
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
                   "Dop_info = (:d_i),"
                   "Date_upd = (:d_u) "
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

    querry.bindValue(":d_u",date_upd);
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

bool Crud::add_zk()
{
     date_add = QDate::currentDate().toString(Qt::ISODate);
     time_add = QTime::currentTime().toString();
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
        zk_id = querry.value(0).toInt();
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

}

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
        return cr;
    }
    else {
        return nullptr;
    }
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

bool Crud::compare_with_base(QString query_tel_num, QString query_fio, int id)
{//id необходим для сортировки запроса
    if( !db_connection::instance()->db_connect() )
        return false;

    QSqlQuery temp(db_connection::instance()->db());
    QString Query;
    ///Собираем строку для запроса
    //Если не ищем номера, то ставим некорректное условие для запроса
    if (query_tel_num.isEmpty())
        query_tel_num = " owners_tel.FK_Telephone_Zk = 0 ";

    Query = "SELECT DISTINCT B.id, C.id, B.t_n"
            " FROM "
            " (SELECT zk.zk_id as id, a.tel_num as t_n"
            " FROM zk"
            " INNER JOIN (SELECT owners_tel.FK_Telephone_Zk AS fk, owners_tel.telephone_num AS tel_num"
            " FROM owners_tel "
            " WHERE ("+query_tel_num+")";

    if (id != 0) //в случае редактирования имеющейся записи
    {            //исключаем редактируемую запись
      Query +=  " AND owners_tel.FK_Telephone_Zk != "+QString::number(id);
    }

    Query +=") AS a"
            " ON zk.zk_id = a.fk) AS B FULL OUTER JOIN"
            " (SELECT zk.zk_id as id"
            " FROM zk"
            " WHERE "+query_fio;

    if (id != 0) //в случае редактирования имеющейся записи
    {            //исключаем редактируемую запись
      Query +=  " AND zk.zk_id !=  "+QString::number(id);
    }

    Query +=") AS C on 1=1"
            " ORDER BY B.id";

    temp.prepare(Query);
        if (!temp.exec())
        {
            db_connection::instance()->lastError = temp.lastError().text();
            qDebug() << temp.lastError() << Query;
            return false;
        }else {
    qDebug() << temp.executedQuery();
}
        while(temp.next())
        {
            zk_id = temp.value(1).toInt();
            Owners_tel *ow = new Owners_tel();
            owt()->append(ow);
            owt()->at(0)->parentZK_id = temp.value(0).toInt();
            owt()->at(0)->tel_num = temp.value(2).toString();
            return false;
        }

    return true;
}
