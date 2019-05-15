#include "analysis.h"
#include "ui_analysis.h"
#include "for_analysis.h"
#include <QDebug>
#include "an_result.h"
#include <QValidator>
#include  "_Crud.h"

Analysis::Analysis(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Analysis)
{
    ui->setupUi(this);
    ui->pb_add_le->setVisible(false);
    ui->le_some_new_zk->setVisible(false);
    //Analysis::set_validators();
    Analysis::clear_rb_3(false);
}

Analysis::~Analysis()
{
    delete ui;
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
        qDebug() << p_b_counter;

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
    ui->lab_for->setVisible(stat);
    ui->le_day_to->setVisible(stat);
     ui->le_day_to->clear();
    ui->le_month_to->setVisible(stat);
     ui->le_month_to->clear();
    ui->le_year_to->setVisible(stat);
     ui->le_year_to->clear();

    ui->lab_from->setVisible(stat);
    ui->le_day_from->setVisible(stat);
     ui->le_day_from->clear();
    ui->le_month_from->setVisible(stat);
     ui->le_month_from->clear();
    ui->le_year_from->setVisible(stat);
    ui->le_year_from->clear();
}

void Analysis::on_pushButton_clicked()
{
    //////////////////     1     //////////////////
    if(ui->rb_all_base->isChecked() && ui->rb_short->isChecked() && ui->rb_to_face->isChecked())
    {
    An_result *ar = new An_result();
    ar->show();

    connect(this, SIGNAL(Send_short_face_analysis_all_db(int)), ar, SLOT(Recieve_short_face_analysis_all_db(int)));

    emit Send_short_face_analysis_all_db(ui->le_an_zk->text().toInt());
    }

    //////////////////     2     //////////////////
    if(ui->rb_all_base->isChecked() && ui->rb_short->isChecked() && ui->rb_to_num->isChecked())
    {
    An_result *ar = new An_result();
    ar->show();

    connect(this, SIGNAL(Send_short_tel_analysis_all_db(int)), ar, SLOT(Recieve_short_tel_analysis_all_db(int)));

    emit Send_short_tel_analysis_all_db(ui->le_an_zk->text().toInt());
    }

    //////////////////     3     //////////////////
    if(ui->rb_all_base->isChecked() && ui->rb_long->isChecked() && ui->rb_to_face->isChecked())
    {
    An_result *ar = new An_result();
    ar->show();

    connect(this, SIGNAL(Send_long_face_analysis_all_db(int)), ar, SLOT(Recieve_long_face_analysis_all_db(int)));

    emit Send_long_face_analysis_all_db(ui->le_an_zk->text().toInt());
    }

    //////////////////     4     //////////////////
   if(ui->rb_all_base->isChecked() && ui->rb_long->isChecked() && ui->rb_to_num->isChecked())
   {
   An_result *ar = new An_result();
   ar->show();

   connect(this, SIGNAL(Send_long_tel_analysis_all_db(int)), ar, SLOT(Recieve_long_tel_analysis_all_db(int)));

   emit Send_long_tel_analysis_all_db(ui->le_an_zk->text().toInt());
   }

    //////////////////     5     //////////////////
   if(ui->rb_some_zk->isChecked() && ui->rb_short->isChecked() && ui->rb_to_face->isChecked())
   {

    foreach(QLineEdit *l, this->findChildren<QLineEdit*>("le_some_new_zk"))
    {
        if(!l->text().isEmpty())
            vector.append(l->text().toInt());
    }

    Analysis::uniq_array();

    An_result *ar = new An_result();
    ar->show();

    connect(this, SIGNAL(Send_short_face_analysis_all_db(QVector<int>, int)), ar, SLOT(Recieve_short_face_analysis_all_db(QVector<int>, int)));

    emit Send_short_face_analysis_all_db(vector,ui->le_an_zk->text().toInt());

    vector.clear();
   }

    //////////////////     6     //////////////////
  if(ui->rb_some_zk->isChecked() && ui->rb_short->isChecked() && ui->rb_to_num->isChecked())
  {
   foreach(QLineEdit *l, this->findChildren<QLineEdit*>("le_some_new_zk"))
   {
       if(!l->text().isEmpty())
          vector.append(l->text().toInt());
   }

   Analysis::uniq_array();

   An_result *ar = new An_result();
   ar->show();

   connect(this, SIGNAL(Send_short_tel_analysis_all_db(QVector<int>, int)), ar, SLOT(Recieve_short_tel_analysis_all_db(QVector<int>, int)));

   emit Send_short_tel_analysis_all_db(vector,ui->le_an_zk->text().toInt());

   vector.clear();
  }

    //////////////////     7     //////////////////
 if(ui->rb_some_zk->isChecked() && ui->rb_long->isChecked() && ui->rb_to_face->isChecked())
 {
  foreach(QLineEdit *l, this->findChildren<QLineEdit*>("le_some_new_zk"))
  {
      if(!l->text().isEmpty())
         vector.append(l->text().toInt());
  }

  Analysis::uniq_array();

  An_result *ar = new An_result();
  ar->show();

  connect(this, SIGNAL(Send_long_face_analysis_all_db(QVector<int>, int)), ar, SLOT(Recieve_long_face_analysis_all_db(QVector<int>, int)));

  emit Send_long_face_analysis_all_db(vector,ui->le_an_zk->text().toInt());

  vector.clear();
 }

    //////////////////     8     //////////////////
 if(ui->rb_some_zk->isChecked() && ui->rb_long->isChecked() && ui->rb_to_num->isChecked())
 {
  foreach(QLineEdit *l, this->findChildren<QLineEdit*>("le_some_new_zk"))
  {
      if(!l->text().isEmpty())
         vector.append(l->text().toInt());
  }

  Analysis::uniq_array();

  An_result *ar = new An_result();
  ar->show();

  connect(this, SIGNAL(Send_long_tel_analysis_all_db(QVector<int>, int)), ar, SLOT(Recieve_long_tel_analysis_all_db(QVector<int>, int)));

  emit Send_long_tel_analysis_all_db(vector,ui->le_an_zk->text().toInt());

  vector.clear();
 }

    //////////////////     9     //////////////////
 if(ui->rb_date->isChecked() && ui->rb_short->isChecked() && ui->rb_to_face->isChecked())
 {
 QDate dateFROM, dateTO;
 dateFROM.setDate(ui->le_year_from->text().toInt(),ui->le_month_from->text().toInt(),ui->le_day_from->text().toInt());
 dateTO.setDate(ui->le_year_to->text().toInt(),ui->le_month_to->text().toInt(),ui->le_day_to->text().toInt());

 qDebug() << "FROM "+dateFROM.toString(Qt::ISODate);
 qDebug() << "TO "+dateTO.toString(Qt::ISODate);
 bool check = dateTO < dateFROM;
    if(check == false)
    {
        An_result *ar = new An_result();
        ar->show();

        connect(this, SIGNAL(Send_short_face_analysis_all_db(QDate, QDate, int)), ar, SLOT(Recieve_short_face_analysis_all_db(QDate, QDate, int)));

        emit Send_short_face_analysis_all_db(dateFROM, dateTO, ui->le_an_zk->text().toInt());

    }
    else {
        qDebug() << "DATE TO < DATE FOR";
    }
 }

    //////////////////     10     //////////////////
if(ui->rb_date->isChecked() && ui->rb_short->isChecked() && ui->rb_to_num->isChecked())
{
QDate dateFROM, dateTO;
dateFROM.setDate(ui->le_year_from->text().toInt(),ui->le_month_from->text().toInt(),ui->le_day_from->text().toInt());
dateTO.setDate(ui->le_year_to->text().toInt(),ui->le_month_to->text().toInt(),ui->le_day_to->text().toInt());

qDebug() << "FROM "+dateFROM.toString(Qt::ISODate);
qDebug() << "TO "+dateTO.toString(Qt::ISODate);
bool check = dateTO < dateFROM;
   if(check == false)
   {
       An_result *ar = new An_result();
       ar->show();

       connect(this, SIGNAL(Send_short_tel_analysis_all_db(QDate, QDate, int)), ar, SLOT(Recieve_short_tel_analysis_all_db(QDate, QDate, int)));

       emit Send_short_tel_analysis_all_db(dateFROM, dateTO, ui->le_an_zk->text().toInt());
   }
   else {
       qDebug() << "DATE TO < DATE FOR";
   }
}

    //////////////////     11     //////////////////
if(ui->rb_date->isChecked() && ui->rb_long->isChecked() && ui->rb_to_face->isChecked())
{
    QDate dateFROM, dateTO;
    dateFROM.setDate(ui->le_year_from->text().toInt(),ui->le_month_from->text().toInt(),ui->le_day_from->text().toInt());
    dateTO.setDate(ui->le_year_to->text().toInt(),ui->le_month_to->text().toInt(),ui->le_day_to->text().toInt());

    qDebug() << "FROM "+dateFROM.toString(Qt::ISODate);
    qDebug() << "TO "+dateTO.toString(Qt::ISODate);
    bool check = dateTO < dateFROM;
    if(check == false)
    {
       An_result *ar = new An_result();
       ar->show();

       connect(this, SIGNAL(Send_long_face_analysis_all_db(QDate, QDate, int)), ar, SLOT(Recieve_long_face_analysis_all_db(QDate, QDate, int)));

       emit Send_long_face_analysis_all_db(dateFROM, dateTO, ui->le_an_zk->text().toInt());
    }
    else {
       qDebug() << "DATE TO < DATE FOR";
    }
}

    //////////////////     12      //////////////////
if(ui->rb_date->isChecked() && ui->rb_long->isChecked() && ui->rb_to_num->isChecked())
    {
        QDate dateFROM, dateTO;
        dateFROM.setDate(ui->le_year_from->text().toInt(),ui->le_month_from->text().toInt(),ui->le_day_from->text().toInt());
        dateTO.setDate(ui->le_year_to->text().toInt(),ui->le_month_to->text().toInt(),ui->le_day_to->text().toInt());

        qDebug() << "FROM "+dateFROM.toString(Qt::ISODate);
        qDebug() << "TO "+dateTO.toString(Qt::ISODate);
        bool check = dateTO < dateFROM;
        if(check == false)
            {
               An_result *ar = new An_result();
               ar->show();

               connect(this, SIGNAL(Send_long_tel_analysis_all_db(QDate, QDate, int)), ar, SLOT(Recieve_long_tel_analysis_all_db(QDate, QDate, int)));

               emit Send_long_tel_analysis_all_db(dateFROM, dateTO, ui->le_an_zk->text().toInt());
            }
            else {
               qDebug() << "DATE TO < DATE FOR";
            }
    }
}

void Analysis::set_validators()
{
    int min, max;
    Crud *cr = new Crud();
    cr->get_min_zk();   min = cr->zk_id;
    cr->get_max_zk();   max = cr->zk_id;
    ui->le_an_zk->setValidator(new QIntValidator(min,max));
    delete cr;
    ui->le_day_to->setValidator(new QIntValidator(1,31));
    ui->le_day_from->setValidator(new QIntValidator(1,31));

    ui->le_month_to->setValidator(new QIntValidator(1,12));
    ui->le_month_from->setValidator(new QIntValidator(1,12));

    ui->le_year_from->setValidator(new QIntValidator(1960,2100));
    ui->le_year_to->setValidator(new QIntValidator(1960,2100));
}

void Analysis::uniq_array()
{
        std::sort(vector.begin(), vector.end());
        vector.erase(std::unique(vector.begin(), vector.end()), vector.end());
}
