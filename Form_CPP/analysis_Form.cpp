#include "analysis_Form.h"
#include "ui_analysis.h"
#include "for_analysis.h"
#include <QDebug>
#include "an_result_Form.h"
#include <QValidator>
#include  "_Crud.h"
#include "search.h"
#include "popup.h"

Analysis::Analysis(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Analysis)
{
    ui->setupUi(this);
    ui->pb_add_le->setVisible(false);
    ui->le_some_new_zk->setVisible(false);
    //Analysis::set_validators();
    ui->le_an_zk->setValidator( new QIntValidator(0, 2147483647 , this) );
    actual_size = this->size();
    ui->pushButton_2->setVisible(false);
    hlForZkHandler();

    //set date forms
    d_from = new Date_form();
    d_to = new Date_form();
    ui->hl_FROM->addWidget(d_from);
    ui->hl_TO->addWidget(d_to);
    Analysis::clear_rb_3(false);
//    set_tab_orders();

}

Analysis::~Analysis()
{
    delete ui;
}

void Analysis::focus_on_widget()
{
    ui->le_an_zk->setFocus();
}

void Analysis::closeEvent(QCloseEvent *event)
{
    (void)event;
    delete this;
}

void Analysis::on_rb_all_base_clicked()
{
    Analysis::clear_rb_3(false);
    Analysis::clear_rb_2();
}

void Analysis::on_rb_some_zk_clicked()
{
    Analysis::clear_rb_3(false);
    ui->pb_add_le->setVisible(true);
    ui->le_some_new_zk->setVisible(true);
}

void Analysis::on_rb_date_clicked()
{
     Analysis::clear_rb_2();
     Analysis::clear_rb_3(true);
}

void Analysis::on_pb_add_le_clicked()
{
    QLineEdit* line = new QLineEdit(this);
    ui->vl_for_new_le->addWidget(line);
    line->setObjectName("le_some_new_zk");
    line->show();
    linelist_zk_id.append(line);
    if (linelist_zk_id.size()>0 && p_b_counter == 0)
    {
        p_b_counter++;

        QPushButton *p_b = new QPushButton;
         p_b->setText("Удалить");
         ui->vl_for_buttons->addWidget(p_b);
         connect(p_b, SIGNAL(clicked()), this ,SLOT(del_le()));
    }
}

void Analysis::del_le()
{
    QLayout *layout =  ui->vl_for_new_le;
      if(layout->count()>0)
      {
          QLayoutItem *item = layout->takeAt(layout->count()-1);
          delete item->widget();
          linelist_zk_id.removeAt(linelist_zk_id.size()-1);
      }
      if (linelist_zk_id.size() == 0)
      {
          QLayout *layout_b = ui->vl_for_buttons->layout();
          while (layout_b->count() != 0)
              {
                  QLayoutItem *item = layout_b->takeAt(0);
                  delete item->widget();
                  p_b_counter--;
              }
      }
}

void Analysis::clear_rb_2()
{
    ui->pb_add_le->setVisible(false);
    ui->le_some_new_zk->clear();
    ui->le_some_new_zk->setVisible(false);

    QLayout *layout_b = ui->vl_for_buttons->layout();
    QLayout *layout =  ui->vl_for_new_le;
    QString str1 = QString::number(layout_b->count());
    QString str2 = QString::number(layout->count());

    while (layout_b->count() > 0)
        {
            QLayoutItem *item = layout_b->takeAt(0);
            delete item->widget();
            if (p_b_counter >0)
                p_b_counter--;
        }
    while (layout->count()>0)
    {
        QLayoutItem *item = layout->takeAt(layout->count()-1);
        delete item->widget();
        linelist_zk_id.removeAt(linelist_zk_id.size()-1);
    }
}

