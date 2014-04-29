#include "dynamixelservo.h"
#include <QDebug>
#include <QProcess>

#define READ_DATA 0x02
#define WRITE_DATA 0x03
#define WRITE_REG 0x04
#define ACTION 0x05
#define RESET 0x06

#define BROADCAST 0xfe

#define GOAL_POSITION 0x1e
#define SPEED 0x20
#define CW_ANGLE_LIMIT 0x06
#define CCW_ANGLE_LIMIT 0x08
#define CW_DIRECTION 1
#define CCW_DIRECTION 0

#define NO_INFO_RET 0
#define ASK_INFO_RET 1
#define ALL_INFO_RET 2

DynamixelServo::DynamixelServo(quint8 id, QextSerialPort *device, QObject *parent) :  QObject(parent)
{
    timer = new QTimer(this);
    this->device = device; //connects device
    this->id = id;

    this->CWAngleLimitHB = 0;
    this->CWAngleLimitLB = 0;
    this->CCWAngleLimitHB = 3;
    this->CCWAngleLimitLB = 255;

    this->LedOn = false;
    this->timerOut = false;

    connect(timer,SIGNAL(timeout()), this, SLOT(setTimerOut()));

    //delete this after tests
    this->temp = true;
    this->torque = false;
    this->CCWAngleLimit = false;

}
DynamixelServo::~DynamixelServo()
{}
//--need to be complited
bool DynamixelServo::delay(int msc)
{
    return true;
}
bool DynamixelServo::setTimerOut()
{
    this->timerOut = true;
    return true;
}
//--end
bool DynamixelServo::setSpeed(double Speed, bool broadcast)
{
    byteArray.clear();
    byteArray.append((char)0xff);
    byteArray.append((char)0xff);

    if(broadcast)
        byteArray.append((char)BROADCAST);
    else
        byteArray.append((char)id);

    byteArray.append((char)0x05);
    byteArray.append((char)WRITE_DATA);

    byteArray.append((char)SPEED);

    //it transform's speed in percent to 2byte array speed in proper value 0 to 1024
    if(Speed>100)
        Speed=100;
    if(Speed<0)
        Speed=0;

    double intSpeed = (double)Speed*1023/(double)100;
    quint8 HB = intSpeed/256;
    quint8 LB = intSpeed-HB*256;
    byteArray.append((char)LB);
    byteArray.append((char)HB);

    addCRC(byteArray);
    send(byteArray);
    return true;
}
bool DynamixelServo::setAngle(double angle, bool now)
{
    byteArray.clear();
    byteArray.append((char)0xff);
    byteArray.append((char)0xff);
    byteArray.append((char)id);
    byteArray.append((char)0x05);
    if(now == false) //writes data to register, waits for function updateServo
        byteArray.append((char)WRITE_REG);
    else //now == true
        byteArray.append((char)WRITE_DATA);

    byteArray.append((char)GOAL_POSITION);

    //it transform's angle in degrees 0-300 to proper value in 2byte array angle from 0 to 1024
    if(angle>300)
        angle=300;
    if(angle<0)
        angle=0;

    double intAngle = (double)angle*1023/(double)300;
    quint8 HB = intAngle/256;
    quint8 LB = intAngle-HB*256;
    byteArray.append((char)LB);
    byteArray.append((char)HB);

    addCRC(byteArray);
    send(byteArray);
    return true;
}
bool DynamixelServo::setAngleAndSpeed(double angle, double Speed, bool now)
{
    byteArray.clear();
    byteArray.append((char)0xff);
    byteArray.append((char)0xff);
    byteArray.append((char)id);
    byteArray.append((char)0x07);
    if(now == false) //writes data to register, waits for function updateServo
        byteArray.append((char)WRITE_REG);
    else //now == true
        byteArray.append((char)WRITE_DATA);

    byteArray.append((char)GOAL_POSITION);

    ///setting angle
    //it transform's angle in degrees 0-300 to proper value in 2byte array angle from 0 to 1024
    if(angle>300)
        angle=300;
    if(angle<0)
        angle=0;

    double intAngle = (double)angle*1023/(double)300;
    quint8 HB = intAngle/256;
    quint8 LB = intAngle-HB*256;
    byteArray.append((char)LB);
    byteArray.append((char)HB);

    ///setting speed
    //moving speed is register next after goal position,
    //so no need to say (can't say) where to write data
    if(Speed=100)
        Speed=100;
    if(Speed<0)
        Speed=0;

    double intSpeed = (double)Speed*1023/(double)100;
    HB = intSpeed/256;
    LB = intSpeed-HB*256;
    byteArray.append((char)LB);
    byteArray.append((char)HB);

    addCRC(byteArray);
    send(byteArray);
    return true;

}

