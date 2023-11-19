/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        OSEK transport protocol version ISO-15765
 *  \description  This versions supports the specification for the OSEK transport protocol with the following
 *                restriction: Maximum message length is 255 byte for RX and TX (no length in TPCI).
 *
 *  \attention    In case using the extended addressing mode with different targets a consistency problem for the
 *                target address may occur, if an unsuccessful SaveTransmit of an FlowControl is delayed and within
 *                this call cycle (e.g. 10ms) the TpTransmit function is called.
 *
 *  --------------------------------------------------------------------------------------------------------------------
 *  COPYRIGHT
 *  --------------------------------------------------------------------------------------------------------------------
 *  \par Copyright
 *  \verbatim
 *  Copyright (c) 2013 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *                This software is only allowed to be used for and with the VECTOR Flash Bootloader!
 *  \endverbatim
 */
/**********************************************************************************************************************/

/***********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  --------------------------------------------------------------------------------------------------------------------
 *  Thomas Balster                Bs            Vector Informatik GmbH
 *  Bernd Baudermann              Ba            Vector Informatik GmbH
 *  Steffen Braendle              Bd            Vector Informatik GmbH
 *  Christian Baeuerle            CB            Vector Informatik GmbH
 *  Gunnar de Haan                dh            Vector Informatik GmbH
 *  Thomas Ebert                  Et            Vector Informatik GmbH
 *  Guenther Foerstner            Fr            Vector Informatik GmbH
 *  Ralf Fritz                    Fz            Vector Informatik GmbH
 *  Alexander Gutlederer          Gu            Vector Informatik GmbH
 *  Oliver Garnatz                Gz            Vector Informatik GmbH
 *  Hartmut Hoerner               Hh            Vector Informatik GmbH
 *  Heike Honert                  Ht            Vector Informatik GmbH
 *  Bernd Jesse                   Js            Vector Informatik GmbH
 *  Patrick Markl                 Ml            Vector Informatik GmbH
 *  Rainer Mueller                Mr            Vector Informatik GmbH
 *  Georg Pfluegel                Pl            Vector Informatik GmbH
 *  Jochen Rein                   Rn            Vector Informatik GmbH
 *  Mishel Shishmanyan            Sa            Vector Informatik GmbH
 *  Bernd Sigle                   Si            Vector Informatik GmbH
 *  Jochen Theuer                 Th            Vector Informatik GmbH
 *  Konrad Lazarus                Ls            Vector Informatik GmbH
 *  Marco Wierer                  WM            Vector Informatik GmbH
 *  Robert Schaeffner             Rr            Vector Informatik GmbH
 *  Michael F. Radwick            MFR           Vector CANtech, Inc.
 *  Quetty Palacios               QPs           Vector Informatik GmbH
 *  Marcel Viole                  MVi           Vector Informatik GmbH
 *  Andre Caspari                 Ci            Vector Informatik GmbH
 *  Thomas Bezold                 TBe           Vector Informatik GmbH
 *  Andreas Wenckebach            AWh           Vector Informatik GmbH
 *  Achim Strobelt                Ach           Vector Informatik GmbH
 *  Alexander Starke              ASe           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  02.52.00   2000-10-05  Js                       can_inc.h included, TP_CHECK_FLOW_STATE corrected
 *             2000-10-25  dH                       Added some compilers for the dummy statement in TpConfirmation
 *             2000-10-30  Hp                       Accept a too large message and indicate error to APP after last CF
 *             2000-11-03  Js                       TP_CAN_CHANNEL added
 *  02.53.00   2000-12-22  Hp                       Order of Init/TpConfirm(Failed) has been changed.
 *                                                  Resetting only Tx-state in case of confirm failed
 *                                                  Removing counter check for TxIntConfirm counter
 *             2001-01-03  Sa                       bRxTimer, bTxTimer, gTpConfTimer are adapted to
 *                                                  8/16 bit depending on the timeout value and processor type
 *             2001-07-12  Ls                       Added the extern fblCanTxObj[] (per define by tpCanTxData)
 *  02.54.00   2001-10-16  Hp                       TpInit() inserted for failure case
 *                                                  wrong STmin calculation on FC corrected
 *  02.55.00   2001-11-28  CB                       No changes
 *  02.56.00   2002-01-09  ACP                      Portation to Hiware HC12 & IAR HC12. FLEXERINT NOT SUPPORTED
 *  02.58.00   2002-09-12  Hp                       ESCAN0000: Adding DLC-Check and ISO-Compliance
 *  02.59.00   2002-11-12  Fz      ESCAN00004157    Cast added
 *  02.60.00   2003-02-04  Hp      ESCAN00004785    Discard STmin and BS on reception of first FC for a specific OEM
 *  02.61.00   2003-02-20  WM                       Removed fblCanTxObj[], Added lint comments
 *                                                  Added stddef.h for size_t
 *  02.62.00   2003-04-01  Rr                       Minor changes for V850SCx
 *  02.63.00   2003-04-30  WM                       Lint cleanup
 *  02.64.00   2003-09-19  Rr                       Minor changes
 *  02.65.00   2003-09-25  Rr                       Minor changes for V850 NEC compiler
 *  02.66.00   2004-02-06  Ls      ESCAN00007291    New OEM, extended addressing,
 *                                                  FBL_FILL_PATTERN added
 *                         CB      ESCAN00007368    Overrun frame added
 *                         WM      ESCAN00007465    Added support for HCS12/IAR
 *  02.67.00   2004-02-27  Rr                       Added new OEM
 *  02.68.00   2004-07-07  Gz      ESCAN00008586    Support for ISO 15765-2 extension
 *  02.69.00   2004-07-16  Ls      ESCAN00008866    FBL_ENABLE_BUFFER_PADDING added
 *  02.70.00   2004-07-20  CB      ESCAN00008829    Added several non-ISO features
 *  02.71.00   2004-08-25  MHz     ESCAN00009326    Added function TpSetSourceAddress() for extendend addressing
 *                         MHz     ESCAN00009328    Added defines FirstDataSFCF and
 *                                                  FirstDataFF for SINGLE_RECEIVE_BUFFER
 *  02.72.00   2004-10-20  MHz                      No changes
 *  02.73.00   2004-11-12  Rr      ESCAN00010244    CF frame handling changed for
 *                                                  CLIENT_RENAULT
 *  02.74.00   2004-11-22  Et      ESCAN00010341    support FBL_USE_OWN_MEMCPY
 *  02.75.00   2004-12-23  ACP                      Version check reworked
 *  02.76.00   2005-03-09  WM      ESCAN00011417    MISRA warnings
 *  02.77.00   2005-04-18  Rr      ESCAN00011940    Add TP_USE_OVERRUN config switch
 *                                 ESCAN00011941    Add TP_USE_WAIT_FOR_CORRECT_SN config switch
 *                                 ESCAN00011942    TP_USE_NO_STMIN_AFTER_FC config switch added
 *  02.78.00   2005-05-17  Rr                       Add variable DLC support (TP_USE_VARIABLE_DLC)
 *  02.79.00   2005-08-10  Mhz     ESCAN00012759    Changed setting of gbRxSourceAddress for OEM Fiat07274
 *                         AMN                      Added version check using v_ver.h
 *  02.80.00   2005-12-21  Ls      ESCAN00014725    moved ApplTpCheckFunctionalTA() check
 *                                                  to top of TpPrecopy() function
 *  02.81.00   2006-08-04  Ls                       added dualprocessor functionality
 *                         Hp      ESCAN00017186    Retry of transmission not handled correctly
 *  02.82.00   2006-09-15  Hp      ESCAN00017709    TP may be blocked when CAN-communcation is disturbed permanenty
 *                                 ESCAN00017706    TpConfirm called twice when Single-Frame
 *                                                  or the last CosencutiveFrame cannot be sent.
 *                         swk     ESCAN00017726    Added support for C_COMP_MTRWRKS_MCS08_MSCAN_COMMENT
 *                         Ls      ESCAN00017768    define for DLC added for 16lx
 *                         Hp      ESCAN00017797    Supporting CONFIRMATION_INTERRUPT with FblCan in polling mode.
 *  02.83.00   2006-09-26  Hp      ESCAN00017833    Changes after internal review
 *  02.84.00   2006-10-13  Hp      ESCAN00017988    FlowControl handling does only work for BS == 0
 *                         Rr                       Minor changes
 *  02.85.00   2006-11-06  Hp      ESCAN00018285    Fbl loses multi frame messages (modifications in TpTask())
 *  02.85.01   2007-01-23  Hp      ESCAN00019153    No function indication "MessageTooLong()" for 8-bit micros
 *                                                  with DiagBuffer lower than 256 bytes
 *  02.85.02   2007-02-27  MFR     ESCAN00019252    Reset TxTimer to STmin upon successful
 *                                                  retransmission of Consecutive Frame.
 *  02.85.03   2007-02-27  QPs     ESCAN00019763    Commonizing interfaces to RI_1.1
 *  02.85.04   2007-06-21  Hp      ESCAN00021164    Compile-Error when activating variable DLC
 *  03.00.00   2008-02-08  Hp      ESCAN00024337    Adapt configuration switches and function names to match
 *                                                  naming rules. FTP_CFG.H now generated by Geny.
 *  03.00.01   2008-02-28  Hp      ESCAN00024955    Add optionally support to map TP-functions into RAM using memmap
 *  03.01.00   2008-03-12  Rr      ESCAN00025302    Removed pragma for V850 with NEC compiler
 *                                 ESCAN00027212    Removed CanCancelTransmit() call
 *  03.02.00   2008-06-05  QPs     ESCAN00027374    Adapted switch name to match name generated by Geny
 *                         Ci      ESCAN00027409    Added support for configurations with multiple CAN channels
 *  03.02.01   2008-06-24  MVi     ESCAN00027820    Replaced an if with an ifdef
 *  03.03.00   2008-11-18  AWh     ESCAN00031393    Allow configurations with both FblTp and tpmc, allow
 *                                                  alternative txDataBuffer and rxDataBuffer configurations.
 *  03.04.00   2009-05-18  Ci      ESCAN00032418    Removed obsolete code
 *                         Ls      ESCAN00035170    No changes
 *                         QPs     ESCAN00035243    Support for variable blocksize and STmin
 *  03.05.00   2009-08-21  Hp      ESCAN00037079    Call Confirmation within FblTpTransmitSF after transmission request
 *                                                  Remove CAN calls if COM wrapper is used.
 *  03.05.01   2009-09-10  Hp      ESCAN00037122    Call to CanTxTask removed due to assertion in CANbedded CAN driver
 *  03.06.00   2009-12-09  TBe     ESCAN00039383    Adaptions for +strict option in Cosmic compiler
 *  03.07.00   2010-02-16  AWh     ESCAN00040879    Adaptions to reduce compiler warnings, changed include order
 *                                                  for string.h/stddef.h, readded kTpTxHandle compatibility macro
 *  03.08.00   2010-10-14  Ci      ESCAN00044300    No changes (see header file)
 *                         Hp      ESCAN00045954    TxTimer isn't reset after successfull send repetition
 *  03.09.00   2012-01-23  AWh     ESCAN00054601    Added configuration to allow dynamic
 *                                                  switch normal/extended addressing
 *  03.10.00   2012-05-25  Ach     ESCAN00057426    STmin = 255 can lead to blocked reception of physical frames
 *                         CB      ESCAN00059071    Added interrupt lock around critical sections.
 *  03.11.00   2012-07-06  CB      ESCAN00059951    Modified Interrupt lock in FblTpTask
 *  03.12.00   2013-04-19  ASe     ESCAN00064581    Adapted comments to use Doxygen
 *                         Hp      ESCAN00066786    TpRx- and TpTx states splitted and simplified, separate var. for rxSN
 *                                 ESCAN00066792    Add support for queued requests
 *  03.12.01   2013-07-25  Hp      ESCAN00068861    Remove compile check INTERRUPT_CONFIRMATION with CAN_CONFIRMATION 
 *                                                  for CBD-Driver usage
 *                                 ESCAN00069345    Separate FC-timer for FC.Wait transmission
 *                         
 *  NOTE: This version has been adapted and must be used with Flash-Bootloader ONLY!
 **********************************************************************************************************************/
