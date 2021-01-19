#include "analysisModels.h"

Analysis_V1::Analysis_V1()
{
   this->from_event_id                  = ""    ;
   this->from_event_category            = ""    ;
   this->from_event_detention_date      = ""    ;
   this->from_event_detention_time      = ""    ;
   this->from_event_detention_reason    = ""    ;
   this->from_event_detention_by        = ""    ;
   this->from_event_keeping_place       = ""    ;
   this->from_event_keeping_additional  = ""    ;
   this->from_name                      = ""    ;
   this->from_lastname                  = ""    ;
   this->from_midname                   = ""    ;
   this->from_telephone                 = ""    ;
   this->from_telephone_oldnum          = false ;
   this->from_telephone_internum        = false ;
   this->to_contact                     = ""    ;
   this->to_contact_alias              = ""    ;
   this->to_contact_oldnum             = false ;
   this->to_contact_internum           = false ;
   this->to_telephone                   = ""    ;
   this->to_telephone_oldnum            = false ;
   this->to_telephone_internum          = false ;
   this->to_name                        = ""    ;
   this->to_lastname                    = ""    ;
   this->to_midname                     = ""    ;
   this->to_event_category              = ""    ;
   this->to_event_detention_date        = ""    ;
   this->to_event_detention_time        = ""    ;
   this->to_event_detention_reason      = ""    ;
   this->to_event_detention_by          = ""    ;
   this->to_event_keeping_place         = ""    ;
   this->to_event_keeping_additional    = ""    ;
   this->to_event_id                    = ""    ;
}

Analysis_V2::Analysis_V2()
{
    this->from_event_id                  = ""    ;
    this->from_event_category            = ""    ;
    this->from_event_detention_date      = ""    ;
    this->from_event_detention_time      = ""    ;
    this->from_event_detention_reason    = ""    ;
    this->from_event_detention_by        = ""    ;
    this->from_event_keeping_place       = ""    ;
    this->from_event_keeping_additional  = ""    ;
    this->from_name                      = ""    ;
    this->from_lastname                  = ""    ;
    this->from_midname                   = ""    ;
    this->from_telephone                 = ""    ;
    this->from_telephone_oldnum          = false ;
    this->from_telephone_internum        = false ;
    this->from_contact                   = ""    ;
    this->from_contact_alias             = ""    ;
    this->from_contact_oldnum            = false ;
    this->from_contact_internum          = false ;
    this->to_telephone                   = ""    ;
    this->to_telephone_oldnum            = false ;
    this->to_telephone_internum          = false ;
    this->to_name                        = ""    ;
    this->to_lastname                    = ""    ;
    this->to_midname                     = ""    ;
    this->to_event_category              = ""    ;
    this->to_event_detention_date        = ""    ;
    this->to_event_detention_time        = ""    ;
    this->to_event_detention_reason      = ""    ;
    this->to_event_detention_by          = ""    ;
    this->to_event_keeping_place         = ""    ;
    this->to_event_keeping_additional    = ""    ;
    this->to_event_id                    = ""    ;
}

Analysis_V3::Analysis_V3()
{
    this->from_event_id                    = ""     ;
    this->from_event_category              = ""     ;
    this->from_event_detention_date        = ""     ;
    this->from_event_detention_time        = ""     ;
    this->from_event_detention_reason      = ""     ;
    this->from_event_detention_by          = ""     ;
    this->from_event_keeping_place         = ""     ;
    this->from_event_keeping_additional    = ""     ;
    this->from_name                        = ""     ;
    this->from_lastname                    = ""     ;
    this->from_midname                     = ""     ;
    this->from_telephone                   = ""     ;
    this->from_telephone_oldnum            = false  ;
    this->from_telephone_internum          = false  ;
    this->from_contact                     = ""     ;
    this->from_contact_alias               = ""     ;
    this->from_contact_oldnum              = false  ;
    this->from_contact_internum            = false  ;
    this->to_contact                       = ""     ;
    this->to_contact_alias                 = ""     ;
    this->to_contact_oldnum                = false  ;
    this->to_contact_internum              = false  ;
    this->to_telephone                     = ""     ;
    this->to_telephone_oldnum              = false  ;
    this->to_telephone_internum            = false  ;
    this->to_name                          = ""     ;
    this->to_lastname                      = ""     ;
    this->to_midname                       = ""     ;
    this->to_event_category                = ""     ;
    this->to_event_detention_date          = ""     ;
    this->to_event_detention_time          = ""     ;
    this->to_event_detention_reason        = ""     ;
    this->to_event_detention_by            = ""     ;
    this->to_event_keeping_place           = ""     ;
    this->to_event_keeping_additional      = ""     ;
    this->to_event_id                      = ""     ;
}

AnalysisModels *AnalysisModels::_instance = 0;

AnalysisModels *AnalysisModels::instance()
{
    if(_instance == 0){
        _instance = new AnalysisModels();
    }
    return _instance;
}

