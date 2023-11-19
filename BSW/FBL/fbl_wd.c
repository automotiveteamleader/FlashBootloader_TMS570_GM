/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        Watchdog functions
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
 *  02.00.00   2013-09-20  HRf     ESCAN00070534    RCRRP message is not processed as expected
 *  02.01.00   2013-10-01  AWh     ESCAN00070817    Allow to configure external Wd Event Timer handling (P2)
 *  02.02.00   2013-10-09  HRf     ESCAN00070988    Change GetCwIdleTaskState() to FblCwGetIdleTaskState() to follow
 *                                                  fbl_cw nomenclature
 *  02.03.00   2014-01-20  AWh     ESCAN00071053    Unused macros for FBL_ENABLE_GENERIC_WDTIMER_EXT_HANDLING
 *                         TBe     ESCAN00071969    No changes
 *                         ASe     ESCAN00072907    Fixed handling of DSP5683X with Metrowerks compiler
 *  02.04.00   2014-06-13  ASe     ESCAN00076257    Modified watchdog copy handling for C_COMP_IAR_RL78_AFCAN
 *  02.05.00   2014-10-17  Ach     ESCAN00078355    Added void wrapper for FblLookForWatchdog()
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "fbl_inc.h"

/***********************************************************************************************************************
 *  VERSION
 **********************************************************************************************************************/

#if ( FBLWD_VERSION != 0x0205u ) || \
    ( FBLWD_RELEASE_VERSION != 0x00u )
# error "Error in FBL_WD.C: Source and header file are inconsistent!"
#endif

#if ( FBLWD_VERSION != _FBLWD_VERSION ) || \
    ( FBLWD_RELEASE_VERSION != _FBLWD_RELEASE_VERSION )
# error "Error in FBL_WD.C: Source and v_ver.h are inconsistent!"
#endif

# if ( FLASH_DRIVER_VERSION_MASKTYPE != 0x05 )
#  if ( FLASH_DRIVER_VERSION_INTERFACE < 0x0B )
#  error "flashdriver version interface 0x0B required."
#  endif
# endif

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/* PRQA S 3453 FblWd_3453 */ /* MD_CBD_19.7 */
/* PRQA S 3458 FblWd_3458 */ /* MD_CBD_19.4 */

#define TRIGGER_WATCHDOG()          ApplFblWDTrigger()

#   define kFblGenericEventMargin           0x01u
#     define FblWdRetransmit()         FblCanRetransmit()
#   define PerformGenericEventAction()  {\
                                           if (GetRcrRpInProgress()                                \
                                               )                                                   \
                                           {                                                       \
                                               FblWdRetransmit();                                  \
                                               SetP2Timer(kFblDiagTimeP2Star);                     \
                                           }                                                       \
                                        }

/* PRQA L:FblWd_3453 */
/* PRQA L:FblWd_3458 */

/***********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

vuint16 P2Timer;        /**< P2 timeout timer, mapped to wdGenericEventTimer in header file */
vuint8  WDInitFlag;     /**< Watchdog initialized flag */

#if defined( FBL_WATCHDOG_ON )
tWdTime WDTimer;                    /* Watchdog timer */
#endif

/***********************************************************************************************************************
 *  EXTERNAL DATA
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL DATA
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  FblInitWatchdog
 **********************************************************************************************************************/
/*! \brief      Polling of watchdog during longer operations
 *  \return     Reports if initialization was successful.
 **********************************************************************************************************************/
# define INITWATCHDOG_START_SEC_CODE
# include "memmap.h"
void V_API_NEAR FblInitWatchdog( void )
{

   wdGenericEventTimer = 0x00u;
   SetWDInit();
}

# define INITWATCHDOG_STOP_SEC_CODE
# include "memmap.h"

# define LOOKFORWATCHDOG_START_SEC_CODE
# include "memmap.h"

/***********************************************************************************************************************
 *  FblLookForWatchdogVoid
 **********************************************************************************************************************/
/*! \brief       Polling of watchdog during longer operations.
 *  \details     This function executes watchdog and timer handling code from RAM. Depending on the Bootloader
 *               configuration, additionally some communication tasks can be handled by this function. The return
 *               value of FblLookForWatchdog() is not returned by this function so it could be used by libraries which
 *               expect a void return value.
 *  \pre         Watchdog code was copied to RAM, watchdog handling is initialized.
 **********************************************************************************************************************/
void V_API_NEAR FblLookForWatchdogVoid( void )
{
   (void)FblLookForWatchdog();
}

/***********************************************************************************************************************
 *  FblLookForWatchdog
 **********************************************************************************************************************/
/*! \brief       Polling of watchdog during longer operations.
 *  \details     This function executes watchdog and timer handling code from RAM. Depending on the Bootloader
 *               configuration, additionally some communication tasks can be handled by this function.
 *  \pre         Watchdog code was copied to RAM, watchdog handling is initialized.
 *  \return      FBL_NO_TRIGGER if no timer/watchdog expired, FBL_TM_TRIGGERED if the millisecond timer expired,
 *               FBL_WD_TRIGGERED if watchdog handling code was called.
 **********************************************************************************************************************/
vuint8 V_API_NEAR FblLookForWatchdog( void )
{
   vuint8 retValue = FBL_NO_TRIGGER;

   if (GetWDInit()) /* Check if watchdog already initialized */
   {
      if (FblTimerGet() != 0x00u)
      {
         FblTimerReset();
         retValue |= FBL_TM_TRIGGERED;

         if (wdGenericEventTimer > 0x00u)
         {
            wdGenericEventTimer--;
            if (wdGenericEventTimer < kFblGenericEventMargin)
            {
               PerformGenericEventAction();                                          /* PRQA S 3109 */ /* MD_CBD_14.3 */
            }
         }

# if defined( FBL_WATCHDOG_ON )
         if (WDTimer != 0x00u)
         {
            WDTimer--;
            if (WDTimer == 0x00u)
            {
               TRIGGER_WATCHDOG();
               WDTimer = FBL_WATCHDOG_TIME;
               retValue |= FBL_WD_TRIGGERED;
            }
         }
# endif /* FBL_WATCHDOG_ON */
      } /* if (FblTimerGet() != 0x00u) */
   } /* if (GetWDInit()) */

#  if defined( FBL_WD_STATE_TASK )
   /* Execute configurable state task
      Task is active immediately and will be executed on every function call
      Has to be executed from RAM or a lock mechanism needs to be implemented in macro to prevent execution while flash
      operations are active  */
   FBL_WD_STATE_TASK();
#  endif /* FBL_WD_STATE_TASK */

   return retValue;
}

# define LOOKFORWATCHDOG_STOP_SEC_CODE
# include "memmap.h"

/***********************************************************************************************************************
 *  END OF FILE: FBL_WD.C
 **********************************************************************************************************************/
