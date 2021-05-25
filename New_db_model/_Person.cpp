#include "_Person.h"

Person::Person()
{
    this->_telephones = 0;
    this->lastname    ="";
    this->name        ="";
    this->midname     ="";
    this->alias       ="";
    this->id          ="";
    this->state       =IsNewing;
    this->hash        ="";
    this->linked_persons = 0;
    this->type           = dbPerson;
    this->_linked_events = 0;
}

Person::~Person()
{
    if (this->_telephones == 0)
    {
       return;
    }
    delete this->_telephones;
}

QList<Telephone *> *Person::telephones()
{
    if (this->_telephones == 0)
    {
        this->_telephones = new QList<Telephone*>;
    }
    return this->_telephones;
}

QSet<QString> *Person::linked_events()
{
    if (this->_linked_events == 0)
    {
        this->_linked_events = new QSet<QString>;
    }
    return this->_linked_events;
}

void Person::setTelephones(QList<Telephone *> *newTelephones)
{
    this->_telephones = newTelephones;
}

bool Person::selectByEventId(QList<Person *> *personsList, QString eventId)
{
    if(personsList==0)
        return false;

    if( !db_connection::instance()->db_connect() )
        return false;

//    qDeleteAll(*personsList);
    personsList->clear();

    QString cname = db_connection::instance()->db().connectionName();
    bool isOk = db_connection::instance()->db().database(cname).transaction();

    QSqlQuery temp(db_connection::instance()->db());
    temp.prepare("SELECT "
                 " p.name, "
                 " p.midname,"
                 " p.lastname,"
                 " p.alias, "
                 " p.id, "
                 " p.country,"
                 " p.city,"
                 " p.street,"
                 " p.house,"
                 " p.corp,"
                 " p.flat,"
                 " p.hash"
                 " FROM notebook2.person as p, "
                 " notebook2.event_person "
                 " WHERE event_person.person_id = p.id "
                 " AND event_person.event_id = (:event_id)");
     temp.bindValue(":event_id", eventId);
     if (!temp.exec())
     {
         qDebug()  << "Person::selectByEventId"<< temp.lastError() << temp.executedQuery();
         db_connection::instance()->lastError = "Person::selectByEventId " + temp.lastError().text();
         db_connection::instance()->db().database(cname).rollback();

         isOk = false;
     }

     while (temp.next())
     {
         Person *person = new Person();
         person->name = temp.value(0).toString();
         person->midname = temp.value(1).toString();
         person->lastname = temp.value(2).toString();
         person->alias = temp.value(3).toString();
         person->id = temp.value(4).toString();
         person->state = IsReaded;

         // Adress
         person->country.setData(temp.value(5).toString());
         person->city.setData(temp.value(6).toString());
         person->street.setData(temp.value(7).toString());
         person->house.setData(temp.value(8).toString());
         person->corp.setData(temp.value(9).toString());
         person->flat.setData(temp.value(10).toString());

         // Hash
         person->hash = temp.value(11).toString();
         if(!Person::getLinkedPersons(person) )  return false;
         personsList->append(person);

     }

     if(personsList->isEmpty()){
         return false;
     }
     db_connection::instance()->db().database(cname).commit();

     return true;
}