bool DynamixelServo::setCWAngleLimit(double CWAngle) //min 0 degree //clockwise
{
    byteArray.clear();
    byteArray.append((char)0xff);
    byteArray.append((char)0xff);
    byteArray.append((char)id);
    byteArray.append((char)0x05);
    byteArray.append((char)WRITE_DATA);

    byteArray.append((char)CW_ANGLE_LIMIT);

    //it transform's angle in degrees 0-300 to proper value in 2byte array angle from 0 to 1024
    if(CWAngle<0)
        CWAngle = 0;
    if(CWAngle>300)
        CWAngle = 300;

    double intAngle = (double)CWAngle*1023/(double)300;
    quint8 HB = intAngle/256;
    quint8 LB = intAngle-HB*256;
    byteArray.append((char)LB);
    byteArray.append((char)HB);

    this->CWAngleLimitHB = HB;  //needs for getting from wheel mode
    this->CWAngleLimitLB = LB;

    addCRC(byteArray);
    send(byteArray);
    return true;

}
bool DynamixelServo::setCCWAngleLimit(double CCWAngle) //max 300 degree //counter clockwise
{
    byteArray.clear();
    byteArray.append((char)0xff);
    byteArray.append((char)0xff);
    byteArray.append((char)id);
    byteArray.append((char)0x05);
    byteArray.append((char)WRITE_DATA);

    byteArray.append((char)CCW_ANGLE_LIMIT);

    //it transform's angle in degrees 0-300 to proper value in 2byte array angle from 0 to 1024

    if(CCWAngle<0)
        CCWAngle = 0;
    if(CCWAngle>300)
        CCWAngle = 300;

    double intAngle = (double)CCWAngle*1023/(double)300;
    quint8 HB = intAngle/256;
    quint8 LB = intAngle-HB*256;
    byteArray.append((char)LB);
    byteArray.append((char)HB);

    this->CCWAngleLimitHB = HB;  //needs for getting from wheel mode
    this->CCWAngleLimitLB = LB;

    addCRC(byteArray);
    send(byteArray);
    return true;
}

bool DynamixelServo::setWheelMode(bool enable, bool broadcast)
{
    if(enable)  ///setting wheel mode
    {
        this->setSpeed(0.1,broadcast);
        byteArray.clear();
        byteArray.append((char)0xff);
        byteArray.append((char)0xff);

        if(broadcast)
            byteArray.append((char)BROADCAST);
        else
            byteArray.append((char)id);

        byteArray.append((char)0x07);
        byteArray.append((char)WRITE_DATA);

        byteArray.append((char)CW_ANGLE_LIMIT);

        byteArray.append((char)0x00);
        byteArray.append((char)0x00);
        byteArray.append((char)0x00);
        byteArray.append((char)0x00);

        addCRC(byteArray);
        send(byteArray);
        this->stop(true,broadcast);
    }
    else    ///getting back from wheel mode
    {
        //this->stop(true,broadcast);
        this->setSpeed(0.1,broadcast);
        this->setTorqueOFF(broadcast);
        byteArray.clear();
        byteArray.append((char)0xff);
        byteArray.append((char)0xff);
        byteArray.append((char)id);

        byteArray.append((char)0x07);
        byteArray.append((char)WRITE_DATA);

        byteArray.append((char)CW_ANGLE_LIMIT);

        byteArray.append((char)this->CWAngleLimitLB);
        byteArray.append((char)this->CWAngleLimitHB);
        byteArray.append((char)this->CCWAngleLimitLB);
        byteArray.append((char)this->CCWAngleLimitHB);

        addCRC(byteArray);
        send(byteArray);

        this->setTorqueOFF(broadcast);
    }
    return true;
}
bool DynamixelServo::setDirectionAndSpeed(quint8 direction, double Speed, bool now)
{
    if(direction !=0 && direction != 1)
        return false;

    byteArray.clear();
    byteArray.append((char)0xff);
    byteArray.append((char)0xff);

    byteArray.append((char)id);

    byteArray.append((char)0x05);
    if(now)
        byteArray.append((char)WRITE_DATA);
    else
        byteArray.append((char)WRITE_REG);

    byteArray.append((char)SPEED);

    //it transform's speed in percent to 2byte array speed in proper value 0 to 1024
    if(Speed>100)
        Speed=100;
    if(Speed<0)
        Speed=0;

    double intSpeed = (double)Speed*1023/(double)100 + direction*1024; //adding direction
    quint8 HB = intSpeed/256;
    quint8 LB = intSpeed-HB*256;
    byteArray.append((char)LB);
    byteArray.append((char)HB);

    addCRC(byteArray);
    send(byteArray);
    return true;
}

