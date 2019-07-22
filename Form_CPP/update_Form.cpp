#include "update_Form.h"
#include "ui_update.h"
#include "_Crud.h"
#include "_Owners_tel.h"
#include "_Contacts.h"
#include "table_delegate.h"

#include <QSqlRecord>
#include <QStringRef>

Update::Update(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Update)
{
    ui->setupUi(this);
    connect(ui->pb_add_line_telephone, SIGNAL(clicked()), ot_model, SLOT(addRow_owner_tel()));
    connect(this, SIGNAL(Add_contact_row(int)),contacts_model,SLOT(addRow_contact(int)));
    set_validators();
    new_cr = nullptr;
    set_splitter_lines();
    Table_delegate *delegate = new Table_delegate(this);
    ui->tableView->setItemDelegateForColumn(0,delegate);
    ui->tableView_2->setItemDelegateForColumn(0,delegate);

}

Update::~Update()
{
    delete ui;
}

void Update::Recieve_data(Crud *cr)
{
    new_cr = cr;
    Fill_fields_update(new_cr);//заполнение полей

    clear_Vl();

     QLabel *lb = new QLabel("<font size = 10> <h1> <div align=\"center\"> Редактирование </div> </h1> </font>");
     ui->vl_for_label->addWidget(lb);

     QPushButton *p_b = new QPushButton;
     p_b->setText("Редактировать");

     ui->vl_for_button->addWidget(p_b);

     connect(p_b, SIGNAL(clicked()), this ,SLOT(on_pb_Update_clicked()));
}

void Update::Fill_fields_update(Crud *new_cr)
{
    ui->le_last_name->setText(new_cr->lastname);
    ui->le_name->setText(new_cr->name);
    ui->le_mid_name->setText(new_cr->mid_name);
if(!new_cr->birth_date.isEmpty())
{
    auto list = new_cr->birth_date.split("-");

    ui->le_birth_date_day->setText(list.at(2));
    ui->le_birth_date_month->setText(list.at(1));
    ui->le_birth_date_year->setText(list.at(0));
}

    ui->le_check_for->setText(new_cr->check_for);
    ui->le_dop_info->setPlainText(new_cr->dop_info);

    ui->le_reg_city->setText(new_cr->reg_city);
    ui->le_reg_street->setText(new_cr->reg_street);
    ui->le_reg_house->setText(new_cr->reg_home);
    ui->le_reg_corp->setText(new_cr->reg_corp);
    ui->le_reg_flat->setText(new_cr->reg_flat);

    ui->le_liv_city->setText(new_cr->liv_city);
    ui->le_liv_street->setText(new_cr->liv_street);
    ui->le_liv_house->setText(new_cr->liv_home);
    ui->le_liv_corp->setText(new_cr->liv_corp);
    ui->le_liv_flat->setText(new_cr->liv_flat);

    QLabel *lb = new QLabel("<font size = 5>  <div align=\"left\"> Дата добавления: "+new_cr->date_add+" "+new_cr->time_add+" </div>  </font>");
    ui->vl_for_date_add->addWidget(lb);
    if(!new_cr->date_upd.isEmpty())
    {
        QLabel *lb2 = new QLabel("<font size = 5> <div align=\"right\"> Дата редактирования: "+new_cr->date_upd+" </div> </font>");
        ui->vl_for_date_upd->addWidget(lb2);
    }

    ///Если список пустой (редактирование телефона из базы),
    /// то достаю телефоны из БАЗЫ ЗАПРОСОМ и заполняю список
    if(new_cr->owt()->isEmpty())
        Owners_tel::selectZkTelForAdd(new_cr->owt(),new_cr->zk_id);
      ot_model->setOTList(new_cr->owt());

    ot_model->state = Edit_Ot; ///меняю флаги для изменения
    ui->tableView->setModel(ot_model);
    ui->tableView->setColumnWidth(0,250);
    contacts_model->reset_ContactModel();
}

