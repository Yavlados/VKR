#ifndef SETTINGS_FORM_H
#define SETTINGS_FORM_H

#include <QWidget>
#include <QSettings>
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
    explicit Settings_Form(QWidget *parent = nullptr);
    ~Settings_Form();
   void dragEnterEvent(QDragEnterEvent *e);

private slots:

    void on_pb_save_settings_clicked();

    void on_groupBox_clicked();

private:
    void closeEvent(QCloseEvent *event);
    Ui::Settings_Form *ui;
signals:
    void Update_main();
};

#endif // SETTINGS_FORM_H
