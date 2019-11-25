#include "table_line_delegate.h"
#include <QLineEdit>
#include <QCheckBox>

Table_line_delegate::Table_line_delegate(QObject *parent): QItemDelegate(parent)
{
  code_num = "7";
}

void Table_line_delegate::set_type(Number_type type_loc)
{
    type = type_loc;
}

void Table_line_delegate::input_code_num(QString c_n)
{
    code_num = c_n;
}

QWidget *Table_line_delegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{/*
    if(index.column() == 2)
    {
        if(type == OT)
        {
            QLineEdit *line = new QLineEdit(parent);
            if (ot_model->actotlist.at(index.row())->oldnum  == true)
            {
                QString temp = "9999999"; //7 цифр
                line->setInputMask(temp);
                line->displayText();
                line->setText(content);
                line->setText(content);
                //line->setPlaceholderText("asd");
                return line;
            }
            if (ot_model->actotlist.at(index.row())->internum == false)
            {
                QString temp = "+"+code_num+"(999)999-99-99"; //ФОРМА ВВОДА НОМЕРА
                line->setInputMask(temp);

                return line;
            }else
            {
                QString temp = "999999999999999999999999999999"; //30 цифр
                line->setPlaceholderText(content);
                line->setText(content);
                line->setInputMask(temp);
                return line;
            }
        }
        if(type == Cont)
        {
            QLineEdit *line = new QLineEdit(parent);
            if (cont->actlist.at(index.row())->oldnum  == true)
            {
                QString temp = "99999999"; //8 цифр
                line->setInputMask(temp);
                line->setPlaceholderText(content);

                return line;
            }
            if (cont->actlist.at(index.row())->internum == false)
            {
                QString temp = "+"+code_num+"(999)999-99-99"; //ФОРМА ВВОДА НОМЕРА
                line->setInputMask(temp);
                line->setPlaceholderText(content);

                return line;
            }else
            {
                QString temp = "999999999999999999999999999999"; //30 цифр
                line->setInputMask(temp);
                line->setPlaceholderText(content);

                return line;
            }
        }
    }

    else if ( index.column() == 0 ||  index.column() == 1)
    {
        QLineEdit *line = new QLineEdit(parent);
        line->setReadOnly(true);
        return line;
    }
        QLineEdit *line = new QLineEdit(parent);
        return line;*/
}

void Table_line_delegate::set_MTM_model(MTM_OwTel *om, MTM_Contacts *cnt)
{
    switch (type)
    {
        case OT:
            ot_model = om;
            return;

        case Cont:
            cont = cnt;
            return;
    }
}

void Table_line_delegate::set_content(QString str)
{
    content = str;
}

