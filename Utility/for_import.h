#ifndef FOR_IMPORT_H
#define FOR_IMPORT_H
#include "simplecrypt.h"
#include "enums.h"
#include <QFile>
#include <QDebug>
#include "Util.h"
#include <QXmlStreamReader>
#include "_Off_tels.h"
#include "_Event.h"
#include "olddbservice.h"
#include <QDir>

struct eventReaderCounter {
    eventReaderCounter();
    int eventIndex;
    int personIndex;
    int telephoneIndex;
};

struct importResult {
    importState state;
    QString message;
    QString filepath;
};

struct oldDbImportResult{
    Event *extractedEvent;
    importResult result;
};

class For_import
{
public:
    For_import();
    importResult openFile(QString filepath, QString password, bool isFolder, bool oldData);
private:
    QList<Off_tels*> *official_telephonesList;
    QList<Event*> *eventsList;

    QString decodeFileContent(QString fileContent, QString password);
    ExportType getDataType(QString fileContent);
    bool isFileEncoded(QString fileContent);

    bool getEventsList(QString fileContent);
    bool getOfficialsList(QString fileContent);

    void setEventFields(Event *e, QString field, QString data);
    void setPersonFields(Person *p,QString field, QString data);
    void setTelephoneFields(Telephone *t,QString field, QString data);
    void setContactFields(Contact *c,QString field, QString data);

    void setOfficialFields(Off_tels *off, QString field, QString data);

    bool uploadEvents();
    bool uploadOfficials();

    importResult importEventOrOfficial(QString filepath, QString password);
    importResult importOldFromFolder(QString filepath);
    oldDbImportResult importOld(QString filepath);
    oldDbImportResult readOldZk(QString filepath);


    bool testHeadFile(QByteArray *arr);
    Event* convertOldEventToNew(OldDbZk *zk);
    void handleOldTelephone(QList<Telephone *> *t, QString telephone);
    void handleOldTelephone(QList<Contact *> *t, OldDbAbonent *abonent);
};

#endif // FOR_IMPORT_H
