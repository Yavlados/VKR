#ifndef IMPORT_FORM_H
#define IMPORT_FORM_H
#include "_Crud.h"
#include "_MTM_Crud.h"

#include <QWidget>

namespace Ui {
class Import_Form;
}

class Import_Form : public QWidget
{
    Q_OBJECT

public:

    MTM_Crud *crud_model = new MTM_Crud;

    void recieve_all(QList <Crud*> *crudlist);
    explicit Import_Form(QWidget *parent = nullptr);
    ~Import_Form();

private slots:

private:
    Ui::Import_Form *ui;
};

#endif // IMPORT_FORM_H
