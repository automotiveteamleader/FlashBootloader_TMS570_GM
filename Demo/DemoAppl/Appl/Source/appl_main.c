/* Kernbauer Version: 1.12 Konfiguration: FBL Erzeugungsgangnummer: 1 */

/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief         Main module of FBL
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
 *  Christian Baeuerle            CB            Vector Informatik GmbH
 *  Armin Happel                  Hp            Vector Informatik GmbH
 *  Konrad Lazarus                Ls            Vector Informatik GmbH
 *  Alexandre C. Plombin          ACP           Vector CANtech, Inc.
 *  Ahmad Nasser                  AMN           Vector CANtech, Inc.
 *  Robert Schaeffner             Rr            Vector Informatik GmbH
 *  Marco Wierer                  WM            Vector Informatik GmbH
 *  Markus Holzer                 MHz           Vector Informatik GmbH
 *  Andreas Pick                  Ap            Vector Informatik GmbH
 *  Florian Hees                  FHE           Vector Informatik GmbH
 *  Michael F. Radwick            MFR           Vector CANtech, Inc.
 *  Andre Caspari                 Ci            Vector Informatik GmbH
 *  Quetty Palacios               QPs           Vector Informatik GmbH
 *  Slawomir Wilk                 swk           Vector Informatik GmbH
 *  Achim Strobelt                Ach           Vector Informatik GmbH
 *  Marcel Viole                  MVi           Vector Informatik GmbH
 *  Dennis O'Donnell              DJO           Vector CANtech, Inc.
 *  Alexander Starke              ASe           Vector Informatik GmbH
 *  Andreas Wenckebach            AWh           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY (KbFbl_mainFrame.c)
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  01.00.00   2004-02-24  ACP                      Release - was not used before (only C file)
 *  01.01.00   2004-03-25  ACP                      No changes - just versioning history
 *                         Rr                       Minor changes
 *                         WM                       Moved transceiver initialization
 *  01.02.00   2004-04-23  ACP                      Minor changes - version check added
 *                         WM                       Removed header include
 *  01.03.00   2004-04-24  AMN                      Reworked the way local data is layed out
 *  01.04.00   2004-07-08  MHz                      Minor changes
 *                         CB                       Support of ANSI CANoe
 *  01.05.00   2004-07-26  WM                       Call of ClrWDInit(), FblCanErrorTask() added
 *  01.06.00   2004-10-21  Ap      ESCAN00010007    Adaptation to support Vector
 *                                                  standard components
 *  01.07.00   2004-12-23  AMN     ESCAN00010448    SessionTimeout in case of ping
 *                                                  corrected from 30s to 5s
 *                                                  Added encapsulation of ApplFblCanParamInit
 *                                                  Added definition FBL_CLIENT_FORD_GGDS_COMMENT
 *                                                  Define name changes PBL->CAN
 *                         ACP                      Minor change for FORD & TesterTimeout
 *                                                  Version check reworked
 *  01.08.00   2005-01-14  CB                       Minor changes for version check
 *  01.09.00   2005-01-21  Rr                       Changed version defines due to new naming
 *                                                  convention
 *  01.10.00   2005-01-26  Ap                       Changed order of initialization starting with
 *                                                  CanInitPowerOn()
 *  01.11.00   2005-03-22  WM      ESCAN00011582    MISRA warnings
 *  01.12.00   2005-08-10  AMN                      Added version check using v_ver.h
 *  01.13.00   2005-08-25  MFR                      Add FBL_CLIENT_GM.
 *  01.14.00   2005-10-25  Rr      ESCAN00013441    Add initialization of fblMode
 *  01.15.00   2005-10-28  FHE                      Added support for CANoe emulation
 *  01.16.00   2005-11-09  MFR                      Minor change: cleanup indentation of #define
 *                                                  for GM.
 *  01.17.00   2006-03-22  QPs     ESCAN00015791    Added Gateway implementation
 *                                                  support for Ford I3 Bootloader
 *  01.18.00   2006-05-18  ACP     ESCAN00015877    Allow HW-specific types for main()
 *                         Ls      ESCAN00011881    Added FblLookForWatchdog() to FblRepeat()
 *                         ACP     ESCAN00016437    Moved memcopy of CanIdTable to default layer
 *                                 ESCAN00016438    Added kStartupPreInit/PostInit definition
 *                                                  for new ApplFblStartup() parameter
 *  01.19.00   2006-07-27  Ls      ESCAN00017084    added Dualprocessor functionality
 *  01.20.00   2006-08-23  Ci      ESCAN00017390    Added token for restoration of
 *                                                  FBL context after call to FblStart
 *  01.21.00   2006-09-12  QPs     ESCAN00017639    Changed prototype of kFblCanIdTable
 *  01.22.00   2006-11-10  swk     ESCAN00018330    Re-organized of fblStartMagicFlag
 *                                                  and CanInitTable. Added new memory mapping.
 *  01.23.00   2007-02-05  Hp                       Allow call to FblRealTimeSupport in main-loop instead of
 *                                                  FblLookForWatchdog for some OEMs.
 *                         Ci      ESCAN00019445    Gateway implementation: Initialize
 *                                                  second CAN channel in FblHardwareInit()
 *  01.24.00   2007-09-27  Ls      ESCAN00022542    FBL_ENABLE_PRE_WDINIT_COMMENT added
 *  01.25.00   2008-03-07  Hp      ESCAN00024976    Add support for timer interrupt management
 *                                                  (SIP and configuration dependent)
 *  01.26.00   2008-03-28  Ci      ESCAN00025071    Added call to ApplFblStartup()
 *                                                  for stay-in-boot configurations
 *                         JHg     ESCAN00025354    Remove COMMENT restriction from FBL_ENABLE_PRE_WDINIT
 *                                 ESCAN00025355    FBL_ENABLE_PRE_TIMERINIT added
 *  01.27.00   2008-06-13  Ci      ESCAN00027328    Added support for stay-in-boot
 *                                                  with Vector standard CAN driver
 *                                 ESCAN00027329    Removed redundant call of
 *                                                  CanErrorTask()
 *  01.28.00   2008-07-25  AWh     ESCAN00028748    corrected memory qualifiers for kFblCanIdTable
 *  01.29.00   2008-09-10  JHg     ESCAN00029859    Added preinitalization function FblPreInit
 *                                                  Moved PRE_WDINIT and PRE_TIMERINIT from fbl_ap
 *  01.30.00   2009-03-03  QPs     ESCAN00033268    Communication wrapper interface introduced
 *                         Rr      ESCAN00033426    Rework structure and Stay in Boot handling
 *                         Ach     ESCAN00033591    Changed memory qualifier for kFblCanIdTable on 16fx
 *  01.31.00   2009-08-18  Rr      ESCAN00037032    Updates due to changes in CW, WD, MIO and HW API handling
 *                                 ESCAN00037191    Add FblDeinit function: Loop in FblRepeat can now be
 *                                                  left to let main() call FblDeinit() and ApplFblReset()
 *  01.32.00   2009-09-24  Rr      ESCAN00037191    Add support for C_COMP_ANSI_CANOE
 *  01.33.00   2009-11-16  Ach     ESCAN00039190    Reset CW initialized flag in main()
 *  01.34.00   2010-02-15  Rr      ESCAN00040822    Change handling of FblCanMsgTransmitted()
 *  01.35.00   2010-08-04  Ci      ESCAN00043842    Added support for gateway component Gw_Route
 *                                 ESCAN00043844    Resolved MISRA findings
 *  01.36.00   2011-03-01  Ci      ESCAN00048985    Moved gateway CAN initialization to default layer
 *  01.37.00   2011-09-22  QPs     ESCAN00053782    COM-Wrapper interface reworked.
 *                         Rr      ESCAN00053912    Rework of CanInitTable section
 *  01.38.00   2011-11-21  Ach     ESCAN00055022    Support stay-in-boot with ComWrapper
 *  01.39.00   2012-03-23  CB      ESCAN00055697    Support for multiple platforms
 *                         Ach     ESCAN00056458    Move application start to callback function
 *                         CB      ESCAN00057444    Polling of platform message
 *                         CB      ESCAN00057664    Added call of FblMemTask()
 *                         QPs     ESCAN00057763    Added code to enable interrupts for bus communication
 *  01.40.00   2012-05-22  Ach     ESCAN00059074    Call FblLookForWatchdog() if timer interrupt is used
 *  01.41.00   2012-08-10  Ci      ESCAN00060597    Reworked handling of start magic tokens
 *  01.42.00   2012-11-30  Ach     ESCAN00062912    Adapted comments to use Doxygen
 *  01.43.00   2013-05-16  Ach     ESCAN00067266    Support XCP Bootloader addon
 *  01.44.00   2013-06-20  CB      ESCAN00068192    Added CW flags for start message window
 *  01.45.00   2013-07-02  ASe     ESCAN00068504    Fixed CW flags for start message window
 *  01.46.00   2013-08-16  Ci      ESCAN00069371    Added support for pipelined verification
 *  01.47.00   2013-09-03  CB      ESCAN00070205    Extended XCP support
 *  01.48.00   2013-11-15  Awh     ESCAN00070492    Define CanInitTable also in case FblStart feature is disabled
 *  01.49.00   2014-01-14  ASe     ESCAN00072927    Minor rework
 **********************************************************************************************************************/

