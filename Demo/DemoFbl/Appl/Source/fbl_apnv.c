/* Kernbauer Version: 1.12 Konfiguration: FBL Erzeugungsgangnummer: 1 */

/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief         Application dependent non-volatile memory access routines
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
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY (KbFbl_apnvFrameOem.c, GM SLP5 and SLP6)
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  05.00.00   2014-01-31  AWh                      First release
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

/***********************************************************************************************************************
 *   VERSION
 **********************************************************************************************************************/

#if ( FBLKBAPI_FRAMENV_GM_VERSION != 0x0500u )
# error "Error in fbl_apnv.c: Source and Header file are inconsistent!"
#endif
#if ( FBLKBAPI_FRAMENV_GM_RELEASE_VERSION != 0x00u )
# error "Error in fbl_apnv.c: Source and Header file are inconsistent!"
#endif

#if ( FBLKBAPI_FRAMENV_GM_VERSION != _FBLKBAPI_FRAMENV_OEM_VERSION ) || \
    ( FBLKBAPI_FRAMENV_GM_RELEASE_VERSION != _FBLKBAPI_FRAMENV_OEM_RELEASE_VERSION )
# error "Error in fbl_apnv.c: Source and v_ver.h are inconsistent!"
#endif

#if ( FBLKBAPI_GM_VERSION != 0x0504u )
# error "Error in fbl_apXX.c: Source and Header file are inconsistent!"
#endif
#if ( FBLKBAPI_GM_RELEASE_VERSION != 0x00u )
# error "Error in fbl_apXX.c: Source and Header file are inconsistent!"
#endif

#if ( FBLKBAPI_GM_VERSION != _FBLKBAPI_OEM_VERSION ) || \
    ( FBLKBAPI_GM_RELEASE_VERSION != _FBLKBAPI_OEM_RELEASE_VERSION )
# error "Error in fbl_ap.c: Source and v_ver.h are inconsistent!"
#endif

#if defined( FBL_ENABLE_COMPRESSION_MODE )
# if( FBLDIAGHDR_GM_VERSION < 0x200u )
#  error "Error in fbl_hdr.c: Version smaller than 2.00 does not have compression interface."
# endif
#endif

#if ( FBLKBAPI_TMS470HECCSCC_VERSION != 0x0110 ) || \
    ( FBLKBAPI_TMS470HECCSCC_RELEASE_VERSION != 0x00 )
# error "Error in FBL_APxx.C: Source and header file are inconsistent!"
#endif

#if ( FBLKBAPI_TMS470HECCSCC_VERSION != _FBLKBAPI_HW_VERSION ) || \
    ( FBLKBAPI_TMS470HECCSCC_RELEASE_VERSION != _FBLKBAPI_HW_RELEASE_VERSION )
# error "Error in FBL_APxx.C: Source and V_VER.H are inconsistent!"
#endif

#if ( FBLKBAPI_VERSION != 0x0161u ) || \
    ( FBLKBAPI_RELEASE_VERSION != 0x00u )
# error "Error in FBL_APxx.C: Source and header file are inconsistent!"
#endif
#if ( FBLKBAPI_VERSION != _FBLKBAPI_VERSION ) || \
    ( FBLKBAPI_RELEASE_VERSION != _FBLKBAPI_RELEASE_VERSION )
# error "Error in FBL_APxx.C: Source and V_VER.H are inconsistent!"
#endif

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/* Change ApplFblCalsPresent() implementation/prototype if there may be need to report
 * kDiagProgStateDefOrNoStartCal on $A2 service.
 */
#define STANDARD_CALS                ((vuint8)1)
#define DEFAULT_OR_NO_START_CALS     ((vuint8)0)
#define ApplFblCalsPresent() (STANDARD_CALS)
/* PRQA S 3453 TAG_FblApNv_3453_1 */ /* MD_CBD_19.7 */
/* Memory driver access */
#define ApplFblReadPattern(buffer, address)           (MemDriver_RReadSync((IO_MemPtrType)(buffer), (IO_SizeType)kFblPresencePatternSize, (IO_PositionType)(address)))
#define ApplFblWritePattern(buffer, length, address)  (MemDriver_RWriteSync((IO_MemPtrType)(buffer), (IO_SizeType)(length), (IO_PositionType)(address)) == IO_E_OK)

