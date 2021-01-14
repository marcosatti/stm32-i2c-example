#include MCU_HEADER
#include "system.h"
#include "abort.h"

void hal_init(void)
{
    // Initialize the HAL library, required before anything else.
    if (HAL_Init() != HAL_OK)
    {
        abort();
    }
}

void clock_init(void)
{
    // Initialize the PLL clock source (72 MHz). STM32F103C6 includes an 8 Mhz external oscillator.
    RCC_OscInitTypeDef RCC_OscInitStruct_PLL;
    RCC_OscInitStruct_PLL.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct_PLL.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct_PLL.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct_PLL.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct_PLL.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct_PLL.PLL.PLLMUL = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct_PLL) != HAL_OK)
    {
        abort();
    }

    // Initialize the system clock.
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        abort();
    }

    // Set the systick source to HCLK, enabling the systick interrupt and triggering every 1ms.
    if (HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000) != HAL_OK)
    {
        abort();
    }
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

    // Turn off the HSI as it's no longer needed (on at boot). Saves power.
    // Must be done after setting the systick source, just in case.
    RCC_OscInitTypeDef RCC_OscInitStruct_HSI;
    RCC_OscInitStruct_HSI.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct_HSI.HSIState = RCC_HSI_OFF;
    RCC_OscInitStruct_HSI.PLL.PLLState = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct_HSI) != HAL_OK)
    {
        abort();
    }
}