#if defined( FBL_TP_ENABLE_INTERNAL_MEMCPY )
#else
# include <string.h>
# include <stddef.h>
#endif
#include "fbl_inc.h"

/* fbl_diag.h included for function prototypes */
#include "fbl_diag.h"
#include "fbl_tp.h"

/***********************************************************************************************************************
 *  VERSION CHECK
 **********************************************************************************************************************/

#if ( FBLTP_ISO_VERSION != 0x0312 )
# error "FBL_TP.C and FBL_TP.H used in different versions, please check!"
#endif
#if ( FBLTP_ISO_RELEASE_VERSION != 0x01 )
# error "FBL_TP.C and FBL_TP.H used in different versions, please check!"
#endif

#if ( FBLTP_ISO_VERSION != _FBLTP_VERSION )
# error "Error in FBL_TP.C: Source and V_VER.H file are inconsistent!"
#endif
#if ( FBLTP_ISO_RELEASE_VERSION != _FBLTP_RELEASE_VERSION )
# error "Error in FBL_TP.C: Source and V_VER.H file are inconsistent!"
#endif

/* PRQA S 3453 FblTp_3453 */ /* MD_CBD_19.7 */
/* PRQA S 3458 FblTp_3458 */ /* MD_CBD_19.4 */

/***********************************************************************************************************************
 *  TRANSPORT LAYER VERSION
 **********************************************************************************************************************/

/* OSEK_TRANSPORT_LAYER_VERSION is defined within the tp.h file */
MEMORY_ROM vuint8 kFblTpMainVersion   = (vuint8)((FBLTP_ISO_VERSION >> 8)  & 0xFFu);
MEMORY_ROM vuint8 kFblTpSubVersion    = (vuint8)(FBLTP_ISO_VERSION & 0xFFu);
MEMORY_ROM vuint8 kFblTpBugFixVersion = FBLTP_ISO_RELEASE_VERSION;

#if !defined( FBLDRVCAN_REFERENCE_IMPLEMENTATION )
# define FBLDRVCAN_REFERENCE_IMPLEMENTATION  0x0100
#endif

/***********************************************************************************************************************
 *  EXTERNAL DEFINE NAME ADAPTATION
 **********************************************************************************************************************/

#if defined( MULTIPLE_RECEIVE_BUFFER )  
  /* Internal check usage only */
# define FBLTP_PROCESSOR_OK
# define kCopyNoData             kCanNoCopyData
#  if (FBLDRVCAN_REFERENCE_IMPLEMENTATION >= 0x0101)
#   define kFblTxOk              kFblCanTxOk
#  else
#   define kFblTxOk              kCanTxOk
#  endif
#endif

#if !defined( FBLTP_PROCESSOR_OK )
# error "Used processor/compiler is not specified correct for usage"
  /* The processor (and in some cases the compiler, too) must be specified within the generated file for your ECU.
   * See the define checks above for supported processor and compiler types. The define is set via the generation tool
   * automatically, so in case of the 'normal way' you should not see this error! */
#endif

/***********************************************************************************************************************
 *  TRANSPORT PROTOCOL TPCI CODES AND PROTOCOL LENGTHS
 **********************************************************************************************************************/

#define kL4_SingleFrame                0x00u
#define kL4_FirstFrame                 0x10u
#define kL4_ConsecutiveFrame           0x20u
#define kL4_FlowControl                0x30u
#define kL4_FlowStatus_Overrun         0x32u
#define kL4_ErrorFrame                 0x40u

#define kL4_FrameTypeMask              0xF0u

#define kL4_SNMask                     0x0Fu
#define kL4_MaxSN                      0x0Fu
#define kL4_BSMask                     0x0Fu
#define kL4_SFData                     0x0Fu
#define kL4_FFData                     0x0Fu

#define kCanFrameLength                8u        /**< Total length of a CAN frame */

#define kFlowStateCTS                  0x00u     /* CTS=Clear to send */
#define kFlowStateWait                 0x01u
#define kFlowStateOverrun              0x02u

#define kWaitForFFConfInterrupt        0x10u     /**< First frame */
#define kWaitForCFConfInterrupt        0x20u     /**< Consecutive frame */
#define kWaitForLastCFConfInterrupt    0x30u     /**< Very last consecutive frame */
#define kWaitForFCConfInterrupt        0x40u     /**< Flow control */
#define kWaitForSFConfInterrupt        0x50u     /**< Single frame */
#define kWaitForFCOverrunConfInterrupt 0x60u     /**< Overrun frame */
#define kWaitForFCWaitFrameConfInterrupt 0x70u   /**< Wait frame transmission (currently not supported) */
#define kWaitForFCQueuedReqWaitFrameConfInterrupt 0x80u  /**< Waif frame transmission for queued requests */

#define kConfInterruptOccured          0x01u

/***********************************************************************************************************************
 *  TRANSPORT PROTOCOL STATE ACCESS
 **********************************************************************************************************************/

/** Enum for bTpRxState */
typedef enum
{
   kTpRxIdle = 0x00u,
   kTpRxWaitCF = 0x01u,
   kTpRxBlocked = 0x10u
} tTpRxState;

/** Tx states */
typedef enum
{
   kTpTxIdle,
   kTpTxWaitFC,
   kTpTxWaitForTxCF,
   kTpTxRepeatTransmit
} tTpTxState;

/***********************************************************************************************************************
 *  SPECIAL FEATURES SUPPORT
 **********************************************************************************************************************/

/* Data length optimization for lower CPU's */
#if defined( C_CPUTYPE_8BIT )
# if ((kFblTpTimeoutCF / TpCallCycle) > 0x00FF)
typedef vuint16 tTpRxTimer;
# else
typedef vuint8 tTpRxTimer;
# endif
# if ((kFblTpTimeoutFC / TpCallCycle) > 0x00FF)
typedef vuint16 tTpTxTimer;
# else
typedef vuint8 tTpTxTimer;
# endif
# if ((kFblTpConfInterruptTimeout / TpCallCycle) > 0x00FF)
typedef vuint16 tTpConfTimer;
# else
typedef vuint8 tTpConfTimer;
# endif
#else
typedef vuint16 tTpRxTimer;
typedef vuint16 tTpTxTimer;
typedef vuint16 tTpConfTimer;
#endif

/***********************************************************************************************************************
 *  TP DATA ACCESS
 **********************************************************************************************************************/

#if defined( FBL_TP_ENABLE_EXTENDED_ADDRESSING )
# define kTargetPos           (0u)
#  define kTargetByteOffset   (1u)
#else
# define kTargetByteOffset    (0u)
#endif /* ! FBL_TP_ENABLE_EXTENDED_ADDRESSING */

#define kTpciPos              (0u + kTargetByteOffset)
#define kBsPos                (1u + kTargetByteOffset)
#define kSTminPos             (2u + kTargetByteOffset)
#define kLengthPos            (1u + kTargetByteOffset)
#define kSFDataPos            (1u + kTargetByteOffset)
#define kFFDataPos            (2u + kTargetByteOffset)
#define kCFDataPos            (1u + kTargetByteOffset)

#if defined( MULTIPLE_RECEIVE_BUFFER )
# if defined( FBL_TP_ENABLE_EXTENDED_ADDRESSING )
#  define TARGET_Byte         (data[0u])
# endif
# define TPCI_Byte            (data[kTpciPos])
# define DL_Byte              (data[kLengthPos])
# define STmin_Byte           (data[kSTminPos])
# define FirstDataSFCF        (&data[kSFDataPos])
# define FirstDataFF          (&data[kFFDataPos])
#endif /* MULTIPLE_RECEIVE_BUFFER */

/* Start value for sequence number counter */
# define kTpSNStartValue      1u

/***********************************************************************************************************************
 *  GLOBAL TRANSPORT LAYER DATA
 **********************************************************************************************************************/

/**
 *  \description Index to receive data in RX flat data buffer model. For RX only the flat data model is supported.
 *  \note        Value range / coding: 0..255
 */
tTpDataType volatile gbTpRxDL;

/**
 *  \description Index to transmit data in TX flat data buffer model. For TX the model of transmit buffer could be
 *               chosen between flat (linear) and cyclic buffer handling.
 *  \note        Value range / coding: 0..255
 */
tTpDataType volatile gbTpTxDL;

/**
 *  \description Length of data to be received. The size of this variable limits the maximum size of messages.
 *  \note        Value range / coding: 0..255
 */
tTpDataType volatile gbTpRxLength;

/**
 *  \description Internal Tx states are stored here. This variable should be stored in bit addressed memory areas for fast
 *               and easy bit access. MEMORY_NEAR is defined within the included generated header v_def.h.
 *
 *  \verbatim
 *   Usage of variable vuint8 bTpTxState:
 *   7654 3210
 *   |||| ++++ unused
 *   ||++ Internal TX state: 0x00 Idle,
 *   ||                      0x10 Wait for first FC,
 *   ||                      0x20 Wait for TX of CF
 *   ||                      0x30 Wait resent last message
 *   ++ Unused
 *  \endverbatim
 *
 *  \attention   There are some typecasts to avoid compiler warnings. These casts have to be adapted if the type of
 *               bTpTxState is changed.
 *  \note        Value range / coding: 0..255
 */
static MEMORY_NEAR tTpTxState volatile bTpTxState;

/**
 *  \description Internal Rx states are stored here. This variable should be stored in bit addressed memory areas for fast
 *               and easy bit access. MEMORY_NEAR is defined within the included generated header v_def.h.
 *
 *  \verbatim
 *   Usage of variable vuint8 bTpRxState:
 *   7654 3210
     |||| |||+ Internal RX state                           0x00 Idle
     |||| |||                                              0x01 kTpRxWaitCF
     |||+---- Lock for single buffer usage (e.g. diagnose) 0x00 kTpRxFree
 *   |||  |||                                              0x10 kTpRxBlocked
 *   +++  +++  unused
 *  \endverbatim
 *
 *  \attention   There are some typecasts to avoid compiler warnings. These casts have to be adapted if the type of
 *               bTpRxState is changed.
 *  \note        Value range / coding: 0..03
 */
static MEMORY_NEAR tTpRxState volatile bTpRxState;

/**
 *  \description The block size (BS) runs from 0 to 15, so the requested BS is stored into the high nibble and the
 *               actual counter value is represented within the low nibble.
 *               Rx means: Send in own FC. The application can change the block size via an access macro.
 *
 *  \verbatim
 *   Usage of variable vuint8 bRxBlockSize:
 *   7654 3210
 *   |||| ++++BlockSize counter for rx usage, range from 0x1 to 0xF (variable)
 *   ||||     (This is a nibble counter from own BS down to 0)
 *   ++++ BlockSize BS start value for rx usage, range 0x0 to 0xF (fix)
 *  \endverbatim
 *
 *  \note        Value range / coding: 0..255
 */
