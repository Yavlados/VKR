#ifndef CRUD_H
#define CRUD_H
#include "_Owners_tel.h"///->_Contacts.h->db_connrction

#include <QString>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSqlQueryModel>
#include <QDate>
#include <QTime>
#include <QMessageBox>


enum CheckState { Checked =1,
                 Unchecked = 0};
class Crud: public Owners_tel
{

public:
    Crud();
    Crud(int id);
    Crud(QString t_n);

    QList<Owners_tel*> *owt = new QList<Owners_tel*>; ///У ЗК есть список телефонов

    CheckState checkState_;
    DbState state;
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

 /////////////////////////////////////////////////////////////
    bool selectAll(QList<Crud*> *list);
    bool select_search(QList<Crud*> *list, QString);
 /////////////////////////////////////////////////////////////

    void select_all();
    bool selectAllDb(QList<Crud*> *list, QList<Owners_tel*> *otlist, QList<Contacts*> *contlist);
    void check() const;
    ///Методы поиска
    void zk_search();
    void zk_search_model(QString qry);
    void zk_search_report(QString qry);

    void call_update_list();
    bool update_zk();
    bool add_zk();
    void del_zk(int);
    void recieve_new_zk_id();
    void id_zk_search();
    void get_max_zk();
    void get_min_zk();
    int get_id_from_tel(QString t_n);


    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQueryModel *model_2 = new QSqlQueryModel();

private:

    //ТАБЛИЦА 2
    QString telephone_num;

    //Переменны для запросо
signals:
    //void Send_search(QString);
};

#endif // CRUD_H

