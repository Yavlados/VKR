#include "search_Form.h"
#include "ui_search.h"
#include "_Owners_tel.h"

Search::Search(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Search)
{
    ui->setupUi(this);
    connect(this, SIGNAL(Send_search_result(QString)), an , SLOT(Recieve_search_result(QString)));
}

Search::~Search()
{
    delete ui;
}

void Search::on_pb_search_clicked()
{
    Owners_tel *o_t = new Owners_tel(ui->le_zk_id->text().toInt(),
                        ui->le_last_name->text(),
                        ui->le_name->text(),
                        ui->le_mid_name->text(),
ui->le_birth_day->text()+"."+ui->le_birth_month->text()+"."+ui->le_birth_year->text(),
                        ui->le_reg_city->text(),
                        ui->le_reg_street->text(),
                        ui->le_reg_house->text(),
                        ui->le_reg_corp->text(),
                        ui->le_reg_flat->text(),
ui->le_form_year->text()+"-"+ui->le_form_month->text()+"-"+ui->le_form_day->text(),
                        ui->le_tel_num->text());
            ///
            ///Сначала собираю DateFrom
            ///
    ///Если указан день, месяц, год
    if (!ui->le_period_from_day->text().isEmpty() && !ui->le_period_from_month->text().isEmpty() && !ui->le_period_from_year->text().isEmpty())
    {
      o_t->Date_From.setDate(ui->le_period_from_year->text().toInt(),ui->le_period_from_month->text().toInt(),ui->le_period_from_day->text().toInt());
    }

    /// Если не указан день, но есть месяц и год
    if (ui->le_period_from_day->text().isEmpty() && !ui->le_period_from_month->text().isEmpty() && !ui->le_period_from_year->text().isEmpty())
    {///За день тогда берем первое число
    o_t->Date_From.setDate(ui->le_period_from_year->text().toInt(),ui->le_period_from_month->text().toInt(),1);
    }

    /// Если только год
    if (ui->le_period_from_day->text().isEmpty() && ui->le_period_from_month->text().isEmpty() && !ui->le_period_from_year->text().isEmpty())
    {/// Беру начало года (1 января)
      o_t->Date_From.setDate(ui->le_period_from_year->text().toInt(),1,1);
    }       ///
            ///Теперь собираю DateTo
            ///
    ///Если указан день, месяц, год
    if (!ui->le_period_to_day->text().isEmpty() && !ui->le_period_to_month->text().isEmpty() && !ui->le_period_to_year->text().isEmpty())
    {
        o_t->Date_To.setDate(ui->le_period_to_year->text().toInt(),ui->le_period_to_month->text().toInt(),ui->le_period_to_day->text().toInt());
    }

    /// Если не указан день, но есть месяц и год
    if (ui->le_period_to_day->text().isEmpty() && !ui->le_period_to_month->text().isEmpty() && !ui->le_period_to_year->text().isEmpty())
    {
        int day; /// локальная переменная для определения кол-ва дней в месяце
     o_t->Date_To.setDate(ui->le_period_to_year->text().toInt(),ui->le_period_to_month->text().toInt(),1);
     day = o_t->Date_To.daysInMonth();
     o_t->Date_To.setDate(ui->le_period_to_year->text().toInt(),ui->le_period_to_month->text().toInt(),day);
    }

    /// Если только год
    if (ui->le_period_to_day->text().isEmpty() && ui->le_period_to_month->text().isEmpty() && !ui->le_period_to_year->text().isEmpty())
    {/// Беру конец года (31 декабря)
      o_t->Date_To.setDate(ui->le_period_to_year->text().toInt(),12,31);
    }

    o_t->check();
    o_t->zk_search();

if(o_t->model_is_ready == true)
    emit Send_Model(o_t->model);

if(o_t->search_is_ready == true)
    emit Send_search_result(o_t->search_res);

     delete o_t;
}

void Search::on_pb_back_tomain_clicked()
{
   this->close();
}

void Search::on_pushButton_clicked()
{
    foreach(QLineEdit *l, this->findChildren<QLineEdit*>())
    {
        l->clear();
    }
    emit Cancel_search();
}
