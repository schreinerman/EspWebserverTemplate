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
 **\file webconfig.c
 **
 ** Web Configuration
 ** A detailed description is available at
 ** @link WebConfigGroup file description @endlink
 **
 ** History:
 ** - 2021-2-3  1.00  Manuel Schreiner
 *******************************************************************************
 */

#if !defined(__WEBCONFIG_H__)
#define __WEBCONFIG_H__

/* C binding of definitions if building with C++ compiler */
//#ifdef __cplusplus
//extern "C"
//{
//#endif

/**
 *******************************************************************************
 ** \defgroup WebConfigGroup Web Configuration
 **
 ** Provided functions of WebConfig:
 **
 **
 *******************************************************************************
 */

//@{

/**
 *******************************************************************************
** \page webconfig_module_includes Required includes in main application
** \brief Following includes are required
** @code
** #include "webconfig.h"
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
//#include <Arduino.h>
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

typedef enum en_webconfig_type
{
  enWebConfigTypeStringLen32 = 0xF0,
  enWebConfigTypeStringLen64 = 0xF1,
  enWebConfigTypeStringLen128 = 0xF2,
  enWebConfigTypeUInt8 = 0x01,
  enWebConfigTypeInt8 = 0x11,
  enWebConfigTypeUInt16 = 0x02,
  enWebConfigTypeInt16 = 0x12,
  enWebConfigTypeUInt32 = 0x04,
  enWebConfigTypeInt32 = 0x14,
  enWebConfigTypeUInt64 = 0x08,
  enWebConfigTypeInt64 = 0x18,
  enWebConfigTypeBool = 0x21,
  enWebConfigTypeTime = 0x30,
  enWebConfigTypeDate = 0x31,
  enWebConfigTypeDayOfWeek = 0x32
} en_webconfig_type_t;

typedef struct stc_webconfig_time
{
   union {
       uint16_t u16Data;
       struct 
       {
          uint8_t u8Hours;
          uint8_t u8Minutes;
       } Time;
   };
} stc_webconfig_time_t;

typedef struct stc_webconfig_date
{
   union {
       uint32_t u32Data;
       struct 
       {
          uint16_t u16Year;
          uint8_t u8Month;
          uint8_t u8Day;
       } Date;
   };
} stc_webconfig_date_t;


typedef struct stc_webconfig_description
{
  const en_webconfig_type_t type; 
  const char* name;
  const char* description;
} stc_webconfig_description_t;

typedef struct stc_webconfig_handle
{
    uint8_t* pu8Data;
    uint32_t u32DataSize;
    uint32_t ItemCount;
    stc_webconfig_description_t* astcData;
} stc_webconfig_handle_t;

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
void WebConfig_Init(ESP8266WebServer* pWebServerHandle, stc_webconfig_handle_t* pstcHandle);
#else
void WebConfig_Init(WebServer* pWebServerHandle, stc_webconfig_handle_t* pstcHandle);
#endif

//@} // WebConfigGroup

//#ifdef __cplusplus
//}
//#endif

#endif /* __WEBCONFIG_H__ */

/**
 *******************************************************************************
 ** EOF (not truncated)
 *******************************************************************************
 */
