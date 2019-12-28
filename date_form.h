#ifndef DATE_FORM_H
#define DATE_FORM_H

#include <QWidget>
#include <QLineEdit>
enum date_form_type {
    search_date,
    update_date
};

namespace Ui {
class Date_form;
}

class Date_form : public QWidget
{
    Q_OBJECT

public:
    explicit Date_form(QWidget *parent = nullptr);
    ~Date_form();
    void set_mode(date_form_type m);
    void recieve_data(QString date, QString month, QString year);
    void send_data();

    QString get_day();
    QString get_month();
    QString get_year();

    void set_day(QString t);
    void set_month(QString t);
    void set_year(QString t);

    QLineEdit *day();
    QLineEdit *month();
    QLineEdit *year();

    void refresh();

signals:
    void year_edited();


private slots:
    void on_day_textEdited(const QString &arg1);

    void on_month_textEdited(const QString &arg1);

    void  on_year_textEdited(const QString &arg1);

private:
    date_form_type mode;
    Ui::Date_form *ui;
};

#endif // DATE_FORM_H
