#include "servodriver.h"

const int axisNumber = 4;
const int servoInit[axisNumber] = { 256, 256, 256, 256};
const int servoMin[axisNumber] = { 256, 140, 256, 256};
const int servoMax[axisNumber] = { 512, 512, 512, 512};

const int servoDumper = 2; //Increase/Decrease actual value by servospeed every timeout

ServoDriver::ServoDriver(quint8 address, QObject *parent) :
  QObject(parent), driver_I2C_address(address)
{
  driverInit();

  actualPWMTable = new int[axisNumber];
  targetPWMTable = new int[axisNumber];
  for(int i=0;i<axisNumber;i++){
    actualPWMTable[i] = servoInit[i];
    targetPWMTable[i] = servoInit[i];
  }

  servoUpdateTimer = new QTimer(this);
  connect(servoUpdateTimer, SIGNAL(timeout()),this,SLOT(updateServo()));
  servoUpdateTimer->start(1);
}

ServoDriver::~ServoDriver(){
  delete[] actualPWMTable;
  delete[] targetPWMTable;
}

void ServoDriver::driverInit(){
    char buf1[2] = {(char)MODE1, (char)0x31};
    char buf2[2] = {(char)PRE_SCALE, (char)104};
    char buf3[2] = {(char)MODE1, (char)0x21};
    char buf4[2] = {(char)MODE1, (char)0xA1};
    char buf5[2] = {(char)MODE2, (char)0x04};

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

//Soft change every serverUpdateTimer timeout()
void ServoDriver::setServoPWM(int index, quint16 value){
  if(index<0 || index>=axisNumber)
    return;

  targetPWMTable[index] = value;
}

void ServoDriver::updateServo(){
  for(int index = 0 ; index<axisNumber; index++){
    if(index>=axisNumber || index<0)
      return;

    if(actualPWMTable[index]>targetPWMTable[index] && actualPWMTable[index]>servoMin[index]){
        actualPWMTable[index] = actualPWMTable[index] - servoDumper;
      if(actualPWMTable[index]<targetPWMTable[index])
        actualPWMTable[index]= targetPWMTable[index];
      if(actualPWMTable[index]<servoMin[index])
        actualPWMTable[index]=servoMin[index];
    }
    if(actualPWMTable[index]<targetPWMTable[index] && actualPWMTable[index]<servoMax[index]){
        actualPWMTable[index] = actualPWMTable[index] + servoDumper;
      if(actualPWMTable[index]>targetPWMTable[index])
        actualPWMTable[index]=targetPWMTable[index];
      if(actualPWMTable[index]>servoMax[index])
        actualPWMTable[index]=servoMax[index];
    }
    if(index == 0){
      servoMove(0, actualPWMTable[0]);
      servoMove(1, servoMax[0]+servoMin[0]-actualPWMTable[0]);
    }
    else{
      servoMove(index+1, actualPWMTable[index]);
    }
  }
}
