#ifndef ADD_FORM_H
#define ADD_FORM_H
#include "MyTableModel.h"
#include "update_Form.h"

#include <QWidget>
#include <QIntValidator>
#include <QDate>
#include <QtDebug>
#include <QMessageBox>
#include <QLineEdit>
#include <QSqlTableModel>
#include <QSqlQuery>
namespace Ui {
class Add_form;
}

class Add_form : public QWidget
{
    Q_OBJECT

public:
    QString adres_reg;
    QString adres_liv;
    QMessageBox msgbx;

    MyTableModel *contacts_model = new MyTableModel;
    MyTableModel *ot_model = new MyTableModel;

    QList<Contacts*> *contactList = new QList<Contacts*>;
    QList<Owners_tel*> *otList = new QList<Owners_tel*>;

   explicit Add_form(QWidget *parent = nullptr);
    ~Add_form();

private slots:

    void on_pb_back_to_main_clicked();

    void on_pb_remove_line_telephone_clicked();

    void on_pb_add_contact_line_clicked();

    void on_pb_remove_contact_line_clicked();

    void Fill_table_in_add();

    void on_pb_add_zk_final_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void set_validators();
    //Слот очистки

    void Clear();

    //Слот Проверки добавления телефона

    void on_pushButton_clicked();

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_checkBox_clicked();

signals:

    void open_update_tab(int);

    void Add_contact_row(int);

    void toMainForm();

private:
    Ui::Add_form *ui;
};

#endif // ADD_FORM_H
