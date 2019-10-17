
#ifndef OWNERS_TEL_H
#define OWNERS_TEL_H
#include "_Contacts.h"

#include <QList>
#include <QString>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QMessageBox>
#include <QDate>
/**
 * \file _Owners_tel.h
 * \brief Класс сущности "Телефон"
*/

class Owners_tel            ///Класс сущности "Телефон"
{

public:

Owners_tel();
Owners_tel(int t_d);
Owners_tel(int id, int zk, bool i_m, DbState);
Owners_tel(QString t_n, int t_id, int zk, bool i_m = false, bool o_n = false, DbState st = IsReaded);
~Owners_tel();

    int tel_id;
    QString tel_num;
    int parentZK_id;
    bool internum;
    bool oldnum;
    DbState state;

    QList<Contacts*> *cont();

    static bool saveAll(QList<Owners_tel*> *list);
    static bool selectZkTelForAdd(QList<Owners_tel*> *list, int);

    ////////////////////////////////////////////////

    static QList<Owners_tel*> *get_ow_list(int zk_id);

    /////////////////////////////////////////////

     int insert_tel(bool setState=true, int zk_id = 0);
     bool update_tel(bool setState=true);
     bool remove_tel();
///////////////////////////////////////////////////
    static QList<int> compare_nums(QList<Owners_tel*> *owt1, QList<Owners_tel*> *owt2);
///////////////////////////////////////////////////

private:

    QList<Contacts*> *_cont; /// У телефона - список контактов

};

#endif // OWNERS_TEL_H
