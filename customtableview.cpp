#include "customtableview.h"
#include <QDebug>

CustomTableView::CustomTableView(QWidget *parent):
    QTableView ()
{

}

void CustomTableView::keyPressEvent(QKeyEvent *event)
{
    currentRow = this->currentIndex().row();
    switch (event->key())
    {
        case Qt::Key::Key_Down:
            newIndex = this->model()->index(currentRow+1,0);
            this->setCurrentIndex(newIndex);
            if(type == contactTable && this->currentIndex().row() != -1)
                emit clickOnContact(this->currentIndex());
        return;

        case Qt::Key::Key_Up:
            newIndex = this->model()->index(currentRow-1,0);
            this->setCurrentIndex(newIndex);
            if(type == contactTable && this->currentIndex().row() != -1)
                emit clickOnContact(this->currentIndex());
        return;

        case Qt::Key::Key_Enter:
            if(type == zkTable)
                emit openUpdateForm(this->currentIndex());
            else if (type == contactTable)
                emit getVar2Zk();
        return;

        case Qt::Key::Key_PageUp:
            if(type == zkTable)
             emit nextPage();
            return;

        case Qt::Key::Key_PageDown:
            if(type == zkTable)
             emit previousPage();
            return;

        case Qt::Key::Key_Right:
        if(this->currentIndex().row() != -1)
        {
            if(type == zkTable)
                emit getOt(this->currentIndex());
            else if(type == otTable)
                emit getCont(this->currentIndex());
        }
            return;

         case Qt::Key::Key_Left:
        if(this->currentIndex().row() != -1)
        {
        if(type == otTable)
                emit backOnZk();
        else if(type == contactTable)
                emit backOnOt();
        }
            return;
    }
}

void CustomTableView::setType(tableType t)
{
    type = t;
}
