#ifndef __GAME_H
#define __GAME_H

#include "bitmap.h"
#include "handler.h"
#include "GameCtrl.h"
#include "vbe.h"

/** @defgroup Game Game
 * @{
 * Functions that initialize game
 */

/**
 * @brief starts game, initializes game controler
 */

void gameStart();

/**
 * @brief starts handler loop
 */

void gameRun();

/**@}*/

#endif
