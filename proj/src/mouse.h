#ifndef __MOUSE_H
#define __MOUSE_H

#include "i8042.h"
#include "keyboard.h"
#include <minix/syslib.h>
#include <minix/sysutil.h>

/** @defgroup Mouse Mouse
 * @{
 */



typedef struct {
    signed long x;
    signed long y;
} xyval;

//global variable to store mouse data packet
extern unsigned long packet[3];
extern int hook_id_mouse;

/**
 * @brief subscribes mouse's interrupts
 *
 * @return Returns 0 upon success and non-zero otherwise
 */
int mouse_subscribe();

/**
 * @brief unsubscribes mouses's interrupts
 *
 * @return Returns 0 upon success and non-zero otherwise
 */
int mouse_unsubscribe();

/**
 * @brief Issues command to mouse
 *
 * @param cmd command byte that will be sent
 *
 * @return Returns 0 upon success and non-zero otherwise
 */
int sendCmdToMouse(unsigned long cmd);

/**
* @brief enables mouse data reporting
* @return Returns 0 upon sucess and non-zero otherwise
*/
int enable_mouse();

/**
* @brief disables mouse data reporting
* @return Returns 0 upon sucess and non-zero otherwise
*/
int disable_mouse();

/**
* @brief processes interrupts from the mouse and stores the data in a global variable
*/
void mouse_handler();

/**
* @brief gets the values for variables x and y from the mouse data packet
*
* @return Returns struct of x and y values
*/
xyval getXY();

/**
 * @brief sets pointer to function that processes interrupts mouse information
 * @param listener pointer to function
 */
void setMouseListener(void (*listener)());

/**@}*/

#endif
