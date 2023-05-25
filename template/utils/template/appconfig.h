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
 **\file appconfig.c
 **
 ** App Configuration
 ** A detailed description is available at
 ** @link AppConfigGroup file description @endlink
 **
 ** History:
 ** - 2021-2-3  1.00  Manuel Schreiner
 *******************************************************************************
 */

#if !defined(__APPCONFIG_H__)
#define __APPCONFIG_H__

/* C binding of definitions if building with C++ compiler */
//#ifdef __cplusplus
//extern "C"
//{
//#endif

/**
 *******************************************************************************
 ** \defgroup AppConfigGroup App Configuration
 **
 ** Provided functions of AppConfig:
 **
 **
 *******************************************************************************
 */

//@{

/**
 *******************************************************************************
** \page appconfig_module_includes Required includes in main application
** \brief Following includes are required
** @code
** #include "appconfig.h"
** @endcode
**
 *******************************************************************************
 */

/**
 *******************************************************************************
 ** (Global) Include files
 *******************************************************************************
 */

#include "stdint.h"
#include "./wifimcu/webconfig.h"

/**
 *******************************************************************************
 ** Global pre-processor symbols/macros ('#define') 
 *******************************************************************************
 */

#define APP_VERSION "V1.0.0"
#define APP_NAME "MYAPP"

#define INITIAL_SSID_STATION_MODE "MyWifi"
#define INITIAL_PASSWORD_STATION_MODE "MyPassword"

#define INITIAL_WWW_NAME "admin"
#define INITIAL_WWW_PASS "admin"


/**
 *******************************************************************************
 ** Global type definitions ('typedef') 
 *******************************************************************************
 */

typedef struct __attribute__((__packed__)) stc_appconfig
{
  char ssidStation[32];
  char passwordStation[32];
  char wwwUser[32];
  char wwwPass[32];
  /*APPVARS_DEFINITION*/
  uint32_t u32magic;
} stc_appconfig_t;

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
void AppConfig_Init(ESP8266WebServer* pWebServerHandle);
#else
void AppConfig_Init(WebServer* pWebServerHandle);
#endif
void AppConfig_Write(void);
const char* AppConfig_GetStaSsid(void);
const char* AppConfig_GetStaPassword(void);
void AppConfig_SetStaSsid(const char* ssid);
void AppConfig_SetStaPassword(const char* pass);
const char* AppConfig_GetWwwUser(void);
void AppConfig_SetWwwUser(const char* WwwUser);
const char* AppConfig_GetWwwPass(void);
void AppConfig_SetWwwPass(const char* WwwPass);
/*APPFUNC_PROTOTYPES*/

//@} // AppConfigGroup

//#ifdef __cplusplus
//}
//#endif

#endif /* __APPCONFIG_H__ */

/**
 *******************************************************************************
 ** EOF (not truncated)
 *******************************************************************************
 */
