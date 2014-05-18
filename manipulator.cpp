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

  myExtSerialPort= new QextSerialPort("/dev/ttyAMA0");
  myExtSerialPort->setBaudRate(BAUD9600);
  myExtSerialPort->setFlowControl(FLOW_OFF);
  myExtSerialPort->setParity(PAR_NONE);
  myExtSerialPort->setDataBits(DATA_8);
  myExtSerialPort->setStopBits(STOP_1);
  //port->setQueryMode(QextSerialPort::Polling);
  myExtSerialPort->close();
  if (myExtSerialPort->open(QIODevice::ReadWrite) == true){
      qDebug("port is opened");
  }
  else{
      qDebug("port is not open");
  }
  myDynamixServo = new DynamixelServo(1,myExtSerialPort);
  myDynamixServo->setStatusReturnPackage(2, true);
  myDynamixServo->setCCWAngleLimit(300);
  myDynamixServo->setSpeed(20,true);

  myLedIndicator->startBlinking(250);
  connect(this,SIGNAL(registerChanged(int,quint16)),this,SLOT(interpretChangedRegister(int,quint16)));

}
void Manipulator::interpretChangedRegister(int index, quint16 value){
  if(index>=0 && index<=3)
    myServoDriver->setServoPWM(index,value);
  if(index==4){
    myDynamixServo->setAngle(value,true);
    }

  if(index ==5 ){
     myMotorDriver->setSpeed(value);
    }
}


