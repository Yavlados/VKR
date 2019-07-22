#ifndef TABLE_DELEGATE_H
#define TABLE_DELEGATE_H

#include <QItemDelegate>

enum Number_type {Normal = 0,
                 Internum = 1};

class Table_delegate: public QItemDelegate
{
    Q_OBJECT
public:
    Number_type type;
    Table_delegate(QObject *parent = nullptr);
    void input_code_num (QString c_n);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
private:
    QString code_num;//код города, 7 по умолчанию
};

#endif // TABLE_DELEGATE_H
