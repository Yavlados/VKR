#ifndef _MTM_CRUD_H
#define _MTM_CRUD_H
#include "_Crud.h"
#include "settings_connection.h"

#include <QAbstractTableModel>
#include <QList>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QCheckBox>


/**
 * \file _MTM_Crud.h
 * \brief Модель отображения данных о ЗК
*/

class MTM_Crud: public QAbstractTableModel      ///Модель отображения данных о ЗК
{
         Q_OBJECT
public:
    MTM_Crud(QObject *parent = 0);

    /// Переопределение количества колонок модели
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

    /// Переопределение количества строк модели
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

    /// Получение списка записных книг моделью
    virtual void setCrudlist(QList<Crud*> *crudl);

    /// Получение одной записной книги моделью (Быстрый поиск)
    virtual void setOneCrud(Crud* crud_res);

    /// Получение списка записных книг моделью
    virtual void setCheckedCrudlist(QList<Crud*> *crudl);

    /// Получение списка ЗК моделью
    virtual void setUnCheckedCrudlist(QList<Crud*> *crudl);

    /// Заполнение модели данными из списка
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    /// Сброс отображаемого списка модели
    void reset_CrudModel();

    /// Загрузка записных книг следующей страницы
    virtual void next_page_crud();

    /// Загрузка записных книг предыдущей страницы
    virtual void previous_page_crud();

    /// Переопределение заголовков таблицы
    virtual QVariant headerData(int section, Qt::Orientation orientation,int role = Qt::DisplayRole) const;

    /// Установка флагов
    virtual Qt::ItemFlags flags ( const QModelIndex & index ) const;

    /// Метод изменения данных в таблице
    virtual bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );

    /// Метод получения данных для отоброжения таблицы (изменение настроек)
    virtual QString Recieve_column(int column, int row) const;

    /// Метод получения заголовков для отоброжения таблицы (изменение настроек)
    virtual QString Recieve_column_name(int column) const;

    //QSettings settings;
    /// Список указателей для работы (является указателем)
    QList<Crud*> *crudlist;

    /// Список указателей для отображения (является переменной)
    QList<Crud*> actcrudlist;
    int showing_count;
private:

public slots:

    ///не изменяю модель Круда

};

#endif // _MTM_CRUD_H
