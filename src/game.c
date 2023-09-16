/*
 * game.c
 *
 *  Created on: 16 SEP 2023 ð.
 *      Author: andyd
 */

#include "stm32f4xx.h"
#include "game.h"


static uint8_t life_field[GAME_FIELD_ARRAY_SIZE_X][GAME_FIELD_ARRAY_SIZE_Y];

uint8_t game_get_cell(uint8_t x, uint8_t y) {
	return !!(life_field[(x)>>3][(y)] & (1 << ((x) % 8)));
}


RESULT game_get_neighbors_num(uint8_t x, uint8_t y, uint8_t *result) {
    if ((x > GAME_FIELD_REAL_SIZE_X) || (y > GAME_FIELD_REAL_SIZE_Y)) {
        return BAD;
    }

    *result = 0;

    if (y == 0) {
        *result += game_get_cell(x, y+1);
        if (x == 0) {
            *result += game_get_cell(x+1, y) +
                       game_get_cell(x+1, y+1);
        } else if (x == (GAME_FIELD_REAL_SIZE_X - 1)) {
            *result += game_get_cell(x-1, y) +
                       game_get_cell(x-1, y+1);
        } else {
            *result += game_get_cell(x-1, y) +
                       game_get_cell(x+1, y) +
                       game_get_cell(x+1, y+1) +
                       game_get_cell(x-1, y+1);
        }
    } else if (y == (GAME_FIELD_REAL_SIZE_Y - 1)) {
        *result += game_get_cell(x, y-1);
        if (x == 0) {
            *result += game_get_cell(x+1, y) +
                       game_get_cell(x+1, y-1);
        } else if (x == (GAME_FIELD_REAL_SIZE_X - 1)) {
            *result += game_get_cell(x-1, y) +
                       game_get_cell(x-1, y-1);
        } else {
            *result += game_get_cell(x-1, y) +
                       game_get_cell(x+1, y) +
                       game_get_cell(x+1, y-1) +
                       game_get_cell(x-1, y-1);
        }
    } else {
        *result += game_get_cell(x, y-1) + game_get_cell(x, y+1);
        if (x == 0) {
            *result += game_get_cell(x+1, y-1) +
                       game_get_cell(x+1, y) +
                       game_get_cell(x+1, y+1);
        } else if (x == (GAME_FIELD_REAL_SIZE_X - 1)) {
            *result += game_get_cell(x-1, y-1) +
                       game_get_cell(x-1, y) +
                       game_get_cell(x-1, y+1);
        } else {
            *result += game_get_cell(x-1, y-1) +
                       game_get_cell(x-1, y-1) +
                       game_get_cell(x-1, y) +
                       game_get_cell(x+1, y) +
                       game_get_cell(x+1, y+1) +
                       game_get_cell(x-1, y+1);
        }
    }

    return OKAY;
}
