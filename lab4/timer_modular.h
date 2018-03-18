#ifndef __TIMER_MODULAR_H
#define __TIMER_MODULAR_H

#include <minix/drivers.h>
#include "i8254.h"
#include "debug.h"
extern int hook_id_timer;

/**
 * @brief Changes the operating frequency of a timer
 * 
 * Must not change the 3 LSBs (mode and BCD/binary) of the timer's control word.
 * 
 * @param timer Timer to configure. (Ranges from 0 to 2)
 * @param freq Timer operating frequency
 * @return Return 0 upon success and non-zero otherwise
 */
int timer_set_frequency(unsigned char timer, unsigned long freq);

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
 * @brief Reads the input timer configuration via read-back command
 *
 * @param timer Timer whose config to read (Ranges from 0 to 2)
 * @param st    Address of memory position to be filled with the timer config
 * @return Return 0 upon success and non-zero otherwise
 */
int timer_get_conf(unsigned char timer, unsigned char *st);

/**
 * @brief Returns Timer address
 *
 * Returns Timer address given its number
 *
 * @param timer Timer chosen
 * @return Return Timer address
 */
unsigned long get_timer_address(unsigned char timer);

/**
 * @brief Tests if timer is valid
 *
 * Tests if timer number ranges from 0 to 2
 *
 * @param timer Timer chosen
 * @return Return 0 if valid
 */
int test_valid_timer(unsigned char timer);

#endif /* __TIMER_MODULAR_H */