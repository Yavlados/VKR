#include "_MTM_OwTel.h"


MTM_OwTel::MTM_OwTel(QObject *parent):
    QAbstractTableModel(parent)
{
    otlist = nullptr;
    //state = Show_Ot;
    mark_rows.clear();
}

void MTM_OwTel::setOTList(QList<Owners_tel *> *OTList)
{
    beginResetModel();

    otlist = OTList;
    actotlist.clear();

    if(otlist!=nullptr)
    {
        for(int i=0 ;i < otlist->size() ;i++)
            if( otlist->at(i)->state!=IsRemoved )
                actotlist.append(otlist->at(i));
    }
    endResetModel();
}

int MTM_OwTel::columnCount(const QModelIndex &parent) const
{
    if(otlist==nullptr)
        return 0;
    else return 3;
}

int MTM_OwTel::rowCount(const QModelIndex &parent) const
{
    (void)parent;
    if ( otlist == nullptr)
        return 0;
    else                          /// Owners_tel
        return actotlist.size();
}

QVariant MTM_OwTel::data(const QModelIndex &index, int role) const
{
    if( !index.isValid() || otlist==nullptr)
        return QVariant();

    int row = index.row();      ///целочисленные указатели на строку
    int col = index.column();   /// и столбец

      if( row>actotlist.size() || row<0 )
          return QVariant();
      if (role == Qt::DisplayRole)
      {
          if(!actotlist.at(row)->tel_num.isEmpty() && actotlist.at(row)->oldnum == false && actotlist.at(row)->internum == false )//&& state == Show_Ot
          {
              QString _temp =  actotlist.at(row)->tel_num;

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
                  return actotlist.at(row)->tel_num;

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

      if (!mark_rows.isEmpty() && role == Qt::BackgroundRole)
      {
          for (int i = 0; i < mark_rows.size(); i++)
          {
              if(row == mark_rows.at(i))
                  return QVariant(QBrush(QColor(Qt::yellow)));
          }
      }
         return QVariant();
}

void MTM_OwTel::reset_OTModel()
{
    beginResetModel();
        actotlist.clear();
     endResetModel();
}

QVariant MTM_OwTel::headerData(int section, Qt::Orientation orientation, int role) const
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

Qt::ItemFlags MTM_OwTel::flags(const QModelIndex &index) const
{
    if( !index.isValid() ||  otlist==nullptr)
        return Qt::NoItemFlags;
    else
    //if( state == Show_Ot )
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;

}
/*
//bool MTM_OwTel::setData(const QModelIndex &index, const QVariant &value, int role)
//{
//    if( !index.isValid() ||  otlist == nullptr )
//        return false;

//    int row = index.row();      ///целочисленные указатели на строку
//    int col = index.column();   /// и столбец

//  if( row>actotlist.size() || row<0 )
//            return false;

//   if (role == Qt::EditRole)
//        {
//            switch(col)
//            {
//            case 2:             /// 1 колонка - номер телефона
//                actotlist.at(row)->tel_num = value.toString();
//                if( actotlist.at(row)->state!=IsNewing )
//                    actotlist.at(row)->state = IsChanged;

//                emit dataChanged(index,index);
//                return true;
//            }
//        }
//   if (role == Qt::CheckStateRole && col == 0)
//    {
//       ~Qt::ItemIsEditable;
//     if (actotlist.at(row)->internum == false && actotlist.at(row)->oldnum == false)
//          actotlist.at(row)->internum = true;
//     else
//         if(actotlist.at(row)->internum == true && actotlist.at(row)->oldnum == false)
//     {
//         actotlist.at(row)->internum = false;
//     }

//     if(actotlist.at(row)->internum == false && actotlist.at(row)->oldnum == true)
//     {
//         actotlist.at(row)->internum = true;
//         actotlist.at(row)->oldnum = false;
//     }

//     if( actotlist.at(row)->state!=IsNewing )
//         actotlist.at(row)->state = IsChanged;

//       emit dataChanged(index,index);
//         return true;
//    }
//   if (role == Qt::CheckStateRole && col == 1)
//    {
//     if (actotlist.at(row)->oldnum == false && actotlist.at(row)->internum == false)
//          actotlist.at(row)->oldnum = true;
//     else
//         if (actotlist.at(row)->oldnum == true && actotlist.at(row)->internum == false)
//         {
//             actotlist.at(row)->oldnum = false;
//         }
//     if (actotlist.at(row)->oldnum == false && actotlist.at(row)->internum == true)
//     {
//         actotlist.at(row)->oldnum = true;
//         actotlist.at(row)->internum = false;
//     }

//     if( actotlist.at(row)->state!=IsNewing )
//         actotlist.at(row)->state = IsChanged;
//       emit dataChanged(index,index);
//         return true;
//    }
//   return false;
//}
*/

QList<Owners_tel *> MTM_OwTel::recall_list()
{
    return *otlist;
}

void MTM_OwTel::addRow_owner_tel(Owners_tel *newc)
{
        if (otlist == nullptr)
             return;
        beginInsertRows(QModelIndex(),actotlist.size(),actotlist.size());

        actotlist.append(newc);
        otlist->append(newc);
        endInsertRows();

}

void MTM_OwTel::delRow_owner_tel(const QModelIndex &index)
{

    if (otlist==nullptr)
        return;

    beginRemoveRows(QModelIndex(),index.row(),index.row());

    Owners_tel *ot = actotlist.at(index.row());
    if( ot!=nullptr)
    {
        actotlist.removeAt(index.row());
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