bool Person::updatePerson(Person *person)
{
    QString cname = db_connection::instance()->db().connectionName();

    bool isOk = db_connection::instance()->db().database(cname).transaction();
    QSqlQuery temp(db_connection::instance()->db());
    temp.prepare("UPDATE notebook2.person "
                 " SET lastname = (:lastname),"
                      " name = (:name),"
                      " midname = (:midname),"
                      " alias = (:alias)    , "
                      " country =   ROW((:liv_country), (:reg_country)),"
                      " city =      ROW((:liv_city), (:reg_city)),"
                      " street =    ROW((:liv_street), (:reg_street)),"
                      " house =     ROW((:liv_house), (:reg_house)),"
                      " corp =      ROW((:liv_corp), (:reg_corp)),"
                      " flat =      ROW((:liv_flat), (:reg_flat))"
                      " WHERE id = (:id) ");

     temp.bindValue(":lastname", person->lastname);
     temp.bindValue(":name", person->name);
     temp.bindValue(":midname", person->midname);
     temp.bindValue(":alias", person->alias);
     temp.bindValue(":id", person->id.toInt());

     // Adress
     temp.bindValue(":liv_country", person->country.liv);
     temp.bindValue(":reg_country", person->country.reg);

     temp.bindValue(":liv_city", person->city.liv);
     temp.bindValue(":reg_city", person->city.reg);

     temp.bindValue(":liv_street", person->street.liv);
     temp.bindValue(":reg_street", person->street.reg);

     temp.bindValue(":liv_house", person->house.liv);
     temp.bindValue(":reg_house", person->house.reg);

     temp.bindValue(":liv_corp", person->corp.liv);
     temp.bindValue(":reg_corp", person->corp.reg);

     temp.bindValue(":liv_flat", person->flat.liv);
     temp.bindValue(":reg_flat", person->flat.reg);

     if (!temp.exec())
     {
         qDebug()  << "Person::updatePerson"<< temp.lastError() << temp.executedQuery();
         db_connection::instance()->lastError = "Person::updatePerson " + temp.lastError().text();

         db_connection::instance()->db().database(cname).rollback();

         isOk = false;
     } else {

         if(!Person::handleTelephones(person->telephones(), person->id)){
             db_connection::instance()->db().database(cname).rollback();
             return false;
         }

//         for( int i=0; i<person->telephones()->size(); i++){
//             Telephone *tel = person->telephones()->at(i);
//             bool a = false;
//             switch (tel->state) {
//                case IsNewing:
//                 a = Telephone::createTelephone(tel, person->id);
//                 break;
//             case IsChanged:
//                 a = Telephone::updateTelephone(tel);
//                 break;
//             case IsRemoved:
//                 a = Telephone::deleteTelephone(tel);
//                 break;
//             case IsReaded:
//                 a=true;
//                 break;
//             }

//             if(!a){
//                 db_connection::instance()->db().database(cname).rollback();
//                 isOk = false;
//                 return isOk;
//             }
//         }

         isOk = true;
         db_connection::instance()->db().database(cname).commit();
         return true;
     }
     return isOk;
}

bool Person::createPerson(Person *person, QString eventId="-1")
{
    if( !db_connection::instance()->db_connect() )
        return false;

    QString cname = db_connection::instance()->db().connectionName();

    bool isOk = db_connection::instance()->db().database(cname).transaction();
    QSqlQuery temp(db_connection::instance()->db());

    temp.prepare("INSERT INTO notebook2.person ( "
                "lastname,"
                 "name,"
                 "midname, "
                 "alias, "
                 "country, "
                 "city, "
                 "street, "
                 "house, "
                 "corp, "
                 "flat,"
                 "hash )"
                 " VALUES ( (:lastname), (:name), (:midname),  (:alias),"
                 " ROW((:liv_country), (:reg_country)),"
                 " ROW((:liv_city), (:reg_city)),"
                 " ROW((:liv_street), (:reg_street)),"
                 " ROW((:liv_house), (:reg_house)),"
                 " ROW((:liv_corp), (:reg_corp)),"
                 " ROW((:liv_flat), (:reg_flat)),"
                 " uuid_generate_v1()"
                 ")"
                 " RETURNING id, hash" );
    temp.bindValue(":lastname", person->lastname);
    temp.bindValue(":name",     person->name);
    temp.bindValue(":midname",  person->midname);
    temp.bindValue(":alias",    person->alias);

    // Adress
    temp.bindValue(":liv_country", person->country.liv);
    temp.bindValue(":reg_country", person->country.reg);

    temp.bindValue(":liv_city", person->city.liv);
    temp.bindValue(":reg_city", person->city.reg);

    temp.bindValue(":liv_street", person->street.liv);
    temp.bindValue(":reg_street", person->street.reg);

    temp.bindValue(":liv_house", person->house.liv);
    temp.bindValue(":reg_house", person->house.reg);

    temp.bindValue(":liv_corp", person->corp.liv);
    temp.bindValue(":reg_corp", person->corp.reg);

    temp.bindValue(":liv_flat", person->flat.liv);
    temp.bindValue(":reg_flat", person->flat.reg);

    if (!temp.exec())
    {
         db_connection::instance()->lastError = "Person::createPerson " + temp.lastError().text();
        qDebug() << "Person::createPerson" << temp.lastError();
        db_connection::instance()->db().database(cname).rollback();

        return false;
    }

    while (temp.next())
    {
        person->id = temp.value(0).toString();
        person->hash = temp.value(1).toString();

        if(Person::handleTelephones(person->telephones(), person->id)){
            if(eventId != "-1")
                if(!Person::linkEventPerson(eventId, person->id)){
                    db_connection::instance()->db().database(cname).rollback();
                    return false;
                }

        } else {
            db_connection::instance()->db().database(cname).rollback();
            return false;
        }

         if(!Person::addHashRowToLinksTable(person)){
            db_connection::instance()->db().database(cname).rollback();
            return false;
        }
    }


    db_connection::instance()->db().database(cname).commit();
    return true;
}

