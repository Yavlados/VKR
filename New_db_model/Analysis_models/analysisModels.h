#ifndef ASD_H
#define ASD_H
#include <QString>
#include <QList>
#include <QVector>

struct Analysis_ContactData{
    QString from_contact;
    QString from_contact_alias;
    bool    from_contact_oldnum;
    bool    from_contact_internum;
    QList<Analysis_ContactData*> *to_contacts;
};

struct Analysis_TelephoneData{
    QString from_telephone;
    bool    from_telephone_oldnum;
    bool    from_telephone_internum;
    QList<Analysis_TelephoneData*> *to_telephones;
    QList<Analysis_ContactData*> *contacts;
    QList<Analysis_ContactData*> *to_contacts;
};

struct Analysis_PersonData{
    QString from_name;
    QString from_lastname;
    QString from_midname;
    QList<Analysis_TelephoneData*> *telephones;
};

struct Analysis_res {
    QString from_event_id;
    QString from_event_category;
    QString from_event_detention_date;
    QString from_event_detention_time;
    QString from_event_detention_reason;
    QString from_event_detention_by;
    QString from_event_keeping_place;
    QString from_event_keeping_additional;
    QList<Analysis_PersonData*> *persons;
};


struct Analysis_V1 {
    Analysis_V1();
    QString from_event_id;
    QString from_event_category;
    QString from_event_detention_date;
    QString from_event_detention_time;
    QString from_event_detention_reason;
    QString from_event_detention_by;
    QString from_event_keeping_place;
    QString from_event_keeping_additional;
    QString from_name;
    QString from_lastname;
    QString from_midname;
    QString from_telephone;
    bool    from_telephone_oldnum;
    bool    from_telephone_internum;
    QString to_contact;
    QString to_contact_alias;
    bool    to_contact_oldnum;
    bool    to_contact_internum;
    QString to_telephone;
    bool    to_telephone_oldnum;
    bool    to_telephone_internum;
    QString to_name;
    QString to_lastname;
    QString to_midname;
    QString to_event_category;
    QString to_event_detention_date;
    QString to_event_detention_time;
    QString to_event_detention_reason;
    QString to_event_detention_by;
    QString to_event_keeping_place;
    QString to_event_keeping_additional;
    QString to_event_id;
};

struct Analysis_V2 {
    Analysis_V2();
    QString from_event_id;
    QString from_event_category;
    QString from_event_detention_date;
    QString from_event_detention_time;
    QString from_event_detention_reason;
    QString from_event_detention_by;
    QString from_event_keeping_place;
    QString from_event_keeping_additional;
    QString from_name;
    QString from_lastname;
    QString from_midname;
    QString from_telephone;
    bool    from_telephone_oldnum;
    bool    from_telephone_internum;
    QString from_contact;
    QString from_contact_alias;
    bool    from_contact_oldnum;
    bool    from_contact_internum;
    QString to_telephone;
    bool    to_telephone_oldnum;
    bool    to_telephone_internum;
    QString to_name;
    QString to_lastname;
    QString to_midname;
    QString to_event_category;
    QString to_event_detention_date;
    QString to_event_detention_time;
    QString to_event_detention_reason;
    QString to_event_detention_by;
    QString to_event_keeping_place;
    QString to_event_keeping_additional;
    QString to_event_id;
};


struct Analysis_V3 {
    Analysis_V3();
    QString from_event_id                   ;
    QString from_event_category             ;
    QString from_event_detention_date       ;
    QString from_event_detention_time       ;
    QString from_event_detention_reason     ;
    QString from_event_detention_by         ;
    QString from_event_keeping_place        ;
    QString from_event_keeping_additional   ;
    QString from_name                       ;
    QString from_lastname                   ;
    QString from_midname                    ;
    QString from_telephone                  ;
    bool    from_telephone_oldnum           ;
    bool    from_telephone_internum         ;
    QString from_contact                    ;
    QString from_contact_alias              ;
    bool    from_contact_oldnum             ;
    bool    from_contact_internum           ;
    QString to_contact                      ;
    QString to_contact_alias                ;
    bool    to_contact_oldnum               ;
    bool    to_contact_internum             ;
    QString to_telephone                    ;
    bool    to_telephone_oldnum             ;
    bool    to_telephone_internum           ;
    QString to_name                         ;
    QString to_lastname                     ;
    QString to_midname                      ;
    QString to_event_category               ;
    QString to_event_detention_date         ;
    QString to_event_detention_time         ;
    QString to_event_detention_reason       ;

    QString to_event_detention_by           ;
    QString to_event_keeping_place          ;
    QString to_event_keeping_additional     ;
    QString to_event_id                     ;
};


struct Analysis_general {
    Analysis_general();
    int analysis_type ;
    QString from_event_id                   ;
    QString from_event_category             ;
    QString from_event_detention_date       ;
    QString from_event_detention_time       ;
    QString from_event_detention_reason     ;
    QString from_event_detention_by         ;
    QString from_event_keeping_place        ;
    QString from_event_keeping_additional   ;
    QString from_name                       ;
    QString from_lastname                   ;
    QString from_midname                    ;
    QString from_telephone                  ;
    bool    from_telephone_oldnum           ;
    bool    from_telephone_internum         ;
    QString from_contact                    ;
    QString from_contact_alias              ;
    bool    from_contact_oldnum             ;
    bool    from_contact_internum           ;
    QString to_contact                      ;
    QString to_contact_alias                ;
    bool    to_contact_oldnum               ;
    bool    to_contact_internum             ;
    QString to_telephone                    ;
    bool    to_telephone_oldnum             ;
    bool    to_telephone_internum           ;
    QString to_name                         ;
    QString to_lastname                     ;
    QString to_midname                      ;
    QString to_event_category               ;
    QString to_event_detention_date         ;
    QString to_event_detention_time         ;
    QString to_event_detention_reason       ;

    QString to_event_detention_by           ;
    QString to_event_keeping_place          ;
    QString to_event_keeping_additional     ;
    QString to_event_id                     ;
};

class AnalysisModels {
  public:
    static AnalysisModels *instance();

    QString v1(QString eventId);
    QString v2(QString eventId);
    QString v3(QString eventId);

    QString v1(QString eventId, QVector<int> *eventIdList);
    QString v2(QString eventId, QVector<int> *eventIdList);
    QString v3(QString eventId, QVector<int> *eventIdList);

    QString v1(QString eventId, QMap<QString, QString> *period);
    QString v2(QString eventId, QMap<QString, QString> *period);
    QString v3(QString eventId, QMap<QString, QString> *period);

    Analysis_general *convert(Analysis_V1 *an);
    Analysis_general *convert(Analysis_V2 *an);
    Analysis_general *convert(Analysis_V3 *an);

private:
    AnalysisModels();
    static AnalysisModels *_instance;
};

#endif // ASD_H
