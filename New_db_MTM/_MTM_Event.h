#ifndef MTM_EVENT_H
#define MTM_EVENT_H
#include <QAbstractTableModel>
#include  "_Event.h"
class MTM_Event:  public QAbstractTableModel
{
    Q_OBJECT
public:
    MTM_Event(QObject *parent = 0);

    /// Получение списка записных книг моделью
    virtual void setEventList(QList<Event*> *eventList);

    /// Заполнение модели данными из списка
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    /// Переопределение заголовков таблицы
    virtual QVariant headerData(int section, Qt::Orientation orientation,int role = Qt::DisplayRole) const;

    /// Переопределение количества колонок модели
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

    /// Переопределение количества строк модели
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

    /// Установка флагов
    virtual Qt::ItemFlags flags ( const QModelIndex & index ) const;

    /// Метод изменения данных в таблице
    virtual bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );

    /// Список указателей для работы (является указателем)
    QList<Event*> *eventList;

    /// Список указателей для отображения (является переменной)
    QList<Event*> actEventList;

    int showing_count;
    int columnsCount;
};

#endif // MTM_EVENT_H
