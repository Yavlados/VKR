#ifndef UPDATE_H
#define UPDATE_H
#include "_MTM_Contacts.h"
#include "_MTM_OwTel.h"
#include "_Crud.h"
#include "list_master.h"

#include <QWidget>
#include <QDebug>
#include <QList>
#include <QLineEdit>
#include <QMessageBox>
#include <QSqlTableModel>

namespace Ui {
class Update;
}
enum Form_type { Update_form =1,
                 Add_form = 0};

enum Import_type {Update_import_data =0,
                  Update_pg_data = 2,
                  Update_pg_data_import = 3,
                  Add_import_data = 1};

class Update : public QWidget
{
    Q_OBJECT

public:
    int main_array_index; //Необходим для очистки памяти
    Form_type frm_t;
    Import_type imprt_t;
    QMessageBox msgbx;

    MTM_OwTel *ot_model = new MTM_OwTel;
    MTM_Contacts *contacts_model = new MTM_Contacts;
    List_master *list;
    Crud *new_cr; //Новая зк
    explicit Update(QWidget *parent = nullptr);

    ~Update();
public slots:

    void Recieve_data(Crud*, int index);
    ///метод заполнения полей формы при редактирвании
    /// имеющейся записи в бд или при импорте
    void Fill_fields_update(Crud *new_cr);
    void Fill_table_in_add();
    void close(); //деструктор класса для сигнал
private:
    Ui::Update *ui;

signals:

    void Ready_for_update(int index);
    void Add_contact_row(int);
    void open_update_tab(Crud *);
    void updated_import_crud(Crud*);
    void add_import_crud(Crud*);
    void update_import_pg();

private slots:

    void clear_ALL();
    void clear_Vl();

    void set_validators();

    void on_pb_Update_clicked();
    void on_pb_Back_to_Main_clicked();
    void on_tableView_clicked(const QModelIndex &index);

    void on_pb_del_line_telephone_clicked();
    void on_pb_del_contact_line_clicked();
    void on_pb_add_contact_line_clicked();
    void on_tableView_2_clicked(const QModelIndex &index);
    void set_splitter_lines();
    bool compare_tel_num();

    ///Методы добавления

     void Add_zk();
     void cb_clicked();

    ///Слоты для редактирования при импорте
    void recieve_import_data(Crud *cr);
    void update_import_data();
};

#endif // UPDATE_H
