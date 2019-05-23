#ifndef MASTER_EXPORT_FORM_H
#define MASTER_EXPORT_FORM_H

#include "search_Form.h"

#include <QWidget>
#include <QFileDialog>


namespace Ui {
class master_export_Form;
}

class master_export_Form : public QWidget
{
    Q_OBJECT

public:
    Search *sr = new Search;
    int p_b_counter = 0;
    explicit master_export_Form(QWidget *parent = nullptr);
    ~master_export_Form();

private slots:
    void on_cb_zk_clicked();

    void on_rb_check_clicked();

    void on_rb_check_all_clicked();

    void on_pb_directory_clicked();

    void on_pb_Export_clicked();

    void on_pushButton_clicked();

private:
    Ui::master_export_Form *ui;
signals:

    void rb_zk_clicked();
    void rb_check_all();
    void TESTING_export(QString filename, QString password);
    void TESTING_open(QString filename, QString password);
};

#endif // MASTER_EXPORT_FORM_H
