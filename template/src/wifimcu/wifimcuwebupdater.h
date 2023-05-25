/**
 *******************************************************************************
 ** Created by Manuel Schreiner
 **
 ** Copyright © 2021 io-expert.com. All rights reserved.
 **
 ** 1. Redistributions of source code must retain the above copyright notice,
 **    this condition and the following disclaimer.
 **
 ** This software is provided by the copyright holder and contributors "AS IS"
 ** and any warranties related to this software are DISCLAIMED.
 ** The copyright owner or contributors be NOT LIABLE for any damages caused
 ** by use of this software.

 *******************************************************************************
 */

/**
 *******************************************************************************
 **\file wifimcuwebupdater.c
 **
 ** Website Update Service
 ** A detailed description is available at
 ** @link WifiMcuWebUpdaterGroup file description @endlink
 **
 ** History:
 ** - 2021-2-20  1.00  Manuel Schreiner
 ** - 2023-05-24 1.10 Manuel Schreiner - Adding RP2040 / Raspberry Pi Pico W Support
 *******************************************************************************
 */

#if !defined(__WIFIMCUWEBUPDATER_H__)
#define __WIFIMCUWEBUPDATER_H__

/* C binding of definitions if building with C++ compiler */
//#ifdef __cplusplus
//extern "C"
//{
//#endif

/**
 *******************************************************************************
 ** \defgroup WifiMcuWebUpdaterGroup Website Update Service
 **
 ** Provided functions of WifiMcuWebUpdater:
 **
 **
 *******************************************************************************
 */

//@{

/**
 *******************************************************************************
** \page wifimcuwebupdater_module_includes Required includes in main application
** \brief Following includes are required
** @code
** #include "wifimcuwebupdater.h"
** @endcode
**
 *******************************************************************************
 */

/**
 *******************************************************************************
 ** (Global) Include files
 *******************************************************************************
 */

#include <stdint.h>
#if defined(ARDUINO_ARCH_ESP8266)
  #include <ESP8266WiFi.h>
#else
  #include <WiFi.h>
#endif
#include <WiFiClient.h>
#if defined(ARDUINO_ARCH_ESP8266)
  #include <ESP8266WebServer.h>
  #include <uri/UriBraces.h>
#else
  #include <WebServer.h>
#endif
/**
 *******************************************************************************
 ** Global pre-processor symbols/macros ('#define') 
 *******************************************************************************
 */

/**
 *******************************************************************************
 ** Global type definitions ('typedef') 
 *******************************************************************************
 */



/**
 *******************************************************************************
 ** Global variable declarations ('extern', definition in C source)
 *******************************************************************************
 */

/**
 *******************************************************************************
 ** Global function prototypes ('extern', definition in C source) 
 *******************************************************************************
 */


#if defined(ARDUINO_ARCH_ESP8266)
  void WifiMcuWebUpdater_Init(ESP8266WebServer* pWebServer);
#else
  void WifiMcuWebUpdater_Init(WebServer* pWebServer);
#endif

//@} // WifiMcuWebUpdaterGroup

//#ifdef __cplusplus
//}
//#endif

#endif /* __WIFIMCUWEBUPDATER_H__ */

/**
 *******************************************************************************
 ** EOF (not truncated)
 *******************************************************************************
 */
