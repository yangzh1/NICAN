#include "nican_period.h"

NICAN_Period::NICAN_Period()
{
    connect(&timer_100ms,SIGNAL(timeout()),this,SLOT(CAN_Message_100ms()));

}

NICAN_Period::~NICAN_Period()
{
    delete objh;
    delete dataStruct_R;
    delete dataStruct_W;
    delete actualDataSize;
}

void NICAN_Period::connectDevice()
{
    CAN_Sta=ncConfig(interface,2,p,attr);
    if(CAN_Sta<0)
        emit error(tr("Config Error"));
    CAN_Sta=ncOpenObject(interface,objh);
    if(CAN_Sta<0)
        emit error(tr("OpenObject Error"));
    else
    {
        timer_1ms.start(1);
        timer_10ms.start(10);
        timer_100ms.start(100);
        timer_1000ms.start(1000);
        CAN_flag=true;
     }
}

void NICAN_Period::CAN_Message_100ms()
{
    /*NCTYPE_UINT32_P attrPrt=new NCTYPE_UINT32(0);
    //init attribution point
    NCTYPE_ATTRID attrID=NC_ATTR_READ_PENDING;
    //attribution ID
    CAN_Sta=ncGetAttribute(*objh,attrID,4,attrPrt);
    if(CAN_Sta<0)
        emit error(tr("GetAttribute Error"));*/
    if(CAN_Sta>=0&&CAN_flag==true)
    {
        CAN_Sta=ncReadMult(*objh,dataSize_R,dataStruct_R,actualDataSize);
        if(CAN_Sta<0)
        {
            emit error(tr("ReadMult Error"));
            CAN_Sta=ncCloseObject(*objh);             //close the object and then reconnect
            CAN_Sta=ncConfig(interface,2,p,attr);
            if(CAN_Sta<0) return;
        }
        else
        {
            emit read(&dataStruct_R);
        }
        /*CANDataWriteConfig();
        CAN_Sta=ncWriteMult(*objh,dataSize_W,dataStruct_W);
        if(CAN_Sta<0)
        {
            emit error(tr("WriteMult Error"));
            return;
        }*/

    }
}

void NICAN_Period::CAN_Message_1ms()
{

}

void NICAN_Period::CAN_Message_10ms()
{

}

void NICAN_Period::CAN_Message_1000ms()
{

}

void  NICAN_Period::CANDataWriteConfig(NCTYPE_CAN_STRUCT sendData)
{
    dataStruct_W->ArbitrationId=sendData.ArbitrationId;
    dataStruct_W->Data[0]=sendData.Data[0];
    dataStruct_W->Data[1]=sendData.Data[1];
    dataStruct_W->Data[2]=sendData.Data[2];
    dataStruct_W->Data[3]=sendData.Data[3];
    dataStruct_W->Data[4]=sendData.Data[4];
    dataStruct_W->Data[5]=sendData.Data[5];
    dataStruct_W->Data[6]=sendData.Data[6];
    dataStruct_W->Data[7]=sendData.Data[7];
    dataStruct_W->DataLength=sendData.DataLength;
    dataStruct_W->FrameType=sendData.FrameType;
    dataStruct_W->Timestamp.HighPart=sendData.Timestamp.HighPart;
    dataStruct_W->Timestamp.LowPart=sendData.Timestamp.LowPart;
}

void NICAN_Period::disConnectDevice()
{
    CAN_Sta=ncCloseObject(*objh);
    if(CAN_Sta<0)
        emit error(tr("CloseObject Error"));
    CAN_flag=false;
    timer_1ms.stop();
    timer_10ms.stop();
    timer_100ms.stop();
    timer_1000ms.stop();
}

void NICAN_Period::trigged_CAN_Send(NCTYPE_CAN_STRUCT sendData)
{
    CANDataWriteConfig(sendData);
    CAN_Sta=ncWriteMult(*objh,dataSize_W,dataStruct_W);
    if(CAN_Sta<0)
    {
        emit error(tr("WriteMult Error"));
        return;
    }
}
