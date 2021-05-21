#ifndef LINKEDPERSON_H
#define LINKEDPERSON_H

#include <QWidget>
#include "_Person.h"

namespace Ui {
class LinkedPerson;
}

class LinkedPerson : public QWidget
{
    Q_OBJECT

public:
    explicit LinkedPerson(QWidget *parent = nullptr);
    ~LinkedPerson();
    void setPerson(Person *p);

signals:
    void openPerson(Person*);
    void deleteLink(Person *);

private slots:
    void on_pb_openView_clicked();

    void on_pb_unlink_clicked();

private:
    Ui::LinkedPerson *ui;
    Person *person;
};

#endif // LINKEDPERSON_H
