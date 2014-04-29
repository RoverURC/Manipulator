#include "manipulator.h"
#include "motor.h"
#include <bcm2835.h>
//0, 1 - Jumbo
//2, 3, 4 = Serwa 5V
//+ Uart

Manipulator::Manipulator(int modbusPort, QObject *parent ):
  ModbusServer(modbusPort, parent )
{
  if(bcm2835_init()) qDebug()<<"Bcm2835 Init Succesfull" ; //if error initializing bcm2835 -> exiting program

  myServoDriver = new ServoDriver(PCA9685_WRITE);
  myMotorDriver = new Motor();
  myLedIndicator = new LedIndicator();

  myExtSerialPort = new QextSerialPort();
  myDynamixServo = new DynamixelServo(1,myExtSerialPort,this);
  myDynamixServo->setSpeed(400);

  myLedIndicator->startBlinking(500);
  connect(this,SIGNAL(registerChanged(int,quint16)),this,SLOT(interpretChangedRegister(int,quint16)));

}
void Manipulator::interpretChangedRegister(int index, quint16 value){
  if(index>=0 && index<=3)
    myServoDriver->setServoPWM(index,value);
  if(index==4)
    myMotorDriver->setSpeed(value);
  if(index ==5 )
    myDynamixServo->setAngle(value);
}


