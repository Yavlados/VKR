#ifndef CONTACTSTABLEMOD_H
#define CONTACTSTABLEMOD_H
#include "_Contacts.h"

#include <QAbstractTableModel>
#include <QList>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
/**
 * \file _MTM_Contacts.h
 * \brief Модель отображения данных о контактах
*/
enum Calling_state_Model {Show_cont = 1,  /// Модель для просмотра
                        Edit_cont = 0}; /// Модель для изменения (формы update и add_form)


class MTM_Contacts: public QAbstractTableModel
{
     Q_OBJECT
public:
    Calling_state_Model state;

    /// Пустой конструктор модели
    MTM_Contacts(QObject *parent = nullptr);

    /// Переопределение кол-ва колонок модели
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

    /// Переопределение кол-ва строк модели
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

    /// Получение списка КОНТАКТОВ моделью
    virtual void setContactList(QList<Contacts*> *contactList);

    /// Заполнение модели данными из списка
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void reset_ContactModel();
    void reset_OffTModel();

    /// Переопределяем заголовки таблицам
    virtual QVariant headerData(int section, Qt::Orientation orientation,int role = Qt::DisplayRole) const;

    virtual Qt::ItemFlags flags ( const QModelIndex & index ) const;

    virtual bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );

    QList<Contacts*> *clist;    ///< исходный список
    QList<Contacts*> actlist;  ///<  отображаемый список
public slots:
    void addRow_contact(int);                      /// Добавление ячейки контакта
    void delRow_contact(const QModelIndex &index); /// Удаление ячейки контакта
    void delBindedContacts(int);                   /// Удаление связанных контактов при удалении ячейки телефонов


};

#endif // CONTACTSTABLEMOD_H
