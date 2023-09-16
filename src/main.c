/*
 * main.c
 *
 *  Created on: 16 SEP 2023 ð.
 *      Author: andyd
 */

#include <game.h>
#include "stm32f4xx.h"

extern RESULT game_get_neighbors_num(uint8_t x, uint8_t y, uint8_t *result);


int main() {
    RESULT status;
    uint8_t temp_res;

    status = game_get_neighbors_num(39, 36, &temp_res);

    while (1) {

    }

	return 0;
}
