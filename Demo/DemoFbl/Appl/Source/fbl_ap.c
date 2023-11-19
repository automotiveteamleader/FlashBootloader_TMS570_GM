/* Kernbauer Version: 1.12 Konfiguration: FBL Erzeugungsgangnummer: 1 */

/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief         Application dependent routines
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
 *  REVISION HISTORY (KbFbl_apFrameOem.c, GM SLP5 and SLP6)
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

#if (FBL_PROJECT_STATE >= FBL_INTEGRATION)
# if defined ( FBL_TEST_SBA_TICKET )
#include "dummySba.h"
# endif
#endif

/***********************************************************************************************************************
 *   VERSION
 **********************************************************************************************************************/

#if ( FBLKBAPI_FRAME_GM_VERSION != 0x0500u )
# error "Error in fbl_ap.c: Source and Header file are inconsistent!"
#endif
#if ( FBLKBAPI_FRAME_GM_RELEASE_VERSION != 0x00u )
# error "Error in fbl_ap.c: Source and Header file are inconsistent!"
#endif

#if ( FBLKBAPI_FRAME_GM_VERSION != _FBLKBAPI_FRAME_OEM_VERSION ) || \
    ( FBLKBAPI_FRAME_GM_RELEASE_VERSION != _FBLKBAPI_FRAME_OEM_RELEASE_VERSION )
# error "Error in fbl_ap.c: Source and v_ver.h are inconsistent!"
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

/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/

#if defined( FBL_ENABLE_RAM_INTEGRITY_CHECK )
typedef struct
{
   tFblAddress begin;
   tFblAddress end;
} tRamTable;
#endif

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL DATA
 **********************************************************************************************************************/

#if defined( FBL_ENABLE_VECTOR_HW )
static vuint16 flashlight;
#endif

/***********************************************************************************************************************
 *  EXTERNAL DATA
 **********************************************************************************************************************/

#  if defined( FBL_ENABLE_MULTIPLE_NODES )||\
     defined( FBL_ENABLE_CAN_CONFIGURATION )
extern tFblCanIdTable fblCanIdTable;
#  endif

/***********************************************************************************************************************
 *  PROTOTYPES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/

#if defined( FBL_ENABLE_DEBUG_STATUS )
/***********************************************************************************************************************
 *  ApplFblInitErrStatus
 **********************************************************************************************************************/
/*! \brief       Initialize error status information.
 **********************************************************************************************************************/
void ApplFblInitErrStatus( void )
{
   errStatFlashDrvVersion[0] = 0;
   errStatFlashDrvVersion[1] = 0;
   errStatFlashDrvVersion[2] = 0;
   errStatFlashDrvErrorCode = 0;
   errStatErrorCode = 0;
   errStatFblStates = 0;
   errStatLastServiceId = 0;
   errStatTpError = (vuint8)0;
   errStatFileName = (const vuint8 *)0;
   errStatLineNumber = (vuint16)0;
}
#endif

/***********************************************************************************************************************
 *  ApplFblInit
 **********************************************************************************************************************/
/*! \brief       Function is called before any other FBL initialization
 **********************************************************************************************************************/
