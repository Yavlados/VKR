#include "_Zk_links.h"

zk_links::zk_links()
{
    uuid1 = nullptr;
    uuid2 = nullptr;
    links = nullptr;
}

void zk_links::take_links(QList<int> *exported_id)
{
      if (links == nullptr)
          links = new QList<zk_links*>;
      QSqlQuery querry(db_connection::instance()->db());

       QString query = "SELECT DISTINCT row_id1, row_id2"
                     " From zk_links as c"
                     " RIGHT join (select row_id, zk_id from zk) as a"
                     " on a.row_id = c.row_id1"
                     " LEFT join (select row_id, zk_id from zk) as b"
                     " on b.row_id = c.row_id2"
                     " WHERE c.row_id1 IS NOT NULL AND "
                     " c.row_id2 IS NOT NULL AND ";
      for (int a=0; a< exported_id->size(); a++)
      {
          if(a == 0)
            query += " ( ";

         if(a != exported_id->size()-1)
            query += " a.zk_id = "+QString::number(exported_id->at(a))+" OR ";
         else
             query += " a.zk_id = "+QString::number(exported_id->at(a))+" ) ";
      }

      for (int a=0; a< exported_id->size(); a++)
      {
          if(a == 0)
            query += " AND (";

         if(a != exported_id->size()-1)
            query += "b.zk_id = "+QString::number(exported_id->at(a))+" OR ";
         else
             query += "b.zk_id = "+QString::number(exported_id->at(a))+")";
      }

      querry.prepare(query);

      if(!querry.exec())
      {
          qDebug() << " zk_links::take_links" << querry.lastError() << querry.executedQuery();
          return;
      }
      while (querry.next())
      {
          zk_links *New_link = new zk_links;
          New_link->uuid1 = querry.value(0).toString();
          New_link->uuid2 = querry.value(1).toString();
          links->append(New_link);
      }
}
