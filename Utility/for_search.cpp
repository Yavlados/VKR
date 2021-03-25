#include "for_search.h"

For_search::For_search()
{
    this->setInitialQuery();
    this->event_id = "";
    this->event_category = "";
    this->event_detention_date = "";
    this->event_detention_time = "";
    this->event_detention_reason = "";
    this->event_detention_by = "";
    this->event_keeping_place = "";
    this->event_detention_from = "";
    this->event_detention_to = "";

    this->person_lastname = "";
    this->person_name = "";
    this->person_midname = "";
    this->person_alias = "";

    this->telephone_num = "";

    this->contact_num = "";
    this->contact_alias = "";
}

void For_search::updateState()
{
    if(!this->event_id              .isEmpty()   ||
       !this->event_category        .isEmpty()   ||
       !this->event_detention_date  .isEmpty()   ||
       !this->event_detention_time  .isEmpty()   ||
       !this->event_detention_reason.isEmpty()   ||
       !this->event_detention_by    .isEmpty()   ||
       !this->event_keeping_place   .isEmpty()   ||
       !this->event_detention_from  .isEmpty()   ||
       !this->event_detention_to    .isEmpty()   )
                this->state.event = true;

    if(!this->person_lastname.isEmpty()    ||
       !this->person_name.isEmpty()        ||
       !this->person_midname.isEmpty()     ||
       !this->person_alias.isEmpty()   )
                this->state.person = true;

    if(!this->telephone_num.isEmpty())
                this->state.telephone = true;

    if(!this->contact_num.isEmpty()    ||
       !this->contact_alias.isEmpty() )
        this->state.contact = true;
}

bool For_search::search()
{
    QString updatedQuery = this->updateQuery();

    if( !db_connection::instance()->db_connect() )
        return 0;

    QString cname = db_connection::instance()->db().connectionName();

    bool isOk = db_connection::instance()->db().database(cname).transaction();
    QSqlQuery temp(db_connection::instance()->db());


    if (!temp.exec(updatedQuery))
    {
        db_connection::instance()->lastError = "For_search::search() " + temp.lastError().text();

        qDebug() << "For_search::search() " << temp.lastError();
        db_connection::instance()->db().database(cname).rollback();
        isOk = false;
        return isOk;
    }

    while(temp.next()){
        this->searchedIds.append(temp.value(0).toString());
    }

    return isOk;
}

const QString For_search::updateQuery()
{
    QString newQuery = this->query;
      if(this->state.event)
          newQuery = this->updateQueryEventFields(newQuery);
      if(this->state.person)
          newQuery = this->updateQueryPersonFields(newQuery);
      if(this->state.telephone)
          newQuery = this->updateQueryTelephoneFields(newQuery);
      if(this->state.contact)
          newQuery = this->updateQueryContactFields(newQuery);

    return newQuery;
}

QString For_search::updateQueryEventFields(QString newQuery)
{
    if(!this->event_id.isEmpty()) newQuery += " AND ep.id = "+this->event_id;
    if(!this->event_category.isEmpty()) newQuery += " AND LOWER(ep.category) LIKE '"+this->event_category+"'";
    if(!this->event_detention_date.isEmpty()) newQuery += " AND ep.detention_date = '"+this->event_detention_date+"'";
    if(!this->event_detention_time.isEmpty()) newQuery += " AND ep.detention_time = '"+this->event_detention_time+"'";
    if(!this->event_detention_reason.isEmpty()) newQuery += " AND LOWER(ep.detention_reason) LIKE '"+this->event_detention_reason+"'";
    if(!this->event_detention_by.isEmpty()) newQuery += " AND LOWER(ep.detention_by) LIKE '"+this->event_detention_by+"'";
    if(!this->event_keeping_place.isEmpty()) newQuery += " AND LOWER(ep.keeping_place) LIKE '"+this->event_keeping_place+"'";
    if(!this->event_detention_from.isEmpty() || !this->event_detention_to.isEmpty())
            newQuery += "AND ep.detention_date BETWEEN '"+this->event_detention_from+"'AND '"+this->event_detention_to+"'";
    return newQuery;
}

QString For_search::updateQueryPersonFields(QString newQuery)
{
    if(!this->person_name.isEmpty())        newQuery += " AND LOWER(ptc.name) LIKE '"+this->person_name+"'";
    if(!this->person_lastname.isEmpty())    newQuery += " AND LOWER(ptc.lastname) LIKE '"+this->person_lastname+"'";
    if(!this->person_midname.isEmpty())     newQuery += " AND LOWER(ptc.midname) LIKE '"+this->person_midname+"'";
    if(!this->person_alias.isEmpty())       newQuery += " AND LOWER(ptc.alias) LIKE '"+this->person_alias+"'";

    return newQuery;
}

QString For_search::updateQueryTelephoneFields(QString newQuery)
{
    if(!this->telephone_num.isEmpty())      newQuery += " AND ptc.telephone_number LIKE '"+this->telephone_num+"'";

    return newQuery;
}

QString For_search::updateQueryContactFields(QString newQuery)
{
    if(!this->contact_num.isEmpty())      newQuery += " AND ptc.contact_number LIKE '"+this->contact_num+"'";
    if(!this->contact_alias.isEmpty())      newQuery += " AND LOWER(ptc.contact_alias) LIKE '"+this->contact_alias+"'";

    return newQuery;
}

void For_search::setInitialQuery()
{
    this->query =
            "SELECT DISTINCT ep.id                               "
            "FROM (                                              "
            "        SELECT *                                    "
            "        FROM notebook2.event_person as ep,          "
            "        notebook2.event as e                        "
            "        WHERE e.id = ep.event_id                    "
            "    )as ep                                          "
            "FULL OUTER JOIN                                     "
            "    (SELECT                                         "
            "            p.id as p_id,                           "
            "            p.name as name,                         "
            "            p.lastname as lastname,                 "
            "            p.midname as midname,                   "
            "            p.alias as alias,                       "
            "                                                    "
            "            tc.telephone as telephone_number,       "
            "            tc.telephone_id as 		telephone_id,"
            "                                                    "
            "            tc.from_contact as contact_number,      "
            "            tc.from_contact_alias as contact_alias  "
            "                                                    "
            "    FROM notebook2.person as p                      "
            "    FULL OUTER JOIN                                 "
            "    (SELECT                                         "
            "            t.person_id as t_id,                    "
            "            t.number as telephone,                  "
            "            t.oldnum as telephone_oldnum,           "
            "            t.internum as telephone_internum,		 "
            "            t.id as telephone_id,                   "
            "            c.number as from_contact,               "
            "            c.alias as from_contact_alias           "
            "            FROM notebook2.telephone as t           "
            "            FULL JOIN (                             "
            "            SELECT *                                "
            "            FROM notebook2.contact) as c            "
            "            ON c.telephone_id = t.id) as tc         "
            "    ON tc.t_id = p.id) as ptc                       "
            " ON ep.person_id = ptc.p_id                         "
            " WHERE  1=1                                         ";
}

searchState::searchState()
{
    event = false;
    person = false;
    telephone = false;
    contact = false;
}