bool DynamixelServo::stop(bool now, bool broadcast)
{
    this->setSpeed(0,broadcast);
    return true;
}
bool DynamixelServo::setMaxTorque(double torque)
{
    byteArray.clear();
    byteArray.append((char)0xff);
    byteArray.append((char)0xff);
    byteArray.append((char)id);

    byteArray.append((char)0x05);
    byteArray.append((char)WRITE_DATA);

    byteArray.append((char)0x22);

    if(torque>100)
        torque=100;
    if(torque<0)
        torque=0;

    double intTorque = (double)torque*1023/(double)100;
    quint8 HB = intTorque/256;
    quint8 LB = intTorque-HB*256;
    byteArray.append((char)LB);
    byteArray.append((char)HB);

    addCRC(byteArray);
    send(byteArray);
    return true;
}

bool DynamixelServo::updateServo(bool broadcast) //this or broadcast
{
    byteArray.clear();
    byteArray.append((char)0xff);
    byteArray.append((char)0xff);

    if(broadcast)
        byteArray.append((char)BROADCAST);
    else
        byteArray.append((char)id);

    byteArray.append((char)0x02);
    byteArray.append((char)ACTION);

    addCRC(byteArray);
    send(byteArray);
    return true;
}
bool DynamixelServo::setTorqueOFF(bool broadcast)
{
    byteArray.clear();
    byteArray.append((char)0xff);
    byteArray.append((char)0xff);

    if(broadcast)
        byteArray.append((char)BROADCAST);
    else
        byteArray.append((char)id);

    byteArray.append((char)0x04);
    byteArray.append((char)WRITE_DATA);

    byteArray.append((char)0x18);
    //set - torque OFF
    byteArray.append((char)0x00);
    addCRC(byteArray);
    send(byteArray);
    return true;
}
bool DynamixelServo::setStatusReturnPackage(quint8 info, bool broadcast) //sets if status package will be returned
{
    if(info !=0 && info != 1 && info != 2)
        return false;

    byteArray.clear();
    byteArray.append((char)0xff);
    byteArray.append((char)0xff);

    if(broadcast)
        byteArray.append((char)BROADCAST);
    else
        byteArray.append((char)id);

    byteArray.append((char)0x04);
    byteArray.append((char)WRITE_DATA);

    byteArray.append((char)0x10);   //status package adress
    byteArray.append((char)info);

    addCRC(byteArray);
    send(byteArray);
    return true;
}
bool DynamixelServo::setAlarmLeds(bool enable, bool broadcast)
{
    byteArray.clear();
    byteArray.append((char)0xff);
    byteArray.append((char)0xff);

    if(broadcast)
        byteArray.append((char)BROADCAST);
    else
        byteArray.append((char)id);

    byteArray.append((char)0x04);
    byteArray.append((char)WRITE_DATA);

    byteArray.append((char)0x11);

    if(enable)
        byteArray.append((char)0x7f);
    else
        byteArray.append((char)0x00);

    addCRC(byteArray);
    send(byteArray);
    return true;
}
bool DynamixelServo::turnLed(bool broadcast)
{
    if(LedOn)
        LedOn = false;
    else
        LedOn = true;

    byteArray.clear();
    byteArray.append((char)0xff);
    byteArray.append((char)0xff);

    if(broadcast)
        byteArray.append((char)BROADCAST);
    else
        byteArray.append((char)id);

    byteArray.append((char)0x04);
    byteArray.append((char)WRITE_DATA);

    byteArray.append((char)0x19);

    if(LedOn)
        byteArray.append((char)0x01);
    else
        byteArray.append((char)0x00);

    addCRC(byteArray);
    send(byteArray);
    return true;
}