bool Person::deletePerson(Person *person)
{
    QString cname = db_connection::instance()->db().connectionName();

    bool isOk = db_connection::instance()->db().database(cname).transaction();
    QSqlQuery temp(db_connection::instance()->db());
    temp.prepare("DELETE FROM notebook2.person WHERE id=(:id)");

    temp.bindValue(":id", person->id.toInt());

    if (!temp.exec())
    {
        qDebug()  << "Person::deletePerson"<< temp.lastError() << temp.executedQuery();
       db_connection::instance()->lastError = "Person::deletePerson " + temp.lastError().text();
        db_connection::instance()->db().database(cname).rollback();
        return false ;
    } else{
        if(Person::unlinkPersonOnDelete(person)){
            db_connection::instance()->db().database(cname).commit();
            return true;
        }
        else {
            db_connection::instance()->db().database(cname).rollback();
            return false;
        }
    }
}

bool Person::handleTelephones(QList<Telephone *> *telephones, QString personId)
{
    for( int i=0; i< telephones->size(); i++){
        Telephone *tel = telephones->at(i);
        bool a = false;
        switch (tel->state) {
           case IsNewing:
            a = Telephone::createTelephone(tel, personId);
            break;
        case IsChanged:
            a = Telephone::updateTelephone(tel);
            break;
        case IsRemoved:
            a = Telephone::deleteTelephone(tel);
            break;
        case IsReaded:
            a=Telephone::handleContacts(tel->cont(), tel->id);
            break;
        }

        if(!a){
            return a;
        }
    }
    return true;
}

bool Person::linkEventPerson(QString eventId, QString personId)
{
    if( !db_connection::instance()->db_connect() )
        return false;

    QString cname = db_connection::instance()->db().connectionName();

    bool isOk = db_connection::instance()->db().database(cname).transaction();
    QSqlQuery temp(db_connection::instance()->db());

    temp.prepare("INSERT INTO notebook2.event_person ( "
                     "event_id,"
                     "person_id)"
                 " VALUES ( (:event_id), (:person_id))");
    temp.bindValue(":event_id", eventId);
    temp.bindValue(":person_id",personId);

    if (!temp.exec())
    {
        db_connection::instance()->lastError = "Person::linkEventPerson " + temp.lastError().text();
        qDebug() << "Person::linkEventPerson" << temp.lastError();
        db_connection::instance()->db().database(cname).rollback();
        return false;
    }else {
//        db_connection::instance()->db().database(cname).commit();
        return true;
    }
}