MEMORY_NEAR volatile vuint8 bRxBlockSize;

/**
 *  \description Due to the block size that runs from 0 to 255 the block size value and block size counter cannot
 *               be stored in a single byte.
 *
 *  \verbatim
 *   Usage of variable vuint8 bRxBSCounter
 *   7654 3210
 *   ++++ ++++ BlockSize counter, range from 0x01 to 0xFF
 *  \endverbatim
 *
 *  \note        Value range / coding: 0..255
 */
MEMORY_NEAR volatile vuint8 bRxBSCounter;

/**
 *  \description The requested or default STmin value is stored in the variable bRxSTmin.
 *               RX means: Send in own FC.
 *               STmin via an access macro.
 *  \note        Value range / coding: 0..255
 */
vuint8 bRxSTmin;

#if defined( FBL_TP_ENABLE_EXTENDED_ADDRESSING )
/**
 *  \description For the OSEK extended addressing mode, the target and source address have to be stored in RAM for
 *               being changed by the application. To support this, the additional functions
 *               FblTpSetTargetAddress and FblTpGetSourceAddress are provided.
 *  \note        Value range / coding: 0..255
 */
vuint8 gbTxTargetAddress;
vuint8 gbRxSourceAddress;
#endif

/***********************************************************************************************************************
 *  LOCAL TRANSPORT LAYER DATA
 **********************************************************************************************************************/

/**
 *  \description The segment counter is calculated at start of transmit and counts down to zero. Attention: SN is
 *               stored within and used from low nibble of can transmit buffer! Do not overwrite can transmit
*                buffer by other functions!
 *  \note        Value range / coding: 0..255
 */
static MEMORY_NEAR volatile tTpDataType bTxSEG;

#if defined( FBL_TP_ENABLE_ONLY_FIRST_FC )
/**
 *  \description The block size (BS) runs from 0 to 15, so the requested BS is stored into the high nibble and the
 *               actual counter value is represented within the low nibble.
 *               TX means: extracted from received FC and used for transmission of data
 *
 *  \verbatim
 *   Usage of variable vuint8 bTxBlockSize:
 *   7654 3210
 *   |||| ++++BlockSize counter for TX usage, range from 0x1 to 0xF (variable)
 *   ||||     (This is a nibble counter from counterpart BS down to 0)
 *   ++++ BlockSize BS start value for TX usage, range 0x0 to 0xF (fix)
 *  \endverbatim
 *
 *  \note        Value range / coding: 0..255
 */
static MEMORY_NEAR volatile vuint8 bTxBlockSize;
#endif

/**
 *  \description Due to the block size that runs from 0 to 255 the block size value and block size counter cannot
 *               be stored in a single byte.
 *
 *  \verbatim
 *   Usage of variable vuint8 bRxBSCounter
 *   7654 3210
 *   ++++ ++++ BlockSize counter, range from 0x01 to 0xFF
 *  \endverbatim
 *
 *  \note        Value range / coding: 0..255
 */
static MEMORY_NEAR volatile vuint8 bTxBSCounter;

/**
 *  \description bTxSN is a modulo 15 counter for the sequence number to be transmitted in consecutive frames.
 *  \note        Value range / coding: 0..255 (internally) , 0..15 (externally)
 */
static volatile vuint8 bTxSN;

/**
 *  \description bRxSN is a modulo 15 counter for the sequence number to be received in consecutive frames.
 *  \note        Value range / coding: 0..255 (internally) , 0..15 (externally)
 */
static volatile vuint8 bRxSN;

/**
 *  \description The timer runs until the timer value is zero. If it changes from 1 to 0 the specified reaction
 *               (e.g. timeout) have to be done.
 *               The maximum possible time depends on the call cycle and the timer size in bits. (e.g. 10ms call cycle,
 *               8 bit is resulting in 255 * 10ms = 2550 ms timer runtime)
 *  \note        Value range / coding: 0..255
 */
static volatile tTpRxTimer bRxTimer;

/**
 *  \description The timer runs until the timer value is zero. If it changes from 1 to 0 the specified reaction
 *               (e.g. timeout) have to be done.
 *               The maximum possible time depends on the call cycle and the timer size in bits. (e.g. 10ms call cycle,
 *               8 bit is resulting in 255 * 10ms = 2550 ms timer runtime)
 *  \note        Value range / coding: 0..255
 */
static volatile tTpTxTimer bTxTimer;

/**
 *  \description Global value to store the requested minimum separation time received in the flow control frames.
 *               The received STmin value will be adjusted to multiples of TpCallCycle.
 *  \note        Value range / coding: 0..255 / in ms
 */
static vuint8  STmin;

#if defined( FBL_TP_ENABLE_TX_FRAME_PADDING ) || \
    defined( FBL_TP_ENABLE_VARIABLE_TX_DLC )
/**
 *  \description Value to store the number of bytes at the end of a frame that have to be padded with zeros.
 *  \note        Value range / coding: 0..kCF_DataLength - 1
 */
static vuint8 bPaddingLength;
#endif

/**
 *  \brief       Additional flags for different purposes
 *
 *  \verbatim
 *  7654 3210
 *          +- FC is already evaluated
 *         +-- Signal flag for interrupt occurred, that set RxTimerValue
 *  ++++ ++--- not used
 *  \endverbatim
 *
 * \attention    There are some typecasts to avoid compiler warnings. These casts have to be adapted if the type of
 *               bStateFlags is changed.
 */
static vuint8 bStateFlags;

#if defined( FBL_TP_ENABLE_VARIABLE_TX_DLC )
/** Storage of current DLC */
static vuint8 tpTxDLC;

#endif

#if defined( FBL_TP_ENABLE_CONFIRMATION_INTERRUPT )
static MEMORY_NEAR vuint8 gTpConfState;         /**< State variable for confirmation interrupt handling */
static MEMORY_NEAR tTpConfTimer gTpConfTimer;   /**< Counter variable for confirmation timeout observation */
#endif /* FBL_TP_ENABLE_CONFIRMATION_INTERRUPT */

/***********************************************************************************************************************
 *  TRANSPORT LAYER VARIABLE ASSIGNMENT
 **********************************************************************************************************************/

#define kDefaultBS         8u

# define txDataBuffer      DiagBuffer
# define rxDataBuffer      DiagBuffer

#define txDataIndex        gbTpTxDL
#define txSEG              bTxSEG
#define txSN               bTxSN

#define rxDataIndex        gbTpRxDL
#define rxDataLength       gbTpRxLength
#define rxSN               bRxSN

/***********************************************************************************************************************
 *  TRANSPORT PROTOCOL TPCI CODES AND PROTOCOL LENGHTS
 **********************************************************************************************************************/

#define kSF_DataLength     (7u - kTargetByteOffset)
#define kFF_DataLength     (6u - kTargetByteOffset)
#define kFC_DataLength     (3u)
#define kCF_DataLength     (7u - kTargetByteOffset)

#define kConfInterruptTimeoutCounter         (kFblTpConfInterruptTimeout / TpCallCycle)

#if defined( FBL_TP_ENABLE_CONFIRMATION_INTERRUPT )

/* Set timeout timer as well */
# define SetWaitForFFConfInterrupt()         { \
                                                gTpConfState = kWaitForFFConfInterrupt; \
                                                gTpConfTimer = kConfInterruptTimeoutCounter; \
                                             }
# define SetWaitForCFConfInterrupt()         { \
                                                gTpConfState = kWaitForCFConfInterrupt; \
                                                gTpConfTimer = kConfInterruptTimeoutCounter; \
                                             }
# define SetWaitForLastCFConfInterrupt()     { \
                                                gTpConfState = kWaitForLastCFConfInterrupt; \
                                                gTpConfTimer = kConfInterruptTimeoutCounter; \
                                             }
# define SetWaitForFCConfInterrupt()         { \
                                                gTpConfState = kWaitForFCConfInterrupt; \
                                                gTpConfTimer = kConfInterruptTimeoutCounter; \
                                             }
# define SetWaitForFCOverrunConfInterrupt()  { \
                                                gTpConfState = kWaitForFCOverrunConfInterrupt; \
                                                gTpConfTimer = kConfInterruptTimeoutCounter; \
                                             }

# define SetWaitForSFConfInterrupt()         { \
                                                gTpConfState = kWaitForSFConfInterrupt; \
                                                gTpConfTimer = kConfInterruptTimeoutCounter; \
                                             }

# define SetWaitForQueuedReqWaitFrameConfInterrupt() \
                                             { \
                                                gTpConfState = kWaitForFCQueuedReqWaitFrameConfInterrupt; \
                                                gTpConfTimer = kConfInterruptTimeoutCounter; \
                                             }

# define IsWaitingForConfInterrupt()         (0u != gTpConfState)
# define GetConfInterruptState()             gTpConfState

# define SetConfInterruptTimeout()           (gTpConfTimer = kConfInterruptTimeoutCounter)
# define GetConfInterruptTimer()             gTpConfTimer

# define ResetConfState()                    (gTpConfState = 0u)
# define InitConfState()                     {gTpConfState = 0u; gTpConfTimer = 0u;}

#endif /* FBL_TP_ENABLE_CONFIRMATION_INTERRUPT */

/***********************************************************************************************************************
 *  TRANSPORT PROTOCOL LOCALLY USED TIMES
 **********************************************************************************************************************/

#define kTimeoutCF         (kFblTpTimeoutCF / TpCallCycle)  /**< Timer value in counter units */
#define kTimeoutFC         (kFblTpTimeoutFC / TpCallCycle)  /**< Timer value in counter units */

#if defined(kFblTpTransmitFCWait)
#define kTransmitFCWait    (kFblTpTransmitFCWait / TpCallCycle)  /**< Retransmission cycle of FC.Wait frames */
#else
#define kTransmitFCWait    (((kFblTpTimeoutFC*8)/10) / TpCallCycle)  /**< Retransmission cycle of FC.Wait frames is 80% of FC.timeout (N_Br/N_Bs) */
#endif

/** Timer value in counter units */
#define kTransmitCF        (((kFblTpTransmitCF > TpCallCycle) ? kFblTpTransmitCF : TpCallCycle) / TpCallCycle)

#define kTimeoutFCDiagFF   (kTimeoutFC + 1u)                /* Timer value in counter units */

#define kTimerOff          0u

/***********************************************************************************************************************
 *  TRANSPORT PROTOCOL TIMER ACCESS
 **********************************************************************************************************************/

#define StartRxTimeoutCF(newVal, newState)   { \
                                                bRxTimer   = (newVal); \
                                                bTpRxState = (newState); \
                                             }
#define StartTxWaitForTxCF(newVal, newState) { \
                                                bTxTimer   = (newVal); \
                                                bTpTxState = (newState); \
                                             }
#define StartTxTimeoutFC(newVal, newState)   { \
                                                bTxTimer   = (newVal); \
                                                bTpTxState = (newState); \
                                             }

#define StopTxTimer()                        (bTxTimer = kTimerOff)
#define StopRxTimer()                        (bRxTimer = kTimerOff)
#define TxTimerValue()                       bTxTimer
#define DecTxTimerValue()                    (bTxTimer--)
#define SetTxTimerValue(t)                   (bTxTimer = (t))
#define RxTimerValue()                       bRxTimer
#define DecRxTimerValue()                    (bRxTimer--)
#define SetRxTimerValue(t)                   (bRxTimer = (t))

/***********************************************************************************************************************
 *  TRANSPORT PROTOCOL STATE ACCESS
 **********************************************************************************************************************/

