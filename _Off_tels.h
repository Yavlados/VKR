#ifndef _OFF_TELS_H
#define _OFF_TELS_H
#include "db_connection.h"

class Off_tels
{
public:
    Off_tels();
    Off_tels(int,QString,QString );

    int of_t_id;
    QString service_name;
    QString tel_num;

   static bool selectOffTel(QList<Off_tels*> *list);

};

#endif // _OFF_TELS_H