QString AnalysisModels::v1(QString eventId, QVector<int> *eventIdList)
{
    QString mainQuery = "SELECT DISTINCT 	f_t.from_event_id as from_event_id,"
                        "f_t.from_event_category as from_event_category,"
                        "f_t.from_event_detention_date as from_event_detention_date,"
                        "f_t.from_event_detention_time as from_event_detention_time,"
                        "f_t.from_event_detention_reason as from_event_detention_reason,"
                       " f_t.from_event_detention_by as from_event_detention_by,"
                       " f_t.from_event_keeping_place as from_event_keeping_place,"
                       " f_t.from_event_keeping_additional as from_event_keeping_additional,"
                        "f_t.from_name as from_name,"
                        "f_t.from_lastname as from_lastname,"
                       " f_t.from_midname as from_midname,"
                       " f_t.from_telephone as from_telephone,"
                       " f_t.from_telephone_oldnum as from_telephone_oldnum,"
                      "  f_t.from_telephone_internum as from_telephone_internum,"
                       " contact.number as to_contact,"
                        "contact.alias as to_conntact_alias,"
                       " contact.oldnum as to_conntact_oldnum,"
                        "contact.internum as to_conntact_internum,"
                        "t.number as to_telephone,"
                        "t.oldnum  as to_telephone_oldnum,"
                        "t.internum  as to_telephone_internum,"
                        "p.name  as to_name,"
                       " p.lastname as to_lastname,"
                        "p.midname as to_midname,"
                        "e.category as to_event_category,"
                       " e.detention_date as to_event_detention_date,"
                        "e.detention_time as to_event_detention_time,"
                        "e.detention_reason as to_event_detention_reason,"
                       " e.detention_by as to_event_detention_by,"
                        "e.keeping_place as to_event_keeping_place,"
                        "e.additional as to_event_keeping_additional,"
                       " e.id as to_event_id"
            "   FROM notebook2.event as e,"
            "   notebook2.event_person as ep,"
            "   notebook2.person as p,                                                                   "
            "   notebook2.telephone as t,                                                                "
            "   notebook2.contact                                                                        "
            "   INNER JOIN                                                                               "
            "   (SELECT DISTINCT                                                                         "
            "            events.from_event_id as from_event_id,                                       "
            "            events.from_event_category as from_event_category,                           "
            "            events.from_event_detention_date as from_event_detention_date,               "
            "            events.from_event_detention_time as from_event_detention_time,               "
            "            events.from_event_detention_reason as from_event_detention_reason,           "
            "            events.from_event_detention_by as from_event_detention_by,                   "
            "            events.from_event_keeping_place as from_event_keeping_place,                 "
            "            events.from_event_keeping_additional as from_event_keeping_additional,       "
            "            person.id,                                                                   "
            "            person.name as from_name,                                                    "
            "            person.lastname as from_lastname,                                            "
            "            person.midname as from_midname,                                              "
            "            t.number as from_telephone,                                                  "
            "            t.oldnum as from_telephone_oldnum,                                           "
            "            t.internum as from_telephone_internum,                                       "
            "            t.id as telephone_id                                                         "
            "            FROM notebook2.person                                                        "
            "                INNER JOIN (                                                             "
            "                            SELECT *                                                     "
            "                            FROM notebook2.telephone                                     "
            "                        )as t                                                            "
            "                        on t.person_id = person.id                                       "
            "            , notebook2.event_person                                                     "
            "                INNER JOIN (                                                             "
            "                        SELECT event.category as from_event_category,                    "
            "                            event.detention_date as from_event_detention_date,           "
            "                            event.detention_time as from_event_detention_time,           "
            "                            event.detention_reason as from_event_detention_reason,       "
            "                            event.detention_by as from_event_detention_by,               "
            "                            event.keeping_place as from_event_keeping_place,             "
            "                            event.additional as from_event_keeping_additional,           "
            "                            event.id as from_event_id                                    "
            "                            FROM notebook2.event                                         "
            "                        WHERE id = "+eventId+") as events                                "
            "                        ON event_person.event_id = events.from_event_id) as f_t          "
            "                        ON f_t.from_telephone = contact.number                           "
            "                        WHERE contact.telephone_id = t.id                                "
            "                        AND t.person_id = p.id                                           "
            "                        AND p.id = ep.person_id                                          "
            "                        AND ep.event_id = e.id                                           "
            "                        AND e.id != "+eventId+"                                          ";
    for (int i=0; i<eventIdList->size(); i++){
        if(i==0) mainQuery += "AND (";

        mainQuery += "e.id == "+QString(eventIdList->at(i));
        if(i<eventIdList->size()-1) mainQuery += " OR ";
        if(i == eventIdList->size()-1) mainQuery += ");";
    }
    return mainQuery;
}

