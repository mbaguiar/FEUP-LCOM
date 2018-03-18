#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H

#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include "vbe.h"
#include "math.h"

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
* @brief returns video mem array
* @return video_mem
*/
short * getVideoMem();

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

/**
 * @brief returns double buffer array
 * @return double buffer
 */
short * getDoubleBuffer();

/**
 * @brief returns vram size
 * @return vram size
 */
int getVramSize();

 /** @} end of video_gr */



#endif /* __VIDEO_GR_H */
