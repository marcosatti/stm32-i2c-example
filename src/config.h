#pragma once

#include MCU_HEADER

// Inbuilt LED is used for minimal diagnostics.
#define LED_CLK_ENABLE __HAL_RCC_GPIOC_CLK_ENABLE
#define LED_PORT GPIOC
#define LED_PIN GPIO_PIN_13

// Defaults to I2C1:
//   Remap off:
//     SCL = PIN B6, SDA = PIN B7.
//   Remap on:
//     SCL = PIN B8, SDA = PIN B9.
#define I2C_INSTANCE i2c_handle
#define I2C_PORT I2C1
#define I2C_CLK_ENABLE __HAL_RCC_I2C1_CLK_ENABLE
#define I2C_GPIO_CLK_ENABLE __HAL_RCC_GPIOB_CLK_ENABLE
#define I2C_USE_REMAP 1
#define I2C_REMAP_ENABLE __HAL_AFIO_REMAP_I2C1_ENABLE
#define I2C_REMAP_CLK_ENABLE __HAL_RCC_AFIO_CLK_ENABLE
#define I2C_GPIO_PORT GPIOB
// No remap pins:
//#define I2C_GPIO_PINS (GPIO_PIN_6 | GPIO_PIN_7)
// Remap pins:
#define I2C_GPIO_PINS (GPIO_PIN_8 | GPIO_PIN_9)

#define SSD1306_I2C_ADDR (0x3C << 1)
#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 64

#define SQUARE_DIM_SIZE 8