QString AnalysisModels::v2(QString eventId, QVector<int> *eventIdList)
{
     QString mainQuery ="            SELECT                                                                                                         "
                        "                            f_c.from_event_id as from_event_id,                                                            "
                        "                            f_c.from_event_category as from_event_category,                                                "
                        "                            f_c.from_event_detention_date as from_event_detention_date,                                    "
                        "                            f_c.from_event_detention_time as from_event_detention_time,                                    "
                        "                            f_c.from_event_detention_reason as from_event_detention_reason,                                "
                        "                            f_c.from_event_detention_by as from_event_detention_by,                                        "
                        "                            f_c.from_event_keeping_place as from_event_keeping_place,                                      "
                        "                            f_c.from_event_keeping_additional as from_event_keeping_additional,                            "
                        "                            f_c.from_name as from_name,                                                                    "
                        "                            f_c.from_lastname as from_lastname,                                                            "
                        "                            f_c.from_midname as from_midname,                                                              "
                        "                            f_c.from_telephone as from_telephone,                                                          "
                        "                            f_c.from_telephone_oldnum as from_telephone_oldnum,                                            "
                        "                            f_c.from_telephone_internum as from_telephone_internum,                                        "
                        "                            f_c.from_contact as from_contact,                                                              "
                        "                            f_c.from_contact_alias as from_contact_alias,                                                  "
                        "                            f_c.from_contact_oldnum as from_contact_oldnum,                                                "
                        "                            f_c.from_contact_internum as from_contact_internum,                                            "
                        "                            telephone.number as to_telephone,                                                              "
                        "                            telephone.oldnum as to_telephone_oldnum,                                                       "
                        "                            telephone.internum as to_telephone_internum,				                                    "
                        "                            person.name as to_name,                                                                        "
                        "                            person.lastname as to_lastname,                                                                "
                        "                            person.midname as to_midname,                                                                  "
                        "                            event.category as to_event_category,                                                           "
                        "                            event.detention_date as to_event_detention_date,                                               "
                        "                            event.detention_time as to_event_detention_time,                                               "
                        "                            event.detention_reason as to_event_detention_reason,                                           "
                        "                            event.detention_by as to_event_detention_by,                                                   "
                        "                            event.keeping_place as to_event_keeping_place,                                                 "
                        "                            event.additional as to_event_keeping_additional,                                               "
                        "                            event.id as to_event_id                                                                        "
                        "            FROM notebook2.telephone                                                                                       "
                        "            INNER JOIN                                                                                                     "
                        "            (				SELECT  DISTINCT                                                                                "
                        "                            f_t.from_event_id as from_event_id,                                                            "
                        "                            f_t.from_event_category as from_event_category,                                                "
                        "                            f_t.from_event_detention_date as from_event_detention_date,                                    "
                        "                            f_t.from_event_detention_time as from_event_detention_time,                                    "
                        "                            f_t.from_event_detention_reason as from_event_detention_reason,                                "
                        "                            f_t.from_event_detention_by as from_event_detention_by,                                        "
                        "                            f_t.from_event_keeping_place as from_event_keeping_place,                                      "
                        "                            f_t.from_event_keeping_additional as from_event_keeping_additional,                            "
                        "                            f_t.from_name as from_name,                                                                    "
                        "                            f_t.from_lastname as from_lastname,                                                            "
                        "                            f_t.from_midname as from_midname,                                                              "
                        "                            f_t.from_telephone as from_telephone,                                                          "
                        "                            f_t.from_telephone_oldnum as from_telephone_oldnum,                                            "
                        "                            f_t.from_telephone_internum as from_telephone_internum,                                        "
                        "                            c.number as from_contact,                                                                      "
                        "                            c.alias as from_contact_alias,                                                                 "
                        "                            c.oldnum as from_contact_oldnum,                                                               "
                        "                            c.internum as from_contact_internum                                                            "
                        "                        FROM  notebook2.contact as c                                                                       "
                        "                        INNER JOIN                                                                                         "
                        "                        (                                                                                                  "
                        "                            SELECT 	DISTINCT                                                                            "
                        "                            events.from_event_id as from_event_id,                                                         "
                        "                            events.from_event_category as from_event_category,                                             "
                        "                            events.from_event_detention_date as from_event_detention_date,                                 "
                        "                            events.from_event_detention_time as from_event_detention_time,                                 "
                        "                            events.from_event_detention_reason as from_event_detention_reason,                             "
                        "                            events.from_event_detention_by as from_event_detention_by,                                     "
                        "                            events.from_event_keeping_place as from_event_keeping_place,                                   "
                        "                            events.from_event_keeping_additional as from_event_keeping_additional,                         "
                        "                            person.id,                                                                                     "
                        "                            person.name as from_name,                                                                      "
                        "                            person.lastname as from_lastname,                                                              "
                        "                            person.midname as from_midname,                                                                "
                        "                            t.number as from_telephone,                                                                    "
                        "                            t.oldnum as from_telephone_oldnum,                                                             "
                        "                            t.internum as from_telephone_internum,                                                         "
                        "                            t.id as telephone_id                                                                           "
                        "                            FROM notebook2.person                                                                          "
                        "                                INNER JOIN (                                                                               "
                        "                                            SELECT *                                                                       "
                        "                                            FROM notebook2.telephone                                                       "
                        "                                        )as t                                                                              "
                        "                                        on t.person_id = person.id                                                         "
                        "                            , notebook2.event_person                                                                       "
                        "                                INNER JOIN (                                                                               "
                        "                                        SELECT event.category as from_event_category,                                      "
                        "                                            event.detention_date as from_event_detention_date,                             "
                        "                                            event.detention_time as from_event_detention_time,                             "
                        "                                            event.detention_reason as from_event_detention_reason,                         "
                        "                                            event.detention_by as from_event_detention_by,                                 "
                        "                                            event.keeping_place as from_event_keeping_place,                               "
                        "                                            event.additional as from_event_keeping_additional,                             "
                        "                                            event.id as from_event_id                                                      "
                        "                                            FROM notebook2.event                                                           "
                        "                                        WHERE id ="+eventId+" ) as events                                                  "
                        "                                        ON event_person.event_id = events.from_event_id                                    "
                        "                        ) as f_t                                                                                           "
                        "                            ON f_t.telephone_id = c.telephone_id ) as f_c                                                  "
                        "                            ON f_c.from_contact = telephone.number,                                                        "
                        "            notebook2.person,                                                                                              "
                        "            notebook2.event_person,                                                                                        "
                        "            notebook2.event                                                                                                "
                        "            WHERE person.id = telephone.person_id                                                                          "
                        "            AND event_person.person_id = person.id                                                                         "
                        "            AND event.id = event_person.event_id                                                                           "
                        "            AND event.id != "+eventId+"                                                                                    ";

     for (int i=0; i<eventIdList->size(); i++){
         if(i==0) mainQuery += "AND (";

         mainQuery += "event.id == "+QString(eventIdList->at(i));
         if(i<eventIdList->size()-1) mainQuery += " OR ";
         if(i == eventIdList->size()-1) mainQuery += ");";
     }
     return mainQuery;
}

