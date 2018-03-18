#ifndef _LCOM_I8254_H_
#define _LCOM_I8254_H


#define KBD_IRQ			1

#define STAT_REG		0x64

#define KBC_CMD_REG     0x64

#define OUT_BUF			0x60

#define IN_BUF			0x60

#define ESC_KEY			0x81

#define IBF             BIT(1)

#define OBF				BIT(0)

#define PAR_ERR			BIT(7)

#define TO_ERR			BIT(6)

#define DELAY_US 		20000

#define DOUBLEBYTE		0xE0

#define BREAKCODE		BIT(7)

#define AUX             BIT(5)

#define DISABLE			0xFE

#endif
