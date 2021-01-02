#ifndef PERSONCARD_H
#define PERSONCARD_H
#include "_Person.h"
#include <QWidget>
#include "EditPerson.h"

namespace Ui {
class PersonCard;
}

class PersonCard : public QWidget
{
    Q_OBJECT

public:
    explicit PersonCard(QWidget *parent = nullptr);
    ~PersonCard();
    void setPerson(Person *person);

signals:
    void openEditWindow(Person*);

private slots:
    void on_editButton_clicked();

private:
    Ui::PersonCard *ui;
    Person *localPerson;
};

#endif // PERSONCARD_H