QString AnalysisModels::v3(QString eventId, QVector<int> *eventIdList)
{
    QString mainQuery =
            "            SELECT DISTINCT                                                                                      "
            "                f_c.from_event_id as from_event_id,                                                              "
            "                f_c.from_event_category as from_event_category,                                                  "
            "                f_c.from_event_detention_date as from_event_detention_date,                                      "
            "                f_c.from_event_detention_time as from_event_detention_time,                                      "
            "                f_c.from_event_detention_reason as from_event_detention_reason,                                  "
            "                f_c.from_event_detention_by as from_event_detention_by,                                          "
            "                f_c.from_event_keeping_place as from_event_keeping_place,                                        "
            "                f_c.from_event_keeping_additional as from_event_keeping_additional,                              "
            "                f_c.from_name as from_name,                                                                      "
            "                f_c.from_lastname as from_lastname,                                                              "
            "                f_c.from_midname as from_midname,                                                                "
            "                f_c.from_telephone as from_telephone,                                                            "
            "                f_c.from_telephone_oldnum as from_telephone_oldnum,                                              "
            "                f_c.from_telephone_internum as from_telephone_internum,	                                      "
            "                f_c.from_contact as from_contact,                                                                "
            "                f_c.from_contact_alias as from_contact_alias,                                                    "
            "                f_c.from_contact_oldnum as from_contact_oldnum,                                                  "
            "                f_c.from_contact_internum as from_contact_internum,	                                          "
            "                contact.number as to_contact,                                                                    "
            "                contact.alias as to_contact_alias,                                                               "
            "                contact.oldnum as to_contact_oldnum,                                                             "
            "                contact.internum as to_contact_internum,                                                         "
            "                t.number as to_telephone,                                                                        "
            "                t.oldnum as to_telephone_oldnum,                                                                 "
            "                t.internum as to_telephone_internum,                                                             "
            "                p.name as to_name,                                                                               "
            "                p.lastname as to_lastname,                                                                       "
            "                p.midname as to_midname,                                                                         "
            "                e.category as to_event_category,                                                                 "
            "                e.detention_date as to_event_detention_date,                                                     "
            "                e.detention_time as to_event_detention_time,                                                     "
            "                e.detention_reason as to_event_detention_reason,                                                 "
            "                e.detention_by as to_event_detention_by,                                                         "
            "                e.keeping_place as to_event_keeping_place,                                                       "
            "                e.additional as to_event_keeping_additional,                                                     "
            "                e.id as to_event_id                                                                              "
            "            FROM notebook2.contact                                                                               "
            "            INNER JOIN                                                                                           "
            "                (                                                                                                "
            "                    SELECT  DISTINCT                                                                             "
            "                                f_t.from_event_id as from_event_id,                                              "
            "                                f_t.from_event_category as from_event_category,                                  "
            "                                f_t.from_event_detention_date as from_event_detention_date,                      "
            "                                f_t.from_event_detention_time as from_event_detention_time,                      "
            "                                f_t.from_event_detention_reason as from_event_detention_reason,                  "
            "                                f_t.from_event_detention_by as from_event_detention_by,                          "
            "                                f_t.from_event_keeping_place as from_event_keeping_place,                        "
            "                                f_t.from_event_keeping_additional as from_event_keeping_additional,              "
            "                                f_t.from_name as from_name,                                                      "
            "                                f_t.from_lastname as from_lastname,                                              "
            "                                f_t.from_midname as from_midname,                                                "
            "                                f_t.from_telephone as from_telephone,                                            "
            "                                f_t.from_telephone_oldnum as from_telephone_oldnum,                              "
            "                                f_t.from_telephone_internum as from_telephone_internum,		                  "
            "                                f_t.telephone_id as from_telephone_id,                                           "
            "                                c.number as from_contact,                                                        "
            "                                c.alias as from_contact_alias,                                                   "
            "                                c.oldnum as from_contact_oldnum,                                                 "
            "                                c.internum as from_contact_internum		                                      "
            "                            FROM  notebook2.contact as c                                                         "
            "                            INNER JOIN                                                                           "
            "                            (                                                                                    "
            "                                SELECT 	DISTINCT                                                              "
            "                                events.from_event_id as from_event_id,                                           "
            "                                events.from_event_category as from_event_category,                               "
            "                                events.from_event_detention_date as from_event_detention_date,                   "
            "                                events.from_event_detention_time as from_event_detention_time,                   "
            "                                events.from_event_detention_reason as from_event_detention_reason,               "
            "                                events.from_event_detention_by as from_event_detention_by,                       "
            "                                events.from_event_keeping_place as from_event_keeping_place,                     "
            "                                events.from_event_keeping_additional as from_event_keeping_additional,           "
            "                                person.id,                                                                       "
            "                                person.name as from_name,                                                        "
            "                                person.lastname as from_lastname,                                                "
            "                                person.midname as from_midname,                                                  "
            "                                t.number as from_telephone,                                                      "
            "                                t.oldnum as from_telephone_oldnum,                                               "
            "                                t.internum as from_telephone_internum,					                          "
            "                                t.id as telephone_id                                                             "
            "                                FROM notebook2.person                                                            "
            "                                    INNER JOIN (                                                                 "
            "                                                SELECT *                                                         "
            "                                                FROM notebook2.telephone                                         "
            "                                            )as t                                                                "
            "                                            on t.person_id = person.id                                           "
            "                                , notebook2.event_person                                                         "
            "                                    INNER JOIN (                                                                 "
            "                                            SELECT event.category as from_event_category,                        "
            "                                                event.detention_date as from_event_detention_date,               "
            "                                                event.detention_time as from_event_detention_time,               "
            "                                                event.detention_reason as from_event_detention_reason,           "
            "                                                event.detention_by as from_event_detention_by,                   "
            "                                                event.keeping_place as from_event_keeping_place,                 "
            "                                                event.additional as from_event_keeping_additional,               "
            "                                                event.id as from_event_id                                        "
            "                                                FROM notebook2.event                                             "
            "                                            WHERE id = "+eventId+") as events                                    "
            "                                            ON event_person.event_id = events.from_event_id                      "
            "                            ) as f_t                                                                             "
            "                                ON f_t.telephone_id = c.telephone_id ) as f_c                                    "
            "                                ON f_c.from_contact = contact.number,                                            "
            "                                notebook2.telephone as t,                                                        "
            "                                notebook2.person as p,                                                           "
            "                                notebook2.event_person as ep,                                                    "
            "                                notebook2.event as e					                                          "
            "                                WHERE contact.telephone_id != f_c.from_telephone_id                              "
            "                                AND contact.telephone_id = t.id                                                  "
            "                                AND t.person_id = p.id                                                           "
            "                                AND p.id = ep.person_id                                                          "
            "                                AND ep.event_id = e.id                                                           "
            "                                AND e.id != "+eventId+"                                                          ";

    for (int i=0; i<eventIdList->size(); i++){
        if(i==0) mainQuery += "AND (";

        mainQuery += "e.id == "+QString(eventIdList->at(i));
        if(i<eventIdList->size()-1) mainQuery += " OR ";
        if(i == eventIdList->size()-1) mainQuery += ");";
    }
    return mainQuery;
}

