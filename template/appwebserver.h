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
 **\file appwaywebserver.c
 **
 ** WebServer used as application
 ** A detailed description is available at
 ** @link AppWebServerGroup file description @endlink
 **
 ** History:
 ** - 2021-1-2  1.00  Manuel Schreiner
 *******************************************************************************
 */

#if !defined(__APPWEBSERVER_H__)
#define __APPWEBSERVER_H__

///* C binding of definitions if building with C++ compiler */
//#ifdef __cplusplus
//extern "C"
//{
//#endif

/**
 *******************************************************************************
 ** \defgroup AppWebServerGroup WebServer used as application
 **
 ** Provided functions of AppWebServer:
 **
 **
 *******************************************************************************
 */

//@{

/**
 *******************************************************************************
** \page appwaywebserver_module_includes Required includes in main application
** \brief Following includes are required
** @code
** #include "appwebserver.h"
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
#include "Arduino.h"
#if defined(ARDUINO_ARCH_ESP8266)
  #include <ESP8266WebServer.h>
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
  void AppWebServer_Init(ESP8266WebServer* pWebServer);
#else
  void AppWebServer_Init(WebServer* pWebServer);
#endif

void AppWebServer_Update(void);

//@} // AppWebServerGroup

//#ifdef __cplusplus
//}
//#endif

#endif /* __APPWAYWEBSERVER_H__ */

/**
 *******************************************************************************
 ** EOF (not truncated)
 *******************************************************************************
 */
