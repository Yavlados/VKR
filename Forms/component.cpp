#include "component.h"
#include "ui_component.h"
#include <QDebug>
#include <QCoreApplication>
#include <QCloseEvent>
#include <QValidator>

Component::Component(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Component)
{
    ui->setupUi(this);
    ui->le_1->setMaxLength(1);
    ui->le_2->setMaxLength(3);
    ui->le_3->setMaxLength(3);
    ui->le_4->setMaxLength(2);
    ui->le_5->setMaxLength(2);

    ui->le_1->setValidator(new QIntValidator());
        ui->le_2->setValidator(new QIntValidator());
            ui->le_3->setValidator(new QIntValidator());
                ui->le_4->setValidator(new QIntValidator());
                    ui->le_5->setValidator(new QIntValidator());

    content = new component_content;

    setFixedSize(this->size());
    setModal(true);
    setFocus();
}

Component::~Component()
{
    QCloseEvent event;
    closeEvent(&event);
    delete content;
    delete ui;
}

void Component::set_focus()
{
    if(ui->lineEdit->isVisible())
        ui->lineEdit->setFocus();
    else if(ui->le_1->isVisible())
         ui->le_1->setFocus();
}

void Component::set_type(component_type c_t)
{
    type = c_t;
}

void Component::set_index_data(Telephone *ow_t, Contact *cnt)
{
//    content = new component_content;

    if(type == Ow_tel_num || type == new_ow_tel)
    {
        Q_ASSERT(ow_t!=0);

        content->tel_num = ow_t->num;
        content->Internum = ow_t->internum;
        content->Oldnum = ow_t->oldnum;

        ui->label_6->setVisible(false);
        ui->lineEdit_7->setVisible(false);
    }
    else if(type == Contact_num || type == new_cont_tel)
    {
        Q_ASSERT(cnt!=0);

        content->tel_num = cnt->number;
        content->mark = cnt->alias;
        content->Internum = cnt->internum;
        content->Oldnum = cnt->oldnum;

        ui->label_6->setVisible(true);
        ui->lineEdit_7->setVisible(true);
    }

    if(ow_t !=0)
    {        
        if(ow_t->oldnum == false && ow_t->internum == false)
        {
            ui->lineEdit->setVisible(false);
            if(type == Ow_tel_num)
            {
                if(!content->tel_num.isEmpty())
                {
                    ui->le_1->setText(content->tel_num.at(0));
                    ui->le_2->setText(content->tel_num.mid(1,3));
                    ui->le_3->setText(content->tel_num.mid(4,3));
                    ui->le_4->setText(content->tel_num.mid(7,2));
                    ui->le_5->setText(content->tel_num.mid(9,-1));
                }
            }
        }        
        else
        {
            ui->lineEdit->setText(content->tel_num);
            if( ow_t->oldnum)
               // ui->lineEdit->setInputMask("99999999");
                ui->lineEdit->setInputMask("00000000");
            else
                //ui->lineEdit->setInputMask("999999999999999999999999999999");
                ui->lineEdit->setInputMask("000000000000000000000000000000");
        }

        if(ow_t->oldnum == true)
            ui->cb_oldnum->setCheckState(Qt::Checked);
        if(ow_t->internum == true)
            ui->cb_m_n->setCheckState(Qt::Checked);
    }
    else if (cnt != 0)
    {
        ui->lineEdit_7->setText(content->mark);        

        if(cnt->oldnum == false && cnt->internum == false)
        {
            ui->lineEdit->setVisible(false);
            if(type == Contact_num)
            {
                ui->le_1->setText(content->tel_num.at(0));
                ui->le_2->setText(content->tel_num.mid(1,3));
                ui->le_3->setText(content->tel_num.mid(4,3));
                ui->le_4->setText(content->tel_num.mid(7,2));
                ui->le_5->setText(content->tel_num.mid(9,-1));
            }
        }
        else
        {
            ui->lineEdit->setText(content->tel_num);
            if( cnt->oldnum )
             //   ui->lineEdit->setInputMask("99999999");
                ui->lineEdit->setInputMask("00000000");
            else
              //  ui->lineEdit->setInputMask("999999999999999999999999999999");
                ui->lineEdit->setInputMask("000000000000000000000000000000");
        }

        if(cnt->oldnum == true)
            ui->cb_oldnum->setCheckState(Qt::Checked);
        if(cnt->internum == true)
            ui->cb_m_n->setCheckState(Qt::Checked);
    }
    // set_focus();
}

void Component::closeEvent(QCloseEvent *event)
{
   (void)event;
    close();
}