Analysis_general *AnalysisModels::convert(Analysis_V1 *an)
{
    Analysis_general *g = new Analysis_general();

    g->analysis_type                  = 1                                ;
    g->from_event_id                  = an->from_event_id                ;
    g->from_event_category            = an->from_event_category          ;
    g->from_event_detention_date      = an->from_event_detention_date    ;
    g->from_event_detention_time      = an->from_event_detention_time    ;
    g->from_event_detention_reason    = an->from_event_detention_reason  ;
    g->from_event_detention_by        = an->from_event_detention_by      ;
    g->from_event_keeping_place       = an->from_event_keeping_place     ;
    g->from_event_keeping_additional  = an->from_event_keeping_additional;
    g->from_name                      = an->from_name                    ;
    g->from_lastname                  = an->from_lastname                ;
    g->from_midname                   = an->from_midname                 ;
    g->from_telephone                 = an->from_telephone               ;
    g->from_telephone_oldnum          = an->from_telephone_oldnum        ;
    g->from_telephone_internum        = an->from_telephone_internum      ;
    g->to_contact                     = an->to_contact                   ;
    g->to_contact_alias               = an->to_contact_alias             ;
    g->to_contact_oldnum              = an->to_contact_oldnum            ;
    g->to_contact_internum            = an->to_contact_internum          ;
    g->to_telephone                   = an->to_telephone                 ;
    g->to_telephone_oldnum            = an->to_telephone_oldnum          ;
    g->to_telephone_internum          = an->to_telephone_internum        ;
    g->to_name                        = an->to_name                      ;
    g->to_lastname                    = an->to_lastname                  ;
    g->to_midname                     = an->to_midname                   ;
    g->to_event_category              = an->to_event_category            ;
    g->to_event_detention_date        = an->to_event_detention_date      ;
    g->to_event_detention_time        = an->to_event_detention_time      ;
    g->to_event_detention_reason      = an->to_event_detention_reason    ;
    g->to_event_detention_by          = an->to_event_detention_by        ;
    g->to_event_keeping_place         = an->to_event_keeping_place       ;
    g->to_event_keeping_additional    = an->to_event_keeping_additional  ;
    g->to_event_id                    = an->to_event_id                  ;

    return g;
}

Analysis_general *AnalysisModels::convert(Analysis_V2 *an)
{
     Analysis_general *g = new Analysis_general();

     g->analysis_type                  = 2                                ;
     g->from_event_id                  = an->from_event_id                ;
     g->from_event_category            = an->from_event_category          ;
     g->from_event_detention_date      = an->from_event_detention_date    ;
     g->from_event_detention_time      = an->from_event_detention_time    ;
     g->from_event_detention_reason    = an->from_event_detention_reason  ;
     g->from_event_detention_by        = an->from_event_detention_by      ;
     g->from_event_keeping_place       = an->from_event_keeping_place     ;
     g->from_event_keeping_additional  = an->from_event_keeping_additional;
     g->from_name                      = an->from_name                    ;
     g->from_lastname                  = an->from_lastname                ;
     g->from_midname                   = an->from_midname                 ;
     g->from_telephone                 = an->from_telephone               ;
     g->from_telephone_oldnum          = an->from_telephone_oldnum        ;
     g->from_telephone_internum        = an->from_telephone_internum      ;
     g->from_contact                   = an->from_contact                 ;
     g->from_contact_alias             = an->from_contact_alias           ;
     g->from_contact_oldnum            = an->from_contact_oldnum          ;
     g->from_contact_internum          = an->from_contact_internum        ;
     g->to_telephone                   = an->to_telephone                 ;
     g->to_telephone_oldnum            = an->to_telephone_oldnum          ;
     g->to_telephone_internum          = an->to_telephone_internum        ;
     g->to_name                        = an->to_name                      ;
     g->to_lastname                    = an->to_lastname                  ;
     g->to_midname                     = an->to_midname                   ;
     g->to_event_category              = an->to_event_category            ;
     g->to_event_detention_date        = an->to_event_detention_date      ;
     g->to_event_detention_time        = an->to_event_detention_time      ;
     g->to_event_detention_reason      = an->to_event_detention_reason    ;
     g->to_event_detention_by          = an->to_event_detention_by        ;
     g->to_event_keeping_place         = an->to_event_keeping_place       ;
     g->to_event_keeping_additional    = an->to_event_keeping_additional  ;
     g->to_event_id                    = an->to_event_id                  ;

     return g;

}