/**********************************************************************************************************************
|             R E V I S I O N   H I S T O R Y (KbFbl_mainOem.c, GM)
| ---------------------------------------------------------------------------------------------------------------------
|  Version    Date       Author  Change Id      Description
| ---------------------------------------------------------------------------------------------------------------------
| 2014-01-31  05.00.00   AWh    -               First Release
| 2014-04-17  05.01.00   Dod    -               No access to signature verification function in the application
| 2014-12-08  05.02.00   AWh    -               Merge of all Gm SLPs
|                                               SLP6: Add call to ApplFblTask() no more called from new core diag.
|                                               Remove FblHeader Address check that now comes from default layer
|                                               Compiler error: Wrong pointer type used as initializer
|**********************************************************************************************************************/
/***********************************************************************************************************************
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY (KbFbl_mainHW.c, TMS470)
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  01.00.00   2004-04-01  Cb                       First Implementations
 *  01.01.00   2004-05-04  Cb                       Section definition for MagicFlag buffer
 *  01.02.00   2006-04-03  Rr                       Reworked Arm Section Pragmas
 *  01.03.00   2007-07-27  AWh                      Added Version Information
 *  01.04.00   2007-11-02  AWh     ESCAN00023082    Added GHS section pragmas,
 *                                                  equalized section names, added CanInitTable - section
 *  01.05.00   2008-08-15  Rr                       Reworked CanInitTable section pragma for TI
 *  01.06.00   2009-02-27  JHg     ESCAN00033356    Added support for TI OMAP
 *  01.07.00   2009-09-24  MVi     ESCAN00038014    Added support for TI TMS570
 *  01.08.00   2010-03-31  PAL     ESCAN00042009    Added support for C_COMP_ARM_TMS470_DCAN
 *  01.09.00   2013-07-04  ASe     ESCAN00068151    Adapted comments to use doxygen
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY (KbFbl_mainDefault.c)
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  01.00.00   2004-02-24  CB                       Release
 *  01.01.00   2004-03-25  CB                       Module reworked
 *                         Rr                       Minor changes
 *                         WM                       Minor changes
 *  01.02.00   2004-07-08  CB                       Support of ANSI CANoe
 *  01.03.00   2004-07-26  WM                       Added call of FblCanParamInit()
 *  01.04.00   2004-10-21  Ap      ESCAN00010007    Adaption to support Vector
 *                                                  standard components
 *  01.05.00   2004-12-23  ACP                      Version check reworked
 *  01.06.00   2005-01-21  Rr                       Changed version defines due to naming
 *                                                  convention
 *  01.07.00   2005-03-22  WM      ESCAN00012119    MISRA warnings
 *  01.08.00   2005-04-29  WM      ESCAN00012116    Remove call of ApplFblWDShort()
 *                                                  in FBL_ENABLE_START_OVER_RESET configuration
 *  01.09.00   2005-08-10  AMN                      Added version check using v_ver.h
 *  01.10.00   2005-10-28  FHE                      Added support for CANoe emulation
 *  01.11.00   2006-05-18  ACP     ESCAN00015877    Allow HW-specific types for main()
 *                                 ESCAN00016437    Moved memcopy of CanIdTable to
 *                                                  default layer
 *  01.12.00   2006-07-27  Ls      ESCAN00017085    added Dualprocessor functionality
 *  01.13.00   2006-11-10  swk     ESCAN00018330    Re-organization of fblStartMagicFlag
 *                                                  and CanInitTable. Added new memory mapping
 *  01.14.00   2007-01-23  Hp                       Frame uses a token to call the trigger-watchdog
 *                                                  function
 *  01.15.00   2007-08-27  FHe     ESCAN00013178    FBL_ENABLE_ROMMASK_FLAG added
 *  01.16.00   2007-09-27  JHg                      Moved rommask check to OEM specific code
 *                         Ls      ESCAN00022543    FBL_ENABLE_FBL_START added
 *  01.17.00   2008-03-28  Ci      ESCAN00025271    Added state handling for
 *                                                  stay-in-boot
 *                         JHg     ESCAN00025354    Remove COMMENT restriction from FBL_ENABLE_PRE_WDINIT
 *                                 ESCAN00025355    FBL_ENABLE_PRE_TIMERINIT added
 *                                 ESCAN00025356    Call ApplFblWDLong() and FblTimerStopp()
 *                                                  before transition to the application
 *  01.18.00   2008-06-13  Ci      ESCAN00027328    Added clear macro for reset of
 *                                                  the stay-in-boot state
 *                                 ESCAN00027546    Harmonized stay-in-boot duration
 *  01.19.00   2008-06-19  Ls      ESCAN00027717    Added BuildVersion information
 *                         Rr      ESCAN00033427    Moved Prototypes into corresponding layer
 *  01.20.00   2009-03-03  Rr      ESCAN00037034    Updates due to changes in CW API handling
 *  01.21.00   2010-05-26  Ci      ESCAN00041834    Use FblInvertBits() macro to
 *                                                  avoid compiler warnings
 *                         MVi     ESCAN00043051    Use renamed FblTp functions
 *  01.22.00   2010-06-28  CB      ESCAN00043639    Adaption for Vector TPMC
 *  01.23.00   2010-09-24  Ci      ESCAN00045550    Take FBL tick duration into
 *                                                  account for task cycle generation
 *  01.24.00   2011-03-01  Ci      ESCAN00048989    Removed explicit casts in MEMCPY command
 *                                 ESCAN00048986    Added gateway CAN initialization
 *  01.25.00   2011-08-30  QPs     ESCAN00053150    Remove CanInitTable when using memmap
 *                         Rr      ESCAN00053913    Rework of CanInitTable section
 *  01.26.00   2012-03-23  CB      ESCAN00055696    Support for multiple platforms
 *  01.27.00   2012-08-10  Ci      ESCAN00060477    Enable MEMMAP mechanism for fblStartMagicFlag
 *                                 ESCAN00060478    Extended FblStart magic pattern
 *  01.28.00   2012-09-10  Rr      ESCAN00061288    Make fblStartMagicFlag volatile
 *  01.29.00   2012-11-30  Ach     ESCAN00062912    Adapted comments to use Doxygen
 *  01.30.00   2013-04-30  Ach     ESCAN00066924    Added explicit FblDiagTimerTask() call
 *                                 ESCAN00067048    Remove CanInitTable from default FblStart() implementation
 *  01.31.00   2014-01-14  ASe     ESCAN00072926    Fixed compiler warning in FblStart() implementation, minor rework
 *  01.32.00   2014-08-22  Ach     ESCAN00077660    Remove switch to choose between FblDiagTask() and FblDiagTimerTask()
 *                         QPs     ESCAN00077987    Added check of header address
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "fbl_inc.h"
# include "fbl_tp.h"

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  VERSION
 **********************************************************************************************************************/

