#include "mainwindow_Form.h"
#include <QTextCodec>
#include <QApplication>
/**
 * \file main.cpp
 * \brief Исполнительный файл запуска программы
 * \par Использует классы:
 *     @ref mainwindow_Form.h
*/

/**   \mainpage Программа для анализа записных книг
 *         \par  Состоит из следующих главных частей:
 *        - \ref Analysis
 *        - \ref Search
 *        - \ref Update
 *        - \ref Master_import_form
 *        - \ref Master_export_Form
 *        - \ref OfficialTelephones
 *          \author Житенев Владислав Германович
 *          \copyright BMSTU
 *          \version 3.0.1
 */


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec * tc = QTextCodec::codecForName("UTF-8");
    //QTextCodec::setCodecForTr(tc);
    QTextCodec::setCodecForLocale(tc);
    //QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());

    // Включение символа "." в качестве разделителя в вещественных числах
    //setlocale(LC_NUMERIC, "en_US.UTF-8");

    db_connection *con = db_connection::instance();
    //db_connection *r = db_connection::instance();
    if( !con->db_connect() )
    {
        qCritical()<<"Error"<<con->db().lastError();
        return -1;
    }


    MainWindow *w = new MainWindow();
    w->show();

    int rez =  a.exec();

    delete con;
    return rez;
}
