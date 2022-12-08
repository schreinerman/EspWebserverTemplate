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

#define __ESP32WIFI_C__

/**
 *******************************************************************************
 ** Include files
 *******************************************************************************
 */

#include "esp32wifi.h"
#include <DNSServer.h>

#if defined(ARDUINO_ARCH_ESP8266)
  #include <ESP8266WiFi.h>
#elif defined(ARDUINO_ARCH_ESP32)
  #include <WiFi.h>
#endif
#include <WiFiClient.h>


/**
 *******************************************************************************
 ** Local pre-processor symbols/macros ('#define') 
 *******************************************************************************
 */

#pragma GCC optimize ("-O3")

#define uS_TO_S_FACTOR 1000000ULL  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  10          /* Time ESP32 will go to sleep (in seconds)       */
#define TIME_TO_WAKE   15          /* Time between sleep waiting for connection      */
#define TIMEOUT_SLEEP  60          /* Seconds after last keepout before sleep, set to 0 to disable */
#define STATION_MODE_REBOOT (5 * 60) /* reboot after 5 minutes in station mode, to keep station mode, set to 0 */

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

const byte DNS_PORT = 53;
DNSServer dnsServer;
static uint32_t millisOld = 0;
static volatile uint32_t u32Counter = 1;
static volatile uint32_t u32StationReboot = 0;
static char* _ssidStationMode;
static char* _passwordStationMode;
static char* _ssidApMode;
static char* _passwordApMode;

static uint32_t u32TimeToSleepSeconds = TIME_TO_SLEEP;

static en_esp32_wifi_mode_t _mode = enESP32WifiModeSoftAP;

/**
 *******************************************************************************
 ** Local function prototypes ('static') 
 *******************************************************************************
 */


static void ConnectStation(void);
static void ConnectSoftAP(void);

/**
 *******************************************************************************
 ** Function implementation - global ('extern') and local ('static') 
 *******************************************************************************
 */

/**
 * Init WiFi
 * 
 * \param mode      Station or AP mode: enESP32WifiModeSoftAP or enESP32WifiModeStation
 * 
 * \param ssid      WiFi SSID
 * 
 * \param password  WiFi Password
 * 
 */
void Esp32Wifi_Init(en_esp32_wifi_mode_t mode, const char* ssid, const char* password)
{
    _mode = mode;
    _ssidStationMode = (char*)ssid;
    _passwordStationMode = (char*)password;
    _ssidApMode = (char*)ssid;
    _passwordApMode = (char*)password;
    #if !defined(ARDUINO_ARCH_ESP8266) //only available with ESP32
        esp_sleep_enable_timer_wakeup(u32TimeToSleepSeconds * uS_TO_S_FACTOR); // ESP32 wakes up every 5 seconds
    #endif
    Esp32Wifi_Connect();
}

/**
 * Init WiFi
 * 
 * \param ssidStation      WiFi SSID for Station Mode
 * 
 * \param passwordStation  WiFi Password for Station Mode
 * 
 * \param ssidAp           WiFi SSID for AP Mode
 * 
 * \param passwordAp       WiFi Password for AP Mode
 * 
 */
void Esp32Wifi_DualModeInit(const char* ssidStation, const char* passwordStation, const char* ssidAp, const char* passwordAp)
{
    _ssidStationMode = (char*)ssidStation;
    _passwordStationMode = (char*)passwordStation;
    _ssidApMode = (char*)ssidAp;
    _passwordApMode = (char*)passwordAp;
    #if !defined(ARDUINO_ARCH_ESP8266) //only available with ESP32
       esp_sleep_enable_timer_wakeup(u32TimeToSleepSeconds * uS_TO_S_FACTOR); // ESP32 wakes up every 5 seconds
    #endif
    WiFi.disconnect();
    WiFi.persistent( false );
    #if defined(ARDUINO_ARCH_ESP8266)
        WiFi.setOutputPower(20.5); // this sets wifi to highest power
    #endif
    WiFi.mode(WIFI_STA);
    WiFi.begin(_ssidStationMode, _passwordStationMode);
    delay(500);
    // Wait for connection
    volatile uint32_t u32Tries = 0;
    while (WiFi.status() != WL_CONNECTED) 
    {   
        u32Tries++;
        if ((u32Tries == 10) || (u32Tries == 20))
        {
           WiFi.disconnect();
           WiFi.persistent( false );
           WiFi.mode(WIFI_STA);
           WiFi.begin(_ssidStationMode, _passwordStationMode);
        }
        if (u32Tries > 30)
        {
           Serial.println("Initializing AP mode...");
           _mode = enESP32WifiModeSoftAP;
           WiFi.disconnect();
           Esp32Wifi_Connect();
           return;
        }
        volatile int i = 0;
        while ((WiFi.status() != WL_CONNECTED) && (i < 500))
        {
            delay(1);
            i++;
        }
    }
    Serial.println("STA Connected...");
    _mode = enESP32WifiModeStation;
    Esp32Wifi_Connect();
}

