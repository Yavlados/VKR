#ifndef _ZK_LINKS_H
#define _ZK_LINKS_H
#include <QString>
#include <QList>
#include <QSqlQuery>
#include "db_connection.h"
#include <QDebug>

class zk_links      ///Класс связей Записнх Книг
{
public:
    QList<zk_links*> *links ;
    zk_links();
    void take_links(QList<int> *exported_id);
    QString uuid1;
    QString uuid2;
};

#endif // _ZK_LINKS_H
