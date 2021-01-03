#ifndef PERSONCARD_H
#define PERSONCARD_H
#include "_Person.h"
#include <QWidget>
#include "EditPerson.h"
#include <QKeyEvent>

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
    void close();

signals:
    void openEditWindow(Person*);
//    void close

private slots:
    void on_editButton_clicked();
//    void keyPressEvent(QKeyEvent *event);

private:
    Ui::PersonCard *ui;
    Person *localPerson;
};

#endif // PERSONCARD_H
