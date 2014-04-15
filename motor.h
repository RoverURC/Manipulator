#ifndef MOTOR_H
#define MOTOR_H

#define PWM_PIN RPI_GPIO_P1_12//pin pwm, jedyny na RPi
#define PWM_CHANNEL 0 //kanal
#define RANGE 256 //?? zakres pwmki?

#include <QObject>

class Motor : public QObject
{
  Q_OBJECT
public:
  explicit Motor(QObject *parent = 0);

  void motorInit();
  void setDirection(quint8 direction);
  void setSpeed(quint8 pwm_value);

signals:
  
public slots:

private:
    bool inA;
    bool inB; 
};

#endif // MOTOR_H
