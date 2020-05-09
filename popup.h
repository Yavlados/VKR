#ifndef POPUP_H
#define POPUP_H

#include <QWidget>

#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QPropertyAnimation>
#include <QTimer>

enum menuType{
    leftMenu =0,
    rightMenu
};

namespace Ui {
class PopUp;
}

class PopUp : public QWidget
{
    Q_OBJECT
public:
    static PopUp *instance();
    void setPopupText(const QString& text, menuType menuT = rightMenu); // Установка текста в уведомление
    void show(menuType menuT);                            /* Собственный метод показа виджета
                                            * Необходимо для преварительной настройки анимации
                                            * */
    ~PopUp();

private:

    static PopUp *_instance;
    Ui::PopUp *ui;
    QLabel label;           // Label с сообщением
    QGridLayout layout;     // Размещение для лейбла
    QTimer *timer;          // Таймер, по которому виджет будет скрыт

private slots:
    void PopupHide();                            /* По окончании анимации, в данном слоте делается проверка,
                                             * виден ли виджет, или его необходимо скрыть
                                             * */

protected:
    explicit PopUp(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);    // Фон будет отрисовываться через метод перерисовки
};

#endif // POPUP_H
