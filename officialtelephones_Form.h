#ifndef OFFICIALTELEPHONES_FORM_H
#define OFFICIALTELEPHONES_FORM_H

#include "MyTableModel.h"

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

    MyTableModel *of_model = new MyTableModel;

    QList<Official_tel*> *ofTlist = new QList<Official_tel*>;

private:
    Ui::OfficialTelephones *ui;
private slots:
    void Fill_table();
};

#endif // OFFICIALTELEPHONES_FORM_H