void ApplFblInit( void )
{
   FBL_GIOGCR0 |= 0x01;
   FBL_GIODIRA |= 0x04;
   FBL_GIODOUTA |= 0x04;
   (void) EepromDriver_InitSync(V_NULL);

/* The following elements FBL_TEST_... can be used during development for test purpose */
#if (FBL_PROJECT_STATE >= FBL_INTEGRATION)
# if defined( FBL_TEST_SBA_TICKET )
   /* This can be used for test purpose to Write SBA ticket from flash sbaBlk0 to NVM */
   ApplFblNvWriteSBA_ticket(0, (vuint8*) sbaBlk0);
# endif
# if defined( FBL_TEST_ECU_ID )
   /* This can be used for test purpose to Write Ecu-Id.
    * Note: The FblHeader field ECUID could be used to fullfill the requirement, if the
    * content is modified in the hexfile for each controller part to be unique.
    */
   ApplFblNvWriteECU_ID(0, (vuint8*) FblHeader.ECUID);
# endif
# if defined( FBL_TEST_KEY_NBID )
   {
      tNBIDInfo dummyKeyNbidInfo;
      dummyKeyNbidInfo.received = 0x0000;
      /* This can be used for test purpose to Write initial KEY_NBID */
      ApplFblNVMWriteKeyNBID(&dummyKeyNbidInfo);
   }
# endif
# if defined( FBL_TEST_APP_NBID )
   {
      tNBIDInfo dummyAppNbidInfo;
      dummyAppNbidInfo.received = 0x0000;
#  if defined( FBL_ENABLE_MULTI_PROCESSOR )
      /* Rewrite for additional modules */
      dummyAppNbidInfo.MID = MODID_OPSW_2;
      ApplFblNVMWriteAppNBID(&dummyAppNbidInfo);
      dummyAppNbidInfo.MID = MODID_OPSW_1;
#endif
      /* This can be used for test purpose to Write initial APP_NBID */
      ApplFblNVMWriteAppNBID(&dummyAppNbidInfo);
   }
# endif
#endif

}

/***********************************************************************************************************************
 *  ApplFblStartup
 **********************************************************************************************************************/
/*! \brief       Bootloader initialization callback
 *  \details     This function is called during the Bootloader initialization if the application software is
 *               not started. It can be used to perform initializations which are not needed if the application
 *               software is started. Please note that the function is called several times with different
 *               initposition values. Timer and hardware can be accessed in post-init phase only.
 *  \param[in]   initposition Initialization state of Bootloader.
 **********************************************************************************************************************/
void ApplFblStartup( vuint8 initposition )
{
   if (kStartupPreInit == initposition)
   {
   }
   else
   {
#if defined( FBL_ENABLE_ROM_INTEGRITY_CHECK ) || \
    defined( FBL_ENABLE_RAM_INTEGRITY_CHECK )
      if ((GetFblMode() & START_FROM_RESET) != (vuint8)0)
      {
#  if defined( FBL_ENABLE_ROM_INTEGRITY_CHECK )
         ApplFblRomIntegrityCheck();
#  endif
#  if defined( FBL_ENABLE_RAM_INTEGRITY_CHECK )
         ApplFblRamIntegrityCheck();
#  endif

         /* Update BIS information according to redundant fblProgrammedState flags */
         if ( kProgrammedStateRomError == (fblProgrammedState & kProgrammedStateRomError))
         {
            FblSetBisLsbByte(SBILSB_ERR_BOOTROMCHK);
         }
         if ( kProgrammedStateRamError == (fblProgrammedState & kProgrammedStateRamError))
         {
            FblSetBisLsbByte(SBILSB_ERR_RAMCHK);
         }
      }
#endif

   }
}

#if defined( FBL_ENABLE_MULTIPLE_NODES ) || \
    defined( FBL_ENABLE_CAN_CONFIGURATION)
/***********************************************************************************************************************
 *  ApplFblCanParamInit
 **********************************************************************************************************************/
/*! \brief       Overwrite default CAN parameters
 *  \details     This function is called before CAN controller initialization so that default initialization values
 *               can be overridden according to the boot mode.
 *               If this function is empty, the default/reset settings are used to initialize the CAN controller.
 *  \pre         fblCanIdTable has to be initialized with default/reset
 **********************************************************************************************************************/
