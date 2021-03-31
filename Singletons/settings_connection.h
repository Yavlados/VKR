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
    bool zk_id;
    bool lastname;
    bool name;
    bool mid_name;
    bool birth_date;
    bool check_for;
    bool dop_info;

    //Адрес регистрации
    bool reg_city;
    bool reg_street;
    bool reg_home;
    bool reg_corp;
    bool reg_flat;

    //Адрес проживания
    bool liv_city;
    bool liv_street;
    bool liv_home;
    bool liv_corp;
    bool liv_flat;

    //Дата и время сейчас
    bool date_add;
    bool time_add;

    // new
    bool nickname;

    Col_contents();
};

class Settings_connection         ///Класс настроек
{
public:
    QList<Columns*> showing_cols;

    Settings_connection();

    int showing_count;
    int columns_count;
    Col_contents content;

    QString HostName,
            DatabaseName,
            User,
            Password;
     int    Port;


    QSettings *settings;

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
