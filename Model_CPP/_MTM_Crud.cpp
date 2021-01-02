#include "_MTM_Crud.h"


MTM_Crud::MTM_Crud(QObject *parent):
    QAbstractTableModel(parent)
{
    crudlist = 0  ;

    showing_count = Settings_connection::instance()->showing_count;//Кол-во отображаемых записей
}

int MTM_Crud::columnCount(const QModelIndex &parent) const
{
    (void)parent;
    if(crudlist==0)
        return 0;
    else return Settings_connection::instance()->columns_count;
}

int MTM_Crud::rowCount(const QModelIndex &parent) const
{
    (void)parent;
    if (crudlist == 0)
        return 0;
    else return actcrudlist.size();
}

void MTM_Crud::setCrudlist(QList<Crud *> *crudl)
{
    beginResetModel();

    crudlist = crudl;
    actcrudlist.clear();

    if(crudlist!=0)
    {
        for(int i=0;( i < showing_count) && (i < crudlist->size()); i++)
            if( crudlist->at(i)->state!=IsRemoved )
                actcrudlist.append(crudlist->at(i));
    }

    endResetModel();
}

void MTM_Crud::setOneCrud(Crud *crud_res)
{
     beginResetModel();
        actcrudlist.clear();

        actcrudlist.append(crud_res);
    /// Почистил список и засунул только один указатель - рез. поиска
    endResetModel();
        return;
}

void MTM_Crud::setCheckedCrudlist(QList<Crud *> *crudl)
{
    beginResetModel();

    crudlist = crudl;
    actcrudlist.clear();
    int iterator = 0;

    if(crudlist!=0)
    {
        for(int i=0; i < crudlist->size(); i++)
            if( crudlist->at(i)->state!=IsRemoved )
            {
                crudlist->at(i)->checkState_ = Checked;
                if (iterator < showing_count && iterator < crudlist->size())
                {
                    actcrudlist.append(crudlist->at(iterator));
                    iterator++;
                }
            }
    }

    endResetModel();
}

void MTM_Crud::setUnCheckedCrudlist(QList<Crud *> *crudl)
{
    beginResetModel();

    int iterator = 0;

    crudlist = crudl;
    actcrudlist.clear();

    if(crudlist!=0)
    {
        for(int i=0; i < crudlist->size(); i++)
            if( crudlist->at(i)->state!=IsRemoved )
            {
                crudlist->at(i)->checkState_ = Unchecked;
                if (iterator < showing_count && iterator < crudlist->size())
                {
                    actcrudlist.append(crudlist->at(iterator));
                    iterator++;
                }
            }
    }

    endResetModel();
}

