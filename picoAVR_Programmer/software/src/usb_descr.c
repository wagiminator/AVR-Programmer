// ===================================================================================
// USB Descriptors
// ===================================================================================

#include "usb_descr.h"

// ===================================================================================
// Endpoint Buffers
// ===================================================================================
__xdata uint8_t EP0_buffer[EP0_BUF_SIZE];
__xdata uint8_t EP1_buffer[EP1_BUF_SIZE];
__xdata uint8_t EP2_buffer[EP2_BUF_SIZE];

// ===================================================================================
// Device Descriptor
// ===================================================================================
__code USB_DEV_DESCR DevDescr = {
  .bLength            = sizeof(DevDescr),       // size of the descriptor in bytes: 18
  .bDescriptorType    = USB_DESCR_TYP_DEVICE,   // device descriptor: 0x01
  .bcdUSB             = 0x0200,                 // USB specification: USB 2.0
  .bDeviceClass       = 0,                      // interface will define class
  .bDeviceSubClass    = 0,                      // unused
  .bDeviceProtocol    = 0,                      // unused
  .bMaxPacketSize0    = EP0_SIZE,               // maximum packet size for Endpoint 0
  .idVendor           = USB_VENDOR_ID,          // VID
  .idProduct          = USB_PRODUCT_ID,         // PID
  .bcdDevice          = USB_DEVICE_VERSION,     // device version
  .iManufacturer      = 1,                      // index of Manufacturer String Descr
  .iProduct           = 2,                      // index of Product String Descriptor
  .iSerialNumber      = 3,                      // index of Serial Number String Descr
  .bNumConfigurations = 1                       // number of possible configurations
};

// ===================================================================================
// Configuration Descriptor
// ===================================================================================
__code USB_CFG_DESCR_ISP CfgDescr = {

  // Configuration Descriptor
  .config = {
    .bLength            = sizeof(USB_CFG_DESCR),  // size of the descriptor in bytes
    .bDescriptorType    = USB_DESCR_TYP_CONFIG,   // configuration descriptor: 0x02
    .wTotalLength       = sizeof(CfgDescr),       // total length in bytes
    .bNumInterfaces     = 3,                      // number of interfaces: 3
    .bConfigurationValue= 1,                      // value to select this configuration
    .iConfiguration     = 0,                      // no configuration string descriptor
    .bmAttributes       = 0x80,                   // attributes = bus powered, no wakeup
    .MaxPower           = USB_MAX_POWER_mA / 2    // in 2mA units
  },

  // Interface Descriptor: Interface 0 (USBasp)
  .interface0 = {
    .bLength            = sizeof(USB_ITF_DESCR),  // size of the descriptor in bytes: 9
    .bDescriptorType    = USB_DESCR_TYP_INTERF,   // interface descriptor: 0x04
    .bInterfaceNumber   = 0,                      // number of this interface: 0
    .bAlternateSetting  = 0,                      // value used to select alternative setting
    .bNumEndpoints      = 1,                      // number of endpoints used: 1
    .bInterfaceClass    = USB_DEV_CLASS_VENDOR,   // vendor specific class: 0xFF
    .bInterfaceSubClass = 0,                      // unused
    .bInterfaceProtocol = 0,                      // unused
    .iInterface         = 4                       // interface string descriptor
  },

  // Endpoint Descriptor: Endpoint 3 (IN, Interrupt)
  .ep3IN = {
    .bLength            = sizeof(USB_ENDP_DESCR), // size of the descriptor in bytes: 7
    .bDescriptorType    = USB_DESCR_TYP_ENDP,     // endpoint descriptor: 0x05
    .bEndpointAddress   = USB_ENDP_ADDR_EP3_IN,   // endpoint: 3, direction: IN (0x83)
    .bmAttributes       = USB_ENDP_TYPE_INTER,    // transfer type: interrupt (0x03)
    .wMaxPacketSize     = EP3_SIZE,               // max packet size
    .bInterval          = 10                      // polling intervall in ms
  },

  // Interface Association Descriptor
  .association = {
    .bLength            = sizeof(USB_IAD_DESCR),  // size of the descriptor in bytes
    .bDescriptorType    = USB_DESCR_TYP_IAD,      // interf association descr: 0x0B
    .bFirstInterface    = 1,                      // first interface
    .bInterfaceCount    = 2,                      // total number of interfaces
    .bFunctionClass     = USB_DEV_CLASS_COMM,     // function class: CDC (0x02)
    .bFunctionSubClass  = 2,                      // 2: Abstract Control Model (ACM)
    .bFunctionProtocol  = 1,                      // 1: AT command protocol
    .iFunction          = 5                       // index of String Descriptor
  },

  // Interface Descriptor: Interface 1 (CDC)
  .interface1 = {
    .bLength            = sizeof(USB_ITF_DESCR),  // size of the descriptor in bytes: 9
    .bDescriptorType    = USB_DESCR_TYP_INTERF,   // interface descriptor: 0x04
    .bInterfaceNumber   = 1,                      // number of this interface: 1
    .bAlternateSetting  = 0,                      // value used to select alternative setting
    .bNumEndpoints      = 1,                      // number of endpoints used: 1
    .bInterfaceClass    = USB_DEV_CLASS_COMM,     // interface class: CDC (0x02)
    .bInterfaceSubClass = 2,                      // 2: Abstract Control Model (ACM)
    .bInterfaceProtocol = 1,                      // 1: AT command protocol
    .iInterface         = 5                       // index of String Descriptor
  },

  // Functional Descriptors for Interface 1
  .functional = {
    0x05,0x24,0x00,0x10,0x01,                     // header functional descriptor
    0x05,0x24,0x01,0x00,0x01,                     // call management functional descriptor
    0x04,0x24,0x02,0x02,                          // direct line management functional descriptor
    0x05,0x24,0x06,0x01,0x02                      // union functional descriptor: CDC IF1, Data IF2
  },

  // Endpoint Descriptor: Endpoint 1 (CDC Upload, Interrupt)
  .ep1IN = {
    .bLength            = sizeof(USB_ENDP_DESCR), // size of the descriptor in bytes: 7
    .bDescriptorType    = USB_DESCR_TYP_ENDP,     // endpoint descriptor: 0x05
    .bEndpointAddress   = USB_ENDP_ADDR_EP1_IN,   // endpoint: 1, direction: IN (0x81)
    .bmAttributes       = USB_ENDP_TYPE_INTER,    // transfer type: interrupt (0x03)
    .wMaxPacketSize     = EP1_SIZE,               // max packet size
    .bInterval          = 1                       // polling intervall in ms
  },

  // Interface Descriptor: Interface 2 (Data)
  .interface2 = {
    .bLength            = sizeof(USB_ITF_DESCR),  // size of the descriptor in bytes: 9
    .bDescriptorType    = USB_DESCR_TYP_INTERF,   // interface descriptor: 0x04
    .bInterfaceNumber   = 2,                      // number of this interface: 2
    .bAlternateSetting  = 0,                      // value used to select alternative setting
    .bNumEndpoints      = 2,                      // number of endpoints used: 2
    .bInterfaceClass    = USB_DEV_CLASS_DATA,     // interface class: data (0x0a)
    .bInterfaceSubClass = 0,                      // interface sub class
    .bInterfaceProtocol = 0,                      // interface protocol
    .iInterface         = 5                       // index of String Descriptor
  },

  // Endpoint Descriptor: Endpoint 2 (OUT)
  .ep2OUT = {
    .bLength            = sizeof(USB_ENDP_DESCR), // size of the descriptor in bytes: 7
    .bDescriptorType    = USB_DESCR_TYP_ENDP,     // endpoint descriptor: 0x05
    .bEndpointAddress   = USB_ENDP_ADDR_EP2_OUT,  // endpoint: 2, direction: OUT (0x02)
    .bmAttributes       = USB_ENDP_TYPE_BULK,     // transfer type: bulk (0x02)
    .wMaxPacketSize     = EP2_SIZE,               // max packet size
    .bInterval          = 0                       // polling intervall (ignored for bulk)
  },

  // Endpoint Descriptor: Endpoint 2 (IN)
  .ep2IN = {
    .bLength            = sizeof(USB_ENDP_DESCR), // size of the descriptor in bytes: 7
    .bDescriptorType    = USB_DESCR_TYP_ENDP,     // endpoint descriptor: 0x05
    .bEndpointAddress   = USB_ENDP_ADDR_EP2_IN,   // endpoint: 2, direction: IN (0x82)
    .bmAttributes       = USB_ENDP_TYPE_BULK,     // transfer type: bulk (0x02)
    .wMaxPacketSize     = EP2_SIZE,               // max packet size
    .bInterval          = 0                       // polling intervall (ignored for bulk)
  }
};