V_MEMROM0 static V_MEMROM1 vuint8 V_MEMROM2 blockPresencePattern[kFblPresencePatternSize] = kFblPresencePattern;
/* PRQA L:TAG_FblApNv_3453_1 */

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

static vuint8 ApplFblChkOpSwProgrammedState(vuint8 basePartId);
static tFblResult ApplFblSetModulePresence( tBlockDescriptor * blockDescriptor );
static tFblResult ApplFblClrModulePresence( tBlockDescriptor * blockDescriptor );
tFblResult ApplFblChkModulePresence( tBlockDescriptor * blockDescriptor );

/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/* set this define, e.g. in fbl_apnv.h in order to implement solutions that better use functions, than macros */
#if defined( FBL_APNV_USE_FUNCTIONS )
/***********************************************************************************************************************
 *  ApplFblNVMReadSBATicket
 **********************************************************************************************************************/
/*! \brief       Fills buffer with SBA ticket from SBA storage location.
 *  \param[out] buffer to read SBA ticket to
 *  \return      kFblOk / kFblFailed result
 **********************************************************************************************************************/
tFblResult ApplFblNVMReadSBATicket( V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * const buffer )
{
   /* NvWrapper function for usage with EepMgr or hw-Eeprom */
   return ApplFblNvReadSBA_ticket(0, buffer);
}

/***********************************************************************************************************************
 *  ApplFblReadECUID
 **********************************************************************************************************************/
/*! \brief       Fills buffer with ECU ID from ECUID storage location.
 *  \param[out]   buffer to read ECUID to
 *  \return      kFblOk / kFblFailed result
 **********************************************************************************************************************/
tFblResult ApplFblNVMReadECUID( V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * const buffer )
{
   /* customer TODO: Please read unique ECU_ID from storage location
    *
    * BE CAREFUL: A process is in place to guarantee this is a UNIQUE value for each given ECU.
    */

   /* NvWrapper function for usage with EepMgr or hw-Eeprom */
   return ApplFblNvReadECU_ID(0, buffer);
}

/***********************************************************************************************************************
 *  ApplFblNVMReadKeyNBID
 **********************************************************************************************************************/
/*! \brief       Read keyNBID to keyNBID->stored out of NVM
 *  \param[out]   Info structure holding buffer to read KeyNBID to
 *  \return      kFblOk / kFblFailed
 **********************************************************************************************************************/
tFblResult ApplFblNVMReadKeyNBID( V_MEMRAM1 tNBIDInfo V_MEMRAM2 V_MEMRAM3 * const keyNBIDInfo )
{
   /* After discussion with GM: Security is more important then download ability in case of read error. */
   /* Make sure ApplFblNVMReadKeyNBID is as reliable as possible! */

   /* NvWrapper function for usage with EepMgr or hw-Eeprom */
   return ApplFblNvReadKey_NBID(0, (V_MEMRAM1 vuint8 V_MEMRAM2*)&keyNBIDInfo->stored);
}

/***********************************************************************************************************************
 *  ApplFblNVMWriteKeyNBID
 **********************************************************************************************************************/
/*! \brief       Update Key-NBID in NVM
 *  \param[in]   Info structure holding buffer that holds new Key-NBID to write
 *  \return      kFblOk / kFblFailed
 **********************************************************************************************************************/
tFblResult ApplFblNVMWriteKeyNBID( const V_MEMRAM1 tNBIDInfo V_MEMRAM2 V_MEMRAM3 * const keyNBIDInfo )
{
   /* KeyNBID could be written in application from Ram content hold over reset, if bootloader shall not write it.
    * (Read only NVM Design in boot, please evaluate security issues )
    */
   /* NvWrapper function for usage with EepMgr or hw-Eeprom */
   return ApplFblNvWriteKey_NBID(0, (V_MEMRAM1 vuint8 V_MEMRAM2*)&keyNBIDInfo->received);
}
#endif
#if defined( FBL_APNV_USE_FUNCTIONS ) || defined( FBL_ENABLE_MULTI_PROCESSOR )
/***********************************************************************************************************************
 *  ApplFblNVMReadAppNBID
 **********************************************************************************************************************/
/*! \brief       Return App-NBID out of NVM
 *  \param[out]   Info structure holding buffer to read AppNBID to
 *  \return      kFblOk / kFblFailed
 **********************************************************************************************************************/