Analysis_general *AnalysisModels::convert(Analysis_V3 *an)
{
     Analysis_general *g = new Analysis_general();

     g->analysis_type                    = 3                                ;
     g->from_event_id                    = an->from_event_id                ;
     g->from_event_category              = an->from_event_category          ;
     g->from_event_detention_date        = an->from_event_detention_date    ;
     g->from_event_detention_time        = an->from_event_detention_time    ;
     g->from_event_detention_reason      = an->from_event_detention_reason  ;
     g->from_event_detention_by          = an->from_event_detention_by      ;
     g->from_event_keeping_place         = an->from_event_keeping_place     ;
     g->from_event_keeping_additional    = an->from_event_keeping_additional;
     g->from_name                        = an->from_name                    ;
     g->from_lastname                    = an->from_lastname                ;
     g->from_midname                     = an->from_midname                 ;
     g->from_telephone                   = an->from_telephone               ;
     g->from_telephone_oldnum            = an->from_telephone_oldnum        ;
     g->from_telephone_internum          = an->from_telephone_internum      ;
     g->from_contact                     = an->from_contact                 ;
     g->from_contact_alias               = an->from_contact_alias           ;
     g->from_contact_oldnum              = an->from_contact_oldnum          ;
     g->from_contact_internum            = an->from_contact_internum        ;
     g->to_contact                       = an->to_contact                   ;
     g->to_contact_alias                 = an->to_contact_alias             ;
     g->to_contact_oldnum                = an->to_contact_oldnum            ;
     g->to_contact_internum              = an->to_contact_internum          ;
     g->to_telephone                     = an->to_telephone                 ;
     g->to_telephone_oldnum              = an->to_telephone_oldnum          ;
     g->to_telephone_internum            = an->to_telephone_internum        ;
     g->to_name                          = an->to_name                      ;
     g->to_lastname                      = an->to_lastname                  ;
     g->to_midname                       = an->to_midname                   ;
     g->to_event_category                = an->to_event_category            ;
     g->to_event_detention_date          = an->to_event_detention_date      ;
     g->to_event_detention_time          = an->to_event_detention_time      ;
     g->to_event_detention_reason        = an->to_event_detention_reason    ;
     g->to_event_detention_by            = an->to_event_detention_by        ;
     g->to_event_keeping_place           = an->to_event_keeping_place       ;
     g->to_event_keeping_additional      = an->to_event_keeping_additional  ;
     g->to_event_id                      = an->to_event_id                  ;

     return g;
}

QString AnalysisModels::v1(QString eventId)
{
    return
"            SELECT DISTINCT 	f_t.from_event_id as from_event_id,                                        "
"                                f_t.from_event_category as from_event_category,                           "
"                                f_t.from_event_detention_date as from_event_detention_date,               "
"                                f_t.from_event_detention_time as from_event_detention_time,               "
"                                f_t.from_event_detention_reason as from_event_detention_reason,           "
"                                f_t.from_event_detention_by as from_event_detention_by,                   "
"                                f_t.from_event_keeping_place as from_event_keeping_place,                 "
"                                f_t.from_event_keeping_additional as from_event_keeping_additional,       "
"                                f_t.from_name as from_name,                                               "
"                                f_t.from_lastname as from_lastname,                                       "
"                                f_t.from_midname as from_midname,                                         "
"                                f_t.from_telephone as from_telephone,                                     "
"                                f_t.from_telephone_oldnum as from_telephone_oldnum,                       "
"                                f_t.from_telephone_internum as from_telephone_internum,                   "
"                                contact.number as to_contact,                                             "
"                                contact.alias as to_conntact_alias,                                       "
"                                contact.oldnum as to_conntact_oldnum,                                     "
"                                contact.internum as to_conntact_internum,                                 "
"                                t.number as to_telephone,                                                 "
"                                t.oldnum  as to_telephone_oldnum,                                         "
"                                t.internum  as to_telephone_internum,                                     "
"                                p.name  as to_name,                                                       "
"                                p.lastname as to_lastname,                                                "
"                                p.midname as to_midname,                                                  "
"                                e.category as to_event_category,                                          "
"                                e.detention_date as to_event_detention_date,                              "
"                                e.detention_time as to_event_detention_time,                              "
"                                e.detention_reason as to_event_detention_reason,                          "
"                                e.detention_by as to_event_detention_by,                                  "
"                                e.keeping_place as to_event_keeping_place,                                "
"                                e.additional as to_event_keeping_additional,                              "
"                                e.id as to_event_id                                                       "
"            FROM notebook2.event as e,                                                                    "
"            notebook2.event_person as ep,                                                                 "
"            notebook2.person as p,                                                                        "
"            notebook2.telephone as t,                                                                     "
"            notebook2.contact                                                                             "
"            INNER JOIN                                                                                    "
"                (SELECT DISTINCT                                                                          "
"                                events.id as from_event_id,                                               "
"                                events.category as from_event_category,                                   "
"                                events.detention_date as from_event_detention_date,                       "
"                                events.detention_time as from_event_detention_time,                       "
"                                events.detention_reason as from_event_detention_reason,                   "
"                                events.detention_by as from_event_detention_by,                           "
"                                events.keeping_place as from_event_keeping_place,                         "
"                                events.additional as from_event_keeping_additional,                       "
"                                person.id,                                                                "
"                                person.name as from_name,                                                 "
"                                person.lastname as from_lastname,                                         "
"                                person.midname as from_midname,                                           "
"                                t.number as from_telephone,                                               "
"                                t.oldnum as from_telephone_oldnum,                                        "
"                                t.internum as from_telephone_internum,                                    "
"                                t.id as telephone_id                                                      "
"                                FROM notebook2.person                                                     "
"                                , notebook2.event_person as ep,                                           "
"                                notebook2.telephone as t,                                                 "
"                                notebook2.event as events                                                 "
"                                WHERE events.id = "+eventId+"                                             "
"                                AND ep.event_id = events.id                                               "
"                                AND ep.person_id = person.id                                              "
"                                AND t.person_id = person.id) as f_t                                       "
"                                            ON f_t.from_telephone = contact.number                        "
"                                            WHERE contact.telephone_id = t.id                             "
"                                            AND t.person_id = p.id                                        "
"                                            AND p.id = ep.person_id                                       "
"                                            AND ep.event_id = e.id                                        "
"                                            AND e.id != "+eventId+"                                       ";
}

