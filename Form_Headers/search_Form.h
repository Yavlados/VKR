#ifndef SEARCH_H
#define SEARCH_H

#include <QWidget>
#include <QSqlQueryModel>
#include "an_result_Form.h"

namespace Ui {
class Search;
}

class Search : public QWidget
{
    Q_OBJECT

public:
    explicit Search(QWidget *parent = nullptr);
    ~Search();

    QString Date_From, Date_To, Birth_date, Add_date;
signals:

    void Show_search_result(QList<Crud*> *crudlist);

    void Send_search_result(QString search_result);

    void Cancel_search();

private slots:

    void Create_search_report(QList<Crud*> *crudlist);

    void on_pb_search_clicked();

    void on_pb_back_tomain_clicked();

    void on_pushButton_clicked();

    QString create_search_query(Crud *search_crud);
    //Работа с датами
    QString get_date_from();

    QString get_date_to();

    QString get_date(QString year, QString month, QString day);

    QString get_date_query(QString date, int flag);

private:
    Ui::Search *ui;
};

#endif // SEARCH_H
