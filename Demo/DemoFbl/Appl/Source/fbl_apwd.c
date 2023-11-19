/* Kernbauer Version: 1.12 Konfiguration: FBL Erzeugungsgangnummer: 1 */

/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief         Application dependent watchdog routines
 *
 *  \note          Please note, that this file contains a collection of callback functions to be used with the
 *                 Flash Bootloader. These functions may influence the behavior of the bootloader in principle.
 *                 Therefore, great care must be taken to verify the correctness of the implementation.
 *                 The contents of the originally delivered files are only examples resp. implementation proposals.
 *                 With regard to the fact that these functions are meant for demonstration purposes only, Vector
 *                 Informatik's liability shall be expressly excluded in cases of ordinary negligence, to the extent
 *                 admissible by law or statute.
 *
 *  --------------------------------------------------------------------------------------------------------------------
 *  COPYRIGHT
 *  --------------------------------------------------------------------------------------------------------------------
 *  \par Copyright
 *  \verbatim
 *  Copyright (c) 2012 by Vector Informatik GmbH.                                                  All rights reserved.
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
 *  Christian Baeuerle            CB            Vector Informatik GmbH
 *  Armin Happel                  Hp            Vector Informatik GmbH
 *  Konrad Lazarus                Ls            Vector Informatik GmbH
 *  Alexandre C. Plombin          ACP           Vector CANtech, Inc.
 *  Ahmad Nasser                  AN            Vector CANtech, Inc.
 *  Robert Schaeffner             Rr            Vector Informatik GmbH
 *  Marco Wierer                  WM            Vector Informatik GmbH
 *  Joern Herwig                  JHG           Vector Informatik GmbH
 *  Andre Caspari                 Ci            Vector Informatik GmbH
 *  Michael F. Radwick            MFR           Vector CANtech, Inc.
 *  Andreas Wenckebach            AWh           Vector Informatik GmbH
 *  Achim Strobelt                Ach           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  03.00.00   2004-07-09  WM                       First implementation
 *  03.01.00   2004-09-22  WM                       Minor changes
 *  03.02.00   2005-01-21  Rr                       Changed version defines due to naming
 *                                                  conventions
 *  03.03.00   2005-04-20  WM      ESCAN00012125    Support copying of the
 *                                                  watchdog function by the startup code
 *  03.04.00   2007-01-17  JHG     ESCAN00017120    Added token for additional functions
 *                         Ls      ESCAN00014726    FBL_COPY_CODE_TO_RAM_ON_STARTUP
 *                         AMN                      Added new watchdog functions
 *  03.05.00   2007-01-22  WM                       Added V_VER.H version cross check
 *  03.06.00   2007-03-29  MFR     ESCAN00020057    Add tokens for local data,
 *                                                  prototypes, etc.
 *  03.07.00   2008-03-07  Hp                       No changes
 *  03.08.00   2009-11-11  AWh     ESCAN00039075    Added Kb-includes for own apwd-defines
 *  03.09.00   2012-03-30  QPs     ESCAN00058043    No changes
 *  03.10.00   2012-12-06  Ach     ESCAN00062912    Adapted comments to use Doxygen
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY (kbFbl_apAPI.c)
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  00.90.00   2003-12-10  CB                       First implementation
 *  00.91.00   2004-01-08  CB                       Decryption/decompression interface changed
 *                         CB                       Parameters for ApplFblVerifyChecksum changed
 *  01.00.00   2004-02-24  CB                       Release
 *  01.01.00   2004-03-24  Rr                       Added new functions
 *  01.02.00   2004-03-25  WM                       Minor changes and enhancements for new OEM
 *  01.03.00   2004-04-23  WM                       Added watchdog trigger functions
 *                         ACP                      Added ApplFblSecurityStatus
 *                         AMN                      Added ApplFblIsValidFlashBlock, and ApplFblReadBlock
 *  01.04.00   2004-05-07  CB                       Added ApplCanBusOff, ApplCanWakeUp
 *  01.05.00   2004-05-19  CB                       Minor changes
 *  01.06.00   2004-06-30  MHz                      No changes
 *  01.07.00   2004-07-22  CB                       Added ApplFblReadEcuIdentification,
 *                                                  ApplFblStoreTesterCode, ApplFblStoreProgrammingDate
 *  01.08.00   2004-08-06  WM                       Added ApplFblRWSecurityAccessDelayFlag
 *  01.09.00   2004-09-09  Rr                       Minor changes for ApplFblCanBusOff
 *  01.10.00   2004-09-16  WM                       Added PRE and POST section for ApplFblWDTrigger
 *  01.11.00   2004-10-25  AP      ESCAN00010007    Adaption to support Vector
 *                                                  component OSEK NM
 *  01.12.00   2004-12-23  AMN                      Changed prototype of ApplFblWriteDataById
 *                                                  Added an ifdef to ApplFblCanParamInit
 *                                                  Added function ApplFblPBLConfigSet
 *                                                  Corrected the paramters in ApplFblRWProgCounter
 *                                                  Define name changes PBL->CAN
 *                         ACP                      Version check reworked
 *                         CB                       Added fingerprint functions
 *  01.13.00   2005-01-14  CB                       Added several API functions
 *  01.14.00   2005-03-22  WM      ESCAN00011595    MISRA warnings
 *  01.15.00   2005-09-29  WM                       _PRE_ and _POST_ section for ApplFblWDTriggerEnd
 *                                                  V_VER.H consistency check added
 *  01.16.00   2005-12-08  WM      ESCAN00014594    FblApSrcApplFblTpParamInit token
 *                                                  added for TPMC
 *  01.17.00   2006-02-28  AMN                      Added ApplFblCopyToRAM and ApplFblIsValidAppInRAM
 *  01.18.00   2006-03-24  QPs                      Added ApplFblGetSubNodeRxCanId for Gateway
 *                                                  implementation
 *  01.19.00   2006-04-25  CB      ESCAN00015917    Return value added for
 *                                                  ApplFblInvalidateBlock
 *                         WM      ESCAN00015960    ApplFblVerifySegmentChecksum added
 *                         Ls                       added KBTok_FblApSrcApplFblCheckProgDependencies
 *                         QPs     ESCAN00016110    Added ApplCanBusOff for the Gateway
 *                         CB      ESCAN00016144    Return value added for
 *                                                  ApplFblInitDecryption
 *                         CB      ESCAN00016147    ApplFblStoreBootSwIdent added
 *                                 ESCAN00016171    ApplFblSetSecAccessDelayFlag,
 *                                                  ApplFblGetSecAccessDelayFlag and
 *                                                  ApplFblClrSecAccessDelayFlag added
 *  01.20.00   2006-05-18  ACP     ESCAN00016438    Added kStartupPreInit/PostInit
 *                                                  definition for new ApplFblStartup() parameter
 *  01.21.00   2006-06-29  FHE                      No changes
 *  01.22.00   2006-07-24  JHg                      Added V_CALLBACK_NEAR to watchdog trigger
 *                         CB      ESCAN00017003    Added ApplFblProgrammingStateChanged
 *                         AMN                      Added ApplFblValidateFile
 *                                                  Added a token to reference local prototypes
 *                                                  from the OEM layer
 *  01.23.00   2006-08-28  JHg                      Added token for additional watchdog functions
 *                         AMN     ESCAN00017479    Modified the API of the
 *                                                  ApplFblValidateApp for Misra compliance
 *                                                  Removed ApplFblIsValidFlashBlock because it's
 *                                                  not used anymore
 *  01.24.00   2006-12-06  FHE     ESCAN00018639    Added ApplFblCheckConditions
 *  01.25.00   2007-02-07  Hp      ESCAN00019129    Add support for CCP in the bootloader
 *                                                  Add PreHandler and presence-pattern support.
 *  01.26.00   2007-03-30  WM      ESCAN00020175    Added ApplFblCheckProgPreconditions
 *  01.27.00   2007-07-06  JHg                      Return value added to ApplFblDecryption and
 *                                                  ApplFblDeinitDecryption
 *  01.28.00   2007-07-27  Ls      ESCAN00022542    FBL_ENABLE_PRE_WDINIT_COMMENT added
 *  01.29.00   2008-03-07  Ci      ESCAN00014726    FBL_COPY_CODE_TO_RAM_ON_STARTUP
 *                                                  renamed to FBL_ENABLE_COPY..
 *                         Hp      ESCAN00014726    Support for memmap and FBL_ENABLE_COPY_CODE_TO_RAM_ON_STARTUP
 *                                                  to map ApplFblWDTrigger and ApplFblWDTriggerEnd to special location
 *                         JHg                      Update CAN driver Tx confirmation routines to RI 1.4 API
 *                         Hp      ESCAN00025029    Support Vector decompression module (optional)
 *  01.30.00   2008-03-28  JHg     ESCAN00025354    Remove COMMENT restriction from FBL_ENABLE_PRE_WDINIT
 *                                                  If enabled watchdog will be already initalized in ApplFblInit
 *                                 ESCAN00025355    FBL_ENABLE_PRE_TIMERINIT added
 *                                                  If enabled timer will be already initalized in ApplFblInit
 *  01.31.00   2008-06-13  Ci      ESCAN00025071    Extended function description of
 *                                                  ApplFblStartup() with kStartupStayInBoot
 *                                 ESCAN00027445    Added support for configurations
 *                                                  with multiple CAN channels
 *  01.32.00   2008-09-10  JHg     ESCAN00029860    Moved PRE_WDINIT and PRE_TIMERINIT to fbl_main
 *  01.33.00   2008-10-15  FHe     ESCAN00030640    Added support for VAG spec 1.2.1
 *  01.34.00   2009-03-13  JHg     ESCAN00032180    Added support for multichannel configurations
 *                                                  to ApplCanTxObjStart, ApplCanTxObjConfirmed
 *                                                  Updated description of ApplCanMsgReceived
 *                         FHe                      Restored old format of ApplFblCheckProgConditions
 *                         Ci      ESCAN00033475    Added gateway specific callback
 *                                                  functions (Gw_Route support)
 *  01.35.00   2009-03-12  QPs     ESCAN00033801    Added communication wrapper specific
 *                                                  callback functions
 *  01.36.00   2009-07-21  QPs     ESCAN00035282    Added DC SLP10 extension:
 *                                                  ApplFblControlFailSafeReaction
 *                         AWh     ESCAN00036825    Inconsistent return value type between
 *                                                  ReadDataByCommonId prototype and implementation
 *  01.37.00   2009-08-31  AWh     ESCAN00037405    No changes
 *                         QPs     ESCAN00037479    Added FrTrcv specific callback functions
 *  01.38.00   2009-09-18  Tbe     ESCAN00037856    Added Ethernet/DoIP specific
 *                                                  callback functions
 *  01.39.00   2009-12-02  Ci      ESCAN00038917    Adapted prototype of
 *                                                  ApplFblFatalError()
 *                                 ESCAN00038918    Added support for validation
 *                                                  struct handling
 *                                 ESCAN00038919    Added callback for flash erased
 *                                                  detection
 *  01.40.00   2010-02-15  Ci      ESCAN00040663    Enhanced flash erased detection
 *                                                  interface
 *                         Ci      ESCAN00040852    Replaced decryption interface by
 *                                                  data processing API
 *  01.41.00   2010-07-08  MVi     ESCAN00043736    Remove pragmas when memmap is used
 *                         Ci      ESCAN00043863    Added function ApplCanTpConfirmation
 *                         QPs     ESCAN00043896    Added support for LIN bootloader
 *  01.42.00   2010-07-23  CB      ESCAN00044299    Default implementation of decompression
 *  01.43.00   2010-09-15  Ci      ESCAN00044991    Added new Gw_Route API functions
 *  01.44.00   2010-12-10  MVi     ESCAN00047478    Enhanced flash erased detection interface
 *                                 ESCAN00047482    Added routine control succeeded interface
 *  01.45.00   2011-02-03  Ach     ESCAN00048191    Use normal fixed addressing without TPMC
 *  01.46.00   2011-07-14  Ci      ESCAN00052149    Adapted Gw_Route task interface
 *  01.47.00   2011-11-25  Hp      ESCAN00052787    Extend support for PresencePatterns
 *  01.48.00   2011-12-14  AWh     ESCAN00055586    Extend Api with ApplFblInvalidateApp function
 *  01.49.00   2012-02-14  Ach     ESCAN00056341    Added ApplFblStartApplication() function
 *  01.50.00   2012-02-22  Ach     ESCAN00052787    Extend support for PresencePatterns
 *                         MVi     ESCAN00056833    Extend support for PresencePatterns (HMC)
 *                                 ESCAN00056834    No changes
 *                         CB      ESCAN00056642    Added ApplFblCanPlatformToPhysChannel()
 *  01.51.00   2012-03-23  QPs     ESCAN00057230    Extend support for FlexRay
 *                         Ach     ESCAN00057636    Remove local memSegment variables
 *                         CB      ESCAN00057670    Added Suspend Programming API
 *  01.52.00   2012-03-30  QPs     ESCAN00058040    No changes
 *  01.53.00   2012-08-16  MVi     ESCAN00060433    Support for PresencePatterns
 *                                                  with 256 byte segment size
 *  01.54.00   2012-12-06  AWh     ESCAN00061619    Moved presence pattern macros to header
 *                         Ach     ESCAN00062912    Adapted comments to use Doxygen
 *  01.55.00   2013-01-24  Ach     ESCAN00064406    Support return value IO_E_ERASED for presence pattern
 *                         JHg     ESCAN00064421    Added callback ApplFblOnTransitionSession
 *                                 ESCAN00064437    Added callback ApplFblStateTask
 *  01.56.00   2013-02-08  ASe     ESCAN00064889    Reworked comments
 *  01.57.00   2014-01-21  ASe     ESCAN00072931    Fixed compiler warning (HMC)
 *  01.58.00   2014-02-24  MVi     ESCAN00073549    Removed HMC exceptions to support new mtab header
 *  01.58.01   2014-04-03  ASe     ESCAN00074701    Fixed comments
 *  01.59.00   2014-07-16  QPs     ESCAN00077127    Extend support for LIN
 *  01.60.00   2014-08-25  Dod     ESCAN00078033    Add callbacks ApplFblMemPreWrite/Erase and ApplFblGetLogBlkHash
 *  01.61.00   2015-01-13  Ach     ESCAN00080481    Change visibility of ApplFblChkModulePresence()
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  REVISION HISTORY (KbFbl_apOem.c, GM SLP5 and SLP6)
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  05.00.00   2014-01-31  AWh                      First release
 *  05.01.00   2014-02-26  AWh     ESCAN00073863    Add user callbacks for Non readable Device use case
 *                                 ESCAN00073906    Enable Multi processor use case
 *  05.02.00   2014-07-14  AWh     ESCAN00075304    Compiler Error: Sleep-Mode enabled
 *                                                  SLP6: Add DID information available at Vector
 *                                                        Move DIDs from diagnostic layer to callback layer
 *  05.03.00   2014-10-15  AWh     ESCAN00078893    SLP6: Check for Default session in Core Diag style
 *                         AWh     ESCAN00078899    SLP6: Update ApplFblReadDataByIdentifier to allow for Core Diag
 *                         AWh     ESCAN00079661    Add compression interface
 *                         Dod     ESCAN00079983    SLP5: Incorrect reaction to securityAccess sendKey request
 *                         AWh     ESCAN00080211    SLP6: On transition from Appl, remove setting targetaddress,
 *                                                   which is redundantly done in ComWrapper
 *                                 -                Rework after Review
 *  05.04.00   2015-01-23  AWh     ESCAN00080768    No changes
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY (KbFbl_apHW.c, TMS470)
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  00.90.00   2004-01-15  CB                       First implementation
 *  01.00.00   2004-04-01  CB                       Release
 *  01.01.00   2004-11-24  Mhz                      Added function ApplFblIsValidApp(),
 *                                                  Minor changes
 *  01.02.00   2005-08-16  FHE                      Encapsulate customer specific SPI driver usage
 *  01.03.00   2006-04-25  WM      ESCAN00016092    Added version scan support
 *  01.04.00   2006-07-26  Ls      ESCAN00017072    added Dualprocessor functionality
 *  01.05.00   2007-11-06  Ls                       added watchdog code
 *  01.06.00   2009-02-27  JHg     ESCAN00033355    Added support for TI OMAP
 *  01.07.00   2009-09-22  Ci      ESCAN00036268    Added PLL and DWD support for
 *                                                  TMS470P family
 *                         JHg     ESCAN00037943    Perform global system reset (OMAP only)
 *  01.08.00   2010-03-26  Pal     ESCAN00041890    Add support for C_PROCESSOR_TMS470_MSF542
 *  01.09.00   2010-03-26  AWh     ESCAN00055193    Use linker WD-copy for older TI TMS470
 *  01.10.00   2013-07-04  Ach     ESCAN00061452    Support TMS470MF0420
 *                         ASe     ESCAN00068154    Adapted comments to use doxygen
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "fbl_inc.h"
#include "fbl_wd.h"

