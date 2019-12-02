#ifndef CRUD_H
#define CRUD_H
#include "_Owners_tel.h"///->_Contacts.h->db_connrction

#include <QString>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSqlQueryModel>
#include <QDate>
#include <QTime>
#include <QMessageBox>

/**
 * \file _Crud.h
 * \brief Класс сущности "Записная книга"
*/
enum CheckState { Checked =1,
                 Unchecked = 0};

enum CompareResult {
                      lastname_CR= 0,
                      name_CR,
                      mid_name_CR,
                      birth_date_CR,
                      check_for_CR,
                      dop_info_CR,
                      reg_city_CR,
                      reg_street_CR,
                      reg_home_CR,
                      reg_corp_CR,
                      reg_flat_CR,
                      liv_city_CR,
                      liv_street_CR,
                      liv_home_CR,
                      liv_corp_CR,
                      liv_flat_CR};

struct Cl_in_db_struct      ///Структура для результатов сравнения
    {
        int     id;
        QString Tel_num;
    };

class Crud          ///Класс сущности "Записная книга"
{

public:
    Crud();
    Crud(int id);
    ~Crud();

    QList<Owners_tel*> *owt();  ///Инициализация пустого списка телефонов
    QString row_id = nullptr;

    CheckState checkState_;
    DbState state;
    QString search_res;
    bool search_is_ready;
    QMessageBox msgbx;
    QString search_result = nullptr;

    // ТАБЛИЦА 1
    int zk_id;
    QString lastname ;
    QString name;
    QString mid_name;
    QString birth_date;
    QString check_for;
    QString dop_info;

    //Адрес регистрации
    QString reg_city;
    QString reg_street;
    QString reg_home;
    QString reg_corp;
    QString reg_flat;

    //Адрес проживания
    QString liv_city;
    QString liv_street;
    QString liv_home;
    QString liv_corp;
    QString liv_flat;

    //Дата и время сейчас
    QString date_add = nullptr;
    QString time_add = nullptr;
    //Дата и время для редактирования
    QString date_upd = nullptr;
 /////////////////////////////////////////////////////////////
    static bool selectAll(QList<Crud*> *list);
 /////////////////////////////////////////////////////////////

    void check() const;
    ///Методы поиска
    void zk_search_report(QString qry);
    bool update_zk(QList<int> *list_id = nullptr);
    bool add_zk();
    static void del_zk(int);
    static Crud* id_zk_search(int zk_id);
    int get_id_from_tel(QString t_n);
    /////////////////////////////////////////////////////////////
    static bool save_all_crud(Crud *cr);

    ///Оператор копирования указателя
    /// используется при импорте и сравнении для подсвечивания линкованных ЗК
    Crud *operator+ (Crud *old_crud);

    //Метод сравнивания (для подсвечивания линкованных ЗК)
    QList<CompareResult> *compare_cruds(Crud *cmp_cr);

    //Метод изменения БД линкованных записей
    static void Change_linked_in_db(bool state,int id, QString linked = nullptr);

    bool compare_with_base(QString query_tel_num = nullptr, QString query_fio = nullptr, int id = 0 , QList<int> *id_list = nullptr);

    static QList<int> string_parsing(QString linked_nums_string);

    ///Метод разрыва связи
    bool destroy_link(int id);

    QList<Cl_in_db_struct> *compare_result = nullptr;

    ///Методы rowid
    static QList<int> *take_links(QString row_id, SqlType sqltype, QString filename = nullptr);

private:
    QList<Owners_tel*> *_owt; ///У ЗК есть список телефонов
signals:
    //void Send_search(QString);
};

#endif // CRUD_H

