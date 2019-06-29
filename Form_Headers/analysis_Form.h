#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <QWidget>
#include <QLineEdit>
#include <QVector>
#include <QDate>


/**
 * @file analysis_Form.h
 * @brief Форма анализа
*/

namespace Ui {
class Analysis;
}

class Analysis : public QWidget
{
    Q_OBJECT

public:
    explicit Analysis(QWidget *parent = nullptr);
    ~Analysis();

   QList<QLineEdit*> linelist_zk_id;
    int p_b_counter = 0;

    QVector<int> vector;
    QString Date_From, Date_To;

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

    void set_validators();

    void uniq_array();

    void on_pushButton_2_clicked();

    QString get_date_from();

    QString get_date_to();

signals:

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
};



#endif // ANALYSIS_H
