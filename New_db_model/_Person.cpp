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

bool Person::createPerson(Person *person, QString eventId)
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

        if(!Person::handleTelephones(person->telephones(), person->id) ||
                !Person::linkEventPerson(eventId, person->id) ||
                !Person::addHashRowToLinksTable(person)){
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
        db_connection::instance()->db().database(cname).commit();
        return true;
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