void Update::on_pb_Update_clicked()
{
    ///Сначала собираю дату рождения, ФИО + телефоны собираются динамически для проверки на совпадения
    new_cr->lastname = ui->le_last_name->text();
    new_cr->name = ui->le_name->text();
    new_cr->mid_name= ui->le_mid_name->text();

    get_birthdate();

    msgbx.setGeometry(960,510, 180,210);
    msgbx.setText("<font size = '8'>Подтверждение</font> <br> <font size = '5'>Вы готовы завершить редактирование записной книги?</font>");
    msgbx.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    int ret = msgbx.exec();

    switch (ret) {
    case QMessageBox::Cancel:
        break;
    case QMessageBox::Ok:
    ///ПРОВЕРКА НОВЫХ НОМЕРОВ
        switch (imprt_t) {
        case Update_import_data:
            break;
        case Add_import_data:
            break;

        case Update_pg_data:            //Проверку номеров с базой
            if (!compare_tel_num())     //Делаем только в случае редактирования
                return;
            else
                break;

        case Update_pg_data_import:
            break;
             }

    new_cr->check_for = ui->le_check_for->text();
    new_cr->dop_info = ui->le_dop_info->toPlainText();
    new_cr->reg_city = ui->le_reg_city->text();
    new_cr->reg_street = ui->le_reg_street->text();
    new_cr->reg_home = ui->le_reg_house->text();
    new_cr->reg_corp = ui->le_reg_corp ->text();
    new_cr->reg_flat = ui->le_reg_flat->text();

    new_cr->liv_city = ui->le_liv_city->text();
    new_cr->liv_street = ui->le_liv_street->text();
    new_cr->liv_home = ui->le_liv_house->text();
    new_cr->liv_corp = ui->le_liv_corp->text();
    new_cr->liv_flat = ui->le_liv_flat->text();

    switch (imprt_t) {
    case Update_import_data:
        emit updated_import_crud(new_cr);
         close();
        return;

    case Add_import_data:
        emit add_import_crud(new_cr);
         close();
        return;

    case Update_pg_data:
        if( new_cr->update_zk() )
        {
            if(Crud::save_all_crud(new_cr)) /// Если сохранили телефоны
            {
                QMessageBox::information(this,QObject::tr("Успех"),QObject::tr("Данные сохранены в БД!")); ///Хвалимся
               clear_ALL();
            }
            else
            {        /// Если не удалось добавить телефоны
         QMessageBox::critical(this,QObject::tr("Ошибка"),QObject::tr("Не удалось выполнить обновление данных!"));
                 clear_ALL();
            }
        }
        else {
            QMessageBox::critical(this,QObject::tr("Ошибка"),QObject::tr("Не удалось выполнить обновление данных!"));
                  clear_ALL();
             }
             return;

    case Update_pg_data_import:
        if( new_cr->update_zk() )
        {
            if(Crud::save_all_crud(new_cr)) /// Если сохранили телефоны
            {
                emit update_import_pg();
            }
            else
            {        /// Если не удалось добавить телефоны
         QMessageBox::critical(this,QObject::tr("Ошибка"),QObject::tr("Не удалось выполнить обновление данных!"));
            }
        }
        else {
            QMessageBox::critical(this,QObject::tr("Ошибка"),QObject::tr("Не удалось выполнить обновление данных!"));
             }
            close();
         }

    }
}

void Update::on_pb_Back_to_Main_clicked()
{
    clear_ALL();
}

void Update::on_tableView_clicked(const QModelIndex &index)
{
    //В добавлении мы работаем исключительно с моделью
                  //Также как и в импорте
            contacts_model->setContactList(new_cr->owt()->at(index.row())->cont(), new_cr->owt()->at(index.row())->tel_id);

    qDebug() << new_cr->zk_id << new_cr->owt()->at(index.row())->tel_id << new_cr->owt()->at(index.row())->state;

    contacts_model->state = Edit_cont;
    ui->tableView_2->setModel(contacts_model);
    ui->tableView_2->setColumnWidth(0,250);
    ui->tableView_2->setColumnWidth(1,250);
    qDebug() << new_cr->zk_id << new_cr->owt()->at(index.row())->tel_id << new_cr->owt()->at(index.row())->state;
}

