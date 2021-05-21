#ifndef PERSONCARD_H
#define PERSONCARD_H
#include "_Person.h"
#include <QWidget>
#include "EditPerson.h"
#include <QKeyEvent>
#include <QDebug>
/// REFACTORED

enum personCardShowingState{
    mainWindow,
    editEventWindow
};

namespace Ui {
class PersonCard;
}

class PersonCard : public QWidget
{
    Q_OBJECT

public:
    explicit PersonCard(QWidget *parent = 0);
    ~PersonCard();
    void setPerson(Person *person);
    void close();
    void setState(personCardShowingState t);
signals:
    void openEditWindow(Person*);
    void removePerson(Person *);

private slots:
    void on_editButton_clicked();
    void on_pb_button_clicked();

private:
    Ui::PersonCard *ui;
    Person *localPerson;
    personCardShowingState state;
};

#endif // PERSONCARD_H
