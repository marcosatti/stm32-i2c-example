#include MCU_HEADER
#include "abort.h"
#include "gpio.h"

void abort(void)
{
    if (is_led_initialized())
    {
        abort_led(1000);
    }

    while (1)
        ;
}

void abort_led(uint32_t delay)
{
    while (1)
    {
        set_led(1);
        HAL_Delay(delay);
        set_led(0);
        HAL_Delay(delay);
    }
}