void Update::on_pb_del_line_telephone_clicked()
{
    QModelIndex ind = ui->tableView->currentIndex();
    if( ind.isValid() && new_cr->owt()->count()>1)
    {
        contacts_model->delBindedContacts(new_cr->owt()->at(ind.row())->tel_id);
        ot_model->delRow_owner_tel(ind);
    }
    contacts_model->reset_ContactModel();
}

void Update::on_pb_del_contact_line_clicked()
{
    QModelIndex ind = ui->tableView_2->currentIndex();
    if( ind.isValid())
    {
        contacts_model->delRow_contact(ind);
    }
}

void Update::on_pb_add_contact_line_clicked()
{
    QModelIndex index = ui->tableView->currentIndex();
    if(index.isValid())
        emit Add_contact_row(new_cr->owt()->at(index.row())->tel_id);
}

void Update::clear_ALL()
{
   emit Ready_for_update(new_cr->zk_id);
}

void Update::clear_Vl()
{
    while (ui->vl_for_label->count() > 0)
        delete ui->vl_for_label->takeAt(0);

    while (ui->vl_for_button->count() > 0)
        delete ui->vl_for_button->takeAt(0);

    while (ui->vl_for_cb->count() > 0)
        delete ui->vl_for_cb->takeAt(0);
}

void Update::set_validators()
{
    ui->le_birth_date_day->setValidator(new QIntValidator(1,31));
    ui->le_birth_date_month->setValidator(new QIntValidator(1,12));
    ui->le_birth_date_year->setValidator(new QIntValidator(1960,2100));
}

void Update::on_tableView_2_clicked(const QModelIndex &index)
{
    QModelIndex indexOT = ui->tableView->currentIndex();
    qDebug() << new_cr->owt()->at(indexOT.row())->cont()->at(index.row())->contact_id
             << new_cr->owt()->at(indexOT.row())->cont()->at(index.row())->parent_OT_id ;
}

void Update::set_splitter_lines()
{
    QSplitterHandle *handle = ui->splitter->handle(1);
    QVBoxLayout *layout = new QVBoxLayout(handle);
    layout->setSpacing(0);
    layout->setMargin(0);

    QFrame *line = new QFrame(handle);
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Sunken);
    layout->addWidget(line);

    QSplitterHandle *handle_2 = ui->splitter_2->handle(1);
    QVBoxLayout *layout_2 = new QVBoxLayout(handle_2);
    layout_2->setSpacing(0);
    layout_2->setMargin(0);

    QFrame *line_2 = new QFrame(handle_2);
    line_2->setFrameShape(QFrame::HLine);
    line_2->setFrameShadow(QFrame::Sunken);
    layout_2->addWidget(line_2);

    QSplitterHandle *handle_3 = ui->splitter_3->handle(1);
    QVBoxLayout *layout_3 = new QVBoxLayout(handle_3);
    layout_3->setSpacing(0);
    layout_3->setMargin(0);

    QFrame *line_3 = new QFrame(handle_3);
    line_3->setFrameShape(QFrame::HLine);
    line_3->setFrameShadow(QFrame::Sunken);
    layout_3->addWidget(line_3);
}

