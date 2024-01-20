// ===================================================================================
// USB Descriptors and Definitions
// ===================================================================================
//
// Definition of USB descriptors and endpoint sizes and addresses.
//
// The following must be defined in config.h:
// USB_VENDOR_ID            - Vendor ID (16-bit word)
// USB_PRODUCT_ID           - Product ID (16-bit word)
// USB_DEVICE_VERSION       - Device version (16-bit BCD)
// USB_MAX_POWER_mA         - Device max power in mA
// All string descriptors.

#pragma once
#include <stdint.h>
#include "usb.h"
#include "config.h"

// ===================================================================================
// USB Endpoint Definitions
// ===================================================================================
#define EP0_SIZE        8
#define EP1_SIZE        8
#define EP2_SIZE        64
#define EP3_SIZE        8

#define EP0_BUF_SIZE    EP_BUF_SIZE(EP0_SIZE)
#define EP1_BUF_SIZE    EP_BUF_SIZE(EP1_SIZE)
#define EP2_BUF_SIZE    EP_BUF_SIZE(EP2_SIZE) + 64
#define EP3_BUF_SIZE    EP_BUF_SIZE(EP3_SIZE)

#define EP_BUF_SIZE(x)  (x+2<64 ? x+2 : 64)

extern __xdata uint8_t EP0_buffer[];
extern __xdata uint8_t EP1_buffer[];
extern __xdata uint8_t EP2_buffer[];
#define EP3_buffer EP1_buffer

// ===================================================================================
// Device and Configuration Descriptors
// ===================================================================================
typedef struct _USB_CFG_DESCR_ISP {
  USB_CFG_DESCR config;
  USB_ITF_DESCR interface0;
  USB_ENDP_DESCR ep3IN;
  USB_IAD_DESCR association;
  USB_ITF_DESCR interface1;
  uint8_t functional[19];
  USB_ENDP_DESCR ep1IN;
  USB_ITF_DESCR interface2;
  USB_ENDP_DESCR ep2OUT;
  USB_ENDP_DESCR ep2IN;
} USB_CFG_DESCR_ISP, *PUSB_CFG_DESCR_ISP;
typedef USB_CFG_DESCR_ISP __xdata *PXUSB_CFG_DESCR_ISP;

extern __code USB_DEV_DESCR DevDescr;
extern __code USB_CFG_DESCR_ISP CfgDescr;

// ===================================================================================
// String Descriptors
// ===================================================================================
extern __code uint16_t LangDescr[];
extern __code uint16_t ManufDescr[];
extern __code uint16_t ProdDescr[];
extern __code uint16_t SerDescr[];
extern __code uint16_t InterfDescr0[];
extern __code uint16_t InterfDescr1[];

#define USB_STR_DESCR_i0    (uint8_t*)LangDescr
#define USB_STR_DESCR_i1    (uint8_t*)ManufDescr
#define USB_STR_DESCR_i2    (uint8_t*)ProdDescr
#define USB_STR_DESCR_i3    (uint8_t*)SerDescr
#define USB_STR_DESCR_i4    (uint8_t*)InterfDescr0
#define USB_STR_DESCR_i5    (uint8_t*)InterfDescr1
#define USB_STR_DESCR_ix    (uint8_t*)SerDescr

// ===================================================================================
// Windows Compatible ID (WCID) descriptors for automated driver installation
// ===================================================================================
#ifdef WCID_VENDOR_CODE
extern __code uint8_t WCID_FEATURE_DESCR[];
extern __code uint16_t MicrosoftDescr[];
#define USB_STR_DESCR_ixee  (uint8_t*)MicrosoftDescr
#endif
