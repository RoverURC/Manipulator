#include "manipulator.h"
#include "motor.h"
#include <bcm2835.h>

const int axisNumber = 5;
const int servoMin[axisNumber] = {150, 150, 100, 100, 100};
const int servoMax[axisNumber] = {500, 500, 400, 550, 550};
Manipulator::Manipulator(int modbusPort, QObject *parent ):
  ModbusServer(modbusPort, parent )
{
  myServoDriver = new ServoDriver(PCA9685_WRITE);
  myMotorDriver = new Motor();
  connect(this,SIGNAL(registerChanged(int,quint16)),this,SLOT(interpretChangedRegister(int,quint16)));

  servoPWMTable = new int[axisNumber];
  for(int i=0;i<4;i++)
    servoPWMTable[i] = servoMin[i]; //TO DO


  servoUpdateTimer = new QTimer(this);

  connect(servoUpdateTimer, SIGNAL(timeout()),this,SLOT(updateServo()));

  servoUpdateTimer->start(10);
//  while(1){
//      //bcm2835_gpio_write()

//      myMotorDriver->setSpeed(255);
//      bcm2835_delay(20000);
//      myMotorDriver->setDirection(LEFT_DIRECTION);
//      qDebug()<<"A";

//      bcm2835_delay(20000);
//      myMotorDriver->setDirection(RIGHT_DIRECTION);
//      qDebug()<<"B";
//    }
}
void Manipulator::interpretChangedRegister(int index, quint16 value){

}

//Soft change every serverUpdateTimer timeout()
void Manipulator::updateServo(){
  quint16 registerValue;

  for(int i=0; i<axisNumber; i++){
    if(i!=0){
      getRegister(i, registerValue);
      if(servoPWMTable[i]>registerValue && servoPWMTable[i]>servoMin[i]){
      servoPWMTable[i] --;
      }
      if(servoPWMTable[i]<registerValue && servoPWMTable[i]<servoMax[i]){
        servoPWMTable[i] ++;
      }
      qDebug()<<"PWM: "<<i<<" "<<servoPWMTable[i];
      if(i==1){ //We need to set servo0 too

        myServoDriver->servoMove(1, servoPWMTable[1]);
        myServoDriver->servoMove(0, servoPWMTable[1]);
      }
      else
        myServoDriver->servoMove(i, servoPWMTable[i]);
    }
  }
}
