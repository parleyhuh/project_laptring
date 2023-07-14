#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QTimer>
#include <QDateTime>
#include "comserial.h"


namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

public slots:
 void myfunction();

private slots:
  //  void ReadData();
    void on_red_slider_valueChanged(int value);

    void on_blue_slider_valueChanged(int value);

    void updateRGB(QString command);

    void on_pushButton_set_clicked();

    void on_pushButton_break_clicked();

    void on_pushButton_reset_clicked();

    void timerr();

    void prgbar();

    void write(char n);

private:
    Ui::Dialog *ui;

    QSerialPort *serial;
    comserial *secprocSerial;
    QTimer *timer,*timer2;
    short int giay;

};

#endif // DIALOG_H
