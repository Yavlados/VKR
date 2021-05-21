#include "mainwindow_Form.h"
#include "ui_mainwindow.h"
#include "db_connection.h"
#include "popup.h"

#include <QDesktopWidget>
#include <QRect>
#include <QSqlDatabase>
#include <QSettings>
#include <QKeyEvent>
#include <QDialogButtonBox>
#include <QShortcut>
#include <QTabBar>

#include "settings_connection.h"
/**
 * \file Mainwindow_Form.cpp
 * \brief Исполняемый файл класса MainWindow
*/

/// REFACTORED

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //ui->tableView->setFocus();
    set_shortcuts();

    this->sr = 0;
    this->exprt = 0;
    this->imprt = 0;
    this->of = 0;
    this->an = 0;
    this->p_b_counter = 0;
    this->p_b_forward = 0;
    this->p_b_back = 0;
    this->mainwindowFocus = FocusOnLeft;

    RefreshTab();
//    QTabBar *tabbar = ui->tabWidget->tabBar();
//    tabbar->tabButton(0,QTabBar::RightSide)->deleteLater();
//    tabbar->setTabButton(0, QTabBar::RightSide, 0);

//    QTabBar *tabbar2 = ui->tabWidget_2->tabBar();
//    tabbar2->tabButton(0,QTabBar::RightSide)->deleteLater();
//    tabbar2->setTabButton(0, QTabBar::RightSide, 0);

    ///---Фиксирую размер окна
        showMaximized();
        int difference = this->frameGeometry().height() - this->geometry().height();
        int height = QApplication::desktop()->availableGeometry().height() - difference;
        this->setFixedSize( QApplication::desktop()->screenGeometry().width(), height);
    ///

    this->size_list = ui->splitter->sizes();
    this->m_c_s = All_unchecked;
    connect(ui->eventTable->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(header_clicked(int)));

    ui->tabWidget->setAttribute(Qt::WA_StyledBackground, true);
    ui->tabWidget_2->setAttribute(Qt::WA_StyledBackground, true);
    set_tab_orders();
}
//-----------------------------------------------------------------------------------//
MainWindow::~MainWindow()
{
    delete ui;
}