void ApplFblCanParamInit( void )
{
#  if defined( FBL_ENABLE_MULTIPLE_NODES )
   /* Replace argument to ComSetCurrentECU with desired ECU based on I/O */
   /* Be careful that this matches the Operating S/W */
   ComSetCurrentECU(0);
#  endif /* FBL_ENABLE_MULTIPLE_NODES */

   if ((GetFblMode() & START_FROM_APPL) != START_FROM_APPL)
   {
      vuint16 canId = MK_ID(DIAG_NODE_PHYSICAL_REQUEST_ID);
      CanInitTable.TpRxIdHigh = (vuint8) (canId >> 8);
      CanInitTable.TpRxIdLow  = (vuint8) (canId & 0xFF);
      /*
       * Direct initialization of physical-rx-id done because
       * FblInit() does not call FblCanParamInit() unless starting from Op/Sw.
       * This way, we don't have to initialize the bit-timing
       * elements of CanInitTable, which are hardware dependent.
       */
      fblCanIdTable.CanRxId[0] = canId;

      canId = MK_ID(DIAG_NODE_PHYSICAL_RESPONSE_ID);
      CanInitTable.TpTxIdHigh = (vuint8) (canId >> 8);
      CanInitTable.TpTxIdLow  = (vuint8) (canId & 0xFF);
   }
   else
   {
      /*
       * When starting from Operating S/W, physical request & response
       * CAN identifiers should already be in CanInitTable.
       * The request ID is copied to fblCanIdTable in FblCanParamInit.
       * The response ID is handled by FblDiagInit.
       *
       * Normally, there is nothing to do.
       */
   }

   /* Set the functional request CAN id (not available from Operating S/W) */
   fblCanIdTable.CanRxId[1] = MK_ID(CAN_TP_FUNC_RXID);

   /* Set the Diagnostics Node Address */
   diagNodeAddr = DIAG_NODE_ADDRESS;
}
#endif

#if defined( FBL_USE_FBL_AS_DEMOAPPL )
#else
/***********************************************************************************************************************
 *  ApplFblStartApplication
 **********************************************************************************************************************/
/*! \brief       This function is called to start the application.
 *  \details     Please note: The function will never return because control is handed over to the application.
 *  \pre         Application validity has been checked, Application is valid
 **********************************************************************************************************************/
void ApplFblStartApplication( void )
{
   /* Start application */
   JSR_APPL();

#if defined( FBL_ENABLE_SYSTEM_CHECK )
   /* Code should never be reached */
   while (1)
   {
      ;
   }
#endif /* FBL_ENABLE_SYSTEM_CHECK */
}
#endif

/***********************************************************************************************************************
 *  ApplFblTpErrorInd - GM Specific
 **********************************************************************************************************************/
/*! \brief       Sends negative-response for TP Layer errors
 *  \param[in]   tpErrorCode Indicates cause of TP error.
 **********************************************************************************************************************/
void ApplFblTpErrorInd( vuint8 tpErrorCode )
{
   /* Consider what happens if called for TP tx error */
   /* Consider what happens if TP timeout due to loss of connection (attempt to tx will fail) */

   DiagNRCGeneralProgFailure();
#if defined( FBL_ENABLE_DEBUG_STATUS )
   FblErrDebugStatus(kDiagErrTPFailed);
   errStatTpError = tpErrorCode;
#endif

   DiagProcessingDone(0);
}

/***********************************************************************************************************************
 *  ApplFblCheckProgConditions
 **********************************************************************************************************************/
/*! \brief       This function is called after receiving the service request sessionControl ProgrammingSession to check
 *               the programming conditions like reprogramming counter, ambient temperature, programming voltage, etc.
 *  \return      If all conditions are correct, the function returns kFblOk, otherwise kFblFailed.
 **********************************************************************************************************************/
tFblResult ApplFblCheckProgConditions( void )
{
   return (kFblOk);
}

/***********************************************************************************************************************
 *  ApplTrcvrNormalMode
 **********************************************************************************************************************/
/*! \brief       Activate transceiver if necessary (e.g. pin configuration ENABLE = 1, _STDBY = 1)
 **********************************************************************************************************************/
void ApplTrcvrNormalMode( void )
{
}

/***********************************************************************************************************************
 *  ApplTrcvrSleepMode
 **********************************************************************************************************************/
/*! \brief       Turn off the transceiver to enter sleep mode (e.g. pin configuration ENABLE = 1, _STDBY = 0)
 **********************************************************************************************************************/
void ApplTrcvrSleepMode( void )
{
}

#if defined( FBL_ENABLE_HIGHSPEED_MODE )
/***********************************************************************************************************************
 *  ApplTrcvrHighSpeedMode
 **********************************************************************************************************************/
/*! \brief       Activate transceiver if necessary
 *               ENABLE=0, _STDBY=1
 **********************************************************************************************************************/