tFblResult ApplFblNVMReadAppNBID( V_MEMRAM1 tNBIDInfo V_MEMRAM2 V_MEMRAM3 * const appNBIDInfo )
{
   /* After discussion with GM: Security is more important then download ability in case of read error. */
   /* Make sure ApplFblNVMReadAppNBID is as reliable as possible! */
#if defined( FBL_ENABLE_MULTI_PROCESSOR )
   if(appNBIDInfo->MID == MODID_OPSW_2)
   {
       return ApplFblNvReadApp_NBID_2ndApp(0, (V_MEMRAM1 vuint8 V_MEMRAM2*)&appNBIDInfo->stored);
   }
   else
#endif
   {
      /* NvWrapper function for usage with EepMgr or hw-Eeprom */
      return ApplFblNvReadApp_NBID(0, (V_MEMRAM1 vuint8 V_MEMRAM2*)&appNBIDInfo->stored);
   }
}
/***********************************************************************************************************************
 *  ApplFblNVMWriteApplNBID
 **********************************************************************************************************************/
/*! \brief       Update App-NBID in NVM
 *  \param[in]   Info structure holding buffer that holds new App-NBID to write
 *  \return      kFblOk / kFblFailed
 **********************************************************************************************************************/
tFblResult ApplFblNVMWriteAppNBID( const V_MEMRAM1 tNBIDInfo V_MEMRAM2 V_MEMRAM3 * const appNBIDInfo )
{
   /* AppNBID could be written in application from Header location, if bootloader shall not write it
    * (Read only NVM Design in boot)
    */
#if defined( FBL_ENABLE_MULTI_PROCESSOR )
   if(appNBIDInfo->MID == MODID_OPSW_2)
   {
       return ApplFblNvWriteApp_NBID_2ndApp(0, (V_MEMRAM1 vuint8 V_MEMRAM2*)&appNBIDInfo->received);
   }
   else
#endif
   {
      /* NvWrapper function for usage with EepMgr or hw-Eeprom */
      return ApplFblNvWriteApp_NBID(0, (V_MEMRAM1 vuint8 V_MEMRAM2*)&appNBIDInfo->received);
   }
}
#endif

/***********************************************************************************************************************
 *  ApplFblGetBaseModulePPRegion
 **********************************************************************************************************************/
/*! \brief       This function has to return address and length of the covered presence pattern region (mask and pattern).
 *  \details     Only Application modules (being base modules) have to be configured.
 *               Note that these are the modules that have to be configured in Logical Block Table.
 *  \param[in]   partId maps to blockNbr entry of Logical block table
 *               Be careful: this is not the "Block Index" information in GENy.
 *  \param[out]  *pPresPtnAddr Presence Pattern Area start address for the given module
 *  \param[out]  *pPresPtnLen Presence Pattern Area length for the given module (for mask and pattern)
 **********************************************************************************************************************/
void ApplFblGetBaseModulePPRegion(vuint8 partId, IO_PositionType *pPresPtnAddr, IO_SizeType *pPresPtnLen )
{
   /* The module PP can be placed                                                               *
    * - relative to ApplFblGetModuleHeaderAddress                                               *
    *   with an offset large enough to contain the application or                               *
    * - statically to fix address (within logical block range, after module header address).    */
   vuint8 blockNr = GetBlockNbrFromPartId(partId);

   *pPresPtnAddr = FblLogicalBlockTable.logicalBlock[blockNr].presencePatternAddress;
   /* Return 2 times presence pattern size for mask + pattern */
   *pPresPtnLen = 2*FBL_PP_SEGMENT_SIZE;

   /* Check start address defined (tool checks if included in Logical Block Table) */
   assertFblUser(kFblDiagMemSegmNotFound != FblMemSegmentNrGet(*pPresPtnAddr),  kFblAssertParameterValid);
   /* Check Psi Region end equals end of physical block (Gm/Vector requirement) */
   assertFblUser((FlashBlock[FblMemSegmentNrGet(*pPresPtnAddr)].end  == (*pPresPtnAddr + (*pPresPtnLen-1))), kFblAssertParameterValid);
}

/***********************************************************************************************************************
 *  ApplFblGetModuleHeaderAddress
 **********************************************************************************************************************/
