#ifndef SERVODRIVER_H
#define SERVODRIVER_H

/* Class for PWM Driver
 * How it works:
 * We use only 5 channels to drive 5 servo motors
 * We hold data in array of 4 integers
 *    0 - First, and second Jumbo Servo
 *    1 - 5V Servo
 *    2 - 5V Servo
 *    3 - 5V Servo
 * Second servo is computed based on first
 *
 * We set PWM by setServoPWM(index, value)
 * Driver doesnt set pwm when requested, instead it remembers it
 * and slowly goes to that value
 *
 */

#include <QObject>
#include "codes.h"
#include <QDebug>
#include <iostream>
#include <bcm2835.h>
#include <stdio.h>
#include <stdint.h>
#include <limits>
#include <QTimer>

class ServoDriver : public QObject
{
    Q_OBJECT

public:
  explicit ServoDriver(quint8 address, QObject *parent = 0);
  ~ServoDriver();
  void driverInit();

public slots:
  void setServoPWM(int index, quint16 value);

private slots:
  void updateServo();

private:
  QTimer *servoUpdateTimer;
  void servoMove(quint8 channel, quint16 inputDutyCycle);

  //driver address - I2C bus
  quint8 driver_I2C_address;

  int *targetPWMTable;
  int *actualPWMTable;
};
#endif // SERVODRIVER_H