void ApplTrcvrHighSpeedMode( void )
{
}
#endif

/***********************************************************************************************************************
 *  ApplFblSetVfp
 **********************************************************************************************************************/
/*! \brief       Function is called when the FBL enters programming mode.
 *  \details     Port bits have to be set so that the required programming voltage is available.
 **********************************************************************************************************************/
void ApplFblSetVfp( void )
{
}

/***********************************************************************************************************************
 *  ApplFblResetVfp
 **********************************************************************************************************************/
/*! \brief       This function is called when the programming is done.
 **********************************************************************************************************************/
void ApplFblResetVfp( void )
{
}

/***********************************************************************************************************************
 *  ApplFblReset
 **********************************************************************************************************************/
/*! \brief       This function performs a reset.
 *  \details     The function will wait until the reset is performed. Please note: This function will
 *               never return to the calling code.
 **********************************************************************************************************************/
void ApplFblReset( void )
{

#if defined( FBL_WATCHDOG_ON )
   /* Prevent return to any other code */
   while (1)
   {
      ;
   }
#else
    /* Jump to startup code entry point. *
     * Replace this by hardware reset    */
   JSR_RESET();
#endif
}

#if defined( FBL_ENABLE_APPL_TASK ) 
/***********************************************************************************************************************
 *  ApplFblTask
 **********************************************************************************************************************/
/*! \brief       1ms user task.
 *  \details     This function is called every millisecond if the Bootloader is in idle state. It is not called during
 *               flash operations.
 **********************************************************************************************************************/
void ApplFblTask( void )
{
    static int loop;
	tMsgObject message;
	if (loop > 0 && loop <= 500) {
		loop--;
	}
	else {
		loop = 500;
		//message.msgObject.Id = 0x50;
		//message.msgObject.DLC = 0x0;
		//FblCanTransmit(&message);
	}
	FBL_GIODOUTA ^= 0x04;
}
#endif /* FBL_ENABLE_APPL_TASK || FBL_ENABLE_APPL_TIMER_TASK */

#if defined( FBL_ENABLE_DATA_PROCESSING )
/***********************************************************************************************************************
 *  ApplFblCheckDataFormatIdentifier
 **********************************************************************************************************************/
/*! \brief       Return kFblOk for all valid data format identifiers.
 *  \param[in]   formatId Format Id from Request-Download service request.
 **********************************************************************************************************************/
tFblResult ApplFblCheckDataFormatIdentifier(vuint8 formatId)
{
   return kFblFailed;
}

/***********************************************************************************************************************
 *  ApplFblInitDataProcessing
 **********************************************************************************************************************/
/*! \brief         This function is called to initialize the application specific data processing function.
 *  \param[in,out] procParam Processing parameter data structure
 *  \return        kFblOk/kFblFailed
 **********************************************************************************************************************/
tFblResult ApplFblInitDataProcessing( tProcParam * procParam )
{
   tFblResult result = kFblFailed;

#if defined( FBL_ENABLE_COMPRESSION_MODE )
   if ( MODULE_DF_COMPR == (procParam->mode & MODULE_DF_COMPR) )
   {
      /* either MODULE_DF_COMPR or MODULE_DF_COMPR_HDR is set here */
      result = ApplFblCmprInit();
   }
#endif
   return result;
}

/***********************************************************************************************************************
 *  ApplFblDataProcessing
 **********************************************************************************************************************/
/*! \brief         Data processing function.
 *  \pre           Data processing has to be initialized by call of ApplFblInitDataProcessing
 *  \param[in,out] procParam Processing parameter data structure
 *  \return        kFblOk/kFblFailed
 **********************************************************************************************************************/
