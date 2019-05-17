#ifndef CRUD_H
#define CRUD_H

#include "db_connection.h"

#include <QString>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSqlQueryModel>
#include <QDate>
#include <QTime>
#include <QMessageBox>

enum DbState { IsReaded = 1,        ///< считан из БД
               IsNewing = 0,        ///< новый, еще не записан в БД
               IsRemoved = -1 ,     ///< удален в ПО, еще не удален в БД
               IsChanged = 2};       ///< считан из БД и изменен в ПО

class Crud{

public:
    Crud();
    Crud(int id);
    Crud(QString t_n);
    Crud(int id, QString l,QString n,QString m,QString b_d,QString c_f,QString d_i,QString d_a, QString t_a);

    /// Временный конструктор редактирования
    Crud(int id, QString l,QString n,QString m, QString c_f, QString d_i,
         QString a_r, QString a_l,
         QString r_c, QString r_s, QString r_h, QString r_cor, QString r_f);

    ///Временный конструктор добавления
    Crud(QString l,QString n,QString m, QString b_d, QString c_f, QString d_i,
         QString a_r, QString a_l,
         QString r_c, QString r_s, QString r_h, QString r_cor, QString r_f);

    ///Временный конструктор для поиска
    Crud(int id,QString l,QString n,QString m_n, QString b_d,QString r_c, QString r_s, QString r_h, QString r_cor, QString r_f,
         QString d_a);

    QString search_res;
    bool search_is_ready, model_is_ready;
    QMessageBox msgbx;
    QString search_result;

    // ТАБЛИЦА 1
    int zk_id;
    int new_zk_id;
    QString lastname;
    QString name;
    QString mid_name;
    QString birth_date;
    QString check_for;
    QString dop_info;

    QString adres_reg;
    QString adres_liv;

    //Адрес регистрации
    QString reg_city;
    QString reg_street;
    QString reg_home;
    QString reg_corp;
    QString reg_flat;

    //Адрес проживания
    QString liv_city;
    QString liv_street;
    QString liv_home;
    QString liv_corp;
    QString liv_flat;

    //Дата и время сейчас
    QString date_add = QDate::currentDate().toString(Qt::ISODate);
    QString time_add = QTime::currentTime().toString();

    void select_all();
    void check() const;

    void refresh_table();
    void recieve_tel_list();
    void call_update_list();
    bool update_zk();
    bool add_zk();
    void del_zk(int);
    void recieve_new_zk_id();
    void id_zk_search();
    void get_max_zk();
    void get_min_zk();
    int get_id_from_tel(QString);

    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQueryModel *model_2 = new QSqlQueryModel();

private:

    //ТАБЛИЦА 2
    QString telephone_num;

    //Переменны для запросов
    QSqlQuery temp;
    QSqlQuery querry;
    QSqlQuery temp_2; // для метода даления

signals:
    //void Send_search(QString);
};

#endif // CRUD_H

