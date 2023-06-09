// ===================================================================================
// User Configurations for CH552E picoSTK
// ===================================================================================

#pragma once

// Pin definitions for ISP bus
#define PIN_RESET           P14       // pin connected to ICSP RESET
#define PIN_MOSI            P15       // pin connected to ICSP MOSI (do not change)
#define PIN_MISO            P16       // pin connected to ICSP MISO (do not change)
#define PIN_SCK             P17       // pin connected to ISCP SCK  (do not change)

// Pin definitions for status LEDs (uncomment if not used)
//#define PIN_LED_PRG                 // pin connected to PROGMODE LED (active LOW)
//#define PIN_LED_ERR                 // pin connected to ERROR LED (active LOW)
//#define PIN_LED_USB                 // pin connected to USB LED (active LOW)

// USB device descriptor
#define USB_VENDOR_ID       0x16C0    // VID (shared www.voti.nl)
#define USB_PRODUCT_ID      0x27DD    // PID (shared CDC-ACM)
#define USB_DEVICE_VERSION  0x0107    // v1.07 (BCD-format)

// USB configuration descriptor
#define USB_MAX_POWER_mA    500       // max power in mA (also to power target board)

// USB descriptor strings
#define MANUFACTURER_STR    'w','a','g','i','m','i','n','a','t','o','r'
#define PRODUCT_STR         'A','r','d','u','i','n','o','I','S','P'
#define SERIAL_STR          'p','i','c','o','S','T','K'
#define INTERFACE_STR       'C','D','C','-','S','e','r','i','a','l'
