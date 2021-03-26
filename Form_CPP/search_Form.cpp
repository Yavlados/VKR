#include "search_Form.h"
#include "ui_search.h"
#include "_Crud.h"
#include "list_master.h"
#include <QRegExpValidator>
#include "popup.h"

Search::Search(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Search)
{
    ui->setupUi(this);
        actual_size = this->size();
    this->updateStates();

//        ui->frame_2->setVisible(false);
//        ui->frame_3->setVisible(false);
//        ui->frame_4->setVisible(false);
//        ui->frame_5->setVisible(false);

//        //

//        ui->frame_widg->setLayout(layout);

//         set_date_forms();

          this->set_tab_orders();
//         //ui->le_zk_id->setCursorPosition(0);

}

Search::~Search()
{
  on_pushButton_clicked();

  delete ui;
}

void Search::keyPressEvent(QKeyEvent *event)
{
       switch(event->key())
    {
     case Qt::Key_Enter:
        on_pb_search_clicked();
        return;
    case Qt::Key_Equal:
        on_pushButton_clicked();
        return;
     case Qt::Key_Escape:
         emit closeThis(this->objectName());
        return;
       case Qt::Key_F1:
          PopUp::instance()->setPopupText("<h2 align=\"middle\">Навигация в окне поиска</h2>"
            "<p><b>\"CTRL\"+\"=\"</b> для сброса результатов поиска</p>"
            "<p><b>\"ENTER\"</b> для выполнения поиска</p>"
            "<p><b>\"ESC\"</b> для закрытия окна поиска</p>", rightMenu);
          return;
    }
}

void Search::set_tab_orders()
{
    ui->lineEdit_id->setFocus();
    ui->hided_le->setFocusProxy(ui->lineEdit_id);
    setTabOrder( ui->pb_search, ui->hided_le);
}

QString Search::create_new_date(QString str)
{
//        if(!str.isEmpty() && str != "--")
//        {
//            if(str.size() < 10)
//            {
//                return "";
//            }

//            //str.replace("*","%");
//            //str.replace("?","_");

////              QString  bd = str;
////            ///ФОРМИРУЕМ
////            QString year = bd.right(4);
////            bd.chop(5);
////            QString month = bd.right(2);
////            bd.chop(3);
////            bd = year+ "-"+month + "-" +bd;
//            return  str;
//        }
//        else
//            return "";
}

