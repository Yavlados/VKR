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
class For_analysis
{
public:
    For_analysis();
    QSqlQuery temp;
    QSqlQuery querry;
    QSqlQuery temp_2; // для метода даления
    QSqlQuery temp_3;

    QString analysis_res;

    QString Name;
    QString Lastname;
    QString Midname;
    QString Tel_num;

    QMessageBox msgbx;

    QVector<int> match_counter;

    List_master *list;

    QList<Crud*> * get_crud(Crud *cr);

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
    void short_face_analysis_all_db(QDate, QDate, int);
    void short_tel_analysis_all_db(QDate, QDate, int);
    void long_face_analysis_all_db(QDate, QDate, int);
    void long_tel_analysis_all_db(QDate, QDate, int);

    QSqlQueryModel *model = new QSqlQueryModel();
};

#endif // FOR_ANALYSIS_H