tFblResult ApplFblDataProcessing( tProcParam * procParam )
{
   tFblResult result  = kFblFailed;
#if defined( FBL_ENABLE_COMPRESSION_MODE )
   vuint16 inMaxOut = procParam->dataOutMaxLength;
   vuint16 altMaxOut;

   assertFblInternal(rcvBuffHandle.progDataOffset <= HDR_MODULE_MAX_RAW_LEN, kFblSysAssertParameterOutOfRange);

   if ( MODULE_DF_COMPR == (procParam->mode & MODULE_DF_COMPR) )
   {
      /* either MODULE_DF_COMPR or MODULE_DF_COMPR_HDR is set here */

      if( MODULE_DF_COMPR == procParam->mode )
      {
         /* Only consume bytes up to Segment border. */
         /* Add rcvBuffHandle.progDataOffset to guarantee that the header is consumed in first run. */
         altMaxOut = FblHdrGetDataInCurrSegment(procParam->dataOutMaxLength) + rcvBuffHandle.progDataOffset;

         if(altMaxOut < procParam->dataOutMaxLength )
         {
            procParam->dataOutMaxLength = altMaxOut;
         }
      }

      if (procParam->dataOutMaxLength > 0)
      {
         /* Perform decompression */
         /* Decompression stops if input-buffer is empty or output-buffer is full */
         result = ApplFblDecompress(procParam, rcvBuffHandle.progDataOffset);

         if ( MODULE_DF_COMPR == procParam->mode )
         {
            FblHdrConsumeCurrSegmentBytes(procParam->dataOutLength);
            rcvBuffHandle.buffIdx += procParam->dataLength;
            /* Always reset progDataOffset after first programmed data chunk ( which includes the header ) */
            rcvBuffHandle.progDataOffset = 0;
         }
      }
      else
      {
         /* Consume all bytes passed in to allow segment switch above Libmem: All required data in current segment is consumed.  */
         result = kFblOk;
      }
      procParam->dataOutMaxLength = inMaxOut;
   }
#endif

   return result;
}

/***********************************************************************************************************************
 *  ApplFblDeinitDataProcessing
 **********************************************************************************************************************/
/*! \brief         Deinitialize data processing function.
 *  \pre           Data processing has to be initialized by call
 *  \param[in,out] procParam Processing parameter data structure
 *  \return        kFblOk/kFblFailed
 **********************************************************************************************************************/
tFblResult ApplFblDeinitDataProcessing( tProcParam * procParam )
{
#if defined( FBL_ENABLE_COMPRESSION_MODE )
   return kFblOk;
#else
   return kFblFailed;
#endif
}
#endif /* FBL_ENABLE_DATA_PROCESSING */

/***********************************************************************************************************************
 *  ApplFblCanWakeUp
 **********************************************************************************************************************/
/*! \brief       This call-back function is called if a wake-up condition on the CAN bus is detected during sleep mode
 *               of the CAN controller.
 **********************************************************************************************************************/
void ApplFblCanWakeUp( void )
{
}

/***********************************************************************************************************************
 *  ApplFblCanBusOff
 **********************************************************************************************************************/
/*! \brief       This call-back function is called if the CAN controller enters bus-off state.
 **********************************************************************************************************************/
void ApplFblCanBusOff( void )
{
}

#if defined( FBL_ENABLE_ROM_INTEGRITY_CHECK )
/***********************************************************************************************************************
 *  ApplFblRomIntegrityCheck
 **********************************************************************************************************************/
/*! \brief       This routine is responsible for calculating the checksum of the bootloader and comparing it
 *               to the expected value stored in the FBL header. If the values do not match,
 *               state variable used by ApplFblReportProgrammedState must be updated to reflect the error.
 **********************************************************************************************************************/
