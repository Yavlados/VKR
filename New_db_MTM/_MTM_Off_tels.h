#ifndef _MTM_OFF_TELS_H
#define _MTM_OFF_TELS_H
#include "_Off_tels.h"

#include <QAbstractTableModel>
#include <QList>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
/**
 * \file _MTM_Off_tels.h
 * \brief Модель отображения данных о служебных телефонах
*/
enum offt_m_state { Edit_mode,
                    Show_mode};

class MTM_Off_Tels: public QAbstractTableModel      ///Модель отображения данных о служебных телефонах
{
     Q_OBJECT
public:

    MTM_Off_Tels(QObject *parent = 0);

    /// Переопределение кол-ва колонок модели
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

    /// Переопределение кол-ва строк модели
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

    /// Получение списка СЛУЖЕБНЫХ ТЕЛЕФОНОВ моделью
   virtual void setOffTList(QList<Off_tels*> *offtlist);

    /// Заполнение модели данными из списка
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void reset_off_t_Model();

    void set_state();

    /// Переопределяем заголовки таблицам
    virtual QVariant headerData(int section, Qt::Orientation orientation,int role = Qt::DisplayRole) const;

    virtual Qt::ItemFlags flags ( const QModelIndex & index ) const;

    virtual bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );

    QList<Off_tels*> *offlist;    ///< исходный список
    QList<Off_tels*> actofflist;  ///<  отображаемый список

private:
    offt_m_state state ;
};

#endif // _MTM_OFF_TELS_H
