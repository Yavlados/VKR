#include "MyTableModel.h"

MyTableModel::MyTableModel(QObject *parent):
    QAbstractTableModel(parent)
{
    clist = nullptr;
    otlist = nullptr;
    state = Show;
    type = ContactMod; ///неважно какой тип модели в пустом конструкторе
}

MyTableModel::MyTableModel(QList<Contacts*> *contactList, QObject *parent):
    QAbstractTableModel(parent)
{
    state = Show;
    setContactList(contactList);
}

MyTableModel::MyTableModel(QList<Owners_tel*> *OTList, QObject *parent)
{
    state = Show;
    setOTList(OTList);
}

MyTableModel::MyTableModel(QList<Official_tel *> *ofTList, QObject *parent)
{
    state = Show;
    setofTList(ofTList);
}

/// Определение методов
int MyTableModel::columnCount(const QModelIndex & parent) const
{
    if( clist==nullptr && otlist==nullptr && oflist == nullptr)
        return 0;
    else {
        if(type == OTMod)             /// Owners_tel
          return 1;
        if(type == ContactMod)        /// Contacts
          return 2;
        if(type == OffTelMod)         /// Official_tel
          return 2;    /// То размер - размер списка контактов
    }
}

int MyTableModel::rowCount(const QModelIndex &parent) const
{
    if ( otlist == nullptr && clist==nullptr && oflist == nullptr)
        return 0;
    else
    {
        if(type == OTMod)              /// Owners_tel
          return actotlist.size();
        if(type == ContactMod)         /// Contacts
          return actlist.size();
        if(type == OffTelMod)
            return actoflist.size();    /// Official_tel
    }
}

void MyTableModel::setContactList(QList<Contacts *> *contactList)
{
    type = ContactMod;
    beginResetModel();

    clist = contactList;
    actlist.clear();

    if(clist!=nullptr)
    {
        for(int i=0; i < clist->size(); i++)
            if( clist->at(i)->state!=IsRemoved )
                actlist.append(clist->at(i));
    }

    endResetModel();
}

void MyTableModel::setContactList(QList<Contacts *> *contactList, int tel_id)
{
    type = ContactMod;
    beginResetModel();

    clist = contactList;
    actlist.clear();

    if(clist!=nullptr)
    {
        for(int i=0; i < clist->size(); i++)
            if( clist->at(i)->state!=IsRemoved && clist->at(i)->parent_OT_id == tel_id)
            {
                qDebug() << clist->at(i)->parent_OT_id;
                actlist.append(clist->at(i));
            }
    }

    endResetModel();
}

void MyTableModel::setOTList(QList<Owners_tel *> *OTList)
{
    type = OTMod;
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

void MyTableModel::setofTList(QList<Official_tel *> *OfList)
{
    type = OffTelMod;
    beginResetModel();

    oflist = OfList;
    actoflist.clear();

    if(oflist!=nullptr)
    {
        for(int i=0 ;i < oflist->size() ;i++)
            if( oflist->at(i)->state!=IsRemoved )
                actoflist.append(oflist->at(i));
    }
    endResetModel();
}

QVariant MyTableModel::data(const QModelIndex &index, int role) const
{
    if( !index.isValid() || (clist==nullptr && otlist==nullptr && oflist == nullptr))
        return QVariant();

    int row = index.row();      ///целочисленные указатели на строку
    int col = index.column();   /// и столбец

    if (type == ContactMod)
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
    else
    {
        if (type == OTMod)
        {
            if( row>actotlist.size() || row<0 )
                return QVariant();

            if (role == Qt::DisplayRole)
            {
                switch(col)
                {
                case 0:            /// 1 колонка - Номер телефона
                    return actotlist.at(row)->tel_num;
                }
            }
            return QVariant();
        }
        else
        {
            if (type == OffTelMod)
            {
                if( row>actoflist.size() || row<0 )
                    return QVariant();

                if (role == Qt::DisplayRole)
                {
                    switch(col)
                    {
                    case 0:            /// 1 колонка - Номер телефона
                        return actoflist.at(row)->tel_num;
                    case 1:
                        return actoflist.at(row)->service_name;
                    }
                }
                return QVariant();
            }
        }
    }
}

void MyTableModel::reset_OTModel()
{
    beginResetModel();
        actotlist.clear();
        endResetModel();
}

void MyTableModel::reset_ContactModel()
{
    beginResetModel();
    actlist.clear();
    endResetModel();
}

void MyTableModel::reset_OffTModel()
{
    beginResetModel();
    actoflist.clear();
    endResetModel();
}

QVariant MyTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
             return QVariant();
    int temp = type;
    switch (temp)
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
    case OTMod:     ///ТЕЛЕФОНЫ
        if (orientation == Qt::Horizontal)
            switch (section)
          {
             case 0:
                return QString("Номер телефона");
//            case 1:
//                return QString("Пометка");
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
}

Qt::ItemFlags MyTableModel::flags ( const QModelIndex & index ) const
{
    if( !index.isValid() || (clist==nullptr && otlist==nullptr && oflist == nullptr) )
        return Qt::NoItemFlags;

    if( state == Show )
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    if( state == Edit )
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

bool MyTableModel::setData ( const QModelIndex & index, const QVariant & value, int role )
{
    if( !index.isValid() || (clist==nullptr && otlist == nullptr) )
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
if (type == OTMod)
    {
    if( row>actotlist.size() || row<0 )
        return false;

    if (role == Qt::EditRole)
    {
        switch(col)
        {
        case 0:             /// 1 колонка - номер телефона
            actotlist.at(row)->tel_num = value.toString();
            if( actotlist.at(row)->state!=IsNewing )
                actotlist.at(row)->state = IsChanged;

            emit dataChanged(index,index);
            return true;
        }
    }
        return false;
    }
}

void MyTableModel::addRow_contact(int OTid)
{
    if (clist==nullptr )
        return;
    QSqlQuery *temp = new QSqlQuery(db_connection::instance()->db);
    temp->prepare("SELECT MAX( \"contacts\".\"Contact_list_id\")"
                 "FROM \"contacts\"");
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

void MyTableModel::addRow_owner_tel()
{
    if (otlist == nullptr)
         return;
     QSqlQuery *temp = new QSqlQuery(db_connection::instance()->db);
     temp->prepare("SELECT MAX( \"owners_tel\".\"Telephone_id\")"
                  "FROM \"owners_tel\"");
     if (!temp->exec())
     {
         qDebug() << temp->lastError();
         return ;
     }
     while (temp->next())
     {
         beginInsertRows(QModelIndex(),actotlist.size(),actotlist.size());

            Owners_tel *newc = new Owners_tel();
            if (actotlist.size() > 0 && actotlist.at(actotlist.size()-1)->tel_id > temp->value(0).toInt())
                   newc->tel_id = actotlist.at(actotlist.size()-1)->tel_id + 1;
            else
                   newc->tel_id = temp->value(0).toInt() +1;

            newc->parentZK_id = actotlist.at(actotlist.size()-1)->parentZK_id;
            newc->state = IsNewing;
            actotlist.append(newc);
            otlist->append(newc);

            endInsertRows();
        }
        delete temp;
   }

void MyTableModel::delRow_owner_tel(const QModelIndex &index)
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
           //delBindedContacts(ot->tel_id, contact_list);
       }
       endRemoveRows();
   }

void MyTableModel::delRow_contact(const QModelIndex &index)
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

void MyTableModel::delBindedContacts(int tel_id)
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