bool Person::addHashRowToLinksTable(Person *person)
{
    if( !db_connection::instance()->db_connect() )
        return false;

    QString cname = db_connection::instance()->db().connectionName();

    bool isOk = db_connection::instance()->db().database(cname).transaction();
    QSqlQuery temp(db_connection::instance()->db());

    temp.prepare("INSERT INTO notebook2.person_links ( "
                 " hash,"
                 " links )"
                 " VALUES ( (:hash), ARRAY[]::uuid[] )" );
    temp.bindValue(":hash", person->hash);
    if (!temp.exec())
    {
         db_connection::instance()->lastError = "Person::addHashRowToLinksTable " + temp.lastError().text();
        qDebug() << "Person::addHashRowToLinksTable" << temp.lastError();
        db_connection::instance()->db().database(cname).rollback();

        return false;
    }

    return true;
}

bool Person::getLinkedPersons(Person *person)
{
    if(person->linked_persons != 0) delete person->linked_persons;

    person->linked_persons = new QList<Person*>();

    QString cname = db_connection::instance()->db().connectionName();

    bool isOk = db_connection::instance()->db().database(cname).transaction();
    QSqlQuery temp(db_connection::instance()->db());
    temp.prepare(
   " SELECT                                "
   " p.name,                               "
   " p.midname,                            "
   " p.lastname,                           "
   " p.alias,                              "
   " p.id,                                 "
   " p.country,                            "
   " p.city,                               "
   " p.street,                             "
   " p.house,                              "
   " p.corp,                               "
   " p.flat,                               "
   " p.hash                                "
   " FROM notebook2.person as p            "
   " INNER JOIN                            "
   " (SELECT unnest(                       "
   "     (SELECT links                     "
   "         FROM notebook2.person_links   "
   "         WHERE hash = (:person_hash) ) "
   " ) as hashes) as links                 "
   " ON links.hashes = p.hash              ");

    temp.bindValue(":person_hash", person->hash);
    if (!temp.exec())
    {
        db_connection::instance()->lastError = "Person::getLinkedPersons " + temp.lastError().text();
        qDebug() << "Person::getLinkedPersons" << temp.lastError();
        db_connection::instance()->db().database(cname).rollback();
        return false;
    }

    while (temp.next())
    {
        Person *personLocal = new Person();
        personLocal->name = temp.value(0).toString();
        personLocal->midname = temp.value(1).toString();
        personLocal->lastname = temp.value(2).toString();
        personLocal->alias = temp.value(3).toString();
        personLocal->id = temp.value(4).toString();
        personLocal->state = IsReaded;

        // Adress
        personLocal->country.setData(temp.value(5).toString());
        personLocal->city.setData(temp.value(6).toString());
        personLocal->street.setData(temp.value(7).toString());
        personLocal->house.setData(temp.value(8).toString());
        personLocal->corp.setData(temp.value(9).toString());
        personLocal->flat.setData(temp.value(10).toString());

        // Hash
        personLocal->hash = temp.value(11).toString();

        person->linked_persons->append(personLocal);
    }

    db_connection::instance()->db().database(cname).commit();
    return true;
}

bool Person::unlinkPersonOnDelete(Person *person)
{
    QString query = ""
          "  SELECT hash, links             "
          "  FROM notebook2.person_links    "
          "  WHERE                          "
          "  '"+person->hash+"' = ANY(links)";

    QString cname = db_connection::instance()->db().connectionName();
    bool isOk = db_connection::instance()->db().database(cname).transaction();
    QSqlQuery temp(db_connection::instance()->db());
    QSqlQuery temp1(db_connection::instance()->db());

    temp.prepare(query);
    if(!temp.exec()){
        db_connection::instance()->lastError = "Person::unlinkPersonOnDelete " + temp.lastError().text();
        qDebug() << "Person::unlinkPersonOnDelete" << temp.lastError();
        db_connection::instance()->db().database(cname).rollback();
        return 0;
    }

    while(temp.next()){
        QString hash = temp.value(0).toString();
        QString links = temp.value(1).toString();

        QString updateLinksQuery = ""
                "    UPDATE notebook2.person_links                       "
                "    SET links = ARRAY["+Person::updateHashArray(links,person->hash)+"]::uuid[]                "
                "    WHERE hash = '"+hash+"' ";
        temp1.prepare(updateLinksQuery);
        if(!temp1.exec()){
            db_connection::instance()->lastError = "Person::unlinkPersonOnDelete-updateLinksQuery " + temp1.lastError().text();
            qDebug() << "Person::unlinkPersonOnDelete-updateLinksQuery " << temp1.lastError();
            db_connection::instance()->db().database(cname).rollback();
            return 0;
        }
    }
    return true;
}

