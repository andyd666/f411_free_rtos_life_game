/*
 * cmsis.c
 *
 *  Created on: 24 SEP 2023 ð.
 *      Author: andyd
 */
#include <stm32f4xx.h>
#include "cmsis.h"

static void cmsis_set_flash_wait_states(uint32_t freq_mhz);

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

    if ((mhz * 1000000) > SystemCoreClock) {
        cmsis_set_flash_wait_states(mhz);
    }

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
         * 1) f_vco = (f_in / M) * N;
         * 2) f_pll = f_vco / P; */
        uint32_t pll_input_speed = (sysclk_src == CLOCK_SOURCE_HSE) ? HSE_VALUE : HSI_VALUE;
        uint32_t m, n, p;
        uint32_t f_vco, f_pll = 0;
        uint32_t m_closest = PLLM_MIN, n_closest = PLLN_MIN, p_closest = PLLP_MIN;
        uint32_t f_pll_closest = ((pll_input_speed / m_closest) * n_closest) / p_closest;
        uint32_t freq_diff_closest = ABS_DIFF(mhz*1000000, f_pll_closest);
        uint32_t mnp_found = 0;
        uint32_t m_range[2] = {pll_input_speed / FVCO_INPUT_MAX + (pll_input_speed % FVCO_INPUT_MAX ? 1 : 0),
                               pll_input_speed / FVCO_INPUT_MIN + (pll_input_speed % FVCO_INPUT_MIN ? 1 : 0)};


        cmsis_set_sysclk_speed(HSI_VALUE/1000000, CLOCK_SOURCE_HSI);

        RCC->CR &= ~RCC_CR_PLLON;
        PWR->CR |= PWR_CR_VOS_0 | PWR_CR_VOS_1; // Voltage Output Selection. Always set to 100 MHz (Maximum)

        for (m = m_range[0]; m <= m_range[1]; m++) {
            for (n = (FVCO_OUTPUT_MIN / pll_input_speed) * m; n <= PLLN_MAX; n++) {
                for (p = PLLP_MIN; p <= PLLP_MAX; p += 2) {
                    f_vco = (pll_input_speed / m) * n;

                    if ((f_vco < FVCO_OUTPUT_MIN) || (f_vco > FVCO_OUTPUT_MAX)) {
                        break;
                    }

                    f_pll = f_vco / p;

                    if (f_pll > FPLL_MAX) {
                        continue;
                    }

                    if (ABS_DIFF(mhz*1000000, f_pll) == 0) {
                        mnp_found = 1;
                        break;
                    } else if ((f_pll <= FPLL_MAX) && (ABS_DIFF(mhz*1000000, f_pll) < freq_diff_closest)) {
                        freq_diff_closest = ABS_DIFF(mhz*1000000, f_pll);
                        f_pll_closest = f_pll;
                        m_closest = m;
                        n_closest = n;
                        p_closest = p;
                    }
                    break;
                }
                if (f_vco < FVCO_OUTPUT_MIN) {
                    continue;
                } else if (f_vco > FVCO_OUTPUT_MAX) {
                    break;
                }
                if (mnp_found) break;
            }
            if (mnp_found) break;
        }

        if (!mnp_found) {
            m = m_closest;
            n = n_closest;
            p = p_closest;
        }
        p = (p >> 1) - 1;

        if (pll_input_speed == HSI_VALUE) {
            /* HSI is always ON */
            RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLSRC;
        } else {
            RCC->CR |= RCC_CR_HSEON;
            while (!(RCC->CR & RCC_CR_HSERDY)) {}
            RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC;
        }

        RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLM | RCC_PLLCFGR_PLLN | RCC_PLLCFGR_PLLP);
        RCC->PLLCFGR |= (m << RCC_PLLCFGR_PLLM_Pos) & RCC_PLLCFGR_PLLM_Msk;
        RCC->PLLCFGR |= (n << RCC_PLLCFGR_PLLN_Pos) & RCC_PLLCFGR_PLLN_Msk;
        RCC->PLLCFGR |= (p << RCC_PLLCFGR_PLLP_Pos) & RCC_PLLCFGR_PLLP_Msk;

        RCC->CR |= RCC_CR_PLLON;
        while (!(RCC->CR & RCC_CR_PLLRDY)) {}
        RCC->CFGR = (RCC->CFGR | RCC_CFGR_SW_1) & (~RCC_CFGR_SW_0);
        while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) {}
    }

    /* Switch to a needed clock */


    /* Update SYSCLK clock speed value */
    SystemCoreClockUpdate();
    if ((mhz * 1000000) < SystemCoreClock) {
        cmsis_set_flash_wait_states(mhz);
    }
}


static void cmsis_set_flash_wait_states(uint32_t freq_mhz) {
    /* Voltage if ~3.3 Volts */
    uint32_t ws = FLASH_ACR_LATENCY_3WS;
    if (freq_mhz <= 30) {
        ws = FLASH_ACR_LATENCY_0WS;
    } else if (freq_mhz <= 64) {
        ws = FLASH_ACR_LATENCY_1WS;
    } else if (freq_mhz <= 90) {
        ws = FLASH_ACR_LATENCY_2WS;
    } else if (freq_mhz <= 100) {
        ws = FLASH_ACR_LATENCY_3WS;
    }
    FLASH->ACR |= FLASH_ACR_LATENCY; /* Set maximum latency */
    FLASH->ACR &= (ws | (~FLASH_ACR_LATENCY));
}
