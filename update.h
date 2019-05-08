#ifndef UPDATE_H
#define UPDATE_H

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
    int tel_id;
    int zk_id;
    QString adres_reg;
    QString adres_liv;
    QString tel_num;

    QMessageBox msgbx;

    QSqlTableModel* model;  //для телефонов
    QSqlTableModel* model_2; //для контактов
    QList<QString> temp;

    explicit Update(QWidget *parent = nullptr);

    ~Update();

private:
    Ui::Update *ui;

signals:
    void Ready_for_update();

private slots:

    void Recieve_data(int);
    void on_pb_Update_clicked();
    void on_pb_Back_to_Main_clicked();
    void on_tableView_clicked(const QModelIndex &index);

    void on_rb_adres_reg_clicked();
    void on_rb_adres_liv_clicked();

    void on_pb_add_contact_line_clicked();
    void on_pb_add_contact_clicked();
    void on_pb_add_line_telephone_clicked();
    void on_pb_del_line_telephone_clicked();
    void on_pb_remove_contact_line_clicked();
    void on_pb_add_telephone_clicked();

};

#endif // UPDATE_H
