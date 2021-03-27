#include "an_result_Form.h"
#include "ui_an_result.h"
#include <QTextCodec>

/// REFACTORED

An_result::An_result(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::An_result)
{
    ui->setupUi(this);
    an = new For_analysis();
}

An_result::~An_result()
{
    delete ui;
}

void An_result::on_pb_back_clicked()
{
    delete this;
}

void An_result::closeEvent(QCloseEvent *event)
{
   (void)event;
   delete this;
}

void An_result::Recieve_short_face_analysis(int zk_id)
{
    an->getDataFromBaseFace(zk_id);
    an->short_face_analysis(zk_id);
    FinalResult += an->analysis_res+"\r\n \r\n";
    an->ClearAll();
}

void An_result::Recieve_short_tel_analysis(int zk_id)
{
    an->getDataFromBaseTelephone(zk_id);
    an->short_tel_analysis(zk_id);
    FinalResult += an->analysis_res+"\r\n \r\n";
    an->ClearAll();
}

void An_result::Recieve_long_face_analysis(int zk_id)
{
    an->getDataFromBaseFullFace(zk_id);
    an->long_face_analysis(zk_id);
    FinalResult += an->analysis_res+"\r\n \r\n";
    an->ClearAll();
}

void An_result::Recieve_long_tel_analysis(int zk_id)
{
    an->getDataFromBaseFullTelephone(zk_id);
    an->long_tel_analysis(zk_id);
    FinalResult += an->analysis_res+"\r\n \r\n";
    an->ClearAll();
}

void An_result::Recieve_short_face_analysis(QVector<int>* vector, int zk_id)
{
    an->getDataFromBaseFace(zk_id, vector);
    an->short_face_analysis(zk_id);
    FinalResult += an->analysis_res+"\r\n \r\n";
    an->ClearAll();
}

void An_result::Recieve_short_tel_analysis(QVector<int>*vector, int zk_id)
{
    an->getDataFromBaseTelephone(zk_id, vector);
    an->short_tel_analysis(zk_id);
    FinalResult += an->analysis_res+"\r\n \r\n";
    an->ClearAll();
}

void An_result::Recieve_long_face_analysis(QVector<int> *vector, int zk_id)
{
    an->getDataFromBaseFullFace(zk_id, vector);
    an->long_face_analysis(zk_id);
    FinalResult += an->analysis_res+"\r\n \r\n";
    an->ClearAll();
}

void An_result::Recieve_long_tel_analysis(QVector<int> *vector, int zk_id)
{
    an->getDataFromBaseFullTelephone(zk_id, vector);
    an->long_tel_analysis(zk_id);
    FinalResult += an->analysis_res+"\r\n \r\n";
    an->ClearAll();
}

void An_result::Recieve_short_face_analysis(QMap<QString, QString> *period, int zk_id)
{
    an->getDataFromBaseFace(zk_id, 0, period);
    an->short_face_analysis(zk_id);
    FinalResult += an->analysis_res+"\r\n \r\n";
    an->ClearAll();
}

void An_result::Recieve_short_tel_analysis(QMap<QString, QString> *period, int zk_id)
{
    an->getDataFromBaseTelephone(zk_id, 0, period);
    an->short_tel_analysis(zk_id);
    FinalResult += an->analysis_res+"\r\n \r\n";
    an->ClearAll();
}

void An_result::Recieve_long_face_analysis(QMap<QString, QString> *period, int zk_id)
{
    an->getDataFromBaseFullFace(zk_id, 0, period);
    an->long_face_analysis(zk_id);
    FinalResult += an->analysis_res+"\r\n \r\n";
    an->ClearAll();
}

void An_result::Recieve_long_tel_analysis(QMap<QString, QString> *period, int zk_id)
{
    an->getDataFromBaseFullTelephone(zk_id, 0, period);
    an->long_tel_analysis(zk_id);
    FinalResult += an->analysis_res+"\r\n \r\n";
    an->ClearAll();
}

void An_result::Recieve_search_result(QString Search_res)
{
    An_result *an_res = new An_result;
    an_res->ui->textEdit->setText(Search_res);
    an_res->show();
}

void An_result::on_pb_to_txt_clicked()
{
    QString Text = ui->textEdit->toPlainText();
    QString filename = QFileDialog::getSaveFileName(
                       this,
                       tr("Save Document"),
                       QDir::currentPath(),
                       tr("Text File (*.txt)") );
           if( !filename.isNull() )
           {
               QFile file(filename);
               if(file.open(QIODevice::WriteOnly))
               {
                   QTextStream in(&file);
                   QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
                   if(codec != 0)
                     in.setCodec(codec);

                   in <<  Text;
                   file.close();
                  QMessageBox::information(this, "Information", "Текстовый файл сформирован и находится по следующему пути: \n"+filename); //выводим сообщение с числом
               }
           }
}

void An_result::import_report(QString text)
{
    An_result *an = new An_result();
    an->ui->textEdit->setText(text);
    an->show();
}

void An_result::Set_final_result()
{
    ui->textEdit->setText(FinalResult);
}