/*! \brief       This function has to return the address of a module header.
 *  \details     Only Application and Reserved Modules have to be configured.
 *               These are the modules that have to be configured in Logical Block Table.
 *
 *  \pre         Logical blocks need a module header
 *  \param[in]   blockNr describes entry number of Logical block
 *               Be careful: this is not the "Block Index" information in GENy.
 *               It is simply the number of the entry: 0 - First entry (usually appl with Block Index/MID of 1 )
 *  \return      Address of module header information structure
 **********************************************************************************************************************/
tFblAddress ApplFblGetModuleHeaderAddress( vuint8 blockNr )
{
   /* The module header may be placed at e.g.                                 *
    * - the beginning of the logical block (offset = +0x00 ) or               *
    * - with an offset to the beginning of the logical block (offset = +0xXX) *
    * - with an offset to the end of the logical block (offset = -0xXX)       *
    * - statically to fix address (within logical block range).               */

   return FblLogicalBlockTable.logicalBlock[blockNr].headerAddress;
}

/***********************************************************************************************************************
 *  ApplFblChkPSIState
 **********************************************************************************************************************/
/*! \brief       Determine PSI state for partition
 *  \param[in]   partId Partition Id of application or calibration file:
 *                 For example Partition Numbers (in hex) have the following meaning.
 *                 0x01 : Main processor Application SW Partition
 *                 0x02 : Main processor 1st Cal Partition
 *                 0x03 : Main processor 2nd Cal Partition
 *                 ..
 *                 0x11 : 2nd processor Application SW Partition
 *                 0x12 : 2nd processor 1st Cal Partition
 *                 0x13 : 2rd processor 2nd Cal Partition
 *
 *  \return      PSI_PART_PRESENT: Presence pattern are set and Mask value are OK
 *               PSI_PART_REVOKED: Presence pattern state is revoked
 *               PSI_PART_INVALID: Presence pattern not valid and state is not revoked
 **********************************************************************************************************************/
tPartPresState ApplFblChkPSIState( vuint8 partId )
{
   tPartPresState retVal = PSI_PART_INVALID;

   if (kFblOk == ApplFblChkModulePresence(FblGetBlockDescriptor(partId)))
   {
      retVal = PSI_PART_PRESENT;
   }
   else
   {
     IO_PositionType  presPtnAddress;
     IO_SizeType      presPtnLen;

      memSegment = ApplFblGetPresencePatternBaseAddress(partId, &presPtnAddress, &presPtnLen);
      if (memSegment >= 0)
      {
         vuint8 buffer[kFblPresencePatternSize];

         if (IO_E_OK == ApplFblReadPattern(buffer, (presPtnAddress+presPtnLen)))
         {
            vuint16 maskReference = FblInvert16Bit( (vuint16) FblMemGetInteger(kFblPresencePatternSize, blockPresencePattern) );
            vuint16 maskRead = (vuint16) FblMemGetInteger(kFblPresencePatternSize, buffer);

            if (maskReference == maskRead)
            {
               retVal = PSI_PART_REVOKED;
            }
         }
      }
   }

   return retVal;
}

/***********************************************************************************************************************
 *  ApplFblChkOpSwProgrammedState
 **********************************************************************************************************************/
/*! \brief       check on given opSwID the Programmed State of OpSW and its calibration files.
 *  \param[in]   basePartId
 *  \return      kDiagProgStateFullyProgrammed: Appl and all Cals are present.
 *               kDiagProgStateNoSoftwareOrCal: Appl not present.
 *               kDiagProgStateNoCalibration: Appl present, some Cal is missing.
 **********************************************************************************************************************/
static vuint8 ApplFblChkOpSwProgrammedState(vuint8 basePartId)
{
   vuint8 retVal = kDiagProgStateFullyProgrammed;
   tPartPresState partPresState = ApplFblChkPSIState(basePartId);

   /* module id == partition Id in case of base modules */
   if (PSI_PART_PRESENT != partPresState)
   {
      retVal = kDiagProgStateNoSoftwareOrCal;
   }
#if defined( FBL_ENABLE_CALIBRATION_MODULES )
   else
   {
      if (kFblOk != FblHdrGetCalPartitionValid(basePartId))
      {
         retVal = kDiagProgStateNoCalibration;
      }
   }
#endif /* FBL_ENABLE_CALIBRATION_MODULES */

   return retVal;
}
/***********************************************************************************************************************
 *  ApplFblGetProgrammedState
 **********************************************************************************************************************/
