#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>

#define Coffee_fee  100
#define Tea_fee     150
#define Cola_fee    200

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->pbCoffee->setEnabled(false);
    ui->pbTea->setEnabled(false);
    ui->pbCola->setEnabled(false);
    ui->pbReset->setEnabled(false);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::changeMoney(int diff) {
    money += diff;
    ui->lcdNumber->display(money);
    ui->pbCoffee->setEnabled(money >= Coffee_fee);
    ui->pbTea->setEnabled(money >= Tea_fee);
    ui->pbCola->setEnabled(money >= Cola_fee);
    ui->pbReset->setEnabled(money > 0);
}

void Widget::on_pb10_clicked()
{
    changeMoney(10);
}

void Widget::on_pb50_clicked()
{
    changeMoney(50);
}

void Widget::on_pb100_clicked()
{
    changeMoney(100);
}

void Widget::on_pb500_clicked()
{
    changeMoney(500);
}

void Widget::on_pbCoffee_clicked()
{
    changeMoney(Coffee_fee * -1);
}

void Widget::on_pbTea_clicked()
{
    changeMoney(Tea_fee * -1);
}

void Widget::on_pbCola_clicked()
{
    changeMoney(Cola_fee * -1);
}

void Widget::on_pbReset_clicked()
{
    QMessageBox::StandardButton msg;

    //inline assembly code
    #if defined (unix) && !defined (WIN32)
    __asm__ ( "xor %%edx, %%edx\n\t"
              "divl %%ebx" : "=a" (coin500), "=d" (coin100)
                           : "a" (money), "b" (500));
    __asm__ ( "xor %%edx, %%edx\n\t"
              "divl %%ebx" : "=a" (coin100), "=d" (coin50)
                           : "a" (coin100), "b" (100));
    __asm__ ( "xor %%edx, %%edx\n\t"
              "divl %%ebx" : "=a" (coin50), "=d" (coin10)
                           : "a" (coin50), "b" (50));
    __asm__ ( "xor %%edx, %%edx\n\t"
              "divl %%ebx" : "=a" (coin10)
                           : "a" (coin10), "b" (10));
    #elif defined (WIN32) && !defined (unix)
    __asm {
        mov eax, money
        mov ebx, 500
        xor edx, edx
        div ebx
        mov coin500, eax
        mov coin100, edx

        mov eax, edx
        mov ebx, 100
        xor edx, edx
        div ebx
        mov coin100, eax
        mov coin50, edx

        mov eax, edx
        mov ebx, 50
        xor edx, edx
        div ebx
        mov coin50, eax
        mov coin10, edx

        mov eax, edx
        mov ebx, 10
        xor edx, edx
        div ebx
        mov coin10, eax
    }
    #else
    coin500 = money / 500;
    coin100 = money % 500 / 100;
    coin50 = money % 500 % 100 / 50;
    coin10 = money % 500 % 100 % 50 / 10;
    #endif

    msg = QMessageBox::question(this, "Change",
                                tr("  Change Money States    \n"    //QObject::tr()
                                   "        Coin   500   :   %1   \n"
                                   "        Coin   100   :   %2   \n"
                                   "        Coin     50   :   %3   \n"
                                   "        Coin     10   :   %4   \n").arg(coin500)
                                                                       .arg(coin100)
                                                                       .arg(coin50)
                                                                       .arg(coin10),
                                QMessageBox::Yes|QMessageBox::No);
    if(msg == QMessageBox::Yes)
        changeMoney(money * -1);
}
