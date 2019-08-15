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
class Crud
{

public:
    Crud();
    Crud(int id);

    QList<Owners_tel*> *owt(); ///Инициализация пустого списка

    CheckState checkState_;
    DbState state;
    QString search_res;
    bool search_is_ready;
    QMessageBox msgbx;
    QString search_result;

    // ТАБЛИЦА 1
    int zk_id;
    QString lastname;
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
    void zk_search();
    void zk_search_model(QString qry);
    void zk_search_report(QString qry);
    bool update_zk();
    bool add_zk();
    static void del_zk(int);
    static Crud* id_zk_search(int zk_id);
    int get_id_from_tel(QString t_n);
    /////////////////////////////////////////////////////////////
    static bool save_all_crud(Crud *cr);
    ///Оператор копирования указателя
    /// используется один раз при импорте
    Crud *operator+ (Crud *old_crud);
    bool compare_with_base(QString query_tel_num = nullptr, QString query_fio = nullptr, int id = 0);
private:
    QList<Owners_tel*> *_owt; ///У ЗК есть список телефонов

signals:
    //void Send_search(QString);
};

#endif // CRUD_H