bool Person::unlinkPersons(Person *person1, Person *person2)
{
    QString query =
           " SELECT hash, links                         "
           " FROM notebook2.person_links                "
           " WHERE hash = '"+person1->hash+"' OR hash = '"+person2->hash+"'";

    QString cname = db_connection::instance()->db().connectionName();
    bool isOk = db_connection::instance()->db().database(cname).transaction();
    QSqlQuery temp(db_connection::instance()->db());
    QSqlQuery temp1(db_connection::instance()->db());

    temp.prepare(query);
    if(!temp.exec()){
        db_connection::instance()->lastError = "Person::unlinkPersons " + temp.lastError().text();
        qDebug() << "Person::unlinkPersons " << temp.lastError();
        db_connection::instance()->db().database(cname).rollback();
        return 0;
    }
    while(temp.next()){
        QString hash = temp.value(0).toString();
        QString links = temp.value(1).toString();
        QString updatedHashArray = "";
        if(hash == person1->hash){
            updatedHashArray = Person::updateHashArray(links, person2->hash);
        } else if(hash == person2->hash){
            updatedHashArray = Person::updateHashArray(links, person1->hash);
        }

        QString updateLinksQuery = ""
                "    UPDATE notebook2.person_links                       "
                "    SET links = ARRAY["+updatedHashArray+"]::uuid[]                "
                "    WHERE hash = '"+hash+"' ";
        temp1.prepare(updateLinksQuery);
        if(!temp1.exec()){
            db_connection::instance()->lastError = "Person::unlinkPersons-updateLinksQuery " + temp1.lastError().text();
            qDebug() << "Person::unlinkPersons-updateLinksQuery " << temp1.lastError();
            db_connection::instance()->db().database(cname).rollback();
            return 0;
        }
    }
    db_connection::instance()->db().database(cname).commit();
    return true;
}

QString Person::updateHashArray(QString hashes, QString hashToExclude)
{
    QString res = "";
    auto hashesList = hashes.remove("}").remove("{").split(",");
    if(hashesList.length() != 1){
        for (int i=0; i < hashesList.length(); i++) {
            QString localHash = hashesList.at(i);
            if(localHash != hashToExclude) res+="'"+localHash+"',";
        }
        res.chop(1);
    }
    return res;
}