#define txGetState()          bTpTxState
#define txSetState(newS)      (bTpTxState = (newS))

#define rxGetState()          bTpRxState
#define rxSetState(newS)      (bTpRxState = (newS))

#define rxGetSN()             (rxSN & kL4_SNMask)
#define rxSetSN(newSN)        (rxSN = (newSN))

#define rxIncSN()             (rxSN++)
#define rxResetSN()           (rxSN = 0u)

#if defined( FBL_TP_ENABLE_ONLY_FIRST_FC )
# define FCEvaFlag            0x01u
# define txSetFCEvaFlag()     (bStateFlags |= FCEvaFlag)
# define txGetFCEvaFlag()     (bStateFlags &  FCEvaFlag)
# define txClearFCEvaFlag()   (bStateFlags &= FblInvert8Bit(FCEvaFlag))
#endif

#define TpRxCFFlag            0x02u
#define SetRxCFFlag()         (bStateFlags |= TpRxCFFlag)
#define GetRxCFFlag()         (bStateFlags &  TpRxCFFlag)
#define ClearRxCFFlag()       (bStateFlags &= FblInvert8Bit(TpRxCFFlag))

#if defined( FBL_TP_ENABLE_ACCEPT_TOO_LARGE_DATA )
# define TpRxBlockTooLargeFlag         0x04u
# define SetRxBlockTooLargeFlag()      (bStateFlags |= TpRxBlockTooLargeFlag)
# define GetRxBlockTooLargeFlag()      (bStateFlags &  TpRxBlockTooLargeFlag)
# define ClearRxBlockTooLargeFlag()    (bStateFlags &= FblInvert8Bit(TpRxBlockTooLargeFlag))
#endif

#define ResetStateFlags()     (bStateFlags = 0u)

#if defined( FBL_TP_ENABLE_ACCEPT_TOO_LARGE_DATA )
# define ResetRxStateFlags()  (bStateFlags &= FblInvert8Bit(TpRxCFFlag | TpRxBlockTooLargeFlag))
#else
# define ResetRxStateFlags()  (bStateFlags &= FblInvert8Bit(TpRxCFFlag))
#endif

/***********************************************************************************************************************
 *  TRANSPORT PROTOCOL BLOCKSIZE ACCESS
 **********************************************************************************************************************/

/* Macros for block size handling with maximum BS-value of 255 */
#if defined( FBL_TP_ENABLE_ONLY_FIRST_FC )
# define txSetBS(newBS)    {bTxBlockSize = (newBS); bTxBSCounter = (newBS);} /* Reset counter-byte also */
# define txReloadBSCnt()   (bTxBSCounter = bTxBlockSize)
#else
# define txSetBS(newBS)    (bTxBSCounter = (newBS))
#endif
#define txDecBSCnt()       (bTxBSCounter--)                 /* Call only if BS>0 */
#define txCheckBSCnt()     (bTxBSCounter)
#define txCheckBSZero()    (bTxBSCounter)
#define txClearBS()        (bTxBSCounter = 0u)

#define rxReloadBSCnt()    (bRxBSCounter = (bRxBlockSize))  /* BS must be counted to 0 */
#define rxDecBSCnt()       (bRxBSCounter--)                 /* Call only if BS > 0 */
#define rxCheckBSCnt()     (bRxBSCounter)
#define rxCheckBSZero()    (bRxBSCounter)

/***********************************************************************************************************************
 *  TRANSPORT PROTOCOL EXTENDED ADDRESSING SET MACROS
 **********************************************************************************************************************/

# if defined( FBL_TP_ENABLE_EXTENDED_ADDRESSING )
#  define SetTargetAddrForTx(addr)  (tpCanTxData[kTargetPos] = (addr))
# endif

/***********************************************************************************************************************
 *  CHECK FOR POSSIBLE COMPILE AND DEFINITION ERRORS
 **********************************************************************************************************************/

#if ( (kFblTpSTMin / TpCallCycle) > 0x00FF )
# error "Time kFblTpSTMin too large!"
#endif

#if ( ((kFblTpTimeoutFC / TpCallCycle) * TpCallCycle) != kFblTpTimeoutFC )
# error "kFblTpTimeoutFC must be a integer multiple of TpCallCycle"
#endif

#if ( ((kFblTpTimeoutCF / TpCallCycle) * TpCallCycle) != kFblTpTimeoutCF )
# error "TpTimeoutCF must be a integer multiple of TpCallCycle"
#endif

#if ( ((kFblTpTransmitCF / TpCallCycle) * TpCallCycle) != kFblTpTransmitCF )
# error "kFblTpTransmitCF must be a integer multiple of TpCallCycle"
#endif

#if defined( FBL_TP_DISABLE_OSEK_DATA_LENGTH ) && \
    defined( C_CPUTYPE_8BIT )
# if ( kFblTpBufferSize > 0xFF )
#  error "Buffer for transport layer is larger than 255 bytes"
# endif
#endif

#if ( kFblTpBufferSize > 0x0FFF )
# error "Buffer for transport layer is larger than 4095 bytes"
#endif

/* PRQA L:FblTp_3453 */
/* PRQA L:FblTp_3458 */

/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
static void AssembleFC(void);
static vuint8 CAN_SaveTransmit(void);
static void TxConfirm(vuint8 state);
static void FblTpInit(void);

/***********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/

/* PRQA S 0277 FblTp_0277 */ /* MD_FblTp_0277 */

/***********************************************************************************************************************
 *  AssembleFC
 **********************************************************************************************************************/
/*! \brief       Assemble an FlowControl Frame
 **********************************************************************************************************************/
static void AssembleFC(void)
{
#if defined( FBL_TP_ENABLE_EXTENDED_ADDRESSING )
   SetTargetAddrForTx(gbRxSourceAddress);
#endif

   tpCanTxData[kTpciPos] = kL4_FlowControl;

   tpCanTxData[kBsPos] = (vuint8)FblTpGetRxBS();

   tpCanTxData[kSTminPos] = (vuint8)FblTpGetRxSTmin();

#if defined( FBL_TP_ENABLE_VARIABLE_TX_DLC )
   tpTxDLC = (vuint8)(kFC_DataLength + kTpciPos);
#else
   {
      vuintx i;
      /* Just for a fine look of the flow control message... */
      for (i = (kFC_DataLength + kTpciPos); i < kCanFrameLength; i++)
      {
         tpCanTxData[i] = kFblTpFillPattern;
      }
   }
#endif

}

/***********************************************************************************************************************
 *  CAN_SaveTransmit
 **********************************************************************************************************************/
/*! \brief       Error proofed CAN_SaveTransmit, extra function used to save ROM
 *  \return      kTpSuccess if succeeded, otherwise kTpFailed
 **********************************************************************************************************************/
static vuint8 CAN_SaveTransmit(void)
{
   vuint8 rval;

# if defined( FBL_TP_ENABLE_VARIABLE_TX_DLC )
   /* Set DLC value for next tp message */
   FblCanSetTxDLC(fblCanTxObj[kTpTxObj], tpTxDLC);
# endif
   if (FblCanTransmit(kFblTpTxHandle) != kFblTxOk)
   {
      txSetState(kTpTxRepeatTransmit);
#if defined( FBL_TP_ENABLE_CONFIRMATION_INTERRUPT )
#else
      /* If transmission fails, make sure the TxTimer is active to limit the number of retries */
      if (TxTimerValue() == 0u)
      {
         /* Do not overwrite the timer value in case it has already been started */
         SetTxTimerValue(kConfInterruptTimeoutCounter);
      }
#endif
      rval = kTpFailed;
   }
   else
   {
#if defined( FBL_TP_ENABLE_CONFIRMATION_INTERRUPT )
#else
      __ApplFblTpCanMessageTransmitted(); /* Used for e.g. reseting of application counters */
#endif
#if defined( FBL_TP_ENABLE_EXTENDED_ADDRESSING )
      SetTargetAddrForTx(gbTxTargetAddress);
#endif
      rval = kTpSuccess;
   }

   return rval;
}

/***********************************************************************************************************************
 *  TxConfirm
 **********************************************************************************************************************/
/*! \brief       Conclusion of a message transmission
 *  \param[in]   state either a kTpBusy or kTpSuccess.
 **********************************************************************************************************************/
static void TxConfirm(vuint8 state)
{
   __ApplFblTpConfirm(state);

}

/***********************************************************************************************************************
 *  FblTpInit
 **********************************************************************************************************************/
/*! \brief       Initialization function
 **********************************************************************************************************************/
static void FblTpInit(void)
{
   txSetBS(kDefaultBS); /* PRQA S 3109 */ /* MD_FblTp_3109 */
   rxSetSN(1u);

   rxSetState(kTpRxIdle);
   txSetState(kTpTxIdle);
   StopTxTimer();
   StopRxTimer();
   ResetStateFlags();

   txSEG = 0u;

   STmin = kTransmitCF;

   txDataIndex = 0u;
   rxDataIndex = 0u;

#if defined( FBL_TP_ENABLE_CONFIRMATION_INTERRUPT )
   InitConfState(); /* PRQA S 3109 */ /* MD_FblTp_3109 */
#endif
#if defined( FBL_TP_ENABLE_VARIABLE_TX_DLC )
   tpTxDLC = 0x0u;
#endif
}

/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  FblTpResetRxBlock
 **********************************************************************************************************************/
/*! \brief       Release lock state of internal buffer
 **********************************************************************************************************************/
void FblTpResetRxBlock(void)
{
   rxSetState(kTpRxIdle);
}

/***********************************************************************************************************************
 *  FblTpSetRxBlock
 **********************************************************************************************************************/
/*! \brief       Set lock state of internal buffer
 **********************************************************************************************************************/
void FblTpSetRxBlock(void)
{
   rxSetState(kTpRxBlocked);
}

/***********************************************************************************************************************
 *  FblTpInitPowerOn
 **********************************************************************************************************************/
/*! \brief       Initialization function
 **********************************************************************************************************************/
void FblTpInitPowerOn(void)
{
   /* Init BS with preconfigured value */
   FblTpSetRxBS(kFblTpBlocksize); /* PRQA S 3109 */ /* MD_FblTp_3109 */

   /* Init STmin with preconfigured value */
   FblTpSetRxSTmin(kFblTpSTMin);

   FblTpInit();      /* Initialize connection specific parameter */

#if defined( FBL_TP_ENABLE_EXTENDED_ADDRESSING )
   gbTxTargetAddress = 0u;
   gbRxSourceAddress = 0u;
#endif
}

/***********************************************************************************************************************
 *  FblTpTransmit
 **********************************************************************************************************************/
/*! \brief       Transmit data in buffer
 *  \param[in]   count Number of bytes to be transmitted
 *  \return      kTpSuccess if succeeded, otherwise an error code
 **********************************************************************************************************************/
