#ifndef _MTM_OFF_TELS_H
#define _MTM_OFF_TELS_H
#include "_Off_tels.h"

#include <QAbstractTableModel>
#include <QList>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>


class MTM_Off_Tels: public QAbstractTableModel
{
     Q_OBJECT
public:

    MTM_Off_Tels(QObject *parent = nullptr);

    /// Переопределение кол-ва колонок модели
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

    /// Переопределение кол-ва строк модели
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

    /// Получение списка КОНТАКТОВ моделью
    virtual void setContactList(QList<Off_tels*> *offtlist);

    /// Получение списка СЛУЖЕБНЫХ ТЕЛЕФОНОВ моделью
   virtual void setOffTList(QList<Off_tels*> *offtlist);

    /// Заполнение модели данными из списка
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void reset_ContactModel();

    /// Переопределяем заголовки таблицам
    virtual QVariant headerData(int section, Qt::Orientation orientation,int role = Qt::DisplayRole) const;

    virtual Qt::ItemFlags flags ( const QModelIndex & index ) const;

    //virtual bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );

private:
    QList<Off_tels*> *offlist;    ///< исходный список
    QList<Off_tels*> actofflist;  ///<  отображаемый список

};

#endif // _MTM_OFF_TELS_H
