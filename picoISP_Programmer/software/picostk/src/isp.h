// ===================================================================================
// ISP Functions for for CH551, CH552 and CH554
// ===================================================================================

#pragma once
#include <stdint.h>
#include "gpio.h"
#include "delay.h"
#include "config.h"
#include "usb_cdc.h"

#define HWVER             2
#define SWMAJ             1
#define SWMIN             18

// STK500V1 Definitions
#define STK_OK            0x10
#define STK_FAILED        0x11
#define STK_UNKNOWN       0x12
#define STK_INSYNC        0x14
#define STK_NOSYNC        0x15
#define CRC_EOP           0x20

#define STK_GET_SYNC      0x30
#define STK_GET_SIGNON    0x31
#define STK_GET_PARM      0x41
#define STK_SET_PARM      0x42
#define STK_SET_PARM_EXT  0x45
#define STK_PMODE_START   0x50
#define STK_PMODE_END     0x51
#define STK_SET_ADDR      0x55
#define STK_UNIVERSAL     0x56
#define STK_PROG_FLASH    0x60
#define STK_PROG_DATA     0x61
#define STK_PROG_PAGE     0x64
#define STK_READ_PAGE     0x74
#define STK_READ_SIGN     0x75

void ISP_init(void);
void avrisp(void);
