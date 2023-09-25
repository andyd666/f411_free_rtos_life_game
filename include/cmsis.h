/*
 * cmsis.h
 *
 *  Created on: 24 SEP 2023 ð.
 *      Author: andyd
 */

#ifndef INCLUDE_CMSIS_H_
#define INCLUDE_CMSIS_H_

#include "stm32f4xx.h"

#define PLLM_MAX 63
#define PLLM_MIN 2
#define PLLN_MAX 432
#define PLLN_MIN 50

#define FVCO_MAX 432
#define FVCO_MIN 100

typedef enum {
    CLOCK_SOURCE_HSI            = 0,
    CLOCK_SOURCE_HSE            = 1,
    CLOCK_SOURCE_PLL            = 2,
    CLOCK_SOURCE_NOT_APPLICABLE = 3
}CLOCK_SOURCE;

void cmsis_set_sysclk_speed(uint32_t mhz, CLOCK_SOURCE sysclk_src);

#endif /* INCLUDE_CMSIS_H_ */
