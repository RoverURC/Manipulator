#ifndef MANIPULATOR_H
#define MANIPULATOR_H
#include <QTimer>
#include <modbusserver.h>
#include <servodriver.h>
#include <motor.h>
#include <ledindicator.h>
#include <dynamixelservo.h>
#include <QextSerialPort>
#define asd 5
class Manipulator : public ModbusServer
{
  Q_OBJECT
public:
    Manipulator(int modbusPort= 1502, QObject *parent = 0);

public slots:
    void interpretChangedRegister(int index, quint16 value);
signals:

private:
    QextSerialPort *myExtSerialPort;
    DynamixelServo *myDynamixServo;
    LedIndicator *myLedIndicator;
    ServoDriver *myServoDriver;
    Motor *myMotorDriver;
    QTimer *servoUpdateTimer;
};

#endif // MANIPULATOR_H
