/*
 * cmsis.c
 *
 *  Created on: 24 SEP 2023 ð.
 *      Author: andyd
 */
#include <stm32f4xx.h>
#include "cmsis.h"

/* Sets SYSCLK speed to be set in MHZ
 * input : mhz - SYSCLK speed in MHz
 * output: void
 *  */
void cmsis_set_sysclk_speed(uint32_t mhz, CLOCK_SOURCE sysclk_src) {
    //uint32_t m, n, p;
    CLOCK_SOURCE clk_src;
    //uint32_t reg;

    /* Get current SYSCLK source */
    clk_src = (RCC->CFGR & RCC_CFGR_SWS_Msk) >> RCC_CFGR_SWS_Pos;

    if (((mhz * 1000000) == SystemCoreClock) && (clk_src == sysclk_src)) {
        return;
    }

    /* Get current M, N, P scalers */
    //reg = RCC->PLLCFGR;
    //m = (reg & RCC_PLLCFGR_PLLM_Msk) >> RCC_PLLCFGR_PLLM_Pos;
    //n = (reg & RCC_PLLCFGR_PLLN_Msk) >> RCC_PLLCFGR_PLLN_Pos;
    //p = (reg & RCC_PLLCFGR_PLLP_Msk) >> RCC_PLLCFGR_PLLP_Pos;

    if ((sysclk_src == CLOCK_SOURCE_HSE) && ((mhz * 1000000) == HSE_VALUE)) {
        /* If requested SYSCLK speed is equal to HSE_OSC speed - just use HSE_OSC */
        RCC->CR |= RCC_CR_HSEON;
        while (!(RCC->CR & RCC_CR_HSERDY)) {}
        RCC->CFGR = (RCC->CFGR | RCC_CFGR_SW_0) & (~RCC_CFGR_SW_1);
        while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSE) {}
    } else if ((mhz * 1000000) == HSI_VALUE) {
        RCC->CR |= RCC_CR_HSION;
        while (!(RCC->CR & RCC_CR_HSIRDY)) {}
        RCC->CFGR = RCC->CFGR & (~(RCC_CFGR_SW_0 | RCC_CFGR_SW_1));
        while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI) {}
    } else {
        /* If requested SYSCLK speed is different - use PLL
         *
         * 1) f_vco = f_in / M * N;
         * 2) f_pll = f_vco / P; */
        //uint32_t pll_input_speed;

        cmsis_set_sysclk_speed(HSI_VALUE/1000000, CLOCK_SOURCE_HSI);

    }

    /* Switch to a needed clock */


    /* Update SYSCLK clock speed value */
    SystemCoreClockUpdate();
}