void Search::on_pb_search_clicked()
{
    QMessageBox msg;
    For_search *sd = new For_search();
    this->serializeFields(sd);
    sd->updateState();
    if(!sd->state.event && !sd->state.person && !sd->state.contact && !sd->state.telephone){
        msg.setWindowTitle("Внимание!");
        msg.setText("Вы не ввели условий поиска!");
        msg.exec();
        delete sd;
        return;
    }
    bool searchState = sd->search();
    if(searchState){
        msg.setWindowTitle("Результаты поиска");
        if(sd->searchedIds.length() == 0){
            msg.setText("Ничего не найдено!");
            msg.setStandardButtons(QMessageBox::Cancel);
            msg.setButtonText(QMessageBox::Cancel, "Отмена");
            msg.exec();
        }
        else{
            // created copy of searched ids
            auto searchedIds = sd->searchedIds;

            msg.setText("По введенным условиям поиска найдено " + QString::number(sd->searchedIds.length()) +" записей");
            msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            msg.setButtonText(QMessageBox::Ok, "Просмотр");
            msg.setButtonText(QMessageBox::Cancel, "Отмена");
            int a = msg.exec();
            switch(a){
            case QMessageBox::Ok :
                    emit this->showSearchedEvents(searchedIds);
                break;
            case QMessageBox::Cancel:
                break;
            }
        }

    } else{
        msg.setWindowTitle("Внимание!");
        msg.setText("Во время поиска возника ошибка");
    }
    delete sd;






    ///
    /// Сбор данных с полей
    ///
//    Crud *o_t = new Crud();
//    o_t->zk_id = ui->le_zk_id->text().toInt();
//    o_t->lastname = ui->le_last_name->text();
//    o_t->name = ui->le_name->text();
//    o_t->mid_name = ui->le_mid_name->text();
//    ///ДАТА ДОБАВЛЕНИЯ И РОЖДЕНИЯ ПЕРЕДЕЛАТЬ

//    //o_t->birth_date = create_new_date(ui->le_birthday->text());

//    //o_t->birth_date = get_date(ui->le_birth_year->text(),ui->le_birth_month->text(), ui->le_birth_day->text());
//    o_t->reg_city = ui->le_reg_city->text();
//    o_t->reg_street = ui->le_reg_street->text();
//    o_t->reg_home = ui->le_reg_house->text();
//    o_t->reg_corp = ui->le_reg_corp->text();
//    o_t->reg_flat = ui->le_reg_flat->text();

//   //o_t->date_add = create_new_date(ui->le_form_date->text());

//    //o_t->date_add = get_date(ui->le_form_year->text(),ui->le_form_month->text(), ui->le_form_day->text());

//    if (!ui->le_tel_num->text().isEmpty())
//    {
//        Owners_tel *ow = new Owners_tel;
//        ow->oldnum = false;

//        if(ui->rb_all->isChecked())
//            ow->tel_num = ui->le_tel_num->text();
//         else if(ui->rb_mid->isChecked())
//             ow->tel_num ="*"+ui->le_tel_num->text()+"*";
//        else if(ui->rb_starts->isChecked())
//            ow->tel_num =ui->le_tel_num->text()+"*";
//        else if(ui->rb_ends->isChecked())
//            ow->tel_num = "*"+ui->le_tel_num->text();

//        if(ui->cb_old_num->isChecked())
//            ow->oldnum = true;

//        o_t->owt()->append(ow);

//    }
//    QString search_query = create_search_query(o_t);

//    if(search_query.isEmpty())//Если условия поиска пустые
//        QMessageBox::critical(this,QObject::tr("Внимание"),QObject::tr("Вы не ввели условия поиска!")); ///Ругаемся
//    else {//Собрал условия для  поиска
//        QList<Crud*> *temp_list =  List_master::search(search_query);
//        if(temp_list->isEmpty())//Если все прошло успешно, но лист пустой
//            QMessageBox::critical(this,QObject::tr("Внимание"),QObject::tr("Ничего не найдено!")); ///Ругаемся
//        else {//Если в листе есть что-то
//            QMessageBox msgbx;
//            msgbx.setText("Результат поиска<br>");
//            //msgbx.setGeometry(960,510, 180,210);
//            msgbx.setText("<font size = '8'> Найдено " + QString::number(temp_list->count()) + " совпадений </font>");
//            msgbx.setStandardButtons(QMessageBox::Ok | QMessageBox::Open | QMessageBox::Cancel);
//            msgbx.setButtonText(QMessageBox::Ok,"ПРОСМОТР");
//            msgbx.setButtonText(QMessageBox::Open,"СФОРМИРОВАТЬ СПРАВКУ");
//            msgbx.setButtonText(QMessageBox::Cancel,"ОТМЕНА");
//            int ret = msgbx.exec();

//            switch (ret) {
//            case QMessageBox::Ok:
//               emit Show_search_result(temp_list);
//                break;

//            case QMessageBox::Open:
//              Create_search_report(temp_list);
//                break;

//            case QMessageBox::Cancel:
//                break;
//                    }
//        }
//    }
}

void Search::Create_search_report(QList<Crud *> *crudlist)
{
//    An_result *an = new An_result;
//    QString search_report;
//    for (int i = 0; i <crudlist->size(); i++)
//    {
//        search_report+="Записная книжка № "+QString::number(crudlist->at(i)->zk_id)+" \r\n Владелец: "+
//                crudlist->at(i)->lastname+" "+crudlist->at(i)->name+" "+crudlist->at(i)->mid_name+
//                ", "+crudlist->at(i)->birth_date+", ";

//            if(ui->rb_reg->isChecked())
//                search_report+= "адрес регистрации:";
//             else if(ui->rb_liv->isChecked())
//                search_report+= "адрес проживания:";

//            search_report += "г."+
//                crudlist->at(i)->reg_city+" ул. "+crudlist->at(i)->reg_street+" д. "+
//               crudlist->at(i)->reg_home+" к. "+crudlist->at(i)->reg_corp+" кв. "+
//                crudlist->at(i)->reg_corp+" \r\n  \r\n ";
//    }
//    connect(this, SIGNAL(Send_search_result(QString)), an , SLOT(Recieve_search_result(QString)));
//   emit Send_search_result(search_report);
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
        auto name = l->objectName();
        if(name.startsWith("lineEdit"))
            l->clear();
    }

    emit Cancel_search();
}