bool Update::compare_tel_num()
{
    /// Проверка на уникальность
    QString query_for_nums, query_for_fio; //добавить иф на пустой список телефонов
    for (int i=0; i < new_cr->owt()->size(); i++)
    {
        if (!new_cr->owt()->at(i)->tel_num.isEmpty())
        //Составление запроса для проверки телефонов
        {
            if (query_for_nums.isEmpty())
                query_for_nums += " owners_tel.Telephone_num = ('"+new_cr->owt()->at(i)->tel_num+"') ";
            else
                query_for_nums += " OR owners_tel.Telephone_num = ('"+new_cr->owt()->at(i)->tel_num+"')";
        }
    }
    //Составление запроса для проверки фио и др
        query_for_fio = " zk.lastname = ('"+new_cr->lastname+"') AND"
        " zk.name = ('"+new_cr->name+"') AND"
        " zk.mid_name = ('"+new_cr->mid_name+"') AND";

        if(!new_cr->birth_date.isEmpty())
            query_for_fio += " zk.birth_date = ('"+new_cr->birth_date+"') ";
        else
            query_for_fio += " zk.birth_date is NULL";

    qDebug() << query_for_nums << query_for_fio;

    Crud *cr = new Crud();
     if (!cr->compare_with_base(query_for_nums,query_for_fio, new_cr->zk_id))
     {
         qDebug() << cr->zk_id << cr->owt()->at(0)->parentZK_id
                  << cr->owt()->at(0)->tel_num;

         if( cr->owt()->at(0)->parentZK_id != 0)//Если обнаружилось совпадение по номеру телефона
         {
             cr->zk_id = cr->owt()->at(0)->parentZK_id;//переписываю
             msgbx.setText("<font size = '5'> ВНИМАНИЕ: введенный телефонный номер " +cr->owt()->at(0)->tel_num+" "
                                "обнаружен принадлежим владельцу записной книжки № "+QString::number(cr->zk_id)+"</font>");
             msgbx.setStandardButtons(QMessageBox::Ok | QMessageBox::Open | QMessageBox::Cancel);
             msgbx.setButtonText(QMessageBox::Ok,"Перейти к записной книжке № "+ QString::number( cr->zk_id));
             msgbx.setButtonText(QMessageBox::Open,"Редактировать телефонный номер");
             msgbx.setButtonText(QMessageBox::Cancel,"Закрыть карточку без сохранения");
             int ret = msgbx.exec();

             switch (ret)
             {
             case QMessageBox::Ok:
                 emit open_update_tab(cr);
                 //delete cr;
                 return false; /// во всех случаях return - мы выходим из функции

             case QMessageBox::Open:
                 return false;

             case QMessageBox::Cancel:
              clear_ALL();
                 return false;
              }
         }
     if( cr->zk_id != 0)//Если обнаружилось совпадение по фио
        {
         msgbx.setText("<font size = '5'> ВНИМАНИЕ: введенные фамилия, имя, отчество и дата рождения "
                            "обнаружены принадлежащими владельцу записной книжки № "+QString::number(cr->zk_id)+"</font>");
         msgbx.setStandardButtons(QMessageBox::Ok | QMessageBox::Open | QMessageBox::Cancel);
         msgbx.setButtonText(QMessageBox::Ok,"Перейти к записной книжке № "+ QString::number( cr->zk_id));
         msgbx.setButtonText(QMessageBox::Open,"Редактировать ФИО и дату рождения");
         msgbx.setButtonText(QMessageBox::Cancel,"Закрыть карточку без сохранения");
         int ret = msgbx.exec();

         switch (ret)
         {
         case QMessageBox::Ok:
             emit open_update_tab(cr);
             return false; /// во всех случаях return - мы выходим из функции

         case QMessageBox::Open:
             return false;

         case QMessageBox::Cancel:
          clear_ALL();
             return false;
          }
        }
     }
     return true;
}

QString Update::get_birthdate()
{
    if (!ui->le_birth_date_day->text().isEmpty() && !ui->le_birth_date_month->text().isEmpty() && !ui->le_birth_date_year->text().isEmpty())
    {
        QString day,month,year;

        day = ui->le_birth_date_day->text();
        month = ui->le_birth_date_month->text();
        year=ui->le_birth_date_year->text();

        if (day.count() == 1)
              day.insert(0,"0");

        if (month.count() == 1)
            month.insert(0,"0");

        if(year.toInt()  <  1900)
        {
            msgbx.setText("<font size = '5'> Вы ввели не корректную дату рождения </font>");
            msgbx.setStandardButtons(QMessageBox::Ok);
            msgbx.setButtonText(QMessageBox::Ok,"Вернуться обратно");

            int ret = msgbx.exec();

            switch (ret) {
            case QMessageBox::Ok:
                return nullptr;
            }
        }
      return  new_cr->birth_date = year+"-"+month+"-"+day;
    } else
        return nullptr;
}

