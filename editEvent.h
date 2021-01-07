#ifndef EDITEVENT_H
#define EDITEVENT_H
#include "_Event.h"
#include "date_form.h"
#include "PersonCard.h"
#include <QWidget>
#include <QMessageBox>


enum editEventState {
    updateEvent,
    addEvent
};

namespace Ui {
class editEvent;
}

class editEvent : public QWidget
{
    Q_OBJECT

public:
    explicit editEvent(QWidget *parent = nullptr);
    ~editEvent();

    void setEventAndType(Event *e, editEventState s);
    Event *localEvent;
    editEventState state;
    int newPersonCounter;
    QMessageBox msgbx;


public slots:
    void updateCardsLayout();
    void clearLayout();
    void addNewPerson(Person *p);
    void closeWidget();

private slots:
    void on_pb_add_person_clicked();

    void on_pb_cancel_clicked();


    void on_pb_save_clicked();

private:
    Ui::editEvent *ui;
    Date_form *d;

signals:
    void addPerson(Person*, editEvent*);
    void closeThis(editEvent *);
};

#endif // EDITEVENT_H
