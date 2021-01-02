#ifndef CUSTOMTABLEVIEW_H
#define CUSTOMTABLEVIEW_H

#include <QObject>
#include <QWidget>
#include <QTableView>
#include <QKeyEvent>

enum tableType{
    zkTable =0,
    otTable,
    contactTable,
    UpdateOT,
    UpdateCont
};

class CustomTableView : public QTableView
{
    Q_OBJECT
public:
    explicit CustomTableView(QWidget *parent = 0);
    void keyPressEvent(QKeyEvent *event) override;
    void setType(tableType t);
signals:
    void openUpdOT(QModelIndex index);
    void openUpdCont(QModelIndex index);

    void deleteTelephone();
    void addNewTelephone();
    void addNewCont();

    void f1Pressed();
    void openUpdateForm(QModelIndex index);
    void nextPage();
    void previousPage();
    void getOt(QModelIndex index);
    void getCont(QModelIndex index);
    void backOnZk();
    void backOnOt();
    void getVar2Zk();
    void clickOnContact(QModelIndex index);
    void stopFocus();

 private:
    QModelIndex newIndex;
    int currentRow;
    int currentColumn;
    tableType type;
};

#endif // CUSTOMTABLEVIEW_H
