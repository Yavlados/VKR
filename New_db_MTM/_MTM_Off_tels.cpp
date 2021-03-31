#include "_MTM_Off_tels.h"

MTM_Off_Tels::MTM_Off_Tels(QObject *parent):
       QAbstractTableModel(parent)
{
    offlist = 0;
}

int MTM_Off_Tels::columnCount(const QModelIndex &parent) const
{
    if( offlist==0)
        return 0;
    else
        return 2;
}

int MTM_Off_Tels::rowCount(const QModelIndex &parent) const
{
    (void)parent;
    if (  offlist==0 )
        return 0;
    else
        return actofflist.size();

}

void MTM_Off_Tels::setOffTList(QList<Off_tels *> *offtlist)
{
    beginResetModel();

    offlist = offtlist;
    actofflist.clear();

    if(offlist!=0)
    {
        for(int i=0; i < offlist->size(); i++)
                actofflist.append(offlist->at(i));
    }

    endResetModel();
}

QVariant MTM_Off_Tels::data(const QModelIndex &index, int role) const
{
    if( !index.isValid() || offlist==0)
        return QVariant();

    int row = index.row();      ///целочисленные указатели на строку
    int col = index.column();   /// и столбец

    if( row>actofflist.size() || row<0 )
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole )
        {
        switch(col)
            {
        case 1:            /// 1 колонка - название службы
            return actofflist.at(row)->service_name;
        case 0:             /// 2 колонка - телефон
            return actofflist.at(row)->tel_num;
            }
        }
    return QVariant();
}

void MTM_Off_Tels::reset_off_t_Model()
{
    beginResetModel();
        actofflist.clear();
        endResetModel();
}

void MTM_Off_Tels::set_state()
{
    state = Show_mode;
}

QVariant MTM_Off_Tels::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
            return QVariant();
 /// СЛУЖЕБНЫЕ ТЕЛЕФОНЫ
        if (orientation == Qt::Horizontal)
            switch (section)
          {
             case 1:
                return QString("Наименование службы");
            case 0:
                return QString("Номер телефона");
          }
        else {
            return QVariant();
        }
    return QVariant();

}

Qt::ItemFlags MTM_Off_Tels::flags(const QModelIndex &index) const
{
    if(!index.isValid() || offlist==0)
        return Qt::NoItemFlags;
    else if(state == Edit_mode)
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable ;
    else if(state == Show_mode)
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled ;
}

bool MTM_Off_Tels::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if( !index.isValid() || offlist==0 )
        return false;
    int row = index.row();      ///целочисленные указатели на строку
    int col = index.column();   /// и столбец

    if( row>actofflist.size() || row<0 )
        return false;

if(state == Edit_mode)
    if (role == Qt::EditRole)
    {
        switch(col)
        {
        case 0:             /// 1 колонка - номер телефона
            actofflist.at(row)->tel_num = value.toString();
            if( actofflist.at(row)->state!=Newing )
                actofflist.at(row)->state = Changed;

            emit dataChanged(index,index);
            return true;

        case 1:             /// 2 колонка - пометка к номеру
            actofflist.at(row)->service_name = value.toString();
            if( actofflist.at(row)->state!=Newing )
                actofflist.at(row)->state = Changed;

            emit dataChanged(index,index);
            return true;
        }
    }

}
