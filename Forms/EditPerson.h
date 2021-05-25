#ifndef EDITPERSON_H
#define EDITPERSON_H

#include <QWidget>
#include "_Person.h"
#include <QKeyEvent>
#include <QMessageBox>
#include "_MTM_Telephone.h"
#include "_MTM_Contact.h"
#include "popup.h"
#include "component.h"
#include "linksmanager.h"
#include "linkedperson.h"
#include "custommsgbox.h"

//#include "editEvent.h"

namespace Ui {
class EditPerson;
}

enum formStates { updatePerson =0,
                 addPerson};

enum editPersonResults{
    addPersonEPResult,
    personMergedEPResult,
};

class EditPerson : public QWidget
{
    Q_OBJECT

public:
    explicit EditPerson(QWidget *parent = 0);
    void setPerson(Person *person);

    ~EditPerson();
    Person *person;
    Person *editablePerson;
    QList<comparsionResult*> *comparsion;
    formStates state;

    MTM_Telephone *ot_model;
    MTM_Contact *contacts_model;
    editPersonResults editResult;

    void keyPressEvent(QKeyEvent *event);
    void setType(formStates s);

    void setFocus();

    QMessageBox msgbx;
signals:
    void closeThis(EditPerson*);
    void personIsAdded(EditPerson*);
    void openLinkedPerson(Person*);

private slots:
    void on_tableView_clicked(const QModelIndex &index);

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_pb_add_line_telephone_clicked();

    void on_pb_del_line_telephone_clicked();

    void on_tableView_2_clicked(const QModelIndex &index);

    void on_tableView_2_doubleClicked(const QModelIndex &index);

    void on_pb_add_contact_line_clicked();

    void on_pb_del_contact_line_clicked();

    void setFocusOnTab1();
    void setFocusOnTab2();

    void set_tab_orders();

    void on_pb_save_clicked();

    void on_pb_cancel_clicked();

    void closeWidget();
    void ShowPopUp();    
    void on_gb_adress_liv_toggled(bool arg1);
    void copyAdresses();
    bool checkLinksBeforeSave(bool isUpdate);
    bool handleLinks();
    void emitOpenLinkedPerson(Person *person);
    void destroyLinkBetweenPersons(Person *person);
private:
    Ui::EditPerson *ui;
    void fillFields();
    void fillLinkedPersons();


};

#endif // EDITPERSON_H
