#ifndef MASTER_EXPORT_FORM_H
#define MASTER_EXPORT_FORM_H

#include "search_Form.h"
#include "enums.h"

#include <QWidget>
#include <QFileDialog>
#include <QCryptographicHash>
#include "simplecrypt.h"
#include "Util.h"
/**
 * \file master_export_Form.h
 * \brief Форма экспорта данных
*/
namespace Ui {
class master_export_Form;
}

class Master_export_Form : public QWidget ///Форма экспорта
{
    Q_OBJECT

public:
    Search *sr;
    int p_b_counter;

    explicit Master_export_Form(QWidget *parent = 0);
    ~Master_export_Form();

    void add_file_path(QString path_from_main);

    QSize actual_size;

    void focus_on_widget();
    void set_tab_orders();

private slots:
    void on_cb_zk_clicked();

    void on_rb_check_clicked();

    void on_rb_check_all_clicked();

    void on_pb_directory_clicked();

    void on_pb_Export_clicked();

    void on_cb_set_password_clicked();

    void keyPressEvent(QKeyEvent *event);

    void on_cb_off_tel_clicked();

private:
    Ui::master_export_Form *ui;
    QString file_path;
    ExportType getExportType();
signals:
    void rb_zk_clicked();
    void rb_check_all();
    void TESTING_export(QString filename, QString password, bool cb_off_tels, bool cb_set_password, bool cb_zk_tels);

    void closeThis(QString);

    /// NEW
    void prepareExport(SimpleCrypt crypt, ExportType type, QString filename);
};

#endif // MASTER_EXPORT_FORM_H
