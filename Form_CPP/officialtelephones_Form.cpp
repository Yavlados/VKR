#include "officialtelephones_Form.h"
#include "ui_officialtelephones.h"
#include <QMessageBox>
#include "popup.h"

OfficialTelephones::OfficialTelephones(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OfficialTelephones)
{
    ui->setupUi(this);
}

OfficialTelephones::~OfficialTelephones()
{
    delete ui;
}

void OfficialTelephones::set_tab_orders()
{
    ui->hided_le->setFocusProxy(ui->le_search_num);
    ui->le_search_num->setFocus();
    setTabOrder(ui->le_search_num, ui->le_search_name);
    setTabOrder(ui->le_search_name, ui->pushButton);
    setTabOrder(ui->pushButton, ui->pushButton_2);
    setTabOrder(ui->pushButton_2, ui->le_set_num);
    setTabOrder(ui->le_set_num, ui->le_set_name);
    setTabOrder(ui->le_set_name, ui->pb_add);
    setTabOrder(ui->pb_add, ui->pb_del);
    setTabOrder(ui->pb_del, ui->pushButton_3);
    setTabOrder(ui->pushButton_3, ui->pushButton_4);
    setTabOrder( ui->pushButton_4, ui->hided_le);
    //setTabOrder(ui->pushButton_3, ui->le_search_num);
}

void OfficialTelephones::set_label(QList<Off_tels *> list)
{
    while (ui->vl_for_label->count())
    {
        QLayoutItem *item = ui->vl_for_label->takeAt(0);
        delete item->widget();
    }

    QLabel *lb = new QLabel;
    QString str = "Всего записей: "+QString::number(list.size());
    lb->setText(str);
    ui->vl_for_label->addWidget(lb);
}


void OfficialTelephones::focus_on_widget()
{
    ui->le_search_num->setFocus();
}

void OfficialTelephones::Fill_table()
{
    of_model = new MTM_Off_Tels;
    ofTlist = new QList<Off_tels*>;

    if(Off_tels::selectOffTel(ofTlist))
           of_model->setOffTList(ofTlist);

       //ui->tableView->setModel(of_model);
       m1 = new QSortFilterProxyModel();

       m1->setSourceModel(of_model);
       ui->tableView->setModel(m1);
       ui->tableView->resizeColumnToContents(1);
       ui->tableView->resizeColumnToContents(0);
       ui->tableView->setWordWrap(false);
       ui->tableView->horizontalHeader()->setStretchLastSection(true);
       set_label(of_model->actofflist);
}

void OfficialTelephones::on_pb_add_clicked()
{
    Off_tels *of_t = new Off_tels;
    of_t->service_name = ui->le_set_name->text();
    of_t->tel_num = ui->le_set_num->text();
    if (Off_tels::add_off_tel(of_t))
    {//или просто добавлять указатель в список
//        delete of_model;
//        of_model = 0;
//        delete ofTlist;
//        ofTlist = 0;

//        Fill_table();

        ofTlist->append(of_t);
         of_model->setOffTList(ofTlist);

         m1->setSourceModel(of_model);
         ui->tableView->setModel(m1);
         set_label(of_model->actofflist);

    }
    else {
        QMessageBox::critical(this,QObject::tr("Ошибка"),QObject::tr("Не удалось добавить служебный телефон!")); ///Хвалимся
    }
}


void OfficialTelephones::on_pb_del_clicked()
{
   QModelIndex index = ui->tableView->currentIndex();
   QModelIndex index1 = m1->mapToSource(index);

   QMessageBox msg;
   msg.setWindowTitle("Подтверждение");
   msg.setText("Вы действительно хотите удалить выбранный служебный номер?");
   msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
   msg.setButtonText(QMessageBox::Ok, "Да");
   msg.setButtonText(QMessageBox::Cancel, "Нет");

   QString filename;
   int ret = msg.exec();

  switch(ret)
  {
    case QMessageBox::Ok :

      if ( index1.isValid())
      {
          Off_tels *oft = of_model->actofflist.at(index1.row());
         if (Off_tels::del_off_tel(oft))
         {
             ofTlist->removeAt(ofTlist->indexOf(oft));
              of_model->setOffTList(ofTlist);

              m1->setSourceModel(of_model);
              ui->tableView->setModel(m1);
                     set_label(of_model->actofflist);
         }
         else
             {
             QMessageBox::critical(this,QObject::tr("Ошибка"),QObject::tr("Не удалось удалить служебный телефон!")); ///Хвалимся
             };
      }
      return;

  case QMessageBox::Cancel :
      return;
  }

}

void OfficialTelephones::on_pushButton_clicked()
{
    QString tel_num, name;
    tel_num = ui->le_search_num->text();
    name = ui->le_search_name->text();
    //Парсинг
    tel_num.replace("*","%");
    tel_num.replace("?","_");
    name.replace("*","%");
    name.replace("?","_");

    Off_tels::search(ofTlist, tel_num, name);
        of_model->setOffTList(ofTlist);

     m1->setSourceModel(of_model);
     ui->tableView->setModel(m1);
     set_label(of_model->actofflist);
}

void OfficialTelephones::on_pushButton_2_clicked()
{
    if(!ui->le_search_num->text().isEmpty())
        ui->le_search_num->clear();
    if(!ui->le_search_name->text().isEmpty())
        ui->le_search_name->clear();

    Off_tels::selectOffTel(ofTlist);
        of_model->setOffTList(ofTlist);

     m1->setSourceModel(of_model);
     ui->tableView->setModel(m1);
     set_label(of_model->actofflist);

}

void OfficialTelephones::on_pushButton_3_clicked()
{
    if(Off_tels::update(of_model->offlist))
    {
        QMessageBox::information(this,QObject::tr("Успех"),QObject::tr("Данные успешно изменены!")); ///Хвалимся
        Fill_table();
    }
    else
        qDebug()<< "Ошибка при изменении данных. Окно служебных телефонов";
}

void OfficialTelephones::on_pushButton_4_clicked()
{
    Fill_table();
}

void OfficialTelephones::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key::Key_F1:
        showPopUp();
       return;
     case Qt::Key::Key_F:
        on_pushButton_clicked();
        return;
     case Qt::Key::Key_Equal:
        on_pushButton_4_clicked();
        return;
    case Qt::Key::Key_Enter:
       on_pushButton_3_clicked();
       return;
    case Qt::Key::Key_Escape:
        if(ui->tableView->hasFocus())
            set_tab_orders();
        else
       emit closeTab(this->objectName());
        return;
    case Qt::Key::Key_T:
        QItemSelectionModel::SelectionFlags flags = QItemSelectionModel::ClearAndSelect | QItemSelectionModel::SelectCurrent;
        QModelIndex tempIndex = ui->tableView->model()->index(0, 0);
        ui->tableView->selectionModel()->select(tempIndex, flags);
        ui->tableView->setFocus();
        return;
    }
}

void OfficialTelephones::showPopUp()
{
    PopUp::instance()->
            setPopupText("<h2 align=\"middle\">Навигация в окне служебных телефонов</h2>"
                                    "<p><b>\"CTRL\"+\"=\"</b> для сброса изменения данных</p>"
                                    "<p><b>\"ENTER\"</b> для сохранения изменений</p>"
                                    "<p><b>\"CTRL\"+\"F\"</b> для поиска</p>"
                                    "<p><b>\"ESC\"</b> для закрытия окна служебных телефонов или перевода фокуса обратно на виджет</p>"
                                    "<p><b>\"CTRL\"+\"T\"</b> для перевода фокуса</p>", rightMenu);
}