/*
 * Connect Station Mode
 */
static void ConnectStation(void)
{
  if (WiFi.status() != WL_CONNECTED)
  {
      WiFi.disconnect();
      WiFi.persistent( false );
      WiFi.mode(WIFI_STA);
      WiFi.begin(_ssidStationMode, _passwordStationMode);
      delay(500);
      //Serial.println("WiFi initiated...");
    
      // Wait for connection
      volatile uint32_t u32Tries = 1;
      while (WiFi.status() != WL_CONNECTED) 
      {
          //Serial.print(".");
          u32Tries++;
          if ((u32Tries % 10) == 0)
          {
            WiFi.disconnect();
            WiFi.persistent( false );
            WiFi.mode(WIFI_STA);
            WiFi.begin(_ssidStationMode, _passwordStationMode);
            //Serial.println("WiFi initiated...");
          }
          if (u32Tries > 60)
          {
              ESP.restart();
          }
          volatile int i = 0;
          while ((WiFi.status() != WL_CONNECTED) && (i < 500))
          {
              delay(1);
              i++;
          }
      }
  }
}

/*
 * Connect/Create AP
 */
static void ConnectSoftAP(void)
{
    static bool bConnected = false;
    if (!bConnected)
    {
      bConnected = true;
      u32StationReboot = 0;
      if (WiFi.softAP(_ssidApMode, _passwordApMode))
      {
          IPAddress myIP = WiFi.softAPIP();
          Serial.print("AP IP address: ");
          Serial.println(myIP);
          dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
          dnsServer.start(DNS_PORT, "*", myIP);
      }
      
      
    }
}

/*
 * Connect to AP / create an AP
 */
void Esp32Wifi_Connect(void)
{
    switch(_mode)
    {
      case enESP32WifiModeSoftAP:
          ConnectSoftAP();
          break;
      case enESP32WifiModeStation:
          ConnectStation();
          break;
      default:
          break;
    }
    
}




/*
 * Update in loop, so ESP32 can be put to sleep mode in station mode
 */
void Esp32Wifi_Update(void)
{
    uint32_t u32Diff = millis();
    if (u32Diff < millisOld)
    {
       u32Diff = 0xFFFFFFFFul - millisOld + u32Diff;
    } else
    {
       u32Diff = u32Diff - millisOld;
    }
  
    if (u32Diff > 0)
    {
        u32Counter += u32Diff;
        u32StationReboot += u32Diff;
        millisOld = millis();
    }
  
    #if STATION_MODE_REBOOT > 0
    if ((_mode == enESP32WifiModeSoftAP) && (u32StationReboot > (STATION_MODE_REBOOT*1000)) && (WiFi.softAPgetStationNum() == 0))
    {
       ESP.restart();
    }
    #endif
    #if TIMEOUT_SLEEP > 0
    if (u32Counter > (TIMEOUT_SLEEP*1000))
    {
        //Serial.println("sleep");
        //Serial.flush(); 
        if (_mode == enESP32WifiModeStation) 
        {
          #if defined(ARDUINO_ARCH_ESP8266)
              WiFi.forceSleepBegin();
              delay(u32TimeToSleepSeconds*1000);
              WiFi.forceSleepWake();
          #elif !defined(ARDUINO_ARCH_ESP8266) //only available with ESP32
              esp_light_sleep_start();
          #endif
           
        }
        //Serial.println("wake");
        Esp32Wifi_Connect();
        u32Counter = ((TIMEOUT_SLEEP-TIME_TO_WAKE)*1000);
    }
    #endif
}

void Esp32Wifi_SetSleepTime(uint32_t u32SleepTime)
{
  if (u32TimeToSleepSeconds != u32SleepTime)
  {
    u32TimeToSleepSeconds = u32SleepTime;
    #if (!defined(ARDUINO_ARCH_ESP8266) && (TIMEOUT_SLEEP > 0)) //only available with ESP32
        esp_sleep_enable_timer_wakeup(u32TimeToSleepSeconds * uS_TO_S_FACTOR); // ESP32 wakes up every 5 seconds
    #endif
  }
}

/*
 * Execute to keep WiFi alive
 */
void Esp32Wifi_KeepAlive(void)
{
    u32Counter = 0;
    u32StationReboot = 0;
}

/**
 *******************************************************************************
 ** EOF (not truncated)
 *******************************************************************************
 */
