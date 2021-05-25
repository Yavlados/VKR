#ifndef CUSTOMMSGBOX_H
#define CUSTOMMSGBOX_H
#include <QDialog>

enum customMsgButtons{
    linkPersons,
    abortSaving,
    doNotLink,
    mergeFirstIsParent,
    mergeSecondIsParent
};


namespace Ui {
class CustomMsgBox;
}

class CustomMsgBox : public QDialog
{
    Q_OBJECT

public:
    explicit CustomMsgBox(QWidget *parent = nullptr);
    ~CustomMsgBox();

    void setMessage(QString text);
    void setText(customMsgButtons type, QString text);
    void closeEvent(QCloseEvent *e);

private slots:
    void on_pb_createLink_clicked();

    void on_pb_abortSaving_clicked();

    void on_pb_dontLink_clicked();

    void on_pb_mergeFirst_clicked();

    void on_pb_mergeSecond_clicked();

private:
    Ui::CustomMsgBox *ui;
};

#endif // CUSTOMMSGBOX_H
