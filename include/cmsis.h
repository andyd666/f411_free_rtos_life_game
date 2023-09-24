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
#define PLLP_MIN 2
#define PLLP_MAX 8

#define PLLM_DEFAULT 16
#define PLLN_DEFAULT 192
#define PLLP_DEFAULT 2


#define FVCO_INPUT_MAX 2000000
#define FVCO_INPUT_MIN 1000000
#define FVCO_OUTPUT_MAX 432000000
#define FVCO_OUTPUT_MIN 100000000
#define FPLL_MAX 100000000

#define ABS_DIFF(x, y) ((x) < (y) ? (y) - (x) : (x) - (y))

typedef enum {
    CLOCK_SOURCE_HSI            = 0,
    CLOCK_SOURCE_HSE            = 1,
    CLOCK_SOURCE_PLL            = 2,
    CLOCK_SOURCE_NOT_APPLICABLE = 3
}CLOCK_SOURCE;

void cmsis_set_sysclk_speed(uint32_t mhz, CLOCK_SOURCE sysclk_src);

#endif /* INCLUDE_CMSIS_H_ */
