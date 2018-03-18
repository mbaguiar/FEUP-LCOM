#ifndef __HANDLER_H
#define __HANDLER_H

#include "video_gr.h"
#include "mouse.h"
#include "rtc.h"
#include "serialport.h"
#include "MultiplayerCtrl.h"
#include "timer_modular.h"
#include "keyboard.h"

/** @defgroup Handler Handler
 * @{
 * Functions for processing interrupts
 */

/**
 * @brief functions that subscribes all the interuupts and starts loop that processes interuupts
 * @return Returns 0 upon sucess and non-zero otherwise
 */
int handler_loop();

/**
 * @brief unsubscribes all interrupts
 * @return Returns 0 upon sucess and non-zero otherwise
 */

int unsubscribe();

/**@}*/

#endif
