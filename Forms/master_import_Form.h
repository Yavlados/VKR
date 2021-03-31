#ifndef MASTER_IMPORT_FORM_H
#define MASTER_IMPORT_FORM_H

#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QKeyEvent>
/**
 * \file master_import_Form.h
 * \brief Форма импорта данных
*/

/// REFACTORED

namespace Ui {
class Master_import_form;
}

class Master_import_form : public QWidget ///Форма импорта
{
    Q_OBJECT

public:
    explicit Master_import_form(QWidget *parent = 0);
    ~Master_import_form();

    QSize actual_size;
    bool folder = false;
    bool oldData = false;

    void keyPressEvent(QKeyEvent *event);
    void set_tab_orders();

    void focus_on_widget();

private slots:
    void on_pushButton_clicked();
    void on_pb_directory_2_clicked();

private:
    Ui::Master_import_form *ui;
   QString file_path = 0;
signals:
    void TESTING_open(QString filename, QString password, bool folder, bool of_t);
    void closeThis(QString);
};

#endif // MASTER_IMPORT_FORM_H
