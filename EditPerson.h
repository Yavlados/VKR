#ifndef EDITPERSON_H
#define EDITPERSON_H

#include <QWidget>
#include "_Person.h"

namespace Ui {
class EditPerson;
}

class EditPerson : public QWidget
{
    Q_OBJECT

public:
    explicit EditPerson(QWidget *parent = nullptr);
    void setPerson(Person *person);

    ~EditPerson();
    Person *person;
private:
    Ui::EditPerson *ui;

};

#endif // EDITPERSON_H
