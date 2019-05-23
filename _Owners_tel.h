//ПРИБЕРИСЬ ТУТ, ТК ТЫ ПЕРЕДЕЛАЛ ТИП
//ОТОБРАЖАЕМЫХ ДАННЫХ В ТЕЙБЕЛ

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


class Owners_tel:public Contacts
{
private:

public:

Owners_tel();
Owners_tel(int t_d);
Owners_tel(int, int, DbState);
Owners_tel(QString t_n, int t_id, int zk,DbState st = IsReaded);
~Owners_tel();

    int tel_id;
    QString tel_num;
    int parentZK_id;
    DbState state;

    QList<Contacts*> *cont = new QList<Contacts*>; /// У телефона - список контактов

    static bool selectAll(QList<Owners_tel*> *list);
    static bool saveAll(QList<Owners_tel*> *list);
    static bool selectZkTel(QList<Owners_tel*> *list, int);
    static bool selectZkTelForAdd(QList<Owners_tel*> *list, int);

     bool insert(bool setState=true);
     bool update(bool setState=true);
     bool remove();
///////////////////////////////////////////////////
    bool compare_with_base(QString tel_num);
///////////////////////////////////////////////////
    //Новые методы и экземпляры для ПОИСКА
     QDate Date_From, Date_To;

     bool stop = false;
     bool drop = false;

     QString Query; ///Строковый запрос для формы поиска
};

#endif // OWNERS_TEL_H
