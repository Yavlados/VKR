#include "olddbservice.h"
#include <QDebug>

OldDbService::OldDbService()
{

}

OldDbZk::OldDbZk()
{

}

OldDbZk::~OldDbZk()
{
    qDeleteAll(contacts);
    contacts.clear();
}


bool OldDbService::Open_Old_db(OldDbZk *zk, int pos, QByteArray arr)
{
    if( readAbonentInfo(&arr,&pos,&(zk->person)) )
    {
        // qDebug()<<zk->person.toString();
        zk->category = readLexem(&arr,&pos);
        zk->data_zad = readLexem(&arr,&pos);
        zk->povod_zad = readLexem(&arr,&pos);
        zk->kem_zad = readLexem(&arr,&pos);
        zk->keep_place = readLexem(&arr,&pos);

        int pos1 = arr.indexOf(char(0x01),pos);
        if( pos1>0 && arr[pos1 + 1]==char(0x00) )
        {
            pos = pos1 + 2;
            pos1 = arr.indexOf(char(0x01),pos);
            while( pos1>0 && arr[pos1 + 1]!=char(0x00) )
            {
                pos = pos1 + 0x2B;
                OldDbAbonent *contact = new OldDbAbonent();
                if( readAbonentInfo(&arr,&pos,contact) )
                    zk->contacts.append(contact);
                else
                    delete contact;
                pos1 = arr.indexOf(char(0x01),pos);
              //  qDebug()<<contact->toString();
            }
        }
        return true;
    }
}

bool OldDbService::readAbonentInfo(QByteArray *arr, int *pos, OldDbAbonent *abonent)
{
    abonent->famil = readLexem(arr,pos);
    abonent->imya = readLexem(arr,pos);
    abonent->otchestvo = readLexem(arr,pos);
    abonent->klichka = readLexem(arr,pos);
    abonent->city = readLexem(arr,pos);
    abonent->street = readLexem(arr,pos);
    abonent->dom = readLexem(arr,pos);
    abonent->korpus = readLexem(arr,pos);
    abonent->kvar = readLexem(arr,pos);
    abonent->tel1 = readLexem(arr,pos);
    abonent->tel2 = readLexem(arr,pos);
    abonent->tel3 = readLexem(arr,pos);
    abonent->tel4 = readLexem(arr,pos);
    abonent->tel5 = readLexem(arr,pos);
    abonent->dopinfo = readLexem(arr,pos);
    return true;
}

QString OldDbService::readLexem(QByteArray *arr, int *pos)
{
    int nend = arr->indexOf(char(0x00),*pos);
    if( nend > *pos )
    {
        QTextCodec *tc = QTextCodec::codecForName("IBM 866");
        QByteArray str = arr->mid(*pos,nend - *pos) ;
        QString new_str = tc->toUnicode(str);
        *pos = nend+1;
        return new_str;
    }
    else
    {
        (*pos)++;
       // qDebug()<<"-"<<*pos;
        return QString();
    }
}
