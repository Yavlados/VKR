#ifndef CRUD_H
#define CRUD_H

#include <QString>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSqlQueryModel>
#include <QDate>
#include <QTime>
#include <QMessageBox>

class Crud{

private:

    //ТАБЛИЦА 2
    QString telephone_num;

    //Переменны для запросов
    QSqlQuery temp;
    QSqlQuery querry;
    QSqlQuery temp_2; // для метода даления

signals:
    void Send_search(QString);

public:

    QString search_res;
    bool search_is_ready, model_is_ready;
    QMessageBox msgbx;
    QString search_result;

    // ТАБЛИЦА 1
    int zk_id;
    QString lastname;
    QString name;
    QString mid_name;
    QString birth_date;
    QString check_for;
    QString dop_info;
    //Адрес регистрации

    QString adres_reg;
    QString adres_liv;

    QString reg_city;
    QString reg_street;
    QString reg_home;
    QString reg_corp;
    QString reg_flat;
    //Адрес проживания

    //Дата и время сейчас
    QString date_add = QDate::currentDate().toString(Qt::ISODate);
    QString time_add = QTime::currentTime().toString();

    Crud(int id, QString l,QString n,QString m,QString b_d,
         QString c_f,QString d_i,QString d_a, QString t_a):
        zk_id(id), lastname(l), name(n), mid_name(m),
        birth_date(b_d), check_for(c_f),
        dop_info(d_i),date_add(d_a), time_add(t_a)
    {
    }
    Crud(QString t_n): telephone_num(t_n)
    {
    }
    Crud(int id): zk_id(id)
    {
    }
    Crud(int id, QString l,QString n,QString m, QString c_f, QString d_i,
         QString a_r, QString a_l,
         QString r_c, QString r_s, QString r_h, QString r_cor, QString r_f):
        zk_id(id),lastname(l), name(n), mid_name(m), check_for(c_f), dop_info(d_i),
        adres_reg(a_r), adres_liv(a_l),
        reg_city(r_c),reg_street(r_s),reg_home(r_h),reg_corp(r_cor),reg_flat(r_f)

    { // Временный конструктор редактирования
    }
    Crud(QString l,QString n,QString m, QString b_d, QString c_f, QString d_i,
         QString a_r, QString a_l,
         QString r_c, QString r_s, QString r_h, QString r_cor, QString r_f):
        lastname(l), name(n), mid_name(m), birth_date(b_d), check_for(c_f), dop_info(d_i),
        adres_reg(a_r), adres_liv(a_l),
        reg_city(r_c),reg_street(r_s),reg_home(r_h),reg_corp(r_cor),reg_flat(r_f)
    {   //Временный конструктор добавления
    }

    Crud(int id,QString l,QString n,QString m_n, QString b_d,QString r_c, QString r_s, QString r_h, QString r_cor, QString r_f,
         QString d_a):
        zk_id(id),lastname(l), name(n), mid_name(m_n),
        birth_date(b_d),
        reg_city(r_c),reg_street(r_s),reg_home(r_h),reg_corp(r_cor),reg_flat(r_f),
        date_add(d_a)
    {   //Временный конструктор для поиска
    }

    Crud()
    {
    }

    void select_all();
    void check() const;
    void select_telephone();

    void refresh_table();
    void recieve_tel_list();
    void call_update_list();
    void update_zk();
    void add_zk();
    void del_zk();
    void recieve_new_zk_id();
    void id_zk_search();
    void get_max_zk();
    void get_min_zk();

    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQueryModel *model_2 = new QSqlQueryModel();
};

#endif // CRUD_H

