#ifndef DIALOG_SETTINGS_H
#define DIALOG_SETTINGS_H
#include "settings_connection.h"

#include <QDialog>

namespace Ui {
class Dialog_settings;
}

class Dialog_settings : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_settings(QWidget *parent = nullptr);
    ~Dialog_settings();

private slots:
    void on_pushButton_clicked();

    void on_groupBox_3_toggled(bool arg1);

    void on_groupBox_4_toggled(bool arg1);

    void on_pushButton_2_clicked();

    void closeEvent(QCloseEvent *event);
private:
    Ui::Dialog_settings *ui;
};

#endif // DIALOG_SETTINGS_H
