#ifndef ADD_FORM_H
#define ADD_FORM_H

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

    int zk_id;
    int tel_id;
    QString tel_num;


   QSqlTableModel* model;
   QSqlTableModel* model_2;
   explicit Add_form(QWidget *parent = nullptr);
    ~Add_form();

private slots:

    void radio_button1_checked();

    void radio_button2_checked();

    void on_pb_back_to_main_clicked();

    void on_pb_add_line_telephone_clicked();

    void on_pb_del_line_telephone_clicked();

    void on_pb_add_contact_line_clicked();

    void on_pb_remove_contact_line_clicked();

    void add_abort();

    void Fill_table_in_add();

    void on_pb_add_telephone_clicked();

    void on_pb_add_zk_final_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_pb_add_contact_clicked();

    //Слот очистки

    void Clear();

    //Слот Проверки добавления телефона

    void Check_accept();

    void on_tableView_2_clicked(const QModelIndex &index);

signals:

    void toMainForm();

    void Refresh_tab();

    void Clear_All();

    void Check_for_accept();

private:
    Ui::Add_form *ui;
};

#endif // ADD_FORM_H
