#ifndef DIALOG_SETTINGS_H
#define DIALOG_SETTINGS_H
#include "settings_connection.h"

#include <QDialog>
#include <QKeyEvent>

namespace Ui {
class Dialog_settings;
}

class Dialog_settings : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_settings(QWidget *parent = 0);
    ~Dialog_settings();

private slots:
    void on_pushButton_clicked();

    void on_groupBox_3_toggled(bool arg1);

    void on_groupBox_4_toggled(bool arg1);

    void on_pushButton_2_clicked();

    void closeEvent(QCloseEvent *event);

    void set_settings_from_ini();
    void set_tab_orders_pg_0();
    void set_tab_orders_pg_1();
    void on_tabWidget_currentChanged(int index);
    void keyPressEvent(QKeyEvent *event);

private:
    Ui::Dialog_settings *ui;
    QSettings *settings;
};

#endif // DIALOG_SETTINGS_H
