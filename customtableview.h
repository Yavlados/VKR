#ifndef CUSTOMTABLEVIEW_H
#define CUSTOMTABLEVIEW_H

#include <QObject>
#include <QWidget>
#include <QTableView>
#include <QKeyEvent>

enum tableType{
    zkTable =0,
    otTable,
    contactTable
};

class CustomTableView : public QTableView
{
    Q_OBJECT
public:
    explicit CustomTableView(QWidget *parent = nullptr);
    void keyPressEvent(QKeyEvent *event) override;
    void setType(tableType t);
signals:
    void openUpdateForm(QModelIndex index);
    void nextPage();
    void previousPage();
    void getOt(QModelIndex index);
    void getCont(QModelIndex index);
    void backOnZk();
    void backOnOt();
    void getVar2Zk();
    void clickOnContact(QModelIndex index);

 private:
    QModelIndex newIndex;
    int currentRow;
    tableType type;
};

#endif // CUSTOMTABLEVIEW_H
