/*
 * sh1106.h
 *
 *  Created on: Jun 17, 2026
 *      Author: Adham
 */

#ifndef INC_SH1106_H_
#define INC_SH1106_H_

#include "main.h"
#include "fonts.h"

// Define resolution
#define SH1106_WIDTH  128
#define SH1106_HEIGHT 64

extern SPI_HandleTypeDef hspi1;

// Function prototypes
void SH1106_Init(void);
void SH1106_Clear(void);
void SH1106_Update(void);
void SH1106_DrawChar(uint8_t x, uint8_t y, char c, FontDef_t* Font);
void SH1106_PrintStr(uint8_t x, uint8_t y, char* str, FontDef_t* Font);

#endif /* INC_SH1106_H_ */
