/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        Header file for the watchdog functions
 *
 *  --------------------------------------------------------------------------------------------------------------------
 *  COPYRIGHT
 *  --------------------------------------------------------------------------------------------------------------------
 *  \par Copyright
 *  \verbatim
 *  Copyright (c) 2014 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 */
/**********************************************************************************************************************/

/***********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  --------------------------------------------------------------------------------------------------------------------
 *  Andreas Wenckebach            AWh           Vector Informatik GmbH
 *  Ralf Haegenlaeuer             HRf           Vector Informatik GmbH
 *  Thomas Bezold                 TBe           Vector Informatik GmbH
 *  Alexander Starke              ASe           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  02.00.00   2013-09-20  HRf     ESCAN00070534    No changes
 *  02.01.00   2013-10-01  AWh     ESCAN00070817    Allow to configure external Wd Event Timer handling (P2)
 *  02.02.00   2013-10-09  HRf     ESCAN00070988    No changes
 *  02.03.00   2014-01-20  AWh     ESCAN00071053    Unused macros for FBL_ENABLE_GENERIC_WDTIMER_EXT_HANDLING
 *                         TBe     ESCAN00071969    Corrected configuration check in event timer handling
 *                         ASe     ESCAN00072907    No changes
 *  02.04.00   2014-06-13  ASe     ESCAN00076257    Modified watchdog copy handling for C_COMP_IAR_RL78_AFCAN
 *  02.05.00   2014-10-17  Ach     ESCAN00078355    Added void wrapper for FblLookForWatchdog()
 **********************************************************************************************************************/

#ifndef __FBL_WD_H__
#define __FBL_WD_H__

/***********************************************************************************************************************
 *  VERSION
 **********************************************************************************************************************/

/* ##V_CFG_MANAGEMENT ##CQProject : FblWd CQComponent : Implementation */
#define FBLWD_VERSION            0x0205u
#define FBLWD_RELEASE_VERSION    0x00u

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/* PRQA S 3453 FblWd_3453 */ /* MD_CBD_19.7 */
/* PRQA S 3458 FblWd_3458 */ /* MD_CBD_19.4 */

/* Return values of LookForWatchdog */
/* Indicates its internal action    */
#define FBL_NO_TRIGGER     0x00u    /* No action performed (just watching the timer) */
#define FBL_WD_TRIGGERED   0x03u    /* Watchdog has been triggered */
#define FBL_TM_TRIGGERED   0x02u    /* Timer has expired and was re-triggered */

#if defined( FBL_WD_TRIGGER_SIZE )
#else
#  define FBL_WD_TRIGGER_SIZE  160u
#endif /* FBL_WD_TRIGGER_SIZE */

/* Macros for watchdog initialization */
#define GetWDInit()        (WDInitFlag == 0x01u)
#define SetWDInit()        (WDInitFlag = 0x01u)
#define ClrWDInit()        (WDInitFlag = 0x00u)

/* Map wdGenericEventTimer to P2 timeout timer */
# define wdGenericEventTimer P2Timer
/* Define P2Timer access macros */
# define GetP2Timer()       (P2Timer)
# define SetP2Timer(val)    (P2Timer = (val))
# define ClrP2Timer()       (P2Timer = 0x00u)

/* PRQA L:FblWd_3453 */
/* PRQA L:FblWd_3458 */

/***********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/

extern vuint16 P2Timer;    /**< Multiple purpose timer */
extern vuint8  WDInitFlag; /**< Watchdog initialized flag */

#if defined( FBL_WATCHDOG_ON )
# if ( FBL_WATCHDOG_TIME > 255u )
typedef vuint16 tWdTime;
# else
typedef vuint8 tWdTime;
# endif

extern tWdTime WDTimer; /**< Watchdog timer */
#endif /* FBL_WATCHDOG_ON */

/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

# define INITWATCHDOG_START_SEC_CODE_EXPORT
# include "memmap.h"
void V_API_NEAR FblInitWatchdog ( void );

# define INITWATCHDOG_STOP_SEC_CODE_EXPORT
# include "memmap.h"

# define LOOKFORWATCHDOG_START_SEC_CODE_EXPORT
# include "memmap.h"

void V_API_NEAR FblLookForWatchdogVoid( void );

vuint8 V_API_NEAR FblLookForWatchdog( void );

# define LOOKFORWATCHDOG_STOP_SEC_CODE_EXPORT
# include "memmap.h"

#endif /* __FBL_WD_H__ */

/***********************************************************************************************************************
 *  END OF FILE: FBL_WD.H
 **********************************************************************************************************************/
