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
 **\file template.ino
 **
 ** History:
 ** - 2021-07-20  1.00  Manuel Schreiner  First Release
 *******************************************************************************
 */

/**
 *******************************************************************************
 ** Include files
 *******************************************************************************
 */

//#define USE_TIME_FUNCTIONS

#if defined(USE_TIME_FUNCTIONS)
#include <ESPDateTime.h>
#include <DateTime.h>
#include <TimeElapsed.h>
#include <DateTimeTZ.h>
#endif

#include <Arduino.h>

#if defined(ARDUINO_ARCH_ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESP8266WebServer.h>
  #include <ESP8266mDNS.h>
#elif defined(ARDUINO_ARCH_ESP32)
  #include <WiFi.h>
  #include <WebServer.h>
  #include <ESPmDNS.h>
#elif defined(ARDUINO_ARCH_RP2040)
  #include <WiFi.h>
  #include <WebServer.h>
  #include <LEAmDNS.h>
#else
#error Not supported architecture
#endif

#include "src/wifimcu/appconfig.h"
#include "src/wifimcu/esp32wifi.h"
#include "src/wifimcu/appwebserver.h"
#include "src/wifimcu/espwebupdater.h"
#include "src/wifimcu/userledbutton.h"

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
    
const char *ssidAp = "MyWifiMcuConfigAP";
const char *passwordAp = "MyWifiMcuConfigAP";
const char *hostName = "my-wifimcu";

static int minsLast = 0;
static int hoursLast = 0;
static int daysLast = 0;
static uint32_t u32LastMillis = 0;

#if defined(ARDUINO_ARCH_ESP8266)
static ESP8266WebServer webServer(80);
#elif defined(ARDUINO_ARCH_ESP32)
static WebServer webServer(80);
#elif defined(ARDUINO_ARCH_RP2040)
static WebServer webServer(80);
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

void setup() {
  // put your setup code here, to run once:

  //intiate serial port
  Serial.begin(115200);
  Serial.println("");
  Serial.println("Welcome...");

  AppConfig_Init(&webServer);

  UserLedButton_Init();

  //initiate WIFI

  #if defined(ARDUINO_ARCH_ESP8266)
     WiFi.hostname(hostName);
  #elif defined(ARDUINO_ARCH_ESP32)
     WiFi.setHostname(hostName);
  #elif defined(ARDUINO_ARCH_RP2040)
     WiFi.setHostname(hostName);
  #endif
  
  Serial.println("Trying to connect with accesspoint (can take a while)...");
  Esp32Wifi_DualModeInit((char*)AppConfig_GetStaSsid(),(char*)AppConfig_GetStaPassword(),ssidAp,passwordAp);
                                                                  
  Serial.println("");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin(hostName)) {
    Serial.println("MDNS responder started");
  }
  
  EspWebUpdater_Init(&webServer);
  
  AppWebServer_Init(&webServer);
  
  webServer.begin();

  #if defined(USE_TIME_FUNCTIONS)
    DateTime.setTimeZone("GMT-2");
    DateTime.begin();

    if (!DateTime.isTimeValid()) {
      Serial.println("Failed to get time from server.");
    } else
    {
      Serial.printf("Date Now is %s\n", DateTime.toISOString().c_str());
      Serial.printf("Timestamp is %ld\n", DateTime.now());
      DateTimeParts parts = DateTime.getParts();
      Serial.printf("H: %d M: %d\n",parts.getHours(),parts.getMinutes());
    }
  #endif
  
  MDNS.addService("http", "tcp", 80);
  MDNS.addService("my-wifimcu","tcp",80);

  //add your initial stuff here
}

void RunEveryMinute(uint8_t u8Hours, uint8_t u8Minutes)
{
    
}

void RunEveryHour(uint8_t u8Hours, uint8_t u8Minutes)
{
    
}

void RunEveryDay(uint8_t u8Hours, uint8_t u8Minutes)
{
  
}



void loop() {
  bool bMinuteUpdated = false;
  bool bHourUpdated = false;
  bool bDayUpdated = false;

  #if defined(USE_TIME_FUNCTIONS)
  DateTimeParts parts = DateTime.getParts();
  #endif

  // put your main code here, to run repeatedly:
  #if defined(ARDUINO_ARCH_ESP8266)
  MDNS.update();
  #endif
  AppWebServer_Update();
  Esp32Wifi_Update();
  AppWebServer_Update();

  if (u32LastMillis != millis())
  {
      u32LastMillis = millis();
  }

  #if defined(USE_TIME_FUNCTIONS)
  if (minsLast != parts.getMinutes())
  {
      RunEveryMinute(parts.getHours(),parts.getMinutes());
      minsLast = parts.getMinutes();
      bMinuteUpdated = true;
  }

  if (hoursLast != parts.getHours())
  {
      if (hoursLast > parts.getHours())
      {
          RunEveryDay(parts.getHours(),parts.getMinutes());
          bDayUpdated = true;
      }
      hoursLast = parts.getHours();
      RunEveryHour(parts.getHours(),parts.getMinutes());
      bHourUpdated = true;
      if (bMinuteUpdated == false)
      {
          RunEveryMinute(parts.getHours(),parts.getMinutes());
      }
  }
  #endif

  //add your cyclic stuff here
}
