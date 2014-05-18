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
#include <QTimer>

class Motor : public QObject
{
  Q_OBJECT
public:
  explicit Motor(QObject *parent = 0);


public slots:
  void setSpeed(qint16 pwmValue);

private slots:
  void updateSpeed();
private:
  void setDirection(quint8 direction);
  int actualSpeed;
  int targetSpeed;
  QTimer *speedRegulationTimer;
};

#endif // MOTOR_H
