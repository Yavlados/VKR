#ifndef UPDATE_H
#define UPDATE_H
#include "_MTM_Contacts.h"
#include "_MTM_OwTel.h"
#include "_Crud.h"
#include "list_master.h"
//#include "table_line_delegate.h"
#include "component.h"
#include "date_form.h"

#include <QWidget>
#include <QDebug>
#include <QList>
#include <QLineEdit>
#include <QMessageBox>
#include <QSqlTableModel>
#include <QString>
#include <QShortcut>

/**
 * \file update_Form.h
 * \brief Форма редактирования/добавления записных книг
*/
namespace Ui {
class Update;
}
enum Form_type { Update_form =0,
                 Add_form,
                 Confluence_form};

enum Import_type {Update_import_data =0,
                  Update_pg_data = 2,
                  Update_pg_data_import = 3,
                  Add_import_data = 1};

class Update : public QWidget   ///Форма поиска
{
    Q_OBJECT

public:
//     Table_line_delegate *delegate_ot = nullptr;
//     Table_line_delegate *delegate_cont = nullptr;

    int main_array_index; //Необходим для очистки памяти
    Form_type frm_t;
    Import_type imprt_t;
    QMessageBox msgbx;

    MTM_OwTel *ot_model = nullptr;
    MTM_Contacts *contacts_model =  nullptr;
    List_master *list = nullptr;

    Crud *new_cr; //Новая зк
    Crud *main_cr = nullptr; //Зк, главная в скписке линкованных или при слиянии
    Crud *added_cr = nullptr; //добавочная при слиянии

    QList<Crud*> *linked_crud();
    QList<int> *linked_crud_id = nullptr;

    QList<Crud*> *list_for_destroy = nullptr;

    explicit Update(QWidget *parent = nullptr);
    void set_tab_orders();
    ~Update();
public slots:

    void Recieve_data(Crud*);    
    void take_linked_zk(Crud *cr);
    ///метод заполнения полей формы при редактирвании
    /// имеющейся записи в бд или при импорте
    void Fill_fields_update(Crud *new_cr);
    void Fill_table_in_add();
    void close(); //деструктор класса для сигнал


    ///-----Методы для слияния-----///
    /// метод начала слияния
    void start_confluence(Crud *confl_cr, Crud *m_cr, Crud *a_cr);

    void focus_on_widget();

private:
    QList<Crud*> * _linked_crud = nullptr;
    int index = 0;
    Ui::Update *ui;
    QPushButton *p_b = nullptr;
    Date_form *bd;
signals:

    void Ready_for_update(int);
    void Add_contact_row(int);
    void open_update_tab(Crud *);
    void updated_import_crud(Crud*);
    void add_import_crud(Crud*);
    void update_import_pg();
    void open_confluence_upd(Crud *confl_crud,Crud *main_crud,Crud *second_crud);

private slots:


    void set_focus();
    void keyPressEvent(QKeyEvent *event);
    void closeEvent(QCloseEvent *event);
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

    ///Метод проверки ФИО+ДР и телефонов
    void set_splitter_lines();
    bool compare_tel_num();

    ///Метод сбора строки дня рождения
    QString get_birthdate();

    ///Методы добавления
     void Add_zk();
     void Add_zk_into_base();

     void cb_clicked();

    ///Слоты для редактирования при импорте
    void recieve_import_data(Crud *cr);
    void update_import_data();

    ///Слот поиска совпадающих полей
    void compare_linked_cruds();

    ///Слот подсветки совпавших
    void mark_le(QLineEdit *le, QLineEdit *le1 = nullptr, QLineEdit *le2 = nullptr);

    ///Слот Заолнения вл
    void fill_vl();

    ///пагинация связаных ЗК
    void prev_page();
    void next_page();

    ///Чистка вл для линковки
    void clear_vl_for_links();

    ///месседжбокс перед слиянием
    void msg_before_confluence(Crud *cr);

    ///метод слияния
    void prepare_confluence_crud(Crud*main_crud, Crud*added_crud);

    ///метод разрыва связи
    void destroy_link(Crud *m_cr, Crud *n_cr);

    ///метод добавления круда на разрыв
    void add_for_destroy();

    void on_tableView_doubleClicked(const QModelIndex &index);
    void on_tableView_entered(const QModelIndex &index);
    void on_cb_adres_clicked();

    void upload_main_cr();

    void on_pb_add_line_telephone_clicked();
    void on_tableView_2_doubleClicked(const QModelIndex &index);
};

#endif // UPDATE_H
