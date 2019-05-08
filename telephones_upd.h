#ifndef TELEPHONES_UPD_H
#define TELEPHONES_UPD_H

#include <QWidget>
#include <QLineEdit>
namespace Ui {
class Telephones_upd;
}

class Telephones_upd : public QWidget
{
    Q_OBJECT

public:
    Telephones_upd(int id): zk_id(id)
    {
    }

    int zk_id;
    int tel_id;
    int contact_id;
    QList<QLineEdit*> linelist_telephone;
    QList<QLineEdit*> linelist_tel_contact;
    QList<QLineEdit*> linelist_tel_info;


    explicit Telephones_upd(QWidget *parent = nullptr);
    ~Telephones_upd();

signals:
    void Ready_to_close();

private slots:
    void on_pb_back_to_page1_clicked();

    void on_pb_add_contact_line_clicked();
    void on_pb_add_contact_clicked();
    void on_pb_add_line_telephone_clicked();
    void on_pb_del_line_telephone_clicked();
    void on_pb_remove_contact_line_clicked();
    void on_pb_add_telephone_clicked();
    void on_pb_del_telephone_clicked();
    void on_pb_del_contact_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void Recieve_id_from_upd(int);
    void Recieve_id_from_add(int);

    void Add_abort();

    void on_tableView_2_clicked(const QModelIndex &index);
private:
    Ui::Telephones_upd *ui;
};

#endif // TELEPHONES_UPD_H