void Analysis::clear_rb_3(bool stat)
{
    ui->frame->setVisible(stat);
    d_from->refresh();
    d_to->refresh();
}
//КНОПКА АНАЛИЗА
void Analysis::on_pushButton_clicked()
{
    QList<int> local_list_id;
    local_list_id.append(ui->le_an_zk->text().toInt());
    //Обработчик результатов
    An_result *ar = new An_result();

    if(linelist_zk_for_analysis!= nullptr && linelist_zk_for_analysis->size() > 0)
    {
        for(int le =0; le < linelist_zk_for_analysis->size(); le++)
        {
            if(linelist_zk_for_analysis->at(le)->text().size() > 0)
                local_list_id.append(linelist_zk_for_analysis->at(le)->text().toInt());
        }
    }

    ///Проверка на валидность номера

    QSqlQuery querry(db_connection::instance()->db());
    QString query = "SELECT zk.zk_id"
                    " FROM  zk"
                    " WHERE ";
                //Сбор строки поиска
            for(int le =0; le < local_list_id.size(); le++)
            {
                if(le ==0)
                    query += " zk.zk_id="+QString::number(local_list_id.at(le));
                else
                    query +=" OR zk.zk_id="+ QString::number(local_list_id.at(le));
            }


    if (querry.exec(query) )
    {
        //Обработка ошибки
        if(querry.size() != local_list_id.size()) //НЕ ВСЕ ЗК НАЙДЕНЫ В БАЗЕ
        {
            QString reportResult = "ЗК ";

            while(querry.next())
            {//Убираю из списка те номера ЗК, которые нашел, оставляю ненайденные
                local_list_id.removeAt(local_list_id.indexOf(querry.value(0).toInt()));
            }
            foreach(int lostNum, local_list_id)
            {//Собираю репорт по ненайденным
                reportResult+="#"+QString::number(lostNum)+" ";
            }
            reportResult+= " нет в базе!";
            QMessageBox::critical(this,QObject::tr("Ошибка"),reportResult);
            return;
        }

        while(querry.next())
        {
           int zk_id_num = querry.value(0).toInt();
        //////////////////     1     //////////////////
        if(ui->rb_all_base->isChecked() && ui->rb_short->isChecked() && ui->rb_to_face->isChecked())
        {
            ar->Recieve_short_face_analysis_all_db(zk_id_num);
        }

        //////////////////     2     //////////////////
        else if(ui->rb_all_base->isChecked() && ui->rb_short->isChecked() && ui->rb_to_num->isChecked())
        {
            ar->Recieve_short_tel_analysis_all_db(zk_id_num);
        }

        //////////////////     3     //////////////////
        else if(ui->rb_all_base->isChecked() && ui->rb_long->isChecked() && ui->rb_to_face->isChecked())
        {
            ar->Recieve_long_face_analysis_all_db(zk_id_num);
        }

        //////////////////     4     //////////////////
        else if(ui->rb_all_base->isChecked() && ui->rb_long->isChecked() && ui->rb_to_num->isChecked())
       {
            ar->Recieve_long_tel_analysis_all_db(zk_id_num);
       }

        //////////////////     5     //////////////////
        else if(ui->rb_some_zk->isChecked() && ui->rb_short->isChecked() && ui->rb_to_face->isChecked())
       {

            foreach(QLineEdit *l, this->findChildren<QLineEdit*>("le_some_new_zk"))
            {
                if(!l->text().isEmpty())
                    vector.append(l->text().toInt());
            }
            Analysis::uniq_array();

            ar->Recieve_short_face_analysis_all_db(vector,zk_id_num);
            vector.clear();
       }

        //////////////////     6     //////////////////
        else if(ui->rb_some_zk->isChecked() && ui->rb_short->isChecked() && ui->rb_to_num->isChecked())
      {
       foreach(QLineEdit *l, this->findChildren<QLineEdit*>("le_some_new_zk"))
       {
           if(!l->text().isEmpty())
              vector.append(l->text().toInt());
       }

       Analysis::uniq_array();

       ar->Recieve_short_tel_analysis_all_db(vector,zk_id_num);
       vector.clear();
      }

        //////////////////     7     //////////////////
        else if(ui->rb_some_zk->isChecked() && ui->rb_long->isChecked() && ui->rb_to_face->isChecked())
     {
      foreach(QLineEdit *l, this->findChildren<QLineEdit*>("le_some_new_zk"))
      {
          if(!l->text().isEmpty())
             vector.append(l->text().toInt());
      }

      Analysis::uniq_array();

      ar->Recieve_long_face_analysis_all_db(vector,zk_id_num);
      vector.clear();
     }

        //////////////////     8     //////////////////
        else if(ui->rb_some_zk->isChecked() && ui->rb_long->isChecked() && ui->rb_to_num->isChecked())
     {
      foreach(QLineEdit *l, this->findChildren<QLineEdit*>("le_some_new_zk"))
      {
          if(!l->text().isEmpty())
             vector.append(l->text().toInt());
      }

      Analysis::uniq_array();

      ar->Recieve_long_tel_analysis_all_db(vector,zk_id_num);
      vector.clear();

     }

        //////////////////     9     //////////////////
        else if(ui->rb_date->isChecked() && ui->rb_short->isChecked() && ui->rb_to_face->isChecked())
     {
        if(get_date_from() < get_date_to())
        {
            ar->Recieve_short_face_analysis_all_db(Date_From, Date_To, zk_id_num);
        }
        else {
            QMessageBox::critical(this,QObject::tr("Ошибка"), "Дата начала периода позже его окончания!");
        }
     }

        //////////////////     10     //////////////////
        else if(ui->rb_date->isChecked() && ui->rb_short->isChecked() && ui->rb_to_num->isChecked())
    {
        if(!get_date_from().isEmpty() || !get_date_to().isEmpty())
        {
           ar->Recieve_short_tel_analysis_all_db(Date_From, Date_To, zk_id_num);
       }
       else {
            QMessageBox::critical(this,QObject::tr("Ошибка"), "Дата начала периода позже его окончания!");
       }
    }

        //////////////////     11     //////////////////
        else if(ui->rb_date->isChecked() && ui->rb_long->isChecked() && ui->rb_to_face->isChecked())
    {
      if(get_date_from() < get_date_to())
        {
           ar->Recieve_long_face_analysis_all_db(Date_From, Date_To, zk_id_num);
        }
        else {
            QMessageBox::critical(this,QObject::tr("Ошибка"), "Дата начала периода позже его окончания!");
        }
    }

        //////////////////     12      //////////////////
    else if(ui->rb_date->isChecked() && ui->rb_long->isChecked() && ui->rb_to_num->isChecked())
    {
        if(get_date_from() < get_date_to())
          {
               ar->Recieve_long_tel_analysis_all_db(Date_From, Date_To, zk_id_num);
          }
          else {
            QMessageBox::critical(this,QObject::tr("Ошибка"), "Дата начала периода позже его окончания!");
          }
        }
     }
        ar->Set_final_result();
        ar->show();
    }
}

