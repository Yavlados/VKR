#ifndef _MTM_CRUD_H
#define _MTM_CRUD_H
#include "_Crud.h"

#include <QAbstractTableModel>
#include <QList>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QCheckBox>
///////pod voprosom edita mojet i ne bit'//////


class MTM_Crud: public QAbstractTableModel
{
         Q_OBJECT
public:
    MTM_Crud(QObject *parent = nullptr);

    /// Переопределение кол-ва колонок модели
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

    /// Переопределение кол-ва строк модели
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

    /// Получение списка ЗК моделью
    virtual void setCrudlist(QList<Crud*> *crudl);

    /// Получение списка ЗК моделью
    virtual void setCheckedCrudlist(QList<Crud*> *crudl);

    /// Получение списка ЗК моделью
    virtual void setUnCheckedCrudlist(QList<Crud*> *crudl);

    /// Заполнение модели данными из списка
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    /// Переопределяем заголовки таблицам
    virtual QVariant headerData(int section, Qt::Orientation orientation,int role = Qt::DisplayRole) const;

    virtual Qt::ItemFlags flags ( const QModelIndex & index ) const;

    virtual bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );

    QList<Crud*> *crudlist;
    QList<Crud*> actcrudlist;

private:

public slots:

    ///не изменяю модель Круда

};

#endif // _MTM_CRUD_H
