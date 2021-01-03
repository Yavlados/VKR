#ifndef MTM_CONTACT_H
#define MTM_CONTACT_H
#include <QAbstractTableModel>
#include "_Contact.h"

class MTM_Contact:  public QAbstractTableModel
{
    Q_OBJECT
public:
    MTM_Contact(QObject *parent = 0);

    /// Переопределение кол-ва колонок модели
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

    /// Переопределение кол-ва строк модели
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

    /// Получение списка КОНТАКТОВ моделью
    virtual void setContactList(QList<Contact*> *contactList);

    /// Заполнение модели данными из списка
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    /// Переопределяем заголовки таблицам
    virtual QVariant headerData(int section, Qt::Orientation orientation,int role = Qt::DisplayRole) const;

    void reset_ContactModel();

    QList<Contact*> *clist;    ///< исходный список
    QList<Contact*> actlist;  ///<  отображаемый список
public slots:
    void addRow_contact(Contact *cnt);                      /// Добавление ячейки контакта
    void delRow_contact(const QModelIndex &index); /// Удаление ячейки контакта
    void delBindedContacts(QString);                   /// Удаление связанных контактов при удалении ячейки телефонов
};

#endif // MTM_CONTACT_H