void Analysis::uniq_array()
{
   std::sort(vector.begin(), vector.end());
   vector.erase(std::unique(vector.begin(), vector.end()), vector.end());
}

void Analysis::on_pushButton_2_clicked()
{
    delete  this;
}

QString Analysis::get_date_from()
{    //
    //Сначала собираю DateFrom
    //

    ///Если указан день, месяц, год
    if (!d_from->get_day().isEmpty() && !d_from->get_month().isEmpty() && !d_from->get_year().isEmpty())
    {
        Date_From = d_from->get_year() +"-"+d_from->get_month() +"-"+d_from->get_day();
     return Date_From;
    }

    // Если не указан день, но есть месяц и год
    if (d_from->get_day().isEmpty() && !d_from->get_month().isEmpty() && !d_from->get_year().isEmpty())
    {///За день тогда берем первое число
        Date_From = d_from->get_year() +"-"+d_from->get_month() +"-01";
         return Date_From;
    }

    // Если только год
    if (d_from->get_day().isEmpty() && d_from->get_month().isEmpty() && !d_from->get_year().isEmpty())
    {/// Беру начало года (1 января)
        Date_From = d_from->get_year() +"-01-01";
         return Date_From;
    }
    return "";
}

QString Analysis::get_date_to()
{
        //
     //Теперь собираю DateTo
     //
    //Если указан день, месяц, год
    if (!d_to->get_day().isEmpty() && !d_to->get_month().isEmpty() && !d_to->get_year().isEmpty())
    {
        Date_To = d_to->get_year() +"-"+ d_to->get_month() +"-"+d_to->get_day();
     return Date_To;
    }

    // Если не указан день, но есть месяц и год
    if (d_to->get_day().isEmpty() && !d_to->get_month().isEmpty() && !d_to->get_year().isEmpty())
    {
        QDate date_to_local;
        int day; // локальная переменная для определения кол-ва дней в месяце
        date_to_local.setDate(d_to->get_year().toInt(),d_to->get_month().toInt(),1);
        day = date_to_local.daysInMonth();
        Date_To = d_to->get_year() +"-"+d_to->get_month() +"-"+QString::number(day);
     return Date_To;
    }
    // Если только год
    if (d_to->get_day().isEmpty() && d_to->get_month().isEmpty() && !d_to->get_year().isEmpty())
    {// Беру конец года (31 декабря)
    Date_To=d_to->get_year()+"-12-31";
     return Date_To;
    }
   return "";
}

