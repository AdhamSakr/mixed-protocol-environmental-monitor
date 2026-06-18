/*
 * bmp180.h
 *
 *  Created on: Jun 15, 2026
 *      Author: Adham
 */

#ifndef _BMP180_H_
#define _BMP180_H_


#include "stm32f4xx_hal.h"

void BMP180_Init (void);

float BMP180_GetTemperature (void);

float BMP180_GetPressure (int oss);

float BMP180_GetAltitude (int oss);

#endif /* INC_BMP180_H_ */
