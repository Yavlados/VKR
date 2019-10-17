#ifndef FOR_ANALYSIS_H
#define FOR_ANALYSIS_H

#include "db_connection.h"
#include "list_master.h"

#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>
#include <QDate>

/**
 * \file for_analysis.h
 * \brief Класс методов анализа
*/

class For_analysis          /// Класс методов анализа
{
public:
    For_analysis();

    QString temp_str_for_num;

    QString temp_str_for_cont_num;

    QString analysis_res;

    QMessageBox msgbx;

    QVector<int> match_counter;

    List_master *list;

    QList<Crud*> * get_crud(Crud *cr = nullptr, QString added_SQL = nullptr);

    ///Методы сбора листа по 3 вариантам знакомства
        QList<Crud*> * get_1_var(Crud *cr, QList<Crud*> *crudlist, QString added_SQL);
        QList<Crud*> * get_2_var(Crud *cr, QList<Crud*> *crudlist, QString   added_SQL);
        QList<Crud*> * get_3_var(Crud *cr, QList<Crud*> *crudlist, QString added_SQL);

    ///Основные методы анализа
    void short_face_analysis(Crud *cr, QList<Crud*> *crudlist);
    void short_tel_analysis(Crud *cr, QList<Crud*> *crudlist);
    void long_face_analysis(Crud *cr, QList<Crud*> *crudlist);
    void long_tel_analysis(Crud *cr, QList<Crud*> *crudlist);

    ///Методы для корректировки входящих списков
    void short_face_analysis_all_db(int);
    void short_tel_analysis_all_db(int);
    void long_face_analysis_all_db(int);
    void long_tel_analysis_all_db(int);
    void short_face_analysis_all_db(QVector<int>,int);
    void short_tel_analysis_all_db(QVector<int>,int);
    void long_face_analysis_all_db(QVector<int> vector, int id);
    void long_tel_analysis_all_db(QVector<int> vector, int id);
    void short_face_analysis_all_db(QString DateFrom, QString DateTo, int);
    void short_tel_analysis_all_db(QString DateFrom, QString DateTo, int);
    void long_face_analysis_all_db(QString DateFrom, QString DateTo, int);
    void long_tel_analysis_all_db(QString DateFrom, QString DateTo, int);

    QSqlQueryModel *model = new QSqlQueryModel();
};

#endif // FOR_ANALYSIS_H
