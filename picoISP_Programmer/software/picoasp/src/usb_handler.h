// ===================================================================================
// USB Handler for CH551, CH552 and CH554
// ===================================================================================

#pragma once
#include <stdint.h>
#include "ch554.h"
#include "usb_descr.h"

// ===================================================================================
// Variables
// ===================================================================================
#define USB_SetupBuf     ((PUSB_SETUP_REQ)EP0_buffer)
extern volatile uint8_t  USB_SetupReq, USB_SetupTyp;
extern volatile uint16_t USB_SetupLen;
extern volatile __bit    USB_ENUM_OK;
extern __code uint8_t*   USB_pDescr;

// ===================================================================================
// Custom External USB Handler Functions
// ===================================================================================
uint8_t ASP_control(void);
void ASP_EP_init(void);
void ASP_EP0_OUT(void);
void ASP_EP0_IN(void);

// ===================================================================================
// USB Handler Defines
// ===================================================================================
// Custom USB handler functions
#define USB_INIT_endpoints        ASP_EP_init     // init custom endpoints
#define USB_VENDOR_SETUP_handler  ASP_control     // handle vendor setup requests
#define USB_VENDOR_OUT_handler    ASP_EP0_OUT     // handle vendor out transfers
#define USB_VENDOR_IN_handler     ASP_EP0_IN      // handle vendor in transfers

// Endpoint callback functions
#define EP0_SETUP_callback        USB_EP0_SETUP
#define EP0_OUT_callback          USB_EP0_OUT
#define EP0_IN_callback           USB_EP0_IN

// ===================================================================================
// Functions
// ===================================================================================
void USB_init(void);
void USB_interrupt(void);
void USB_EP0_copyDescr(uint8_t len);