void ApplFblRomIntegrityCheck( void )
{
   vuint16 checksum = 0;
   vuint8 noar;
   vuint8 buffer[16];

   FblHdrInitFblRomCheck(&parsedModuleInfo.segmentList);

   /* Integration OPTION:
    * ------------------
    * Try to only use one region for Fbl Integrity check. If this is not possible,
    * add additional region in case Fbl is splitted into two regions manually here.
    * (The first GENy configured region is set inside FblHdrInitFblRomCheck())
    *
    * E.g.: Add 2nd region:
    * ----------------------------------------------------------------
    * parsedModuleInfo.segmentList.nrOfSegments = 2;
    * parsedModuleInfo.segmentList.segmentInfo[1].targetAddress = 0xYYYYYYYY;
    * parsedModuleInfo.segmentList.segmentInfo[1].length = 0xZZZZZZZZ;
    */

   /* Calculate over all address-regions */
   for (noar = 0; noar < parsedModuleInfo.segmentList.nrOfSegments; noar++)
   {
      tFblAddress addr = parsedModuleInfo.segmentList.segmentInfo[noar].targetAddress;
      tFblLength len = parsedModuleInfo.segmentList.segmentInfo[noar].length;

      while (len > 0)
      {
         tFblLength readLen = (tFblLength) ( (len < sizeof(buffer))?  len : sizeof(buffer));

         if (FlashDriver_RReadSync((IO_MemPtrType)buffer, (IO_SizeType)readLen, (IO_PositionType)addr) != IO_E_OK)
         {
            fblProgrammedState |= kProgrammedStateRomError;
         }
         if (kFblOk != ApplFblUpdateChecksum(&checksum, (SecM_LengthType)readLen, buffer))
         {
            fblProgrammedState |= kProgrammedStateRomError;
         }

         addr += readLen;
         len  -= readLen;
      }
   }

   if (kFblOk != ApplFblFinalizeChecksum(&checksum))
   {
      fblProgrammedState |= kProgrammedStateRomError;
   }
}
#endif /* FBL_ENABLE_ROM_INTEGRITY_CHECK */

#if defined( FBL_ENABLE_RAM_INTEGRITY_CHECK )
/***********************************************************************************************************************
 *  ApplFblRamIntegrityCheck
 **********************************************************************************************************************/
/*! \brief       This routine is responsible for verifying that the Random-Access-Memory (RAM) device(s)
                 are operating correctly. If an error is detected, the state variable used by
 *               ApplFblReportProgrammedState must be updated.
 **********************************************************************************************************************/
void ApplFblRamIntegrityCheck( void )
{
   static MEMORY_ROM tRamTable kRamTable[] =
   {
      /* Edit this to match the start/end address of RAM */
      /* Add further entries to the below GENy generated entry if multiple regions are required */
      { FBL_RAM_INTEGRITY_START_ADDR, FBL_RAM_INTEGRITY_END_ADDR }
   };

   static volatile vuint8 *pCurrAddr;
   static volatile vuint8 tmp;
   volatile vuint8 i;

   /* check local variables */
   tmp = 0x55;
   if (tmp != 0x55)
   {
      fblProgrammedState |= kProgrammedStateRamError;
      return;
   }
   tmp = 0xAA;
   if (tmp != 0xAA)
   {
      fblProgrammedState |= kProgrammedStateRamError;
      return;
   }

   pCurrAddr = (volatile vuint8 *)0;
   if (pCurrAddr != 0)
   {
      fblProgrammedState |= kProgrammedStateRamError;
      return;
   }
   pCurrAddr = (volatile vuint8 *) - 1;
   if (pCurrAddr != (volatile vuint8 *) - 1)
   {
      fblProgrammedState |= kProgrammedStateRamError;
      return;
   }

   /*
    * Note: This implementation only checks for stuck data bits.
    *       It does not check for shorted data bits.
    *       It does not check for stuck/shorted address lines.
    */
   for (i=0; i < (vuint8)((sizeof(kRamTable)/sizeof(tRamTable))); ++i)
   {
      pCurrAddr = (volatile vuint8 *)kRamTable[i].begin;
      do
      {
         (void)FblLookForWatchdog();

         if (( (pCurrAddr < (volatile vuint8 *)&pCurrAddr) ||
               (pCurrAddr >= (volatile vuint8 *)(&((&pCurrAddr)[1]))) ) &&
               (pCurrAddr != &tmp))
         {
            tmp = *pCurrAddr;

            *pCurrAddr = 0x55;
            if (*pCurrAddr != 0x55)
            {
               fblProgrammedState |= kProgrammedStateRamError;
            }

            *pCurrAddr = 0xAA;
            if (*pCurrAddr != 0xAA)
            {
               fblProgrammedState |= kProgrammedStateRamError;
            }

            *pCurrAddr = tmp;
         }

         /* Increment address */
         pCurrAddr = &pCurrAddr[1];
      } while ((pCurrAddr <= (volatile vuint8 *)kRamTable[i].end) && ((fblProgrammedState & kProgrammedStateRamError) == 0));
   }
}
#endif /* FBL_ENABLE_RAM_INTEGRITY_CHECK */

