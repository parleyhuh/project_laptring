#include "dialog.h"
#include "ui_dialog.h"
#include <QDebug>
#include <QMessageBox>
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    status = "";
    ui->lcdNumber_2->display("-------");

    serial = new QSerialPort(this);
    secprocSerial = new comserial(serial);
    timer2 = new QTimer(this);
    timer = new QTimer(this);

    giay = 0;
    timer2->setInterval(1000);
    timer->setInterval(1000);

    timeset = "";
    connect(timer2,SIGNAL(timeout()), this, SLOT(myfunction()));
    connect(timer2,SIGNAL(timeout()),this, SLOT(timerr2));
    connect(timer, SIGNAL(timeout()), this, SLOT(timerr()));
    connect(timer, SIGNAL(timeout()), this, SLOT(prgbar()));

    timer2->start();
    ui->spinBox->setMinimum(0);
    ui->groupBox_waterpump->hide();
    ui->groupBox_LED->hide();
    }

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::timerr()
    {
    giay = giay + 1;
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
        ui->pushButton_set->setText("Bắt đầu");
    }
    ui->lcdNumber->value();
    timer->start();
//        ui->pushButton_set->setText("Tạm dừng");

}

void Dialog::on_pushButton_break_clicked()
{
    if(timer->isActive())
    {
        timer->stop();
        QMessageBox::warning(this,"Cảnh báo","Đã tạm dừng chương trình");
        ui->pushButton_set->setText("Continue");
    }
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


void Dialog::on_red_slider_valueChanged(int value)
{
    ui->red_label->setText(QString("<span style=\" font-size:12pt; font-weight:700; color:#f50105;\">%1</span>").arg(value));
    //Dialog::write(QString("r%1").arg(value));
    qDebug() << value;
}

void Dialog::on_blue_slider_valueChanged(int value)
{
    ui->blue_label->setText(QString("<span style=\" font-size:12pt; font-weight:700; color:#0000ff;\">%1</span>").arg(value));
   // Dialog::write(QString("b%1").arg(value));
    qDebug() << value;
}
QString Dialog::Read(){

    QString buf;
    while(serial->waitForReadyRead(20)){
        buf += serial->readAll();
    }
    return buf;
}

void Dialog::write(QString command)
{
    if(serial->isWritable()){
        serial->write(command.toStdString().c_str());
    }else{
        qDebug() << "Couldn't write to serial!";
    }
}

void Dialog::ReadData()
{
    QString data = Dialog::Read();
    qDebug() <<"Input: "<<data<<"\n";
    ui->lcdNumber_2->display(data);
    /* Xuất dữ liệu cảm biển */
}


void Dialog::on_radioButton_pump_clicked()
{
//    ui->groupBox_LED->hide();
    ui->groupBox_waterpump->show();
}


void Dialog::on_radioButton_led_clicked()
{
//    ui->groupBox_waterpump->hide();
    ui->groupBox_LED->show();
}


void Dialog::on_radioButton_hide_clicked()
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
void Dialog::myfunction()
{
    QDateTime time_text = QDateTime::currentDateTime();
    ui->label_time->setText(time_text.toString("dd/MM/yyyy    HH : mm : ss"));
}
void timerr2(){
    if( QTime::currentTime().toString("hh : mm : ss") == QString("13 : 47 : 00") ){
        //        qDebug() << QTime::currentTime().toString("hh : mm : ss");
    //QMessageBox::information(this,"Thông báo","Đã tắt đèn LED");
        QMessageBox::information(this,"Thông báo","Đã hết thời gian");
}
}
void Dialog::on_pushButton_timer_clicked()
{
    short int a,b,c;
    a = ui->spinBox_hour->value();
    b = ui->spinBox_minute->value();
    c = ui->spinBox_second->value();
    timeset = QString("%1 : %2 : %3").arg(a).arg(b).arg(c);
    status = status +  QString("Đã đặt giờ làm việc là : ") + timeset + "\n";
    ui->textEdit->setText(status);   
}


void Dialog::on_checkBox_pump_stateChanged(int arg1)
{
    arg1 = 1;
    qDebug() << arg1;
    return;
}


void Dialog::on_pushButton_ledoff_clicked()
{
    ui->red_slider->setSliderPosition(0);
    ui->blue_slider->setSliderPosition(0);
    Dialog::write(QString("b"));
}


void Dialog::on_pushButton_ledon_clicked()
{
    if(serial->isOpen()){
    int a= ui->red_slider->value();
    int b= ui->blue_slider->value();

    Dialog::write(QString("r%1").arg(a));
    Dialog::write(QString("b%1").arg(b));

    status = status + QString("Màu sắc hiển thị:\nĐỏ: %1\nXanh dương: %2").arg(a).arg(b) + " \n";
    ui->textEdit->setText(status);
    }
}


void Dialog::on_pushButton_pumpOn_clicked()
{
    if(serial->isOpen()){
        Dialog::write("n");
        return;
    }
    status += QString("Đã bật máy bơm \n ");
    ui->textEdit->setText(status);
}


void Dialog::on_pushButton_pumpOff_clicked()
{
    if(serial->isOpen()){
        Dialog::write("m");
        return;
    }
    status += QString("Đã tắt máy bơm \n");
    ui->textEdit->setText(status);
}