/* ##V_CFG_MANAGEMENT ##CQProject : FblKbMain_Frame CQComponent : Implementation */
#define FBLKBMAIN_FRAME_VERSION           0x0149u
#define FBLKBMAIN_FRAME_RELEASE_VERSION   0x00u

#if ( FBLKBMAIN_FRAME_VERSION != _FBLKBMAIN_FRAME_VERSION ) || \
    ( FBLKBMAIN_FRAME_RELEASE_VERSION != _FBLKBMAIN_FRAME_RELEASE_VERSION )
# error "Error in FBL_MAIN.C: Source and v_ver.h are inconsistent!"
#endif

/* ---  Module Version --- */
/* ##V_CFG_MANAGEMENT ##CQProject : FblKbMain_GM CQComponent : Implementation */
#define FBLKBMAIN_GM_VERSION           0x0502u
#define FBLKBMAIN_GM_RELEASE_VERSION   0x00u

#if (FBLKBMAIN_GM_VERSION != _FBLKBMAIN_OEM_VERSION) || \
    (FBLKBMAIN_GM_RELEASE_VERSION != _FBLKBMAIN_OEM_RELEASE_VERSION)
#error "Error in FBL_MAIN.C: Source and v_ver.h are inconsistent!"
#endif

/* ##V_CFG_MANAGEMENT ##CQProject : FblKbMain_Tms470 CQComponent : Implementation */
#define FBLKBMAIN_TMS470_VERSION           0x0109u
#define FBLKBMAIN_TMS470_RELEASE_VERSION   0x00u

