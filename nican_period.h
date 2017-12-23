#ifndef NICAN_PERIOD_H
#define NICAN_PERIOD_H
#include "Nican.h"
#include <QTimer>
#include <QObject>

class NICAN_Period:public QObject
{
    Q_OBJECT
public:
    NICAN_Period();
    ~NICAN_Period();

public:
    void connectDevice();
    void disConnectDevice();
    void CANDataWriteConfig(NCTYPE_CAN_STRUCT sendData);
private:
    NCTYPE_OBJH_P objh=new NCTYPE_OBJH(0);      //init objecthandle
    NCTYPE_STATUS CAN_Sta=0;                    //init can_sta
    bool CAN_flag=false;
    NCTYPE_UINT32 dataSize_R=196;                 //size of read data struct
    NCTYPE_UINT32 dataSize_W=22;                 //size of write data struct
    NCTYPE_CAN_STRUCT_P dataStruct_R=new NCTYPE_CAN_STRUCT;    //read data struct point
    NCTYPE_CAN_STRUCT_P dataStruct_W=new NCTYPE_CAN_STRUCT;    //write data struct point
    NCTYPE_STRING interface="CAN0";
    NCTYPE_ATTRID p[2]={NC_ATTR_START_ON_OPEN,NC_ATTR_BAUD_RATE};
    //NICAN Interface Config,8473 only NC_ATTR_START_ON_OPEN and NC_ATTR_BAUD_RATE are valid
    NCTYPE_UINT32 attr[2]={1,500000};
    NCTYPE_UINT32_P actualDataSize=new NCTYPE_UINT32(0);
    QTimer timer_1ms;
    QTimer timer_10ms;
    QTimer timer_100ms;
    QTimer timer_1000ms;

public slots:
    void CAN_Message_1ms();
    void CAN_Message_10ms();
    void CAN_Message_100ms();
    void CAN_Message_1000ms();
    void trigged_CAN_Send(NCTYPE_CAN_STRUCT sendData);
signals:
    void read(NCTYPE_CAN_STRUCT_P *p);
    void error(const QString &s);
};

#endif // NICAN_PERIOD_H
