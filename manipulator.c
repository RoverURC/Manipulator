/*
 * manipulator.c
 *
 *  Created on: 2013-10-23
 *      Author: Tomek
 */
/*
 * manipulator.c
 *
 *  Created on: 2013-10-23
 *      Author: Tomek
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "manipulator.h"
#include "PCA9685.h"
#include "I2C.h"

void arm_move(uint8_t address, uint8_t ile_kanalow)
{
	I2C_start();
	I2C_write(address);
	I2C_write(FIRST_CHANNEL);
	for (uint8_t i=0; i<ile_kanalow; i++)
	{
		I2C_write(0x01);
		I2C_write(0x00);
		I2C_write(0b00000000);
		I2C_write(0b00000010);
	}
	I2C_stop();
}

