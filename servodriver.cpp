#include "servodriver.h"
#include <QDebug>
#include <iostream>

ServoDriver::ServoDriver(quint8 address, QObject *parent) :
  QObject(parent), driver_I2C_address(address)
{
    driverInit();
}

ServoDriver::~ServoDriver(){

}

void ServoDriver::driverInit(){
    char buf1[2] = {(char)MODE1, (char)0x31};
    char buf2[2] = {(char)PRE_SCALE, (char)122};
    char buf3[2] = {(char)MODE1, (char)0x21};
    char buf4[2] = {(char)MODE1, (char)0xA1};
    char buf5[2] = {(char)MODE2, (char)0x04};


    if(bcm2835_init()) qDebug()<<"GOOD" ; //if error initializing bcm2835 -> exiting program
    bcm2835_i2c_begin(); //i2c start
    bcm2835_delay(1);//ms
    bcm2835_i2c_set_baudrate(100000);
    bcm2835_delay(1);//ms
    bcm2835_i2c_setSlaveAddress(driver_I2C_address);
    printf("address = %d \n", driver_I2C_address);
    bcm2835_delay(1);//ms
    printf("running driver_init()...\n");
    bcm2835_i2c_write(buf1, 2);
    bcm2835_delay(1);//ms
    bcm2835_i2c_write(buf2, 2);
    bcm2835_delay(1);//ms
    bcm2835_i2c_write(buf3, 2);
    bcm2835_delay(1);//ms
    bcm2835_i2c_write(buf4, 2);
    bcm2835_delay(1);//ms
    bcm2835_i2c_write(buf5, 2);
    bcm2835_i2c_end();
}

void ServoDriver::servoMove(quint8 channel, quint16 inputDutyCycle){

  //Format frame
  char *ptr = reinterpret_cast<char *> (&inputDutyCycle);
  quint8 low = *ptr;
  ptr++;
  quint8 high = *ptr;

  //12 Bit servo driver
  if(high >15){
    high = 15;
    low = 255;
  }
  char bufChannel[5] = {(char)(6+channel*4), (char)0x00, (char)0x00,
                        (char)low, (char)high};

  bcm2835_i2c_begin();
  bcm2835_i2c_write(bufChannel, 5);
  bcm2835_i2c_end();
}
