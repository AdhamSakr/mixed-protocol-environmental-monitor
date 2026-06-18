/*
 * sh1106.c
 *
 *  Created on: Jun 17, 2026
 *      Author: Adham
 */

#include "sh1106.h"
#include <string.h>


// Local screen video buffer
static uint8_t OLED_Buffer[SH1106_WIDTH * SH1106_HEIGHT / 8];

// Helper macro to drive Control Signals
#define OLED_DC_Cmd()   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET)
#define OLED_DC_Data()  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET)

static void SH1106_WriteCmd(uint8_t cmd) {
    OLED_DC_Cmd();
    HAL_SPI_Transmit(&hspi1, &cmd, 1, 10);
}

void SH1106_Init(void) {
    // 1. Hardware Reset Cycle
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
    HAL_Delay(50);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
    HAL_Delay(50);

    // 2. Main SH1106 Init Command Sequence
    SH1106_WriteCmd(0xAE); // Turn display off
    SH1106_WriteCmd(0xD5); // Set clock divide ratio
    SH1106_WriteCmd(0x80);
    SH1106_WriteCmd(0xA8); // Set multiplex ratio
    SH1106_WriteCmd(0x3F); // 1/64 duty
    SH1106_WriteCmd(0xD3); // Set display offset
    SH1106_WriteCmd(0x00); // No offset
    SH1106_WriteCmd(0x40); // Set start line to 0
    SH1106_WriteCmd(0xAD); // Charge pump setup
    SH1106_WriteCmd(0x8B); // Enable internal DC-DC
    SH1106_WriteCmd(0xA1); // Segment remap (flip horizontally)
    SH1106_WriteCmd(0xC8); // COM output scan direction (flip vertically)
    SH1106_WriteCmd(0xDA); // COM pins hardware configuration
    SH1106_WriteCmd(0x12);
    SH1106_WriteCmd(0x81); // Contrast control
    SH1106_WriteCmd(0x80); // Medium brightness
    SH1106_WriteCmd(0xD9); // Pre-charge period
    SH1106_WriteCmd(0x22);
    SH1106_WriteCmd(0xDB); // VCOMH deselect level
    SH1106_WriteCmd(0x40);
    SH1106_WriteCmd(0xA4); // Entire display ON resume
    SH1106_WriteCmd(0xA6); // Normal display mode (not inverted)
    SH1106_WriteCmd(0xAF); // Turn display ON

    SH1106_Clear();
    SH1106_Update();
}

void SH1106_Clear(void) {
    memset(OLED_Buffer, 0, sizeof(OLED_Buffer));
}

void SH1106_Update(void) {
    for (uint8_t page = 0; page < 8; page++) {
        SH1106_WriteCmd(0xB0 + page); // Set page address

        // SH1106 controller RAM defaults to 132 columns.
        // Most 128-width panel modules match columns 2 to 129.
        uint8_t col_start = 2;
        SH1106_WriteCmd(0x00 | (col_start & 0x0F));        // Lower column start address
        SH1106_WriteCmd(0x10 | ((col_start >> 4) & 0x0F)); // Upper column start address

        OLED_DC_Data();
        HAL_SPI_Transmit(&hspi1, &OLED_Buffer[page * SH1106_WIDTH], SH1106_WIDTH, 10);
    }
}

void SH1106_DrawChar(uint8_t x, uint8_t y, char c, FontDef_t* Font) {
    // Check boundaries using the dynamically passed Font structure
    if (x > (SH1106_WIDTH - Font->FontWidth) || y > (SH1106_HEIGHT - Font->FontHeight)) {
        return;
    }

    // Filter unsupported characters (Your font files typically start at space ASCII 32)
    if (c < 32 || c > 126) {
        c = ' ';
    }

    // Calculate index location inside your 16-bit array
    uint32_t font_index = (c - 32) * Font->FontHeight;

    for (uint8_t i = 0; i < Font->FontHeight; i++) {
        // Read row bitmask data
        uint16_t line = Font->data[font_index + i];

        for (uint8_t j = 0; j < Font->FontWidth; j++) {
            // Your fonts are MSB-aligned, check if pixel is active
            if (line & (0x8000 >> j)) {
                uint16_t idx = x + j + ((y + i) / 8) * SH1106_WIDTH;
                if (idx < sizeof(OLED_Buffer)) {
                    OLED_Buffer[idx] |= (1 << ((y + i) % 8));
                }
            }
        }
    }
}

void SH1106_PrintStr(uint8_t x, uint8_t y, char* str, FontDef_t* Font) {
    while (*str) {
        SH1106_DrawChar(x, y, *str, Font);
        x += Font->FontWidth; // Automatically space characters using font width
        str++;
    }
}