#if ( FBLKBMAIN_TMS470_VERSION != _FBLKBMAIN_HW_VERSION ) || \
    ( FBLKBMAIN_TMS470_RELEASE_VERSION != _FBLKBMAIN_HW_RELEASE_VERSION )
#error "Error in FBL_MAIN.C: Source and v_ver.h are inconsistent!"
#endif

/* ##V_CFG_MANAGEMENT ##CQProject : FblKbMain_Default CQComponent : Implementation */
#define FBLKBMAIN_DEFAULT_VERSION         0x0132u
#define FBLKBMAIN_DEFAULT_RELEASE_VERSION 0x00u

#if ( FBLKBMAIN_DEFAULT_VERSION != _FBLKBMAIN_DEFAULT_VERSION ) || \
    ( FBLKBMAIN_DEFAULT_RELEASE_VERSION != _FBLKBMAIN_DEFAULT_RELEASE_VERSION )
# error "Error in FBL_MAIN.C: Source and v_ver.h are inconsistent!"
#endif

#if ( FBLKBMAIN_DEFAULT_VERSION < 0x0119 )
# error "A higher default layer version is required for compatibility reasons"
#endif

#if defined( FBL_ENABLE_STAY_IN_BOOT )
#endif /* FBL_ENABLE_STAY_IN_BOOT */

