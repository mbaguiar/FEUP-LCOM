#ifndef __TIMER_MODULAR_H
#define __TIMER_MODULAR_H

#include <minix/drivers.h>
#include "i8254.h"
#include "i8042.h"
#include "debug.h"
extern int hook_id_timer;

/** @defgroup Timer Timer
 * @{
 */

/**
 * @brief Subscribes and enables Timer 0 interrupts
 *
 * @return Returns bit order in interrupt mask; negative value on failure
 */
int timer_subscribe();

/**
 * @brief Unsubscribes Timer 0 interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int timer_unsubscribe();

/**
 * @brief timer interrupt handler
 */
void timer_handler();

/**
 * @breif sets pointer to function that processes timer handler information
 * @param listener pointer to function
 */
void setTimerListener(void (*listener)());

/**@}*/

#endif /* __TIMER_MODULAR_H */
