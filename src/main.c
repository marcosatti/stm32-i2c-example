#include <stdio.h>
#include MCU_HEADER
#include "config.h"
#include "system.h"
#include "gpio.h"
#include "i2c.h"
#include "ssd1306.h"

void main(void)
{
    hal_init();
    clock_init();
    led_init();
    i2c_init();

    while (HAL_I2C_IsDeviceReady(&I2C_INSTANCE, SSD1306_I2C_ADDR, 1, 100) != HAL_OK) {
        set_led(1);
        HAL_Delay(100);
        set_led(0);
        HAL_Delay(100);
    }

    ssd1306_Init();

    int start_x = 0;
    int start_y = 0;
    while (1)
    {
        ssd1306_Fill(Black);

        ssd1306_SetCursor(0, 0);
        ssd1306_WriteString("Hello world!", Font_6x8, White);

        char string[32];
        sprintf(string, "Drawing @ (%d x %d)", start_x, start_y);
        ssd1306_SetCursor(0, 8);
        ssd1306_WriteString(string, Font_6x8, White);

        for (int y = start_y; y < (start_y + SQUARE_DIM_SIZE); y++) {
            for (int x = start_x; x < (start_x + SQUARE_DIM_SIZE); x++) {
                ssd1306_DrawPixel(x, y, White);
            }
        }

        set_led(1);
        ssd1306_UpdateScreen();
        set_led(0);
        
        start_x += SQUARE_DIM_SIZE;

        if (start_x >= SSD1306_WIDTH) {
            start_x = 0;
            start_y += SQUARE_DIM_SIZE;

            if (start_y >= SSD1306_HEIGHT) {
                start_y = 0;
            }
        }
    }
}