#if defined( FBL_ENABLE_STAY_IN_BOOT ) 
# ifndef FBL_START_DELAY
#  define FBL_START_DELAY (20u / FBL_REPEAT_CALL_CYCLE)  /* Wait 20 ms before jump into application */
# endif
# define SetFblWaitForPing() (fblMode |= WAIT_FOR_PING)
# define ClrFblWaitForPing() (fblMode &= FblInvert8Bit(WAIT_FOR_PING))
#endif

#if !defined( FBL_TASK_CALL_CYCLE )
# define FBL_TASK_CALL_CYCLE   TpCallCycle
#endif

/* Check consistency of FBL tick duration and task call cycle */
#if ( ((FBL_TASK_CALL_CYCLE / FBL_REPEAT_CALL_CYCLE) * FBL_REPEAT_CALL_CYCLE) != FBL_TASK_CALL_CYCLE )
# error "The task call cycle must be an integer multiple of FBL_REPEAT_CALL_CYCLE"
#endif

#if defined( FBL_ENABLE_SYSTEM_CHECK )
#undef FBL_ENABLE_SYSTEM_CHECK
#endif

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

/* -- FBL header -- */
#if defined( FBL_ENABLE_FBL_START )
void FblStart( tCanInitTable *pCanInitTable );
#endif

# define FBLHEADER_START_SEC_CONST
# include "memmap.h"
V_MEMROM0 V_MEMROM1 tFblHeader V_MEMROM2 FblHeader =
{
   FBL_GMHEADER_CSUM,                     /* Module Checksum                           */
   FBL_GMHEADER_MODID_FBL,                /* Module ID                                 */
   FBL_GMHEADER_ECUNAME,                  /* Ecu Name                                  */
   FBL_GMHEADER_ECUID,                    /* Ecu Id                                    */
   FBL_GMHEADER_SUBJNAME,                 /* Subject name                              */
   FBL_GMHEADER_SWMI_FBL,                 /* SoftWare Module Identifier                */
   FBL_GMHEADER_DLS_FBL,                  /* Design Level Suffix                       */
   FBL_GMHEADER_DCID_FBL,                 /* Data Compatibility Identifier             */
   {0x00u, FBL_GMHEADER_NOAR_FBL},        /* Number Of Address Regions                 */
   FBL_GMHEADER_ADDR_REG,                 /* Product Memory Address + Number Of Bytes  */
#if defined( FBL_ENABLE_FBL_START )
   FblStart,                              /* FBL start function                        */
#else
   NULL,
#endif
#if defined( SEC_SECURITY_CLASS )
# if ( (SEC_SECURITY_CLASS == SEC_CLASS_C) || (SEC_SECURITY_CLASS == SEC_CLASS_CCC) )
   (tExportFct)SecM_VerifySignature,
# endif
# if defined( FBL_ENABLE_KEY_EXPORT )
#  if ( SEC_SECURITY_CLASS == SEC_CLASS_CCC )
   {SecM_RsaMod, sizeof(SecM_RsaMod)},
   {SecM_RsaExp, sizeof(SecM_RsaExp)},
#  endif
# endif /* FBL_ENABLE_KEY_EXPORT */
#endif /* SEC_SECURITY_CLASS */
   (vuint8)(FBLOEM_GM_VERSION >> 8u),      /* FBL main version                          */
   (vuint8)(FBLOEM_GM_VERSION & 0xffu),    /* FBL sub version                           */
   (vuint8)FBLOEM_GM_RELEASE_VERSION,     /* FBL bugfix version                        */
#ifdef _VECTOR_SIP_BUILD_VERSION
   (vuint8)_VECTOR_SIP_BUILD_VERSION,     /* build version from v_ver.h                */
#else
   (vuint8)FBL_FLASH_DELETED,             /* Align data                                */
#endif
};
# define FBLHEADER_STOP_SEC_CONST
# include "memmap.h"

#  define CANPARAM_START_SEC_DATA_EXPORT
#  include "memmap.h"
tCanInitTable CanInitTable;
#  define CANPARAM_STOP_SEC_DATA_EXPORT
#  include "memmap.h"

#if defined( FBL_ENABLE_FBL_START)
#  define MAGICFLAG_START_SEC_DATA
#  include "memmap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
volatile vuint8 fblStartMagicFlag[kFblNoOfMagicBytes];
#  define MAGICFLAG_STOP_SEC_DATA
#  include "memmap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
#endif /* FBL_ENABLE_FBL_START */

/* CAN initialization structure */
V_MEMROM0 extern V_MEMROM1 tFblCanIdTable V_MEMROM2 kFblCanIdTable;
tFblCanIdTable fblCanIdTable;

MEMORY_NEAR vuint8 fblMode;

/***********************************************************************************************************************
 *  LOCAL DATA
 **********************************************************************************************************************/

static vsint8 callCounter;
#if defined( FBL_ENABLE_STAY_IN_BOOT ) 
static vuint16 startDelay;
#endif /* FBL_ENABLE_STAY_IN_BOOT || FBL_ENABLE_MULTIPLE_PLATFORMS */

