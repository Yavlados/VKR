#include "linksmanager.h"

LinksManager *LinksManager::_instance = 0;


LinksManager *LinksManager::instance()
{
    if(_instance == 0)
        _instance = new LinksManager();
    return _instance;
}

QList<comparsionResult*> *LinksManager::findLinks(Person *editablePerson, bool isUpdate)
{
    QString hashesToExclude = "";

    // create hashes condition
    if(isUpdate){
        hashesToExclude += " AND p.hash != '"+editablePerson->hash+"' ";
        if(editablePerson->linked_persons != 0){
            for (int i=0; i<editablePerson->linked_persons->length(); i++) {
                Person *linkedPerson = editablePerson->linked_persons->at(i);
                hashesToExclude += " AND p.hash != '"+linkedPerson->hash+"' ";
            }
        }
    }

    QString query = ""
          "  SELECT DISTINCT                                     "
          "          p.id,                                       "
          "          p.name,                                     "
          "          p.midname,                                  "
          "          p.lastname,                                 "
          "          p.alias,                                    "
          "          p.country,                                  "
          "          p.city,                                     "
          "          p.street,                                   "
          "          p.house,                                    "
          "          p.corp,                                     "
          "          p.flat,                                     "
          "          p.hash,                                     "
          "          ep.event_id as events,                      "
          "          'personal' as flag,                         "
          "          NULL as number                              "
          "      FROM notebook2.person as p,                     "
          "           notebook2.event_person as ep               "
          "      WHERE ep.person_id = p.id                       "
          "      AND p.name = '"+editablePerson->name+"'         "
          "      AND p.lastname = '"+editablePerson->lastname+"' "
          "      AND p.midname = '"+editablePerson->midname+"'   "
          "      "+hashesToExclude+"                             "
          "  UNION                                               "
          "  SELECT DISTINCT                                     "
          "          p.id,                                       "
          "          p.name,                                     "
          "          p.midname,                                  "
          "          p.lastname,                                 "
          "          p.alias,                                    "
          "          p.country,                                  "
          "          p.city,                                     "
          "          p.street,                                   "
          "          p.house,                                    "
          "          p.corp,                                     "
          "          p.flat,                                     "
          "          p.hash,                                     "
          "          ep.event_id as events,                      "
          "          'telephone' as flag,                        "
          "          t.number as number                          "
          "      FROM notebook2.person as p,                     "
          "      notebook2.telephone as t,                       "
          "      notebook2.event_person as ep                    "
          "      WHERE t.person_id = p.id                        "
          "      AND ep.person_id = p.id                         "
          "      "+hashesToExclude+"                             ";
    if(editablePerson->telephones()->length()==0)
        query += "AND 1=0";
    else{
        for (int i =0 ; i < editablePerson->telephones()->length(); i++) {
            Telephone *tel = editablePerson->telephones()->at(i);
            if(tel->num.length()>0 && tel->state != IsRemoved){
                    if(i==0){
                        query += " AND (t.number = '"+tel->num+"')";
                    } else {
                        query.chop(1);
                        query += " OR t.number = '"+tel->num+"')";
                    }
            } else {
                // added incomplitable condition
                query  += " AND 1=0";
            }
        }
    }

    QString cname = db_connection::instance()->db().connectionName();

    bool isOk = db_connection::instance()->db().database(cname).transaction();
    QSqlQuery temp(db_connection::instance()->db());
    temp.prepare(query);
    if(!temp.exec()){
        db_connection::instance()->lastError = "LinksManager::findLinks " + temp.lastError().text();
        qDebug() << "LinksManager::findLinks" << temp.lastError();
        db_connection::instance()->db().database(cname).rollback();
        return 0;
    }
    bool first = true;
    QList<comparsionResult*> *returningList = 0;
    QList<Person*> *findedPersons = 0;

    while (temp.next()){
        Person *person = new Person();
        person->id = temp.value(0).toString();
        person->name = temp.value(1).toString();
        person->midname = temp.value(2).toString();
        person->lastname = temp.value(3).toString();
        person->alias = temp.value(4).toString();
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
        if(first){
            first = false;
            returningList = new QList<comparsionResult*>();
            findedPersons = new QList<Person*>();
        }
        // if i have added this person previously
        int personListIndex = LinksManager::instance()->getPersonIndex(findedPersons, person);
        if(personListIndex != -1){
            comparsionResult *oldPerson = returningList->at(personListIndex);

                oldPerson->events->insert(temp.value(12).toString());

            if(oldPerson->reason == telephone  && temp.value(13).toString() == "telephone"){
                if(oldPerson->telephones == 0) {
                    oldPerson->telephones = new QList<QString>();
                }

                oldPerson->telephones->append(temp.value(14).toString());

            }
        } else{
            // if this comparsion is new
            comparsionResult *res = new comparsionResult();
            res->person = person;
            res->reason = temp.value(13).toString() == "personal" ? personal : telephone;

            res->events = new QSet<QString>();
            res->events->insert(temp.value(12).toString());


            if(res->reason == telephone){
                res->telephones = new QList<QString>();
                res->telephones->append(temp.value(14).toString());
            }
            returningList->append(res);
            findedPersons->append(person);
        }
    }
    return LinksManager::instance()->updateMessages(returningList);
}

