#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H

#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include "vbe.h"
#include "math.h"
#include "sprite.h"

/** @defgroup video_gr video_gr
 * @{
 *
 * Functions for outputing data to screen in graphics mode
 */

/**
 * @brief Initializes the video module in graphics mode
 *
 * Uses the VBE INT 0x10 interface to set the desired
 *  graphics mode, maps VRAM to the process' address space and
 *  initializes static global variables with the resolution of the screen,
 *  and the number of colors
 *
 * @param mode 16-bit VBE mode to set
 * @return Virtual address VRAM was mapped to. NULL, upon failure.
 */
void *vg_init(unsigned short mode);

 /**
 * @brief Returns to default Minix 3 text mode (0x03: 25 x 80, 16 colors)
 *
 * @return 0 upon success, non-zero upon failure
 */
int vg_exit(void);

/**
* @brief Draws square with center in (x,y)
* @param x center of square
* @param y center of square
* @param size square size
* @param color square color
* @return returns 0 upon success and non zero otherwise
*/
int vg_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color);

/**
* @brief draws line from point (xi,yi) to point (xf,yf)
* @param xi x value of first point
* @param yi y value of first point
* @param xf x value of end point
* @param yf x value of end point
* @param color line color
* @return returns 0 upon success and non zero otherwise
*/
int vg_line(int xi, int yi, int xf, int yf, int color);

/**
* @brief draws xpm
* @param xpm
* @param xi x value for top-left corner of xpm
* @param yi y value for top-left corner of xpm
* @return return 0 upon success and non zero otherwise
*/
int vg_xpm(char *xpm[], unsigned short xi, unsigned short yi);

/**
* @brief gets real position of x and y
* @param x
* @param y
* @return returns position in array
*/
unsigned long getArrayCords(unsigned short x, unsigned short y);

/**
* @brief converts x from coordinates with respect to the center of the screen for real coordinates
* @param x
* @return return real x coordinate
*/
unsigned long convertX(unsigned short x);

/**
* @brief converts y from coordinates with respect to the center of the screen for real coordinates
* @param y
* @return return real y coordinate
*/
unsigned long convertY(unsigned short y);

/**
* @brief returns video mem array
* @return video_mem
*/
char * getVideoMem();

/**
* @brief sets pixel with coordinates (x,y) to color
* @param x value of x
* @param y value of y
* @param color
* @return returns 0 upon success and non zero otherwise
*/
int fillPixel(long x, long y, unsigned color);

/**
 * @brief returns h_res value
 * @return returns h_res
 */
int getHRES();

/**
 * @brief returns v_res value
 * @return returns v_res
 */
int getVRES();

 /** @} end of video_gr */



#endif /* __VIDEO_GR_H */
