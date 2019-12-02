#include "mainwindow_Form.h"
#include "dialog_settings.h"
#include <QTextCodec>
#include <QApplication>
#include <QtGlobal>
#include <QtGlobal>
#include <QVBoxLayout>
#include <QMessageBox>

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

void myMessageHandler(QtMsgType type, const QMessageLogContext &, const QString & msg)
{
    QString txt;
    switch (type) {
    case QtDebugMsg:
        txt = QString("Debug: %1").arg(msg) + "\n \r";
        break;
    case QtWarningMsg:
        txt = QString("Warning: %1").arg(msg)+ "\n \r";
    break;
    case QtCriticalMsg:
        txt = QString("Critical: %1").arg(msg)+ "\n \r";
    break;
    case QtFatalMsg:
        txt = QString("Fatal: %1").arg(msg)+ "\n \r";
    break;
    }
    QFile outFile("log");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt << endl;
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(myMessageHandler);
    QApplication a(argc, argv);
    QStringList paths = QCoreApplication::libraryPaths();
    paths.append(".");
    paths.append("imageformats");
    paths.append("platforms");
    paths.append("sqldrivers");
    QCoreApplication::setLibraryPaths(paths);

    QTextCodec * tc = QTextCodec::codecForName("UTF-8");
    //QTextCodec::setCodecForTr(tc);
    QTextCodec::setCodecForLocale(tc);
    //QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());

    // Включение символа "." в качестве разделителя в вещественных числах
    //setlocale(LC_NUMERIC, "en_US.UTF-8");



    Dialog_settings *set = new Dialog_settings;

    int s = set->exec();

    switch (s)
    {
    case QDialog::Rejected :
        return 0;
     case QDialog::Accepted :
        MainWindow *w = new MainWindow();
        w->show();
        int rez =  a.exec();
        return rez;
    }

}