/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

static void FblHardwareInit( void );
static void FblPreInit( void );
static void FblInit( void );
static void FblDeinit( void );
static void FblRepeat( void );
static void FblStartAppl( void );
#if defined( FBL_ENABLE_STAY_IN_BOOT ) 
static void FblRepeatCheckForStartMsg( void );
#endif

/***********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  FblStartAppl
 **********************************************************************************************************************/
/*! \brief       This function is called for leaving bootloader and enter application software.
 *  \pre         Only call if application is present and valid
 **********************************************************************************************************************/
static void FblStartAppl( void )
{
#if defined( FBL_ENABLE_STAY_IN_BOOT )
   FblDeinit();
#else
# if defined( FBL_ENABLE_PRE_WDINIT )
#  if defined( FBL_WATCHDOG_ON )
   /* Synchronize watchdog before transition */
   ApplFblWDLong();
#  endif /* FBL_WATCHDOG_ON */
# endif /* FBL_ENABLE_PRE_WDINIT */
# if defined( FBL_ENABLE_PRE_TIMERINIT )
   /* Stopp timer to avoid a timer interrupt *
    * after application start                */
   FblTimerStopp();
# endif /* FBL_ENABLE_PRE_TIMERINIT */
#endif /* FBL_ENABLE_STAY_IN_BOOT */

   /* Start application code */
   ApplFblStartApplication();
}

#if defined( FBL_ENABLE_STAY_IN_BOOT ) 
/***********************************************************************************************************************
 *  FblRepeatCheckForStartMsg
 **********************************************************************************************************************/
/*! \brief       This function waits during the acceptance window if the start message is received.
 *  \pre         Hardware has to be initialized.
 **********************************************************************************************************************/
static void FblRepeatCheckForStartMsg( void )
{
   while (startDelay > 0)
   {
      /* Check for CAN errors */
      FblCanErrorTask();

      /* Check if 1ms hardware timer expired */
#  if (FBLWD_VERSION >= 0x0172)
      if ((FblLookForWatchdog() & FBL_TM_TRIGGERED) == FBL_TM_TRIGGERED)
#  else
      if (FblTimerGet() != 0)
#  endif
      {
         /*--------------------------------------------------------*/
         /*--- 1ms is over. Trigger timer counter and tasks -------*/
         /*--------------------------------------------------------*/
# if (FBLWD_VERSION >= 0x0172)
# else
         (void)FblLookForWatchdog();
# endif
         startDelay--;
         if (startDelay == 0)
         {
            /* startDelay expired, start message not received */

# if defined( FBL_ENABLE_STAY_IN_BOOT)
            FblStartAppl();
            /* Function should never return */
# endif
         }
      }

      if ((FblCanReceive() != 0) && (FblCanStartMsg() == kFblOk))
      {
         /* Startup message has been received.                        */
         /* Set corresponding states and perform processings.         */
         SetFblMode(FBL_START_WITH_PING);
# if defined( FBL_ENABLE_STAY_IN_BOOT )
         ApplFblStartup(kStartupStayInBoot);
# endif /* FBL_ENABLE_STAY_IN_BOOT */
         FblCanProcessRxMsg();

         startDelay = 0;
         ClrFblWaitForPing(); /* PRQA S 0277 */ /* MD_FblMain_0277 */
# if defined( FBL_ENABLE_STAY_IN_BOOT )
         ResetTesterTimeout();
# endif
      }
   }
}
#endif /* FBL_ENABLE_STAY_IN_BOOT || FBL_ENABLE_MULTIPLE_PLATFORMS */

/***********************************************************************************************************************
 *  FblRepeat
 **********************************************************************************************************************/
/*! \brief       This function contains the main loop of the FBL.
 *  \details     In this loop, the Bus is polled and the watchdog is triggered.
 *               This function is used by the boot procedure for the start delay and for the real operation.
 *  \pre         Hardware has to be initialized before.
 *  \return      The return value is used by the boot procedure to determine whether to stay in FBL or start the appl.
 **********************************************************************************************************************/
