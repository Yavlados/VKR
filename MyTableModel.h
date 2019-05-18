#ifndef CONTACTSTABLEMOD_H
#define CONTACTSTABLEMOD_H

#include "_Contacts.h"
#include "_Owners_tel.h"
#include "_Official_tel.h"

#include <QAbstractTableModel>
#include <QList>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>

enum Calling_state {Show = 1,  /// Модель для просмотра
                    Edit = 0}; /// Модель для изменения (формы update и add_form)

enum Model_type {OffTelMod = -1,
                 ContactMod = 1,
                 OTMod = 0 };

const int COLS= 3; /// кол-во отображаемых колонк в модели

class MyTableModel: public QAbstractTableModel
{
     Q_OBJECT
public:
    Calling_state state;
    Model_type type;
    /// Пустой конструктор модели
    MyTableModel(QObject *parent = nullptr);

    /// Конструктор модели со списком объектов класса Contacts
    MyTableModel(QList<Contacts*> *contactList, QObject *parent = nullptr);

    /// Конструктор модели со списком объектов класса Owners_tel
    MyTableModel(QList<Owners_tel*> *OTList, QObject *parent = nullptr);

    /// Конструктор модели со списком объектов класса Owners_tel
    MyTableModel(QList<Official_tel*> *ofTList, QObject *parent = nullptr);

    /// Переопределение кол-ва колонок модели
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

    /// Переопределение кол-ва строк модели
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

    /// Получение списка КОНТАКТОВ моделью
    virtual void setContactList(QList<Contacts*> *contactList);

    /// Получения списка моделью для формы Добавления и редактирования
     virtual void setContactList(QList<Contacts*> *contactList, int);

    /// Получение списка ТЕЛЕФОНОВ моделью
    virtual void setOTList(QList<Owners_tel*> *OTtList);

    /// Получение списка СЛУЖЕБНЫХ ТЕЛЕФОНОВ моделью
    virtual void setofTList(QList<Official_tel*> *OfList);

    /// Заполнение модели данными из списка
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void reset_OTModel();
    void reset_ContactModel();
    void reset_OffTModel();

    /// Переопределяем заголовки таблицам
    virtual QVariant headerData(int section, Qt::Orientation orientation,
                                    int role = Qt::DisplayRole) const;

    virtual Qt::ItemFlags flags ( const QModelIndex & index ) const;

    virtual bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );

private:
    QList<Contacts*> *clist;    ///< исходный список
    QList<Contacts*> actlist;  ///<  отображаемый список

    QList<Owners_tel*> *otlist;    ///< исходный список
    QList<Owners_tel*> actotlist;  ///<  отображаемый список

    QList<Official_tel*> *oflist;    ///< исходный список
    QList<Official_tel*> actoflist;  ///<  отображаемый список

public slots:
    void addRow_contact(int);                      /// Добавление ячейки контакта
    void delRow_contact(const QModelIndex &index); /// Удаление ячейки контакта
    void delBindedContacts(int);                   /// Удаление связанных контактов при удалении ячейки телефонов
    void addRow_owner_tel();                       /// Удаление ячейки контакта
    void delRow_owner_tel(const QModelIndex &index);/// Удаление ячейки телефона

};

#endif // CONTACTSTABLEMOD_H
