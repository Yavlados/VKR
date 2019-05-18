#ifndef _OFFICIAL_TEL_H
#define _OFFICIAL_TEL_H

#include "_Crud.h"

#include <QList>
#include <QString>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QVector>

class Official_tel: public Crud ///Для DbState
{
public:
    Official_tel();
    Official_tel(int,QString, QString, DbState);
    int of_t_id;
    DbState state;
    QString tel_num;
    QString service_name;

    static bool selectAll(QList<Official_tel*> *list);

};

#endif // _OFFICIAL_TEL_H
