#include "editEvent.h"
#include "ui_editEvent.h"

editEvent::editEvent(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::editEvent)
{
    ui->setupUi(this);
    this->d = new Date_form();
    ui->hl_date->addWidget(d);
    this->newPersonCounter=0;
}

editEvent::~editEvent()
{
    delete ui;

}

void editEvent::setEventAndType(Event *e, editEventState s)
{
    this->localEvent = e;
    this->state = s;

    ui->le_time->           setText(e->detention_time);
    ui->le_reason->         setText(e->detention_reason);
    ui->le_category->       setText(e->category);
    ui->le_additional->     setText(e->additional);
    ui->le_detention_by->   setText(e->detention_by);
    ui->le_keeping_place->  setText(e->keeping_place);
    if(this->state == updateEvent){
        QStringList list = e->detention_date.split("-");
        this->d->set_day(list[2]);
        this->d->set_month(list[1]);
        this->d->set_year(list[0]);
        this->updateCardsLayout();
    }
}

void editEvent::updateCardsLayout()
{
    this->clearLayout(ui->cardsLayout);
    for ( int i =0; i < this->localEvent->persons()->size(); i++){
        Person *p = this->localEvent->persons()->at(i);

        if(p->state != IsRemoved){
            PersonCard *card = new PersonCard();
            connect(card, SIGNAL(removePerson(Person*)), this, SLOT(setPersonToRemove(Person*)));
            card->setPerson(p);
//            connect(card, SIGNAL(openEditWindow(Person*)), this, SLOT(openEditWindow(Person*)));
            ui->cardsLayout->addWidget(card);
        }
    }
}

void editEvent::clearLayout(QLayout *layout)
{
    QLayoutItem *item;
        while((item = layout->takeAt(0))) {
            if (item->layout()) {
                clearLayout(item->layout());
                delete item->layout();
            }
            if (item->widget()) {
               delete item->widget();
            }
            delete item;
        }
}

void editEvent::on_pb_add_person_clicked()
{
    Person *p = new Person();
    p->id = "raw"+QString::number( this->newPersonCounter );
    this->newPersonCounter += 1;

    emit addPerson(p, this);
}

void editEvent::on_pb_cancel_clicked()
{
    msgbx.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

    msgbx.setText("<font size = '8'>Подтверждение</font> <br> <font size = '5'>"
                  "Вы готовы выйти без сохранения?</font>");

    msgbx.setWindowTitle("Выберите действие");

    int ret = msgbx.exec();

    switch (ret) {
    case QMessageBox::Cancel:
        break;
    case QMessageBox::Ok:
        this->closeWidget();
        break;
    }
}

void editEvent::addNewPerson(Person *p)
{
    this->localEvent->persons()->append(p);
}

void editEvent::closeWidget()
{
    emit closeThis(this);
}

void editEvent::setPersonToRemove(Person *pers)
{
    for(int a = 0; a<this->localEvent->persons()->size(); a++ ){
        auto person = this->localEvent->persons()->at(a);
        if(person->id == pers->id){
            person->state = IsRemoved;
            this->updateCardsLayout();
            return;
        }
    }
}

void editEvent::on_pb_save_clicked()
{
    this->localEvent->detention_time    = ui->le_time->text().trimmed();
    this->localEvent->detention_reason = ui->le_reason->text().trimmed();
    this->localEvent->category =  ui->le_category->text().trimmed();
    this->localEvent->additional = ui->le_additional->toPlainText().trimmed();
    this->localEvent->detention_by = ui->le_detention_by->text().trimmed();
    this->localEvent->keeping_place = ui->le_keeping_place->text().trimmed();

    this->localEvent->detention_date = this->d->get_day().trimmed()+"."+this->d->get_month().trimmed()+"."+this->d->get_year().trimmed();

    msgbx.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    if(this->state==updateEvent)
        msgbx.setText("<font size = '8'>Подтверждение</font> <br> <font size = '5'>"
                  "Вы готовы завершить редактирование уголовного дела?</font>");

    else if(this->state==addEvent)
        msgbx.setText("<font size = '8'>Подтверждение</font> <br> <font size = '5'>"
              "Вы готовы завершить добавление уголовного дела?</font>");

    msgbx.setWindowTitle("Выберите действие");

    int ret = msgbx.exec();

    switch (ret) {
    case QMessageBox::Cancel:
        break;
    case QMessageBox::Ok:
        bool state = false;
        if(this->state == updateEvent){
          state = Event::updateEvent(this->localEvent);
         } else if(this->state == addEvent){
           state = Event::createEvent(this->localEvent);
         }

        if(state){
               msgbx.setText("<font size = '5'>Данные усешно сохранены.</font>");
               msgbx.setWindowTitle("Успех");
               msgbx.setStandardButtons(QMessageBox::Ok);
               msgbx.exec();
                this->closeWidget();
           } else {
               msgbx.setText("<font size = '5'>Во время сохранения данных возникла следующая ошибка:"+
                             db_connection::instance()->lastError+"</font>");
               msgbx.setWindowTitle("Ошибка");
               msgbx.setStandardButtons(QMessageBox::Ok);
               msgbx.exec();
           }
        break;
    }
}
