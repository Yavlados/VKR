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
                 "p.name, "
                 "p.midname,"
                 "p.lastname,"
                 "p.alias, "
                 "p.id "
                 "FROM notebook2.person as p, "
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
                      " alias = (:alias) "
                 " WHERE id = (:id) ");

     temp.bindValue(":lastname", person->lastname);
     temp.bindValue(":name", person->name);
     temp.bindValue(":midname", person->midname);
     temp.bindValue(":alias", person->alias);
     temp.bindValue(":id", person->id.toInt());

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
                     "alias)"
                 " VALUES ( (:lastname), (:name), (:midname),  (:alias))"
                 " RETURNING id" );
    temp.bindValue(":lastname", person->lastname);
    temp.bindValue(":name",     person->name);
    temp.bindValue(":midname",  person->midname);
    temp.bindValue(":alias",    person->alias);

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

        if(!Person::handleTelephones(person->telephones(), person->id) ||
                !Person::linkEventPerson(eventId, person->id)){
            db_connection::instance()->db().database(cname).rollback();
            return false;
        }
//        for( int i=0; i<person->telephones()->size(); i++){
//            Telephone *tel = person->telephones()->at(i);
//            bool a = false;
//            switch (tel->state) {
//               case IsNewing:
//                a = Telephone::createTelephone(tel, person->id);
//                break;
//            case IsChanged:
//                a = Telephone::updateTelephone(tel);
//                break;
//            case IsRemoved:
//                a = Telephone::deleteTelephone(tel);
//                break;
//            case IsReaded:
//                a=true;
//                break;
//            }

//            if(!a){
//                db_connection::instance()->db().database(cname).rollback();
//                isOk = false;
//                break;
//            }
//        }

//        if(!Person::linkEventPerson(eventId, person->id)){
//            db_connection::instance()->db().database(cname).rollback();
//            return false;
//        }
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
//            db_connection::instance()->db().database(cname).rollback();
//            isOk = false;
//            break;
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
