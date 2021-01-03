#include "_MTM_Telephone.h"

MTM_Telephone::MTM_Telephone(QObject *parent):
    QAbstractTableModel(parent)
{
    this->otlist = 0;
}

void MTM_Telephone::setOTList(QList<Telephone *> *list)
{
    beginResetModel();

    this->otlist = list;
    this->actotlist.clear();

    if(this->otlist!=0)
    {
        for(int i=0 ;i < this->otlist->size() ;i++)
            if( this->otlist->at(i)->state!=IsRemoved )
                this->actotlist.append(this->otlist->at(i));
    }
    endResetModel();
}

int MTM_Telephone::columnCount(const QModelIndex &parent) const
{
    if(this->otlist==0)
        return 0;
    else return 3;
}

int MTM_Telephone::rowCount(const QModelIndex &parent) const
{
    (void)parent;
    if ( this->otlist == 0)
        return 0;
    else                          /// Owners_tel
        return this->actotlist.size();
}

QVariant MTM_Telephone::data(const QModelIndex &index, int role) const
{
    if( !index.isValid() || otlist==0)
        return QVariant();

    int row = index.row();      ///целочисленные указатели на строку
    int col = index.column();   /// и столбец

      if( row>actotlist.size() || row<0 )
          return QVariant();
      if (role == Qt::DisplayRole)
      {
          if(!actotlist.at(row)->num.isEmpty() &&
                  actotlist.at(row)->oldnum == false &&
                  actotlist.at(row)->internum == false )//&& state == Show_Ot
          {
              QString _temp =  actotlist.at(row)->num;

              _temp.insert(0,"+");
              _temp.insert(2,"(");
              _temp.insert(6,")");
              _temp.insert(10,"-");
              _temp.insert(13,"-");
              switch(col)
              {
              case 2:            /// 1 колонка - Номер телефона
                  return _temp;
              }
          }
          else
          {
              switch(col)
              {
              case 2:            /// 1 колонка - Номер телефона
                  return actotlist.at(row)->num;

              }
          }

      }
      if (role == Qt::CheckStateRole && col == 0)  // this shows the checkbox
              {
                  bool aBool = actotlist.at(row)->internum;
                  if (aBool)
                          return Qt::Checked;
                  else
                          return Qt::Unchecked;
              }
      if (role == Qt::CheckStateRole && col == 1)  // this shows the checkbox
              {
                  bool aBool = actotlist.at(row)->oldnum;
                  if (aBool)
                          return Qt::Checked;
                  else
                          return Qt::Unchecked;
              }

//      if (!mark_rows.isEmpty() && role == Qt::BackgroundRole)
//      {
//          for (int i = 0; i < mark_rows.size(); i++)
//          {
//              if(row == mark_rows.at(i))
//                  return QVariant(QBrush(QColor(Qt::yellow)));
//          }
//      }
      return QVariant();
}

QVariant MTM_Telephone::headerData(int section, Qt::Orientation orientation, int role) const
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
          }
        else {
            return QVariant();
        }
}

void MTM_Telephone::addRow_owner_tel(Telephone *newc)
{
    if (this->otlist == 0)
         return;
    beginInsertRows(QModelIndex(),this->actotlist.size(),this->actotlist.size());

    this->actotlist.append(newc);
    this->otlist->append(newc);
    endInsertRows();
}

void MTM_Telephone::delRow_owner_tel(const QModelIndex &index)
{
    if (this->otlist==0)
        return;

    beginRemoveRows(QModelIndex(),index.row(),index.row());

    Telephone *ot = this->actotlist.at(index.row());
    if( ot!=0)
    {
        this->actotlist.removeAt(index.row());
        if( ot->state==IsNewing )
        {
            otlist->removeAll(ot);
            delete ot;
        }
        else
        {
            ot->state = IsRemoved;
        }
    }
    endRemoveRows();
}