QString AnalysisModels::v2(QString eventId)
{
    return
"            SELECT                                                                                    "
"                            f_c.from_event_id as from_event_id,                                       "
"                            f_c.from_event_category as from_event_category,                           "
"                            f_c.from_event_detention_date as from_event_detention_date,               "
"                            f_c.from_event_detention_time as from_event_detention_time,               "
"                            f_c.from_event_detention_reason as from_event_detention_reason,           "
"                            f_c.from_event_detention_by as from_event_detention_by,                   "
"                            f_c.from_event_keeping_place as from_event_keeping_place,                 "
"                            f_c.from_event_keeping_additional as from_event_keeping_additional,       "
"                            f_c.from_name as from_name,                                               "
"                            f_c.from_lastname as from_lastname,                                       "
"                            f_c.from_midname as from_midname,                                         "
"                            f_c.from_telephone as from_telephone,                                     "
"                            f_c.from_telephone_oldnum as from_telephone_oldnum,                       "
"                            f_c.from_telephone_internum as from_telephone_internum,                   "
"                            f_c.from_contact as from_contact,                                         "
"                            f_c.from_contact_alias as from_contact_alias,                             "
"                            f_c.from_contact_oldnum as from_contact_oldnum,                           "
"                            f_c.from_contact_internum as from_contact_internum,                       "
"                            telephone.number as to_telephone,                                         "
"                            telephone.oldnum as to_telephone_oldnum,                                  "
"                            telephone.internum as to_telephone_internum,				               "
"                            person.name as to_name,                                                   "
"                            person.lastname as to_lastname,                                           "
"                            person.midname as to_midname,                                             "
"                            event.category as to_event_category,                                      "
"                            event.detention_date as to_event_detention_date,                          "
"                            event.detention_time as to_event_detention_time,                          "
"                            event.detention_reason as to_event_detention_reason,                      "
"                            event.detention_by as to_event_detention_by,                              "
"                            event.keeping_place as to_event_keeping_place,                            "
"                            event.additional as to_event_keeping_additional,                          "
"                            event.id as to_event_id                                                   "
"            FROM notebook2.telephone                                                                  "
"            INNER JOIN                                                                                "
"            (                                                                                         "
"                SELECT DISTINCT                                                                       "
"                                events.id as from_event_id,                                           "
"                                events.category as from_event_category,                               "
"                                events.detention_date as from_event_detention_date,                   "
"                                events.detention_time as from_event_detention_time,                   "
"                                events.detention_reason as from_event_detention_reason,               "
"                                events.detention_by as from_event_detention_by,                       "
"                                events.keeping_place as from_event_keeping_place,                     "
"                                events.additional as from_event_keeping_additional,                   "
"                                person.id,                                                            "
"                                person.name as from_name,                                             "
"                                person.lastname as from_lastname,                                     "
"                                person.midname as from_midname,                                       "
"                                t.number as from_telephone,                                           "
"                                t.oldnum as from_telephone_oldnum,                                    "
"                                t.internum as from_telephone_internum,                                "
"                                t.id as telephone_id,                                                 "
"                                c.number as from_contact,                                             "
"                                c.alias as from_contact_alias,                                        "
"                                c.oldnum as from_contact_oldnum,                                      "
"                                c.internum as from_contact_internum                                   "
"                                FROM notebook2.person                                                 "
"                                , notebook2.event_person as ep,                                       "
"                                notebook2.telephone as t,                                             "
"                                notebook2.event as events,                                            "
"                                notebook2.contact as c                                                "
"                                WHERE events.id = "+eventId+"                                         "
"                                AND ep.event_id = events.id                                           "
"                                AND ep.person_id = person.id                                          "
"                                AND t.person_id = person.id                                           "
"                                AND c.telephone_id = t.id                                             "
"            ) as f_c                                                                                  "
"                            ON f_c.from_contact = telephone.number,                                   "
"            notebook2.person,                                                                         "
"            notebook2.event_person,                                                                   "
"            notebook2.event                                                                           "
"            WHERE person.id = telephone.person_id                                                     "
"            AND event_person.person_id = person.id                                                    "
"            AND event.id=event_person.event_id                                                        "
"            AND event.id != "+eventId+"                                                                ";

}

