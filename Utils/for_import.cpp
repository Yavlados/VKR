#include "for_import.h"

For_import::For_import()
{
    this->official_telephonesList = 0;
    this->eventsList = 0;
}

importResult For_import::openFile(QString filepath, QString password, bool isFolder, bool oldData)
{
    if(!isFolder && !oldData){
        return this->importEventOrOfficial(filepath,password);
    } else if(oldData){
        if(isFolder) return this->importOldFromFolder(filepath);
        else{
            oldDbImportResult res = this->importOld(filepath);
            if(res.extractedEvent == 0) return res.result;
            if(!Event::createEvent(res.extractedEvent)){
                res.result.state = uploadFileError;
                res.result.message = "Во время загрузки данных в базу возникли проблемы!";
            }
            return res.result;
        }
    }
}

QString For_import::decodeFileContent(QString fileContent, QString password)
{
   QString encodedPass = Util::instance()->convertKey(password);
   SimpleCrypt crypt(encodedPass.toLongLong());
   return crypt.decryptToString(fileContent);
}

ExportType For_import::getDataType(QString fileContent)
{
    QXmlStreamReader reader(fileContent);
    while(!reader.atEnd() && !reader.hasError()){
        QXmlStreamReader::TokenType token = reader.readNext();
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
        QXmlStreamReader::TokenType token = reader.readNext();
        if (token == QXmlStreamReader::StartDocument)
            continue;
        if (token == QXmlStreamReader::StartElement){
            if(reader.name()=="Events"){
                this->eventsList=new QList<Event*>();
            }

           if( reader.name() == "event"){
                Event *e = new Event();
                QXmlStreamAttributes attributes = reader.attributes();
                for(int a=0; a < attributes.size(); a++){
                    QXmlStreamAttribute attr = attributes.at(a);
                    this->setEventFields(e, attr.name().toString(), attr.value().toString());
                    }
                this->eventsList->append(e);
                counter.eventIndex = this->eventsList->length()-1;
                counter.personIndex = 0;
                counter.telephoneIndex = 0;
           }
           else if( reader.name() == "person"){
               Person *p = new Person();
               QXmlStreamAttributes attributes = reader.attributes();
               for(int a=0; a < attributes.size(); a++){
                   QXmlStreamAttribute attr = attributes.at(a);
                   this->setPersonFields(p, attr.name().toString(), attr.value().toString());
                   }
               Event *currentEvent = this->eventsList->at(counter.eventIndex);
               currentEvent->persons()->append(p);
               counter.personIndex = currentEvent->persons()->length()-1;
               counter.telephoneIndex = 0;
           }
           else if( reader.name() == "telephone"){
               Telephone *t = new Telephone();
               QXmlStreamAttributes attributes = reader.attributes();
               for(int a=0; a < attributes.size(); a++){
                   QXmlStreamAttribute attr = attributes.at(a);
                   this->setTelephoneFields(t, attr.name().toString(), attr.value().toString());
                   }
               Person *currentPerson = this->eventsList->at(counter.eventIndex)->persons()->at(counter.personIndex);
               currentPerson->telephones()->append(t);
               counter.telephoneIndex = currentPerson->telephones()->length()-1;
           }
           else if( reader.name() == "contact"){
               Contact *c = new Contact();
               QXmlStreamAttributes attributes = reader.attributes();
               for(int a=0; a < attributes.size(); a++){
                   QXmlStreamAttribute attr = attributes.at(a);
                   this->setContactFields(c, attr.name().toString(), attr.value().toString());
                   }
               Telephone *currentTelephone = this->eventsList->at(counter.eventIndex)->persons()->at(counter.personIndex)->telephones()->at(counter.telephoneIndex);
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
            QXmlStreamReader::TokenType token = reader.readNext();
            if (token == QXmlStreamReader::StartDocument)
                continue;
            if (token == QXmlStreamReader::StartElement){
                if(reader.name()=="Official_telephones"){
                    this->official_telephonesList=new QList<Off_tels*>();
                }
                if(reader.name()== "official_telephone"){
                    Off_tels *of = new Off_tels();
                    QXmlStreamAttributes attributes = reader.attributes();
                    for(int a=0; a < attributes.size(); a++){
                        QXmlStreamAttribute attr = attributes.at(a);
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
        Event *ev = this->eventsList->at(a);
        flag = Event::createEvent(ev);
        if(!flag) return flag;
     }
    return true;
}

bool For_import::uploadOfficials()
{
    bool flag = true;
     for(int a=0; a<this->official_telephonesList->size(); a++){
        Off_tels *of = this->official_telephonesList->at(a);
        flag = Off_tels::add_off_tel(of);
        if(!flag) return flag;
     }
     return true;
}

importResult For_import::importEventOrOfficial(QString filepath, QString password)
{
    importResult result;
    ExportType loadedDataType;
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
    file.close();
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

importResult For_import::importOldFromFolder(QString filepath)
{
    importResult importRes;
    importRes.message = "<style>th,td{padding:5px 5px;}</style><table><tr><th>#</th><th>Файл</th><th>Результат</th></tr>";
    importRes.state = success;
    QDir direcotry(filepath);
    QStringList filelist = direcotry.entryList(QStringList("*"), QDir::Files);
    QList<oldDbImportResult> list;

    foreach (QString file, filelist )
    {
        oldDbImportResult result = this->importOld(filepath+"/"+file);
        list.append(result);
    }
    for (int a = 0; a<list.size(); a++) {
        oldDbImportResult res = list.at(a);
        if(res.extractedEvent != 0) {
            if(!Event::createEvent(res.extractedEvent)){
                res.result.state = uploadFileError;
                res.result.message = "Во время загрузки данных в базу возникли проблемы!";
            }
        }
        importRes.message += "<tr><td>"+QString::number(a+1)+"</td><td>"+res.result.filepath+"</td><td>"+res.result.message+"</td></tr>";
    }
    importRes.message += "</table>";
    return importRes;
}

oldDbImportResult For_import::importOld(QString filepath)
{
    oldDbImportResult importMeta = this->readOldZk(filepath);
    importMeta.result.filepath = filepath;
    return importMeta;
}

oldDbImportResult For_import::readOldZk(QString filepath)
{
    oldDbImportResult responce;
    importResult result;
    ExportType loadedDataType;
    QFile file(filepath);
    QString fileContent = "";
    if(!file.open(QIODevice::ReadOnly | QIODevice::Unbuffered)){
        qDebug() << "Cannot read file!";
        result.state = readFileError;
        result.message = "Системная ошибка: Программа не может прочитать файл "+filepath+", проверьте доступ. Импорт был прерван.";
        responce.result = result    ;
        responce.extractedEvent = 0;
        return responce;
    }

    QByteArray arr=file.readAll();
    if(arr.count()==0)
    {
        result.state = readFileError;
        result.message = "Файл либо не подходит под формат old_db, либо пуст!";
        responce.result = result    ;
        responce.extractedEvent = 0;
        return responce;
    }

    if( !this->testHeadFile(&arr) ||
            arr[0xC2]==0 )
    {
        result.state = readFileError;
        result.message ="Файл не подходит под формат old_db";
        responce.result = result    ;
        responce.extractedEvent = 0;
        return responce;
    }else
    {
        OldDbService *service;
        OldDbZk *zk = new OldDbZk();
        int pos = 0xC2;
        if (service->Open_Old_db(zk,pos,arr))
        {
              Event *e = this->convertOldEventToNew(zk);
              file.close();
              result.state = success;
              result.message ="Данные из файла успешно импортированы в базу данных";
              responce.result = result    ;
              responce.extractedEvent = e;
              return responce;
        }

    }
}

bool For_import::testHeadFile(QByteArray *arr)
{
    return arr->mid(3,6)=="prolog";
}

Event* For_import::convertOldEventToNew(OldDbZk *zk)
{
    Event *ev = new Event();
    ev->category = zk->category;
    ev->detention_date = zk->data_zad;
    ev->keeping_place = zk->keep_place;
    ev->detention_reason = zk->povod_zad;
    ev->additional = zk->person.dopinfo;

    // TODO: add new fields: address, dopInfo
    Person *p = new Person();
    p->name = zk->person.imya;
    p->lastname = zk->person.famil;
    p->midname = zk->person.otchestvo;
    p->alias = zk->person.klichka;

    this->handleOldTelephone( p->telephones(), zk->person.tel1);
    this->handleOldTelephone( p->telephones(), zk->person.tel2);
    this->handleOldTelephone( p->telephones(), zk->person.tel3);
    this->handleOldTelephone( p->telephones(), zk->person.tel4);
    this->handleOldTelephone( p->telephones(), zk->person.tel5);

    if(p->telephones()->size() > 0){
        Telephone *telephone = p->telephones()->at(0);
        for (int a =0; a<zk->contacts.size(); a++) {
            this->handleOldTelephone(telephone->cont(), zk->contacts.at(a));
        }
    }
    ev->persons()->append(p);
    return ev;
}

void For_import::handleOldTelephone(QList<Telephone *> *t, QString telephone)
{
    if(telephone.isEmpty()) return;

    Telephone *tel = new Telephone();
    tel->num = telephone;

    t->append(tel);
}

void For_import::handleOldTelephone(QList<Contact *> *t, OldDbAbonent *abonent)
{
    Contact *cnt = new Contact;
    cnt->number = abonent->tel1;
    cnt->alias = abonent->famil+" "+abonent->imya+" "+abonent->otchestvo+" \""+abonent->klichka+"\"";
    t->append(cnt);
}

eventReaderCounter::eventReaderCounter()
{
    this->eventIndex = 0;
    this->personIndex = 0;
    this->telephoneIndex = 0;
}



