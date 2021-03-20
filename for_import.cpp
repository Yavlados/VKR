#include "for_import.h"

For_import::For_import()
{
    this->official_telephonesList = 0;
    this->eventsList = 0;
}

importResult For_import::openFile(QString filepath, QString password, bool isFolder)
{
    importResult result;
    ExportType loadedDataType;
    if(!isFolder){
        QFile file(filepath);
        QString fileContent = "";
        if(!file.open(QIODevice::ReadOnly)){
            qDebug() << "Cannot read file!";
            result.state = readFileError;
            result.message = "Системная ошибка: Программа не может прочитать файл "+filepath+", проверьте доступ. Импорт был прерван.";
            return result;
        }else{
            //--- reading file
           QTextStream in(&file);
           while (!in.atEnd()) {
                   QString line = in.readLine();
                   fileContent += line;
               }
            // ---
           if(isFileEncoded(fileContent)){
                fileContent =this->decodeFileContent(fileContent, password);
                if(fileContent.isEmpty()) {
                    qDebug() << "Wrong password!";
                    result.state = wrongPassword;
                    result.message = "Вы ввели неверный пароль. Импорт из файла "+filepath+" был прерван.";
                    return result;
                }
             }
            loadedDataType = this->getDataType(fileContent);
            bool importFlag = false;
            switch (loadedDataType){
            case events:
                importFlag = this->getEventsList(fileContent);
                break;
            case official:
                importFlag = this->getOfficialsList(fileContent);
                break;
            }
            if(!importFlag){
                qDebug() << "Some errors appears";
                result.state = readFileError;
                result.message = "Во время чтения файла "+filepath+" возникли ошибки. Импорт был прерван.";
                return result;
            }
            switch (loadedDataType){
            case events:
                importFlag = this->uploadEvents();
                break;
            case official:
                importFlag = this->uploadOfficials();
                break;
            }
            if(!importFlag){ qDebug() << "Some errors appears";
                result.state = uploadFileError;
                result.message = "Ошибка загрузки данных в базу. Импорт был прерван.";
                return result;
            }
        }
        QString fileType;
        int importedRowsCount;
        switch(loadedDataType){
        case events:
            fileType = "данные о событиях";
            importedRowsCount = this->eventsList->size();
            break;
         case official:
            fileType = "данные о служебных телефонах";
            importedRowsCount = this->official_telephonesList->size();
            break;
        }
        if(this->eventsList != 0){
            delete this->eventsList;
            this->eventsList = 0;
        }
        if(this->official_telephonesList != 0){
             delete this->official_telephonesList;
            this->official_telephonesList = 0;
        }

        result.state = success;
        result.message ="Файл "+filepath+" содержал "+ fileType + ",  успешно импортировано "+QString::number(importedRowsCount)+" записей";
        return result;
    }
}

QString For_import::decodeFileContent(QString fileContent, QString password)
{
   auto encodedPass = Util::instance()->convertKey(password);
   SimpleCrypt crypt(encodedPass.toLongLong());
   return crypt.decryptToString(fileContent);
}

ExportType For_import::getDataType(QString fileContent)
{
    QXmlStreamReader reader(fileContent);
    while(!reader.atEnd() && !reader.hasError()){
        auto token = reader.readNext();
        if (token == QXmlStreamReader::StartDocument)
            continue;
        if (token == QXmlStreamReader::StartElement){
           if( reader.name() == "Official_telephones") return official;
           else if( reader.name() == "Events") return events;
           else return wrong;
        }
    }
}

bool For_import::isFileEncoded(QString fileContent)
{
    return !fileContent.startsWith("<?xml version");
}

