/* Kernbauer Version: 1.12 Konfiguration: FBL Erzeugungsgangnummer: 1 */

/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief         Diagnostic services supported in boot mode
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
 *  REVISION HISTORY (KbFbl_apFrameOem.h, GM SLP5 and SLP6)
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  05.00.00   2014-01-31  AWh                      First release
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  00.90.00   2003-12-10  CB                       First implementation
 *  00.91.00   2003-12-19  CB                       Decryption/decompression interface changed
 *  01.00.00   2004-02-24  CB                       Release
 *  01.01.00   2004-03-24  Rr                       Added new functions
 *  01.02.00   2004-03-25  WM                       Minor changes
 *  01.03.00   2004-04-23  WM                       Added watchdog trigger functions
 *                         ACP                      Added ApplFblSecurityStatus
 *                         AMN                      Added ApplFblIsValidFlashBlock, ApplFblReadBlock
 *  01.04.00   2004-05-07  CB                       Added ApplCanBusOff, ApplCanWakeUp
 *  01.05.00   2004-05-19  CB                       Minor changes
 *  01.06.00   2004-06-30  MHz     ESCAN00007536    Added prototype for function
 *                                                  ApplFblStatusGet()
 *                         WM                       Added prototype for ApplFblInitErrStatus()
 *  01.07.00   2004-07-22  CB                       Added ApplFblReadEcuIdentification,
 *                                                  ApplFblStoreTesterCode, ApplFblStoreProgrammingDate
 *  01.08.00   2004-08-06  WM                       Added ApplFblSecurityAccessDelayActive
 *  01.09.00   2004-09-09  Rr                       No changes
 *  01.10.00   2004-09-16  WM                       No changes
 *  01.11.00   2004-10-25  AP      ESCAN00010007    Adaptation to support Vector
 *                                                  component OSEK NM
 *  01.12.00   2004-12-23  AMN                      Changed prototype of ApFblWriteDataByIdentifier
 *                                                  Added ApplFblPBLConfigSet prototype
 *                         ACP                      Version check reworked
 *                         CB                       Added fingerprint functions
 *  01.13.00   2005-01-14  CB                       Added several API functions
 *  01.14.00   2005-03-22  WM      ESCAN00011595    MISRA warnings
 *  01.15.00   2005-09-29  WM                       No changes
 *  01.16.00   2005-12-08  WM      ESCAN00014594    FblApSrcApplFblTpParamInit token
 *                                                  added for TPMC
 *  01.17.00   2006-02-28  AMN                      Added tokens ApplFblCopyToRAM and
 *                                                  ApplFblIsValidAppInRAM
 *  01.18.00   2006-03-24  QPs                      Added ApplFblGetSubNodeRxCanId for gateway
 *                                                  implementation
 *  01.19.00   2006-04-25  CB      ESCAN00015917    Return value added for
 *                                                  ApplFblInvalidateBlock
 *                         WM      ESCAN00015960    Segment verification added
 *                         QPs     ESCAN00016110    Added ApplCanBusOff for the gateway
 *                         CB      ESCAN00016144    Return value added for
 *                                                  ApplFblInitDecryption
 *                         CB      ESCAN00016147    ApplFblStoreBootSwIdent added
 *                                 ESCAN00016171    ApplFblSetSecAccessDelayFlag,
 *                                                  ApplFblGetSecAccessDelayFlag and
 *                                                  ApplFblClrSecAccessDelayFlag added
 *  01.20.00   2006-05-18  ACP     ESCAN00016438    Added kStartupPreInit/PostInit
 *                                                  definition for new ApplFblStartup() parameter
 *  01.21.00   2006-06-29  FHE                      Added header for ApplFblCheckProgDependencies()
 *  01.22.00   2006-07-24  JHg                      Moved watchdog trigger function to non banked
 *                                                  memory for MCS12X
 *                                                  PRE_ and POST_FblApSrcApplFblWDTrigger
 *                         CB      ESCAN00017003    Added ApplFblProgrammingStateChanged
 *                         AMN                      added prototype for ApplFblValidateApp
 *  01.23.00   2006-08-10  JHg                      Added token for additional watchdog functions
 *                         AMN     ESCAN00017479    Modifed API of ApplFblValidateApp
 *                                                  for Misra compliance
 *  01.24.00   2006-12-06  FHE     ESCAN00018639    Added ApplFblCheckConditions
 *  01.25.00   2007-02-07  Hp      ESCAN00019129    Add support for CCP in the bootloader
 *                                                  ApplFblWriteDataByLocalId returns now vuint8.
 *                                                  Add PreHandler and presence-pattern support.
 *  01.26.00   2007-03-30  WM      ESCAN00020175    Added ApplFblControlFailSafeReaction
 *  01.27.00   2007-07-06  JHg                      Return value added to ApplFblDecryption and
 *                                                  ApplFblDeinitDecryption
 *  01.28.00   2007-07-27  Ls                       No changes
 *  01.29.00   2008-03-07  Ci      ESCAN00014726    No changes
 *                         Hp      ESCAN00014726    No changes
 *                         JHg                      No changes
 *                         Hp      ESCAN00025029    Support Vector decompression module (optional)
 *  01.30.00   2008-03-28  JHg     ESCAN00025354    No changes
 *                                 ESCAN00025355    No changes
 *  01.31.00   2008-06-13  Ci      ESCAN00025071    Extended function description of
 *                                                  ApplFblStartup() with kStartupStayInBoot
 *                                 ESCAN00027445    Added support for configurations
 *                                                  with multiple CAN channels
 *  01.32.00   2008-09-10  JHg     ESCAN00029860    No changes
 *  01.33.00   2008-10-15  FHe     ESCAN00030640    Added support for VAG spec 1.2.1
 *  01.34.00   2009-03-13  JHg     ESCAN00032180    Added support for multichannel configurations
 *                                                  to ApplCanTxObjStart, ApplCanTxObjConfirmed
 *                         FHe                      Restored old format of ApplFblCheckProgConditions
 *                         Ci      ESCAN00033475    Added gateway specific callback
 *                                                  functions (Gw_Route support)
 *  01.35.00   2009-03-12  QPs     ESCAN00033801    Added communication wrapper specific
 *                                                  callback functions
 *  01.36.00   2009-07-21  QPs     ESCAN00035282    Added DC SLP10 extension:
 *                                                  ApplFblControlFailSafeReaction
