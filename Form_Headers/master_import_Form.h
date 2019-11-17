#ifndef MASTER_IMPORT_FORM_H
#define MASTER_IMPORT_FORM_H

#include <QWidget>
#include <QFileDialog>
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
private slots:
    void on_pushButton_clicked();
    void on_pb_directory_2_clicked();
private:
    Ui::Master_import_form *ui;
signals:
    void TESTING_open(QString filename, QString password);

};

#endif // MASTER_IMPORT_FORM_H
