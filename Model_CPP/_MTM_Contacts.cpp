#include "_MTM_Contacts.h"
#include <QPushButton>

void MTM_Contacts::up_flag()
{
    linked_flg = true;
}

MTM_Contacts::MTM_Contacts(QObject *parent):
    QAbstractTableModel(parent)
{
    clist = nullptr;
    state = Show_cont;
}

/// Определение методов
int MTM_Contacts::columnCount(const QModelIndex & parent) const
{
    if( clist==nullptr)
        return 0;
    else {
        if (state == Show_cont)
        {
            for (int i = 0; i < actlist.size(); i++)
            {
               if (actlist.at(i)->linked_id != 0)
                   return 5;
            }
            return 4;
        }
        if (state == Edit_cont)
          return 4;
    }
}

int MTM_Contacts::rowCount(const QModelIndex &parent) const
{
    (void)parent;
    if (  clist==nullptr )
        return 0;
    else
    {
          return actlist.size();
    }
}

/////////?????????????????????///////////////
void MTM_Contacts:: setContactList(QList<Contacts *> *contactList)
{
    beginResetModel();

    clist = contactList;
    actlist.clear();

    if(clist!=nullptr)
    {
        for(int i=0; i < clist->size(); i++)
            if( clist->at(i)->cont_state!=IsRemoved )
                actlist.append(clist->at(i));
    }

    endResetModel();
}

QVariant MTM_Contacts::data(const QModelIndex &index, int role) const
{
    if( !index.isValid() || clist==nullptr)
        return QVariant();

    int row = index.row();      ///целочисленные указатели на строку
    int col = index.column();   /// и столбец

    if( row>actlist.size() || row<0 )
        return QVariant();

    if (role == Qt::DisplayRole)
        {
        if(!actlist.at(row)->contact_tel_num.isEmpty() && actlist.at(row)->oldnum == false && actlist.at(row)->internum == false)
        {
            QString _temp =  actlist.at(row)->contact_tel_num;
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
                return actlist.at(row)->mark;
            case 4:
                if (actlist.at(row)->linked_id != 0)
                    return QString("Перейти к ЗК №")+ QString::number(actlist.at(row)->linked_id);
                else
                    return QVariant();
                }
        }
        else {
            switch(col)
                {
            case 2:            /// 1 колонка - номер телефона
               return actlist.at(row)->contact_tel_num;
            case 3:             /// 2 колонка - пометка к номеру
                return actlist.at(row)->mark;
            case 4:
                if (actlist.at(row)->linked_id != 0)
                    return QString("Перейти к ЗК №")+ QString::number(actlist.at(row)->linked_id);
                else
                    return QVariant();
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

    if (role == Qt::BackgroundRole)
    {
        for (int i = 0; i < actlist.size(); i++)
            if (actlist.at(i)->linked_id != 0 && row == i)
            {
                return QVariant(QBrush(QColor(Qt::yellow)));
            }
    }
      return QVariant();

}

void MTM_Contacts::reset_ContactModel()
{
    beginResetModel();
    actlist.clear();
    endResetModel();
}

QVariant MTM_Contacts::headerData(int section, Qt::Orientation orientation, int role) const
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
        else {
            return QString("%1").arg(section+1);
        }

    return QVariant();

}

Qt::ItemFlags MTM_Contacts::flags ( const QModelIndex & index ) const
{
    if( !index.isValid() || clist==nullptr)
        return Qt::NoItemFlags;

    if( state == Show_cont )
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    if( state == Edit_cont )
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsUserCheckable;
}

bool MTM_Contacts::setData ( const QModelIndex & index, const QVariant & value, int role )
{
    if( !index.isValid() || clist==nullptr )
        return false;

    int row = index.row();      ///целочисленные указатели на строку
    int col = index.column();   /// и столбец

        if( row>actlist.size() || row<0 )
            return false;

        if (role == Qt::EditRole)
        {
            switch(col)
            {
            case 2:             /// 1 колонка - номер телефона
                actlist.at(row)->contact_tel_num = value.toString();
                if( actlist.at(row)->cont_state!=IsNewing )
                    actlist.at(row)->cont_state = IsChanged;

                emit dataChanged(index,index);
                return true;

            case 3:             /// 2 колонка - пометка к номеру
                actlist.at(row)->mark = value.toString();
                if( actlist.at(row)->cont_state!=IsNewing )
                    actlist.at(row)->cont_state = IsChanged;

                emit dataChanged(index,index);
                return true;
            }
        }
        if (role == Qt::CheckStateRole && col == 0)
         {
            if (actlist.at(row)->internum == false && actlist.at(row)->oldnum == false)
                 actlist.at(row)->internum = true;
            else
                if(actlist.at(row)->internum == true && actlist.at(row)->oldnum == false)
            {
                actlist.at(row)->internum = false;
            }

            if(actlist.at(row)->internum == false && actlist.at(row)->oldnum == true)
            {
                actlist.at(row)->internum = true;
                actlist.at(row)->oldnum = false;
            }

            if( actlist.at(row)->cont_state!=IsNewing )
                actlist.at(row)->cont_state = IsChanged;

              emit dataChanged(index,index);
                return true;
         }
        if (role == Qt::CheckStateRole && col == 1)
         {
            if (actlist.at(row)->oldnum == false && actlist.at(row)->internum == false)
                actlist.at(row)->oldnum = true;
           else
               if (actlist.at(row)->oldnum == true && actlist.at(row)->internum == false)
               {
                   actlist.at(row)->oldnum = false;
               }
           if (actlist.at(row)->oldnum == false && actlist.at(row)->internum == true)
           {
               actlist.at(row)->oldnum = true;
               actlist.at(row)->internum = false;

           }

           if( actlist.at(row)->cont_state!=IsNewing )
               actlist.at(row)->cont_state = IsChanged;
             emit dataChanged(index,index);
               return true;
        }

        return false;
}

void MTM_Contacts::addRow_contact(Contacts *cnt)
{
    if (clist==nullptr )
        return;

    beginInsertRows(QModelIndex(),actlist.size(),actlist.size());

    actlist.append(cnt);
    clist->append(cnt);
    endInsertRows();
}

void MTM_Contacts::delRow_contact(const QModelIndex &index)
   {
       if (clist==nullptr)
           return;

       beginRemoveRows(QModelIndex(),index.row(),index.row());

       Contacts *cont = actlist.at(index.row());
       if( cont!=nullptr)
       {
           actlist.removeAt(index.row());

           if( cont->cont_state==IsNewing )
           {
               clist->removeAll(cont);
               delete cont;
           }
           else
           {
               cont->cont_state = IsRemoved;
           }
       }
       endRemoveRows();
   }

void MTM_Contacts::delBindedContacts(int tel_id)
{
       if (clist == nullptr)
          return;

       qDebug() << clist->size();
       QList<Contacts*> *temp_list = new  QList<Contacts*>;
       for (int i =0; i < clist->size(); i++)
       {
           qDebug() << clist->at(i)->contact_id;

           Contacts *cnt = clist->at(i);
           if (cnt->parent_OT_id == tel_id)
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
               Contacts *cnt = temp_list->at(i);
               clist->removeAll(cnt);
             }
                delete temp_list;
       }
}
