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

#if !defined(__USERLEDBUTTON_H__)
#define __USERLEDBUTTON_H__

/* C binding of definitions if building with C++ compiler */
//#ifdef __cplusplus
//extern "C"
//{
//#endif

/**
 *******************************************************************************
 ** \defgroup UserLedButtonGroup Support for user LED and user button
 **
 ** Provided functions of UserLedButton:
 **
 **
 *******************************************************************************
 */

//@{

/**
 *******************************************************************************
** \page userledbutton_module_includes Required includes in main application
** \brief Following includes are required
** @code
** #include "userledbutton.h"
** @endcode
**
 *******************************************************************************
 */

/**
 *******************************************************************************
 ** (Global) Include files
 *******************************************************************************
 */

#include "appconfig.h"

/**
 *******************************************************************************
 ** Global pre-processor symbols/macros ('#define') 
 *******************************************************************************
 */

#define USER_LED -1
#define USER_BUTTON -1
/**
 *******************************************************************************
 ** Global type definitions ('typedef') 
 *******************************************************************************
 */


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


void UserLedButton_Init(void);
void UserLedButton_SetLed(bool onoff);
bool UserLedButton_GetUserButton(void);

//@} // UserLedButtonGroup

//#ifdef __cplusplus
//}
//#endif

#endif /* __USERLEDBUTTON_H__ */

/**
 *******************************************************************************
 ** EOF (not truncated)
 *******************************************************************************
 */
