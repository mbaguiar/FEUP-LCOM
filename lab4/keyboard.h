#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "i8042.h"
#include "timer_modular.h"
#include <minix/syslib.h>
#include <minix/sysutil.h>

extern int hook_id_kbd;
int cntr;
long scancodeASM;

/**
 * @brief subscribes keyboard's interrupts
 *
 * @return Returns 0 upon success and non-zero otherwise
 */
int keyboard_subscribe();

/**
 * @brief unsbscribes keyboard's interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int keyboard_unsubscribe();

/**
 * @brief reads status byte from kbc controller
 *
 * @param *statusbyte Pointer to unsigned long variable that will store new status byte
 *
 * return Return 0 upon success and non-zero otherwise
 */
int readStatusByte(unsigned long *statusbyte);

/**
 * @brief Reads from out buffer
 *
 * @param *data Pointer to unsigned long variable that will store the data read from the out buffer
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int readOutBuf(unsigned long *data);

/**
 * @brief Issues command to KBC
 *
 * @param reg adress of register to send command
 * @param cmd command byte that will be sent
 *
 * @return Returns 0 upon success and non-zero otherwise
 */
int sendCmdToKBC(unsigned long reg, unsigned long cmd);

/**
 * @brief Reads Out Buffer (written in assembly)
 *
 * @return Returns 0 upon success and non-zero otherwise
 */
int readOutBufASM();


#endif
