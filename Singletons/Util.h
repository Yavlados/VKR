#ifndef UTIL_H
#define UTIL_H
#include <QLayout>
#include "EditPerson.h"
#include "editEvent.h"
#include <QList>

struct AddEventManager {
    editEvent *parent;
    QList<EditPerson*> *childs;
    AddEventManager();
};

class Util
{
public:
   ~Util();
    static Util *instance();
    void clearLayout(QLayout *layout);
    QList<EditPerson *> *editPersonList();
    QList<editEvent *> *editEventList();

    QList<AddEventManager *> *addEventManager();

    /// Срабатывает при открытии editPerson из editEvent
    void linkAddEventPerson(editEvent *ee, EditPerson *ep);

    /// Срабатывает при закрытии editEvent
    void unlinkAddEventPerson(editEvent *ee);

    /// Срабатывает при закрытии EditPerson
    editEvent *getManagerParent(EditPerson*);

    /// Конвертирует стринг в набор чисел, нужно для ключа
    QString convertKey(QString key);

    /// Преобразует wildcards и lowercase для входящего значения (нужно для поискового запроса)
    QString modifyForSearch(QString value);
private:
     Util();
     static Util *_instance;
     QList<EditPerson*> *_editPersonList;

     QList<editEvent*>  *_editEventList;

     QList<AddEventManager*> *_addEventManager;

};

#endif // UTIL_H