int LinksManager::getPersonIndex(QList<Person *> *personsList, Person *person)
{
    for (int i=0; i<personsList->length(); i++) {
        if(person->id == personsList->at(i)->id)
            return i;
    }
    return -1;
}

QList<comparsionResult*> *LinksManager::updateMessages(QList<comparsionResult*> *compResults)
{
    if(compResults != 0)
        for (int i=0; i<compResults->length(); i++) {
        comparsionResult *res = compResults->at(i);
        res->message = "<b>ВНИМАНИЕ!</b>"
                      "<br/>"
                      "Во время добавления данных о фигуранте, система обнаружила совпадение "
                      "с персоной <b>"+res->person->lastname+"</b> "
                      "<b>"+res->person->name+"</b> "
                      "<b>"+res->person->midname+"</b>, "
                      "который фигурирует в событиях ";
        QSetIterator<QString> iterator(*res->events);
        while (iterator.hasNext())
            res->message += "<b>"+iterator.next()+"</b>, ";
        res->message.chop(2);
        res->message += "<br/>"
                       "По причине ";
        switch(res->reason){
        case personal:
                res->message += "совпадения фамилии, имени и отчества.";
            break;
        case telephone:
            res->message += "совпадения телефон";
            for (int o=0; o<res->telephones->length(); o++) {
                QString telephone = res->telephones->at(o);

                if(o==0){
                    if(res->telephones->length() > 1) res->message += "ных номеров: ";
                    else  res->message += "ного номера:";
                }
                res->message += " "+telephone;
                if(o != res->telephones->length()-1 ) res->message += ",";
            }
            break;
        }

        compResults->at(i)->message  = res->message;
    }
    return compResults;
}

bool LinksManager::createLinks(Person *person, QList<comparsionResult *> *list)
{
    QString linkChildsHashes = "";
    QString linkParentHashes = "";

    for (int i =0; i<list->length(); i++) {
        auto comp = list->at(i);
        if(i==0) {
            linkChildsHashes += " hash = '"+comp->person->hash+"' ";
        } else {
             linkChildsHashes += " OR hash = '"+comp->person->hash+"' ";
        }
        linkParentHashes += "'"+comp->person->hash+"'";
        if(i<list->length()-1) linkParentHashes+=",";
    }

    QString linkChildsQuery = "UPDATE notebook2.person_links    "
          "  SET                                                            "
          "  links = array_cat(links, ARRAY['"+person->hash+"']::uuid[])    "
          "  WHERE "+linkChildsHashes;

    QString linkParentQuery = "UPDATE notebook2.person_links                       "
        "   SET                                                                    "
        "       links = array_cat(links, ARRAY["+linkParentHashes+"]::uuid[])      "
        "   WHERE hash = '"+person->hash+"'                                        ";
    QString query = linkChildsQuery+"; "+linkParentQuery;

    QString cname = db_connection::instance()->db().connectionName();
    bool isOk = db_connection::instance()->db().database(cname).transaction();
    QSqlQuery temp(db_connection::instance()->db());
    QSqlQuery temp1(db_connection::instance()->db());

    temp.prepare(linkChildsQuery);
    if(!temp.exec()){
        db_connection::instance()->lastError = "LinksManager::createLinks-linkChildsQuery " + temp.lastError().text();
        qDebug() << "LinksManager::createLinks-linkChildsQuery" << temp.lastError();
        db_connection::instance()->db().database(cname).rollback();
        return 0;
    }
    temp1.prepare(linkParentQuery);
    if(!temp1.exec()){
        db_connection::instance()->lastError = "LinksManager::createLinks-linkParentQuery " + temp1.lastError().text();
        qDebug() << "LinksManager::createLinks-linkParentQuery" << temp1.lastError();
        db_connection::instance()->db().database(cname).rollback();
        return 0;
    }
    db_connection::instance()->db().database(cname).commit();
    return true;
}

LinksManager::LinksManager()
{

}

comparsionResult::comparsionResult()
{
    this->message = "";
    this->reason = initial;
    this->person = 0;
    this->telephones = 0;
    this->events = 0;
    this->solution = initial_solution;
}
