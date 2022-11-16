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

#define __WEBCONFIG_C__

/**
 *******************************************************************************
 ** Include files
 *******************************************************************************
 */


#include "stdint.h"
#include "webconfig.h"
#include "appconfig.h"
#include <Arduino.h>
#if defined(ARDUINO_ARCH_ESP8266)
  #include <ESP8266WebServer.h>
#else
  #include <WebServer.h>
#endif

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


static const ICACHE_FLASH_ATTR char successResponse[] = "<META http-equiv=\"refresh\" content=\"15;URL=/\">Update Success! Rebooting...";


/**
 *******************************************************************************
 ** Local function prototypes ('static') 
 *******************************************************************************
 */
static stc_webconfig_handle_t* pstcWebConfig;

#if defined(ARDUINO_ARCH_ESP8266)
static ESP8266WebServer* _pServer;
#else
static WebServer* _pServer;
#endif

/**
 *******************************************************************************
 ** Function implementation - global ('extern') and local ('static') 
 *******************************************************************************
 */

/*************************************
 * ToString Function for type in a buffer at a special position
 * 
 * \param type variable type
 * 
 * \param pu8Data buffer
 * 
 * \param ppos pointer to position of buffer (will be automatically incremented
 * 
 * \return String
 *************************************
 */
static String typeToString(en_webconfig_type_t type, uint8_t* pu8Data, int* ppos)
{
    uint8_t tmp[130];
    memset(tmp,0,130);
    switch(type)
    {
        case enWebConfigTypeStringLen32:
           tmp[32] = 0;
           memcpy(tmp,&pu8Data[*ppos],32);
           *ppos = *ppos + 32;
           return String((char*)tmp);
        case enWebConfigTypeStringLen64:
           tmp[64] = 0;
           memcpy(tmp,&pu8Data[*ppos],64);
           *ppos = *ppos + 64;
           return String((char*)tmp);
        case enWebConfigTypeStringLen128:
          tmp[128] = 0;
          memcpy(tmp,&pu8Data[*ppos],128);
          *ppos = *ppos + 128;
          return String((char*)tmp);
        case enWebConfigTypeUInt8:
          memcpy(tmp,&pu8Data[*ppos],1);
          *ppos = *ppos + 1;
          return String(*((uint8_t*)tmp));
        case enWebConfigTypeInt8:
          memcpy(tmp,&pu8Data[*ppos],1);
          *ppos = *ppos + 1;
          return String(*((int8_t*)tmp));
        case enWebConfigTypeUInt16:
          memcpy(tmp,&pu8Data[*ppos],2);
          *ppos = *ppos + 2;
          return String(*((uint16_t*)tmp));
        case enWebConfigTypeInt16:
          memcpy(tmp,&pu8Data[*ppos],2);
          *ppos = *ppos + 2;
          return String(*((int16_t*)tmp));
        case enWebConfigTypeUInt32:
          memcpy(tmp,&pu8Data[*ppos],4);
          *ppos = *ppos + 4;
          return String(*((uint32_t*)tmp));
        case enWebConfigTypeInt32:
          memcpy(tmp,&pu8Data[*ppos],4);
          *ppos = *ppos + 4;
          return String(*((int32_t*)tmp));
        case enWebConfigTypeBool:
          memcpy(tmp,&pu8Data[*ppos],1);
          *ppos = *ppos + 1;
          if (tmp[0] == 0) return String(false);
          return String(true);
        default:
          return String("");
    }
}

/*************************************
 * Set a value in a configuration buffer
 * 
 * \param index variable index
 * 
 * \param value new value
 * 
 ************************************* 
 */
static void setValue(int index,char* value)
{
  int dataPos = 0;
  int32_t tmp;
  uint32_t utmp;
  
  for (int i = 0; i < index;i++)
  {
    typeToString(pstcWebConfig->astcData[i].type, pstcWebConfig->pu8Data, &dataPos);
  }
  switch(pstcWebConfig->astcData[index].type)
  {
    case enWebConfigTypeStringLen32:
       strncpy((char*)&pstcWebConfig->pu8Data[dataPos],value,32);
       return;
    case enWebConfigTypeStringLen64:
       strncpy((char*)&pstcWebConfig->pu8Data[dataPos],value,64);
       return;
    case enWebConfigTypeStringLen128:
       strncpy((char*)&pstcWebConfig->pu8Data[dataPos],value,128);
       return;
    case enWebConfigTypeUInt8:
       sscanf(value,"%ld",&tmp);
       *((uint8_t*)&pstcWebConfig->pu8Data[dataPos]) = tmp;
       return;
    case enWebConfigTypeInt8:
       sscanf(value,"%ld",&tmp);
       *((int8_t*)&pstcWebConfig->pu8Data[dataPos]) = tmp;
       return;
    case enWebConfigTypeUInt16:
       sscanf(value,"%ld",&tmp);
       *((uint16_t*)&pstcWebConfig->pu8Data[dataPos]) = tmp;
       return;
    case enWebConfigTypeInt16:
       sscanf(value,"%ld",&tmp);
       *((int16_t*)&pstcWebConfig->pu8Data[dataPos]) = tmp;
       return;
    case enWebConfigTypeUInt32:
       sscanf(value,"%lu",&utmp);
       memcpy(&pstcWebConfig->pu8Data[dataPos],(uint8_t*)&utmp,4);
       return;
    case enWebConfigTypeInt32:
       sscanf(value,"%ld",&tmp);
       memcpy(&pstcWebConfig->pu8Data[dataPos],(uint8_t*)&tmp,4);
       return;
  }
}

