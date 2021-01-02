#ifndef SETTINGS_FORM_H
#define SETTINGS_FORM_H

#include <QWidget>
#include <QSettings>
#include <QKeyEvent>
#include <QCloseEvent>
#include "settings_connection.h"

/**
 * \file settings_Form.h
 * \brief Форма настроек
*/

namespace Ui {
class Settings_Form;
}

class Settings_Form : public QWidget    ///Форма поиска
{
    Q_OBJECT

public:
    explicit Settings_Form(QWidget *parent = 0);
    ~Settings_Form();
   void dragEnterEvent(QDragEnterEvent *e);

private slots:

    void up_check_boxes();

    void on_pb_save_settings_clicked();

    void on_groupBox_clicked();

    void on_pushButton_clicked();

    void set_settings_from_ini();

    void keyPressEvent(QKeyEvent *event);

private:
    void closeEvent(QCloseEvent *event);
    void set_tab_orders();

    Ui::Settings_Form *ui;

private slots:
   void on_groupBox_4_toggled(bool arg1);
   void on_groupBox_3_toggled(bool arg1);

signals:
    void Update_main();
};

#endif // SETTINGS_FORM_H