bool DynamixelServo::getPresentAngle(quint16 &value)
{
    //sending
    byteArray.clear();
    byteArray.append((char)0xff);
    byteArray.append((char)0xff);
    byteArray.append((char)id);

    byteArray.append((char)0x04);
    byteArray.append((char)READ_DATA);
    byteArray.append((char)0x24);
    byteArray.append((char)0x02); //how many bytes to read

    addCRC(byteArray);
    send(byteArray);

    ///TO BE FINISHED, prototype
    /*
    //getting
    QByteArray readByteArray = device->readAll();

    //ff ff id length instr error param checksum
    byteArray.clear();
    if(readByteArray.size() < ((quint8)readByteArray[3])+4) //not long enough
        return false;
    for(int i=0; i<readByteArray.size()-1; i++) //copying without CRC
        byteArray.append((char)((quint8)readByteArray[i]));
    addCRC(byteArray);
    if((quint8)byteArray[byteArray.size()-1] != (quint8)readByteArray[readByteArray.size()-1]) //if CRCs are not the same
        return false;
    if((quin8)byteArray[2] != this->id)
        return false;

    //if everything is fine
    double valueTemp = (quint8)readByteArray[readByteArray.size()-3] + (quint8)readByteArray[readByteArray.size()-2]*256 +1;
    valueTemp = valueTemp/(double)1024*300;
    value = (quint16) valueTemp;
   */
    return true;
}
bool DynamixelServo::getPresentVoltage(quint8 &value)
{
    //sending
    byteArray.clear();
    byteArray.append((char)0xff);
    byteArray.append((char)0xff);
    byteArray.append((char)id);

    byteArray.append((char)0x04);
    byteArray.append((char)READ_DATA);
    byteArray.append((char)0x2A);
    byteArray.append((char)0x01); //how many bytes to read

    addCRC(byteArray);
    send(byteArray);

    ///TO BE FINISHED, prototype
    /*
    //getting
    QByteArray readByteArray = device->readAll();

    //ff ff id length instr error param checksum
    byteArray.clear();
    if(readByteArray.size() < ((quint8)readByteArray[3])+4) //not long enough
        return false;
    for(int i=0; i<readByteArray.size()-1; i++) //copying without CRC
        byteArray.append((char)((quint8)readByteArray[i]));
    addCRC(byteArray);
    if((quint8)byteArray[byteArray.size()-1] != (quint8)readByteArray[readByteArray.size()-1]) //if CRCs are not the same
        return false;
    if((quin8)byteArray[2] != this->id)
        return false;

    //if everything is fine
    value = (quint8)readByteArray[readByteArray.size()-2];
   */
    return true;
}
bool DynamixelServo::getActualSpeed(quint16 &value)
{
    //sending
    byteArray.clear();
    byteArray.append((char)0xff);
    byteArray.append((char)0xff);
    byteArray.append((char)id);

    byteArray.append((char)0x04);
    byteArray.append((char)READ_DATA);
    byteArray.append((char)0x26);
    byteArray.append((char)0x02); //how many bytes to read

    addCRC(byteArray);
    send(byteArray);

    ///TO BE FINISHED, prototype
    /*
    //getting
    QByteArray readByteArray = device->readAll();

    //ff ff id length instr error param checksum
    byteArray.clear();
    if(readByteArray.size() < ((quint8)readByteArray[3])+4) //not long enough
        return false;
    for(int i=0; i<readByteArray.size()-1; i++) //copying without CRC
        byteArray.append((char)((quint8)readByteArray[i]));
    addCRC(byteArray);
    if((quint8)byteArray[byteArray.size()-1] != (quint8)readByteArray[readByteArray.size()-1]) //if CRCs are not the same
        return false;
    if((quin8)byteArray[2] != this->id)
        return false;

    double valueTemp;
    //if everything is fine
    valueTemp = (quint8)readByteArray[readByteArray.size()-3]+ (quint8)readByteArray[readByteArray.size()-2]*256+1;
    valueTemp = (double)valueTemp/(double)1024*100;
    value = (quint8)valueTemp;
   */
    return true;
}
bool DynamixelServo::getSettedPosition(quint16 &value)
{
    //sending
    byteArray.clear();
    byteArray.append((char)0xff);
    byteArray.append((char)0xff);
    byteArray.append((char)id);

    byteArray.append((char)0x04);
    byteArray.append((char)READ_DATA);
    byteArray.append((char)GOAL_POSITION);
    byteArray.append((char)0x02); //how many bytes to read

    addCRC(byteArray);
    send(byteArray);

    ///TO BE FINISHED, prototype
    /*
    //getting
    QByteArray readByteArray = device->readAll();

    //ff ff id length instr error param checksum
    byteArray.clear();
    if(readByteArray.size() < ((quint8)readByteArray[3])+4) //not long enough
        return false;
    for(int i=0; i<readByteArray.size()-1; i++) //copying without CRC
        byteArray.append((char)((quint8)readByteArray[i]));
    addCRC(byteArray);
    if((quint8)byteArray[byteArray.size()-1] != (quint8)readByteArray[readByteArray.size()-1]) //if CRCs are not the same
        return false;
    if((quin8)byteArray[2] != this->id)
        return false;

    double valueTemp;
    //if everything is fine
    valueTemp = (quint8)readByteArray[readByteArray.size()-3]+ (quint8)readByteArray[readByteArray.size()-2]*256 + 1;
    valueTemp = (double)valueTemp/(double)1024*300;
    value = (quint16)valueTemp;
   */

    return true;}