/***********************************************************************************************************************
 *  VERSION
 **********************************************************************************************************************/

#if ( FBLKBAPI_FRAMEWD_VERSION != 0x0310u ) || \
    ( FBLKBAPI_FRAMEWD_RELEASE_VERSION != 0x00u )
# error "Error in FBL_APWD.C: Source and header file are inconsistent!"
#endif

#if ( FBLKBAPI_FRAMEWD_VERSION != _FBLKBAPI_FRAMEWD_VERSION ) || \
    ( FBLKBAPI_FRAMEWD_RELEASE_VERSION != _FBLKBAPI_FRAMEWD_RELEASE_VERSION )
# error "Error in FBL_APWD.C: Source and V_VER.H are inconsistent!"
#endif

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL DATA
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  EXTERNAL DATA
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  PROTOTYPES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  ApplFblWDInit
 **********************************************************************************************************************/
/*! \brief       Initialize watchdog.
 *  \details     WDTimer has to be initialized here.
 **********************************************************************************************************************/
void ApplFblWDInit( void )
{
#if defined( FBL_WATCHDOG_ON )
   WDTimer = (tWdTime)FBL_WATCHDOG_TIME;
   
   FBL_RTIDWDPRLD = 0xFF;
   FBL_RTIDWDCTRL = 0xA98559DA;
   FBL_RTIDWDKEY = 0xE51A;
#endif /* FBL_WATCHDOG_ON */

}