void Update::Fill_table_in_add()
{
    new_cr = new Crud();

    new_cr->zk_id = 0; //Зануляю, тк новая ЗК
    if(Owners_tel::selectZkTelForAdd(new_cr->owt(), new_cr->zk_id))
           ot_model->setOTList(new_cr->owt());

       ot_model->state = Edit_Ot; ///меняю флаги для изменения
       ui->tableView->setModel(ot_model);
       ui->tableView->setColumnWidth(0,250);

            clear_Vl();

       QLabel *lb = new QLabel("<font size = 10> <h1> <div align=\"center\"> Добавление новой записной книжки  </div></h1> </font>");
       ui->vl_for_label->addWidget(lb);

       QCheckBox *cb = new QCheckBox("<h3> <b> Адреса совпадают </b></h3>");
       cb->setText("Адреса совпадают");
       ui->vl_for_cb->addWidget(cb);

       QPushButton *p_b = new QPushButton;
       p_b->setText("Добавить новую записную книжку");
       ui->vl_for_button->addWidget(p_b);

       connect(cb, SIGNAL(clicked()), this, SLOT(cb_clicked()));
       connect(p_b, SIGNAL(clicked()), this ,SLOT(Add_zk()));
}

void Update::Add_zk()
{
    /// СНАЧАЛА  СОБИРАЕМ НУЖНЫЕ ПОЛЯ, А ПОТОМ
    /// ПРОВЕРЯЕМ ВВЕДЕННЫЕ НОМЕРА

    get_birthdate();

    new_cr->lastname = ui->le_last_name->text();
    new_cr->name=ui->le_name->text();
    new_cr->mid_name = ui->le_mid_name->text();

    if (!compare_tel_num())
        return;

    msgbx.setText("<font size = '5'><h1> Подтверждение </h1> <br>Вы готовы завершить добавление записной книги?</font>");
    msgbx.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgbx.setButtonText(QMessageBox::Ok,"Подтвердить");
    msgbx.setButtonText(QMessageBox::Cancel,"Отмена");
    int ret = msgbx.exec();

    switch (ret) {
    case QMessageBox::Cancel:
        return;
    case QMessageBox::Ok:

        new_cr->check_for = ui->le_check_for->text();
        new_cr->dop_info = ui->le_dop_info->toPlainText();
        new_cr->reg_city = ui->le_reg_city->text();
        new_cr->reg_street = ui->le_reg_street->text();
        new_cr->reg_home = ui->le_reg_house->text();
        new_cr->reg_corp = ui->le_reg_corp ->text();
        new_cr->reg_flat = ui->le_reg_flat->text();

    foreach (QCheckBox *cb, this->findChildren<QCheckBox*>())

        if (cb->checkState() == Qt::Checked)
        {
            new_cr->liv_city = new_cr->reg_city;
            new_cr->liv_street = new_cr->reg_street;
            new_cr->reg_home = new_cr->liv_home;
            new_cr->reg_corp = new_cr->liv_corp;
            new_cr->reg_flat = new_cr->liv_flat;
        }
        else
        {
            new_cr->liv_city = ui->le_liv_city->text();
            new_cr->liv_street = ui->le_liv_street->text();
            new_cr->liv_home = ui->le_liv_house->text();
            new_cr->liv_corp = ui->le_liv_corp->text();
            new_cr->liv_flat = ui->le_liv_flat->text();
        }

        new_cr->check();

    QList<Crud*> *temp_crudlist = new  QList<Crud*>;
    temp_crudlist->append(new_cr);  ///Лист для работы с list

        if( list->insert_crud_in_db(temp_crudlist))
        {
            QMessageBox::information(this,QObject::tr("Успех"),QObject::tr("ЗК успешно добавлена!"));
            clear_ALL();
        }
        else {
            QMessageBox::critical(this,QObject::tr("Ошибка"),QObject::tr("Не удалось выполнить обновление данных!"));
             clear_ALL();
             }
    }
}