|   01.36.00               AWh     ESCAN00036825    No changes
 *  01.37.00   2009-08-31  AWh     ESCAN00037405    Inconsistent return value type between
 *                                                  WriteDataByCommonId prototype and implementation
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
 *  01.42.00   2010-07-23  CB      ESCAN00044299    No changes
 *  01.43.00   2010-09-15  Ci      ESCAN00044991    Added new Gw_Route API functions
 *  01.44.00   2010-12-10  MVi     ESCAN00047478    Enhanced flash erased detection interface
 *                                 ESCAN00047482    Added routine control succeeded interface
 *  01.45.00   2011-02-03  Ach     ESCAN00048191    No changes
 *  01.46.00   2011-07-14  Ci      ESCAN00052149    Adapted Gw_Route task interface
 *  01.47.00   2011-11-25  Hp      ESCAN00052787    Extend support for PresencePatterns
 *  01.48.00   2011-12-14  AWh     ESCAN00055586    Extend Api with ApplFblInvalidateApp function
 *  01.49.00   2012-02-14  Ach     ESCAN00056341    Added ApplFblStartApplication() function
 *  01.50.00   2012-02-22  Ach     ESCAN00052787    Extend support for PresencePatterns
 *                         MVi     ESCAN00056833    No changes
 *                                 ESCAN00056834    Added _EXPORT suffix to memmap defines
 *                         CB      ESCAN00056642    Added ApplFblCanPlatformToPhysChannel()
 *  01.51.00   2012-03-23  QPs     ESCAN00057230    Extend support for FlexRay
 *                         Ach     ESCAN00057636    No changes
 *                         CB      ESCAN00057670    Added Suspend Programming API
 *  01.52.00   2012-03-30  QPs     ESCAN00058040    Reworked PRE/POST tokens for ApplFblWDTrigger
 *  01.53.00   2012-08-16  MVi     ESCAN00060433    No changes
 *  01.54.00   2012-12-06  AWh     ESCAN00061619    Moved presence pattern macros to header
 *                         Ach     ESCAN00062912    Adapted comments to use Doxygen
 *  01.55.00   2013-01-24  Ach     ESCAN00064406    No changes
 *                         JHg     ESCAN00064421    Added callback ApplFblOnTransitionSession
 *                                 ESCAN00064437    Added callback ApplFblStateTask
 *  01.56.00   2013-02-08  ASe     ESCAN00064889    Reworked comments
 *  01.57.00   2014-01-21  ASe     ESCAN00072931    No changes
 *  01.58.00   2014-02-24  MVi     ESCAN00073549    No changes
 *  01.58.01   2014-04-03  ASe     ESCAN00074701    Fixed comments
 *  01.59.00   2014-07-16  QPs     ESCAN00077127    No changes
 *  01.60.00   2014-08-25  Dod     ESCAN00078033    Add callbacks ApplFblMemPreWrite/Erase and ApplFblGetLogBlkHash
 *  01.61.00   2015-01-13  Ach     ESCAN00080481    Change visibility of ApplFblChkModulePresence()
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  REVISION HISTORY (KbFbl_apOem.h, GM SLP5 and SLP6)
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  05.00.00   2014-01-31  AWh                      First release
 *  05.01.00   2014-02-27  AWh     ESCAN00073863    Add user callbacks for Non readable Device use case
 *                                 ESCAN00073906    Enable Multi processor use case
 *  05.02.00   2014-07-14  AWh     ESCAN00075304    No changes
 *                                                  SLP6: Add DID information available at Vector
 *                                                        Move DIDs from diagnostic layer to callback layer
 *                                                        Add ApplFblReadTpTargetAddr/ApplFblWriteTpTargetAddr
 *  05.03.00   2014-10-15  AWh     ESCAN00078893    No changes
 *                         AWh     ESCAN00078899    No changes
 *                         AWh     ESCAN00079661    Add compression interface
 *                         AWh     ESCAN00080211    No changes
 *                         Dod     ESCAN00079983    No changes
 *                                 -                Rework after Review
 *  05.04.00   2015-01-23  AWh     ESCAN00080768    Add HighSpeed/Lowspeed Macros required for single wire config
 **********************************************************************************************************************/