/*************************************
 * Handle /config/
 * 
 ************************************* 
 */
static void handleConfig() {
  int dataPos = 0;
  int i;
  String postForms = 
  "<html>"
  "<head>"
  "<title>App Configuration</title>"
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
  "<form method=\"post\" enctype=\"application/x-www-form-urlencoded\" action=\"/postform/\">"
  "<table>";
  
  for(i = 0;i < pstcWebConfig->ItemCount;i++)
  {
    postForms += "<tr><td>" + String(pstcWebConfig->astcData[i].description) + "</td><td><input type=\"";
    switch(pstcWebConfig->astcData[i].type)
    {
      case enWebConfigTypeTime:
        postForms += "time";
        break;
      case enWebConfigTypeDate:
        postForms += "date";
        break;
      default:
        postForms += "text";
        break;
    }
    postForms += "\" ";
    postForms += "value=\"" + typeToString(pstcWebConfig->astcData[i].type,pstcWebConfig->pu8Data,&dataPos) + "\" ";
    postForms += "name=\"" + String(pstcWebConfig->astcData[i].name) + "\"></td></tr>";
  }
  
  postForms += "<tr><td>Version</td><td>"APP_VERSION"</td><tr>"
  "<tr><td>&nbsp;</td><td align=right><a href=\"/firmware\" class=\"button\">FW Update</a>&nbsp;&nbsp;<input type=\"submit\" value=\"Save\"></td></tr>"
  "</table>"
  "</form>"
  "</div>"
  "</body>"
  "</html>";
  if (!_pServer->authenticate(AppConfig_GetWwwUser(), AppConfig_GetWwwPass())) {
      return _pServer->requestAuthentication();
    }
  _pServer->send(200,"text/html",postForms);
}

/*************************************
 * Handle /postform/
 * 
 ************************************* 
 */
void handleForm() {
  int i;
  if (!_pServer->authenticate(AppConfig_GetWwwUser(), AppConfig_GetWwwPass())) {
      return _pServer->requestAuthentication();
    }
  if (_pServer->method() != HTTP_POST) {
    _pServer->send(405, "text/plain", "Method Not Allowed");
  } else {
    for (i = 0; i < _pServer->args(); i++) {
      for(int confIndex = 0;confIndex < pstcWebConfig->ItemCount;confIndex++)
      {
        if (strcmp((char*)_pServer->argName(i).c_str(),pstcWebConfig->astcData[confIndex].name) == 0)
        {
          setValue(confIndex,(char*)_pServer->arg(i).c_str());
        }
      }
    }

    AppConfig_Write();
    _pServer->client().setNoDelay(true);
    _pServer->send_P(200, PSTR("text/html"), successResponse);
    delay(100);
    _pServer->client().stop();
    ESP.restart();
  }
}

/*************************************
 * Initiate
 * 
 * \param pWebServerHandle Webserver handle
 * 
 * \param pstcHandle Web configuration handle
 * 
 ************************************* 
 */
#if defined(ARDUINO_ARCH_ESP8266)
void WebConfig_Init(ESP8266WebServer* pWebServerHandle, stc_webconfig_handle_t* pstcHandle)
#else
void WebConfig_Init(WebServer* pWebServerHandle, stc_webconfig_handle_t* pstcHandle)
#endif
{
  pstcWebConfig = pstcHandle;
  _pServer = pWebServerHandle;
  _pServer->on("/config/",handleConfig);
  _pServer->on("/config",handleConfig);
  _pServer->on("/postform/", handleForm);
  #if defined(APP_VERSION)
  _pServer->on("/appversion", []() {
      _pServer->send(200, "text/plain", APP_VERSION);
  });
  #endif
  #if defined(APP_NAME)
  _pServer->on("/appname", []() {
      _pServer->send(200, "text/plain", APP_NAME);
  });
  #endif
}

/**
 *******************************************************************************
 ** EOF (not truncated)
 *******************************************************************************
 */
