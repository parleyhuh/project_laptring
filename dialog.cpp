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
    timer2 = new QTimer(this);
    connect(timer2,SIGNAL(timeout()),this,SLOT(myfunction()));


    giay = 0;
    timer = new QTimer(this);
    timer->setInterval(1000);
    connect (timer, SIGNAL(timeout()), this, SLOT(timerr()));
    connect (timer, SIGNAL(timeout()), this, SLOT(prgbar()));
    timer->start();
    timer2->start();
    ui->spinBox->setMinimum(0);

    ui->groupBox_waterpump->hide();
    ui->groupBox_LED->hide();

    }

void Dialog::timerr()
    {
    int a=ui->spinBox->value();

    if(giay==a+1)
    {
        timer->stop();
        QMessageBox::information(this,"Thông báo","Đã hết thời gian");
    }
    else
    {
        ui->lcdNumber->display(giay);
    }
    giay = giay + 1;
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
    ui->label_time->setText(time_text.toString("hh : mm : ss"));
}

void Dialog::on_red_slider_valueChanged(int value)
{
    ui->red_label->setText(QString("<span style=\" font-weight:700; color:#f50105;\">%1</span>").arg(value));
    Dialog::updateRGB(QString("r%1").arg(value));    
    qDebug() << value;
}

void Dialog::on_blue_slider_valueChanged(int value)
{
    ui->blue_label->setText(QString("<span style=\" font-weight:700; color:#0000ff;\">%1</span>").arg(value));
    Dialog::updateRGB(QString("b%1").arg(value));
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

void Dialog::on_radioButton_4_clicked()
{
    ui->groupBox_LED->hide();
    ui->groupBox_waterpump->show();
}


void Dialog::on_radioButton_5_clicked()
{
    ui->groupBox_waterpump->hide();
    ui->groupBox_LED->show();
}


void Dialog::on_radioButton_6_clicked()
{
    ui->groupBox_waterpump->hide();
    ui->groupBox_LED->hide();
}

void Dialog::on_pushButton_exit_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question (this,"Thông báo","Bạn có muốn thoát không?", QMessageBox::Yes|QMessageBox::No);
    if(reply == QMessageBox::Yes){
        qApp->closeAllWindows();
    }
    else{
        return;
    }

}


void Dialog::on_pushButton_clicked()
{
    int a= ui->red_slider->value();
    int b= ui->blue_slider->value();
    Dialog::updateRGB(QString("r%1").arg(a));
    Dialog::updateRGB(QString("b%1").arg(b));
    ui->textEdit->setText(QString("Màu sắc hiển thị:\nĐỏ: %1\nXanh dương: %2").arg(a).arg(b));
}

