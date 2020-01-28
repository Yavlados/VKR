#ifndef SETTINGS_CONNECTION_H
#define SETTINGS_CONNECTION_H
#include <QString>
#include <QSettings>
#include <QCheckBox>
struct Columns      ///Структура для колонок
{
    int number;
    QString column_name;
};

struct Col_contents //последние настройки - подкачка
{
    bool zk_id = false;
    bool lastname  = false;
    bool name = false;
    bool mid_name = false;
    bool birth_date = false;
    bool check_for = false;
    bool dop_info = false;

    //Адрес регистрации
    bool reg_city = false;
    bool reg_street = false;
    bool reg_home = false;
    bool reg_corp = false;
    bool reg_flat = false;

    //Адрес проживания
    bool liv_city = false;
    bool liv_street = false;
    bool liv_home = false;
    bool liv_corp = false;
    bool liv_flat = false;

    //Дата и время сейчас
    bool date_add = false;
    bool time_add = false;

    // new
    bool nickname = false;
};

class Settings_connection         ///Класс настроек
{
public:
    QList<Columns*> showing_cols;

    Settings_connection();

    int showing_count = 50;
    int columns_count;
    Col_contents content;

    QString HostName,
            DatabaseName,
            User,
            Password;
     int    Port = 0;


    QSettings *settings = nullptr;

    QSettings *ret_settings();

    void    Set_settings();

    void up_checkboxes(QCheckBox *cb);
    void set_content(QString col_name);

    /// Доступ к статической переменной класса
    static Settings_connection *instance();

   private:
    /// Скрытая перемнная - единственный экземпляр класса
    static Settings_connection *_instance;

};

#endif // SETTINGS_CONNECTION_H
