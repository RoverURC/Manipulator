/*
 * PCA9685.c
 *
 *  Created on: 2013-10-09
 *      Author: Tomek
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "PCA9685.h"
#include "I2C.h"

void pca9685_init(uint8_t address)
{
    I2C_start();                // Start
    I2C_write(PCA9685_WRITE);   // Slave address
    I2C_write(MODE1);           // Mode 1 address
    I2C_write(0b00110001);      // Setting mode to sleep so we can change the default PWM frequency
    I2C_stop();                 // Stop
	_delay_us(500);             // Required at least 50 us delay
    I2C_start();                // Start
    I2C_write(address);         // Slave address
    I2C_write(PRE_SCALE);       // PWM frequency PRE_SCALE address
	I2C_write(0b01100001);      // [osc_clk/(4096*update_rate)]-1 // [25000000/(4096*50Hz)]-1= ~121 - should output 50Hz but it does 66Hz piece of shit
    I2C_stop();                 // Stop
	_delay_us(500);             // delay at least 500 us
    I2C_start();                // Start
	I2C_write(PCA9685_WRITE);   // Slave address
    I2C_write(MODE1);           // Mode 1 register address
	I2C_write(0b10100001);      // Set to our prefered mode[ Reset, INT_CLK, Auto-Increment, Normal Mode]
    I2C_stop();                 // Stop
	_delay_us(500);             // delay at least 500 us
    I2C_start();                // Start
	I2C_write(PCA9685_WRITE);   // Slave Address
    I2C_write(MODE2);           // Mode2 register address
    I2C_write(0b00000100);      // Set to our prefered mode[Output logic state not inverted, Outputs change on STOP,
    I2C_stop();                 // totem pole structure, When OE = 1 (output drivers not enabled), LEDn = 0]
    _delay_ms(500);
}

uint16_t PWMData[16]= {
    //MSB            LSB
    0b111111111111,    // Channel 0
    0b100000000000,    // Channel 1
    0b011111111111,    // Channel 2
    0b000000000000,    // Channel 3
    0b000001011010,    // Channel 4
    0b000000000000,    // Channel 5
    0b000000000000,    // Channel 6
    0b000000000000,    // Channel 7
    0b100000000000,    // Channel 8
    0b100000000000,    // Channel 9
    0b100000000000,    // Channel 10
    0b100000000000,    // Channel 11
    0b100000000000,    // Channel 12
    0b100000000000,    // Channel 13
    0b100000000000,    // Channel 14
    0b100000000000     // Channel 15
};


void pca9685_send_all(uint8_t address)
{
    uint8_t i = 0;                  // temp regiter for channel
    uint16_t pwm;                   // temp register for PWM
    for(i=0; i<=MAX_CHANNELS; i++)  // cycle thru all 16 LED
    {
    	I2C_start();            // Start
    	I2C_write(address);     // write to selected pca9685
    	I2C_write(FIRST_CHANNEL + 4 * i);// start from LED0 address, each pwm constructed from
    	I2C_write(0x01);        // 4 12bit register, LED_ON_L, starts at step 0 out of 4095(PWM 12bit resolution)
    	I2C_write(0x01);        // LED_ON_H
        pwm = PWMData[i];       // update selected LED data in the array
        I2C_write(pwm);         // LED_OFF_L
        pwm = PWMData[i]>>8;    // updtae selected LED data in the array
        I2C_write(pwm);         // LED_OFF_H
        I2C_stop();             // Stop
    }
}

void pca9685_send(uint8_t address,  uint16_t value, uint8_t led)
{
	uint8_t pwm;                    // temp variable for PWM
    if(value > 4095) value = 4095;  // cant be larger than 4095
    if(led > 15) led = MAX_CHANNELS;//***** need to implement to selecet next pcs9685
    I2C_start();                	// Start
    I2C_write(address);         	// Address of selected pca9685
    I2C_write(FIRST_CHANNEL + 4 * led);  	// select slected LED address
    I2C_write(0x01);           		// LED_ON_L
    I2C_write(0x01);            	// LED_ON_H
    pwm = value;                	// PWM value lo byte
    I2C_write(pwm);             	// LED_OFF_L
    pwm = value>>8;             	// pwm 16 bit long, now shift upper 8 to lower 8
    I2C_write(pwm);             	// LED_OFF_H
    I2C_stop();                 	// STop
}


void pca9685_brightness(uint8_t address, uint8_t percent, uint8_t led)
{
/********************************************/
/* Calculate the register values for a      */
/* given percentage and dupdate pca9685     */
/* Input:[address of the chip where LED is_ */
/* percent of PWM on period 0%to100%      _ */
/* LED to set brightness 0to15]             */
/* Output:[void]                            */
/********************************************/
	//uint_16 x;                             // temp variable
    float off;                          // temp variable
    const float onePercent = 40.96;
    if (percent < 1) {                  // if % less than 1 than LED OFF
    PWMData[led] = PCA9685_LED_OFF>>8;  // update data in array in case we use update all LED next
    pca9685_send(address,0,led);        // update selected LED
    return;                             // return from function
    }
    if (percent >= 100) {               // if % greather than 100 than LED ON
    PWMData[led] = PCA9685_LED_ON>>8;   // update data in array
    pca9685_send(address,4095,led);     // update selected LED
    return;                             // return from function
    }
    off = onePercent * percent;         // different aproach with float need to check if code faster than int32 way ?
//    off = (int32)4096 * percent;      // calculate percent (max*percent)/100
//    off = off / 100;                  // ex (4096*50%)=204800/100=2048
//    x = make16(off>>8,off);           // make 16 of 32 ?! why.. dont care at this time
    PWMData[led] = off;                 // update datat array in case we update all LED next
    pca9685_send(address,off,led);      // send it to pca9685
}
void PCA9685AllLedOff(uint8_t address)
{
	I2C_start();                        // Start
	I2C_write(address);                 // select pca9685
	I2C_write(ALL_LED_OFF_L);       	// AllLED Off regiter
	I2C_write(0b00000000);//??          // data
	I2C_write(0b00010000);//??          // data
	I2C_stop();                         // Stop
}

void servo_left(uint8_t address, uint8_t channel)
{
	I2C_start();
	I2C_write(address);
	I2C_write(FIRST_CHANNEL + 4*channel);
	I2C_write(0x01);
	I2C_write(0x00);
	I2C_write(0b01000000);
	I2C_write(0b00000000);
	I2C_stop();
}

void servo_neutral(uint8_t address, uint8_t channel)
{
	I2C_start();
	I2C_write(address);
	I2C_write(FIRST_CHANNEL + 4*channel);
	I2C_write(0x01);
	I2C_write(0x00);
	I2C_write(0b00000000);
	I2C_write(0b00000001);
	I2C_stop();
}

void servo_right(uint8_t address, uint8_t channel)
{
	I2C_start();
	I2C_write(address);
	I2C_write(FIRST_CHANNEL + 4*channel);
	I2C_write(0x01);
	I2C_write(0x00);
	I2C_write(0b00000000);
	I2C_write(0b00000010);
	I2C_stop();
}
void servo_pwm(uint8_t address, uint8_t channel, uint16_t pwm)
{
	I2C_start();
	I2C_write(address);
	I2C_write(FIRST_CHANNEL + 4*channel);
	I2C_write(0x01);
	I2C_write(0x00);

    uint16_t temp_pwm = pwm;
    I2C_write(temp_pwm);
    temp_pwm = pwm>>8;
    I2C_write(temp_pwm);
	I2C_stop();
}

