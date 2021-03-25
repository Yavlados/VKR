#ifndef FOR_SEARCH_H
#define FOR_SEARCH_H
#include <QString>
#include "db_connection.h"

struct searchState{
    bool event;
    bool person;
    bool telephone;
    bool contact;
    searchState();
};

class For_search
{
public:

    QString event_id;
    QString event_category;
    QString event_detention_date;
    QString event_detention_time;
    QString event_detention_reason;
    QString event_detention_by;
    QString event_keeping_place;
    QString event_detention_from;
    QString event_detention_to;

    QString person_lastname;
    QString person_name;
    QString person_midname;
    QString person_alias;

    QString telephone_num;

    QString contact_num;
    QString contact_alias;

    searchState state;
    For_search();
    void updateState();
    bool search();
    const QString updateQuery();

    QString updateQueryEventFields(QString newQuery);
    QString updateQueryPersonFields(QString newQuery);
    QString updateQueryTelephoneFields(QString newQuery);
    QString updateQueryContactFields(QString newQuery);

private:
    QString query;
    QList<QString> searchedIds;
    void setInitialQuery();
};

#endif // FOR_SEARCH_H