bool DynamixelServo::addCRC(QByteArray &array) //adds CRC at the end of array
{
    quint8 result =0 ;
    for(int i=2; i<array.size();i++){
       result += (quint8)array[i];
    }
    result = ~result;
    array.append((char)result);
    return true;
}
bool DynamixelServo::send(const QByteArray array) //send QbyteArray using device;
{

    //sprawdzić mutex, czekanie z odczytem, aż do wysłania danych, zmienić preDkośc transmisji
    //device->flush();
    //mutex.lock();
    quint64 quantity = device->write(array);
    device->waitForBytesWritten(100);
    if(quantity != array.size())
        return false;
    /*qDebug()<<"wyslane dane";
    for(int i=0;i<array.size();i++){
        qDebug()<<(quint8)array[i];
    }*/
//    mutex.unlock();
    return true;
}
bool DynamixelServo::getIfMoving(bool &moving)
{
    //sending
    byteArray.clear();
    byteArray.append((char)0xff);
    byteArray.append((char)0xff);
    byteArray.append((char)id);

    byteArray.append((char)0x04);
    byteArray.append((char)READ_DATA);
    byteArray.append((char)0x2e);
    byteArray.append((char)0x01); //how many bytes

    addCRC(byteArray);
    send(byteArray);

    //device->readLine();

    ///TO BE FINISHED, prototype
/*
    //getting
    QByteArray readByteArray = device->readAll();

    //ff ff id length instr error param checksum
    byteArray.clear();
    if(readByteArray.size() < ((quint8)readByteArray[3])+4) //not long enough
        return false;
    for(int i=0; i<readByteArray.size()-1; i++) //copying without CRC
        byteArray.append((char)((quint8)readByteArray[i]));
    addCRC(byteArray);
    if((quint8)byteArray[byteArray.size()-1] != (quint8)readByteArray[readByteArray.size()-1]) //if CRCs are not the same
        return false;
    if((quin8)byteArray[2] != this->id)
        return false;

   //if everything is fine
    if((quint8)readByteArray[readByteArray.size()-2] == 1)
        moving = true;
    else
        moving = false;
   */
    return true;
}
