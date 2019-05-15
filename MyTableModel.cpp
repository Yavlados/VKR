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

/// Определение методов
int MyTableModel::columnCount(const QModelIndex & parent) const
{
    if( clist==nullptr && otlist==nullptr)
        return 0;
    else {
        if(type == OTMod)              /// Если лсит контактов пустой
          return 1;  /// То размер - размер списка телефонов
        if(type == ContactMod)             /// Если лист телефонов пустой
          return 2;    /// То размер - размер списка контактов
    }
}

int MyTableModel::rowCount(const QModelIndex &parent) const
{
    if ( otlist == nullptr && clist==nullptr)
        return 0;
    else
    {
        if(type == OTMod)              /// Если лсит контактов пустой
          return actotlist.size();  /// То размер - размер списка телефонов
        if(type == ContactMod)             /// Если лист телефонов пустой
          return actlist.size();    /// То размер - размер списка контактов
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


QVariant MyTableModel::data(const QModelIndex &index, int role) const
{
    if( !index.isValid() || (clist==nullptr && otlist==nullptr))
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
            break;
        case 1:             /// 2 колонка - пометка к номеру
            return actlist.at(row)->mark;
            break;
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
                    break;
                }
            }
            return QVariant();
        }
    }
}

void MyTableModel::reset_model()
{
    beginResetModel();
        actotlist.clear();
        actlist.clear();
    endResetModel();
}

Qt::ItemFlags MyTableModel::flags ( const QModelIndex & index ) const
{
    if( !index.isValid() || (clist==nullptr && otlist==nullptr) )
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
        auto a = actlist.size();
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