QString Search::create_search_query(Crud *search_crud)
{

}

QString Search::get_date_from()
{

}

QString Search::get_date_to()
{

}

QString Search::get_date(QString year, QString month, QString day )
{

}

QString Search::get_date_query(QString date, int flag)
{

}

void Search::set_date_forms()
{


}

void Search::bd_edited()
{
    //    ui->le_reg_city->setFocus();
}

void Search::updateStates()
{
    this->detentionDateState = ui->checkBox_detention_date->isChecked();
    this->detentionTimeState = ui->checkBox_detention_time->isChecked();
    this->detentionIntervalState = ui->checkBox_detention_interval->isChecked();
    this->detentionVisibility();
}

void Search::detentionVisibility()
{
    ui->dateEdit_detention_date->setDisabled(!this->detentionDateState);
    ui->timeEdit_detention_time->setDisabled(!this->detentionTimeState);

    ui->dateEdit_detention_date_from->setDisabled(!this->detentionIntervalState);
    ui->dateEdit_detention_date_to->setDisabled(!this->detentionIntervalState);
}

void Search::serializeFields(For_search *fs)
{
    fs->event_id = Util::instance()->modifyForSearch(ui->lineEdit_id->text().toLower());
    fs->event_category = Util::instance()->modifyForSearch( ui->lineEdit_category->text().toLower());
    fs->event_detention_by = Util::instance()->modifyForSearch( ui->lineEdit_dentention_by->text().toLower());
    fs->event_keeping_place = Util::instance()->modifyForSearch( ui->lineEdit_keeping_place->text().toLower());
    fs->event_detention_reason = Util::instance()->modifyForSearch( ui->lineEdit_detention_reason->text().toLower());

    if(this->detentionTimeState)
        fs->event_detention_time = ui->timeEdit_detention_time->time().toString("hh:mm");
    if(this->detentionDateState)
        fs->event_detention_date= ui->dateEdit_detention_date->date().toString(Qt::DateFormat::ISODate);
    if(this->detentionIntervalState){
        fs->event_detention_from = ui->dateEdit_detention_date_from->date().toString(Qt::DateFormat::ISODate);
        fs->event_detention_to = ui->dateEdit_detention_date_to->date().toString(Qt::DateFormat::ISODate); ;
    }

    fs->person_name = Util::instance()->modifyForSearch(ui->lineEdit_name->text().toLower());
    fs->person_lastname= Util::instance()->modifyForSearch(ui->lineEdit_lastname->text().toLower());
    fs->person_midname= Util::instance()->modifyForSearch(ui->lineEdit_midname->text().toLower());
    fs->person_alias= Util::instance()->modifyForSearch(ui->lineEdit_alias->text().toLower());

    fs->telephone_num = Util::instance()->modifyForSearch( ui->lineEdit_telephone_num->text().toLower());
    fs->contact_num = Util::instance()->modifyForSearch( ui->lineEdit_contact_num->text().toLower());

    fs->contact_alias = Util::instance()->modifyForSearch( ui->lineEdit_contact_alias->text().toLower());
}

void Search::on_checkBox_detention_time_toggled(bool checked)
{
    this->detentionTimeState = checked;
    this->detentionVisibility();
}

void Search::on_checkBox_detention_date_toggled(bool checked)
{
    this->detentionDateState = checked;
    this->detentionVisibility();
}

void Search::on_checkBox_detention_interval_toggled(bool checked)
{
    this->detentionIntervalState = checked;
    this->detentionVisibility();
}
