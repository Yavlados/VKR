#include "search_Form.h"
#include "ui_search.h"
#include "_Crud.h"
#include "list_master.h"

Search::Search(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Search)
{
    ui->setupUi(this);
        actual_size = this->size();
}

Search::~Search()
{
    delete ui;
}


void Search::on_pb_search_clicked()
{
    ///
    /// Сбор данных с полей
    ///
    Crud *o_t = new Crud();
    o_t->zk_id = ui->le_zk_id->text().toInt();
    o_t->lastname = ui->le_last_name->text();
    o_t->name = ui->le_name->text();
    o_t->mid_name = ui->le_mid_name->text();
    ///ДАТА ДОБАВЛЕНИЯ И РОЖДЕНИЯ ПЕРЕДЕЛАТЬ
    o_t->birth_date = get_date(ui->le_birth_year->text(),ui->le_birth_month->text(), ui->le_birth_day->text());
    o_t->reg_city = ui->le_reg_city->text();
    o_t->reg_street = ui->le_reg_street->text();
    o_t->reg_home = ui->le_reg_house->text();
    o_t->reg_corp = ui->le_reg_corp->text();
    o_t->reg_flat = ui->le_reg_flat->text();
    o_t->date_add = get_date(ui->le_form_year->text(),ui->le_form_month->text(), ui->le_form_day->text());

    if (!ui->le_tel_num->text().isEmpty())
    {
        Owners_tel *ow = new Owners_tel;
        ow->tel_num = ui->le_tel_num->text();
        o_t->owt()->append(ow);
    }
    QString search_query = create_search_query(o_t);

    if(search_query.isEmpty())//Если условия поиска пустые
        QMessageBox::critical(this,QObject::tr("Внимание"),QObject::tr("Вы не ввели условия поиска!")); ///Ругаемся
    else {//Собрал условия для  поиска
        QList<Crud*> *temp_list =  List_master::search(search_query);
        if(temp_list->isEmpty())//Если все прошло успешно, но лист пустой
            QMessageBox::critical(this,QObject::tr("Внимание"),QObject::tr("Ничего не найдено!")); ///Ругаемся
        else {//Если в листе есть что-то
            QMessageBox msgbx;
            msgbx.setText("Результат поиска<br>");
            //msgbx.setGeometry(960,510, 180,210);
            msgbx.setText("<font size = '8'> Найдено " + QString::number(temp_list->count()) + " совпадений </font>");
            msgbx.setStandardButtons(QMessageBox::Ok | QMessageBox::Open | QMessageBox::Cancel);
            msgbx.setButtonText(QMessageBox::Ok,"ПРОСМОТР");
            msgbx.setButtonText(QMessageBox::Open,"СФОРМИРОВАТЬ СПРАВКУ");
            msgbx.setButtonText(QMessageBox::Cancel,"ОТМЕНА");
            int ret = msgbx.exec();

            switch (ret) {
            case QMessageBox::Ok:
               emit Show_search_result(temp_list);
                break;

            case QMessageBox::Open:
              Create_search_report(temp_list);
                break;

            case QMessageBox::Cancel:
                break;
                    }
        }
    }
}

void Search::Create_search_report(QList<Crud *> *crudlist)
{
    An_result *an = new An_result;
    QString search_report;
    for (int i = 0; i <crudlist->size(); i++)
    {
        search_report+="Записная книжка № "+QString::number(crudlist->at(i)->zk_id)+" \r\n Владелец: "+
                crudlist->at(i)->lastname+" "+crudlist->at(i)->name+" "+crudlist->at(i)->mid_name+
                ", "+crudlist->at(i)->birth_date+", адрес регистрации: г."+
                crudlist->at(i)->reg_city+" ул. "+crudlist->at(i)->reg_street+" д. "+
               crudlist->at(i)->reg_home+" к. "+crudlist->at(i)->reg_corp+" кв. "+
                crudlist->at(i)->reg_corp+" \r\n  \r\n ";
    }
    connect(this, SIGNAL(Send_search_result(QString)), an , SLOT(Recieve_search_result(QString)));
   emit Send_search_result(search_report);
}

