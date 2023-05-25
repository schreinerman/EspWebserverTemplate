#ifndef __HTMLFS_H__
#define __HTMLFS_H__
#include <stdint.h>
#include <Arduino.h>
#if defined(ARDUINO_ARCH_ESP8266)
#include <ESP8266WebServer.h>
#else
#include <WebServer.h>
#endif

#if defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_ESP32)
#define PLACE_IROM ICACHE_FLASH_ATTR
#endif

#if defined(ARDUINO_ARCH_RP2040)
  #define PLACE_IROM
#endif


#if defined(ARDUINO_ARCH_ESP8266)
  void HtmlFs_Init(ESP8266WebServer* pServer);
#else
  void HtmlFs_Init(WebServer* pServer);
#endif

#endif