bool For_import::getEventsList(QString fileContent)
{   eventReaderCounter counter;

    QXmlStreamReader reader(fileContent);
    while(!reader.atEnd() && !reader.hasError()){
        auto token = reader.readNext();
        if (token == QXmlStreamReader::StartDocument)
            continue;
        if (token == QXmlStreamReader::StartElement){
            if(reader.name()=="Events"){
                this->eventsList=new QList<Event*>();
            }

           if( reader.name() == "event"){
                Event *e = new Event();
                auto attributes = reader.attributes();
                for(int a=0; a < attributes.size(); a++){
                    auto attr = attributes.at(a);
                    this->setEventFields(e, attr.name().toString(), attr.value().toString());
                    }
                this->eventsList->append(e);
                counter.eventIndex = this->eventsList->length()-1;
                counter.personIndex = 0;
                counter.telephoneIndex = 0;
           }
           else if( reader.name() == "person"){
               Person *p = new Person();
               auto attributes = reader.attributes();
               for(int a=0; a < attributes.size(); a++){
                   auto attr = attributes.at(a);
                   this->setPersonFields(p, attr.name().toString(), attr.value().toString());
                   }
               auto currentEvent = this->eventsList->at(counter.eventIndex);
               currentEvent->persons()->append(p);
               counter.personIndex = currentEvent->persons()->length()-1;
               counter.telephoneIndex = 0;
           }
           else if( reader.name() == "telephone"){
               Telephone *t = new Telephone();
               auto attributes = reader.attributes();
               for(int a=0; a < attributes.size(); a++){
                   auto attr = attributes.at(a);
                   this->setTelephoneFields(t, attr.name().toString(), attr.value().toString());
                   }
               auto currentPerson = this->eventsList->at(counter.eventIndex)->persons()->at(counter.personIndex);
               currentPerson->telephones()->append(t);
               counter.telephoneIndex = currentPerson->telephones()->length()-1;
           }
           else if( reader.name() == "contact"){
               Contact *c = new Contact();
               auto attributes = reader.attributes();
               for(int a=0; a < attributes.size(); a++){
                   auto attr = attributes.at(a);
                   this->setContactFields(c, attr.name().toString(), attr.value().toString());
                   }
               auto currentTelephone = this->eventsList->at(counter.eventIndex)->persons()->at(counter.personIndex)->telephones()->at(counter.telephoneIndex);
               currentTelephone->cont()->append(c);
           }
        }
    }

    return !reader.hasError();

}

bool For_import::getOfficialsList(QString fileContent)
{
    eventReaderCounter counter;

        QXmlStreamReader reader(fileContent);
        while(!reader.atEnd() && !reader.hasError()){
            auto token = reader.readNext();
            if (token == QXmlStreamReader::StartDocument)
                continue;
            if (token == QXmlStreamReader::StartElement){
                if(reader.name()=="Official_telephones"){
                    this->official_telephonesList=new QList<Off_tels*>();
                }
                if(reader.name()== "official_telephone"){
                    Off_tels *of = new Off_tels();
                    auto attributes = reader.attributes();
                    for(int a=0; a < attributes.size(); a++){
                        auto attr = attributes.at(a);
                        this->setOfficialFields(of, attr.name().toString(), attr.value().toString());
                        }
                    this->official_telephonesList->append(of);
                }
            }
        }
    return !reader.hasError();
}

void For_import::setEventFields(Event *e, QString field, QString data)
{
    if(field == "id"){
         e->id = data;
    } else if(field == "additional"){
         e->additional = data;
    }else if(field == "keeping_place"){
         e->keeping_place = data;
    }else if(field == "detention_by"){
         e->detention_by = data;
    }else if(field == "detention_reason"){
         e->detention_reason = data;
    }else if(field == "detention_time"){
         e->detention_time = data;
    }else if(field == "detention_date"){
         e->detention_date = data;
    }else if(field == "category"){
         e->category = data;
    }
}

void For_import::setPersonFields(Person *p, QString field, QString data)
{
    if(field == "id"){
         p->id = data;
    } else if(field == "alias"){
         p->alias = data;
    }else if(field == "midname"){
        p->midname = data;
   }else if(field == "name"){
        p->name = data;
   }else if(field == "lastname"){
        p->lastname = data;
   }
}

void For_import::setTelephoneFields(Telephone *t, QString field, QString data)
{
    if(field == "id"){
         t->id = data;
    }else if(field == "oldnum"){
        t->oldnum = data == "true";
   }else if(field == "internum"){
        t->internum = data == "true";
   }else if(field == "person_id"){
        t->person_id = data;
   }else if(field == "num"){
        t->num = data;
   }
}

void For_import::setContactFields(Contact *c, QString field, QString data)
{
    if(field == "id"){
         c->id = data;
    }else if(field == "oldnum"){
        c->oldnum = data == "true";
   }else if(field == "internum"){
        c->internum = data == "true";
   }else if(field == "telephone_id"){
        c->telephone_id = data;
   }else if(field == "alias"){
        c->alias = data;
   }else if(field == "number"){
        c->number = data;
    }
}

void For_import::setOfficialFields(Off_tels *off, QString field, QString data)
{
    if(field == "id"){
         off->of_t_id = data.toInt();
    }else if(field == "name"){
        off->service_name = data;
   }else if(field == "num"){
        off->tel_num = data;
   }
}

bool For_import::uploadEvents()
{
    bool flag = true;
     for(int a=0; a<this->eventsList->size(); a++){
        auto ev = this->eventsList->at(a);
        flag = Event::createEvent(ev);
        if(!flag) return flag;
     }
    return true;
}

bool For_import::uploadOfficials()
{
    bool flag = true;
     for(int a=0; a<this->official_telephonesList->size(); a++){
        auto of = this->official_telephonesList->at(a);
        flag = Off_tels::add_off_tel(of);
        if(!flag) return flag;
     }
    return true;
}

eventReaderCounter::eventReaderCounter()
{
    this->eventIndex = 0;
    this->personIndex = 0;
    this->telephoneIndex = 0;
}


