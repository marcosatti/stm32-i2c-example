#include MCU_HEADER
#include "config.h"
#include "abort.h"

I2C_HandleTypeDef I2C_INSTANCE;

void i2c_init(void)
{
    // Initialize the I2C port.
    I2C_INSTANCE.Instance = I2C_PORT;
    I2C_INSTANCE.Init.ClockSpeed = 400000;
    I2C_INSTANCE.Init.DutyCycle = I2C_DUTYCYCLE_2;
    I2C_INSTANCE.Init.OwnAddress1 = 0;
    I2C_INSTANCE.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    I2C_INSTANCE.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    I2C_INSTANCE.Init.OwnAddress2 = 0;
    I2C_INSTANCE.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    I2C_INSTANCE.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&I2C_INSTANCE) != HAL_OK)
    {
        abort();
    }
}

// Callback which is invoked as part of the I2C initialization process - see the HAL_I2C_Init source code.
void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
    if (hi2c->Instance != I2C_PORT) 
    {
        abort();
    }

    // Initialize the GPIO and I2C clocks.
    I2C_GPIO_CLK_ENABLE();
    I2C_CLK_ENABLE();

    // Initialize the AFIO and I2C remap clocks if needed.
#if I2C_USE_REMAP
    I2C_REMAP_CLK_ENABLE();
    I2C_REMAP_ENABLE();
#endif

    // Initialize the I2C GPIO pins.
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = I2C_GPIO_PINS;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(I2C_GPIO_PORT, &GPIO_InitStruct);
}