/*! \brief       Checks if the application and/or calibration presence pattern all set.
 *  \return      kDiagProgStateFullyProgrammed: Appl and all Cals are present.
 *               kDiagProgStateNoSoftwareOrCal: Appl not present.
 *               kDiagProgStateNoCalibration: Appl present, some Cal is missing.
 *               kDiagProgStateDefOrNoStartCal: Appl present, all Cals are present (special Cals)
 *               (Will never be reported currently, e.g. change
 *               ApplFblCalsPresent() implementation to detect default/no start calls
 *               to allow this return value).
 **********************************************************************************************************************/
vuint8 ApplFblGetProgrammedState(void)
{
   if (ApplFblCalsPresent() != STANDARD_CALS)
   {
      return kDiagProgStateDefOrNoStartCal;
   }
   else
   {
      return ApplFblChkOpSwProgrammedState(PARTID_OPSW_1);
   }
}
/***********************************************************************************************************************
 *  ApplFblGetPresencePatternBaseAddress
 **********************************************************************************************************************/
/*! \brief       Returns the base address of the presence pattern and mask and the length of both fields.
 *  \param[in]   blockNr module ID of a downloaded module.
 *  \param[out]   pPresPtnAddr Pointer to RAM location to place the address to the begin of presence pattern region
 *                (pattern, not mask)
 *  \param[out]   pPresPtnLen Pointer to the RAM location where the length of the presence pattern shall be stored to.
 *                The length is the length of the pattern only (mask is excluded).
 *  \return      memSegment of the presence pattern location or kFblDiagMemSegmNotFound in case of an error.
 **********************************************************************************************************************/
vsint16 ApplFblGetPresencePatternBaseAddress(vuint8 blockNr, IO_PositionType *pPresPtnAddr, IO_SizeType *pPresPtnLen)
{
   vuint8 partition = blockNr;

   FblHdrGetPsiRegion(partition, pPresPtnAddr, pPresPtnLen);

   /* Region consists of two equal sized sections for mask and pattern */
   *pPresPtnLen /=2;

   if (NO_PP_ADDRESS_DEFINED == *pPresPtnAddr)
   {
      return kFblDiagMemSegmNotFound;
   }
   else
   {
      return FblMemSegmentNrGet(*pPresPtnAddr);
   }
}

#if defined( FBL_ENABLE_ROM_INTEGRITY_CHECK ) || defined( FBL_ENABLE_VERIFY_INTEGRITY_WORD )
/***********************************************************************************************************************
 *  ApplFblUpdateChecksum
 **********************************************************************************************************************/
/*! \brief       Checksum calculation Update function for His Security module Interface
 *  \param[in]   checksum The checksum
 *  \param[in]   regLen Length of the region
 *  \param[in]   regStartAddr Start address of the region
 *  \return      kFblOk / kFblFailed
 **********************************************************************************************************************/
tFblResult ApplFblUpdateChecksum(V_MEMRAM1 vuint16 V_MEMRAM2 * const checksum, SecM_LengthType regLen, const V_MEMRAM1 vuint8 V_MEMRAM2 * const regStartAddr)
{
#if defined( FBL_ENABLE_SYSTEM_CHECK )
   if ( ((regLen & (tFblLength)1) != (tFblLength)0) || (((tFblLength)regStartAddr & (tFblLength)1) != (tFblLength)0))
   {
      return kFblFailed;
   }
#endif

  *checksum = FblCalculateCsum16Bit( (const V_MEMRAM1 vuint16 V_MEMRAM2*) regStartAddr, regLen, *checksum);
  return kFblOk;
}

/***********************************************************************************************************************
 *  ApplFblFinalizeChecksum
 **********************************************************************************************************************/
/*! \brief       Checksum calculation finalize function for His Security module Interface
 *  \param[in]   checksum The checksum
 *  \return      kFblOk / kFblFailed
 **********************************************************************************************************************/
tFblResult ApplFblFinalizeChecksum(V_MEMRAM1 vuint16 V_MEMRAM2 * const checksum)
{
   if (*checksum == 0)
   {
      return kFblOk;
   }

   return kFblFailed;
}
#endif

/***********************************************************************************************************************
 *  ApplFblFillGaps
 **********************************************************************************************************************/