//-----------------------------------------------------------------------------------//
void MainWindow::Add_pagination_buttons()
{
    while(ui->button_layout_back->count() != 0)
    {
        QLayoutItem *item = ui->button_layout_back->takeAt(0);
        delete item->widget();
        p_b_back = 0;
    }
    while(ui->button_layout_forward->count() != 0)
    {
        QLayoutItem *item = ui->button_layout_forward->takeAt(0);
        delete item->widget();
        p_b_forward = 0;
    }


     if(this->eventModel->eventList != 0 &&
        this->eventModel->actEventList.size() < this->eventModel->eventList->size())
    {
        if(this->eventModel->eventList->indexOf(this->eventModel->actEventList.at(0)) != 0)
        {
            p_b_back = new QPushButton;
            p_b_back->setText("<<");
            ui->button_layout_back->addWidget(p_b_back);
            connect(p_b_back,SIGNAL(clicked()),this,SLOT(previous_page()));
        }

        if(this->eventModel->eventList->indexOf(this->eventModel->actEventList.at(this->eventModel->actEventList.size()-1))
                < this->eventModel->eventList->size()-1)
        {
            p_b_forward = new QPushButton;
            p_b_forward->setText(">>");
            ui->button_layout_forward->addWidget(p_b_forward);
            connect(p_b_forward,SIGNAL(clicked()),this,SLOT(next_page()));
        }
    }
}
//-----------------------------------------------------------------------------------//
void MainWindow::RefreshTab()
{
    this->eventModel = new MTM_Event();

    QList<Event*> *list = new QList<Event*>;
    if(Event::selectAll(list)){
        eventModel->setEventList(list);
        ui->eventTable->setModel(eventModel);
    }

        ui->eventTable->setWordWrap(false);
        ui->eventTable->resizeColumnsToContents();

        this->Add_pagination_buttons();
        this->clearLabel();
        this->createLabel();
}
//-----------------------------------------------------------------------------------//
void MainWindow::on_action_add_triggered()
{
    Event *newEvent = new Event();

    editEvent *ee = new editEvent;
    ee->setEventAndType(newEvent, addEvent);
    connect(ee, SIGNAL(addPerson(Person*, editEvent*)), this, SLOT(openAddPersonWindow(Person*, editEvent*)));
    connect(ee, SIGNAL(closeThis(editEvent*)), this, SLOT(closeEditEvent(editEvent*)));

    Util::instance()->editEventList()->append(ee);

    ui->tabWidget->insertTab( ui->tabWidget->count()+1 ,
                              Util::instance()->editEventList()->at(Util::instance()->editEventList()->size()-1),
                              "Создание нового события");
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
}
//-----------------------------------------------------------------------------------//
void MainWindow::on_action_delete_triggered()
{
    if(this->eventTableIndex.isValid() && this->eventTableIndex == ui->eventTable->currentIndex())
    {
        Event *selectedEvent = this->eventModel->actEventList.at(this->eventTableIndex.row());
        msgbx.setText("Удаление");
        msgbx.setWindowTitle("Удаление");
        msgbx.setInformativeText("Вы действительно хотите удалить выбранное событие № "+selectedEvent->id);
        msgbx.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        int ret = msgbx.exec();

        switch (ret) {
        case QMessageBox::Cancel:
            break;
        case QMessageBox::Ok:
            if (this->eventTableIndex.isValid())
            {
                for(int a=0; a < Util::instance()->addEventManager()->size(); a++){
                    AddEventManager* aem =  Util::instance()->addEventManager()->at(a);
                    if(aem->parent->localEvent->id == selectedEvent->id){
                        for(int i=0; i<ui->tabWidget->count(); i++){
                            QWidget *tab = ui->tabWidget->widget(i);
                            QString tabName = tab->objectName();

                            if(tabName=="editEvent"){
                                editEvent *localEE = dynamic_cast<editEvent*>(tab);
                                if(localEE->localEvent->id == aem->parent->localEvent->id){
                                    ui->tabWidget->widget(i)->deleteLater();

                                    for(int d=0; d < Util::instance()->editEventList()->size(); d++){
                                        if(aem->parent->localEvent->id == Util::instance()->editEventList()->at(d)->localEvent->id){
                                            Util::instance()->editEventList()->removeAt(d);
                                            break;
                                        }
                                    }

                                }
                            }
                            else if(tabName=="EditPerson"){
                                EditPerson *localEP = dynamic_cast<EditPerson*>(tab);
                                for(int c=0; c < aem->childs->size(); c++){
                                    EditPerson *personOnList = aem->childs->at(c);

                                    if(localEP->person->id == personOnList->person->id){
                                        ui->tabWidget->widget(i)->deleteLater();
                                        for(int r=0; r<Util::instance()->editPersonList()->size(); r++){
                                            if(personOnList->person->id == Util::instance()->editPersonList()->at(r)->person->id){
                                                Util::instance()->editPersonList()->removeAt(r);
                                            }
                                        }
                                        break;
                                    }
                                }
                            }
                        }
                    }

                }

                Event::deleteEvent(selectedEvent);
                this->RefreshTab();
            }
            break;
        }
    }
    else {
        QMessageBox::critical(this,QObject::tr("Внимание"),QObject::tr("Вы не выбрали событие для удаления!")); ///Хвалимся
    }
}
//-----------------------------------------------------------------------------------//
void MainWindow::on_action_update_triggered()
{
    if(this->eventTableIndex.isValid() &&
       this->eventTableIndex == ui->eventTable->currentIndex()) {
        Event *localEvent = this->eventModel->actEventList.at(this->eventTableIndex.row());
        Person::selectByEventId(localEvent->persons(), localEvent->id);

        for (int i=0; i < Util::instance()->editEventList()->size(); i++)
        {
            if(Util::instance()->editEventList()->at(i)->localEvent->id == localEvent->id)
            {
                ui->tabWidget->setCurrentIndex(i+1);
                return;
            }
        }

        editEvent *ee = new editEvent;
        ee->setEventAndType(localEvent, updateEvent);
        connect(ee, SIGNAL(addPerson(Person*, editEvent*)), this, SLOT(openAddPersonWindow(Person*, editEvent*)));
        connect(ee, SIGNAL(closeThis(editEvent*)), this, SLOT(closeEditEvent(editEvent*)));
        connect(ee, SIGNAL(openEditPerson(Person*)), this, SLOT(openEditPersonWindow(Person*)));
        Util::instance()->editEventList()->append(ee);

        ui->tabWidget->insertTab( ui->tabWidget->count()+1 ,
                                  Util::instance()->editEventList()->at(Util::instance()->editEventList()->size()-1),
                                  "Редактирование события №"+localEvent->id);
        ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
    }
    else {
            QMessageBox::critical(this,QObject::tr("Внимание"),QObject::tr("Вы не выбрали событие для изменения!")); ///Хвалимся
        }
}
//-----------------------------------------------------------------------------------//
void MainWindow::on_action_analysis_triggered()
{
    if (an == 0)
    {
        an = new class Analysis;
        ui->tabWidget_2->insertTab( ui->tabWidget_2->count()+1 ,an,"Анализ");
        ui->tabWidget_2->setCurrentIndex(ui->tabWidget_2->count()-1);
        connect(an, SIGNAL(closeThis(QString)), this, SLOT(findIndexByNameTab2(QString)));
    }
    else
        ui->tabWidget_2->setCurrentIndex( ui->tabWidget_2->indexOf(an));

    set_normal_width(an->actual_size.width());
    an->focus_on_widget();
    an->set_tab_orders();

}
//-----------------------------------------------------------------------------------//
void MainWindow::on_action_search_triggered()
{
    if(sr == 0)
    {
         sr = new Search;
         ui->tabWidget_2->insertTab( ui->tabWidget_2->count()+1 ,sr,"Расширенный поиск");
         ui->tabWidget_2->setCurrentIndex(ui->tabWidget_2->count()-1);
         connect(sr,SIGNAL(Cancel_search()),this, SLOT(cancelSearchResults()));
         connect(sr, SIGNAL(closeThis(QString)), this, SLOT(findIndexByNameTab2(QString)));
         connect(sr, SIGNAL(showSearchedEvents(QList<QString>)), this, SLOT(searchedResults(QList<QString>)));

         sr->set_tab_orders();
    }
    else
    {
        ui->tabWidget_2->setCurrentIndex( ui->tabWidget_2->indexOf(sr));
       // sr->set_tab_orders();
    }

    set_normal_width(sr->actual_size.width());
}
//-----------------------------------------------------------------------------------//
void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    ///обработка редактирования/добавления
    if ( ui->tabWidget->widget(index)->objectName() == "OfficialTelephones")
    {
        ui->tabWidget->widget(index)->deleteLater();
        delete of;
        of = 0;

    }
    ///NEW
    else if(ui->tabWidget->widget(index)->objectName() == "EditPerson"){

        if(Util::instance()->editPersonList() != 0){
            EditPerson *ep = dynamic_cast<EditPerson*>(ui->tabWidget->widget(index)); //Приведение типа от виджета к классу
            this->closePersonEdit(ep);
        }
    }
    else if(ui->tabWidget->widget(index)->objectName() == "editEvent"){
        if(Util::instance()->editEventList() != 0){
            editEvent *ee = dynamic_cast<editEvent*>(ui->tabWidget->widget(index)); //Приведение типа от виджета к классу
            this->closeEditEvent(ee);

        }
    }
    this->RefreshTab();
}
//-----------------------------------------------------------------------------------//
void MainWindow::on_tabWidget_2_tabCloseRequested(int index)
{
    ui->tabWidget_2->indexOf(an);
    if ( ui->tabWidget_2->widget(index)->objectName() == "Search")
    {
        ui->tabWidget_2->widget(index)->deleteLater();
        delete sr;
        sr = 0;
        return;
    } else
    if ( ui->tabWidget_2->widget(index)->objectName() == "Analysis")
    {
        ui->tabWidget_2->widget(index)->deleteLater();
        delete an;
        an = 0;
        return;
    } else
    if ( ui->tabWidget_2->widget(index)->objectName() == "master_export_Form")
    {
        ui->tabWidget_2->widget(index)->deleteLater();
        delete exprt;
        exprt = 0;
        return;
    } else
    if ( ui->tabWidget_2->widget(index)->objectName() == "Master_import_form")
    {
        ui->tabWidget_2->widget(index)->deleteLater();
        delete imprt;
        imprt = 0;
        return;
    }

}
//-----------------------------------------------------------------------------------//
void MainWindow::findIndexByNameTab2( QString WidgetName )
{
    int countOfTabs = ui->tabWidget_2->count();
    for(int a=0; a<countOfTabs; a++)
    {
        if(ui->tabWidget_2->widget(a)->objectName() == WidgetName)
        {
            on_tabWidget_2_tabCloseRequested(a);
            break;
        }
    }
}
//-----------------------------------------------------------------------------------//
void MainWindow::closeOF(QString WidgetName)
{
    int countOfTabs = ui->tabWidget->count();
    for(int a=0; a<countOfTabs; a++)
    {
        if(ui->tabWidget->widget(a)->objectName() == WidgetName)
        {
            on_tabWidget_tabCloseRequested(a);
            break;
        }
    }
}
//-----------------------------------------------------------------------------------//
void MainWindow::on_action_official_tel_triggered()
{
    if(of == 0)
        {
        of = new OfficialTelephones;
        ui->tabWidget->insertTab( ui->tabWidget->count()+1 ,of,"Cлужебные телефоны");
        ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
        connect(this,SIGNAL(Fill_table_of()), of, SLOT(Fill_table()));
        connect(of, SIGNAL(closeTab(QString)), this, SLOT(closeOF(QString)));
        emit Fill_table_of();
        of->set_tab_orders();
        }
    else
    {
        ui->tabWidget->setCurrentIndex( ui->tabWidget->indexOf(of));
        of->set_tab_orders();
    }
    of->focus_on_widget();
}
//-----------------------------------------------------------------------------------//
void MainWindow::on_actionexport_triggered()
{
    if(exprt == 0)
    {
        exprt = new Master_export_Form;
        ui->tabWidget_2->insertTab( ui->tabWidget_2->count()+1 ,exprt,"Экспорт данных");
        ui->tabWidget_2->setCurrentIndex(ui->tabWidget_2->count()-1);
        connect(exprt,SIGNAL(rb_zk_clicked()),this, SLOT(on_action_search_triggered()));
        connect(exprt, SIGNAL(prepareExport(SimpleCrypt, ExportType, QString)), this, SLOT(prepare_export(SimpleCrypt, ExportType, QString)));
        connect(exprt, SIGNAL(closeThis(QString)), this, SLOT(findIndexByNameTab2(QString)));
    }
    else
        ui->tabWidget_2->setCurrentIndex( ui->tabWidget_2->indexOf(exprt));

    set_normal_width(exprt->actual_size.width());
    exprt->focus_on_widget();

}
//-----------------------------------------------------------------------------------//
void MainWindow::prepare_export(SimpleCrypt crypt, ExportType type, QString filePath){
    if (filePath.isEmpty())
    {
        QMessageBox::critical(exprt,QObject::tr("Ошибка"),QObject::tr("Вы не указали путь!"));
        return;
    }

    QString messageStart;
    QString exportedFilePath;

    if(type == events || type == eventsPassword){
        QList<Event*> *checkedEvents = this->getSelectedEvents();

        if(checkedEvents->size() == 0){
           QMessageBox::warning(exprt,QObject::tr("Внимание"),QObject::tr("Экспорт не был выполнен, так как вы не выбрали данные!"));
           return;
        }

        exportedFilePath = this->for_exprt.exportEvents(checkedEvents, crypt, type, filePath);
        delete checkedEvents;
        switch (type){
        case events:
            messageStart = "Дела экспортированы успешно без пароля. Данные сохранены в файл: ";
            break;
        case eventsPassword:
            messageStart = "Дела экспортированы и зашифрованы паролем успешно. Данные сохранены в файл: ";
            break;
        }
    }
    else if( type == official || type == officialPassword){
        QList<Off_tels*> *offList = new QList<Off_tels*>();

        if(Off_tels::selectOffTel(offList)){
            exportedFilePath = this->for_exprt.exportOfficial(offList, crypt, type, filePath);
            delete offList;

            switch (type){
            case official:
                messageStart = "Служебные телефоны экспортированы успешно без пароля. Данные сохранены в файл: ";
                break;
            case officialPassword:
                messageStart = "Служебные телефоны экспортированы и зашифрованы паролем успешно. Данные сохранены в файл: ";
                break;
            }
        }
        else{
            QMessageBox::warning(exprt,QObject::tr("Внимание"),QObject::tr("Экспорт не был выполнен, так как во время выгрузки данных из базы возникли неполадки!"));
            return;
        }
    }
    QMessageBox::information(exprt,QObject::tr("Успех"), messageStart + exportedFilePath);

}
//-----------------------------------------------------------------------------------//
void MainWindow::testing_opening(QString filename, QString password, bool folder, bool oldData)
{
    importResult importResults= this->for_import.openFile(filename, password, folder, oldData);

    if(importResults.state == success)
        QMessageBox::information(this,QObject::tr("Успех"), importResults.message);
    else
        QMessageBox::warning(this,QObject::tr("Ошибка"),  importResults.message);
    this->RefreshTab();
}
//-----------------------------------------------------------------------------------//
void MainWindow::on_action_import_triggered()
{
    if(imprt == 0)
    {
        imprt = new Master_import_form(this);
        ui->tabWidget_2->insertTab( ui->tabWidget_2->count()+1 ,imprt,"Импорт данных");
        ui->tabWidget_2->setCurrentIndex(ui->tabWidget_2->count()-1);
        connect(imprt, SIGNAL(TESTING_open(QString,QString, bool, bool)), this, SLOT(testing_opening(QString, QString, bool, bool)));
        connect(imprt, SIGNAL(closeThis(QString)), this, SLOT(findIndexByNameTab2(QString)));

    }
    else
        ui->tabWidget_2->setCurrentIndex( ui->tabWidget_2->indexOf(imprt));

    imprt->set_tab_orders();
    imprt->focus_on_widget();

    set_normal_width(imprt->actual_size.width());
}
//-----------------------------------------------------------------------------------//
void MainWindow::on_action_Settings_triggered()
{
        Settings_Form *settings = new Settings_Form;
        settings->show();
        connect (settings, SIGNAL(Update_main()), this, SLOT(RefreshTab()));
}
//-----------------------------------------------------------------------------------//
void MainWindow::next_page()
{
    if(ui->button_layout_forward->count() >0)
    {
        this->eventModel->next_page();
        ui->eventTable->setModel(this->eventModel);
        Add_pagination_buttons();

        this->clearLabel();
        this->createLabel();
    }
}
//-----------------------------------------------------------------------------------//
void MainWindow::previous_page()
{
   if(ui->button_layout_back->count() >0)
   {
       this->eventModel->previous_page();
       ui->eventTable->setModel(this->eventModel);
       Add_pagination_buttons();

       this->clearLabel();
       this->createLabel();
   }

}
//-----------------------------------------------------------------------------------//
void MainWindow::set_fonts()
{

}
//-----------------------------------------------------------------------------------//
void MainWindow::on_action_add_1_triggered()
{
    on_action_add_triggered();
}
//-----------------------------------------------------------------------------------//
void MainWindow::on_action_2_upd_triggered()
{
    on_action_update_triggered();
}
//-----------------------------------------------------------------------------------//
void MainWindow::on_action_3_del_triggered()
{
    on_action_delete_triggered();
}
//-----------------------------------------------------------------------------------//
void MainWindow::on_action_5_show_triggered()
{
       ui->tabWidget->setCurrentIndex(0);
}
//-----------------------------------------------------------------------------------//
void MainWindow::set_normal_width(int size)
{
    QList<int> size_list;
    size_list.append(size);
    size_list.append(this->size().width() - size);
    ui->splitter->setSizes(size_list);
}
//-----------------------------------------------------------------------------------//
void MainWindow::header_clicked(int id)
{
    if(id != 0)
        return;
    else
    {
        switch (m_c_s)
        {
        case All_unchecked:
            this->eventModel->changeCheckedStateEventList(true);
            ui->eventTable->setModel(this->eventModel);
            m_c_s = All_checked;
            Add_pagination_buttons();
        return;
        case All_checked:
            this->eventModel->changeCheckedStateEventList(false);
            ui->eventTable->setModel(this->eventModel);
            m_c_s = All_unchecked;
            Add_pagination_buttons();
        return;
        }
    }
}
//-----------------------------------------------------------------------------------//
void MainWindow::on_action_4_analysis_triggered()
{
    on_action_analysis_triggered();
}
//-----------------------------------------------------------------------------------//
void MainWindow::on_action_6_triggered()
{
    ui->tabWidget->setCurrentIndex(0);
    ui->tabWidget_2->setCurrentIndex(0);
    ui->splitter->setSizes(size_list);
}
//-----------------------------------------------------------------------------------//
void MainWindow::on_action_8_triggered()
{
    on_action_search_triggered();
}
//-----------------------------------------------------------------------------------//
void MainWindow::on_action_9_triggered()
{
    on_action_official_tel_triggered();
}
//-----------------------------------------------------------------------------------//
void MainWindow::on_action_10_triggered()
{
    on_actionexport_triggered();
}
//-----------------------------------------------------------------------------------//
void MainWindow::on_action_12_triggered()
{
    on_action_import_triggered();
}
//-----------------------------------------------------------------------------------//
void MainWindow::on_action_13_triggered()
{
    on_action_Settings_triggered();
}
//-----------------------------------------------------------------------------------//
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
        case Qt::Key_Enter:
            if(mainwindowFocus == FocusOnRight && this->eventTableIndex.isValid())
                on_action_update_triggered();
            return;

         case Qt::Key_PageUp:
            if(p_b_forward != 0)
                p_b_forward->click();
            return;

        case Qt::Key_PageDown:
           if(p_b_back != 0)
               p_b_back->click();
           return;

        case Qt::Key_F1:
            openPopUp();
        return;

        case Qt::Key_Escape:
            RefreshTab();
        return;
    }

}
//-----------------------------------------------------------------------------------//
void MainWindow::on_pb_refresh_clicked()
{
    RefreshTab();
}
//-----------------------------------------------------------------------------------//
void MainWindow::openPopUp(){
    PopUp::instance()->setPopupText("<h2 align=\"middle\">Навигация в главном окне</h2>"
                                    "<p><b>\"CTRL\"+\"Q\" и \"CTRL\"+\"W\"</b> для переключения вкладок правого окна</p>"
                                    "<p><b>\"CTRL\"+\"S\" и \"CTRL\"+\"D\"</b> для преключения вкладок левого окна</p>"
                                    "<p><b>\"ESC\"</b> для сброса результатов поиска</p>"
                                    "<h2 align=\"middle\">Навигация в таблицах</h2>"
                                    "<p>&rarr; для раскрытия телефонных номеров и контактов фокуса</p>"
                                    "<p>&uarr; и &darr; для перемещения фокуса вверх и вниз в пределах таблицы</p>"
                                    "<p>&larr; для возвращения фокуса назад</p>"
                                    "<p><b>\"Z\"</b> для перемещения фокуса налево по горизонтали</p>"
                                    "<p><b>\"X\"</b> для перемещения фокуса направо по горизонтали</p>", leftMenu);
}
//-----------------------------------------------------------------------------------//
void MainWindow::set_shortcuts()
{
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_W), this, SLOT(next_tab()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q), this, SLOT(prev_tab()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_D), this, SLOT(next_tab_tab2()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_S), this, SLOT(prev_tab_tab2()));

    new QShortcut(QKeySequence(Qt::Key_F2), this, SLOT(on_action_update_triggered()));
    new QShortcut(QKeySequence(Qt::Key_F3), this, SLOT(on_action_add_triggered()));
    new QShortcut(QKeySequence(Qt::Key_F4), this, SLOT(on_action_6_triggered()));
    new QShortcut(QKeySequence(Qt::Key_F6), this, SLOT(on_action_analysis_triggered()));
    new QShortcut(QKeySequence(Qt::Key_F7), this, SLOT(on_action_search_triggered()));
    new QShortcut(QKeySequence(Qt::Key_F8), this, SLOT(on_action_delete_triggered()));
    new QShortcut(QKeySequence(Qt::Key_F10), this, SLOT(on_actionexport_triggered()));
    new QShortcut(QKeySequence(Qt::Key_F11), this, SLOT(on_action_import_triggered()));
    new QShortcut(QKeySequence(Qt::Key_F12), this, SLOT(on_action_Settings_triggered()));
}
//-----------------------------------------------------------------------------------//
void MainWindow::   next_tab()
{
    ui->tabWidget->setCurrentIndex(ui->tabWidget->currentIndex() + 1);
    QWidget *widget = ui->tabWidget->widget(ui->tabWidget->currentIndex());
    QString widgetName = widget->objectName();
    setFocusOnTab(widgetName, widget);
}
//-----------------------------------------------------------------------------------//
void MainWindow::prev_tab()
{
    ui->tabWidget->setCurrentIndex(ui->tabWidget->currentIndex() - 1);
    QWidget *widget = ui->tabWidget->widget(ui->tabWidget->currentIndex());
    QString widgetName = widget->objectName();
    setFocusOnTab(widgetName, widget);
}
//-----------------------------------------------------------------------------------//
void MainWindow::next_tab_tab2()
{
    ui->tabWidget_2->setCurrentIndex(ui->tabWidget_2->currentIndex() + 1);
    QWidget *widget = ui->tabWidget_2->widget(ui->tabWidget_2->currentIndex());
    QString widgetName = widget->objectName();
    setFocusOnTab(widgetName, widget);
}
//-----------------------------------------------------------------------------------//
void MainWindow::prev_tab_tab2()
{
    ui->tabWidget_2->setCurrentIndex(ui->tabWidget_2->currentIndex() - 1);
    QWidget *widget = ui->tabWidget_2->widget(ui->tabWidget_2->currentIndex());
    QString widgetName = widget->objectName();
    setFocusOnTab(widgetName, widget);
}
//-----------------------------------------------------------------------------------//
void MainWindow::setFocusOnTab(QString widgetName, QWidget *widgetOnTab)
{
    ///Ветвления от имени виджета
    if(widgetName == "OfficialTelephones")
    {
        OfficialTelephones *of = dynamic_cast<OfficialTelephones*>(widgetOnTab);
        of->focus_on_widget();
        return;
    }
    else if(widgetName =="tab")
    {
        mainwindowFocus = FocusOnRight;
        QItemSelectionModel::SelectionFlags flags = QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows;
        return;
    }
    else if(widgetName =="Analysis")
    {
        class Analysis *an = dynamic_cast<class Analysis*>(widgetOnTab);
        an->focus_on_widget();
        return;
    }
    else if(widgetName =="tab_2"){
        mainwindowFocus = FocusOnLeft;
        ui->lineEdit->setFocus();
        return;
    }
    else if(widgetName =="Search"){
        Search *sr = dynamic_cast<Search*>(widgetOnTab);
        sr->set_tab_orders();
        return;
    }
    else if(widgetName =="master_export_Form"){

       Master_export_Form *exp = dynamic_cast<Master_export_Form*>(widgetOnTab);
       exp->focus_on_widget();
       return;
   }
    else if(widgetName =="Master_import_form"){
        Master_import_form *imp = dynamic_cast<Master_import_form*>(widgetOnTab);
        imp->focus_on_widget();
        return;
    }
    else if(widgetName =="EditPerson"){
        EditPerson *ep = dynamic_cast<EditPerson*>(widgetOnTab);
        ep->setFocus();
        return;
    }
}
//-----------------------------------------------------------------------------------//
void MainWindow::set_tab_orders()
{
    ui->pb_hided->setFocusProxy(ui->lineEdit);

    setTabOrder(ui->lineEdit, ui->pushButton);
    setTabOrder( ui->pushButton, ui->pb_refresh);
    setTabOrder( ui->pb_refresh, ui->pb_hided);

}
//-----------------------------------------------------------------------------------//
void MainWindow::on_eventTable_clicked(const QModelIndex &index)
{
    this->eventTableIndex = index;
    Event *localEvent = this->eventModel->actEventList.at(index.row());
    Person::selectByEventId(localEvent->persons(), localEvent->id);
    Util::instance()->clearLayout(ui->cardsLayout);
    for ( int i =0; i < localEvent->persons()->size(); i++){
        PersonCard *card = new PersonCard();
        card->setPerson(localEvent->persons()->at(i));
        card->setState(mainWindow);
        ui->cardsLayout->addWidget(card);
    }
}
//-----------------------------------------------------------------------------------//
void MainWindow::openEditPersonWindow(Person *p)
{
    for (int i=0; i < Util::instance()->editPersonList()->size(); i++)
    {
        if(Util::instance()->editPersonList()->at(i)->person->id == p->id)
        {
            ui->tabWidget->setCurrentIndex(i+1);
            return;
        }
    }
    Person::getLinkedPersons(p);
    EditPerson *ep = new EditPerson;
    ep->setType(updatePerson);
    connect(ep, SIGNAL(closeThis(EditPerson*)), this, SLOT(closePersonEdit(EditPerson*)));
    connect(ep, SIGNAL(openLinkedPerson(Person*)), this, SLOT(openEditPersonWindow(Person*)));

    ep->setPerson(p);
    Util::instance()->editPersonList()->append(ep);

    ui->tabWidget->insertTab( ui->tabWidget->count()+1 ,
                              Util::instance()->editPersonList()->at(Util::instance()->editPersonList()->size()-1),
                              "Редактирование фигуранта "+p->name + " "+ p->lastname + " " + p->midname);
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
}
//-----------------------------------------------------------------------------------//
void MainWindow::openAddPersonWindow(Person *p, editEvent *ee)
{
    EditPerson *ep = new EditPerson;
    ep->setType(addPerson);
    ep->setParent(ee);
    connect(ep, SIGNAL(closeThis(EditPerson*)), this, SLOT(closePersonEdit(EditPerson*)));
    connect(ep, SIGNAL(personIsAdded(EditPerson*)), this, SLOT(personIsAdded(EditPerson*)));
    ep->setPerson(p);
    Util::instance()->editPersonList()->append(ep);
    Util::instance()->linkAddEventPerson(ee,ep);

    ui->tabWidget->insertTab( ui->tabWidget->count()+1 ,
                              Util::instance()->editPersonList()->at(Util::instance()->editPersonList()->size()-1),
                              "Добавление нового фигуранта для дела "+ee->localEvent->id);
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
}
//-----------------------------------------------------------------------------------//
void MainWindow::personIsAdded(EditPerson *ep)
{
    editEvent *ee = Util::instance()->getManagerParent(ep);
    ee->addNewPerson(ep->person);
    ee->updateCardsLayout();
    int a = ui->tabWidget->indexOf(ee);
    ui->tabWidget->setCurrentIndex(a);
    QWidget *widget = ui->tabWidget->widget(ui->tabWidget->currentIndex());
    QString widgetName = widget->objectName();
    this->setFocusOnTab(widgetName, widget);
}
//-----------------------------------------------------------------------------------//
void MainWindow::closePersonEdit(EditPerson *ep)
{
    int i = 0;
    QString personIdInTab =  ep->person->id;
    while( i< Util::instance()->editPersonList()->size()){
        if(Util::instance()->editPersonList()->at(i)->person->id == personIdInTab){
            Util::instance()->unlinkEditPersonFromEditEvent(ep);
            delete  Util::instance()->editPersonList()->at(i);
            Util::instance()->editPersonList()->removeAt(i);


            int epIndex = ui->tabWidget->indexOf(ep);
            ui->tabWidget->widget(epIndex)->deleteLater();
            if (Util::instance()->editPersonList()->isEmpty())
            {
                return;
            }
        } else
            i++;
    }

//    int i = 0;
//    while( i< Util::instance()->editPersonList()->size()){
//        if(Util::instance()->editPersonList()->at(i)->editablePerson->id == eventIdInTab){
//            delete  Util::instance()->editPersonList()->at(i);
//            Util::instance()->editPersonList()->removeAt(i);
//            ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
//            QWidget *widget = ui->tabWidget->widget(ui->tabWidget->currentIndex());
//            QString widgetName = widget->objectName();
//            this->setFocusOnTab(widgetName, widget);
//            this->RefreshTab();
//            return;
//        } else
//            i++;
//    }
}
//-----------------------------------------------------------------------------------//
void MainWindow::on_eventTable_doubleClicked(const QModelIndex &index)
{
    this->eventTableIndex = index;
    this->on_action_update_triggered();
}
//-----------------------------------------------------------------------------------//
void MainWindow::closeEditEvent(editEvent *ee)
{
    if(Util::instance()->editEventList() != 0){
        // removing all linked editPerson tabs
        AddEventManager *aem = Util::instance()->getEventManager(ee);
        if(aem != 0)
            this->closeLinkedEditPersons(aem->childs);

        Util::instance()->unlinkAddEventPerson(ee);
        int i = 0;
        QString eventIdInTab =  ee->localEvent->id;
        while( i< Util::instance()->editEventList()->size()){
            if(Util::instance()->editEventList()->at(i)->localEvent->id == eventIdInTab){
                delete  Util::instance()->editEventList()->at(i);
                Util::instance()->editEventList()->removeAt(i);
                ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
                QWidget *widget = ui->tabWidget->widget(ui->tabWidget->currentIndex());
                QString widgetName = widget->objectName();
                this->setFocusOnTab(widgetName, widget);
                this->RefreshTab();
                return;
            } else
                i++;
        }
     }
}

