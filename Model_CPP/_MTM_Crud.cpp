#include "_MTM_Crud.h"


MTM_Crud::MTM_Crud(QObject *parent):
    QAbstractTableModel(parent)
{
    crudlist = nullptr  ;
    iterator = 0;
    QSettings settings( "C:/Users/Vladya/Documents/qt_projects/untitled9/CONFIG/TEST.ini", QSettings::IniFormat);
    settings.beginGroup("ZK_COLUMNS");                    //Подключаюсь к настройкам
    showing_count = settings.value("PAGE_COUNT").toInt();;//Кол-во отображаемых записей
}

int MTM_Crud::columnCount(const QModelIndex &parent) const
{
    QSettings settings( "C:/Users/Vladya/Documents/qt_projects/untitled9/CONFIG/TEST.ini", QSettings::IniFormat);
    settings.beginGroup("ZK_COLUMNS");
    (void)parent;
    if(crudlist==nullptr)
        return 0;
    else return settings.value("COLUMNS_COUNT").toInt();
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
        while (iterator < showing_count && iterator < crudlist->size())
        {
            if( crudlist->at(iterator)->state!=IsRemoved )
            {
                crudlist->at(iterator)->checkState_ = Unchecked;
                actcrudlist.append(crudlist->at(iterator));
            }
            iterator++;
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
               return Recieve_column(1,row);
           case 2:
               return Recieve_column(2,row);
           case 3:
               return Recieve_column(3,row);
           case 4:
               return Recieve_column(4,row);
           case 5:
               return Recieve_column(5,row);
           case 6:
               return Recieve_column(6,row);
           case 7:
               return Recieve_column(7,row);
           case 8:
               return Recieve_column(8,row);
           case 9:
               return Recieve_column(9,row);
           case 10:
               return Recieve_column(10,row);
           case 11:
               return Recieve_column(11,row);
           case 12:
               return Recieve_column(12,row);
           case 13:
               return Recieve_column(13,row);
           case 14:
               return Recieve_column(14,row);
           case 15:
               return Recieve_column(15,row);
           case 16:
               return Recieve_column(16,row);
           case 17:
               return Recieve_column(17,row);
           case 18:
               return Recieve_column(18,row);
           case 19:
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

    actcrudlist.clear();

    if(crudlist!=nullptr)
    {
        int local = iterator+showing_count;
        while (iterator < local && iterator < crudlist->size())
        {
            if( crudlist->at(iterator)->state!=IsRemoved )
            {
                crudlist->at(iterator)->checkState_ = Unchecked;
                actcrudlist.append(crudlist->at(iterator));
            }
            iterator++;
        }
    }

    endResetModel();
}

void MTM_Crud::previous_page_crud()
{
    beginResetModel();

    actcrudlist.clear();

    if(crudlist!=nullptr)
    {
        int local = iterator-showing_count;
        iterator -= 2*showing_count;
        while (iterator < local && iterator < crudlist->size())
        {
            if( crudlist->at(iterator)->state!=IsRemoved )
            {
                crudlist->at(iterator)->checkState_ = Unchecked;
                actcrudlist.append(crudlist->at(iterator));
            }
            iterator++;
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

QString MTM_Crud::Recieve_column(int column, int row) const
{
    QSettings settings( "C:/Users/Vladya/Documents/qt_projects/untitled9/CONFIG/TEST.ini", QSettings::IniFormat);
    settings.beginGroup("ZK_COLUMNS");
    QString settings_str = settings.value(QString::number(column)).toString();

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
}
