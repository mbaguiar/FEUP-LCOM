#ifndef __MULTIPLAYERCTRL_H
#define __MULTIPLAYERCTRL_H

#include <minix/drivers.h>
#include "debug.h"
#include "serialport.h"

/** @defgroup MultiplayerCtrl MultiplayerCtrl
 * @{
 * Functions for syncing two players for multiplayer mode
 */

typedef enum {
  LOOKING_FOR_HOST,
  LOOKING_FOR_PLAYER,
  CANCEL_LOOKING,
  FOUND_HOST,
  FOUND_PLAYER,
  CONNECTED,
  READY_PLAYER,
  READY_HOST,
  READY_BOTH
} lobby_state_t;

typedef enum {
    ERR,
    TIMEOUT,
    CANCELED,
    SYNCED
} lobby_exit_cause_t;

typedef enum {
    PLAYER,
    HOST,
    NOT_EST
} connection_state_t;

/**
 * @brief resets sync lobby variables
 */
void enterLobby();
/**
 * @brief lobby manager - syncs to players
 * @return returns 0 if synced, -1 if timeout and 1 otherwise
 */
int lobbyManager();
/**
 * @brief returns game seed
 * @return game seed
 */
unsigned int getSeed();

/**@}*/
unsigned int generateSeed();

#endif
