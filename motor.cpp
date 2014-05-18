#include "motor.h"
#include <bcm2835.h>
#include <QDebug>

#define PWM_PIN RPI_GPIO_P1_12//pin pwm, jedyny na RPi
#define EDGE_SWITCH_1 RPI_GPIO_P1_07
#define EDGE_SWITCH_2 RPI_GPIO_P1_15
const int maxSpeed = 20;
Motor::Motor(QObject *parent) :
  QObject(parent)
{
  actualSpeed = 0;
  targetSpeed = 0;

  bcm2835_gpio_fsel(PWM_PIN, BCM2835_GPIO_FSEL_ALT5); //ustawiamy pin pwki na ALtFun 5 - bo tak trzeba

  //Set RPI pin 13 & 15 as inputs
  bcm2835_gpio_fsel(EDGE_SWITCH_1, BCM2835_GPIO_FSEL_INPT);
  bcm2835_gpio_fsel(EDGE_SWITCH_2, BCM2835_GPIO_FSEL_INPT);
  //with pulldown
  bcm2835_gpio_set_pud(EDGE_SWITCH_1, BCM2835_GPIO_PUD_DOWN);
  bcm2835_gpio_set_pud(EDGE_SWITCH_2, BCM2835_GPIO_PUD_DOWN);

  //clock divider jst domyslnie na 16
  // RPIclock * 1/clockdivider * 1/RANGE = nasza czestotliwosc
  bcm2835_pwm_set_clock(BCM2835_PWM_CLOCK_DIVIDER_4);
  bcm2835_pwm_set_mode(PWM_CHANNEL, 1, 1); //markspace =1 i enabled =1
  bcm2835_pwm_set_range(PWM_CHANNEL, RANGE);
  bcm2835_pwm_set_data(PWM_CHANNEL, 0); //2arg - data
  bcm2835_delay(50); //us

  bcm2835_gpio_fsel(RPI_GPIO_P1_16, BCM2835_GPIO_FSEL_OUTP);
  bcm2835_gpio_fsel(RPI_GPIO_P1_11, BCM2835_GPIO_FSEL_OUTP);

  speedRegulationTimer = new QTimer(this);
  connect(speedRegulationTimer,SIGNAL(timeout()),this,SLOT(updateSpeed()));
  speedRegulationTimer->start(10);

  qDebug()<<"Motor Driver Init After";
}

//--------------------------------------------------

void Motor::updateSpeed(){
  //------------------------------------------------
  //do weryfikacji bo nie wiem kiedy jest lewo kiedy prawo
  if(targetSpeed<0 && bcm2835_gpio_lev(EDGE_SWITCH_1)){
    targetSpeed = 0;
  }
  //do weryfikacji bo nie wiem kiedy jest lewo kiedy prawo
  if(targetSpeed>0 && bcm2835_gpio_lev(EDGE_SWITCH_2)){
    targetSpeed = 0;
  }
  //---------------------------------------------

  if(actualSpeed<targetSpeed){
    actualSpeed++;
  }
  if(actualSpeed>targetSpeed){
    actualSpeed--;
  }
  if(targetSpeed<3 && targetSpeed>-3){
    actualSpeed = 0;
  }

  int pwmValue = abs(actualSpeed);
  if(pwmValue>RANGE)
    pwmValue = RANGE;

  bcm2835_pwm_set_data(PWM_CHANNEL, pwmValue); //2arg - data
  bcm2835_delay(50); //us

  if(actualSpeed<0)
    setDirection(LEFT_DIRECTION);
  if(actualSpeed>0)
    setDirection(RIGHT_DIRECTION);
  if(actualSpeed ==0){
    setDirection(BREAK_DIRECTION);
  }
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
    bcm2835_gpio_clr(RPI_GPIO_P1_16);
    bcm2835_gpio_set(RPI_GPIO_P1_11);
  }
  else if (direction == RIGHT_DIRECTION) //counter clockwise direction
  {
    bcm2835_gpio_set(RPI_GPIO_P1_16);
    bcm2835_gpio_clr(RPI_GPIO_P1_11);
  }
  else //loose brake
  {
    bcm2835_gpio_clr(RPI_GPIO_P1_16);
    bcm2835_gpio_clr(RPI_GPIO_P1_11);
  }
}
void Motor::setSpeed(qint16 pwmValue)
{
  pwmValue = (float)pwmValue/100*maxSpeed;
  if(pwmValue>maxSpeed)
    pwmValue = maxSpeed;
  if(pwmValue<(-1)*maxSpeed)
    pwmValue = (-1)*maxSpeed;

  targetSpeed = pwmValue;
}
