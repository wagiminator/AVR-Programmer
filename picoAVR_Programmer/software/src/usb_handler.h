// ===================================================================================
// USB Handler for CH551, CH552 and CH554
// ===================================================================================

#pragma once
#include <stdint.h>
#include "usb_descr.h"

// ===================================================================================
// Endpoint Buffer
// ===================================================================================
__xdata __at (EP0_ADDR) uint8_t EP0_buffer[EP0_BUF_SIZE];     
__xdata __at (EP1_ADDR) uint8_t EP1_buffer[EP1_BUF_SIZE];
__xdata __at (EP2_ADDR) uint8_t EP2_buffer[EP2_BUF_SIZE];
__xdata __at (EP3_ADDR) uint8_t EP3_buffer[2 * EP3_BUF_SIZE];

#define USB_setupBuf ((PUSB_SETUP_REQ)EP0_buffer)
extern uint8_t SetupReq;
extern volatile uint8_t USB_MSG_flags;

// ===================================================================================
// Custom External USB Handler Functions
// ===================================================================================
uint8_t ISP_control(void);
void ISP_setup(void);
void ISP_reset(void);
void ISP_EP0_IN(void);
void ISP_EP0_OUT(void);
void ISP_EP1_IN(void);
void CDC_EP2_IN(void);
void CDC_EP3_IN(void);
void CDC_EP3_OUT(void);

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
#define EP2_IN_callback     CDC_EP2_IN
#define EP3_IN_callback     CDC_EP3_IN
#define EP3_OUT_callback    CDC_EP3_OUT

// ===================================================================================
// Functions
// ===================================================================================
void USB_interrupt(void);
void USB_init(void);
