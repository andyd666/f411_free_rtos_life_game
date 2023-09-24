/*
 * cmsis.c
 *
 *  Created on: 24 SEP 2023 ð.
 *      Author: andyd
 */
#include "stm32f4xx.h"
#include "cmsis.h"

/* Sets SYSCLK speed to be set in MHZ
 * input : mhz - SYSCLK speed in MHz
 * output: void
 *  */
void cmsis_set_sysclk_speed(uint32_t mhz) {
    uint32_t m, n, p;
    CLOCK_SOURCE clk_src;
    uint32_t reg;

    /* Get current SYSCLK source */
    clk_src = (RCC->CFGR & RCC_CFGR_SWS_Msk) >> RCC_CFGR_SWS_Pos;

    if ((mhz * 1000000) == SystemCoreClock) {
#if !CMSIS_USE_HSI
        if (clk_src != CLOCK_SOURCE_HSI)
#endif /* !CMSIS_USE_HSI */
            return;
    }

    /* Get current M, N, P scalers */
    reg = RCC->PLLCFGR;
    m = (reg & RCC_PLLCFGR_PLLM_Msk) >> RCC_PLLCFGR_PLLM_Pos;
    n = (reg & RCC_PLLCFGR_PLLN_Msk) >> RCC_PLLCFGR_PLLN_Pos;
    p = (reg & RCC_PLLCFGR_PLLP_Msk) >> RCC_PLLCFGR_PLLP_Pos;

    if ((mhz * 1000000) == HSE_VALUE) {
        /* If requested SYSCLK speed is equal to HSE_OSC speed - just use HSE_OSC */
        if (clk_src == CLOCK_SOURCE_HSE) {
            return;
        }
#if CMSIS_USE_HSI
    } else if ((mhz * 1000000) == HSI_VALUE) {
        if (clk_src == CLOCK_SOURCE_HSI) {
            return;
        }
#endif /* CLOCK_SOURCE_HSI */
    } else {
        /* If requested SYSCLK speed is different - use PLL
         * NOTE: Don't use HSI, because it is less stable than HSE
         *
         * 1) f_vco = f_in / M * N;
         * 2) f_pll = f_vco / P; */

        uint32_t pll_input_speed;

        if ((clk_src == CLOCK_SOURCE_PLL)
#if !CMSIS_USE_HSI
                && (RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC)
#endif /* !CMSIS_USE_HSI */
                && ((mhz * 1000000) == SystemCoreClock)
        ) {
            return;
        }

#if CMSIS_USE_HSI
        pll_input_speed = (RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) ? HSE_VALUE : HSI_VALUE;
#else
        pll_input_speed = HSE_VALUE;
        RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC;
#endif /*  */

        if ((m >= PLLM_MIN) && (m <= PLLM_MAX) &&
           ((pll_input_speed / m) >= 1000000) && ((pll_input_speed / m) <= 2000000) &&
           (n >= PLLN_MIN) && (n <= PLLN_MAX)) {

        }


    }

    /* Switch to a needed clock */


    /* Update SYSCLK clock speed value */
    SystemCoreClockUpdate();
}
