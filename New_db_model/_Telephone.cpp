#include "_Telephone.h"

Telephone::Telephone()
{
    this->num = "";
    this->alias = "";
    this->internum = 0;
    this->oldnum  = 0;
    this->_cont = 0;
}


QList<Contact *> *Telephone::cont()
{
    if (this->_cont == 0)
    {
        this->_cont = new QList<Contact*>;
    }
    return this->_cont;
}

bool Telephone::selectTelephone(QList<Telephone *> *list, QString personId)
{
    if(list==0)
        return false;

    qDeleteAll(*list);
    list->clear();

    if( !db_connection::instance()->db_connect() )
        return false;

    QSqlQuery temp(db_connection::instance()->db());
    temp.prepare("SELECT number,"
                 "id,"
                 "person_id,"
                 "oldnum,"
                 "internum "
                 "FROM notebook2.telephone "
                 "WHERE person_id = (:id)");
    temp.bindValue(":id", personId);
    if (!temp.exec())
    {
        qDebug() << "Owners_tel::selectZkTelForAdd" << temp.lastError();
        return false;
    }

    while (temp.next())
    {
        Telephone *ot = new Telephone();
        ot->num = temp.value(0).toString();
        ot->id = temp.value(1).toString();
        ot->person_id = temp.value(2).toString();
        ot->oldnum = temp.value(3).toBool();
        ot->internum = temp.value(4).toBool();
        ot->state = IsReaded;
        list->append(ot);
    }

    return true;
}
