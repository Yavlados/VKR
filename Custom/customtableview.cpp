#include "customtableview.h"
#include <QDebug>

CustomTableView::CustomTableView(QWidget *parent):
    QTableView ()
{

}

void CustomTableView::keyPressEvent(QKeyEvent *event)
{
    currentRow = this->currentIndex().row();
    currentColumn = this->currentIndex().column();
    QAbstractItemModel* tableModel=  this->model();
    int rowsCount = tableModel->rowCount();
    int colsCount = tableModel->columnCount();

    switch (event->key())
    {
        case Qt::Key_F1:
            emit f1Pressed();
        return;

        case Qt::Key_X:
        if(currentColumn == colsCount-1)    return;
            newIndex = this->model()->index(currentRow, currentColumn+1);
            this->setCurrentIndex(newIndex);
        return;

        case Qt::Key_Z:
        if(currentColumn == 0)    return;
            newIndex = this->model()->index(currentRow, currentColumn-1);
            this->setCurrentIndex(newIndex);
        return;

        case Qt::Key_Down:
        if(currentRow == rowsCount-1) return;
            newIndex = this->model()->index(currentRow+1, currentColumn);
            this->setCurrentIndex(newIndex);
            if(type == contactTable && this->currentIndex().row() != -1)
                emit clickOnContact(this->currentIndex());
        return;

        case Qt::Key_Up:
        if(currentRow == 0)     return;
            newIndex = this->model()->index(currentRow-1, currentColumn);
            this->setCurrentIndex(newIndex);
            if(type == contactTable && this->currentIndex().row() != -1)
                emit clickOnContact(this->currentIndex());
        return;

        case Qt::Key_Enter:
            if(type == zkTable)
                emit openUpdateForm(this->currentIndex());
            else if (type == contactTable)
                emit getVar2Zk();
            else if(type == UpdateOT)
                emit openUpdOT(this->currentIndex());
            else if(type == UpdateCont)
                 emit openUpdCont(this->currentIndex());
        return;

        case Qt::Key_PageUp:
            if(type == zkTable)
             emit nextPage();
            return;

        case Qt::Key_PageDown:
            if(type == zkTable)
             emit previousPage();
            return;

        case Qt::Key_Right:
        if(this->currentIndex().row() != -1)
        {
            if(type == zkTable)
                emit getOt(this->currentIndex());
            else if(type == otTable || type == UpdateOT)
                emit getCont(this->currentIndex());
        }
            return;

         case Qt::Key_Left:
        if(this->currentIndex().row() != -1)
        {
        if(type == otTable)
                emit backOnZk();
        else if(type == contactTable || type == UpdateCont)
                emit backOnOt();
        }
            return;

         case Qt::Key_Escape:
            if(type == UpdateOT || type == UpdateCont)
                emit stopFocus();
        return;

        case Qt::Key_D:
        if(type== UpdateOT || type == UpdateCont)
            emit deleteTelephone();
        return;

        case Qt::Key_A:
        if(type== UpdateOT || type == UpdateCont)
            emit addNewTelephone();
        return;

        case Qt::Key_S:
        if(type== UpdateOT || type == UpdateCont)
            emit addNewCont();
        return;
    }
}

void CustomTableView::setType(tableType t)
{
    type = t;
}
