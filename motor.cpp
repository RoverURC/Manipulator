#include "motor.h"
#include <bcm2835.h>
#include <QDebug>

Motor::Motor(QObject *parent) :
  QObject(parent), inA(false), inB(false)
{
  motorInit();
}

void Motor::setDirection(quint8 direction)
{
  if (direction == 0) //hard brake
    {
      inA = true; inB = true;
    }

  else if (direction == 1) //clockwise direction
    {
      inA = true; inB = false;
    }

  else if (direction == 2) //counter clockwise direction
    {
      inA = false; inB = true;
    }

  else //loose brake
    {
      inA = false; inB = false;
    }
}

void Motor::setSpeed(quint8 pwm_value)
{
  bcm2835_pwm_set_data(PWM_CHANNEL, pwm_value); //2arg - data
  bcm2835_delay(50); //us
}

void Motor::motorInit()
{
  //if(!bcm2835_init()) return -69; //nie wiem ile razy mozna inicjalizowac scalak bcm2835 bo juz jest raz w serodriver ta komenda
  qDebug()<<"inicjalizacja sterownika silnika\n";
  bcm2835_gpio_fsel(PWM_PIN, BCM2835_GPIO_FSEL_ALT5); //ustawiamy pin pwki na ALtFun 5 - bo tak trzeba

  //clock divider jst domyslnie na 16
  // RPIclock * 1/clockdivider * 1/RANGE = nasza czestotliwosc
  bcm2835_pwm_set_clock(BCM2835_PWM_CLOCK_DIVIDER_4);
  bcm2835_pwm_set_mode(PWM_CHANNEL, 1, 1); //markspace =1 i enabled =1
  bcm2835_pwm_set_range(PWM_CHANNEL, RANGE);
  bcm2835_pwm_set_data(PWM_CHANNEL, 0); //2arg - data
  bcm2835_delay(50); //us
}
