#include "i8042.h"
#include "i8254.h"
#define RTC_REG_C 12
#define RTC_UF BIT(4)
#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71

.global _rtcIntHandlerASM
.global _rtcData

.data
  _rtcData:
    .short 0

.text

_rtcIntHandlerASM:

xor %eax, %eax
movb $RTC_REG_C, %al
outb $RTC_ADDR_REG
inb  $RTC_DATA_REG
movb %al, _rtcData
ret
