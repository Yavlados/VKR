#ifndef FOR_EXPORT_H
#define FOR_EXPORT_H
#include "_Off_tels.h"
#include "list_master.h"
#include "_Zk_links.h"

#include <QString>
#include <QFile>
#include <QMessageBox>
#include <QObject>
#include <QDate>
#include <QFileInfo>
#include <QTextCodec>


/**
 * \file for_export.h
 * \brief Класс методов экспорта
*/

class For_export            /// Класс методов экспорта
{

public:
    For_export();

    bool Do_export(QString filename, QList<Crud*> *crud, QString password , bool cb_checked, bool cb_set_password, QList<Off_tels*> *offtel, zk_links *linklist);

    QString Create_report_filename(QString filename);

    List_master *list;

    QString info_text;
    QList<int> exported_zk_id;

    /// NEW
    void prepareExport();
private:
    QFile db_file;
    QFile export_report;
};

#endif // FOR_EXPORT_H
