#ifndef _MTM_OWTEL_H
#define _MTM_OWTEL_H
#include "_Owners_tel.h"

#include <QAbstractTableModel>
#include <QList>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QCheckBox>
/**
 * \file _MTM_OwTel.h
 * \brief Модель отображения данных о номерах телефонов
*/
enum Calling_state_Ot{Show_Ot = 1,  /// Модель для просмотра
                      Edit_Ot = 0}; /// Модель для изменения (формы update и add_form)

class MTM_OwTel: public QAbstractTableModel     ///Модель отображения данных о номерах телефонов
{
    Q_OBJECT
public:
     Calling_state_Ot state;
     QList<int> mark_rows; //индексы строчек для подсвечивания
    MTM_OwTel(QObject *parent = nullptr);
    /// Получение списка ТЕЛЕФОНОВ моделью
    virtual void setOTList(QList<Owners_tel*> *OTList);

    /// Переопределение кол-ва колонок модели
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

    /// Переопределение кол-ва строк модели
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

    /// Заполнение модели данными из списка
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void reset_OTModel();

    /// Переопределяем заголовки таблицам
    virtual QVariant headerData(int section, Qt::Orientation orientation,int role = Qt::DisplayRole) const;

    virtual Qt::ItemFlags flags ( const QModelIndex & index ) const;

    virtual bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );

    ///TESTING
    virtual QList<Owners_tel*> recall_list();

    QList<Owners_tel*> *otlist;    ///< исходный список
    QList<Owners_tel*> actotlist;  ///<  отображаемый список
private:

public slots:
    void addRow_owner_tel();                       /// Удаление ячейки контакта
    void delRow_owner_tel(const QModelIndex &index);/// Удаление ячейки телефона
};

#endif // _MTM_OWTEL_H