/***********************************************************************************************************************
 *  ApplFblSleepModeAllowed
 **********************************************************************************************************************/
/*! \brief       This function is called when the FBL-Active-timer expires.
 *  \details     The function return value decides, if the bootloader can now enter the STOP instruction and can recover
 *               from there (kFblOk) or not (kFblFailed).
 *  \return      kFblOk if controller can enter sleepmode and can recover from stop-mode normally.
 *               kFblFailed otherwise.
 **********************************************************************************************************************/
tFblResult ApplFblSleepModeAllowed( void )
{
   tFblResult retVal = kFblFailed;
   {
# if defined( FBL_ENABLE_WAKEUP_INT )
      /* Default: Returns if the default bootloader IntTable (Fbl_Applvect resp. APPLVECT_FROM_BOOT
       * is present or not. Note that the APPLVECT_FROM_BOOT includes a reset-entry to the boot. */
      retVal = (FblCheckBootVectTableIsValid())? kFblOk : kFblFailed;
#else
      /* In case wakeup is not per interrupt sleep mode is always allowed. This also applies in case
       * of a interrupt base register used. */
      retVal = kFblOk;
#endif
   }
   return retVal;
}

/***********************************************************************************************************************
 *  ApplFblEnterStopMode
 **********************************************************************************************************************/
/*! \brief       This function is called before the FBL enters the sleep mode.
 *  \details     It is important that all peripheral interrupts are disabled by this function, because interrupts
 *               are enabled after this function to handle the wakeup interrupt.
 **********************************************************************************************************************/
