#ifndef FOR_EXPORT_H
#define FOR_EXPORT_H
#include "_Off_tels.h"
#include "_Event.h"
#include "simplecrypt.h"

#include <QString>
#include <QFile>
#include <QMessageBox>
#include <QObject>
#include <QDate>
#include <QFileInfo>
#include <QTextCodec>
#include <QXmlStreamWriter>

/**
 * \file for_export.h
 * \brief Класс методов экспорта
*/

class For_export            /// Класс методов экспорта
{

public:
    For_export();

    QString info_text;
    QList<int> exported_zk_id;

    /// NEW
    QString exportEvents(QList<Event*> *events, SimpleCrypt crypt, ExportType type, QString filePath);
    QString exportOfficial(QList<Off_tels*> *officials, SimpleCrypt crypt, ExportType type, QString filePath);

    QString changeFileExtension(ExportType type, QString filePath);

    QXmlStreamAttributes getEventAttributes(Event* ev);
    QXmlStreamAttributes getPersonAttributes(Person *per);
    QXmlStreamAttributes getTelephoneAttributes(Telephone *tel);
    QXmlStreamAttributes getContactAttributes(Contact *cont);

    QXmlStreamAttributes getOfficialTelephoneAttributes(Off_tels *offtel);
private:
    QFile db_file;
    QFile export_report;
};

#endif // FOR_EXPORT_H
