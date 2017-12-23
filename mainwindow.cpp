#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /*item->setText(tr("abc%1").arg(1));
    ui->tableWidget->setItem(1,1,item);*/

    connect(ui->action_Connect,&QAction::triggered,nican1,&NICAN_Period::connectDevice);
    connect(ui->action_Disconnect,&QAction::triggered,nican1,&NICAN_Period::disConnectDevice);
    connect(nican1,&NICAN_Period::read,this,&MainWindow::showResponse);
    connect(nican1,&NICAN_Period::error,this,&MainWindow::processError);
    connect(ui->sendButton,&QPushButton::clicked,this,&MainWindow::sendFrames);
    connect(this,&MainWindow::triggedSend,nican1,&NICAN_Period::trigged_CAN_Send);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete nican1;
}

void MainWindow::showResponse(NCTYPE_CAN_STRUCT_P *p)
{
    ui->label->setText(tr("CAN_ID:%1\nCAN_Datalength:%2\nCAN_Data:%3 %4 %5 %6 %7 %8 %9 %10")
                       .arg((*p)->ArbitrationId)
                       .arg((*p)->DataLength)
                       .arg((*p)->Data[0])
                       .arg((*p)->Data[1])
                       .arg((*p)->Data[2])
                       .arg((*p)->Data[3])
                       .arg((*p)->Data[4])
                       .arg((*p)->Data[5])
                       .arg((*p)->Data[6])
                       .arg((*p)->Data[7]));
    ui->label_3->setText(tr("CAN_Timestamp:%1%2").arg((*p)->Timestamp.HighPart)
                       .arg((*p)->Timestamp.LowPart));
    const QString can_data=tr("%1  %2  %3 %4 %5 %6 %7 %8 %9 %10 ")
                       .arg((*p)->ArbitrationId)
                       .arg((*p)->DataLength)
                       .arg((*p)->Data[0])
                       .arg((*p)->Data[1])
                       .arg((*p)->Data[2])
                       .arg((*p)->Data[3])
                       .arg((*p)->Data[4])
                       .arg((*p)->Data[5])
                       .arg((*p)->Data[6])
                       .arg((*p)->Data[7]);
    ui->textEdit->append(can_data);

}

void MainWindow::processError(const QString &s)
{
    ui->label_2->setText(tr( "Error: %1").arg(s));
}

void MainWindow::sendFrames()
{
    const uint frameId = ui->frameIdEdit->text().toUInt(nullptr,16);     //16 is Hex
    QString data = ui->payloadEdit->text();
    QByteArray payload=QByteArray::fromHex(data.remove(QLatin1Char(' ')).toLatin1()); //remove' '.then from hex change to char*
    sendData.ArbitrationId=frameId;
    sendData.DataLength=8;
    sendData.FrameType=0;
    sendData.Timestamp.HighPart=0;
    sendData.Timestamp.LowPart=0;
    sendData.Data[0]=payload[0];
    sendData.Data[1]=payload[1];
    sendData.Data[2]=payload[2];
    sendData.Data[3]=payload[3];
    sendData.Data[4]=payload[4];
    sendData.Data[5]=payload[5];
    sendData.Data[6]=payload[6];
    sendData.Data[7]=payload[7];
    emit triggedSend(sendData);
}
