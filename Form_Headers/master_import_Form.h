#ifndef MASTER_IMPORT_FORM_H
#define MASTER_IMPORT_FORM_H

#include <QWidget>
#include <QFileDialog>

namespace Ui {
class Master_import_form;
}

class Master_import_form : public QWidget
{
    Q_OBJECT

public:
    explicit Master_import_form(QWidget *parent = nullptr);
    ~Master_import_form();

private slots:
    void on_pushButton_clicked();
    void on_pb_directory_2_clicked();
private:
    Ui::Master_import_form *ui;
signals:
    void TESTING_open(QString filename, QString password);

};

#endif // MASTER_IMPORT_FORM_H
