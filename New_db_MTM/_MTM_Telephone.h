#ifndef MTM_TELEPHONE_H
#define MTM_TELEPHONE_H

#include <QAbstractTableModel>
#include "_Telephone.h"

class MTM_Telephone:  public QAbstractTableModel
{
    Q_OBJECT
public:
    MTM_Telephone(QObject *parent = 0);

    /// Получение списка ТЕЛЕФОНОВ моделью
    virtual void setOTList(QList<Telephone*> *list);

    /// Переопределение кол-ва колонок модели
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

    /// Переопределение кол-ва строк модели
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

    /// Заполнение модели данными из списка
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    /// Переопределяем заголовки таблицам
    virtual QVariant headerData(int section, Qt::Orientation orientation,int role = Qt::DisplayRole) const;

    QList<Telephone*> *otlist;    ///< исходный список
    QList<Telephone*> actotlist;  ///<  отображаемый список
public slots:
    void addRow_owner_tel(Telephone *newc);                       /// Удаление ячейки контакта
    void delRow_owner_tel(const QModelIndex &index);/// Удаление ячейки телефона
};

#endif // MTM_TELEPHONE_H
