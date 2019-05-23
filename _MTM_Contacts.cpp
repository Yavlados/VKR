#include "_MTM_Contacts.h"

MTM_Contacts::MTM_Contacts(QObject *parent):
    QAbstractTableModel(parent)
{
    clist = nullptr;
    state = Show_cont;
    type = ContactMod; ///неважно какой тип модели в пустом конструкторе
}


/// Определение методов
int MTM_Contacts::columnCount(const QModelIndex & parent) const
{
    if( clist==nullptr)
        return 0;
    else {
        if(type == ContactMod || type == OffTelMod)        /// Contacts
          return 2;
    }
}

int MTM_Contacts::rowCount(const QModelIndex &parent) const
{
    (void)parent;
    if (  clist==nullptr )
        return 0;
    else
    {
        if(type == ContactMod || type == OffTelMod) /// Contacts
          return actlist.size();
    }
}

/////////?????????????????????///////////////
void MTM_Contacts::setContactList(QList<Contacts *> *contactList)
{
    type = ContactMod;
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

void MTM_Contacts::setContactList(QList<Contacts *> *contactList, int tel_id)
{
    type = ContactMod;
    beginResetModel();

    clist = contactList;
    actlist.clear();

    if(clist!=nullptr)
    {
        for(int i=0; i < clist->size(); i++)
            if( clist->at(i)->cont_state!=IsRemoved && clist->at(i)->parent_OT_id == tel_id)
            {
                qDebug() << clist->at(i)->parent_OT_id;
                actlist.append(clist->at(i));
            }
    }

    endResetModel();
}

void MTM_Contacts::setOffTList(QList<Contacts *> *contactList)
{
    type = OffTelMod;
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

    if (type == ContactMod || type == OffTelMod)
    {
    if( row>actlist.size() || row<0 )
        return QVariant();

    if (role == Qt::DisplayRole)
        {
        switch(col)
            {
        case 0:            /// 1 колонка - номер телефона
           return actlist.at(row)->contact_tel_num;
        case 1:             /// 2 колонка - пометка к номеру
            return actlist.at(row)->mark;
            }
        }
      return QVariant();
    }
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

    switch (type)
    {
    case ContactMod:  ///Контакты
        if (orientation == Qt::Horizontal)
            switch (section)
          {
            case 0:
                return QString("Номер телефона");
            case 1:
                return QString("Пометка");
          }
        else {
            return QString("%1").arg(section+1);
        }
        break;
    case OffTelMod:  /// СЛУЖЕБНЫЕ ТЕЛЕФОНЫ
        if (orientation == Qt::Horizontal)
            switch (section)
          {
             case 0:
                return QString("Номер телефона");
            case 1:
                return QString("Наименование службы");
          }
        else {
            return QString("%1").arg(section+1);
        }
        break;
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
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

bool MTM_Contacts::setData ( const QModelIndex & index, const QVariant & value, int role )
{
    if( !index.isValid() || clist==nullptr )
        return false;

    int row = index.row();      ///целочисленные указатели на строку
    int col = index.column();   /// и столбец
if (type == ContactMod)
    {
        if( row>actlist.size() || row<0 )
            return false;

        if (role == Qt::EditRole)
        {
            switch(col)
            {
            case 0:             /// 1 колонка - номер телефона
                actlist.at(row)->contact_tel_num = value.toString();
                if( actlist.at(row)->cont_state!=IsNewing )
                    actlist.at(row)->cont_state = IsChanged;

                emit dataChanged(index,index);
                return true;

            case 1:             /// 2 колонка - пометка к номеру
                actlist.at(row)->mark = value.toString();
                if( actlist.at(row)->cont_state!=IsNewing )
                    actlist.at(row)->cont_state = IsChanged;

                emit dataChanged(index,index);
                return true;
            }
        }

        return false;
    }
}

void MTM_Contacts::addRow_contact(int OTid)
{
    if (clist==nullptr )
        return;
    QSqlQuery *temp = new QSqlQuery(db_connection::instance()->db());
    temp->prepare("SELECT MAX( contacts.Contact_list_id) "
                 " FROM contacts");
    if (!temp->exec())
    {
        qDebug() << temp->lastError();
        return ;
    }
    while (temp->next())
    {
        beginInsertRows(QModelIndex(),actlist.size(),actlist.size());

        Contacts *newc = new Contacts();

            if (actlist.size() > 0 && actlist.at(actlist.size()-1)->contact_id > temp->value(0).toInt())
                newc->contact_id = actlist.at(actlist.size()-1)->contact_id + 1;
            else
                newc->contact_id = temp->value(0).toInt() +1;

        newc->cont_state = IsNewing;
        newc->parent_OT_id = OTid;
        actlist.append(newc);
        clist->append(newc);
        newc->check();
        endInsertRows();
    }
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
