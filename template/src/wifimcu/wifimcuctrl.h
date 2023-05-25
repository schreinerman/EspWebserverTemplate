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
 **\file wifimcuctrl.c
 **
 ** Handle WiFi
 ** A detailed description is available at
 ** @link ESP32WifiGroup file description @endlink
 **
 ** History:
 ** - 2021-1-2  1.00  Manuel Schreiner
 ** - 2023-05-24 1.10 Manuel Schreiner - Adding RP2040 / Raspberry Pi Pico W Support
 *******************************************************************************
 */

#if !defined(__WIFIMCUCTRL_H__)
#define __WIFIMCUCTRL_H__

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
** \page wifimcuctrl_module_includes Required includes in main application
** \brief Following includes are required
** @code
** #include "wifimcuctrl.h"
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

typedef enum en_wifi_mcu_ctrl_mode
{
  enESP32WifiModeSoftAP = 0,
  enESP32WifiModeStation = 1
} en_wifi_mcu_ctrl_mode_t;

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

void WifiMcuCtrl_Init(en_wifi_mcu_ctrl_mode_t mode, const char* ssid, const char* password);
void WifiMcuCtrl_DualModeInit(const char* ssidStation, const char* passwordStation, const char* ssidAp, const char* passwordAp);
void WifiMcuCtrl_Connect(void);
void WifiMcuCtrl_Update(void);
void WifiMcuCtrl_KeepAlive(void);
void WifiMcuCtrl_SetSleepTime(uint32_t u32SleepTime);

//@} // ESP32WifiGroup

//#ifdef __cplusplus
//}
//#endif

#endif /* __WIFIMCUCTRL_H__ */

/**
 *******************************************************************************
 ** EOF (not truncated)
 *******************************************************************************
 */
