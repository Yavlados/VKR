#ifndef SETTINGS_CONNECTION_H
#define SETTINGS_CONNECTION_H
#include <QString>
#include <QSettings>

struct Columns      ///Структура для колонок
{
    int number;
    QString column_name;
};

class Settings_connection         ///Класс настроек
{
public:
    QList<Columns*> showing_cols;

    Settings_connection();

    int showing_count;
    int columns_count;

    QSettings *settings = nullptr;

    void    Set_settings();

    /// Доступ к статической переменной класса
    static Settings_connection *instance();

   private:
    /// Скрытая перемнная - единственный экземпляр класса
    static Settings_connection *_instance;

};

#endif // SETTINGS_CONNECTION_H