vuint8 FblTpTransmit(tTpDataType count)
{
#if defined( FBL_TP_ENABLE_CONFIRMATION_INTERRUPT )
   if (IsWaitingForConfInterrupt() || (txGetState() != kTpTxIdle)) /* PRQA S 3415 */ /* MD_FblTp_3415 */
#else
   if (txGetState() != kTpTxIdle)
#endif
   {
      __ApplFblTpConfirm(kTpBusy);  /* Transport layer busy, notify application */
      return kTpBusy;           /* Sending not possible while waiting or not idle */
   }

#if defined( FBL_TP_ENABLE_EXTENDED_ADDRESSING )
   SetTargetAddrForTx(gbTxTargetAddress);
#endif

   if (count <= kSF_DataLength)     /* (0)1 to 6/7 bytes will be a SingleFrame */
   {
      txSEG = 0u;                   /* In case of repetition due to CAN-disturbance
                                     * This ensures, that the RX / TX state is re-init */
      tpCanTxData[kTpciPos] = (vuint8)count; /* kL4_SingleFrame TPCI is 0x00 */

#if defined( FBL_TP_ENABLE_INTERNAL_MEMCPY )
      {
         vuintx i;
         for (i = 0u; i < count; i++)
         {
            tpCanTxData[kSFDataPos + i] = txDataBuffer[i];
         }
      }
#else
      __ApplFblTpCopyToCAN(&tpCanTxData[kSFDataPos], &txDataBuffer[0], count);
#endif

#if defined( FBL_TP_ENABLE_VARIABLE_TX_DLC )
      tpTxDLC = (vuint8)(count + kLengthPos);
#else
# if defined( FBL_TP_ENABLE_TX_FRAME_PADDING )
      /* Padding unused data within the frame */
      while ((count + kSFDataPos) < kCanFrameLength)
      {
         tpCanTxData[(count + kSFDataPos)] = kFblTpFillPattern;
         count++;
      }
# endif
#endif

#if defined( FBL_TP_ENABLE_CONFIRMATION_INTERRUPT )
      SetWaitForSFConfInterrupt(); /* PRQA S 3109 */ /* MD_FblTp_3109 */
      (void)CAN_SaveTransmit();
      rxSetState(kTpRxIdle);
#else
      if (CAN_SaveTransmit() == kTpSuccess)
      {
         rxSetState(kTpRxIdle);
         TxConfirm(kTpSuccess);
      }
#endif
   }
   else
   {
      /* Correct calculation means txSEG = (count - 6 + 6) / 7 or (count - 5 + 5) / 6 */
      txSEG = (tTpDataType)(count / kCF_DataLength);
#if defined( FBL_TP_ENABLE_TX_FRAME_PADDING ) || \
     defined( FBL_TP_ENABLE_VARIABLE_TX_DLC )
      bPaddingLength = (vuint8)(((txSEG * kCF_DataLength) - (count - kFF_DataLength)) & 0xffu);
#endif
      txSN = kTpSNStartValue; /* Start value of sequence number counter */
#if defined( FBL_TP_DISABLE_OSEK_DATA_LENGTH ) && \
     defined( C_CPUTYPE_8BIT ) /* Means length <= 255 */
      tpCanTxData[kTpciPos]   = (vuint8)kL4_FirstFrame; /* No length in TPCI byte */
#else /* Means length <= 4095 */
      tpCanTxData[kTpciPos]   = (vuint8)(kL4_FirstFrame + (vuint8)(count >> 8)); /* DL in TPCI byte */
#endif
      tpCanTxData[kLengthPos] = (vuint8)count;

#if defined( FBL_TP_ENABLE_INTERNAL_MEMCPY )
      for (txDataIndex = 0u; txDataIndex < kFF_DataLength; txDataIndex++)
      {
         tpCanTxData[kFFDataPos + txDataIndex] = txDataBuffer[txDataIndex];
      }
#else
      __ApplFblTpCopyToCAN(&tpCanTxData[kFFDataPos], &txDataBuffer[0], kFF_DataLength);
      txDataIndex = kFF_DataLength; /* First bytes are transmitted */
#endif

      txClearBS(); /* Necessary for correct setting of next state kTpTxWaitFC */
      /* For diagnostic usage, the first timeout loop have to be increased by one loop */
      StartTxTimeoutFC(kTimeoutFCDiagFF, kTpTxWaitFC); /* PRQA S 3109 */ /* MD_FblTp_3109 */
      rxSetState(kTpRxBlocked);      /* Ensure the buffer is locked (normally already by previous request) */
#if defined( FBL_TP_ENABLE_CONFIRMATION_INTERRUPT )
      SetWaitForFFConfInterrupt(); /* PRQA S 3109 */ /* MD_FblTp_3109 */
#else
      __ApplFblTpNotifyTx(kFF_DataLength);
#endif/* Use confirmation interrupt */
#if defined( FBL_TP_ENABLE_VARIABLE_TX_DLC )
      tpTxDLC = (vuint8)8u;
#endif
      (void)CAN_SaveTransmit();
   }

   /* !!!NOTE: 'count' is destroyed here depending on the configuration */
   /* ================================================================= */

#if defined( FBL_TP_ENABLE_ONLY_FIRST_FC )
   txClearFCEvaFlag();
#endif

   return kTpSuccess;
} /* PRQA S 2006 */ /* MD_FblTp_2006 */

/***********************************************************************************************************************
 *  FblTpTransmitSF
 **********************************************************************************************************************/
/*! \brief       Transmit data which is already stored in CAN driver buffer for special usage within diagnostic
 *               module (negative acknowledge codes)
 *  \param[in]   count Number of bytes to be transmitted
 *  \return      kTpSuccess if succeeded, otherwise an error code
 **********************************************************************************************************************/
vuint8 FblTpTransmitSF(vuint8 count)
{
#if defined( FBL_TP_ENABLE_CONFIRMATION_INTERRUPT )
   if (IsWaitingForConfInterrupt() || (txGetState() != kTpTxIdle)) /* PRQA S 3415 */ /* MD_FblTp_3415 */
#else
   if (txGetState() != kTpTxIdle)
#endif
   {
      __ApplFblTpConfirm(kTpBusy);  /* Transport layer busy, notify application */
      return kTpBusy;               /* Sending not possible while waiting or not idle */
   }

   if (count > kSF_DataLength)
   {
      __ApplFblTpConfirm(kTpBusy);  /* Transport layer busy, notify application */
      return kTpBusy;               /* Sending not possible while waiting or not idle */
   }

   tpCanTxData[kTpciPos] = count; /* kL4_SingleFrame is 0x00 */
#if defined( FBL_TP_ENABLE_CONFIRMATION_INTERRUPT )
   SetWaitForSFConfInterrupt(); /* PRQA S 3109 */ /* MD_FblTp_3109 */
#endif
#if defined( FBL_TP_ENABLE_EXTENDED_ADDRESSING )
   SetTargetAddrForTx(gbTxTargetAddress);
#endif
#if defined( FBL_TP_ENABLE_VARIABLE_TX_DLC )
   tpTxDLC = (vuint8)(count + kLengthPos);
#endif

# if defined( FBL_TP_ENABLE_VARIABLE_TX_DLC )
   /* Set DLC value for next tp message */
   FblCanSetTxDLC(fblCanTxObj[kTpTxObj], tpTxDLC);
# endif
   if (FblCanTransmit(kFblTpTxHandle) != kFblTxOk)
   {
#if defined( FBL_TP_ENABLE_CONFIRMATION_INTERRUPT )
      InitConfState(); /* PRQA S 3109 */ /* MD_FblTp_3109 */
#endif
      TxConfirm(kTpFailed);

      return kTpFailed;
   }
#if defined( FBL_TP_ENABLE_CONFIRMATION_INTERRUPT )
#else
   TxConfirm(kTpSuccess);
#endif

   return kTpSuccess;
} /* PRQA S 2006 */ /* MD_FblTp_2006 */

/***********************************************************************************************************************
 *  FblTpPrecopy
 **********************************************************************************************************************/
/*! \brief       Receive function for CAN driver
 *  \return      kCopyNoData
 **********************************************************************************************************************/