/***********************************************************************************************************************
 *  ApplFblWDLong
 **********************************************************************************************************************/
/*! \brief       Synchronize window watchdog for application.
 *  \details     This function is called just before application start.
 **********************************************************************************************************************/
void ApplFblWDLong( void )
{
}

/***********************************************************************************************************************
 *  ApplFblWDShort
 **********************************************************************************************************************/
/*! \brief       Synchronize window watchdog for bootloader.
 *  \details     This function is called right after start of the bootloader from the application.
 **********************************************************************************************************************/
void ApplFblWDShort( void )
{
#if defined( FBL_WATCHDOG_ON )
   WDTimer = 1;
#endif
}

# define WDTRIGGER_START_SEC_CODE
# include "memmap.h"   /* PRQA S 5087 *//* MD_MSR_19.1 */
/***********************************************************************************************************************
 *  ApplFblWDTrigger
 **********************************************************************************************************************/
/*! \brief       Cyclic function to handle the actual watchdog trigger.
 *  \details     This function is called by FblLookForWatchdog() and contains the actual watchdog trigger
 *               code. Depending on the configuration, please note the following points:
 *               - If the function is copied by FblCopyWatchdog(), it has to be relocatable and normally
 *                 no function calls out of this function are allowed.
 *               - If the function is placed to RAM by the linker, it has to be present before the first call.
 *                 If any function calls are done, the called functions have to be placed in RAM as well.
 *  \pre         This function has to be executed from RAM.
 **********************************************************************************************************************/
void V_CALLBACK_NEAR ApplFblWDTrigger( void )
{
#if defined(FBL_WATCHDOG_ON)  
	FBL_RTIDWDKEY = 0xA35C;
	FBL_RTIDWDKEY = 0xE51A;
#endif
}
# define WDTRIGGER_STOP_SEC_CODE
# include "memmap.h"   /* PRQA S 5087 *//* MD_MSR_19.1 */

/* Additional watchdog functions **********************************************/

/***********************************************************************************************************************
 *  END OF FILE: FBL_APWD.C
 **********************************************************************************************************************/