/*! \brief       This function guarantees that all memory associated to a module is being filled with
 *               the configured fill pattern.
 *  \return      kFblOk if regions were successfully saved.
 *               kFblFailed otherwise
 **********************************************************************************************************************/
tFblResult ApplFblFillGaps( void )
{
#if defined( FBL_ENABLE_GAP_FILL )
   return FblHdrFillGaps();
#else
   /* You may implement your own Gap fill here, e.g. by using fill segments attached to application at
    * static address location, protected with some flag indicator (after Plain Header header).
    * This way you might be able to save code size.
    */
   return kFblOk;
#endif
}

/***********************************************************************************************************************
 *  ApplFblModuleSetPresence
 **********************************************************************************************************************/
/*! \brief       Writes the presence pattern into the flash memory.
 *  \details     The location of the presence pattern will be taken from the logical block descriptor.
 *  \pre         Module programming successfully performed and checksum calculation succeeded.
 *  \param[in]   blockDescriptor Pointer to the logical block descriptor
 *  \return      kFblOk: Presence pattern successfully set, kFblFailed: Error writing presence pattern
 **********************************************************************************************************************/
static tFblResult ApplFblSetModulePresence( tBlockDescriptor * blockDescriptor )
{
   IO_PositionType  presPtnAddress;
   IO_SizeType      presPtnLen;
   IO_ErrorType     readResult;
   vuint32          i;
   tFblResult       result;
   vuint16          flashHeader[(FBL_PP_SEGMENT_SIZE + 1) / 2];
   vuint8           *pFlashHeader;

   /* Set presence pattern for this module */

   /* Calculate location of presence pattern.           */
   /* Note that the end of the block descriptor already */
   /* points to the presence pattern itself.            */
   memSegment = ApplFblGetPresencePatternBaseAddress(blockDescriptor->blockNr, &presPtnAddress, &presPtnLen);

   if (memSegment >= 0)
   {
      /* FlashWrite requires an even address to RAM */
      pFlashHeader = (vuint8 *)flashHeader;

      /* Copy presence pattern to RAM buffer */
      for (i = 0u; i < kFblPresencePatternSize; i++)
      {
         pFlashHeader[i] = blockPresencePattern[i];
      }
#if ( FBL_PP_SEGMENT_SIZE > kFblPresencePatternSize )
      /* Clear remaining buffer if any */
      for (i = kFblPresencePatternSize; i < FBL_PP_SEGMENT_SIZE; i++)
      {
         pFlashHeader[i] = 0;
      }
#endif
      (void)FblRealTimeSupport();

      /* Write presence pattern */
      if (!ApplFblWritePattern(pFlashHeader, presPtnLen, presPtnAddress))
      {
         return kFblFailed;
      }

      /* Read mask pattern */
      readResult = ApplFblReadPattern(pFlashHeader, (presPtnAddress+presPtnLen));
      if ((readResult != IO_E_OK) && (readResult != IO_E_ERASED))
      {
         return kFblFailed;
      }

      /* Check if the erase pattern is set correctly */
      for (i = 0u; i < kFblPresencePatternSize; i++)
      {
         if (pFlashHeader[i] != FBL_FLASH_DELETED)
         {
            return kFblFailed;
         }
      }

      /* Return value depends on the Check if presence pattern is now valid */
      result = ApplFblChkModulePresence(blockDescriptor);
   }
   else
   {
      /* Memory segment for the presence pattern address not found */
      result = kFblFailed;
   }

   return result;
}  /* PRQA S 2006 */ /* MD_CBD_14.7 */
/***********************************************************************************************************************
 *  ApplFblModuleClrPresence
 **********************************************************************************************************************/
/*! \brief       Sets the mask presence pattern in flash memory to invalidate the block.
 *  \details     The location of the presence pattern will be taken from the logical block descriptor.
 *  \pre         Module shall be marked as invalid now.
 *  \param[in]   blockDescriptor Pointer to the logical block descriptor
 *  \return      kFblOk: Mask for invalidation successfully written, kFblFailed: Error writing invalidation mask
 **********************************************************************************************************************/