// ===================================================================================
// String Descriptors
// ===================================================================================

// Language Descriptor (Index 0)
__code uint16_t LangDescr[] = {
  ((uint16_t)USB_DESCR_TYP_STRING << 8) | sizeof(LangDescr), 0x0409 };  // US English

// Manufacturer String Descriptor (Index 1)
__code uint16_t ManufDescr[] = {
  ((uint16_t)USB_DESCR_TYP_STRING << 8) | sizeof(ManufDescr), MANUFACTURER_STR };

// Product String Descriptor (Index 2)
__code uint16_t ProdDescr[] = {
  ((uint16_t)USB_DESCR_TYP_STRING << 8) | sizeof(ProdDescr), PRODUCT_STR };

// Serial String Descriptor (Index 3)
__code uint16_t SerDescr[] = {
  ((uint16_t)USB_DESCR_TYP_STRING << 8) | sizeof(SerDescr), SERIAL_STR };

// Interface String Descriptor (Index 4)
__code uint16_t InterfDescr0[] = {
  ((uint16_t)USB_DESCR_TYP_STRING << 8) | sizeof(InterfDescr0), INTERFACE0_STR };

// Interface String Descriptor (Index 5)
__code uint16_t InterfDescr1[] = {
  ((uint16_t)USB_DESCR_TYP_STRING << 8) | sizeof(InterfDescr1), INTERFACE1_STR };

// ===================================================================================
// Windows Compatible ID (WCID) descriptors for automated driver installation
// ===================================================================================
#ifdef WCID_VENDOR_CODE
// Microsoft WCID feature descriptor (index 0x0004)
__code uint8_t WCID_FEATURE_DESCR[] = {
    0x28, 0x00, 0x00, 0x00,                         // length = 40 bytes
    0x00, 0x01,                                     // version 1.0 (in BCD)
    0x04, 0x00,                                     // compatibility descriptor index 0x0004
    0x01,                                           // number of sections
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,       // reserved (7 bytes)
    0x00,                                           // interface number 0
    0x01,                                           // reserved
    'W', 'I', 'N', 'U', 'S', 'B', 0x00, 0x00,       // Compatible ID "WINUSB\0\0"
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Subcompatible ID (unused)
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00              // reserved 6 bytes
};

// Microsoft OS string descriptor for WCID driver (index 0xEE)
__code uint16_t MicrosoftDescr[] = {
  ((uint16_t)USB_DESCR_TYP_STRING << 8) | sizeof(MicrosoftDescr),
  'M','S','F','T','1','0','0', WCID_VENDOR_CODE };
#endif
