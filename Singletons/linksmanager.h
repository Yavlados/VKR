#ifndef LINKSMANAGER_H
#define LINKSMANAGER_H
#include "_Person.h"
#include <QSet>

enum linkReason {
    initial,
    telephone,
    personal
};

enum comparsionSolution{
    initial_solution,
    link_solution,
    ignore_solution
};

struct comparsionResult {
    QString message;
    linkReason reason;
    Person* person;
    QList<QString> *telephones;
    QSet<QString> *events;
    comparsionSolution solution;

    comparsionResult();
};

class LinksManager
{
public:
   static LinksManager *instance();


   static QList<comparsionResult*> *findLinks(Person *person);
   static int getPersonIndex(QList<Person*> *personsList, Person *person);
   QList<comparsionResult*> *updateMessages(QList<comparsionResult *> *compResults);
   static bool createLinks(Person *person, QList<comparsionResult*> *list);

private:
    LinksManager();
    static LinksManager *_instance;
};

#endif // LINKSMANAGER_H
