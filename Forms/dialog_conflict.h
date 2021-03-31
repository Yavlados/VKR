#ifndef DIALOG_CONFLICT_H
#define DIALOG_CONFLICT_H

#include <QDialog>
#include <QString>

namespace Ui {
class Dialog_conflict;
}

class Dialog_conflict : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_conflict(QWidget *parent = 0);
    ~Dialog_conflict();

    int state = 4;
    void setText(QString str);
    void setButtonText(QString str);
private slots:
    void on_pb_save_link_clicked();

    void on_pb_merge_clicked();

    void on_pb_edit_tel_clicked();

    void on_pb_go_to_clicked();

    void on_pb_close_clicked();

private:
    Ui::Dialog_conflict *ui;
};

#endif // DIALOG_CONFLICT_H
