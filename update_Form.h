#ifndef UPDATE_H
#define UPDATE_H
#include "_MTM_Contacts.h"
#include "_MTM_OwTel.h"

#include <QWidget>
#include <QDebug>
#include <QList>
#include <QLineEdit>
#include <QMessageBox>
#include <QSqlTableModel>

namespace Ui {
class Update;
}

class Update : public QWidget
{
    Q_OBJECT

public:
    int zk_id;

    QString tel_num;

    QMessageBox msgbx;

    MTM_Contacts *contacts_model = new MTM_Contacts;
    MTM_OwTel *ot_model = new MTM_OwTel;

    QList<Contacts*> *contactList = new QList<Contacts*>;
    QList<Owners_tel*> *otList = new QList<Owners_tel*>;

    explicit Update(QWidget *parent = nullptr);

    ~Update();
public slots:

    void Recieve_data(int);

private:
    Ui::Update *ui;

signals:
    void Ready_for_update();
    void Update_old_tel(int);
    void Add_contact_row(int);

private slots:

    void clear_ALL();

    void set_validators();

    void on_pb_Update_clicked();
    void on_pb_Back_to_Main_clicked();
    void on_tableView_clicked(const QModelIndex &index);

    void on_rb_adres_reg_clicked();
    void on_rb_adres_liv_clicked();

    void on_pb_del_line_telephone_clicked();
    void on_pb_del_contact_line_clicked();
    void on_pb_add_contact_line_clicked();
    void on_tableView_2_clicked(const QModelIndex &index);
};

#endif // UPDATE_H