Person *Person::mergePersons(Person *person1, Person *person2, mergeType type)
{
    Person *res = new Person();
    switch(type){
    case firstIsParent:
        res->name       = person1->name;
        res->lastname   = person1->lastname;
        res->midname    = person1->midname;
        res->alias      = person1->alias;
        res->id         = person1->id;

        if(res->id.contains("new")){
            res->state  = IsNewing;
        } else {
            res->state  = IsChanged;
            Person::concatEventIds(person1, person2, res);
        }

        res->country    = person1->country;
        res->city       = person1->city   ;
        res->street     = person1->street ;
        res->house      = person1->house  ;
        res->corp       = person1->corp   ;
        res->flat       = person1->flat   ;

        res->hash       = person1->hash   ;

        if(person1->type == dbPerson && res->state != IsNewing){
            Person::getLinkedPersons(person1);
            QList<Telephone*> *telList = new QList<Telephone*>();
            Telephone::selectTelephone(telList, person1->id);
            for (int i = 0; i < telList->size(); i++) {
                auto tel = telList->at(i);
                tel->state = IsRemoved;
            }
            res->telephones()->append(*telList);
        }

        if(person2->type == dbPerson && person2->state != IsNewing){
            QList<Telephone*> *telList = new QList<Telephone*>();
            Telephone::selectTelephone(telList, person2->id);
            res->telephones()->append(*telList);

            Person::getLinkedPersons(person2);
        } else{
            res->telephones()->append(*person2->telephones());
        }

        if(person2->state != IsNewing)
            person2->state  = IsRemoved;

        res->linked_persons = person1->linked_persons;

        if(res->linked_persons == 0) res->linked_persons = new QList<Person*>();
        if(person2->linked_persons != 0)
            res->linked_persons->append(*person2->linked_persons);

        break;
    case secondIsParent:
        res->name       = person2->name;
        res->lastname   = person2->lastname;
        res->midname    = person2->midname;
        res->alias      = person2->alias;
        res->id         = person2->id;

        if(res->id.contains("new")){
            res->state  = IsNewing;
        } else {
            res->state  = IsChanged;
            Person::concatEventIds(person1, person2, res);
        }

        if(person1->state != IsNewing){
            Person::getLinkedEvents(person2);

            auto localList = person2->linked_events()->toList();
            for (int a = 0; a<localList.size(); a++) {
                auto event =localList.at(a);
                res->linked_events()->insert(event);
            }
        }

        res->country    = person2->country;
        res->city       = person2->city   ;
        res->street     = person2->street ;
        res->house      = person2->house  ;
        res->corp       = person2->corp   ;
        res->flat       = person2->flat   ;

        res->hash       = person2->hash   ;

        if(person2->type == dbPerson && person2->state != IsNewing){
            Person::getLinkedPersons(person2);

            QList<Telephone*> *telList = new QList<Telephone*>();
            Telephone::selectTelephone(telList, person2->id);
            for (int i = 0; i < telList->size(); i++) {
                auto tel = telList->at(i);
                tel->state = IsRemoved;
            }
            res->telephones()->append(*telList);

        }

        if(person1->type == dbPerson){
            if(person1->state != IsNewing) {
                QList<Telephone*> *telList = new QList<Telephone*>();
                Telephone::selectTelephone(telList, person1->id);
                res->telephones()->append(*telList);

                Person::getLinkedPersons(person1);
            } else {
                res->telephones()->append(*person1->telephones());
            }

        }
        if(person1->state != IsNewing)
            person1->state  = IsRemoved;

        res->linked_persons = person2->linked_persons;
        if(res->linked_persons == 0) res->linked_persons = new QList<Person*>();
        if(person1->linked_persons != 0)
            res->linked_persons->append(*person1->linked_persons);

        break;
    }
    return res;
}

bool Person::handleListOfPersons(QList<Person *> *list)
{
    bool state = true;

    for (int i=0; i<list->size(); i++) {
        Person *p = list->at(i);
        QSet<QString> *set = new QSet<QString>();
        QSet<QString> substructed;

        switch(p->state){
        case IsReaded:
            break;
        case IsNewing:
            // I don't have eventId...
            state = Person::createPerson(p);
            if(!state) break;
            state = Person::setLinkedEvents(p);
           break;
        case IsRemoved:
            state = Person::deletePerson(p);
            break;
        case IsChanged:
            state = Person::updatePerson(p);
            if(!state) break;
            state = Person::getLinkedEvents(p);
            if(!state) break;
            state = Person::dropLinksToEvents(p);
            if(!state) break;
            state = Person::setLinkedEvents(p);
            break;
        }
        if(!state) break;
    }
    return state;
}

bool Person::getLinkedEvents(Person *p, QSet<QString> *resList)
{

    QString query =
          " SELECT event_id             "
          " FROM notebook2.event_person "
          " WHERE person_id = "+p->id;

    QString cname = db_connection::instance()->db().connectionName();
    bool isOk = db_connection::instance()->db().database(cname).transaction();
    QSqlQuery temp(db_connection::instance()->db());

    temp.prepare(query);
    if(!temp.exec()){
        db_connection::instance()->lastError = "Person::getLinkedEvents " + temp.lastError().text();
        qDebug() << "Person::getLinkedEvents " << temp.lastError();
        db_connection::instance()->db().database(cname).rollback();
        return 0;
    }

    while(temp.next()){
        QString eventId = temp.value(0).toString();
        if(resList == 0) p->linked_events()->insert(eventId);
        else resList->insert(eventId);
    }
    db_connection::instance()->db().database(cname).commit();
    return true;
}

