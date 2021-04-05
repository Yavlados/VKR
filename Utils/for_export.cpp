#include "for_export.h"
For_export::For_export()
{
}

QString For_export::exportEvents(QList<Event*> *eventsList, SimpleCrypt crypt, ExportType type, QString filePath)
{
    // Events Encoding
    QString exportData;
    QXmlStreamWriter stream(&exportData);
    stream.writeStartDocument();
    stream.writeStartElement("Events");
    for (int a = 0; a < eventsList->size(); a++) {
        Event *ev = eventsList->at(a);
        QXmlStreamAttributes eventAttribs = this->getEventAttributes(ev);
        stream.writeStartElement("event");
            stream.writeAttributes(eventAttribs);

            for (int b =0; b<ev->persons()->size(); b++) {
                Person  *per = ev->persons()->at(b);
                QXmlStreamAttributes personAttribs = this->getPersonAttributes(per);

                stream.writeStartElement("person");
                stream.writeAttributes(personAttribs);
                for (int c=0; c<per->telephones()->size(); c++) {
                    Telephone *tel = per->telephones()->at(c);
                    QXmlStreamAttributes telAttribs = this->getTelephoneAttributes(tel);

                    stream.writeStartElement("telephone");
                    stream.writeAttributes(telAttribs);
                    for (int d=0; d<tel->cont()->size(); d++) {
                         Contact *cont = tel->cont()->at(d);
                         QXmlStreamAttributes contAttribs = this->getContactAttributes(cont);

                         stream.writeStartElement("contact");
                         stream.writeAttributes(contAttribs);
                         stream.writeEndElement();
                    }
                    stream.writeEndElement();
                }
                stream.writeEndElement();
            }
        stream.writeEndElement();
    }
    stream.writeEndElement();
    stream.writeEndDocument();
    // ----------------------
    QString exportDataPath = this->changeFileExtension(type, filePath);
    QFile exportDataFile(exportDataPath);
    QString exportEventData = type==events ? exportData : crypt.encryptToString(exportData);

    if (exportDataFile.open(QIODevice::WriteOnly)){
        QTextStream in(&exportDataFile);
        if(exportDataFile.isOpen())
            in << exportEventData;
    }
    exportDataFile.close();
    return exportDataPath;
}

QString For_export::exportOfficial(QList<Off_tels *> *officials, SimpleCrypt crypt, ExportType type, QString filePath)
{
    // Official telephones encoding
    QString exportData;
    QXmlStreamWriter stream(&exportData);
    stream.writeStartDocument();
    stream.writeStartElement("Official_telephones");
        for (int a =0; a<officials->size(); a++) {
            Off_tels* off = officials->at(a);
            stream.writeStartElement("official_telephone");
            stream.writeAttributes( this->getOfficialTelephoneAttributes(off));
            stream.writeEndElement();
        }
    stream.writeEndElement();
    stream.writeEndDocument();
    // ----------------------
    QString exportDataPath = this->changeFileExtension(type, filePath);
    QFile exportDataFile(exportDataPath);
    QString exportEventData = type==official ? exportData : crypt.encryptToString(exportData);
    if (exportDataFile.open(QIODevice::WriteOnly)){
        QTextStream in(&exportDataFile);
        if(exportDataFile.isOpen())
            in << exportEventData;
    }
    exportDataFile.close();
    return exportDataPath;
}

QString For_export::changeFileExtension(ExportType type, QString filePath)
{
    QString newExtension;
    QStringList pathList = filePath.split(".");
    switch(type){
    case events:
        newExtension = "nbd";
        break;
    case official:
        newExtension = "nbod";
        break;
     case eventsPassword:
        newExtension = "nbds";
        break;
    case officialPassword:
        newExtension = "nbods";
        break;
    }
    pathList[pathList.size()-1] = newExtension;
    QString path = pathList.join(".");
    return path;
}

QXmlStreamAttributes For_export::getEventAttributes(Event *ev)
{
    QXmlStreamAttributes attrib;

    attrib.append("id", ev->id);
    attrib.append("additional", ev->additional);
    attrib.append("keeping_place", ev->keeping_place);
    attrib.append("detention_by", ev->detention_by);
    attrib.append("detention_reason", ev->detention_reason);
    attrib.append("detention_time", ev->detention_time);
    attrib.append("detention_date", ev->detention_date);
    attrib.append("category", ev->category);

    return attrib;
}

QXmlStreamAttributes For_export::getPersonAttributes(Person *per)
{
    QXmlStreamAttributes attrib;

    attrib.append("id", per->id);
    attrib.append("alias", per->alias);
    attrib.append("midname", per->midname);
    attrib.append("name", per->name);
    attrib.append("lastname", per->lastname);

    return attrib;
}

QXmlStreamAttributes For_export::getTelephoneAttributes(Telephone *tel)
{
    QXmlStreamAttributes attrib;

    attrib.append("id", tel->id);
    attrib.append("oldnum", tel->oldnum ? "true" : "false");
    attrib.append("internum", tel->internum ? "true" : "false");
    attrib.append("person_id", tel->person_id);
    attrib.append("num", tel->num);

    return attrib;
}

QXmlStreamAttributes For_export::getContactAttributes(Contact *cont)
{
    QXmlStreamAttributes attrib;

    attrib.append("id", cont->id);
    attrib.append("oldnum", cont->oldnum ? "true" : "false");
    attrib.append("internum", cont->internum ? "true" : "false");
    attrib.append("telephone_id", cont->telephone_id);
    attrib.append("alias", cont->alias);
    attrib.append("number", cont->number);

    return attrib;
}

QXmlStreamAttributes For_export::getOfficialTelephoneAttributes(Off_tels *offtel)
{
    QXmlStreamAttributes attrib;
    attrib.append("id", QString::number(offtel->of_t_id) );
    attrib.append("name", offtel->service_name);
    attrib.append("num", offtel->tel_num);

     return attrib;
}




