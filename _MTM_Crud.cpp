#include "_MTM_Crud.h"


MTM_Crud::MTM_Crud(QObject *parent):
    QAbstractTableModel(parent)
{
    crudlist = nullptr  ;
}

int MTM_Crud::columnCount(const QModelIndex &parent) const
{
    if(crudlist==nullptr)
        return 0;
    else return 20;
}

int MTM_Crud::rowCount(const QModelIndex &parent) const
{
    (void)parent;
    if (crudlist == nullptr)
        return 0;
    else return actcrudlist.size();
}

void MTM_Crud::setCrudlist(QList<Crud *> *crudl)
{
    beginResetModel();

    crudlist = crudl;
    actcrudlist.clear();

    if(crudlist!=nullptr)
    {
        for(int i=0; i < crudlist->size(); i++)
            if( crudlist->at(i)->state!=IsRemoved )
                actcrudlist.append(crudlist->at(i));
    }

    endResetModel();
}

void MTM_Crud::setOneCrud(Crud* crud_res)
{

    if(crud_res != nullptr)
    {
        beginResetModel();
        actcrudlist.clear();

        actcrudlist.append(crud_res);
    ///Почистил список и засунул только один указатель - рез. поиска
    endResetModel();
    }
    else
        return;
}

void MTM_Crud::setCheckedCrudlist(QList<Crud *> *crudl)
{
    beginResetModel();

    crudlist = crudl;
    actcrudlist.clear();

    if(crudlist!=nullptr)
    {
        for(int i=0; i < crudlist->size(); i++)
            if( crudlist->at(i)->state!=IsRemoved )
            {
                crudlist->at(i)->checkState_ = Checked;
                actcrudlist.append(crudlist->at(i));
            }
    }

    endResetModel();
}

void MTM_Crud::setUnCheckedCrudlist(QList<Crud *> *crudl)
{
    beginResetModel();

    crudlist = crudl;
    actcrudlist.clear();

    if(crudlist!=nullptr)
    {
        for(int i=0; i < crudlist->size(); i++)
            if( crudlist->at(i)->state!=IsRemoved )
            {
                crudlist->at(i)->checkState_ = Unchecked;
                actcrudlist.append(crudlist->at(i));
            }
    }

    endResetModel();
}

QVariant MTM_Crud::data(const QModelIndex &index, int role) const
{
    if( !index.isValid() || crudlist == nullptr)
        return QVariant();

    int row = index.row();      ///целочисленные указатели на строку
    int col = index.column();   /// и столбец

        if (role == Qt::DisplayRole)
        {
           switch(col)
          {///// dobav 1 column checkbox
           case 1:
              return actcrudlist.at(row)->zk_id;
           case 2:
              return actcrudlist.at(row)->lastname;
           case 3:
              return actcrudlist.at(row)->name;
           case 4:
              return actcrudlist.at(row)->mid_name;
           case 5:
              return actcrudlist.at(row)->birth_date;
           case 6:
              return actcrudlist.at(row)->reg_city;
           case 7:
              return actcrudlist.at(row)->reg_street;
           case 8:
              return actcrudlist.at(row)->reg_home;
           case 9:
              return actcrudlist.at(row)->reg_corp;
           case 10:
              return actcrudlist.at(row)->reg_flat;
           case 11:
              return actcrudlist.at(row)->liv_city;
           case 12:
              return actcrudlist.at(row)->liv_street;
           case 13:
              return actcrudlist.at(row)->liv_home;
           case 14:
              return actcrudlist.at(row)->liv_corp;
           case 15:
              return actcrudlist.at(row)->liv_flat;
           case 16:
              return actcrudlist.at(row)->check_for;
           case 17:
              return actcrudlist.at(row)->dop_info;
           case 18:
              return actcrudlist.at(row)->date_add;
           case 19:
              return actcrudlist.at(row)->time_add;

          }
        }
        if (role == Qt::CheckStateRole && col == 0)  // this shows the checkbox
                {
                    bool aBool = actcrudlist.at(row)->checkState_;
                    if (aBool)
                            return Qt::Checked;
                    else
                            return Qt::Unchecked;
                }
              return QVariant();
}


QVariant MTM_Crud::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole && role != Qt::CheckStateRole)
            return QVariant();
       ///ЗАПИСНЫЕ КНИГИ

   if (role == Qt::DisplayRole)
        if (orientation == Qt::Horizontal)
            switch (section)
          {
            case 1:
                return QString("Номер ЗК");
            case 2:
                return QString("Фамилия");
            case 3:
                return QString("Имя");
            case 4:
                return QString("Отчество");
            case 5:
                return QString("Дата рождения");
            case 6:
                return QString("Город регистрации");
            case 7:
                return QString("Улица регистрации");
            case 8:
                return QString("Дом регистрации");
            case 9:
                return QString("Корпус регистрации");
            case 10:
                return QString("Квартира регистрации");
            case 11:
                return QString("Город проживания");
            case 12:
                return QString("Улица проживания");
            case 13:
                return QString("Дом проживания");
            case 14:
                return QString("Корпус проживания");
            case 15:
                return QString("Квартира проживания");
            case 16:
                return QString("Проверяется в интересах");
            case 17:
                return QString("Дополнительная информация");
            case 18:
                return QString("День добавления");
            case 19:
                return QString("Время добавления");
          }

        return QVariant(); /// вот сюда внимание в случае краша
    }

Qt::ItemFlags MTM_Crud::flags(const QModelIndex &index) const
{
    if( !index.isValid() || crudlist==nullptr )
        return Qt::NoItemFlags;
    else
        return  Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
}

bool MTM_Crud::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if( !index.isValid() || crudlist == nullptr )
        return false;

    int row = index.row();      ///целочисленные указатели на строку
    int col = index.column();   /// и столбец

    if( row>actcrudlist.size() || row<0 )
            return false;

    if (role == Qt::CheckStateRole)
     {
      if (actcrudlist.at(row)->checkState_ == Unchecked)
           actcrudlist.at(row)->checkState_ = Checked;
       else
        if (actcrudlist.at(row)->checkState_ == Checked)
              actcrudlist.at(row)->checkState_= Unchecked;

        emit dataChanged(index,index);
          return true;
      }
         return false;

}