QVariant MTM_Crud::data(const QModelIndex &index, int role) const
{
    if( !index.isValid() || crudlist == 0)
        return QVariant();

    int row = index.row();      ///целочисленные указатели на строку
    int col = index.column();   /// и столбец

    if (role == Qt::DisplayRole)
        {
           switch(col)
          {///// dobav 1 column checkbox
           case 1:
               return Recieve_column(0,row);
           case 2:
               return Recieve_column(1,row);
           case 3:
               return Recieve_column(2,row);
           case 4:
               return Recieve_column(3,row);
           case 5:
               return Recieve_column(4,row);
           case 6:
               return Recieve_column(5,row);
           case 7:
               return Recieve_column(6,row);
           case 8:
               return Recieve_column(7,row);
           case 9:
               return Recieve_column(8,row);
           case 10:
               return Recieve_column(9,row);
           case 11:
               return Recieve_column(10,row);
           case 12:
               return Recieve_column(11,row);
           case 13:
               return Recieve_column(12,row);
           case 14:
               return Recieve_column(13,row);
           case 15:
               return Recieve_column(14,row);
           case 16:
               return Recieve_column(15,row);
           case 17:
               return Recieve_column(16,row);
           case 18:
               return Recieve_column(17,row);
           case 19:
               return Recieve_column(18,row);
           case 20:
               return Recieve_column(19,row);
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

void MTM_Crud::reset_CrudModel()
{
    beginResetModel();
        actcrudlist.clear();
        endResetModel();
}

void MTM_Crud::next_page_crud()
{
    beginResetModel();
    int a;
    a = crudlist->indexOf(actcrudlist.at(actcrudlist.size()-1)); //индекс последнего элемента
    int b = a+1; //индекс добавления
    actcrudlist.clear();

    if(crudlist!=0)
    {
        while (b < a+showing_count+1 && b < crudlist->size())
        {
            if( crudlist->at(b)->state!=IsRemoved )
            {
                actcrudlist.append(crudlist->at(b));
                b++;
            }
        }
    }

    endResetModel();
}

void MTM_Crud::previous_page_crud()
{
    beginResetModel();
    int a;
    a = crudlist->indexOf(actcrudlist.at(0)); //индекс последнего элемента
    int b = a-showing_count; //индекс добавления

    actcrudlist.clear();

    if(crudlist!=0)
    {
        while (b < a)
        {
          if( crudlist->at(b)->state!=IsRemoved )
          {
              actcrudlist.append(crudlist->at(b));
              b++;
          }
        }
    }
    endResetModel();
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
            case 0:
                return "Все";
            case 1:
                return Recieve_column_name(0);
            case 2:
                return Recieve_column_name(1);
            case 3:
                return Recieve_column_name(2);
            case 4:
                return Recieve_column_name(3);
            case 5:
                return Recieve_column_name(4);
            case 6:
                return Recieve_column_name(5);
            case 7:
                return Recieve_column_name(6);
            case 8:
                return Recieve_column_name(7);
            case 9:
                return Recieve_column_name(8);
            case 10:
                return Recieve_column_name(9);
            case 11:
                return Recieve_column_name(10);
            case 12:
                return Recieve_column_name(11);
            case 13:
                return Recieve_column_name(12);
            case 14:
                return Recieve_column_name(13);
            case 15:
                return Recieve_column_name(14);
            case 16:
                return Recieve_column_name(15);
            case 17:
                return Recieve_column_name(16);
            case 18:
                return Recieve_column_name(17);
            case 19:
                return Recieve_column_name(18);
             case 20:
                return Recieve_column_name(19);
          }

        return QVariant(); /// вот сюда внимание в случае краша
    }

Qt::ItemFlags MTM_Crud::flags(const QModelIndex &index) const
{
    if( !index.isValid() || crudlist==0 )
        return Qt::NoItemFlags;
    else
        return  Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
}

bool MTM_Crud::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if( !index.isValid() || crudlist == 0 )
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

QString MTM_Crud::Recieve_column(int column, int row) const
{
    QString settings_str = Settings_connection::instance()->showing_cols.at(column)->column_name;

    if (settings_str == "ZK_ID")
        return QString::number(actcrudlist.at(row)->zk_id);

    if (settings_str == "LASTNAME")
        return actcrudlist.at(row)->lastname;

    if (settings_str == "NAME")
        return actcrudlist.at(row)->name;

    if (settings_str == "MID_NAME")
        return actcrudlist.at(row)->mid_name;

    if (settings_str == "BIRTH_DATE")
        return actcrudlist.at(row)->birth_date;

    if (settings_str == "CHECK_FOR")
        return actcrudlist.at(row)->check_for;

    if (settings_str == "DOP_INFO")
        return actcrudlist.at(row)->dop_info;

    if (settings_str == "LIV_CITY")
        return actcrudlist.at(row)->liv_city;

    if (settings_str == "LIV_STREET")
        return actcrudlist.at(row)->liv_street;

    if (settings_str == "LIV_HOME")
        return actcrudlist.at(row)->liv_home;

    if (settings_str == "LIV_CORP")
        return actcrudlist.at(row)->liv_corp;

    if (settings_str == "LIV_FLAT")
        return actcrudlist.at(row)->liv_flat;

    if (settings_str == "ADD_DATE")
        return actcrudlist.at(row)->date_add;

    if (settings_str == "REG_CITY")
        return actcrudlist.at(row)->reg_city;

    if (settings_str == "REG_STREET")
        return actcrudlist.at(row)->reg_street;

    if (settings_str == "REG_HOME")
        return actcrudlist.at(row)->reg_home;

    if (settings_str == "REG_CORP")
        return actcrudlist.at(row)->reg_corp;

    if (settings_str == "REG_FLAT")
        return actcrudlist.at(row)->reg_flat;

    if (settings_str == "ADD_TIME")
        return actcrudlist.at(row)->time_add;

    if (settings_str == "NICKNAME")
        return actcrudlist.at(row)->nickname;
}

QString MTM_Crud::Recieve_column_name(int column) const
{
    QString settings_str = Settings_connection::instance()->showing_cols.at(column)->column_name;

    if (settings_str == "ZK_ID")
        return QString("Номер ЗК");

    if (settings_str == "LASTNAME")
        return QString("Фамилия");

    if (settings_str == "NAME")
        return QString("Имя");

    if (settings_str == "MID_NAME")
       return QString("Отчество");

    if (settings_str == "BIRTH_DATE")
        return QString("Д/р");

    if (settings_str == "CHECK_FOR")
        return QString("П. в и.");

    if (settings_str == "DOP_INFO")
        return QString("Доп. инф.");

    if (settings_str == "LIV_CITY")
        return QString("Гор. прож.");

    if (settings_str == "LIV_STREET")
        return QString("Ул. прож.");

    if (settings_str == "LIV_HOME")
        return QString("Дом прож.");

    if (settings_str == "LIV_CORP")
        return QString("Корп. прож.");

    if (settings_str == "LIV_FLAT")
        return QString("Кв. прож.");

    if (settings_str == "ADD_DATE")
        return QString("День доб.");

    if (settings_str == "REG_CITY")
        return QString("Гор. рег.");

    if (settings_str == "REG_STREET")
        return QString("Ул. рег.");

    if (settings_str == "REG_HOME")
        return QString("Дом рег.");

    if (settings_str == "REG_CORP")
        return QString("Корп. рег.");

    if (settings_str == "REG_FLAT")
        return QString("Кв. рег.");

    if (settings_str == "ADD_TIME")
         return QString("Время доб.");

    if (settings_str == "NICKNAME")
         return QString("Кличка");
}
