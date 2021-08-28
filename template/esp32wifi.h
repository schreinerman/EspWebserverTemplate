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
 **\file esp32wifi.c
 **
 ** Handle WiFi
 ** A detailed description is available at
 ** @link ESP32WifiGroup file description @endlink
 **
 ** History:
 ** - 2021-1-2  1.00  Manuel Schreiner
 *******************************************************************************
 */

#if !defined(__ESP32WIFI_H__)
#define __ESP32WIFI_H__

/* C binding of definitions if building with C++ compiler */
//#ifdef __cplusplus
//extern "C"
//{
//#endif

/**
 *******************************************************************************
 ** \defgroup ESP32WifiGroup Handle WiFi
 **
 ** Provided functions of ESP32Wifi:
 **
 **
 *******************************************************************************
 */

//@{

/**
 *******************************************************************************
** \page esp32wifi_module_includes Required includes in main application
** \brief Following includes are required
** @code
** #include "esp32wifi.h"
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

typedef enum en_esp32_wifi_mode
{
  enESP32WifiModeSoftAP = 0,
  enESP32WifiModeStation = 1
} en_esp32_wifi_mode_t;

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

void Esp32Wifi_Init(en_esp32_wifi_mode_t mode, const char* ssid, const char* password);
void Esp32Wifi_DualModeInit(const char* ssidStation, const char* passwordStation, const char* ssidAp, const char* passwordAp);
void Esp32Wifi_Connect(void);
void Esp32Wifi_Update(void);
void Esp32Wifi_KeepAlive(void);
void Esp32Wifi_SetSleepTime(uint32_t u32SleepTime);

//@} // ESP32WifiGroup

//#ifdef __cplusplus
//}
//#endif

#endif /* __ESP32WIFI_H__ */

/**
 *******************************************************************************
 ** EOF (not truncated)
 *******************************************************************************
 */
