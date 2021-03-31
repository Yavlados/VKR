#ifndef _OFF_TELS_H
#define _OFF_TELS_H
#include "db_connection.h"
/**
* \file _Off_tels.h
* \brief Класс сущности "Служебные телефоны"
*/
enum oft_st { Readed = 4,        ///< считан из БД
              Newing ,        ///< новый, еще не записан в БД
              Removed  ,     ///< удален в ПО, еще не удален в БД
              Changed
                };


class Off_tels          ///Класс сущности "Служебные телефоны"
{
public:
    Off_tels();
    Off_tels(int,QString,QString );

    int         of_t_id;
    QString     service_name;
    QString     tel_num;
    oft_st state;

   static bool search(QList<Off_tels *> *list, QString tel_num, QString name);
   static bool selectOffTel(QList<Off_tels*> *list);
   static bool add_off_tel(Off_tels * of_t);
   static bool del_off_tel (Off_tels * of_t);
   static bool update(QList<Off_tels*> *list);
   static QList<Off_tels *> *compare_with_base(QString query);
   static void clear_list(QList<Off_tels *> *list);
   static bool  del_off_tel_by_id(int id);
};

#endif // _OFF_TELS_H
