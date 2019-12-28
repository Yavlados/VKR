#include "date_form.h"
#include "ui_date_form.h"

#include <QDebug>


Date_form::Date_form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Date_form)
{
    ui->setupUi(this);
    focusPolicy();
    setFocusPolicy(Qt::TabFocus);
}

Date_form::~Date_form()
{
    delete ui;
}

void Date_form::set_mode(date_form_type m)
{
    mode = m;
}

void Date_form::recieve_data(QString date, QString month, QString year)
{

}

QString Date_form::get_day()
{
    return ui->day->text();
}

QString Date_form::get_month()
{
    return ui->month->text();
}

QString Date_form::get_year()
{
    return ui->year->text();
}

void Date_form::set_day(QString t)
{
    ui->day->setText(t);
}

void Date_form::set_month(QString t)
{
    ui->month->setText(t);
}

void Date_form::set_year(QString t)
{
    ui->year->setText(t);
}

QLineEdit *Date_form::day()
{
    return ui->day;
}

QLineEdit *Date_form::month()
{
    return ui->month;
}

QLineEdit *Date_form::year()
{
    return ui->year;
}

void Date_form::refresh()
{
    foreach(QLineEdit *l, this->findChildren<QLineEdit*>())
    {
        l->clear();
    }
}

void Date_form::on_day_textEdited(const QString &arg1)
{
    if (arg1.length() == 2)
    {
        ui->month->setFocus();
        ui->month->setCursorPosition(0);
         ui->month->selectAll();
    }
}

void Date_form::on_month_textEdited(const QString &arg1)
{
    if (arg1.length() == 2)
        {
            ui->year->setFocus();
            ui->year->setCursorPosition(0);
             ui->year->selectAll();
        }

}

void Date_form::on_year_textEdited(const QString &arg1)
{
    if(arg1.length() == 4)
    {
        emit year_edited();
    }
}
