#include "custom_date_edit.h"

Custom_Date_Edit::Custom_Date_Edit()
{

}

Custom_Date_Edit::Custom_Date_Edit(const QDate &date, QWidget *parent)
: QDateEdit(date, parent)
{
    this->setSpecialValueText("Null");
}

Custom_Date_Edit::Custom_Date_Edit(QWidget *parent)
    : QDateEdit(parent)
{
        this->setSpecialValueText("Null");
}

Custom_Date_Edit::~Custom_Date_Edit()
{

}

QDate Custom_Date_Edit::nullDate() const
{
    if (date() == this->minimumDate())
            return QDate();
        return date();
}

void Custom_Date_Edit::clear()
{
    this->setDate(this->minimumDate());
}

void Custom_Date_Edit::setDate(const QDate &date)
{
    this->setSpecialValueText("Null");
//    if (date.isNull())
//            QDateEdit::setDate(this->minimumDate());
//        QDateEdit::setDate(date);
}
