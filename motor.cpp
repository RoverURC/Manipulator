#include "motor.h"
#include <bcm2835.h>
#include <QDebug>

Motor::Motor(QObject *parent) :
  QObject(parent)
{
  motorInit();

  bcm2835_gpio_fsel(RPI_GPIO_P1_16, BCM2835_GPIO_FSEL_OUTP);
  bcm2835_gpio_fsel(RPI_GPIO_P1_11, BCM2835_GPIO_FSEL_OUTP);

}

void Motor::setDirection(quint8 direction)
{
  if (direction == BREAK_DIRECTION) //hard brake
  {

      bcm2835_gpio_set(RPI_GPIO_P1_16);
    bcm2835_gpio_set(RPI_GPIO_P1_11);
  }
  else if (direction == LEFT_DIRECTION) //clockwise direction
  {
      qDebug()<<"TU";
      bcm2835_gpio_clr(RPI_GPIO_P1_16);
    bcm2835_gpio_set(RPI_GPIO_P1_11);
  }
  else if (direction == RIGHT_DIRECTION) //counter clockwise direction
  {
qDebug()<<"Tam";
bcm2835_gpio_set(RPI_GPIO_P1_16);
bcm2835_gpio_set(RPI_GPIO_P1_13);
    bcm2835_gpio_clr(RPI_GPIO_P1_11);
  }
  else //loose brake
  {
      bcm2835_gpio_clr(RPI_GPIO_P1_16);
    bcm2835_gpio_clr(RPI_GPIO_P1_11);
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
