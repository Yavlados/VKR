#ifndef OFFICIALTELEPHONES_FORM_H
#define OFFICIALTELEPHONES_FORM_H

#include "_MTM_Off_tels.h"

#include <QWidget>

namespace Ui {
class OfficialTelephones;
}

class OfficialTelephones : public QWidget
{
    Q_OBJECT

public:
    explicit OfficialTelephones(QWidget *parent = nullptr);
    ~OfficialTelephones();

    MTM_Off_Tels *of_model = new MTM_Off_Tels;

    QList<Off_tels*> *ofTlist = new QList<Off_tels*>;

private:
    Ui::OfficialTelephones *ui;
private slots:
    void Fill_table();
};

#endif // OFFICIALTELEPHONES_FORM_H
