#include "table_delegate.h"
#include <QLineEdit>
#include <QCheckBox>

Table_delegate::Table_delegate(QObject *parent): QItemDelegate(parent)
{
  type = Normal;
  code_num = "7";
}

void Table_delegate::input_code_num(QString c_n)
{
    code_num = c_n;
}

QWidget *Table_delegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QLineEdit *line = new QLineEdit(parent);
//    QCheckBox *cb = new QCheckBox(parent);

    QString temp = "+"+code_num+"(999)999-99-99";

//    if(index.column() == 1)
//        return cb;

    switch (type)
    {
    case Normal:
            line->setInputMask(temp);
            return line;
    case Internum:
            return line;
    }
}
