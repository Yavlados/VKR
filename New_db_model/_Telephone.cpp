#include "_Telephone.h"

Telephone::Telephone()
{
    this->id        ="";
    this->num       ="";
    this->person_id ="";
    this->internum  =false;
    this->oldnum    =false;
    this->state     =IsNewing;
    this->_cont = 0;
}


QList<Contact *> *Telephone::cont()
{
    if (this->_cont == 0)
    {
        this->_cont = new QList<Contact*>;
    }
    return this->_cont;
}

bool Telephone::selectTelephone(QList<Telephone *> *list, QString personId)
{
    if(list==0)
        return false;

    qDeleteAll(*list);
    list->clear();

    if( !db_connection::instance()->db_connect() )
        return false;

    QSqlQuery temp(db_connection::instance()->db());
    temp.prepare("SELECT number,"
                 "id,"
                 "person_id,"
                 "oldnum,"
                 "internum "
                 "FROM notebook2.telephone "
                 "WHERE person_id = (:id)");
    temp.bindValue(":id", personId);
    if (!temp.exec())
    {
        db_connection::instance()->lastError = "Person::selectTelephone " + temp.lastError().text();
        qDebug() << "Telephone::selectTelephone" << temp.lastError();
        return false;
    }

    while (temp.next())
    {
        Telephone *ot = new Telephone();
        ot->num = temp.value(0).toString();
        ot->id = temp.value(1).toString();
        ot->person_id = temp.value(2).toString();
        ot->oldnum = temp.value(3).toBool();
        ot->internum = temp.value(4).toBool();
        ot->state = IsReaded;
        list->append(ot);
    }

    return true;
}

bool Telephone::createTelephone(Telephone *telephone, QString personId)
{
    if( !db_connection::instance()->db_connect() )
        return false;

    QString cname = db_connection::instance()->db().connectionName();
    bool isOk = db_connection::instance()->db().database(cname).transaction();

    QSqlQuery temp(db_connection::instance()->db());
    temp.prepare("INSERT INTO notebook2.telephone ( number,"
                                                   "person_id,"
                                                   "internum,"
                                                   "oldnum)"
                 " VALUES ( (:number), (:person_id), (:internum), (:oldnum))"
                 " RETURNING id" );
    temp.bindValue(":number",telephone->num);
    temp.bindValue(":person_id", personId.toInt());
    temp.bindValue(":internum", telephone->internum);
    temp.bindValue(":oldnum", telephone->oldnum);

    if (!temp.exec())
    {
        db_connection::instance()->lastError = "Telephone::createTelephone " + temp.lastError().text();
        qDebug() << "Telephone::createTelephone" << temp.lastError();
        return false;
    }
    if (temp.next())
    {
        if(!Telephone::handleContacts(telephone->cont(), temp.value(0).toString())){
            db_connection::instance()->lastError = temp.lastError().text();
            db_connection::instance()->db().database(cname).rollback();
            return false;
        }

//        for( int i=0; i<telephone->cont()->size(); i++){
//            Contact *cont = telephone->cont()->at(i);

//            switch(cont->state){
//            case IsNewing:
//             Contact::createContact(cont, temp.value(0).toString());
//             break;
//         case IsChanged:
//             Contact::updateContact(cont);
//             break;
//         case IsRemoved:
//             Contact::deleteContact(cont);
//             break;
//         case IsReaded:
//             break;
//            }
//        }
//        db_connection::instance()->db().database(cname).commit();
//        return true;
    }
    db_connection::instance()->db().database(cname).commit();
    return true;
//        db_connection::instance()->lastError = temp.lastError().text();
//        db_connection::instance()->db().database(cname).rollback();

//        return false;
}

bool Telephone::updateTelephone(Telephone *telephone)
{
    QString cname = db_connection::instance()->db().connectionName();

    bool isOk = db_connection::instance()->db().database(cname).transaction();
    QSqlQuery temp(db_connection::instance()->db());
    temp.prepare("UPDATE notebook2.telephone "
                 " SET 	number = (:number),"
                         " person_id = (:person_id),"
                         " internum = (:internum),"
                         " oldnum = (:oldnum)"
                 " WHERE id= (:id)");

     temp.bindValue(":number", telephone->num);
     temp.bindValue(":person_id", telephone->person_id.toInt());
     temp.bindValue(":internum", telephone->internum);
     temp.bindValue(":oldnum", telephone->oldnum);
     temp.bindValue(":id", telephone->id.toInt());

     if (!temp.exec())
     {
         db_connection::instance()->lastError = "Telephone::updateTelephone " + temp.lastError().text();

         qDebug()  << "Telephone::updateTelephone"<< temp.lastError() << temp.executedQuery();

         isOk = false;
     } else {

         if(!Telephone::handleContacts(telephone->cont(), telephone->id)){
             db_connection::instance()->lastError = temp.lastError().text();
             db_connection::instance()->db().database(cname).rollback();
             return false;
         }

//         for( int i=0; i<telephone->cont()->size(); i++){
//             Contact *cont = telephone->cont()->at(i);

//             switch(cont->state){
//             case IsNewing:
//              Contact::createContact(cont, telephone->id);
//              break;
//          case IsChanged:
//              Contact::updateContact(cont);
//              break;
//          case IsRemoved:
//              Contact::deleteContact(cont);
//              break;
//          case IsReaded:
//              break;
//             }
//         }
//         db_connection::instance()->db().database(cname).commit();
//         return true;
     }
         db_connection::instance()->db().database(cname).commit();
         return true;
}

bool Telephone::deleteTelephone(Telephone *telephone)
{
    QString cname = db_connection::instance()->db().connectionName();

    bool isOk = db_connection::instance()->db().database(cname).transaction();
    QSqlQuery temp(db_connection::instance()->db());
    temp.prepare("DELETE FROM notebook2.telephone WHERE id=(:id)");

    temp.bindValue(":id", telephone->id.toInt());

    if (!temp.exec())
    {
        db_connection::instance()->lastError = "Telephone::updateTelephone " + temp.lastError().text();
        qDebug()  << "Telephone::updateTelephone"<< temp.lastError() << temp.executedQuery();
        db_connection::instance()->db().database(cname).rollback();
        return false ;
    } else{
        db_connection::instance()->db().database(cname).commit();
        return true;
    }
}

bool Telephone::handleContacts(QList<Contact *> *cont, QString telephoneId)
{
    for( int i=0; i< cont->size(); i++){
        bool state = false;
        Contact *c = cont->at(i);

        switch(c->state){
     case IsNewing:
         state = Contact::createContact(c, telephoneId);
         break;
     case IsChanged:
         state = Contact::updateContact(c);
         break;
     case IsRemoved:
         state = Contact::deleteContact(c);
         break;
     case IsReaded:
         state = true;
         break;
        }

        if(!state)
            return state;
    }
    return true;
}
