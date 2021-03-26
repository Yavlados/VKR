#ifndef SEARCH_H
#define SEARCH_H

#include <QWidget>
#include <QSqlQueryModel>
#include "an_result_Form.h"
#include "date_form.h"
#include "for_search.h"
#include "Util.h"

/**
 * \file search_Form.h
 * \brief Форма поиска
*/
namespace Ui {
class Search;
}

class Search : public QWidget   ///Форма поиска
{
    Q_OBJECT

public:
    explicit Search(QWidget *parent = 0);
    ~Search();

    QString Date_From, Date_To, Birth_date, Add_date;

    QSize actual_size;

    void set_tab_orders();

    // NEW
    bool detentionDateState;
    bool detentionTimeState;
    bool detentionIntervalState;
signals:

     void closeThis(QString);

    void Show_search_result(QList<Crud*> *crudlist);

    void Send_search_result(QString search_result);

    void Cancel_search();

    // NEW
    void showSearchedEvents(QList<QString> searchedIds);

private slots:
    void keyPressEvent(QKeyEvent *event);
    QString create_new_date(QString str);

    void Create_search_report(QList<Crud*> *crudlist);

    void on_pb_search_clicked();

    void on_pb_back_tomain_clicked();

    void on_pushButton_clicked();

    QString create_search_query(Crud *search_crud);
    //Работа с датами
    QString get_date_from();

    QString get_date_to();

    static QString get_date(QString year, QString month, QString day);

    QString get_date_query(QString date, int flag);

    void set_date_forms();
    void bd_edited();

    // NEW
    void updateStates();
    void detentionVisibility();
    void serializeFields(For_search *fs);
    void on_checkBox_detention_time_toggled(bool checked);
    void on_checkBox_detention_date_toggled(bool checked);
    void on_checkBox_detention_interval_toggled(bool checked);

private:
    Ui::Search *ui;
     Date_form *bd;
     Date_form *form_d;
     Date_form *d_from;
     Date_form *d_to;
};

#endif // SEARCH_H