#ifndef __FBL_AP_H__
#define __FBL_AP_H__

/* ---  Module Version --- */
/* ##V_CFG_MANAGEMENT ##CQProject : FblKbApi CQComponent : Implementation */
#define FBLKBAPI_VERSION                           0x0161u
#define FBLKBAPI_RELEASE_VERSION                   0x00u

/* ##V_CFG_MANAGEMENT ##CQProject : FblKbApi_GM CQComponent : Implementation */
#define FBLKBAPI_GM_VERSION           0x0504u
#define FBLKBAPI_GM_RELEASE_VERSION   0x00u

/* ##V_CFG_MANAGEMENT ##CQProject : FblKbApi_Frame_GM CQComponent : Implementation */
#define FBLKBAPI_FRAME_GM_VERSION           0x0500u
#define FBLKBAPI_FRAME_GM_RELEASE_VERSION   0x00u

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/* The following checks verify correct configuration for GMW3110 V1.6 */
# if  defined( FBL_TP_ENABLE_ACCEPT_TOO_LARGE_DATA ) ||\
     !defined( FBL_TP_ENABLE_ISO_COMPLIANCE )        ||\
     !defined( FBL_TP_ENABLE_ISO15765_2_2 )          ||\
     !defined( FBL_TP_ENABLE_OVERRUN_FLAG_IN_FC )    ||\
     !defined( FBL_TP_ENABLE_ONLY_FIRST_FC )         ||\
     !defined( FBL_TP_ENABLE_FLOW_STATE )
#  error "Tp-configuration seems to be not compliant to GMW3110 V1.6"
# endif

#define APPL_FBL_RESET() (ApplFblReset())

#if defined( FBL_ENABLE_COMPRESSION_MODE )
/* The below functions are defined if you ordered the Vector Compression interface,
 * the interface has to be implemented by yourself else.
 */
# include "cmpr.h"
# define ApplFblDecompress CmprArleDecompress
# define ApplFblCmprInit   CmprArleInit
# define ApplFblCmprReadHeader CmprArleReadCmprHeader
#endif

/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  PROTOTYPES
 **********************************************************************************************************************/

void ApplFblInit( void );
void ApplFblStartup( vuint8 initposition );

#if defined( FBL_ENABLE_MULTIPLE_NODES ) || \
    defined( FBL_ENABLE_CAN_CONFIGURATION )
void ApplFblCanParamInit( void );
#endif

#if defined( FBL_USE_FBL_AS_DEMOAPPL )
#else
void ApplFblStartApplication( void );
#endif
tFblResult ApplFblCheckProgConditions( void );
void ApplTrcvrNormalMode( void );
void ApplTrcvrSleepMode( void );
void ApplFblSetVfp( void );
void ApplFblResetVfp( void );
void ApplFblReset( void );

#if defined( FBL_ENABLE_APPL_TASK )
void ApplFblTask( void );
#endif

#if defined( FBL_ENABLE_DATA_PROCESSING )
tFblResult ApplFblCheckDataFormatIdentifier(vuint8 formatId);
tFblResult ApplFblInitDataProcessing( tProcParam * procParam );
tFblResult ApplFblDataProcessing( tProcParam * procParam );
tFblResult ApplFblDeinitDataProcessing( tProcParam * procParam );
#endif /* FBL_ENABLE_DATA_PROCESSING */

#if defined( FBL_ENABLE_DEBUG_STATUS )
void ApplFblInitErrStatus( void );
#endif

void ApplFblCanWakeUp( void );
void ApplFblCanBusOff( void );

void ApplFblTpErrorInd( vuint8 tpErrorCode );

#if defined( FBL_ENABLE_HIGHSPEED_MODE )
void ApplTrcvrHighSpeedMode( void );
#endif

#if defined( FBL_ENABLE_ROM_INTEGRITY_CHECK )
void ApplFblRomIntegrityCheck( void );
#endif

#if defined( FBL_ENABLE_RAM_INTEGRITY_CHECK )
void ApplFblRamIntegrityCheck( void );
#endif

tFblResult ApplFblSleepModeAllowed( void );
void ApplFblEnterStopMode( void );

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

#endif /* __FBL_AP_H__ */

/***********************************************************************************************************************
 *  END OF FILE: KBFBL_APFRAMEOEM.H
 **********************************************************************************************************************/

