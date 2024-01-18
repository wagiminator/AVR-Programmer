// ===================================================================================
// USBasp Functions for CH551, CH552 and CH554                                * v1.2 *
// ===================================================================================

#pragma once
#include <stdint.h>
#include "isp.h"
#include "tpi.h"
#include "usb_handler.h"

// USB function call identifiers
#define USBASP_FUNC_CONNECT           1
#define USBASP_FUNC_DISCONNECT        2
#define USBASP_FUNC_TRANSMIT          3
#define USBASP_FUNC_READFLASH         4
#define USBASP_FUNC_ENABLEPROG        5
#define USBASP_FUNC_WRITEFLASH        6
#define USBASP_FUNC_READEEPROM        7
#define USBASP_FUNC_WRITEEEPROM       8
#define USBASP_FUNC_SETLONGADDRESS    9
#define USBASP_FUNC_SETISPSCK         10
#define USBASP_FUNC_TPI_CONNECT       11
#define USBASP_FUNC_TPI_DISCONNECT    12
#define USBASP_FUNC_TPI_RAWREAD       13
#define USBASP_FUNC_TPI_RAWWRITE      14
#define USBASP_FUNC_TPI_READBLOCK     15
#define USBASP_FUNC_TPI_WRITEBLOCK    16
#define USBASP_FUNC_GETCAPABILITIES   127

// USBASP capabilities
#define USBASP_CAP_0_TPI              0x01

// Block mode flags
#define PROG_BLOCKFLAG_FIRST          1
#define PROG_BLOCKFLAG_LAST           2

// Functions
void ASP_init(void);
