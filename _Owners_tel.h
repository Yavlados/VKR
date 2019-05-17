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
#include "_Crud.h"
#include <QDate>


class Owners_tel: public Crud
{
private:

    QList<QString> telephones;


public:

Owners_tel();
Owners_tel(int t_d);
Owners_tel(int, int, DbState);
Owners_tel(int id,QString l,QString n,QString m_n, QString b_d,
               QString r_c, QString r_s, QString r_h, QString r_cor,
               QString r_f, QString d_a, QString t_n);
Owners_tel(QString t_n, int t_id, int zk,DbState st = IsReaded);
~Owners_tel();
    int tel_id;
    QString tel_num;
    int parentZK_id;
    DbState state;

    static bool selectAll(QList<Owners_tel*> *list);
    static bool saveAll(QList<Owners_tel*> *list);
    static bool selectZkTel(QList<Owners_tel*> *list, int);
    static bool selectZkTelForAdd(QList<Owners_tel*> *list, int);

     bool insert(bool setState=true);
     bool update(bool setState=true);
     bool remove();

///////////////////////////////////////////////////
    //Новые методы и экземпляры для ПОИСКА
     QDate Date_From, Date_To;

     bool stop = false;
     bool drop = false;

    void zk_search();
    void zk_search_model(QString);
    void zk_search_report(QString);

    void check_tel_num(QString);

    QSqlQuery querry;
    QSqlQuery temp;

signals:
    void insert_error();
};

#endif // OWNERS_TEL_H
