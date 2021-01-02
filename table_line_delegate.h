#ifndef TABLE_DELEGATE_H
#define TABLE_DELEGATE_H

#include <QItemDelegate>
#include "_MTM_OwTel.h"
#include "_MTM_Contacts.h"

enum Number_type {OT = 0,
                 Cont = 1};

class Table_line_delegate: public QItemDelegate     ///Класс делегата LineEdit
{
    Q_OBJECT
public:

    QString content;

    Table_line_delegate(QObject *parent = 0);
    void set_type(Number_type type_loc);
    void input_code_num (QString c_n);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void set_MTM_model(MTM_OwTel*,MTM_Contacts*);

    void set_content(QString str);
private:
    Number_type type;
    QString code_num;//код города, 7 по умолчанию
    MTM_OwTel *ot_model = 0;
    MTM_Contacts *cont = 0;
};

#endif // TABLE_DELEGATE_H
