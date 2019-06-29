#ifndef AN_RESULT_H
#define AN_RESULT_H
#include <QFileDialog>
#include <QWidget>
#include <QDate>
#include "for_analysis.h"

/**
 * \file an_result_Form.h
 * \brief Форма вывода справок
*/
namespace Ui {
class An_result;
}

class An_result : public QWidget
{
    Q_OBJECT

public:
    explicit An_result(QWidget *parent = nullptr);
    ~An_result();

        //Search *sr = new Search();

private slots:
    void on_pb_back_clicked();

    void closeEvent(QCloseEvent *event);

    //User slots
    void Recieve_short_face_analysis_all_db(int);

    void Recieve_short_tel_analysis_all_db(int);

    void Recieve_long_face_analysis_all_db(int);

    void Recieve_long_tel_analysis_all_db(int);

    void Recieve_short_face_analysis_all_db(QVector<int>, int);

    void Recieve_short_tel_analysis_all_db(QVector<int>, int);

    void Recieve_long_face_analysis_all_db(QVector<int>, int);

    void Recieve_long_tel_analysis_all_db(QVector<int>, int);

    void Recieve_short_face_analysis_all_db(QString DateFrom, QString DateTo, int);

    void Recieve_short_tel_analysis_all_db(QString DateFrom, QString DateTo, int);

    void Recieve_long_face_analysis_all_db(QString DateFrom, QString DateTo, int);

    void Recieve_long_tel_analysis_all_db(QString DateFrom, QString DateTo, int);

    void Recieve_search_result(QString);

    void on_pb_to_txt_clicked();

private:
    Ui::An_result *ui;
};

#endif // AN_RESULT_H
