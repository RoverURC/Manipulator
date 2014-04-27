#ifndef MANIPULATOR_H
#define MANIPULATOR_H
#include <QTimer>
#include <modbusserver.h>
#include <servodriver.h>
#include <motor.h>
#define asd 5
class Manipulator : public ModbusServer
{
  Q_OBJECT
public:
    Manipulator(int modbusPort= 1502, QObject *parent = 0);

public slots:
    void interpretChangedRegister(int index, quint16 value);
    void updateServo();
signals:

private:
    ServoDriver *myServoDriver;
    Motor *myMotorDriver;
    int *servoPWMTable;
    QTimer *servoUpdateTimer;
};

#endif // MANIPULATOR_H