void MainWindow::closeLinkedEditPersons(QList<EditPerson *> *epList)
{
    QList<EditPerson *> *copyList = new QList<EditPerson *>();

    for(int a =0; a < epList->size(); a++){
        copyList->append(epList->at(a));
    }

    for (int a =0; a < copyList->size(); a++) {
        EditPerson *ep = copyList->at(a);
        this->closePersonEdit(ep);
    }
}
//-----------------------------------------------------------------------------------//
void MainWindow::searchedResults(QList<QString> searchedIds)
{
    QLabel *lb = new QLabel;
    QString str = "Результаты поиска: "+QString::number(searchedIds.size())+" записей";
    lb->setText(str);
    QPushButton *pb = new QPushButton;
    pb->setText("Сбросить");
    connect( pb, SIGNAL(clicked()),this, SLOT(cancelSearchResults()));

    ui->searched_data_layout->addWidget(lb);
    ui->searched_data_layout->addWidget(pb);

    QList<Event*> *list = new QList<Event*>;
    if(Event::selectSearchedIds(list, searchedIds)){
        eventModel->setEventList(list);
        ui->eventTable->setModel(eventModel);
    }
}
//-----------------------------------------------------------------------------------//
void MainWindow::cancelSearchResults()
{
    int size = ui->searched_data_layout->count();
    for (int a = 0; a< size; a++) {
        QLayoutItem *item = ui->searched_data_layout->takeAt(0);
        delete item->widget();
    }
    this->RefreshTab();
}

