#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "analysis_Form.h"
#include "search_Form.h"

#include "master_export_Form.h"
#include "master_import_Form.h"
#include "settings_Form.h"

#include "db_connection.h"
#include "officialtelephones_Form.h"
#include "for_export.h" // ->_Crud.h->_Owners_tel.h->_Contacts.h->db_connection
#include "Import_Form.h"
#include "text_handler.h"
#include "an_result_Form.h"

#include "customtableview.h"
#include "_Event.h"
#include "_MTM_Event.h"
#include "PersonCard.h"
#include "for_import.h"

#include <QMainWindow>
#include <QPushButton>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QFileDialog>
#include <QStyle>
#include "Util.h"

#include <QIcon>
/** \file mainwindow_form.h
    \brief Заголовочный файл класса MainWindow
*/

enum focusOnMain {
    FocusOnLeft =0,
    FocusOnRight
};

namespace Ui {
class MainWindow;
}
enum Model_check_state { All_checked = 1,
                         All_unchecked = 0};

class MainWindow : public QMainWindow ///Форма главного окна
                                      /**
                                         * \brief Форма главного окна
                                         * \par Использует классы форм:
                                         *     @ref analysis_Form.h
                                         *     @ref update_Form.h
                                         *     @ref search_Form.h
                                         *     @ref master_export_Form.h
                                         *     @ref master_import_Form.h
                                         *     @ref settings_Form.h
                                         *     @ref Import_Form.h
                                         *     @ref officialtelephones_Form.h
                                         * \par Использует классы моделей:
                                         *     @ref _MTM_Contacts.h
                                         *     @ref _MTM_Crud.h
                                         *     @ref _MTM_OwTel.h
                                         * \par Использует вспомогательные классы:
                                         *     @ref list_master.h
                                         *     @ref db_connection.h
                                         *     @ref for_export.h
                                        */
{

    Q_OBJECT

public:
    Search *sr;                 //форма поиска
    Master_export_Form  *exprt;  //форма экспорта
    Master_import_form  *imprt;  //форма импорта
    OfficialTelephones *of;     //форма служебных телефонов
    class Analysis *an;         //форма анализа

    ///Атрибут класса MessageBox
    /**
     * \brief Переменная MessageBox
     * \par Необходима  для отображения сообщений в формате MessageBox
     */
    QMessageBox msgbx;

    Model_check_state m_c_s;

     MTM_Event *eventModel;

    ///Указатель на класс для экспорта
    For_export for_exprt;
    For_import for_import;

    ///Счетчик для кнопки
    int p_b_counter;

    ///Указатель на индекс первой таблицы
    QModelIndex eventTableIndex;

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:


private:
    Ui::MainWindow *ui;
    QList<int> size_list;
    QPushButton *p_b_forward;
    QPushButton *p_b_back;

    void setFocusOnTab(QString widgetName , QWidget *widgetOnTab);
    focusOnMain mainwindowFocus;

private slots:
    ///Добавление кнопок пагинации
    /**
     * \brief Кнопки пагинации
     * \par Добавляются PushButton в зависимости от настроек, указанных в форме
     * \ref settings_Form.h
     */
    void Add_pagination_buttons();

    void RefreshTab();

    void on_action_add_triggered();

    void on_action_delete_triggered();

    void on_action_update_triggered();

    void on_action_analysis_triggered();

    void on_action_search_triggered();

    void on_tabWidget_tabCloseRequested(int index);

    void on_action_official_tel_triggered();

    void on_actionexport_triggered();

    void prepare_export(SimpleCrypt crypt, ExportType type, QString filePath);

    void testing_opening(QString filename, QString password, bool folder, bool oldData);

    void on_action_import_triggered();

    void on_action_Settings_triggered();

    void next_page();

    void previous_page();

    void set_fonts();

    void on_tabWidget_2_tabCloseRequested(int index);

    void on_action_add_1_triggered();

    void on_action_2_upd_triggered();

    void on_action_3_del_triggered();

    void on_action_5_show_triggered();

    void set_normal_width(int size);

    void header_clicked(int id);

    void on_action_4_analysis_triggered();

    void on_action_6_triggered();

    void on_action_8_triggered();

    void on_action_9_triggered();

    void on_action_10_triggered();

    void on_action_12_triggered();

    void on_action_13_triggered();

    void keyPressEvent(QKeyEvent *event);

    void on_pb_refresh_clicked();

    void set_shortcuts();

    void next_tab();

    void prev_tab();

    void next_tab_tab2();

    void prev_tab_tab2();

    void set_tab_orders();

    void openPopUp();

    void findIndexByNameTab2(QString);

    void closeOF(QString name);

    void on_eventTable_clicked(const QModelIndex &index);

    void openEditPersonWindow(Person *p);
    void openAddPersonWindow(Person *p, editEvent *ee);
    void personIsAdded(EditPerson *ep);
    void closePersonEdit(EditPerson*);
    void on_eventTable_doubleClicked(const QModelIndex &index);
    void closeEditEvent(editEvent*);
    void closeLinkedEditPersons(QList<EditPerson*> *epList);
    void searchedResults(QList<QString> searchedIds);
    void cancelSearchResults();
    QList<Event *> *getSelectedEvents();
    void clearLabel();
    void createLabel();
signals:
    void Set_validators_an();

    void Fill_table_of();

    void Close_tab();
};



#endif // MAINWINDOW_H