void Analysis::hlForZkHandler()
{
    if(ui->hl_for_new_zk->count() > 0)
        ui->pb_del_zk->setVisible(true);
    else
        ui->pb_del_zk->setVisible(false);
}

void Analysis::on_pb_add_zk_clicked()
{
    QLineEdit* line = new QLineEdit(this);
    line->setFont(*DefaultLEFont);
//    line->setInputMask("99999999999999999999999999999999999999999");
    line->setValidator( new QIntValidator(0, 2147483647 , this) );
    ui->hl_for_new_zk->addWidget(line);
    line->setObjectName("new_zk_for_analysis");
    line->show();

    if(linelist_zk_for_analysis == nullptr)
        linelist_zk_for_analysis = new QList<QLineEdit*>;

    linelist_zk_for_analysis->append(line);

    hlForZkHandler();
    change_tab_order();
}

void Analysis::on_pb_del_zk_clicked()
{
    if(linelist_zk_for_analysis != nullptr && linelist_zk_for_analysis->size() > 0)
    {
        QLayout *layout = ui->hl_for_new_zk;
        QLayoutItem *item = layout->takeAt(layout->count()-1);
        delete item->widget();
        linelist_zk_for_analysis->removeAt(linelist_zk_for_analysis->size()-1);
        if(linelist_zk_for_analysis->size() == 0)
        {
            delete linelist_zk_for_analysis;
            linelist_zk_for_analysis = nullptr;
        }
    }
    hlForZkHandler();
}

void Analysis::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
        case Qt::Key::Key_Enter:
            on_pushButton_clicked();
            return ;
         case Qt::Key::Key_Escape:
            emit closeThis(this->objectName());
        return;
    case Qt::Key::Key_F1:
       PopUp::instance()->setPopupText("<h2 align=\"middle\">Навигация в окне анализа</h2>"
                                       "<p><b>\"CTRL\"+ \"+\"</b> для добавления строк поиска</p>"
                                       "<p><b>\"CTRL\"+ \"-\"</b> для удаления строк поиска"
                                       "<p><b>\"ENTER\"</b> для запуска анализа</p>"
                                       "<p><b>\"ESC\"</b> для закрытия окна анализа</p>", rightMenu);
       return;
    case Qt::Key::Key_Plus:
        on_pb_add_zk_clicked();
        return;
    case Qt::Key::Key_Minus:
        on_pb_del_zk_clicked();
        return;
    }
}

void Analysis::set_tab_orders()
{
    ui->le_an_zk->setFocus();
    ui->le_hided->setFocusProxy(ui->le_an_zk);

      setTabOrder(ui->le_an_zk, ui->pb_add_zk);
       setTabOrder( ui->pb_add_zk, ui->rb_all_base);
       setTabOrder( ui->rb_all_base, ui->rb_short);
       setTabOrder(ui->rb_short, ui->rb_to_face);
       setTabOrder( ui->rb_to_face, ui->pushButton);
       setTabOrder( ui->pushButton, ui->le_hided);

}

void Analysis::change_tab_order()
{
    for( int a = 0; a < linelist_zk_for_analysis->size();   a++)
    {
        if(a == linelist_zk_for_analysis->size())
            continue;

     if(a == 0)
        setTabOrder( ui->pb_add_zk, linelist_zk_for_analysis->at(a));
     else
        setTabOrder(linelist_zk_for_analysis->at(a-1), linelist_zk_for_analysis->at(a));

     if(a+1 == linelist_zk_for_analysis->size())
         setTabOrder(linelist_zk_for_analysis->at(a), ui->pb_del_zk);
    }
}