static tFblResult ApplFblClrModulePresence( tBlockDescriptor * blockDescriptor )
{
   tFblResult       result = kFblOk;
   vuint32          i;
   IO_PositionType  presPtnAddress;
   IO_SizeType      presPtnLen;
   vuint16          flashHeader[(FBL_PP_SEGMENT_SIZE + 1) / 2];
   vuint8           *pFlashHeader;

   /* Calculate location of presence pattern.           */
   /* Note that the end of the block descriptor already */
   /* points to the presence pattern itself.            */
   memSegment = ApplFblGetPresencePatternBaseAddress(blockDescriptor->blockNr, &presPtnAddress, &presPtnLen);

   /* valid segment found */
   if (memSegment >= 0)
   {
     /* FlashWrite requires an even address to RAM */
     pFlashHeader = (vuint8 *)flashHeader;

      /* Check if presence pattern is set for this module */
      /* Only clear and remove the marker if not already disabled */
      if (ApplFblChkModulePresence(blockDescriptor) == kFblOk)
      {
         /* Copy presence pattern to RAM buffer */
         for (i = 0u; i < kFblPresencePatternSize; i++)
         {
            /* Set the inverse of the presence pattern */
            pFlashHeader[i] = FblInvert8Bit(blockPresencePattern[i]);
         }
         /* Clear remaining buffer if any */
         for (i = kFblPresencePatternSize; i < FBL_MAX_SEGMENT_SIZE; i++)
         {
            pFlashHeader[i] = 0;
         }
         /* Now write them */
         if (!ApplFblWritePattern(pFlashHeader, presPtnLen, (presPtnAddress + presPtnLen)))
         {
            /* Writing the mask has failed. */
            result = kFblFailed;
         }
         (void)FblRealTimeSupport();
      }
   }
   else
   {
      /* Check if presence pattern is set for this module */
      /* Only clear and remove the marker if not already disabled */
      if (ApplFblChkModulePresence(blockDescriptor)==kFblOk)
      {
         result = kFblFailed;
      }
   }

   return result;
}
/***********************************************************************************************************************
 *  ApplFblChkModulePresence
 **********************************************************************************************************************/
/*! \brief       Checks if mask and value of the presence pattern are set for a valid module.
 *  \details     The location of the presence pattern and mask is taken from the logical block table in FBL_MTAB.C.
 *  \param[in]   blockDescriptor Pointer to the logical block descriptor
 *  \return      kFblOk:     Presence pattern are set and Mask value are OK,
 *               kFblFailed: Presence pattern not set or mask flag not correct.
 **********************************************************************************************************************/
tFblResult ApplFblChkModulePresence( tBlockDescriptor * blockDescriptor )
{
   vuint8           flashPresPtn[kFblPresencePatternSize];
   vuint8           flashPresMsk[kFblPresencePatternSize];
   IO_PositionType  presPtnAddress;
   IO_SizeType      presPtnLen;
   IO_ErrorType     readResult;
   vuint32          i;
   tFblResult       result;

   /* Calculate location of presence pattern.           */
   /* Note that the end of the block descriptor already */
   /* points to the presence pattern itself.            */
   memSegment = ApplFblGetPresencePatternBaseAddress(blockDescriptor->blockNr, &presPtnAddress, &presPtnLen);

   if (memSegment >= 0)
   {
      /* Read presence pattern */
      readResult = ApplFblReadPattern(flashPresPtn, presPtnAddress);
      if ((readResult != IO_E_OK) && (readResult != IO_E_ERASED))
      {
         /* Read has failed */
         result = kFblFailed;
      }
      else
      {
         /* Read presence pattern */
         readResult = ApplFblReadPattern(flashPresMsk, (presPtnAddress + presPtnLen));
         if ((readResult != IO_E_OK) && (readResult != IO_E_ERASED))
         {
            /* Read has failed */
            result = kFblFailed;
         }
         else
         {
            result = kFblOk;
            for (i = 0u; ((i < kFblPresencePatternSize) && (kFblOk == result)); i++)
            {
               /* Compare the PP-value against the expected one */
               if ((flashPresMsk[i] ^ flashPresPtn[i]) != (blockPresencePattern[i] ^ FBL_FLASH_DELETED))
               {
                  /* PP value is different or MASK does not have its expected value */
                  result = kFblFailed;
               }
            }
         }
      }
   }
   else
   {
      result = kFblFailed;
   }

   return result;
}

/***********************************************************************************************************************
 *  ApplFblExtProgRequest
 **********************************************************************************************************************/
