#ifndef TABLE_CB_DELEGATE_H
#define TABLE_CB_DELEGATE_H

#include <QCheckBox>
#include <QItemDelegate>

class Table_show_delegate: public QItemDelegate       ///Класс делегата CheckBox
{
    Q_OBJECT
public:
    QString code_num;
    Table_show_delegate(QObject *parent = nullptr);
     bool hasClipping() const;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
private:

};

#endif // TABLE_CB_DELEGATE_H
