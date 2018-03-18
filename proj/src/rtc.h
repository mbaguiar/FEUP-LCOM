#ifndef __RTC_H
#define __RTC_H

#include <minix/drivers.h>
#include "i8254.h"
#include "i8042.h"
#include "debug.h"

/** @defgroup Rtc Rtc
 * @{
 */


#define RTC_ADDR_REG        0x70
#define RTC_DATA_REG        0x71

#define RTC_REG_A           10
#define RTC_REG_B           11
#define RTC_REG_C           12
#define RTC_REG_D           13

//REG_B BITS
#define RTC_SET             BIT(7)
#define RTC_PIE             BIT(6)
#define RTC_AIE             BIT(5)
#define RTC_UIE             BIT(4)
#define RTC_SQWE            BIT(3)
#define RTC_DM              BIT(2)
#define RTC_2412            BIT(1)
#define RTC_DSE             BIT(0)
#define CLEAR_INT_SOURCE    0x8F

//REG_C BITS
#define RTC_IRQF            BIT(7)
#define RTC_PF              BIT(6)
#define RTC_AF              BIT(5)
#define RTC_UF              BIT(4)

//DATE BITS
#define RTC_SEC             0
#define RTC_MIN             2
#define RTC_HOUR            4
#define RTC_DAY             7
#define RTC_MONTH           8
#define RTC_YEAR            9

#define RTC_IRQ             8

typedef struct {
    unsigned long year;
    unsigned long month;
    unsigned long day;
    unsigned long hour;
    unsigned long minutes;
    unsigned long seconds;
} Date;

extern int hook_id_rtc;

extern int rtcData;

/**
 * @brief subscribes rtc's interrupts
 * @param  PI [description]
 * @param  AI [description]
 * @param  UI [description]
 * @return returns 0 upon sucess and non-zero otherwise
 */
int rtc_subscribe(int PI, int AI, int UI);
/**
 * @brief unsubscribes rtc's interrupts
 * @return returns 0 upon sucess and non-zero otherwise
 */
int rtc_unsubscribe();
/**
 * @brief processes rtc's interrupts data
 */
void rtc_handler();
/**
 * @brief tests if data is in binary or BCD mode
 * @return 1 if data is binary or 0 if data is BCD
 */
int binaryMode();
/**
 * @brief updates time and date
 */
void handle_update_int();
/**
 * @brief converts data passed as argument from bcd to binary
 * @param data [description]
 */
void BCDtoBinary(unsigned long * data);
/**
 * @brief sets pointer to function that processes rtc's interrupts information
 * @param listener pointer to function
 */
void setRTCListener (void (*listener)(Date date));
/**
 * @brief gets current date
 * @return returns current date
 */
Date getCurrentDate();
/**
 * @brief updates date
 * @return returns current date
 */
int updateDate();
/**
 * @brief update time
 * @return returns current time
 */
int updateHour();
/**
 * @brief reads rtc register to determine interrupt type
 * @return
 */
int rtcIntHandlerASM();

/**@}*/



#endif
