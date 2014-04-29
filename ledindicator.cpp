#include "ledindicator.h"
#define LED_PIN RPI_GPIO_P1_16
#define ON true
#define OFF false
LedIndicator::LedIndicator(QObject *parent) :
  QObject(parent)
{
  state = OFF;
  bcm2835_gpio_fsel(LED_PIN, BCM2835_GPIO_FSEL_OUTP);

  timer = new QTimer(this);
  connect(timer,SIGNAL(timeout()),this,SLOT(blink()));
}

LedIndicator::startBlinking(int ms){
  timer->start(ms);
}
LedIndicator::stopBlinking(){
  timer->stop();
}

LedIndicator::blink(){
  if(state == ON){
    state = OFF;
    bcm2835_gpio_clr(RPI_GPIO_P1_16);
  }
  if(state == OFF){
    state = ON;
    bcm2835_gpio_set(RPI_GPIO_P1_11);
  }
}
