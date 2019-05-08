#include "crud.h"
#include <QMessageBox>
#include <QDialogButtonBox>

#include <QMetaObject>

void Crud::select_all()
{
    querry.prepare("SELECT "
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
                   "\"zk\""
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
    model->setHeaderData(10,Qt::Horizontal, QObject::tr("Проверяется в интересах"));
    model->setHeaderData(11,Qt::Horizontal, QObject::tr("Дополнительная информация"));
    model->setHeaderData(12,Qt::Horizontal, QObject::tr("День добавления"));
    model->setHeaderData(13,Qt::Horizontal, QObject::tr("Время добавления"));

    querry.clear();
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

    qDebug() << check_for;
    qDebug() << dop_info;
    qDebug() << date_add;
    qDebug() << time_add;

}

void Crud::select_telephone()
{
    querry.prepare("SELECT \"owners_tel\".\"Telephone_num\" "
                   "FROM  \"owners_tel\" "
                   "WHERE "
                   " \"owners_tel\".\"FK_Telephone_Zk\" = (:id)");

        querry.bindValue(":id",zk_id);

    if (!querry.exec())
        qDebug() << querry.lastError();
    model->setQuery(querry);
    model->setHeaderData(0,Qt::Horizontal, QObject::tr("Номер телефона"));
    querry.clear();
}

void Crud::refresh_table()
{
    model_2->clear();
}

void Crud::recieve_tel_list()
{
    temp.prepare("SELECT \"owners_tel\".\"Telephone_id\""
                 "FROM \"owners_tel\""
                 "WHERE \"owners_tel\".\"Telephone_num\" = (:tel)");

    temp.bindValue(":tel", telephone_num);
    if (!temp.exec())
        qDebug() << temp.lastError();

    querry.prepare("SELECT \"contacts\".\"cl_telephone\","
                   "regexp_replace(\"contacts\".\"cl_info\", '\\s+$', '')"
                   "FROM \"contacts\""
                   "WHERE \"contacts\".\"FK_Cl_telephone\" = (:id) ");
    while (temp.next())
    {
        qDebug() << temp.value(0);
        querry.bindValue(":id",temp.value(0).toInt());
    }

    if (!querry.exec())
        qDebug() << querry.lastError();
    qDebug() << querry.executedQuery();
    model->setQuery(querry);
    model->setHeaderData(0,Qt::Horizontal, QObject::tr("Номер телефона"));
    model->setHeaderData(1,Qt::Horizontal, QObject::tr("Пометка"));
    querry.clear();
    temp.clear(); //чистим темп для айди номера телефона
}

void Crud::call_update_list()
{
    querry.prepare("SELECT "
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
                   "\"zk\""
                   "WHERE \"zk\".\"Zk_id\" = (:id)");
    querry.bindValue(":id", zk_id);
   if (!querry.exec())
      qDebug() << querry.lastError();
    while (querry.next())
    {
        lastname = querry.value(0).toString();
        name = querry.value(1).toString();
        mid_name = querry.value(2).toString();
        //birth_date.fromString(querry.value(3).toString());

//        adres_reg = querry.value(4).toString();
//        adres_liv = querry.value(5).toString();

        reg_city = querry.value(4).toString();
        reg_street = querry.value(5).toString();
        reg_home = querry.value(6).toString();
        reg_corp = querry.value(7).toString();
        reg_flat = querry.value(8).toString();

        check_for = querry.value(9).toString();
        dop_info = querry.value(10).toString();
    }
         qDebug() << querry.executedQuery();
    querry.clear();
}

void Crud::update_zk()
{
    querry.prepare("UPDATE \"zk\""
                   "SET \"Lastname\" = (:lastname),"
                   "\"Name\" = (:name),"
                   "\"Mid_name\" = (:mid_name),"
                   ""
                   //"\"Adres_reg\" = (:a_r),"
                   //"\"Adres_liv\" = (:a_l),"
                   ""
                   "\"Reg_city\" = (:r_c),"
                   "\"Reg_street\" = (:r_s),"
                   "\"Reg_home\" = (:r_h),"
                   "\"Reg_corp\" = (:r_corp),"
                   "\"Reg_flat\" = (:r_f),"
                   ""
                   "\"Check_for\" = (:c_f),"
                   "\"Dop_info\" = (:d_i)"
                   "WHERE \"zk\".\"Zk_id\" = (:id)");
    querry.bindValue(":lastname",lastname);
    querry.bindValue(":name",name);
    querry.bindValue(":mid_name",mid_name);

    //querry.bindValue(":a_r", adres_reg);
    //querry.bindValue(":a_l", adres_liv);

    querry.bindValue(":r_c",reg_city);
    querry.bindValue(":r_s",reg_street);
    querry.bindValue(":r_h",reg_home);
    querry.bindValue(":r_corp",reg_corp);
    querry.bindValue(":r_f",reg_flat);

    querry.bindValue(":c_f",check_for);
    querry.bindValue(":d_i",dop_info);

    querry.bindValue(":id", zk_id);
    if (!querry.exec())
       qDebug() << querry.lastError();
    qDebug() << querry.executedQuery();
    querry.clear();
}

//ДОДЕЛАЙ
void Crud::add_zk()
{
    querry.prepare("INSERT INTO \"zk\""
                   "(\"Lastname\", \"Name\",\"Mid_name\", \"Birth_date\","
                // "\"Adres_reg\",\"Adres_liv\","
                   "\"Reg_city\",\"Reg_street\",\"Reg_home\",\"Reg_corp\","
                   "\"Reg_flat\","
                   "\"Check_for\", \"Dop_info\","
                   "\"Date_add\", \"Time_add\")"
                   " VALUES ((:lastname),(:name),(:mid_name), (:b_d),"
              //   "(:a_r), (:a_l),"
                   "(:r_c),(:r_s),(:r_h),(:r_corp),(:r_f),(:c_f),(:d_i),"
                   "(:d_a), (:t_a))");
    querry.bindValue(":lastname",lastname);
    querry.bindValue(":name",name);
    querry.bindValue(":mid_name",mid_name);
    querry.bindValue(":b_d", birth_date);

    // querry.bindValue(":a_r", adres_reg);
    // querry.bindValue(":a_l", adres_liv);

    querry.bindValue(":r_c",reg_city);
    querry.bindValue(":r_s",reg_street);
    querry.bindValue(":r_h",reg_home);
    querry.bindValue(":r_corp",reg_corp);
    querry.bindValue(":r_f",reg_flat);

    querry.bindValue(":c_f",check_for);
    querry.bindValue(":d_i",dop_info);

    querry.bindValue(":d_a",date_add);
    querry.bindValue(":t_a",time_add);

    if (!querry.exec())
       qDebug() << querry.lastError();
    qDebug() << querry.executedQuery();
    querry.clear();

    querry.prepare("SELECT MAX(\"zk\".\"Zk_id\")"
                   " FROM \"zk\"");
    if (!querry.exec())
       qDebug() << querry.lastError();
    while (querry.next())
    {
        zk_id = querry.value(0).toInt();
    }
    qDebug() << querry.executedQuery();
    querry.clear();
}

void Crud::del_zk()
{
    //Удаляю ЗК
    temp.prepare("DELETE FROM \"zk\""
                 "WHERE \"zk\".\"Zk_id\" = (:id)");
    temp.bindValue(":id", zk_id);
    if (!temp.exec())
       qDebug() << temp.lastError();
    temp.clear();
}


void Crud::id_zk_search()
{
    querry.prepare("SELECT "
                   "\"zk\".\"Zk_id\","
                   "\"zk\".\"Lastname\", "
                   "\"zk\".\"Name\", "
                   "\"zk\".\"Mid_name\", "
                   "\"zk\".\"Birth_date\","
                //   "\"Zk\".\"Adres_liv\","
                //   "\"Zk\".\"Adres_reg\", "
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
    querry.prepare("SELECT MAX(\"zk\".\"Zk_id\")"
                   " FROM \"zk\"");
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
    querry.prepare("SELECT MIN(\"zk\".\"Zk_id\")"
                   " FROM \"zk\"");
    if (!querry.exec())
       qDebug() << querry.lastError();
    while (querry.next())
    {
        zk_id = querry.value(0).toInt();
    }
    qDebug() << querry.executedQuery();
    querry.clear();
}



