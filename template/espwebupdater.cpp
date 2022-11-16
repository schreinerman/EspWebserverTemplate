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
 **\file espwebupdater.c
 **
 ** Website Update Service located at http://<IP>/firmware
 ** A detailed description is available at
 ** @link EspWebUpdaterGroup file description @endlink
 **
 ** History:
 ** - 2021-2-20  1.00  Manuel Schreiner
 *******************************************************************************
 */

#define __ESPWEBUPDATER_C__

/**
 *******************************************************************************
 ** Include files
 *******************************************************************************
 */

#include <Arduino.h>
#include "appconfig.h"
#include <stdint.h>
#include "espwebupdater.h"
#include "StreamString.h"
#if defined(ARDUINO_ARCH_ESP8266)
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <flash_hal.h>
#include <FS.h>

#elif defined(ARDUINO_ARCH_ESP32)
#include <Update.h>
#include <WiFiUdp.h>
#else 
#error Unsupported platform, requires ESP32 or ESP8266 core
#endif

/**
 *******************************************************************************
 ** Local pre-processor symbols/macros ('#define') 
 *******************************************************************************
 */

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
static ESP8266WebServer* pWebServer;
#else
static WebServer* pWebServer;
#endif

/**
 *******************************************************************************
 ** Local function prototypes ('static') 
 *******************************************************************************
 */

static const ICACHE_FLASH_ATTR char serverIndex[] = "<html>"
  "<head>"
  "<title>App Update</title>"
  "<meta name=\"viewport\" content=\"width=device-width,initial-scale=1.0\">"
  "<link rel=\"stylesheet\" href=\"/site.css\">"
  "</head>"
  "<body width=300px>"
  "<div class=\"bar\">"
  "<div class=\"topcornerleft\">"
  "<a href=\"/\">"
  "<svg class=mdi-icon-32 viewBox=\"0 0 32 32\">"
  "  <path fill=\"currentColor\" d=\"M2,12A10,10 0 0,1 12,2A10,10 0 0,1 22,12A10,10 0 0,1 12,22A10,10 0 0,1 2,12M18,11H10L13.5,7.5L12.08,6.08L6.16,12L12.08,17.92L13.5,16.5L10,13H18V11Z\" />"
  "</svg>"
  "</a></div>"
  "<div class=\"title\"><script>document.write(document.title);</script></div>"
  "</div>"
  "<div class=\"formular\">"
  "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update' style=\"font-size: 0.8rem; width: 290px\"><input type='submit' value='Update'></form>"
  "</div>"
  "</body"
  "</html>";

static const ICACHE_FLASH_ATTR char successResponse[] = "<META http-equiv=\"refresh\" content=\"15;URL=/\">Update Success! Rebooting...";

/**
 *******************************************************************************
 ** Function implementation - global ('extern') and local ('static') 
 *******************************************************************************
 */

/*********************************************
 * Init Firmware Updater
 * 
 * pWebServerHandle Webserver handle
 * 
 ********************************************* 
 */
#if defined(ARDUINO_ARCH_ESP8266)
  void EspWebUpdater_Init(ESP8266WebServer* _pWebServer)
#else
  void EspWebUpdater_Init(WebServer* _pWebServer)
#endif
{
  pWebServer = _pWebServer;
  pWebServer->on("/firmware", HTTP_GET, []() {
    pWebServer->sendHeader("Connection", "close");
    pWebServer->send(200, "text/html", serverIndex);
  });
  pWebServer->on("/update", HTTP_POST, []() {
    if (!pWebServer->authenticate(AppConfig_GetWwwUser(), AppConfig_GetWwwPass())) {
      return pWebServer->requestAuthentication();
    }
    pWebServer->sendHeader("Connection", "close");
    if (Update.hasError()) {
      StreamString str;
      Update.printError(str);
      pWebServer->send(200, F("text/html"), String(F("Update error: ")) + str.c_str());
    } else {
      pWebServer->client().setNoDelay(true);
      pWebServer->send_P(200, PSTR("text/html"), successResponse);
      delay(100);
      pWebServer->client().stop();
      ESP.restart();
    }
  }, []() {
    if (!pWebServer->authenticate(AppConfig_GetWwwUser(), AppConfig_GetWwwPass())) {
      return pWebServer->requestAuthentication();
    }
    HTTPUpload &upload = pWebServer->upload();
    if (upload.status == UPLOAD_FILE_START) {
      Serial.setDebugOutput(true);
      Serial.printf("Update: %s\n", upload.filename.c_str());
      
#if defined(ARDUINO_ARCH_ESP8266)
      WiFiUDP::stopAll();
#endif
      
 #if defined(ARDUINO_ARCH_ESP8266)
      uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
      if (!Update.begin(maxSketchSpace, U_FLASH)){//start with max available size
 #else
      if (!Update.begin()) { //start with max available size
 #endif
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) { //true to set the size to the current progress
        Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
      } else {
        Update.printError(Serial);
      }
      Serial.setDebugOutput(false);
    } else {
      Serial.printf("Update Failed Unexpectedly (likely broken connection): status=%d\n", upload.status);
    }
  });
}


/**
 *******************************************************************************
 ** EOF (not truncated)
 *******************************************************************************
 */