void ApplFblEnterStopMode( void )
{
/* ----------------------------------------------------------------------------
 * There are 3 basic configurations for the FBL sleep mode.  They are as
 * follows:
 *
 * 1) Integrated sleep mode enabled
 * 2) Integrated sleep mode enabled with wakeup interrupt
 * 3) Integrated sleep mode handling disabled
 * Each of the configurations are detailed below.
 *
 * 1) Integrated sleep mode enabled - check the checkbox in GENy for "Fbl
 *    enable sleepmode". Wakeup on CAN-message reception (ApplFblCanWakeUp()
 *    is called on reception). Only configurable if CAN-cell supports low
 *    power mode/wakeup on CAN-message reception. Adapt ApplFblSleepModeAllowed
 *    to return kFblOk if conditions are correct to go to sleep.
 *    If a user-specific reason exists to not go to sleep, then return
 *    kFblFailed.
 *
 * 2) Integrated sleep mode enabled with wakeup interrupt - Description 1)
 *    above applies, with the difference being that a real interrupt has to be
 *    generated for the given controller to wakeup on CAN-message reception.
 *    The tag FBL_ENABLE_WAKEUP_INT must additionally be defined
 *    ( via GENy-preconfig file ). This configuration should only be used if a
 *    non-interrupt based configuration is not possible for a given controller.
 *    This will require modification to the vector tables and callbacks. Be
 *    sure the required measures to again wakeup the CAN-cell are handled.
 *    See the FBL_ENABLE_WAKEUP_INT comment below for more configuration
 *    aspects and potential problems that may come along.
 *
 * 3) Integrated Sleep mode handling disabled - Simply uncheck the checkbox
 *    in GENy for "Fbl enable sleepmode". Adapt ApplFblSleepModeAllowed to
 *    return kFblFailed. This is also the configuration to use if a
 *    user-specific wake-up mechanism other than CAN-message reception is to
 *    be used (e.g. when transceivers that allow turning of an Ecu via an
 *    inhibit pin are used.)
 *
 * ApplFblReset() may be called after the controller wakes up, instead of
 * continuing with disabling interrupts and reinitializing the hardware. Be
 * sure to implement a hardware-reset inside ApplFblReset() in this case.
 *
 * ----------------------------------------------------------------------------
 *   NOTE THAT SLEEP MODE IMPLEMENTATION IS HIGHLY HARDWARE DEPENDANT AND
 *   THEREFORE ALL GIVEN EXAMPLE CODE HAS TO BE ADAPTED TO EACH ENVIRONMENT
 *   AND LOW POWER USE CASE IMPERATIVELY!
 *
 *   BE SURE TO TEST AND VERIFY ALL REUSED CODE FOR THE GIVEN ENVIRONMENT!
 * ------------------------------------------------------------------------- */

   /* Check if boot loader vector table is inside flash */
   if(ApplFblSleepModeAllowed() == kFblOk)
   {
      /* Deactivate Transceiver */
      ApplTrcvrSleepMode();

      /**********************************************************
       ** If a CAN message has been received meanwhile, the CAN
       ** controller will not be set to sleepmode.
       ***********************************************************/
      if (FblCanReceive() == 0)
      {
         /* Set CAN controller into sleep mode and stop timer   */
#if defined( FBL_ENABLE_SLEEPMODE )
         if (FblCanSleep() != kFblFailed)
         {
# if defined( FBL_ENABLE_WAKEUP_INT )
            /* ----------------------------------------------------------------------------
             * Using a wakeup interrupt is not recommended as long as there is other
             * possibilities for waking up again without interrupt.
             *
             * Interrupts should be globally enabled if Can Wakeup interrupt is to be used.
             * CAN Wakeup interrupt is to be enabled in FblCanSleep(). All other peripheral
             * interrupts except the CAN Wakeup interrupt should stay masked ( disabled ).
             *
             * A sleep mode implementation using a wakeup interrupt should only be used
             * before any application is ever programmed if the wakeup interrupt is called
             * from reprogrammable memory ( this usually can only be avoided if there is a
             * configurable vector table base address register ). This is to avoid enabling
             * interrupts while there potentially is a corrupted application vector table.
             * The function ApplFblSleepModeAllowed should call the function
             * FblCheckBootVectTableIsValid to verify that it returns true ( no application
             * was ever programmed ).
             *
             * There is a potential deadlock problem when enabling global interrupts if the
             * wakeup interrupt is served before the controller itself goes to low power
             * mode: If the controller goes to low power mode after the CAN cell woke up,
             * the ECU may not be able to wake up any more without
             * externally resetting it. It must be verified if this is applicable for any
             * given controller.
             * ------------------------------------------------------------------------- */
             FblInterruptEnable();
# endif
            /* Insert application specific code here */
         } /* if (FblCanSleep() != kFblFailed) */
#endif /* FBL_ENABLE_SLEEPMODE */
      } /* if (FblCanReceive() == 0) */

      /* Switch transceiver to normal mode                     */
      ApplTrcvrNormalMode();
#if defined( FBL_ENABLE_SLEEPMODE )
      FblCanWakeUp();
#endif
      /* Reinitialize disabled timer                            */
      FblTimerInit();
      /* Globally disable interrupts after stop mode is left. */
      FblInterruptDisable();
   } /* if(ApplFblSleepModeAllowed() == kFblOk) */
}

#if defined( FBL_ENABLE_ASSERTION )
/***********************************************************************************************************************
 *  ApplFblFatalError
 **********************************************************************************************************************/
/*! \brief       If the bootloader assertions are enabled, this function will be called in case an invalid bootloader
 *               state is encountered.
 *  \param[in]   errorCode Code number of the encountered assertion
 *  \param[in]   module Name of the affected module (Only if extended info is enabled)
 *  \param[in]   line Line number where the assertion occurred (Only if extended info is enabled)
 **********************************************************************************************************************/
void ApplFblFatalError( FBL_DECL_ASSERT_EXTENDED_INFO(vuint8 errorCode) )
{

   /* change this variable in debugger in order to see where function was called. */
   volatile vuint8 keepInWhileAllowToExit = 1;

   while (keepInWhileAllowToExit)
   {
      /* Stay in endless loop in order to indicate error condition */
      FblLookForWatchdog();
   }
}
#endif

/***********************************************************************************************************************
 *  END OF FILE: KBFBL_APFRAMEOEM.C
 **********************************************************************************************************************/

