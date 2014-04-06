#include "manipulator.h"

Manipulator::Manipulator(int modbusPort, QObject *parent )
//ModbusServer(modbusPort, parent )
{
  myServoDriver = new ServoDriver(PCA9685_WRITE);
  connect(this,SIGNAL(registerChanged(int,quint16)),this,SLOT(interpretChangedRegister(int,quint16)));
}
void Manipulator::interpretChangedRegister(int index, quint16 value){
  myServoDriver->servoMove(index, value);
}
