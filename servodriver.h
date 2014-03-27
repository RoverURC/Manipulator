#ifndef SERVODRIVER_H
#define SERVODRIVER_H

#include <QObject>
#include "codes.h"
#include <bcm2835.h>
#include <stdio.h>
#include <stdint.h>

class ServoDriver : public QObject
{
    Q_OBJECT
public:
    explicit ServoDriver(quint8 address, QObject *parent = 0);
    ~ServoDriver();


    //initializes servo driver with default settings
    void driverInit();

    //sends desired duty cycle to desired actuator
    void servoMove(quint8 channel, quint8 angle);

signals:
    
public slots:
    void interpretChangedRegister(int index, quint16 value);

private:
    //driver address - I2C bus
    quint8 driver_I2C_address;

    quint16 dutyCycleCalc(quint8 value);

};
#endif // SERVODRIVER_H
