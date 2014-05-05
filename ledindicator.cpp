#include "ledindicator.h"
#define LED_PIN RPI_GPIO_P1_18
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

void LedIndicator::startBlinking(int ms){
  timer->start(ms);
}
void LedIndicator::stopBlinking(){
  timer->stop();
}

void LedIndicator::blink(){
  if(state == ON){
    state = OFF;
    bcm2835_gpio_clr(LED_PIN);
  }
  else if(state == OFF){
    state = ON;
    bcm2835_gpio_set(LED_PIN);
  }
}
