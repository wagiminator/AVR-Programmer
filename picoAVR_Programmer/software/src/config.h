// ===================================================================================
// User Configurations for CH55x AVR ISP/UPDI Programmer
// ===================================================================================

#pragma once

// Pin definitions
#define PIN_RESET           P14       // pin connected to ICSP RESET
#define PIN_MOSI            P15       // pin connected to ICSP MOSI (do not change)
#define PIN_MISO            P16       // pin connected to ICSP MISO (do not change)
#define PIN_SCK             P17       // pin connected to ISCP SCK  (do not change)
#define PIN_RXD             P30       // pin connected to UPDI      (do not change)
#define PIN_TXD             P31       // pin connected to UPDI via resistor (do not change)
#define PIN_LED             P32       // pin connected to BUSY LED  (active LOW)

// USB device descriptor
#define USB_VENDOR_ID       0x1781    // VID (private use only)
#define USB_PRODUCT_ID      0x0C9F    // PID (private use only)
#define USB_DEVICE_VERSION  0x0107    // v1.07 (BCD-format)

// USB configuration descriptor
#define USB_MAX_POWER_mA    500       // max power in mA (also to power target board)

// USB descriptor strings
#define MANUFACTURER_STR    'w','a','g','i','m','i','n','a','t','o','r'
#define PRODUCT_STR         'p','i','c','o','A','V','R',' ','I','S','P','/','U','P','D','I'
#define SERIAL_STR          'C','H','5','5','x','I','S','P','/','U','P','D','I'
#define INTERFACE0_STR      'T','i','n','y','S','P','I'
#define INTERFACE1_STR      'S','e','r','i','a','l','U','P','D','I'
