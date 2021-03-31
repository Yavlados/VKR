#ifndef SEARCH_H
#define SEARCH_H

#include <QWidget>
#include <QSqlQueryModel>
#include "an_result_Form.h"
#include "for_search.h"
#include "Util.h"

/**
 * \file search_Form.h
 * \brief Форма поиска
*/

/// REFACTORED

namespace Ui {
class Search;
}

class Search : public QWidget   ///Форма поиска
{
    Q_OBJECT

public:
    explicit Search(QWidget *parent = 0);
    ~Search();

    QSize actual_size;

    void set_tab_orders();

    // NEW
    bool detentionDateState;
    bool detentionTimeState;
    bool detentionIntervalState;
signals:

     void closeThis(QString);

    void Send_search_result(QString search_result);

    void Cancel_search();

    // NEW
    void showSearchedEvents(QList<QString> searchedIds);

private slots:
    void keyPressEvent(QKeyEvent *event);

    void on_pb_search_clicked();

    void on_pb_back_tomain_clicked();

    void on_pushButton_clicked();

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
