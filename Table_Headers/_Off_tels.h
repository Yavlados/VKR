#ifndef _OFF_TELS_H
#define _OFF_TELS_H
#include "db_connection.h"
/**
* \file _Off_tels.h
* \brief Класс сущности "Служебные телефоны"
*/
class Off_tels          ///Класс сущности "Служебные телефоны"
{
public:
    Off_tels();
    Off_tels(int,QString,QString );

    int of_t_id;
    QString service_name;
    QString tel_num;

   static bool search(QList<Off_tels *> *list, QString tel_num, QString name);
   static bool selectOffTel(QList<Off_tels*> *list);
   static bool add_off_tel(Off_tels * of_t);
   static bool del_off_tel (Off_tels * of_t);
};

#endif // _OFF_TELS_H
