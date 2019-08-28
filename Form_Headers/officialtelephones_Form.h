#ifndef OFFICIALTELEPHONES_FORM_H
#define OFFICIALTELEPHONES_FORM_H

#include "_MTM_Off_tels.h"

#include <QWidget>
/**
 * \file officialtelephones_Form.h
 * \brief Форма служебных телефонов
*/
namespace Ui {
class OfficialTelephones;
}

class OfficialTelephones : public QWidget
{
    Q_OBJECT

public:
    explicit OfficialTelephones(QWidget *parent = nullptr);
    ~OfficialTelephones();

    MTM_Off_Tels *of_model = nullptr;

    QList<Off_tels*> *ofTlist = nullptr;

private:
    Ui::OfficialTelephones *ui;
private slots:
    void Fill_table();
    void on_pb_add_clicked();
    void on_pb_del_clicked();
    void on_pushButton_clicked();
};

#endif // OFFICIALTELEPHONES_FORM_H
