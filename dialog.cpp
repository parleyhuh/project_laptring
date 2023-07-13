#include "dialog.h"
#include "ui_dialog.h"
#include <QDebug>
#include <QMessageBox>
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    serial = new QSerialPort(this);
    secprocSerial = new comserial(serial);

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(myfunction()));
    giay = 0;


    timer->setInterval(1000);
    connect (timer, SIGNAL(timeout()), this, SLOT(timerr()));
    connect (timer, SIGNAL(timeout()), this, SLOT(prgbar()));
//    timer->start();
    ui->spinBox->setMinimum(0);

    }

void Dialog::timerr()
    {
    int a=ui->spinBox->value();
    //    int b=ui->spinBox_2->value();
    //    int c=ui->spinBox_3->value();
    giay = giay + 1;
//    if (giay == 60)
//    {
//        giay = 0;
//        phut = phut + 1;
//        if(phut==60)
//        {
//            phut = 0;
//            giay = 0;
//            gio = gio + 1;
//        }
//    }
    if(/*(giay==(a+1) && phut==b) && gio==c*/ giay/*+phut*60+gio*60*60*/==a+1)
    {
        timer->stop();
        QMessageBox::information(this,"Thông báo","Đã hết thời gian");
    }
    else
    {
        ui->lcdNumber->display(giay);
//        ui->lcdNumber_2->display(phut);
//        ui->lcdNumber_3->display(gio);
        //        qDebug()<<giay;
        //        qDebug()<<phut;
        //        qDebug()<<gio;
    }
    }



void Dialog::on_pushButton_reset_clicked()          //RESET
{
    giay=0;

    ui->lcdNumber->display(0);

    ui->progressBar->setValue(0);
    ui->spinBox->setValue(0);

    //    ui->label->setText(QString::number(giay)+"%");
    timer->stop();
    //    qApp->processEvents();

}
void Dialog::on_pushButton_set_clicked() //BATDAU
{
    if(timer->isActive())
    {
        timer->stop();
        QMessageBox::warning(this,"Cảnh báo","Đã tạm dừng chương trình");
        ui->pushButton_set->setText("Bắt đầu");
    }
    else
    {
        ui->lcdNumber->value();
        timer->start();
        ui->pushButton_set->setText("Tạm dừng");

    }
}


void Dialog::on_pushButton_break_clicked()
{

}

void Dialog::prgbar()
{
    int a=ui->spinBox->value();     //giây
    ui->progressBar->setMinimum(0);ui->progressBar->setMaximum(a);
    int q;
    q=ui->progressBar->value();
    q++;
    ui->progressBar->setValue(q);
}



Dialog::~Dialog()
{
    delete ui;
}

void Dialog::myfunction()

{
    QTime time_text = QTime::currentTime();
    ui->label_time->setText(time_text.toString());
}

void Dialog::on_red_slider_valueChanged(int value)
{
    ui->red_label->setText(QString("<span style=\" font-size:15pt; font-weight:600;\">%1</span>").arg(value));
    Dialog::updateRGB(QString("r%1").arg(value));
    ui->textEdit->setText(QString::number(value));
    qDebug() << value;
}

void Dialog::on_blue_slider_valueChanged(int value)
{
    ui->blue_label->setText(QString("<span style=\" font-size:15pt; font-weight:600;\">%1</span>").arg(value));
    Dialog::updateRGB(QString("b%1").arg(value));
    ui->textEdit->setText(QString::number(value));
    qDebug() << value;
}

void Dialog::updateRGB(QString command)
{
    if(serial->isWritable()){
        serial->write(command.toStdString().c_str());
    }else{
        qDebug() << "Couldn't write to serial!";
    }
}

void Dialog::write(char n){



}