QList<Event *> *MainWindow::getSelectedEvents()
{
    QList<Event *> *events = new QList<Event *>();

    for (int a = 0; a<this->eventModel->eventList->size(); a++) {
        Event * ev = this->eventModel->eventList->at(a);
        if( ev->checkState_ == Unchecked_) continue;
        else{
            Person::selectByEventId(ev->persons(), ev->id);
            for (int b = 0; b<ev->persons()->size();b++) {
                Person *per = ev->persons()->at(b);
                Telephone::selectTelephone(per->telephones(), per->id);
                for (int c=0; c<per->telephones()->size(); c++) {
                    Telephone *tel =    per->telephones()->at(c);
                    Contact::selectContacts(tel->cont(), tel->id);
                }
            }
        }
        events->append(ev);
    }

    return events;
}

void MainWindow::clearLabel()
{
    while(ui->label_layout->count())
    {
        QLayoutItem *item = ui->label_layout->takeAt(0);
        delete item->widget();
    }
}

void MainWindow::createLabel()
{
    if(this->eventModel->eventList == 0) return;

    QLabel *lb = new QLabel;
    QString str = "Всего записей: "+QString::number(this->eventModel->eventList->size()) +" ("+this->eventModel->actEventList.at(0)->id+"..."+
            this->eventModel->actEventList.at(this->eventModel->actEventList.size()-1)->id+")";
    lb->setText(str);
    ui->label_layout->addWidget(lb);
}