#if defined( MULTIPLE_RECEIVE_BUFFER )
vuint8 FblTpPrecopy(const pChipDataPtr data)
{
#endif
#if defined( FBL_TP_ENABLE_FIX_RX_DLC_CHECK )
# if defined( FBL_TP_ENABLE_VARIABLE_RX_DLC )
# else
   if (CanRxActualDLC != 8u)
   {
      return kCopyNoData;
   }
# endif
#endif
#if defined( FBL_TP_ENABLE_EXTENDED_ADDRESSING )
   {
# if defined( FBL_TP_ENABLE_CHECK_FOR_FUNCTIONAL_TA )
      if (TARGET_Byte != kFblTpOwnEcuNumber)
      {
         /* This means functional addressing - all other cases are handled above */
         if (! ApplTpCheckFunctionalTA(TARGET_Byte))
         {
            return kCopyNoData;
         }
      }
# else /* ! FBL_TP_ENABLE_CHECK_FOR_FUNCTIONAL_TA */
#  if ( kFblTpHighestFunctionalAddress == 0xFF )
      if ((TARGET_Byte != kFblTpOwnEcuNumber) && (TARGET_Byte < kFblTpLowestFunctionalAddress))
      {
         return kCopyNoData;
      }
#  else
      if ((TARGET_Byte != kFblTpOwnEcuNumber) &&
         ((TARGET_Byte < kFblTpLowestFunctionalAddress) || (TARGET_Byte > kFblTpHighestFunctionalAddress)))
      {
         return kCopyNoData;
      }
#  endif /* TP_HIGHEST_FUNCTIONAL_ADDRESS */
# endif /* ! FBL_TP_ENABLE_CHECK_FOR_FUNCTIONAL_TA */
   }
#endif /* FBL_TP_ENABLE_EXTENDED_ADDRESSING */

#if defined( FBL_TP_ENABLE_CONFIRMATION_INTERRUPT )
   if (IsWaitingForConfInterrupt())
   {
      /* This shall prevent RX-before-TX inversion */
      if (FblCanMsgTransmitted() == kFblTxOk)
      {
         FblTpConfirmation(0u);
      }
      else
      {
         return kCopyNoData; /* Receive nothing while waiting */
      }
   }
#endif

   __ApplFblTpCanMessageReceived(); /* Used for e.g. reseting of application counters */

   switch (TPCI_Byte & kL4_FrameTypeMask)
   {
      /*-----------------------------------------------------------------------------
      | Receive SingleFrame
      -----------------------------------------------------------------------------*/
      case kL4_SingleFrame:
      {
#if defined( FBL_TP_ENABLE_ISO_COMPLIANCE )
#else
# if defined( FBL_TP_ENABLE_CONFIRMATION_INTERRUPT )
         if (IsWaitingForConfInterrupt())
         {
            return kCopyNoData; /* Receive nothing while waiting */
         }
# endif
#endif
         /* Check if the receive buffer is free */
         if (kTpRxBlocked == rxGetState())
         {
            /* Transport Layer blocked, SF not allowed
             * only buffer is blocked - reception not in progress - kill that frame */
            __ApplFblTpErrorIndication((vuint8)kTpErrRxNotIdle); /* Notify application */
            return kCopyNoData;
         }
#if defined( FBL_TP_ENABLE_ISO_COMPLIANCE )
# if defined( FBL_TP_ENABLE_CONFIRMATION_INTERRUPT )
         /* Reset on-going reception when receiving a TP Single-frame. */
         /* Note: Even if invalid! */
         if (IsWaitingForConfInterrupt())
         {
            /* Reception is in progress */
            /* First cancel transmission object */
            /* Free the state machine */
            InitConfState(); /* PRQA S 3109 */ /* MD_FblTp_3109 */
            StopRxTimer();
            rxSetState(kTpRxIdle);
            ResetRxStateFlags();
         }
         else
# endif
         {
            if (rxGetState() == kTpRxWaitCF)
            {
               /* Reception is in progress */
               /* Free the state machine */
               StopRxTimer();
               rxSetState(kTpRxIdle);
               ResetRxStateFlags();
            }
         }
#endif

         if ((TPCI_Byte > kSF_DataLength)
#if defined( FBL_TP_ENABLE_ISO15765_2_2 )
            || (0u == TPCI_Byte)
#endif
         )
         {
            __ApplFblTpErrorIndication((vuint8)kTpErrRxSFDL);  /* Notify application */
            return kCopyNoData;                                /* The frame is not accepted */
         }
         else
         {
            /* Store length for application usage */
            rxDataLength = TPCI_Byte; /* & kL4_SFData not necessary */
         }

#if defined( FBL_TP_ENABLE_VARIABLE_RX_DLC )
         /* Variable checking of CAN-DLC */
         if (CanRxActualDLC <= rxDataLength+kTpciPos)
         {
            return kCopyNoData;
         }
#endif

#if defined( FBL_TP_ENABLE_EXTENDED_ADDRESSING )
         gbRxSourceAddress = (canuint8)TESTER_NODE_ADDRESS;
#endif /* FBL_TP_ENABLE_EXTENDED_ADDRESSING */

         __ApplFblTpRxSF(); /* Defined in tp_cfg.h / tp.cfg; Normally not used */

#if defined( FBL_TP_ENABLE_INTERNAL_MEMCPY )
         {
            vuintx i;
            for (i = 0u; i < rxDataLength; i++)
            {
               rxDataBuffer[i] = *(FirstDataSFCF + i);
            }
         }
#else /* FBL_TP_ENABLE_INTERNAL_MEMCPY */
         __ApplFblTpCopyFromCAN(rxDataBuffer, FirstDataSFCF, rxDataLength); /* PRQA S 0312 */ /* MD_FblTp_0312 */
#endif
         rxSetState(kTpRxBlocked);
         __ApplFblTpDataInd((tTpDataType)rxDataLength); /* Ready, a single frame was received */
         break;
      }
      /*-----------------------------------------------------------------------------
      | Receive FirstFrame
      -----------------------------------------------------------------------------*/
      case kL4_FirstFrame:
      {
         /* Use a temporary data length variable until it's certain that the frame is valid */
         vuint16 tmpDL;

#if defined( FBL_TP_ENABLE_VARIABLE_RX_DLC )
         /* Checking CAN-DLC */
         if (CanRxActualDLC != 8u)
         {
            return kCopyNoData;
         }
#endif
#if defined( FBL_TP_ENABLE_ISO_COMPLIANCE )
#else
# if defined( FBL_TP_ENABLE_CONFIRMATION_INTERRUPT )
         if (IsWaitingForConfInterrupt())
         {
            return kCopyNoData; /* Receive nothing while waiting */
         }
# endif
#endif
         if (kTpRxBlocked == rxGetState())
         {
            /* Transport Layer blocked, FF not allowed */
            /* Only buffer is blocked - reception not in progress - kill that frame */
            __ApplFblTpErrorIndication((vuint8)kTpErrRxNotIdle); /* Notify application */
            return kCopyNoData;
         }

#if defined( FBL_TP_ENABLE_ISO_COMPLIANCE )
# if defined( FBL_TP_ENABLE_CONFIRMATION_INTERRUPT )
         /* NOTE: Non-Compliance for FBL_TP */
         if (IsWaitingForConfInterrupt())
         {
            /* Reception is in progress */
            /* First cancel transmission object */
            /* CanCancelTransmit(kFblTpTxHandle); */
            /* Free the state machine */
            InitConfState(); /* PRQA S 3109 */ /* MD_FblTp_3109 */
            StopRxTimer();
            rxSetState(kTpRxIdle);
            ResetRxStateFlags();
         }
         else
# endif
         {
            if (rxGetState() == kTpRxWaitCF)
            {
               /* Reception is in progress */
               /* Free the state machine */
               StopRxTimer();
               rxSetState(kTpRxIdle);
               ResetRxStateFlags();
            }
         }
#endif

         /* Optimized version for diagnostic only with *(data+1) */
         tmpDL  = (((vuint16)(TPCI_Byte & kL4_FFData)) << 8);
         tmpDL |=   (vuint16)DL_Byte;

         /* Check for invalid length */
         if (tmpDL <= kSF_DataLength)
         {
            return kCopyNoData; /* The frame is not accepted */
         }

         if (tmpDL > kFblTpBufferSize)
         {
#if defined( FBL_TP_ENABLE_ISO15765_2_2 ) || \
    defined( FBL_TP_ENABLE_OVERRUN_FLAG_IN_FC )
            AssembleFC();
            rxReloadBSCnt();
            tpCanTxData[kTpciPos] = kL4_FlowStatus_Overrun;
# if defined( FBL_TP_ENABLE_CONFIRMATION_INTERRUPT )
            SetWaitForFCOverrunConfInterrupt(); /* PRQA S 3109 */ /* MD_FblTp_3109 */
# endif
            (void)CAN_SaveTransmit();
            return kCopyNoData;
#else
            /* Inform application */
            __ApplFblTpMsgTooLong((vuint8)DL_Byte, (vuint8)(TPCI_Byte & 0x0Fu));
# if defined( FBL_TP_ENABLE_ACCEPT_TOO_LARGE_DATA )
            /* Receiving a large block that exceeds the capacity
             * of the DiagBuffer. FlowControl is transmitted in this
             * configuration, but no further data (excepting
             * FirstFrame data) are copied to the buffer. */
            SetRxBlockTooLargeFlag();
# else
            return kCopyNoData; /* Don't accept frames longer than our buffer */
# endif /* ! FBL_TP_ENABLE_ACCEPT_TOO_LARGE_DATA */
#endif /* ! FBL_TP_ENABLE_ISO15765_2_2 */
         }
#if defined( FBL_TP_ENABLE_ACCEPT_TOO_LARGE_DATA )
         else
         {
            ClearRxBlockTooLargeFlag();
         }
#endif
         rxDataLength = (tTpDataType)tmpDL;

#if defined( FBL_TP_ENABLE_EXTENDED_ADDRESSING )
         gbRxSourceAddress = (canuint8) TESTER_NODE_ADDRESS;
#endif /* FBL_TP_ENABLE_EXTENDED_ADDRESSING */

         __ApplFblTpRxFF(); /* Defined in tp_cfg.h/tp.cfg; Normally not used */

#if defined( FBL_TP_ENABLE_INTERNAL_MEMCPY )
         {
            vuintx i;
            for (i = 0u; i < kFF_DataLength; i++)
            {
               rxDataBuffer[i] = *(FirstDataFF + i);
            }
         }
#else
         __ApplFblTpCopyFromCAN( rxDataBuffer, FirstDataFF, kFF_DataLength ); /* PRQA S 0312 */ /* MD_FblTp_0312 */
#endif /* ! FBL_TP_ENABLE_INTERNAL_MEMCPY */

         rxDataIndex = kFF_DataLength;    /* Set RX index to next free data element */
         AssembleFC();                    /* FF always required TX of a FC */
         rxReloadBSCnt();
#if defined( FBL_TP_ENABLE_CONFIRMATION_INTERRUPT )
         SetWaitForFCConfInterrupt(); /* PRQA S 3109 */ /* MD_FblTp_3109 */
#else
         SetRxCFFlag();
         StartRxTimeoutCF(kTimeoutCF, kTpRxWaitCF); /* Wait for next CF */ /* PRQA S 3109 */ /* MD_FblTp_3109 */
#endif
         /* Make sure that repetition is not interpreted accidentally for TX-finish */
         txSEG = 1u;
         rxSetSN(kTpSNStartValue);  /* Await CF with SN = kTpSNStartValue first */
         (void)CAN_SaveTransmit();
         break;
      }
      /*-----------------------------------------------------------------------------
      | kL4_ConsecutiveFrame
      -----------------------------------------------------------------------------*/
      case kL4_ConsecutiveFrame:
      {
         tTpDataType const locRxDataLength = rxDataLength; /* Use local variable to avoid warnings: ESCAN00040879 */
#if defined( FBL_TP_ENABLE_CONFIRMATION_INTERRUPT )
         if (IsWaitingForConfInterrupt())
         {
            return kCopyNoData; /* Receive nothing while waiting */
         }
#endif
         if (rxGetState() != kTpRxWaitCF)
         {
            __ApplFblTpErrorIndication((vuint8)kTpErrRxCFNotExpected); /* Notify application */
            return kCopyNoData; /* No FF received, CF not allowed */
         }
         __ApplFblTpRxCF(); /* Defined in tp_cfg.h/tp.cfg; Normally not used */

         if ((TPCI_Byte&kL4_SNMask) != rxGetSN()) /* Wrong SN received? */
         {
#if defined( FBL_TP_ENABLE_WAIT_CORRECT_SN )
#else
            rxSetState(kTpRxIdle); /* Stop receiving immediately! */
            __ApplFblTpErrorIndication((vuint8)kTpErrRxWrongSN); /* Notify application */
            /*Possible TX of error frame here?*/
#endif
            return kCopyNoData;
         }

         /* A correct SN is received, calculate next expected SN in case of more CF are
          * expected. (Calculation is only necessary if more messages are expected!) */

#if defined( FBL_TP_ENABLE_ACCEPT_TOO_LARGE_DATA )
         /* BlockTooLarge indicates the reception of the data block, that exceeds the capacity of the DiagBuffer.
          * ConsecutiveFrame is accepted but no further data are copied to the buffer. */
         if (0u != GetRxBlockTooLargeFlag())
         {
            if ((locRxDataLength - rxDataIndex) >= kCF_DataLength)
            {
               rxDataIndex += kCF_DataLength;
            }
            else
            {
               rxDataIndex = locRxDataLength;
            }
         }
         else
#endif
         {
            /* Normally we are able to copy all data to RX buffer, but if the last CAN message can not be copied as
             * one block, the following check will be done to prevent buffer overrun. Then bytes are copied
             * until the RX buffer is full. */
            /* Rx buffer with free elements? */
            if ((locRxDataLength - rxDataIndex) < (tTpDataType)kCF_DataLength)
            {
#if defined( FBL_TP_ENABLE_INTERNAL_MEMCPY )
               vuint8 li; /* Load index to copy bytes of the last frame */
#endif
#if defined( FBL_TP_ENABLE_VARIABLE_RX_DLC )
            /* Checking CAN-DLC */
               if (CanRxActualDLC <= (locRxDataLength - rxDataIndex + kTpciPos))
               {
                  return kCopyNoData;
               }
#endif
#if defined( FBL_TP_ENABLE_INTERNAL_MEMCPY )
               li = 0x01;
               while (rxDataIndex < locRxDataLength)
               {
                  /* Copy data with start at (TPCI + 1) byte */
                  rxDataBuffer[rxDataIndex] = *(&TPCI_Byte + li);
                  rxDataIndex++;
                  li++;
               }
#else
               __ApplFblTpCopyFromCAN(&rxDataBuffer[rxDataIndex], FirstDataSFCF, (locRxDataLength - rxDataIndex)); /* PRQA S 0312 */ /* MD_FblTp_0312 */
               rxDataIndex = kFblTpBufferSize; /* No more free data element! */
#endif
            }
            else
            {
#if defined( FBL_TP_ENABLE_VARIABLE_RX_DLC )
               /* Checking CAN-DLC */
               if (CanRxActualDLC != 8u)
               {
                  return kCopyNoData;
               }
#endif
#if defined( FBL_TP_ENABLE_INTERNAL_MEMCPY )
               {
                  vuintx i;
                  for (i = 0u; i < kCF_DataLength; i++)
                  {
                     rxDataBuffer[rxDataIndex + i] = *(FirstDataSFCF + i);
                  }
               }
#else /* FBL_TP_ENABLE_INTERNAL_MEMCPY */
               __ApplFblTpCopyFromCAN(&rxDataBuffer[rxDataIndex], FirstDataSFCF, kCF_DataLength); /* PRQA S 0312 */ /* MD_FblTp_0312 */
#endif

               rxDataIndex += kCF_DataLength; /* Set RX index to next free data element */
            }
         } /* To prevent misunderstandings */

         if (rxDataIndex >= locRxDataLength) /* Last message? (Also if buffer is full!) */
         {
            rxSetState(kTpRxBlocked);
            /* StopRxTimer(); */
#if defined( FBL_TP_ENABLE_ACCEPT_TOO_LARGE_DATA )
            if (0u != GetRxBlockTooLargeFlag())
            {
               ClearRxBlockTooLargeFlag();

               /* Indicate reception of overrun data.
                * First argument is number of data in receive buffer,
                * second argument is no. of expected data.
                * (BO: Buffer Overrun). */
               __ApplFblTpDataIndOverrun((vuint8)kSF_DataLength, (tTpDataType)locRxDataLength);
            }
            else
            {
#endif
               __ApplFblTpDataInd((tTpDataType)locRxDataLength);
#if defined( FBL_TP_ENABLE_ACCEPT_TOO_LARGE_DATA )
            } /* To prevent misunderstandings */
#endif

         }
         else
         {                             /* No, wait for next CF */
            SetRxCFFlag();
            StartRxTimeoutCF(kTimeoutCF, kTpRxWaitCF); /* PRQA S 3109 */ /* MD_FblTp_3109 */
            if (rxCheckBSZero() != 0u) /* Check if a FC is requested (BS == 0?) */
            {
               rxDecBSCnt();           /* FC is requested! */
               if (! rxCheckBSCnt())   /* Check if a FC must be send (counter is zero) */
               {
                  AssembleFC();        /* Assemble FC and reload BS counter */
                  rxReloadBSCnt();
#if defined( FBL_TP_ENABLE_CONFIRMATION_INTERRUPT )
                  SetWaitForFCConfInterrupt(); /* PRQA S 3109 */ /* MD_FblTp_3109 */
                  StopRxTimer();       /* Stop RX timeout CF */
#endif
                  (void)CAN_SaveTransmit();
               }
            }
            if (rxGetSN() < kL4_MaxSN)
            {
               /* SN is calculated modulo 15 */
               rxIncSN(); /* SN increment; Optimization! See define! */
            }
            else
            {
               rxResetSN(); /* SN set to zero; optimization! See define! */
            }
         }
         break;
      }
      /*-----------------------------------------------------------------------------
      | kL4_FlowControl
      -----------------------------------------------------------------------------*/
      case kL4_FlowControl:
      {
#if defined( FBL_TP_ENABLE_CONFIRMATION_INTERRUPT )
         if (IsWaitingForConfInterrupt())
         {
            return kCopyNoData; /* Receive nothing while waiting */
         }
#endif
#if defined( FBL_TP_ENABLE_VARIABLE_RX_DLC )
         /* Checking CAN-DLC */
         if (CanRxActualDLC < (kFC_DataLength + kTpciPos))
         {
            return kCopyNoData;
         }
#endif
         /* Attention: No FC after last data message allowed */
         if (txGetState() != kTpTxWaitFC)
         {
            /** This notification does not reset the current transmission status */
            TxConfirm(kTpErrTxFCNotExpected); /* Notify application */
            return kCopyNoData;
         }

         if ((TPCI_Byte & 0x0Fu) == kFlowStateWait)
         {
            /* WaitFrame received, set timer again */
            StartTxTimeoutFC(kTimeoutFC, kTpTxWaitFC); /* PRQA S 3109 */ /* MD_FblTp_3109 */
            return kCopyNoData;
         }
#if defined( FBL_TP_ENABLE_ISO15765_2_2 ) || \
    defined( FBL_TP_ENABLE_OVERRUN_FLAG_IN_FC )
         else if ((TPCI_Byte & 0x0Fu) == kFlowStateOverrun)
         {
            FblTpInit();
            /** This notification does not reset the current transmission status */
            TxConfirm(kTpErrTxOverrun);  /* Notify application */
            return kCopyNoData;
         }
#endif

#if defined( FBL_TP_ENABLE_FLOW_STATE )
         else if ((TPCI_Byte & 0x0Fu) != kFlowStateCTS)
         {
            FblTpInit();
            /* Not a valid FlowControl frame - discard */
            TxConfirm(kTpErrTxFSInvalid);  /* Notify application */
            return kCopyNoData;
         }
#endif
         else
         {
            /* MISRA */
            ;
         }

# if defined( FBL_TP_ENABLE_ONLY_FIRST_FC )
         if (! txGetFCEvaFlag())
         {
            txSetFCEvaFlag();    /* Set flag with the first received FC */
# endif

            txSetBS( DL_Byte); /* Take the full 8 bit BS */ /* PRQA S 3109 */ /* MD_FblTp_3109 */

# if defined( FBL_TP_ENABLE_ISO15765_2_2 )
            if ((STmin_Byte & 0x80u) == 0x80u)
            {
               if ((STmin_Byte > 0xF0u) && (STmin_Byte <= 0xF9u))
               {
                  STmin = 1u;
               }
               else
               {
                  /* Max STmin time */
                  STmin = (vuint8)((127u / TpCallCycle) + 1u);
               }
            }
            else
# endif
            {
# if defined( FBL_TP_ENABLE_OLD_STMIN_CALCULATION )    /* User configuration. Not supported by GENy/GUI. */
               if (STmin_Byte <= TpCallCycle)
               {
                  STmin = 1u;
               }
               else
               {
                  STmin = (vuint8)((STmin_Byte + (TpCallCycle - 1u)) / TpCallCycle);
               }
# else
               STmin = (vuint8)((STmin_Byte / TpCallCycle) + 1u);
               /* Check for integer overflow */
               if (STmin == 0x00u)
               {
                  /* Recover from overflow */
                  STmin = 255u;
               }
# endif
            }

# if defined( FBL_TP_ENABLE_ONLY_FIRST_FC )
         }
         else
         {
            txReloadBSCnt();
         }
# endif
#if defined( FBL_TP_ENABLE_NO_STMIN_AFTER_FC )
         StartTxWaitForTxCF(1, kTpTxWaitForTxCF); /* PRQA S 3109 */ /* MD_FblTp_3109 */
#else
         StartTxWaitForTxCF(STmin, kTpTxWaitForTxCF); /* PRQA S 3109 */ /* MD_FblTp_3109 */
#endif
         break;
      }
      default:
      {
         /* Not used */
         break;
      }
   }

   return kCopyNoData;
} /* PRQA S 4700 */ /* MD_FblTp_4700 */ /* PRQA S 2006 */ /* MD_FblTp_2006 */