void Search::on_pb_back_tomain_clicked()
{
    ///Переделать
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

QString Search::create_search_query(Crud *search_crud)
{
    QString qry;
    if(search_crud->zk_id != 0)
    {
        QString temp = QString::number(search_crud->zk_id);
        qry += " AND zk.Zk_id = "+temp+" ";
    }

    if (!search_crud->owt()->isEmpty())
    {//Замена символов для шаблона поиска
        search_crud->owt()->at(0)->tel_num.replace("*","%");
        search_crud->owt()->at(0)->tel_num.replace("?","_");

       qry += " AND owners_tel.Telephone_num LIKE '"+search_crud->owt()->at(0)->tel_num+"'"
               " AND owners_tel.FK_Telephone_Zk = zk.Zk_id ";
    }

    if (!search_crud->lastname.isEmpty())
    {
        qry += " AND LOWER(zk.Lastname) LIKE LOWER('"+search_crud->lastname+"%') ";
    }
    if (!search_crud->name.isEmpty())
    {
        qry += " AND LOWER(zk.Name) LIKE LOWER('"+search_crud->name+"%')";
    }
    if (!search_crud->mid_name.isEmpty())
    {
        qry += " AND LOWER(zk.Mid_name) LIKE LOWER('"+search_crud->mid_name+"%')";
    }
    //////////////////////////////////////////
    if(!search_crud->birth_date.isEmpty())
    {
       qry += get_date_query(search_crud->birth_date, 1);
    }

    if (!search_crud->reg_city.isEmpty())
    {
        qry += " AND LOWER(zk.Reg_city) LIKE LOWER('"+search_crud->reg_city+"%')";
    }
    if (!search_crud->reg_street.isEmpty())
    {
        qry += " AND LOWER(zk.Reg_street ) LIKE LOWER('"+search_crud->reg_street+"%')";
    }

    if (!search_crud->reg_home.isEmpty())
    {
        qry += " AND zk.Reg_home =('"+search_crud->reg_home+"')";
    }

    if (!search_crud->reg_corp.isEmpty())
    {
        qry += " AND zk.Reg_corp = ('"+search_crud->reg_corp+"')";
    }
    if (!search_crud->reg_flat.isEmpty())
    {
        qry += " AND zk.Reg_flat = ('"+search_crud->reg_flat+"')";
    }
/////////////////////////////////////////////////////////
    if(!search_crud->date_add.isEmpty())
    {
        qry += get_date_query(search_crud->date_add, 2);
    }
/////////////////////////////////////////////////////
        if(!get_date_from().isEmpty())
        {
            qry += " AND zk.date_add >= ('"+ Date_From+"')";
        }
        if(!get_date_to().isEmpty())
        {
            qry += " AND zk.date_add <= ('"+ Date_To+"')";
        }
    return  qry;
}

QString Search::get_date_from()
{
       ///Сначала собираю DateFrom
       ///
   ///Если указан день, месяц, год
   if (!ui->le_day_from->text().isEmpty() && !ui->le_month_from->text().isEmpty() && !ui->le_year_from->text().isEmpty())
   {
       Date_From = ui->le_year_from->text() +"-"+ui->le_month_from->text() +"-"+ui->le_day_from->text();
    return Date_From;
   }

   /// Если не указан день, но есть месяц и год
   if (ui->le_day_from->text().isEmpty() && !ui->le_month_from->text().isEmpty() && !ui->le_year_from->text().isEmpty())
   {///За день тогда берем первое число
   Date_From = ui->le_year_from->text() +"-"+ui->le_month_from->text() +"-01";
    return Date_From;
   }

   /// Если только год
   if (ui->le_day_from->text().isEmpty() && ui->le_month_from->text().isEmpty() && !ui->le_year_from->text().isEmpty())
   {/// Беру начало года (1 января)
   Date_From = ui->le_year_from->text() +"-01-01";
    return Date_From;
   }
   return nullptr;
}

QString Search::get_date_to()
{
 ///Теперь собираю DateTo
 ///
///Если указан день, месяц, год
    if (!ui->le_day_to->text().isEmpty() && !ui->le_month_to->text().isEmpty() && !ui->le_year_to->text().isEmpty())
    {
        Date_To = ui->le_year_to->text() +"-"+ ui->le_month_to->text() +"-"+ui->le_day_to->text();
     return Date_To;
    }

/// Если не указан день, но есть месяц и год
    if (ui->le_day_to->text().isEmpty() && !ui->le_month_to->text().isEmpty() && !ui->le_year_to->text().isEmpty())
    {
        QDate date_to_local;
        int day; /// локальная переменная для определения кол-ва дней в месяце
        date_to_local.setDate(ui->le_year_to->text().toInt(),ui->le_month_to->text().toInt(),1);
        day = date_to_local.daysInMonth();
        Date_To = ui->le_year_to->text() +"-"+ui->le_month_to->text() +"-"+QString::number(day);
     return Date_To;
    }

    /// Если только год
    if (ui->le_day_to->text().isEmpty() && ui->le_month_to->text().isEmpty() && !ui->le_year_to->text().isEmpty())
    {/// Беру конец года (31 декабря)
    Date_To=ui->le_year_to->text()+"-12-31";
     return Date_To;
    }
    return nullptr;
}

QString Search::get_date(QString year, QString month, QString day )
{
    if(month.count() == 1)
        month = "0"+month;

    if(day.count() == 1)
        day = "0"+day;

    QString date;
    ///Полная дата
    if (!year.isEmpty() && !month.isEmpty() && !day.isEmpty())
    {
        date = "F  "; //Префикс полной даты (3 символа)
        date += year +"-"+ month +"-"+day;
        return date;
    }

    /// Если не указан день, но есть месяц и год
    if (day.isEmpty() && !month.isEmpty() && !year.isEmpty())
    {
    date = "MY "; //Префикс месяца и года (3 символа)
    date += month +"-"+year;
    return date;

    }

    /// Если только год
    if (day.isEmpty() && month.isEmpty() && !year.isEmpty())
    {
    date = "Y  "; //Префикс года (3 символа)
    date +=year;
    return date;
    }

    /// Если только месяц
    if (day.isEmpty() && !month.isEmpty() && year.isEmpty())
    {
    date = "M  "; //Префикс месяца (3 символа)
    date += month;
    return date;
    }

    /// Если только день
    if (!day.isEmpty() && month.isEmpty() &&year.isEmpty())
    {
    date = "D  "; //Префикс дня (3 символа)
    date += day;
    return date;
    }

    /// Если только день и месяц
    if (!day.isEmpty() && !month.isEmpty() &&year.isEmpty())
    {
    date = "DM "; //Префикс дня (3 символа)
    date += day+"-"+month;
    return date;
    }

    /// Если только день и год
    if (!day.isEmpty() && month.isEmpty() && !year.isEmpty())
    {
    date = "DY "; //Префикс дня (3 символа)
    date += day+"-"+year;
    return date;
    }

   if(date.isEmpty())
       return nullptr;
}

QString Search::get_date_query(QString date, int flag)
{
    QString query, temp;
    temp = date.left(3);
    date.remove(0,3);
    if (temp == "F  ")
    {
        switch (flag) {
        case 1:
            query = " AND zk.Birth_date = ('"+ date+"')";
            return query;
        case 2:
            query = " AND zk.date_add = ('"+ date+"')";
            return query;
        }
    }
    if (temp == "MY ")
    {
        switch (flag) {
        case 1:
     query =" AND  EXTRACT(MONTH FROM zk.Birth_date) = ('"+date.left(2)+"') "
            " AND  EXTRACT(YEAR FROM zk.Birth_date) = ('"+date.right(4)+"')";
            return query;
        case 2:
     query =" AND  EXTRACT(MONTH FROM zk.date_add) = ('"+date.left(2)+"') "
            " AND  EXTRACT(YEAR FROM zk.date_add) = ('"+date.right(4)+"')";
            return query;
        }
    }
    if (temp == "Y  ")
    {
        switch (flag) {
        case 1:
     query = "AND  EXTRACT(YEAR FROM zk.Birth_date) = ('"+date.right(4)+"')";
            return query;
        case 2:
     query = " AND  EXTRACT(YEAR FROM zk.date_add) = ('"+date.right(4)+"')";
            return query;
        }
    }
    if (temp == "M  ")
    {
        switch (flag) {
        case 1:
     query =" AND  EXTRACT(MONTH FROM zk.Birth_date) = ('"+date.left(2)+"')";
            return query;
        case 2:
     query = " AND  EXTRACT(MONTH FROM zk.date_add) = ('"+date.left(2)+"')";
            return query;
        }
    }
    if (temp == "D  ")
    {
        switch (flag) {
        case 1:
     query =" AND  EXTRACT(DAY FROM zk.Birth_date) = ('"+date.left(2)+"')";
            return query;
        case 2:
     query = " AND  EXTRACT(DAY FROM zk.date_add) = ('"+date.left(2)+"')";
            return query;
        }
    }
    if (temp == "DM ")
    {
        switch (flag) {
        case 1:
     query =" AND  EXTRACT(MONTH FROM zk.Birth_date) = ('"+date.left(2)+"')"
            " AND  EXTRACT(DAY FROM zk.Birth_date) = ('"+date.right(2)+"')";
            return query;
        case 2:
     query = " AND  EXTRACT(MONTH FROM zk.date_add) = ('"+date.left(2)+"')"
             " AND  EXTRACT(DAY FROM zk.date_add) = ('"+date.right(2)+"')";
            return query;
        }
    }
    if (temp == "DY ")
    {
        switch (flag) {
        case 1:
     query =" AND  EXTRACT(MONTH FROM zk.Birth_date) = ('"+date.left(2)+"')"
            " AND  EXTRACT(YEAR FROM zk.Birth_date) = ('"+date.right(4)+"')";
            return query;
        case 2:
     query = " AND  EXTRACT(MONTH FROM zk.date_add) = ('"+date.left(2)+"')"
             " AND  EXTRACT(YEAR FROM zk.date_add) = ('"+date.right(4)+"')";
            return query;
        }
    }
    return query;
}
