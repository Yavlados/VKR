#ifndef COMPONENT_H
#define COMPONENT_H

#include <QDialog>
#include "_Telephone.h"
#include "_Contact.h"

enum component_type{    Ow_tel_num = 0,
                        Contact_num = 1,
                        new_ow_tel = 2,
                        new_cont_tel = 3};

struct component_content{
                        bool    Oldnum   ;
                        bool    Internum ;
                        QString tel_num  ;
                        QString mark     ;
                        component_content();
                        };

namespace Ui {
class Component;
}

class Component : public QDialog
{
    Q_OBJECT

public:
    explicit Component(QWidget *parent = 0);
    ~Component();

    component_content *content;

    QString str;
public slots:
    void set_focus();

    void set_type(component_type c_t);

    void set_index_data(Telephone *ow_t = 0, Contact *cnt = 0);

    void closeEvent(QCloseEvent *event);

    void change_layouts();

    void setFocus();
    void set_Tab_Order();

private slots:

    void on_le_1_textEdited(const QString &arg1);

    void on_le_2_textEdited(const QString &arg1);

    void on_le_3_textEdited(const QString &arg1);

    void on_le_4_textEdited(const QString &arg1);


    void on_cb_m_n_toggled(bool checked);

    void on_pb_cancel_clicked();

    void on_cb_oldnum_toggled(bool checked);

    void on_pb_ok_clicked();

    void on_le_1_inputRejected();

    void setTelNum();

private:
    Ui::Component *ui;

    component_type type;
};

#endif // COMPONENT_H
