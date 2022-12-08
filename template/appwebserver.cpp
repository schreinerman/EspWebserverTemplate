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
 **\file appwebserver.c
 **
 ** WebServer used as application
 ** A detailed description is available at
 ** @link AppWebServerGroup file description @endlink
 **
 ** History:
 ** - 2021-1-2  1.00  Manuel Schreiner
 *******************************************************************************
 */

#define __APPWEBSERVER_C__

/**
 *******************************************************************************
 ** Include files
 *******************************************************************************
 */

#if defined(ARDUINO_ARCH_ESP8266)
  #include <ESP8266WiFi.h>
#else
  #include <WiFi.h>
#endif
#include <WiFiClient.h>
#if defined(ARDUINO_ARCH_ESP8266)
  #include <ESP8266WebServer.h>
#elif defined(ARDUINO_ARCH_ESP32)
  #include <WebServer.h>
#endif
#include "appconfig.h"
#include "appwebserver.h"
#include "htmlfs.h"
#include "esp32wifi.h"


/**
 *******************************************************************************
 ** Local pre-processor symbols/macros ('#define') 
 *******************************************************************************
 */

#pragma GCC optimize ("-O3")

/**
 *******************************************************************************
 ** Global variable definitions (declared in header file with 'extern') 
 *******************************************************************************
 */

/**
 *******************************************************************************
 ** Local type definitions ('typedef') 
 *******************************************************************************
 */

/**
 *******************************************************************************
 ** Local variable definitions ('static') 
 *******************************************************************************
 */

#if defined(ARDUINO_ARCH_ESP8266)
static ESP8266WebServer* pServer;
#else
static WebServer* pServer;
#endif

/**
 *******************************************************************************
 ** Local function prototypes ('static') 
 *******************************************************************************
 */

/**
 *******************************************************************************
 ** Function implementation - global ('extern') and local ('static') 
 *******************************************************************************
 */

/*
 * Init Webserver Service
 * 
 * \param pWebServer webserver handle
 */
#if defined(ARDUINO_ARCH_ESP8266)
  void AppWebServer_Init(ESP8266WebServer* pWebServer)
#else
  void AppWebServer_Init(WebServer* pWebServer)
#endif
{
  pServer = pWebServer;

  pServer->on("/cmd/keepalive", []() {
      Esp32Wifi_KeepAlive();
      pServer->send(200, "text/plain", "keep alive accepted");
  });

  HtmlFs_Init(pServer);
}

/*
 * Update Webserver in loop
 */
void AppWebServer_Update(void)
{
    pServer->handleClient();
}
/**
 *******************************************************************************
 ** EOF (not truncated)
 *******************************************************************************
 */
