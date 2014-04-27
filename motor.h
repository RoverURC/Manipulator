#ifndef MOTOR_H
#define MOTOR_H

#define PWM_PIN RPI_GPIO_P1_12//pin pwm, jedyny na RPi
#define PWM_CHANNEL 0 //kanal
#define RANGE 256 //?? zakres pwmki?

#define BREAK_DIRECTION 0
#define LEFT_DIRECTION 1
#define RIGHT_DIRECTION 2
#define IDLE_DIRECTION 3
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
};

#endif // MOTOR_H
