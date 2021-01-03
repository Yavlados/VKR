#include "_MTM_Contact.h"

MTM_Contact::MTM_Contact(QObject *parent):
    QAbstractTableModel(parent)
{
    clist = 0;
}

int MTM_Contact::columnCount(const QModelIndex &parent) const
{
    if( clist==0)
        return 0;
    else {
//        if (state == Show_cont)
//        {
//            return 4;
//        }
//        if (state == Edit_cont)
          return 4;
    }
}

int MTM_Contact::rowCount(const QModelIndex &parent) const
{
    (void)parent;
    if (  clist==0 )
        return 0;
    else
    {
          return actlist.size();
    }
}

void MTM_Contact::setContactList(QList<Contact *> *contactList)
{
    beginResetModel();

    clist = contactList;
    actlist.clear();

    if(clist!=0)
    {
        for(int i=0; i < clist->size(); i++)
            if( clist->at(i)->state!=IsRemoved )
                actlist.append(clist->at(i));
    }

    endResetModel();
}

QVariant MTM_Contact::data(const QModelIndex &index, int role) const
{
    if( !index.isValid() || clist==0)
        return QVariant();

    int row = index.row();      ///целочисленные указатели на строку
    int col = index.column();   /// и столбец

    if( row>actlist.size() || row<0 )
        return QVariant();

    if (role == Qt::DisplayRole)
        {
        if(!actlist.at(row)->number.isEmpty() && actlist.at(row)->oldnum == false && actlist.at(row)->internum == false)
        {
            QString _temp =  actlist.at(row)->number;
            _temp.insert(0,"+");
            _temp.insert(2,"(");
            _temp.insert(6,")");
            _temp.insert(10,"-");
            _temp.insert(13,"-");

            switch(col)
                {
            case 2:            /// 1 колонка - номер телефона
               return _temp;
            case 3:             /// 2 колонка - пометка к номеру
                return actlist.at(row)->alias;
                }
        }
        else {
            switch(col)
                {
            case 2:            /// 1 колонка - номер телефона
               return actlist.at(row)->number;
            case 3:             /// 2 колонка - пометка к номеру
                return actlist.at(row)->alias;
                }
            }
        }
    if (role == Qt::CheckStateRole && col == 0)  // this shows the checkbox
            {
                bool aBool = actlist.at(row)->internum;
                if (aBool)
                        return Qt::Checked;
                else
                        return Qt::Unchecked;
            }
    if (role == Qt::CheckStateRole && col == 1)  // this shows the checkbox
            {
                bool aBool = actlist.at(row)->oldnum;
                if (aBool)
                        return Qt::Checked;
                else
                        return Qt::Unchecked;
            }

//    if (role == Qt::BackgroundRole)
//    {
//        for (int i = 0; i < actlist.size(); i++)
//            if (actlist.at(i)->linked_id != 0 && row == i)
//            {
//                return QVariant(QBrush(QColor(Qt::yellow)));
//            }
//    }
      return QVariant();

}

QVariant MTM_Contact::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
            return QVariant();


        if (orientation == Qt::Horizontal)
            switch (section)
          {
            case 0:
                return QString("М.");
            case 1:
                return QString("С.");
            case 2:
                return QString("Телефон");
            case 3:
                return QString("Пометка");
          }

        return QVariant();
}

void MTM_Contact::reset_ContactModel()
{
    beginResetModel();
    actlist.clear();
    endResetModel();
}

void MTM_Contact::addRow_contact(Contact *cnt)
{
    if (clist==0 )
        return;

    beginInsertRows(QModelIndex(),actlist.size(),actlist.size());

    actlist.append(cnt);
    clist->append(cnt);
    endInsertRows();
}

void MTM_Contact::delRow_contact(const QModelIndex &index)
{
    if (clist==0)
        return;

    beginRemoveRows(QModelIndex(),index.row(),index.row());

    Contact *cont = actlist.at(index.row());
    if( cont!=0)
    {
        actlist.removeAt(index.row());

        if( cont->state==IsNewing )
        {
            clist->removeAll(cont);
            delete cont;
        }
        else
        {
            cont->state = IsRemoved;
        }
    }
    endRemoveRows();
}

void MTM_Contact::delBindedContacts(QString tel_id )
{
    if (clist == 0)
       return;
    QList<Contact*> *temp_list = new  QList<Contact*>;
    for (int i =0; i < clist->size(); i++)
    {
        Contact *cnt = clist->at(i);
        if (cnt->telephone_id == tel_id)
        {
            temp_list->append(cnt); ///выношу во временный список
           delete cnt;
        }
    }
    if(temp_list->isEmpty())
    {
        return;
    }
    else
    {
      for (int i =0;  i < temp_list->size(); i++)
          {
            Contact *cnt = temp_list->at(i);
            clist->removeAll(cnt);
          }
             delete temp_list;
    }
}