static void FblRepeat( void )
{
   /* This loop will be left only for reset */
   while ((fblMode & FBL_RESET_REQUEST) != FBL_RESET_REQUEST)
   {
      /* Check for CAN errors */
      FblCanErrorTask();

      /* Check if 1ms hardware timer expired */
#  if (FBLWD_VERSION >= 0x0172)
      if ((FblLookForWatchdog() & FBL_TM_TRIGGERED) == FBL_TM_TRIGGERED)
#  else
      if (FblTimerGet() != 0)
#  endif
      {
         /*--------------------------------------------------------*/
         /*--- 1ms is over. Trigger timer counter and tasks -------*/
         /*--------------------------------------------------------*/
#if (FBLWD_VERSION >= 0x0172)
#else
         (void)FblLookForWatchdog();
#endif
         {
            callCounter++;
            if (callCounter >= (FBL_TASK_CALL_CYCLE / FBL_REPEAT_CALL_CYCLE))
            {
               callCounter = 0u;
               /* Call transport layer cyclic function */
               FblTpTask();
               /* Call diagnosis layer cyclic function */
               FblDiagTimerTask();
            }
            /* -- Sleep mode timing --*/
            if (0u != sleepCounter)
            {
               sleepCounter--;
               if (0u == sleepCounter)
               {
                     ApplFblEnterStopMode();
                     /* Reload sleep counter                                   */
                     FblSleepCounterReload();
               }
            }
         }
      }
      /* Polling of CAN receive messages */
      if (FblCanReceive() != 0)
      {
         /*-----------------------------------------------------------------*/
         /*--- CAN messages has been received. Check for new data.          */
         /*-----------------------------------------------------------------*/
         {
         /* Process the RxMsg if the ID is valid                            */
         /* and call FblCanHandleRxMsg with a handle                        */
            FblCanProcessRxMsg();
         }
      }

#if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED )
      if (FblMemTaskIsPending())
      {
         FblMemTask();
      }
#endif
   }
}

/***********************************************************************************************************************
 *  FblHardwareInit
 **********************************************************************************************************************/
/*! \brief       Initialization of hardware (timer, Bus-interface)
 **********************************************************************************************************************/
static void FblHardwareInit( void )
{

#if defined( FBL_ENABLE_PRE_TIMERINIT )
   /* Initialization of timer in FblPreInit() */
#else
   /* Initialize Timer */
   FblTimerInit();
#endif /* FBL_ENABLE_PRE_TIMERINIT */

   /* Activate Transceiver */
   ApplTrcvrNormalMode();
   /* Initialize CAN interface */
   FblCanInit();
}

/***********************************************************************************************************************
 *  FblPreInit
 **********************************************************************************************************************/
/*! \brief       Do preinitializations after reset
 **********************************************************************************************************************/
static void FblPreInit( void )
{
#if defined( FBL_ENABLE_PRE_WDINIT )
   /* Initialize and activate watchdog */
   FblInitWatchdog();
   ApplFblWDInit();
#endif /* FBL_ENABLE_PRE_WDINIT */
#if defined( FBL_ENABLE_PRE_TIMERINIT )
   /* Initialize Timer */
   FblTimerInit();
#endif /* FBL_ENABLE_PRE_TIMERINIT */

   fblStates = 0u;

   fblProgrammedState  = 0u;

   /* Sleep timer not running during startup */
   FblSleepCounterClear();

   FblHdrInitPowerOn();
}

/***********************************************************************************************************************
 *  FblInit
 **********************************************************************************************************************/
/*! \brief       Do initializations after reset or start out of application
 **********************************************************************************************************************/
static void FblInit( void )
{
#ifdef FBL_ENABLE_PRE_WDINIT
   /* Initialization of WD in FblPreInit() */
#else
   /* Initialize and activate watchdog */
   FblInitWatchdog();
   ApplFblWDInit();                       /* Initialize Watchdog             */
#endif /* FBL_ENABLE_PRE_WDINIT */

   /* -- Copy CAN initialization data into RAM structure -- */
   /* Use reset CAN initialization data */
   (void)MEMCPY(&fblCanIdTable, &kFblCanIdTable, sizeof(tFblCanIdTable));

# if defined( FBL_ENABLE_MULTIPLE_NODES )||\
     defined( FBL_ENABLE_CAN_CONFIGURATION)
   /* Callback function for ECU-specific CAN initialization */
   ApplFblCanParamInit();
# endif

   if ((GetFblMode() & START_FROM_APPL) != 0u)
   {
      FblCanParamInit();
   }

   /* Counter value for call of cyclic functions */
   callCounter = 0;

#if defined( FBL_ENABLE_SYSTEM_CHECK )
   if (FblHeaderLocal != FblHeaderTable)   /* PRQA S 0306 */ /* MD_FblMain_0303_0306 */
   {
      /* Header address not configured correctly, halt FBL */
      while (1)
      {
         ;
      }
   }
#endif
   /* Initialize hardware (timer, Bus-interface) */
   FblHardwareInit();

   /* Initialize Transport Layer */
   FblTpInitPowerOn();

   /* Initialize diagnostic module */
   FblDiagInit();

   if ((GetFblMode() & START_FROM_APPL) != 0u)
   {
      FblDiagInitStartFromAppl();
   }

   /* Initialize sleep mode counter */
   FblSleepCounterReload();
}

/***********************************************************************************************************************
 *  FblDeinit
 **********************************************************************************************************************/
/*! \brief       Do necessary deinitializations
 **********************************************************************************************************************/
