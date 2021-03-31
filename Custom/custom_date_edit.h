#ifndef CUSTOM_DATE_EDIT_H
#define CUSTOM_DATE_EDIT_H
#include <QDateEdit>

/// UNUSED

class Custom_Date_Edit : public QDateEdit
{
    Q_OBJECT
        Q_PROPERTY(QDate nullDate READ nullDate WRITE setDate USER true)
public:
    Custom_Date_Edit();
    Custom_Date_Edit(const QDate& date, QWidget* parent);
       Custom_Date_Edit(QWidget* parent);
       ~Custom_Date_Edit();

       QDate nullDate() const;
public slots:
    void clear();
    void setDate(const QDate& date);
};

#endif // CUSTOM_DATE_EDIT_H
