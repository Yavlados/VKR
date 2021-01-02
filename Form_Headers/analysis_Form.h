#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <QWidget>
#include <QLineEdit>
#include <QVector>
#include <QDate>
#include <QFont>
#include "date_form.h"
#include <QKeyEvent>

/**
 * @file analysis_Form.h
 * @brief Форма анализа
*/

namespace Ui {
class Analysis;
}

class Analysis : public QWidget         ///Форма анализа
{
    Q_OBJECT

public:
    explicit Analysis(QWidget *parent = 0);
    ~Analysis();

   QList<QLineEdit*> linelist_zk_id;
   QList<QLineEdit*> *linelist_zk_for_analysis;

    int p_b_counter;

    QVector<int> vector;
    QString Date_From, Date_To;

    QSize actual_size;

    void focus_on_widget();
    void set_tab_orders();
    void change_tab_order();

private slots:

    void closeEvent(QCloseEvent *event);

    void on_rb_all_base_clicked();

    void on_rb_some_zk_clicked();

    void on_rb_date_clicked();

    void on_pb_add_le_clicked();

   //user slots
    void del_le();

    void clear_rb_2();

    void on_pushButton_clicked();

    void clear_rb_3(bool);

    void uniq_array();

    void on_pushButton_2_clicked();

    QString get_date_from();

    QString get_date_to();

    void hlForZkHandler();

    void on_pb_add_zk_clicked();

    void on_pb_del_zk_clicked();

    void keyPressEvent(QKeyEvent *event);

signals:

    void closeThis(QString);

    void Send_short_face_analysis_all_db(int);

    void Send_short_tel_analysis_all_db(int);

    void Send_long_face_analysis_all_db(int);

    void Send_long_tel_analysis_all_db(int);

    void Send_short_face_analysis_all_db(QVector<int>, int);

    void Send_short_tel_analysis_all_db(QVector<int>, int);

    void Send_long_face_analysis_all_db(QVector<int>, int);

    void Send_long_tel_analysis_all_db(QVector<int>, int);

    void Send_short_face_analysis_all_db(QString DateFrom, QString DateTo, int);

    void Send_short_tel_analysis_all_db(QString DateFrom, QString DateTo, int);

    void Send_long_face_analysis_all_db(QString DateFrom, QString DateTo, int);

    void Send_long_tel_analysis_all_db(QString DateFrom, QString DateTo, int);

private:
    Ui::Analysis *ui;
     QFont *DefaultLEFont;
     Date_form *d_from;
     Date_form *d_to;
};



#endif // ANALYSIS_H