static void FblDeinit( void )
{
# if ( FBLDRVCAN_REFERENCE_IMPLEMENTATION >= 0x0102u )
   FblCanStop();
# endif
#if defined( FBL_WATCHDOG_ON )
   ApplFblWDLong();
#endif /* FBL_WATCHDOG_ON */
   FblTimerStopp();  /* PRQA S 3109 */ /* MD_CBD_14.3 */
}

/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  main
 **********************************************************************************************************************/
/*! \brief       If a valid application program is available, the application will be started.
 *  \details     Otherwise the FBL initializes the hardware and waits for a download request.
 **********************************************************************************************************************/
int V_API_NEAR main( void )
{
   /* Disable all interrupts */
   FblInterruptDisable();  /* PRQA S 3109 */ /* MD_CBD_14.3 */

   /* Clear watchdog initialized flag */
   ClrWDInit();

   /* Init fblMode */
   fblMode = 0;

   SetFblMode(START_FROM_RESET);

#define FBL_NO_JSR_APPL
#if defined( FBL_ENABLE_STAY_IN_BOOT ) 
   startDelay = 0;   /* Initialize counter for start message window*/
#endif /* FBL_ENABLE_STAY_IN_BOOT */

   /* Preinitialization step */
   FblPreInit();

   /* Call application function */
   ApplFblInit();

#if defined( FBL_NO_JSR_APPL )
#else
   /* Run application or flash boot loader */
   if (ApplFblExtProgRequest() != kProgRequest)
   {
      if (ApplFblIsValidApp() == kApplValid)
      {
# if defined( FBL_ENABLE_STAY_IN_BOOT )
         /* Valid application present, activate stay-in-boot */
         SetFblWaitForPing();
         /* Delay time to wait for stay-in-boot message */
         startDelay = FBL_START_DELAY + 1u;
# else   /* FBL_ENABLE_STAY_IN_BOOT */
         FblStartAppl();
# endif /* FBL_ENABLE_STAY_IN_BOOT */

         /* Code after the previous call is never reached */
      }
   }
   else
   {
      SetFblMode(START_FROM_APPL);
   }
#endif

   /* Do all initializations */
   ApplFblStartup(kStartupPreInit);

   FblInit();

   ApplFblStartup(kStartupPostInit);

#if defined( FBL_ENABLE_STAY_IN_BOOT ) 
   FblRepeatCheckForStartMsg();
#endif

   /* Polling of Bus events */
   FblRepeat();

   /* Deinitialize bootloader */
   FblDeinit();

   /* Perform reset */
   ApplFblReset();

   return 0; /* Return statement is never reached */
} /* PRQA S 6050 */ /* MD_FblMain_STCAL */

#if defined( FBL_ENABLE_FBL_START )
/***********************************************************************************************************************
 *  FblStart
 **********************************************************************************************************************/
/*! \brief       Transition from application into flash boot loader
 *  \pre         Valid application program running
 *  \param[in]   pCanInitTable CanInitTable pointer with CAN initialization data
 **********************************************************************************************************************/
void FblStart( tCanInitTable * pCanInitTable )
{
   FblInterruptDisable();  /* PRQA S 3109 */ /* MD_CBD_14.3 */

#if defined( FBL_USE_OWN_MEMCPY )
   ClrWDInit();
#endif

   /* Copy CanInitTable */
   /* Be careful that the location of CanInitTable does not overlap  *
    * with the application stack, that the FBL startup code does     *
    * not zero memory containing CanInitTable, and that a RESET      *
    * does not in any way corrupt RAM.                               */
   if ((pCanInitTable != NULL) && (pCanInitTable != &CanInitTable))
   {
      CanInitTable = *pCanInitTable;
   }

   /* Set FblStart magic flag */
   /* Please pay attention: The FblStart magic flag does not use     *
    * a shared memory area so that the buffer may overlap with       *
    * the application stack.                                         */
   FblSetFblStartMagicFlag();

   /* Generate reset */
   APPL_FBL_RESET();
}

#endif /* FBL_ENABLE_FBL_START */

/***********************************************************************************************************************
 *  MISRA DEVIATIONS
 **********************************************************************************************************************/

/* module specific MISRA deviations:

   MD_FblMain_0277:
     Reason: Usage of FblInvert.. macros implies usage of ~ operator and appropriate casts. This includes a cast from negative integer to unsigned value.
             The integer is only an intermediate type that occurs because of integer promotion while using ~ operator, though only a bit mask is operated on.
     Risk: No risk.
     Prevention: No prevention defined.

   MD_FblDiag_STCAL:
     Reason: A typical approach to reduce STCAL is deeper nesting of functions. This approach is not followed due to
             higher voted requirements for minimized stack and runtime usage applied on the code.
     Risk: Understandability and testability might become too complex due to fan-out to many functions. 
     Prevention: Design and code review + clearly structured and commented code.

*/

/***********************************************************************************************************************
 *  END OF FILE: FBL_MAIN.C
 **********************************************************************************************************************/
