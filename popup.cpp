#include "popup.h"
#include "ui_popup.h"
#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>

PopUp *PopUp::_instance = 0;


PopUp::PopUp(QWidget *parent) : QWidget(parent)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint |        // Отключаем оформление окна
                       Qt::Tool |                       // Отменяем показ в качестве отдельного окна
                       Qt::WindowStaysOnTopHint);       // Устанавливаем поверх всех окон
        setAttribute(Qt::WA_TranslucentBackground);     // Указываем, что фон будет прозрачным
        setAttribute(Qt::WA_ShowWithoutActivating);     // При показе, виджет не получается фокуса автоматически


        label.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter); // Устанавливаем по центру

        label.setStyleSheet("QLabel { color : white; "
                            "margin-top: 6px;"
                            "margin-bottom: 6px;"
                            "margin-left: 10px;"
                            "margin-right: 10px; }");

        // Производим установку текста в размещение, ...
        layout.addWidget(&label, 0, 0);
        setLayout(&layout); // которое помещаем в виджет

        // По сигналу таймера будет произведено скрытие уведомления, если оно видимо
        timer = new QTimer(this);

        connect( timer, SIGNAL( timeout() ), this, SLOT( PopupHide()) );
}

void PopUp::PopupHide()
{
    hide();
}

void PopUp::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    /* А теперь настраиваем фон уведомления,
     * который является прямоугольником с чёрным фоном
     * */
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); // Включаем сглаживание

    // Подготавливаем фон. rect() возвращает внутреннюю геометрию виджета уведомления, по содержимому
    QRect roundedRect;
    roundedRect.setX(rect().x() + 5);
    roundedRect.setY(rect().y() + 5);
    roundedRect.setWidth(rect().width() - 10);
    roundedRect.setHeight(rect().height() - 10);

    // Кисть настраиваем на чёрный цвет в режиме полупрозрачности 180 из 255
    painter.setBrush(QBrush(QColor(0,0,0,180)));
    painter.setPen(Qt::NoPen); // Край уведомления не будет выделен

    // Отрисовываем фон с закруглением краёв в 10px
    painter.drawRoundedRect(roundedRect, 10, 10);
}


void PopUp::setPopupText(const QString &text, menuType menuT)
{
    label.setText(text);    // Устанавливаем текст в Label
    adjustSize();           // С пересчётом размеров уведомления
    PopUp::show(menuT);
}

void PopUp::show(menuType menuT)
{
    int remTm = timer->remainingTime();
    if(remTm > 0) {
        timer->stop();
        PopupHide();
        return ;
    }

    if(menuT == rightMenu)
        setGeometry(QApplication::desktop()->availableGeometry().width() - 36 - width() + QApplication::desktop() -> availableGeometry().x(),
                    QApplication::desktop()->availableGeometry().height() - 36 - height() + QApplication::desktop() -> availableGeometry().y(),
                    width(),
                    height());
    else
        setGeometry(0 + 50 + QApplication::desktop() -> availableGeometry().x(),
                    QApplication::desktop()->availableGeometry().height() - 36 - height() + QApplication::desktop() -> availableGeometry().y(),
                    width(),
                    height());
    QWidget::show();                // Отображаем виджет, который полностью прозрачен
    timer->start(10000);             // А также стартуем таймер, который запустит скрытие уведомления через 3 секунды
}

PopUp *PopUp::instance()
{
    if( _instance == 0)
        _instance = new PopUp();

    return _instance;
}

PopUp::~PopUp()
{
    delete ui;
}