/***********************************************************************************************************************
 *  FblTpTask
 **********************************************************************************************************************/
/*! \brief       Cyclicly called task function (e.g. every 10 ms)
 *  \attention   This functions stores the SN within the used CAN transmit buffer. No other function should write to
 *               this buffer!
 **********************************************************************************************************************/
void FblTpTask(void)
{

   /* Disabling the interrupt is not necessary */
   /* Do RX stuff even if waiting for confirmation interrupt */
   if ((rxGetState() & kTpRxWaitCF) != 0u)
   {
      if (RxTimerValue() != 0u)
      {
            /* Set a prestate for checking a interrupt while decrementing */
            ClearRxCFFlag();
            DecRxTimerValue();

            if (! RxTimerValue())
            {
               /* CF timeout occurred, do not notify application now! */
               FblTpInit(); /* And go to idle state */
               __ApplFblTpErrorIndication((vuint8)kTpErrRxTimeout); /* Notify application */
            }
            else
            {
               if (GetRxCFFlag() != 0u)
               {
                  /* Receive interrupt occurred */
                  /* The RxTimerValue is undefined, because of an interrupt */
                  /* during the decrementing -> reload timer */
                  SetRxTimerValue(kTimeoutCF);
               }
            }
      }
   }

   /* RepeatTransmit only necessary with disabled can queue */
   if (txGetState() == kTpTxRepeatTransmit)
   {
      if (FblCanTransmit(kFblTpTxHandle) == kFblTxOk)
      {
#if defined( FBL_TP_ENABLE_CONFIRMATION_INTERRUPT )
#else
         __ApplFblTpCanMessageTransmitted(); /* Used for e.g. reseting of application counters */
#endif
         if (txSEG == 0u)
         {
            /* For a single frame txSEG is always zero */
            txSetState(kTpTxIdle);
            rxSetState(kTpRxIdle);
            StopTxTimer();

#if defined( FBL_TP_ENABLE_CONFIRMATION_INTERRUPT )
#else
            TxConfirm(kTpSuccess);
#endif
         }
         else
         {
#if defined( FBL_TP_ENABLE_EXTENDED_ADDRESSING )
            SetTargetAddrForTx( gbTxTargetAddress );
#endif
            /* Check for frame type of repetition and switch state */
            if ((tpCanTxData[kTpciPos] & kL4_FrameTypeMask) == kL4_FirstFrame)
            {
               StartTxTimeoutFC(kTimeoutFC, kTpTxWaitFC); /* PRQA S 3109 */ /* MD_FblTp_3109 */
            }
            else if ((tpCanTxData[kTpciPos] & kL4_FrameTypeMask) == kL4_FlowControl)
            {
               txSetState(kTpTxIdle);
#if defined( FBL_TP_ENABLE_CONFIRMATION_INTERRUPT )
#else
               StartRxTimeoutCF(kTimeoutCF, kTpRxWaitCF); /* Wait for next CF */ /* PRQA S 3109 */ /* MD_FblTp_3109 */
#endif
               StopTxTimer();
            }
            else if ((tpCanTxData[kTpciPos] & kL4_FrameTypeMask) == kL4_ConsecutiveFrame)
            {
               StartTxWaitForTxCF(STmin, kTpTxWaitForTxCF); /* PRQA S 3109 */ /* MD_FblTp_3109 */
#if defined( FBL_TP_ENABLE_CONFIRMATION_INTERRUPT )
#else
               if (txCheckBSZero() != 0u)
               {
                  txDecBSCnt();
                  if (! txCheckBSCnt())
                  {
                     /* Check for FC of counterpart now */
                     /* Overwrite state from kTpTxWaitForTxCF to kTpTxWaitFC */
                     /* bTpTxState for TX changed */
                     StartTxTimeoutFC(kTimeoutFC, kTpTxWaitFC); /* PRQA S 3109 */ /* MD_FblTp_3109 */
                  }
               }
#endif
            }
            else
            {
               /* ("else" required by MISRA) */
            }
         }
         /* If transmission was successful here, wait for next call, to keep the timing
          * (would be too fast if Tx is started in the upcoming switch statement ). */
         if (STmin != 0u)
         {
            return;
         }
      }
   }

#if defined( FBL_TP_ENABLE_CONFIRMATION_INTERRUPT )
   /* Disabling the interrupt is not necessary */
   if (IsWaitingForConfInterrupt())
   {
      if (FblCanMsgTransmitted() == kFblTxOk)
      {
         FblTpConfirmation(0u);
      }
      else
      {
         if (GetConfInterruptTimer() != 0u)
         {
            gTpConfTimer--;
            return;  /* Neglect all other things while waiting   */
         }
         else
         {
            FblTpInit();
            /* Timeout, apparently no access to bus  */
            TxConfirm(kTpCanTxFailed);  /* Notify application and go to idle state */
         }
      }
   }
#endif /* FBL_TP_ENABLE_CONFIRMATION_INTERRUPT */

   if (TxTimerValue() != 0u)
   {
      DecTxTimerValue();

      if (! TxTimerValue())
      {
         switch (txGetState())
         {
            case kTpTxWaitFC:
            {
               FblTpInit();
               /* FC timeout occurred! */
               TxConfirm(kTpTimeoutFC);  /* Notify application and go to idle state */
               break;
            }
            case kTpTxWaitForTxCF:
            {
               txSEG--;
               /*Assemble CF now*/
               tpCanTxData[kTpciPos]=(vuint8)(kL4_ConsecutiveFrame | (txSN & kL4_MaxSN));

               txSN++;                      /* SN increment */

               /* Prepare the CAN-frame for CF */
#if defined( FBL_TP_ENABLE_INTERNAL_MEMCPY )
               {
                  vuintx i;
                  for (i = 0u; i < kCF_DataLength; i++)
                  {
                     tpCanTxData[kCFDataPos + i] = txDataBuffer[txDataIndex + i];
                  }
               }
#else
               __ApplFblTpCopyToCAN(&tpCanTxData[kCFDataPos], &txDataBuffer[txDataIndex], kCF_DataLength);
#endif
#if defined( FBL_TP_ENABLE_EXTENDED_ADDRESSING )
               SetTargetAddrForTx(gbTxTargetAddress);
#endif
               txDataIndex += kCF_DataLength;

               if (txSEG == 0u)
               {
#if defined( FBL_TP_ENABLE_VARIABLE_TX_DLC )
                  tpTxDLC = (vuint8)(kCanFrameLength - bPaddingLength);
#endif
#if defined( FBL_TP_ENABLE_TX_FRAME_PADDING )
                  /* (void)memset((void*)&tpCanTxData[ 8 - bPaddingLength ], kFblTpFillPattern, bPaddingLength);  (old style) */
                  while (bPaddingLength > 0u)
                  {
                     tpCanTxData[kCanFrameLength - bPaddingLength] = kFblTpFillPattern;
                     bPaddingLength--;
                  }
                  /* ! Note: bPaddingLength destroyed ! */
#endif

                  /* Send last CF now and we're done! No FC after last CF */
#if defined( FBL_TP_ENABLE_CONFIRMATION_INTERRUPT )
                  SetWaitForLastCFConfInterrupt(); /* PRQA S 3109 */ /* MD_FblTp_3109 */
                  txSetState(kTpTxIdle);
                  rxSetState(kTpRxIdle);
                  (void)CAN_SaveTransmit();
#else
                  if (CAN_SaveTransmit() == kTpSuccess)
                  {
                     txSetState(kTpTxIdle);
                     rxSetState(kTpRxIdle);
                     TxConfirm(kTpSuccess);
                  }
#endif
               }
               else
               {
#if defined( FBL_TP_ENABLE_VARIABLE_TX_DLC )
                  tpTxDLC = (vuint8)8u;
#endif
#if defined( FBL_TP_ENABLE_CONFIRMATION_INTERRUPT )
                  StartTxWaitForTxCF(STmin, kTpTxWaitForTxCF); /* PRQA S 3109 */ /* MD_FblTp_3109 */
                  SetWaitForCFConfInterrupt(); /* PRQA S 3109 */ /* MD_FblTp_3109 */
                  (void)CAN_SaveTransmit();
#else
                  if (CAN_SaveTransmit() == kTpSuccess)
                  {
                     __ApplFblTpNotifyTx(kCF_DataLength);
                     StartTxWaitForTxCF(STmin, kTpTxWaitForTxCF); /* PRQA S 3109 */ /* MD_FblTp_3109 */

                     if (txCheckBSZero() != 0u)
                     {
                        txDecBSCnt();
                        if (! txCheckBSCnt())
                        {
                           /* Check for FC of counterpart now */
                           /* Overwrite state from kTpTxWaitForTxCF to kTpTxWaitFC */
                           /* bTpTxState for TX changed */
                           StartTxTimeoutFC(kTimeoutFC, kTpTxWaitFC); /* PRQA S 3109 */ /* MD_FblTp_3109 */
                        }
                     }
                  }
#endif
               }
               break;
            }
            case kTpTxIdle:
            {
               break;
            }
            case kTpTxRepeatTransmit:
            default:
            {
               /* typically, when the txTimer expires for Repeated messages */
               txSetState(kTpTxIdle);
               rxSetState(kTpRxIdle);
               TxConfirm(kTpCanTxFailed);
               break;
            }
         } /* End of "switch (txGetState())" */
      } /* End of "if (TxTimerValue())" */
   }
} /* PRQA S 4700 */ /* MD_FblTp_4700 */ /* PRQA S 2006 */ /* MD_FblTp_2006 */

