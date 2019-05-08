

#ifndef SEARCH_H
#define SEARCH_H

#include <QWidget>
#include <QSqlQueryModel>
#include "an_result.h"

namespace Ui {
class Search;
}

class Search : public QWidget
{
    Q_OBJECT

public:
    explicit Search(QWidget *parent = nullptr);
    ~Search();

    An_result *an = new An_result();

signals:

    void Send_search_result(QString);

    void Send_Model(QSqlQueryModel*);

    void Cancel_search();

private slots:

    void on_pb_search_clicked();

    void on_pb_back_tomain_clicked();

    void on_pushButton_clicked();

private:
    Ui::Search *ui;
};

#endif // SEARCH_H
