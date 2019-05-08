//ПРИБЕРИСЬ ТУТ, ТК ТЫ ПЕРЕДЕЛАЛ ТИП
//ОТОБРАЖАЕМЫХ ДАННЫХ В ТЕЙБЕЛ

#ifndef OWNERS_TEL_H
#define OWNERS_TEL_H
#include <QList>
#include <QString>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QMessageBox>
#include "crud.h"
#include <QDate>


class Owners_tel: public Crud
{
private:

    QList<QString> telephones;


public:

    Owners_tel()
{
}

    Owners_tel(int t_d): tel_id(t_d)
{
}
    Owners_tel(int id,QString l,QString n,QString m_n, QString b_d,
               QString r_c, QString r_s, QString r_h, QString r_cor,
               QString r_f, QString d_a, QString t_n, QString p_f, QString p_t):
     Crud(id,l, n, m_n, b_d, r_c, r_s, r_h, r_cor,r_f, d_a),
      tel_num(t_n), period_from(p_f),period_to(p_t)
{
        auto list = period_from.split('-');
        Date_From.setDate(list.at(0).toInt(),list.at(1).toInt(),list.at(2).toInt());
        list.clear();
        list = period_to.split('-');
        Date_To.setDate(list.at(0).toInt(),list.at(1).toInt(),list.at(2).toInt());
}

    QString tel_num;
    QString period_from, period_to;
    QDate Date_From, Date_To;

    int null_counter;

    int new_zk_id;

    void append_telephones(QString);

    void return_values() const;

    void add_numbers(int);

    void recieve_contacts(int);

    void recieve_tel_id(QString);

    void del_tel();

    void add_telephones();

    //Новые методы к новой модели

    void get_filter_for_add();

    void get_new_zk_id();

    void del_where_fk_null();

    void check_for_null(); //проверяю на нулл перед выходом из формы

    void zk_search();
    void zk_search_model(QString);
    void zk_search_report(QString);

    void check_tel_num(QString);

    int tel_id;

    QSqlQuery querry;
    QSqlQuery temp;

};

#endif // OWNERS_TEL_H