void Component::change_layouts()
{
    if(ui->cb_oldnum->checkState() == Qt::Unchecked && ui->cb_m_n->checkState() == Qt::Unchecked)
    {//Вкл норм строку

        for(int a = 0; a< ui->hl_for_normal_n->count(); a++)
        {
            QWidget *w = ui->hl_for_normal_n->itemAt(a)->widget();
            if( w!=0)
                w->setVisible(true);
        }

        ui->lineEdit->setVisible(false);
        if(type == Contact_num)
        {
            ui->lineEdit_7->setVisible(true);
            //ui->lineEdit_7->setText(content->mark);
        }
        if(!content->tel_num.isEmpty())
        {
            ui->le_1->setText(content->tel_num.at(0));
            ui->le_2->setText(content->tel_num.mid(1,3));
            ui->le_3->setText(content->tel_num.mid(4,3));
            ui->le_4->setText(content->tel_num.mid(7,2));
            ui->le_5->setText(content->tel_num.mid(9,-1));
        }

        content->Oldnum = false;
        content->Internum = false;
    }
    else if(ui->cb_oldnum->checkState() == Qt::Checked && ui->cb_m_n->checkState() == Qt::Unchecked)
    {
        //Вкл сплошную
        //content->tel_num.clear();
        content->Oldnum = true;
        content->Internum = false;

     /*   for(int a = 0; a< ui->hl_for_normal_n->count(); a++)
        {
           QWidget *w = ui->hl_for_normal_n->itemAt(a)->widget();
           QString name = w->objectName();
           if(name.startsWith("le"))
           {
               QLineEdit *le = dynamic_cast<QLineEdit*>(w); //Приведение типа от виджета к классу
               content->tel_num.append(le->text());
           }
            w->setVisible(false);
        } */

        ui->lineEdit->setVisible(true);
      // ui->lineEdit->setInputMask("99999999");
        ui->lineEdit->setInputMask("00000000");
        ui->lineEdit->setText(content->tel_num);
        ui->lineEdit->setFocus();
        ui->lineEdit->setCursorPosition(0);
         ui->lineEdit->selectAll();

        if(type == Contact_num)
        {
            ui->lineEdit_7->setVisible(true);
            //ui->lineEdit_7->setText(content->mark);
        }

        for(int a = 0; a< ui->hl_for_normal_n->count(); a++)
        {
            QWidget *w = ui->hl_for_normal_n->itemAt(a)->widget();
            if( w!=0)
                w->setVisible(false);
        }

    }
    else if (ui->cb_oldnum->checkState() == Qt::Unchecked && ui->cb_m_n->checkState() == Qt::Checked)
    {
        content->Oldnum = false;
        content->Internum = true;

        ui->lineEdit->setVisible(true);
        ui->lineEdit->setText(content->tel_num);
        ui->lineEdit->setFocus();
        ui->lineEdit->setCursorPosition(0);
         ui->lineEdit->selectAll();

      //  ui->lineEdit->setInputMask("999999999999999999999999999999");
        ui->lineEdit->setInputMask("000000000000000000000000000000");
        if(type == Contact_num)
        {
            ui->lineEdit_7->setVisible(true);
           // ui->lineEdit_7->setText(content->mark);
        }

        for(int a = 0; a< ui->hl_for_normal_n->count(); a++)
        {
            QWidget *w = ui->hl_for_normal_n->itemAt(a)->widget();
            if( w!=0)
                w->setVisible(false);
        }

    }
    //set_focus();
}

void Component::setFocus()
{
    ui->cb_oldnum->setFocus();
    set_Tab_Order();
}

void Component::set_Tab_Order()
{
//    setTabOrder( ui->cb_oldnum, ui->cb_m_n );
//        setTabOrder( ui->cb_m_n, ui->le_1);
//        setTabOrder( ui->le_1, ui->le_2);
//        setTabOrder( ui->le_2, ui->le_3);
//        setTabOrder( ui->le_3, ui->le_4);
//        setTabOrder( ui->le_4, ui->le_5);
//        setTabOrder( ui->le_5, ui->lineEdit);
//        setTabOrder( ui->le_5, ui->lineEdit_7);
//        setTabOrder( ui->lineEdit, ui->lineEdit_7);
//        setTabOrder( ui->lineEdit, ui->pb_ok);
//        setTabOrder( ui->pb_ok, ui->pb_cancel);
}

void Component::on_le_1_textEdited(const QString &arg1)
{
    if (arg1.length() == 1)
    {
        ui->le_2->setFocus();
        ui->le_2->setCursorPosition(0);
         ui->le_2->selectAll();
    }
}

void Component::on_le_2_textEdited(const QString &arg1)
{
    if (arg1.length() == 3)
    {
        ui->le_3->setFocus();
        ui->le_3->setCursorPosition(0);
        ui->le_3->selectAll();
    }
}

void Component::on_le_3_textEdited(const QString &arg1)
{
    if (arg1.length() == 3)
    {
        ui->le_4->setFocus();
        ui->le_4->setCursorPosition(0);
        ui->le_4->selectAll();
    }
}

void Component::on_le_4_textEdited(const QString &arg1)
{
    if (arg1.length() == 2)
    {
        ui->le_5->setFocus();
        ui->le_5->setCursorPosition(0);
        ui->le_5->selectAll();
    }
}

void Component::on_cb_m_n_toggled(bool checked)
{
    setTelNum();

    if (checked == true && ui->cb_oldnum->checkState() == Qt::Checked)
    {
        ui->cb_oldnum->setCheckState(Qt::Unchecked);
    }
    change_layouts();
}

void Component::on_pb_cancel_clicked()
{
    QCloseEvent event;
    closeEvent(&event);
}

void Component::on_cb_oldnum_toggled(bool checked)
{
    setTelNum();

    if (checked == true && ui->cb_m_n->checkState() == Qt::Checked)
    {
        ui->cb_m_n->setCheckState(Qt::Unchecked);
    }
    change_layouts();
}

void Component::on_pb_ok_clicked()
{
    content->Oldnum = ui->cb_oldnum->checkState();
    content->Internum = ui->cb_m_n->checkState();

        content->mark = ui->lineEdit_7->text();

    if(!content->Oldnum && !content->Internum)
        content->tel_num= ui->le_1->text() + ui->le_2->text() + ui->le_3->text() + ui->le_4->text() + ui->le_5->text();
    else
        content->tel_num = ui->lineEdit->text();

    accept();
    close();
}

void Component::on_le_1_inputRejected()
{
}

void Component::setTelNum()
{
    if( content->Oldnum || content->Internum )
    {
        content->tel_num = ui->lineEdit->text();
    }
    else
    {
        content->tel_num= ui->le_1->text() + ui->le_2->text() + ui->le_3->text() + ui->le_4->text() + ui->le_5->text();
    }
}

component_content::component_content()
{
    Oldnum = false;
    Internum = false;
    tel_num = "";
    mark = "";
}
