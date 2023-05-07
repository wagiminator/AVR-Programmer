// ===================================================================================
// USB Handler for CH551, CH552 and CH554
// ===================================================================================

#pragma once
#include <stdint.h>
#include "usb_descr.h"

// ===================================================================================
// Variables
// ===================================================================================
#define USB_setupBuf ((PUSB_SETUP_REQ)EP0_buffer)
extern uint8_t  USB_MSG_flags;
extern uint8_t  SetupReq;
extern uint16_t SetupLen;
extern __code uint8_t *pDescr;

// ===================================================================================
// Custom External USB Handler Functions
// ===================================================================================
uint8_t ISP_control(void);
void ISP_setup(void);
void ISP_reset(void);
void ISP_EP0_IN(void);
void ISP_EP0_OUT(void);
void ISP_EP1_IN(void);

// ===================================================================================
// USB Handler Defines
// ===================================================================================
// Custom USB handler functions
#define USB_INIT_handler    ISP_setup         // init custom endpoints
#define USB_RESET_handler   ISP_reset         // custom USB reset handler
#define USB_CTRL_NS_handler ISP_control       // handle custom non-standard requests

// Endpoint callback functions
#define EP0_SETUP_callback  USB_EP0_SETUP
#define EP0_IN_callback     ISP_EP0_IN
#define EP0_OUT_callback    ISP_EP0_OUT
#define EP1_IN_callback     ISP_EP1_IN

// USB Message Flags
#define USB_FLG_USE_USER_RW       (1 << 0)
#define USB_FLG_MSGPTR_IS_ROM     (1 << 6)
#define USB_FLG_MSGPTR_IS_RAM     (1 << 7)

// ===================================================================================
// Functions
// ===================================================================================
void USB_interrupt(void);
void USB_init(void);
void USB_EP0_copyDescr(uint8_t len);
