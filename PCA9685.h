/*
 * PCA9685.h
 *
 *  Created on: 2013-10-09
 *      Author: Tomek
 */

#ifndef PCA9685_H_
#define PCA9685_H_

/* General registers */
#define PCA9685_WRITE	0x80        // I2C address for PCA9865 with all inputs at zero - MOSI transfer
#define PCA9685_READ	0x81        // I2C address for PCA9865 with all inputs at zero - MISO transfer (when it comes to PCA9685 driver, we're not going to use this address)
#define Reset   		0x01        // Reset the device
#define FIRST_CHANNEL   0x06        // location for start of LED0 registers
#define MAX_CHANNELS 	15          // number of LEDS to light 15 max

/* Control Registers */
#define MODE1 			0x00
#define MODE2			0x01
#define SUBADR1			0x02
#define SUBADR2			0x03
#define SUBADR3			0x04
#define ALLCALLADR		0x05
#define LED0_ON_L		0x06
#define LED0_ON_H		0x07
#define LED0_OFF_L		0x08
#define LED0_OFF_H 		0x09
#define LED1_ON_L		0x0A
#define LED1_ON_H		0x0B
#define LED1_OFF_L		0x0C
#define LED1_OFF_H 		0x0D
#define LED2_ON_L		0x0E
#define LED2_ON_H		0x0F
#define LED2_OFF_L		0x10
#define LED2_OFF_H 		0x11
#define LED3_ON_L		0x12
#define LED3_ON_H		0x13
#define LED3_OFF_L		0x14
#define LED3_OFF_H 		0x15
#define LED4_ON_L		0x16
#define LED4_ON_H		0x17
#define LED4_OFF_L		0x18
#define LED4_OFF_H 		0x19
#define LED5_ON_L		0x1A
#define LED5_ON_H		0x1B
#define LED5_OFF_L		0x1C
#define LED5_OFF_H 		0x1D
#define LED6_ON_L		0x1E
#define LED6_ON_H		0x1F
#define LED6_OFF_L		0x20
#define LED6_OFF_H 		0x21
#define LED7_ON_L		0x22
#define LED7_ON_H		0x23
#define LED7_OFF_L		0x24
#define LED7_OFF_H 		0x25
#define LED8_ON_L		0x26
#define LED8_ON_H		0x27
#define LED8_OFF_L		0x28
#define LED8_OFF_H 		0x29
#define LED9_ON_L		0x2A
#define LED9_ON_H		0x2B
#define LED9_OFF_L		0x2C
#define LED9_OFF_H 		0x2D
#define LED10_ON_L		0x2E
#define LED10_ON_H		0x2F
#define LED10_OFF_L		0x30
#define LED10_OFF_H 	0x31
#define LED11_ON_L		0x32
#define LED11_ON_H		0x33
#define LED11_OFF_L		0x34
#define LED11_OFF_H 	0x35
#define LED12_ON_L		0x36
#define LED12_ON_H		0x37
#define LED12_OFF_L		0x38
#define LED12_OFF_H 	0x39
#define LED13_ON_L		0x3A
#define LED13_ON_H		0x3B
#define LED13_OFF_L		0x3C
#define LED13_OFF_H 	0x3D
#define LED14_ON_L		0x3E
#define LED14_ON_H		0x3F
#define LED14_OFF_L		0x40
#define LED14_OFF_H 	0x41
#define LED15_ON_L		0x42
#define LED15_ON_H		0x43
#define LED15_OFF_L		0x44
#define LED15_OFF_H 	0x45
//... reserved
#define ALL_LED_ON_L 	0xFA
#define ALL_LED_ON_H 	0xFB
#define ALL_LED_OFF_L 	0xFC
#define ALL_LED_OFF_H 	0xFD
#define PRE_SCALE 		0xFE
#define TestMode 		0xFF //don't you event try this mode! That is the register of DOOM.

/* Mode Register 1 - MODE1 - bits */
#define MODE1_RESTART 0x10000000	//default at 0 [0-restart disabled, 1-restart enabled]
#define MODE1_EXTCLK  0x01000000	//default at 0 [0-use internal clock, 1-use EXTCLK pin clock]
#define MODE1_AI      0x00100000	//default at 0 [0-register Auto-Increment disabled, 1-register Auto-Increment enabled]
#define MODE1_SLEEP   0x00010000	//default at 1 [0-normal mode, 1-low power mode. oscillator is off]
#define MODE1_SUB1    0x00001000	//default at 0 [0-does not respond to SUB1 addresss, 1-responds to SUB1 addresss]
#define MODE1_SUB2    0x00000100	//default at 0 [0-does not respond to SUB2 addresss, 1-responds to SUB2 addresss]
#define MODE1_SUB3    0x00000010	//default at 0 [0-does not respond to SUB3 addresss, 1-responds to SUB3 addresss]
#define MODE1_ALLCALL 0x00000001	//default at 1 [0-does not respond to LED All Call addresss, 1-responds to LED All Call addresss]

/* Mode Register 2 - MODE2 - bits */
#define MODE2_INVRT   0x00010000	//default at 0 [0-output logic state not inverted, 1-output logic state invertet]
#define MODE2_OCH     0x00001000	//default at 0 [0-output change on STOP command, 1-output change on ACK]
#define MODE2_OUTDRV  0x00000100	//default at 1 [0-outputs are configured with an open-drain structure, 1-outputs are configured with a totem pole structure]
#define MODE2_OUTNE1  0x00000010	/*default at 00 [00- if OE=HIGH, then outputs not enabled & LEDn=LOW]   */
#define MODE2_OUTNE0  0x00000001	/*				[01- if OE=HIGH, then outputs not enabled & LEDn=HIGH in case OUTDRV=1 | LEDn=HiZ in case OUTDRV=0] */
									/*				[1X- if OE-HIGH, then LEDn=HiZ] */

/* LEDX_ON_H bits */
#define PCA9685_LED_ON 0x10

/* LEDX_OFF_H bits */
#define PCA9685_LED_OFF 0x10


/* functions */
void pca9685_init(uint8_t address);

void pca9685_send_all(uint8_t address);

void pca9685_send(uint8_t address,  uint16_t value, uint8_t led);

void pca9685_brightness(uint8_t address, uint8_t percent, uint8_t led);

void PCA9685AllLedOff(uint8_t address);

void servo_left(uint8_t address, uint8_t channel);

void servo_neutral(uint8_t address, uint8_t channel);

void servo_right(uint8_t address, uint8_t channel);

void servo_pwm(uint8_t address, uint8_t channel, uint16_t pwm);

#endif /* PCA9685_H_ */

