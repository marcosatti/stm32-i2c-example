#include MCU_HEADER
#include "config.h"
#include "gpio.h"

static int led_initialized = 0;

void led_init(void)
{
    // Enable the led GPIO clock and pin.
    LED_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = LED_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);
    
    led_initialized = 1;

    set_led(0);
}

void set_led(int state)
{
    if (led_initialized)
    {
        HAL_GPIO_WritePin(LED_PORT, LED_PIN, state ? GPIO_PIN_RESET : GPIO_PIN_SET);
    }
}

int is_led_initialized(void) {
    return led_initialized;
}
