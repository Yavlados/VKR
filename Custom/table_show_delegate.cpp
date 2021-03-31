#include "table_show_delegate.h"
#include <QLineEdit>
#include "QHBoxLayout"
#include "QDebug"

Table_show_delegate::Table_show_delegate(QObject *parent): QItemDelegate(parent)
{
   code_num = "7";

}

bool Table_show_delegate::hasClipping() const
{
    return true;
}

QWidget *Table_show_delegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QLineEdit *le = new QLineEdit;
        if(index.column() == 0)
    {
    QString temp = "+"+code_num+"(999)999-99-99"; //ФОРМА ВВОДА НОМЕРА
    le->setInputMask(temp);
    le->displayText();
    //le->setReadOnly(true);
    return le;
    }
    else {
       return le;
    }
}
