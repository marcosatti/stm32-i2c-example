#pragma once

#include "config.h"
#include "i2c.h"

#define SSD1306_I2C_PORT I2C_INSTANCE

// Mirror the screen if needed
// #define SSD1306_MIRROR_VERT
// #define SSD1306_MIRROR_HORIZ

// Set inverse color if needed
// # define SSD1306_INVERSE_COLOR

// Include only needed fonts
#define SSD1306_INCLUDE_FONT_6x8
// #define SSD1306_INCLUDE_FONT_7x10
// #define SSD1306_INCLUDE_FONT_11x18
// #define SSD1306_INCLUDE_FONT_16x26