bool Person::dropLinksToEvents(Person *p)
{
    QString cname = db_connection::instance()->db().connectionName();
    bool isOk = db_connection::instance()->db().database(cname).transaction();

    QSqlQuery temp(db_connection::instance()->db());

    temp.prepare(" DELETE FROM notebook2.event_person WHERE person_id=(:id)");
    temp.bindValue(":id", p->id);

    if(!temp.exec()){
        db_connection::instance()->lastError = "Person::getLinkedEvents " + temp.lastError().text();
        qDebug() << "Person::getLinkedEvents " << temp.lastError();
        db_connection::instance()->db().database(cname).rollback();
        return 0;
    }
    db_connection::instance()->db().database(cname).commit();
    return true;
}

bool Person::setLinkedEvents(Person *p)
{
    if(p->linked_events()->size() == 0) return true;

    QString query =
          " INSERT INTO notebook2.event_person "
          " (event_id, person_id)              "
          " VALUES                             ";

    auto localList = p->linked_events()->toList();
    for (int i=0; i<localList.size(); i++) {
     QString eventId = localList.at(i);
     query += "("+eventId+", "+p->id+"),";
    }
    query.chop(1);

    QString cname = db_connection::instance()->db().connectionName();
    bool isOk = db_connection::instance()->db().database(cname).transaction();
    QSqlQuery temp(db_connection::instance()->db());

    temp.prepare(query);
    if(!temp.exec()){
        db_connection::instance()->lastError = "Person::setLinkedEvents " + temp.lastError().text();
        qDebug() << "Person::setLinkedEvents " << temp.lastError();
        db_connection::instance()->db().database(cname).rollback();
        return 0;
    }
    db_connection::instance()->db().database(cname).commit();
    return true;
}

bool Person::setLinksForPerson(Person *p)
{
    if(p->linked_persons == 0 ) return true;
    QString hashes = "";
    for (int i=0; i<p->linked_persons->size(); i++) {
        Person *per = p->linked_persons->at(i);
        hashes += "'"+per->hash+"',";
    }
    hashes.chop(1);
    if( !db_connection::instance()->db_connect() )
        return false;

    QString cname = db_connection::instance()->db().connectionName();

    bool isOk = db_connection::instance()->db().database(cname).transaction();
    QSqlQuery temp(db_connection::instance()->db());

    temp.prepare("UPDATE notebook2.person_links "
                 " SET links = ARRAY["+hashes+"]::uuid[]"
                 " WHERE hash = (:hash)" );
    temp.bindValue(":hash", p->hash);
    if (!temp.exec())
    {
         db_connection::instance()->lastError = "Person::setLinksForPerson " + temp.lastError().text();
        qDebug() << "Person::setLinksForPerson" << temp.lastError();
        db_connection::instance()->db().database(cname).rollback();

        return false;
    }

    return true;
}

bool Person::concatEventIds(Person *p1, Person *p2, Person *res)
{
    bool state = false ;
    if(p1->state != IsNewing){
        state = Person::getLinkedEvents(p1);
        if(!state) return state;
    }

    auto localList = p1->linked_events()->toList();

    if(p2->state != IsNewing){
        state = Person::getLinkedEvents(p2);
        if(!state) return state;
    }

    localList.append(p2->linked_events()->toList());

    for (int i=0; i<localList.size(); i++) {
         QString eventId = localList.at(i);
         res->linked_events()->insert(eventId);
    }
    return true;
}

Adress::Adress()
{
    this->liv = "";
    this->reg = "";
}

void Adress::setData(QString sqlRow)
{
    auto sqlCopy = sqlRow;
    QStringList splittedData = sqlCopy.split("\",\"");
    if(splittedData.length() == 1)
        splittedData = sqlCopy.split(",");
    this->liv = static_cast<QString>(splittedData.at(0)).remove("(").remove("\"");
    this->reg = static_cast<QString>(splittedData.at(1)).remove(")").remove("\"");
    return;
}