/*! \brief       Function is called on power-on reset to check if an external programming request exists.
 *  \details     If the flag is set, the flag is cleared and the function returns kProgRequest.
 *  \return      kProgRequest = external programming request, kNoProgRequest = programming request flag not set
 **********************************************************************************************************************/
tFblProgStatus ApplFblExtProgRequest( void )
{
#if defined( FBL_ENABLE_FBL_START )
   if (FblChkFblStartMagicFlag() == 0)
   {
      /* No programming request was received*/
      return kNoProgRequest;
   }
   else /* MagicFlags are set */
   {
      /* Clear magic flags */
      FblClrFblStartMagicFlag();
      /* Indicate that FBL started from APPL */
      SetFblMode(START_FROM_APPL);

      /* bootloader shall always send positive response*/
      /* to the programming request received in the application */
      return kProgRequest;

   }
#else
   /* TODO: Add functionality, e.g. function ApplFblReadCanInitTable(tCanInitTable * InitTable) to read required CanInittable parameters
    *       after Reset
    */
   ApplFblReadCanInitTable(&CanInitTable);

   /* TODO: Add functionality, e.g. function ApplFblReadProgrammingFlag(..) to read required Reprogramming flag information.
    *       Return kProgRequest if found to be set.
    */

   /* Check flag for reprogramming request */
   return kNoProgRequest;
#endif
}
/***********************************************************************************************************************
 *  ApplFblIsValidApp
 **********************************************************************************************************************/
/*! \brief       Function is called on power-up to check if the application is successfully validated or not.
 *  \details     Returns kApplValid if application is valid (all mandatory blocks available) and application start is
 *               allowed. Otherwise it returns kApplInvalid.
 **********************************************************************************************************************/
tApplStatus ApplFblIsValidApp( void )
{
   vuint8 applValidity = kApplInvalid;
   /* default checks Programmed State for two OPSW */
   if ((kDiagProgStateFullyProgrammed == ApplFblChkOpSwProgrammedState(PARTID_OPSW_1))
#if defined( FBL_ENABLE_MULTI_PROCESSOR )
        && (kDiagProgStateFullyProgrammed == ApplFblChkOpSwProgrammedState(PARTID_OPSW_2))
#endif
      )
   {
      applValidity = kApplValid;
   }

#if defined( FBL_ENABLE_DEBUG_STATUS )
   /* This check is useful for development purpose but shall be removed for production.
    * (Remember: Removal should be automatic, as the option debug status is to be disabled for production.  ) */
   if (FblCheckBootVectTableIsValid())
   {
      applValidity = kApplInvalid;
   }
#endif

   return applValidity;
}  /* PRQA S 2006 */ /* MD_CBD_14.7 */
/***********************************************************************************************************************
 *  ApplFblValidateBlock
 **********************************************************************************************************************/
/*! \brief       Function is called after a successful download (CRC check was successful) to validate the logical block
 *  \pre         Logical block must be successfully downloaded
 *  \param[in]   blockDescriptor Block descriptor structure
 *  \return      kFblOk = validation successfully performed, kFblFailed = validation failed
 **********************************************************************************************************************/
tFblResult ApplFblValidateBlock( tBlockDescriptor blockDescriptor )
{
   return ApplFblSetModulePresence(&blockDescriptor);
}
/***********************************************************************************************************************
 *  ApplFblInvalidateBlock
 **********************************************************************************************************************/
/*! \brief       Whenever the bootloader needs to delete data, this function is called
 *               to invalidate the current logical block.
 *  \param[in]   blockDescriptor Block descriptor structure
 *  \return      kFblOk: invalidation successfully performed, kFblFailed: invalidation failed
 **********************************************************************************************************************/
tFblResult ApplFblInvalidateBlock( tBlockDescriptor blockDescriptor )
{
   if (kFblOk != ApplFblClrModulePresence(&blockDescriptor))
   {
#if defined( FBL_ENABLE_DEBUG_STATUS )
      FblErrDebugStatus(kDiagErrNoMemoryRegionFound);
      FblErrDebugDriver(0, IO_E_OK);
#endif
      DiagNRCGeneralProgFailure();
      return kFblFailed;
   }

   return kFblOk;
}

/***********************************************************************************************************************
 *  END OF FILE: KBFBL_APNVFRAMEOEM.C
 **********************************************************************************************************************/

