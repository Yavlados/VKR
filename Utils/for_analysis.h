#ifndef FOR_ANALYSIS_H
#define FOR_ANALYSIS_H

#include "db_connection.h"
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>
#include <QDate>
#include "analysisModels.h"
#include <QVector>
#include <QMap>

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
    QSqlQueryModel *model = new QSqlQueryModel();
    void ClearAll();



    ///NEW
    QList<Analysis_V1*> *v1Result;
    QList<Analysis_V2*> *v2Result;
    QList<Analysis_V3*> *v3Result;
    QMap< QString, QList<Analysis_general> > analysisResult;
    QMap< QString, QMap<QString, QList<Analysis_general> > > analysisResultFull;

    void getDataFromBaseFace(int eventId = 0, QVector<int> *eventIdList = 0, QMap<QString, QString> *period = 0);
    void getDataFromBaseTelephone(int eventId = 0, QVector<int> *eventIdList = 0, QMap<QString, QString> *period = 0);
    void getDataFromBaseFullFace(int eventId = 0, QVector<int> *eventIdList = 0, QMap<QString, QString> *period = 0);
    void getDataFromBaseFullTelephone(int eventId = 0, QVector<int> *eventIdList = 0, QMap<QString, QString> *period = 0);

    QList<Analysis_V1*> *getV1(int eventId = 0, QVector<int> *eventIdList = 0, QMap<QString, QString> *period = 0);
    QList<Analysis_V2*> *getV2(int eventId = 0, QVector<int> *eventIdList = 0, QMap<QString, QString> *period = 0);
    QList<Analysis_V3 *> *getV3(int eventId = 0, QVector<int> *eventIdList = 0, QMap<QString, QString> *period = 0);


    void short_face_analysis(int eventId);
    void short_tel_analysis(int eventId);
    void long_face_analysis(int eventId);
    void long_tel_analysis(int eventId);

};

#endif // FOR_ANALYSIS_H
