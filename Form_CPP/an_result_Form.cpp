#include "an_result_Form.h"
#include "ui_an_result.h"
#include <QFileDialog>

An_result::An_result(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::An_result)
{
    ui->setupUi(this);
}

An_result::~An_result()
{
    delete ui;
}

void An_result::on_pb_back_clicked()
{
    delete an;
    delete this;
}

void An_result::closeEvent(QCloseEvent *event)
{
   (void)event;
   delete an;
   delete this;
}

void An_result::Recieve_short_face_analysis_all_db(int zk_id)
{
    an->short_face_analysis_all_db(zk_id);
    ui->textEdit->setText(an->analysis_res);
}

void An_result::Recieve_short_tel_analysis_all_db(int zk_id)
{
    For_analysis *an = new For_analysis();
    an->short_tel_analysis_all_db(zk_id);
    ui->textEdit->setText(an->analysis_res);
    delete an;
}

void An_result::Recieve_long_face_analysis_all_db(int zk_id)
{
    For_analysis *an = new For_analysis();
    an->long_face_analysis_all_db(zk_id);
    ui->textEdit->setText(an->analysis_res);
    delete an;
}

void An_result::Recieve_long_tel_analysis_all_db(int zk_id)
{
    For_analysis *an = new For_analysis();
    an->long_tel_analysis_all_db(zk_id);
    ui->textEdit->setText(an->analysis_res);
    delete an;
}

void An_result::Recieve_short_face_analysis_all_db(QVector<int> vector, int zk_id)
{
    For_analysis *an = new For_analysis();
    an->short_face_analysis_all_db(vector, zk_id);
    ui->textEdit->setText(an->analysis_res);
    delete an;
}

void An_result::Recieve_short_tel_analysis_all_db(QVector<int>vector, int zk_id)
{
    For_analysis *an = new For_analysis();
    an->short_tel_analysis_all_db(vector, zk_id);
    ui->textEdit->setText(an->analysis_res);
    delete an;
}

void An_result::Recieve_long_face_analysis_all_db(QVector<int> vector, int zk_id)
{
    For_analysis *an = new For_analysis();
    an->long_face_analysis_all_db(vector, zk_id);
    ui->textEdit->setText(an->analysis_res);
    delete an;
}

void An_result::Recieve_long_tel_analysis_all_db(QVector<int> vector, int zk_id)
{
    For_analysis *an = new For_analysis();
    an->long_tel_analysis_all_db(vector, zk_id);
    ui->textEdit->setText(an->analysis_res);
    delete an;
}

void An_result::Recieve_short_face_analysis_all_db(QString DateFrom, QString DateTo, int id)
{
    For_analysis *an = new For_analysis();
    an->short_face_analysis_all_db(DateFrom, DateTo, id);
    ui->textEdit->setText(an->analysis_res);
    delete an;
}

void An_result::Recieve_short_tel_analysis_all_db(QString DateFrom, QString DateTo, int id)
{
    For_analysis *an = new For_analysis();
    an->short_tel_analysis_all_db(DateFrom, DateTo, id);
    ui->textEdit->setText(an->analysis_res);
    delete an;
}

void An_result::Recieve_long_face_analysis_all_db(QString DateFrom, QString DateTo, int id)
{
    For_analysis *an = new For_analysis();
    an->long_face_analysis_all_db(DateFrom, DateTo, id);
    ui->textEdit->setText(an->analysis_res);
    delete an;
}

void An_result::Recieve_long_tel_analysis_all_db(QString DateFrom, QString DateTo, int id)
{
    For_analysis *an = new For_analysis();
    an->long_tel_analysis_all_db(DateFrom, DateTo, id);
    ui->textEdit->setText(an->analysis_res);
    delete an;
}

void An_result::Recieve_search_result(QString Search_res)
{
    An_result *an_res = new An_result;
    qDebug() << Search_res;
    an_res->ui->textEdit->setText(Search_res);
    an_res->show();
}

void An_result::on_pb_to_txt_clicked()
{
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
                   in <<  an->analysis_res;
                   file.close();
                  QMessageBox::information(this, "Information", "Текстовый файл сформирован и находится по следующему пути: \n"+filename); //выводим сообщение с числом
               }
           }
}

