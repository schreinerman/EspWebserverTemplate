#ifndef __HTMLFS_H__
#define __HTMLFS_H__
#include <stdint.h>
#include <Arduino.h>
#if defined(ARDUINO_ARCH_ESP8266)
#include <ESP8266WebServer.h>
#else
#include <WebServer.h>
#endif
#define PLACE_IROM ICACHE_FLASH_ATTR


#if defined(ARDUINO_ARCH_ESP8266)
  void HtmlFs_Init(ESP8266WebServer* pServer);
#else
  void HtmlFs_Init(WebServer* pServer);
#endif

#endif
