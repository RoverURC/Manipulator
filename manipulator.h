#ifndef MANIPULATOR_H
#define MANIPULATOR_H
#include <modbusserver.h>
#include <servodriver.h>
#include <motor.h>

class Manipulator : public ModbusServer
{
  Q_OBJECT
public:
    Manipulator(int modbusPort= 1502, QObject *parent = 0);

public slots:
    void interpretChangedRegister(int index, quint16 value);
signals:

private:
    ServoDriver *myServoDriver;
    Motor *myMotorDriver;
};

#endif // MANIPULATOR_H
