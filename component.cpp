#include "component.h"
#include "ui_component.h"
#include <QDebug>
#include <QCoreApplication>

Component::Component(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Component)
{
    ui->setupUi(this);
    setFixedSize(this->size());
    setModal(true);
    set_input_mask();
}

Component::~Component()
{
    QCloseEvent event;
    closeEvent(&event);
    delete content;
    delete ui;
}

void Component::set_input_mask()
{
//    QString ipregx("9");
//    ui->le_1->setInputMask(ipregx);

//    ui->le_1->validator();
//    ui->le_1->setInputMask("9");

//    ui->le_2->setInputMask("999");
//    ui->le_3->setInputMask("999");

//    ui->le_4->setInputMask("99");
    //    ui->le_5->setInputMask("99");
}

void Component::set_type(component_type c_t)
{
    type = c_t;
}

void Component::set_index_data(Owners_tel *ow_t, Contacts *cnt)
{
    content = new component_content;

    if(type == Ow_tel_num || type == new_ow_tel)
    {
        content->tel_num = ow_t->tel_num;

        ui->label_6->setVisible(false);
        ui->lineEdit_7->setVisible(false);
    }else
   if(type == Contact_num || type == new_cont_tel)
    {
        content->tel_num = cnt->contact_tel_num;
        content->mark = cnt->mark;

        ui->label_6->setVisible(true);
        ui->lineEdit_7->setVisible(true);
    }

    if(ow_t !=nullptr)
    {
        if(ow_t->oldnum == true)
            ui->cb_oldnum->setCheckState(Qt::Checked);
        if(ow_t->internum == true)
            ui->cb_m_n->setCheckState(Qt::Checked);
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
    }
    else if (cnt != nullptr)
    {
        ui->lineEdit_7->setText(content->mark);

        if(cnt->oldnum == true)
            ui->cb_oldnum->setCheckState(Qt::Checked);
        if(cnt->internum == true)
            ui->cb_m_n->setCheckState(Qt::Checked);
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
    }
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
            w->setVisible(true);
        }

        ui->lineEdit->setVisible(false);
        if(type == Contact_num)
        {
            ui->lineEdit_7->setVisible(true);
            //ui->lineEdit_7->setText(content->mark);
        }
        ui->le_1->setText(content->tel_num.at(0));
        ui->le_2->setText(content->tel_num.mid(1,3));
        ui->le_3->setText(content->tel_num.mid(4,3));
        ui->le_4->setText(content->tel_num.mid(7,2));
        ui->le_5->setText(content->tel_num.mid(9,-1));
    }
    else if(ui->cb_oldnum->checkState() == Qt::Checked && ui->cb_m_n->checkState() == Qt::Unchecked)
    {//Вкл сплошную
        content->tel_num.clear();

        for(int a = 0; a< ui->hl_for_normal_n->count(); a++)
        {
            QWidget *w = ui->hl_for_normal_n->itemAt(a)->widget();
           QString name = w->objectName();
           if(name.startsWith("le"))
           {
               QLineEdit *le = dynamic_cast<QLineEdit*>(w); //Приведение типа от виджета к классу
               content->tel_num.append(le->text());
           }
            w->setVisible(false);
        }

        ui->lineEdit->setVisible(true);
        ui->lineEdit->setText(content->tel_num);
        ui->lineEdit->setInputMask("99999999");

        if(type == Contact_num)
        {
            ui->lineEdit_7->setVisible(true);
            //ui->lineEdit_7->setText(content->mark);
        }

    }
    else if (ui->cb_oldnum->checkState() == Qt::Unchecked && ui->cb_m_n->checkState() == Qt::Checked)
    {
        ui->lineEdit->setVisible(true);
        ui->lineEdit->setText(content->tel_num);
        ui->lineEdit->setInputMask("999999999999999999999999999999");
        if(type == Contact_num)
        {
            ui->lineEdit_7->setVisible(true);
           // ui->lineEdit_7->setText(content->mark);
        }

        for(int a = 0; a< ui->hl_for_normal_n->count(); a++)
        {
            QWidget *w = ui->hl_for_normal_n->itemAt(a)->widget();
            w->setVisible(false);
        }

    }
}

void Component::on_le_1_textEdited(const QString &arg1)
{
    if (arg1.length() == 1)
        ui->le_2->setFocus();
}

void Component::on_le_2_textEdited(const QString &arg1)
{
    if (arg1.length() == 3)
        ui->le_3->setFocus();
}

void Component::on_le_3_textEdited(const QString &arg1)
{
    if (arg1.length() == 3)
        ui->le_4->setFocus();
}

void Component::on_le_4_textEdited(const QString &arg1)
{
    if (arg1.length() == 2)
        ui->le_5->setFocus();
}

void Component::on_cb_m_n_toggled(bool checked)
{
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
    qDebug() << "asd";
}