void Update::cb_clicked()
{
  foreach (QCheckBox *ch, this->findChildren<QCheckBox*>())
  {
      if (ch->isChecked())
      {
          ui->le_liv_city->setEnabled(false);
          ui->le_liv_street->setEnabled(false);
          ui->le_liv_house->setEnabled(false);
          ui->le_liv_corp->setEnabled(false);
          ui->le_liv_flat->setEnabled(false);
      }
      else {
          ui->le_liv_city->setEnabled(true);
          ui->le_liv_street->setEnabled(true);
          ui->le_liv_house->setEnabled(true);
          ui->le_liv_corp->setEnabled(true);
          ui->le_liv_flat->setEnabled(true);
      }
  }
}

void Update::recieve_import_data(Crud *cr)
{
    new_cr = cr;
    Fill_fields_update(new_cr);
    ui->pb_Back_to_Main->setVisible(false);
    clear_Vl();

    QLabel *lb = new QLabel();

    QPushButton *p_b = new QPushButton;

    QPushButton *p_b_back = new QPushButton;
    switch (imprt_t)
    {
    case Add_import_data:
        lb->setText("<font size = 10> <h1> <div align=\"center\"> Редактирование импортируемых данных </div></h1> </font>");
         p_b->setText("Редактировать импортируемые данные");
         p_b_back->setText("Вернуться обратно");
        connect(p_b, SIGNAL(clicked()), this ,SLOT(on_pb_Update_clicked()));
        break;
    case Update_pg_data_import:
        lb->setText("<font size = 10> <h1> <div align=\"center\"> Редактирование данных из базы </div></h1> </font>");
         p_b->setText("Редактировать данные из базы");
         p_b_back->setText("Вернуться обратно");
         connect(p_b, SIGNAL(clicked()), this ,SLOT(on_pb_Update_clicked()));
        break;
    case Update_import_data:
        lb->setText("<font size = 10> <h1> <div align=\"center\"> Редактирование импортируемых данных  </div></h1> </font>");
        p_b->setText("Редактировать импортируемые данные");
        p_b_back->setText("Вернуться обратно");
        connect(p_b, SIGNAL(clicked()), this ,SLOT(on_pb_Update_clicked()));
        break;
    }

    ui->vl_for_button->addWidget(p_b);
    ui->vl_for_label->addWidget(lb);
    ui->vl_for_button->addWidget(p_b_back);

    connect(p_b_back, SIGNAL(clicked()), this ,SLOT(close()));
}

void Update::close()
{
    delete this;
}

void Update::update_import_data()
{//РАБОТА с списком, а не с базой!!
    new_cr->lastname = ui->le_last_name->text();
    new_cr->name = ui->le_name->text();
    new_cr->mid_name= ui->le_mid_name->text();
    new_cr->check_for = ui->le_check_for->text();
    new_cr->dop_info = ui->le_dop_info->toPlainText();
    new_cr->reg_city = ui->le_reg_city->text();
    new_cr->reg_street = ui->le_reg_street->text();
    new_cr->reg_home = ui->le_reg_house->text();
    new_cr->reg_corp = ui->le_reg_corp ->text();
    new_cr->reg_flat = ui->le_reg_flat->text();

    get_birthdate();

    new_cr->liv_city = ui->le_liv_city->text();
    new_cr->liv_street = ui->le_liv_street->text();
    new_cr->liv_home = ui->le_liv_house->text();
    new_cr->liv_corp = ui->le_liv_corp->text();
    new_cr->liv_flat = ui->le_liv_flat->text();

    //Самoе главное - телефоны
    int a = 0;
    while (a<new_cr->owt()->size())
    {
        if(new_cr->owt()->at(a)->state == IsRemoved)
            new_cr->owt()->removeAt(a);
        else
            a++;
    }
    switch (imprt_t) {
    case Update_import_data:
        emit updated_import_crud(new_cr);
        break;
    case Add_import_data:
        emit add_import_crud(new_cr);
        break;
    }
    close();
}

