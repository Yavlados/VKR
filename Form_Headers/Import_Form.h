#ifndef IMPORT_FORM_H
#define IMPORT_FORM_H
#include "_Off_tels.h"
#include "_MTM_Off_tels.h"
#include "text_handler.h"

#include <QWidget>
#include <QCloseEvent>
#include <QVector>
#include <QDialogButtonBox>


#include "olddbservice.h"
/**
 * @file Import_Form.h
 * @brief Форма импорта
*/

namespace Ui {
class Import_Form;
}

class Import_Form : public QWidget ///Форма импорта
{
    Q_OBJECT

public:

    explicit Import_Form(QWidget *parent = 0);
    ~Import_Form();


private:

    Ui::Import_Form *ui;

};

#endif // IMPORT_FORM_H