/***********************************************************************************************************************
 *  FblTpConfirmation
 **********************************************************************************************************************/
/*! \brief       Confirmation function
 *  \description Without confirmation interrupt: done in Can_SaveTransmit
 *               Can_SaveTransmit is called in:
 *               - TpPrecopy    to send a FlowControl
 *               - TpTransmit   to send a SingleFrame or a FirstFrame
 *               - TpTransmitSF to send a SingleFrame
 *               - TpTask       to send a ConsecutiveFrame
 * \param[in]    txObject A transmit object handle
 **********************************************************************************************************************/
void FblTpConfirmation(CanTransmitHandle txObject)
{
#if defined( FBL_TP_ENABLE_CONFIRMATION_INTERRUPT )
   vuint8 confIntState;
#endif

#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   (void)txObject; /* PRQA S 3112 */ /* MD_FblTp_3112 */
#endif

#if defined( FBL_TP_ENABLE_CONFIRMATION_INTERRUPT )
   confIntState = GetConfInterruptState();
   ResetConfState();

   /* Compiler warning about unused argument can be safely ignored */
   switch (confIntState)
   {
      case kWaitForSFConfInterrupt:
      {
         /* Without confirmation interrupt: done in TpTransmit */
         /* !! Not done, if SF is send via TpTransmitSF !! */
         TxConfirm(kTpSuccess);    /* inform application */
         break;
      }
      case kWaitForFFConfInterrupt:
      {
         /* Without confirmation interrupt: done in TpTransmit */

# if defined( FBL_TP_DISABLE_OSEK_DATA_LENGTH ) && \
     defined( C_CPUTYPE_8BIT )
# else
         /* Reset sequence counter after sending first frame */
         tpCanTxData[kTpciPos] = (tTpDataType)kL4_FirstFrame;
# endif

         __ApplFblTpNotifyTx(kFF_DataLength);
         break;
      }
      case kWaitForFCConfInterrupt:
      {
         /* Without confirmation interrupt: done in TpPrecopy or TpTask */
         SetRxCFFlag();
         StartRxTimeoutCF(kTimeoutCF, kTpRxWaitCF);   /* Wait for next CF */ /* PRQA S 3109 */ /* MD_FblTp_3109 */
         break;
      }
# if defined( FBL_TP_ENABLE_ISO15765_2_2 ) || \
     defined( FBL_TP_ENABLE_OVERRUN_FLAG_IN_FC )
      case kWaitForFCOverrunConfInterrupt:
      {
         FblTpInit();
         break;
      }
# endif
      case kWaitForCFConfInterrupt:
      {
         /* Without confirmation interrupt: done in TpTask */
         __ApplFblTpNotifyTx(kCF_DataLength);
         if (txCheckBSZero() != 0u)
         {
            txDecBSCnt();
            if (! txCheckBSCnt())
            {
               /* Check for FC of counterpart now */
               /* Overwrite state from kTpTxWaitForTxCF to kTpTxWaitFC */
               /* bTpTxState for TX changed */
               StartTxTimeoutFC(kTimeoutFC, kTpTxWaitFC); /* PRQA S 3109 */ /* MD_FblTp_3109 */
            }
         }
         break;
      }
      case kWaitForLastCFConfInterrupt:
      {
         /* Without confirmation interrupt: done in TpTask   */
         TxConfirm(kTpSuccess);
         break;
      }
      case kWaitForFCWaitFrameConfInterrupt:
      {
         /* Not yet supported */
         break;
      }
      case kWaitForFCQueuedReqWaitFrameConfInterrupt:
      {
         /* No action required */
         break;
      }
      default:
      {
         /* Nothing to do */
         break;
      }
   } /* End of switch state */

   __ApplFblTpCanMessageTransmitted(); /* Used for e.g. reseting of application counters */
#endif /* Use of confirmation interrupt */
} /* PRQA S 4700 */ /* MD_FblTp_4700 */

#if defined( FBL_TP_ENABLE_EXTENDED_ADDRESSING )

/***********************************************************************************************************************
 *  EXTENDED API FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  FblTpSetTargetAddress
 **********************************************************************************************************************/
/*! \brief       Additional API function to manipulate target address using extended addressing
 **********************************************************************************************************************/
void FblTpSetTargetAddress(vuint8 address)
{
   gbTxTargetAddress = address;
}

/***********************************************************************************************************************
 *  FblTpGetSourceAddress
 **********************************************************************************************************************/
/*! \brief       Additional API function to read source address using extended addressing
 **********************************************************************************************************************/
vuint8 FblTpGetSourceAddress(void)
{
   return gbRxSourceAddress;
}

/***********************************************************************************************************************
 *  FblTpSetSourceAddress
 **********************************************************************************************************************/
/*! \brief       Additional API function to manipulate source address using extended addressing
 **********************************************************************************************************************/
void FblTpSetSourceAddress(vuint8 address)
{
   gbRxSourceAddress = address;
}

#endif /* FBL_ENABLE_EXTENDED_ADDRESSING */

/* PRQA L:FblTp_0277 */

/**********************************************************************************************************************
 *  MISRA
 *********************************************************************************************************************/

/* Module specific MISRA deviations:

   MD_FblTp_0277:
      Reason: Usage of FblInvert.. macros implies usage of ~ operator and appropriate casts. This includes a cast from
              negative integer to unsigned value. The integer is only an intermediate type that occurs because of
              integer promotion while using ~ operator, though only a bit mask is operated on.
      Risk: No risk.
      Prevention: No prevention defined.

   MD_FblTp_2006:
      Reason: Error handling is quite complex and single return path would be require complicated control flow.
      Risk: Resulting code is difficult to understand and maintain.
      Prevention: All changes to the affected code parts have to be reviewed by the module responsible.

   MD_FblTp_3109:
      Reason: Multi-line macros encapsulated in curly brackets to reduce side-effects. Results in empty statement after
              the closing bracket.
      Risk: No identifiable risk.
      Prevention: No prevention required.

   MD_FblTp_3112:
      Reason: Dummy statements have been added to avoid compiler warnings because of unused variables.
      Risk: Unused statements add to program complexity just by being there - adding lines to the program and causing
            readers to puzzle over its presence.
      Prevention: Dummy statements can be turned off externally (V_DISABLE_USE_DUMMY_STATEMENT).

   MD_FblTp_4700:
      Reason: Error handling is quite complex resulting in complicated control flow.
      Risk: Resulting code is difficult to understand and maintain.
      Prevention: All changes to the affected code parts have to be reviewed by the module responsible.

*/

/***********************************************************************************************************************
 *  END OF FILE: FBL_TP.C
 **********************************************************************************************************************/
