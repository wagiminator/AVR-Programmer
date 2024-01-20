// ===================================================================================
// picoAVR Functions for CH551, CH552 and CH554                               * v1.0 *
// ===================================================================================

#pragma once
#include <stdint.h>
#include "config.h"
#include "system.h"
#include "delay.h"
#include "gpio.h"
#include "updi.h"
#include "usb_asp.h"
#include "usb_cdc.h"

inline void AVR_init(void) {
  ISP_disconnect();                       // disconnect ISP/TPI bus
  UPDI_init();                            // setup UPDI
  USB_init();                             // setup USB
}
