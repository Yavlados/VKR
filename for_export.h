#ifndef FOR_EXPORT_H
#define FOR_EXPORT_H
#include "_Off_tels.h"
#include "list_master.h"

#include <QString>
#include <QFile>
#include <QMessageBox>
#include <QObject>

/**
 * \file for_export.h
 * \brief Класс методов экспорта
*/

class For_export
{

public:
    For_export();

    bool Do_export(QString filename, QList<Crud*> *crud, QString password , bool cb_checked, bool cb_set_password, QList<Off_tels*> *offtel);

    List_master *list;
private:
    QFile db_file;
};

#endif // FOR_EXPORT_H
