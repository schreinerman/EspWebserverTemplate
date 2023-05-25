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
 **\file userledbutton.c
 **
 ** Support for user LED and user button
 ** A detailed description is available at
 ** @link UserLedButtonGroup file description @endlink
 **
 ** History:
 ** - 2021-2-21  1.00  Manuel Schreiner
 *******************************************************************************
 */

#define __USERLEDBUTTON_C__

/**
 *******************************************************************************
 ** Include files
 *******************************************************************************
 */


#include "appconfig.h"
#include "userledbutton.h"

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
static int statusLED = -1;
static int userButton = -1;
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

/*********************************************
 * Init User Button / Status LEDs
 * 
 ********************************************* 
 */
void UserLedButton_Init(void)
{
  statusLED = USER_LED;
  userButton = USER_BUTTON;
  if (statusLED > -1)
  {
    pinMode(statusLED,OUTPUT);
  }
  if (userButton > -1)
  {
    pinMode(userButton,INPUT);
  }
}

/*********************************************
 * Set Status LED on/off
 * 
 * \param onoff  true = on, false = off
 * 
 ********************************************* 
 */
void UserLedButton_SetLed(bool onoff)
{
  if (statusLED < 0)
  {
    return; 
  }
  if (onoff)
  {
    digitalWrite(statusLED,HIGH);
  } else
  {
    digitalWrite(statusLED,LOW);
  } 
}

/*********************************************
 * Get button status
 * 
 * \return  true = on, false = off
 * 
 ********************************************* 
 */
bool UserLedButton_GetUserButton(void)
{
  if (userButton < 0)
  {
    return false; 
  }
  if (digitalRead(userButton))
  {
    return true;
  } else
  {
    return false;
  }
}



/**
 *******************************************************************************
 ** EOF (not truncated)
 *******************************************************************************
 */
