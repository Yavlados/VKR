#include "_MTM_Off_tels.h"

MTM_Off_Tels::MTM_Off_Tels(QObject *parent):
       QAbstractTableModel(parent)
{
    offlist = nullptr;
}

int MTM_Off_Tels::columnCount(const QModelIndex &parent) const
{
    if( offlist==nullptr)
        return 0;
    else
        return 2;
}

int MTM_Off_Tels::rowCount(const QModelIndex &parent) const
{
    (void)parent;
    if (  offlist==nullptr )
        return 0;
    else
        return actofflist.size();

}

void MTM_Off_Tels::setContactList(QList<Off_tels *> *offtlist)
{

}

void MTM_Off_Tels::setOffTList(QList<Off_tels *> *offtlist)
{
    beginResetModel();

    offlist = offtlist;
    actofflist.clear();

    if(offlist!=nullptr)
    {
        for(int i=0; i < offlist->size(); i++)
                actofflist.append(offlist->at(i));
    }

    endResetModel();
}

QVariant MTM_Off_Tels::data(const QModelIndex &index, int role) const
{
    if( !index.isValid() || offlist==nullptr)
        return QVariant();

    int row = index.row();      ///целочисленные указатели на строку
    int col = index.column();   /// и столбец

    if( row>actofflist.size() || row<0 )
        return QVariant();

    if (role == Qt::DisplayRole)
        {
        switch(col)
            {
        case 0:            /// 1 колонка - название службы
            return actofflist.at(row)->service_name;
        case 1:             /// 2 колонка - телефон
            return actofflist.at(row)->tel_num;
            }
        }
      return QVariant();
}

QVariant MTM_Off_Tels::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
            return QVariant();
 /// СЛУЖЕБНЫЕ ТЕЛЕФОНЫ
        if (orientation == Qt::Horizontal)
            switch (section)
          {
             case 0:
                return QString("Наименование службы");
            case 1:
                return QString("Номер телефона");
          }
        else {
            return QString("%1").arg(section+1);
        }
    return QVariant();

}

Qt::ItemFlags MTM_Off_Tels::flags(const QModelIndex &index) const
{
    if( !index.isValid() || offlist==nullptr)
        return Qt::NoItemFlags;
    else
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}
