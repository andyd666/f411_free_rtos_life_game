/*
 * game.h
 *
 *  Created on: 16 SEP 2023 ð.
 *      Author: andyd
 */

#ifndef INCLUDE_GAME_H_
#define INCLUDE_GAME_H_

#define GAME_FIELD_REAL_SIZE_X 240
#define GAME_FIELD_REAL_SIZE_Y 200

#if GAME_FIELD_SIZE_X % 8 != 0
#error
#endif

#if GAME_FIELD_SIZE_Y % 8 != 0
#error
#endif

#define GAME_FIELD_ARRAY_SIZE_X (GAME_FIELD_REAL_SIZE_X / 8)
#define GAME_FIELD_ARRAY_SIZE_Y (GAME_FIELD_REAL_SIZE_Y)

typedef enum {
    OKAY = 0,
    BAD  = 1,
} RESULT;

#endif /* INCLUDE_GAME_H_ */
