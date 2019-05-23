#ifndef OFFICIALTELEPHONES_FORM_H
#define OFFICIALTELEPHONES_FORM_H

#include "_MTM_Contacts.h"

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

    MTM_Contacts *of_model = new MTM_Contacts;

    QList<Contacts*> *ofTlist = new QList<Contacts*>;

private:
    Ui::OfficialTelephones *ui;
private slots:
    void Fill_table();
};

#endif // OFFICIALTELEPHONES_FORM_H
