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
namespace Ui {
class Master_import_form;
}

class Master_import_form : public QWidget ///Форма импорта
{
    Q_OBJECT

public:
    explicit Master_import_form(QWidget *parent = nullptr);
    ~Master_import_form();

    QSize actual_size;
    bool folder = false;
    bool of_t = false;

    void keyPressEvent(QKeyEvent *event);

private slots:
    void on_pushButton_clicked();
    void on_pb_directory_2_clicked();

private:
    QWidget *parent_win;
    Ui::Master_import_form *ui;
   QString file_path = nullptr;
signals:
    void TESTING_open(QString filename, QString password, bool folder, bool of_t);

};

#endif // MASTER_IMPORT_FORM_H