QString AnalysisModels::v3(QString eventId)
{
    return
"            SELECT DISTINCT                                                                       "
"                f_c.from_event_id as from_event_id,                                               "
"                f_c.from_event_category as from_event_category,                                   "
"                f_c.from_event_detention_date as from_event_detention_date,                       "
"                f_c.from_event_detention_time as from_event_detention_time,                       "
"                f_c.from_event_detention_reason as from_event_detention_reason,                   "
"                f_c.from_event_detention_by as from_event_detention_by,                           "
"                f_c.from_event_keeping_place as from_event_keeping_place,                         "
"                f_c.from_event_keeping_additional as from_event_keeping_additional,               "
"                f_c.from_name as from_name,                                                       "
"                f_c.from_lastname as from_lastname,                                               "
"                f_c.from_midname as from_midname,                                                 "
"                f_c.from_telephone as from_telephone,                                             "
"                f_c.from_telephone_oldnum as from_telephone_oldnum,                               "
"                f_c.from_telephone_internum as from_telephone_internum,	                       "
"                f_c.from_contact as from_contact,                                                 "
"                f_c.from_contact_alias as from_contact_alias,                                     "
"                f_c.from_contact_oldnum as from_contact_oldnum,                                   "
"                f_c.from_contact_internum as from_contact_internum,	                           "
"                contact.number as to_contact,                                                     "
"                contact.alias as to_contact_alias,                                                "
"                contact.oldnum as to_contact_oldnum,                                              "
"                contact.internum as to_contact_internum,                                          "
"                t.number as to_telephone,                                                         "
"                t.oldnum as to_telephone_oldnum,                                                  "
"                t.internum as to_telephone_internum,                                              "
"                p.name as to_name,                                                                "
"                p.lastname as to_lastname,                                                        "
"                p.midname as to_midname,                                                          "
"                e.category as to_event_category,                                                  "
"                e.detention_date as to_event_detention_date,                                      "
"                e.detention_time as to_event_detention_time,                                      "
"                e.detention_reason as to_event_detention_reason,                                  "
"                e.detention_by as to_event_detention_by,                                          "
"                e.keeping_place as to_event_keeping_place,                                        "
"                e.additional as to_event_keeping_additional,                                      "
"                e.id as to_event_id                                                               "
"            FROM notebook2.contact                                                                "
"            INNER JOIN                                                                            "
"                (                                                                                 "
"                    SELECT  DISTINCT                                                              "
"                                e.id as from_event_id,                                            "
"                                e.category as from_event_category,                                "
"                                e.detention_date as from_event_detention_date,                    "
"                                e.detention_time as from_event_detention_time,                    "
"                                e.detention_reason as from_event_detention_reason,                "
"                                e.detention_by as from_event_detention_by,                        "
"                                e.keeping_place as from_event_keeping_place,                      "
"                                e.additional as from_event_keeping_additional,                    "
"                                p.name as from_name,                                              "
"                                p.lastname as from_lastname,                                      "
"                                p.midname as from_midname,                                        "
"                                t.number as from_telephone,                                       "
"                                t.oldnum as from_telephone_oldnum,                                "
"                                t.internum as from_telephone_internum,		                       "
"                                t.id as from_telephone_id,                                        "
"                                c.number as from_contact,                                         "
"                                c.alias as from_contact_alias,                                    "
"                                c.oldnum as from_contact_oldnum,                                  "
"                                c.internum as from_contact_internum		                       "
"                            FROM                                                                  "
"                            notebook2.event as e,                                                 "
"                            notebook2.contact as c,                                               "
"                            notebook2.telephone as t,                                             "
"                            notebook2.person as p,                                                "
"                            notebook2.event_person as ep                                          "
"                            WHERE e.id = "+eventId+"                                                       "
"                            AND e.id = ep.event_id                                                "
"                            AND ep.person_id = p.id                                               "
"                            AND t.person_id = p.id                                                "
"                            AND c.telephone_id = t.id                                             "
"                                                                                                  "
"                    ) as f_c                                                                      "
"                                ON f_c.from_contact = contact.number,                             "
"                                notebook2.telephone as t,                                         "
"                                notebook2.person as p,                                            "
"                                notebook2.event_person as ep,                                     "
"                                notebook2.event as e					                           "
"                                WHERE contact.telephone_id != f_c.from_telephone_id               "
"                                AND contact.telephone_id = t.id                                   "
"                                AND t.person_id = p.id                                            "
"                                AND p.id = ep.person_id                                           "
"                                AND ep.event_id = e.id                                            "
"                                AND e.id != "+eventId+"                                           ";
}

AnalysisModels::AnalysisModels()
{

}

Analysis_general::Analysis_general()
{
    this->analysis_type                    = 0      ;
    this->from_event_id                    = ""     ;
    this->from_event_category              = ""     ;
    this->from_event_detention_date        = ""     ;
    this->from_event_detention_time        = ""     ;
    this->from_event_detention_reason      = ""     ;
    this->from_event_detention_by          = ""     ;
    this->from_event_keeping_place         = ""     ;
    this->from_event_keeping_additional    = ""     ;
    this->from_name                        = ""     ;
    this->from_lastname                    = ""     ;
    this->from_midname                     = ""     ;
    this->from_telephone                   = ""     ;
    this->from_telephone_oldnum            = false  ;
    this->from_telephone_internum          = false  ;
    this->from_contact                     = ""     ;
    this->from_contact_alias               = ""     ;
    this->from_contact_oldnum              = false  ;
    this->from_contact_internum            = false  ;
    this->to_contact                       = ""     ;
    this->to_contact_alias                 = ""     ;
    this->to_contact_oldnum                = false  ;
    this->to_contact_internum              = false  ;
    this->to_telephone                     = ""     ;
    this->to_telephone_oldnum              = false  ;
    this->to_telephone_internum            = false  ;
    this->to_name                          = ""     ;
    this->to_lastname                      = ""     ;
    this->to_midname                       = ""     ;
    this->to_event_category                = ""     ;
    this->to_event_detention_date          = ""     ;
    this->to_event_detention_time          = ""     ;
    this->to_event_detention_reason        = ""     ;
    this->to_event_detention_by            = ""     ;
    this->to_event_keeping_place           = ""     ;
    this->to_event_keeping_additional      = ""     ;
    this->to_event_id                      = ""     ;
}
