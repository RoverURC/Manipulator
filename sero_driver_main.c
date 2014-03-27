/*
 * main.c
 *
/*
 * main.c
 *
 *  Created on: 2013-10-09
 *      Author: Tomek
 */

#include <avr/io.h>
#include <util/delay.h>

#include "I2C.h"
#include "PCA9685.h"

int main (void)
{
	I2C_init();
	i2cSetBitrate(100);
	pca9685_init(PCA9685_WRITE);
	/*just for calibration purpose*/
	uint16_t pwm_data[] = {
			//0b0000000000101001, //decimal=41 -> 1% PWM
			0b0000000001010010,   //decimal=82 -> 2% PWM
			0b0000000001111011,   //decimal=123 -> 3% PWM
			0b0000000010100111,   //decimal=167 -> 4% PWM
			0b0000000011001101,   //decimal=205 -> 5% PWM
			0b0000000011110110,   //decimal=246 -> 6% PWM
			0b0000000100011111,   //decimal=287 -> 7% PWM
			0b0000000101001000,   //decimal=328 -> 8% PWM
			0b0000000101101100,   //decimal=365 -> 9% PWM
			0b0000000110011010,   //decimal=410 -> 10% PWM
			0b0000000111000011,   //decimal=451 -> 11% PWM
			0b0000000111101100,   //decimal=492 -> 12% PWM
			0b0000001000010100,   //decimal=532 -> 13% PWM
			0b0000001000111101,   //decimal=573 -> 14% PWM
			0b0000001001100110,   //decimal=614 -> 15% PWM
			//0b0000001010001111,   //decimal=655 -> 16% PWM
			//0b0000001010111000,   //decimal=696 -> 17% PWM
			//0b0000001011100001,   //decimal=737 -> 18% PWM
			//0b0000001100001010,   //decimal=778 -> 19% PWM
			//0b0000001100110011,   //decimal=819 -> 20% PWM
	};

	while(1)
	{

		/*servo_neutral(PCA9685_WRITE, 3);
		_delay_ms(500);
		servo_left(PCA9685_WRITE, 3);
		_delay_ms(500);
		servo_neutral(PCA9685_WRITE, 3);
		_delay_ms(500);
		servo_right(PCA9685_WRITE, 3);
		_delay_ms(500);*/

		I2C_start();
		I2C_write(PCA9685_WRITE);
		I2C_write(FIRST_CHANNEL);
		I2C_write(0x01);			//channel 0
		I2C_write(0x00);
		I2C_write(pwm_data[8]);
		I2C_write(pwm_data[8]>>8);
		I2C_write(0x01);			//channel 1
		I2C_write(0x00);
		I2C_write(pwm_data[11]);
		I2C_write(pwm_data[11]>>8);
		I2C_write(0x01);			//channel 2
		I2C_write(0x00);
		I2C_write(pwm_data[1]);
		I2C_write(pwm_data[1]>>8);
		I2C_stop();
		_delay_ms(2000);
		I2C_start();
		I2C_write(PCA9685_WRITE);
		I2C_write(FIRST_CHANNEL);
		I2C_write(0x01);			//channel 0
		I2C_write(0x00);
		I2C_write(pwm_data[1]);
		I2C_write(pwm_data[1]>>8);
		I2C_write(0x01);			//channel 1
		I2C_write(0x00);
		I2C_write(pwm_data[3]);
		I2C_write(pwm_data[3]>>8);
		I2C_write(0x01);			//channel 2
		I2C_write(0x00);
		I2C_write(pwm_data[1]);
		I2C_write(pwm_data[1]>>8);
		I2C_stop();
		_delay_ms(2000);

	}
}

