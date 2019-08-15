#include "table_cb_delegate.h"
#include <QPushButton>
#include "QHBoxLayout"
#include "QDebug"

Table_cb_delegate::Table_cb_delegate(QObject *parent): QItemDelegate(parent)
{
   qDebug() << "Создание делегата";

}

bool Table_cb_delegate::hasClipping() const
{
    return true;
}

QWidget *Table_cb_delegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QPushButton *pb = new QPushButton("kuku");

    return pb;
}
