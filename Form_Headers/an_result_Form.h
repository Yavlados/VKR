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

class An_result : public QWidget ///Форма вывода справок
{
    Q_OBJECT

public:
    explicit An_result(QWidget *parent = 0);
    ~An_result();

        //Search *sr = new Search();
    static void import_report(QString text);
    For_analysis *an;

    void Set_final_result();

public slots:
    void on_pb_back_clicked();

    void closeEvent(QCloseEvent *event);

    //User slots
    void Recieve_short_face_analysis(int);

    void Recieve_short_tel_analysis(int);

    void Recieve_long_face_analysis(int);

    void Recieve_long_tel_analysis(int);

    void Recieve_short_face_analysis(QVector<int>*, int);

    void Recieve_short_tel_analysis(QVector<int>*, int);

    void Recieve_long_face_analysis(QVector<int>*, int);

    void Recieve_long_tel_analysis(QVector<int>*, int);

    void Recieve_short_face_analysis(QMap<QString, QString> *period, int);

    void Recieve_short_tel_analysis (QMap<QString, QString> *period, int);

    void Recieve_long_face_analysis (QMap<QString, QString> *period, int);

    void Recieve_long_tel_analysis  (QMap<QString, QString> *period, int);

    void Recieve_search_result(QString);

    void on_pb_to_txt_clicked();


private:
    Ui::An_result *ui;
    QString FinalResult;
};

#endif // AN_RESULT_H
