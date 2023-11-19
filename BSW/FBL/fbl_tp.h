/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        OSEK transport protocol version ISO-15765
 *  \description  This versions supports the specification for the OSEK transport protocol with the following
 *                restriction: Maximum message length is 255 byte for RX and TX (no length in TPCI).
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
 *  Alexandre C. Plombin          ACP           Vector CANtech, Inc.
 *  Michael F. Radwick            MFR           Vector CANtech, Inc.
 *  Quetty Palacios               QPs           Vector Informatik GmbH
 *  Marcel Viole                  MVi           Vector Informatik GmbH
 *  Andre Caspari                 Ci            Vector Informatik GmbH
 *  Thomas Bezold                 TBe           Vector Informatik GmbH
 *  Achim Strobelt                Ach           Vector Informatik GmbH
 *  Alexander Starke              ASe           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  02.52.00   2000-11-14  Js                       No changes
 *  02.53.00   2000-12-22  Hp                       No changes
 *  02.53.00   2000-12-22  Hp                       No changes
 *  02.54.00   2001-10-16  Hp                       TpInit() inserted for failure case
 *                                                  Wrong STmin calculation on FC corrected
 *  02.55.00   2001-11-28  CB                       Prototype for TpPrecopy added
 *  02.56.00   2002-01-09  ACP                      Hiware HC12 and IAR HC12
 *  02.57.00   2002-05-13  Hp                       Added extended addressing and external memcopy feature
 *  02.58.00   2002-09-12  Hp                       ESCAN0000: Adding DLC-check and ISO-compliance
 *  02.59.00   2002-11-11  Fz                       No changes
 *  02.60.00   2003-02-04  Hp                       No changes here (see TP.C)
 *  02.61.00   2004-02-12  WN                       No changes
 *  02.62.00   2003-04-01  Rr                       Minor changes for V850SCx
 *  02.63.00   2003-04-30  WM                       No changes
 *  02.64.00   2003-09-19  Rr                       No changes
 *  02.65.00   2003-09-25  Rr                       Minor changes for V850 NEC compiler
 *  02.66.00   2004-02-06  Ls      ESCAN00007291    New OEM, extended addressing, FBL_FILL_PATTERN added
 *                         CB      ESCAN00007368    Overrun frame added
 *                         WM                       No changes
 *  02.67.00   2004-02-27  Rr                       Added new OEM
 *  02.68.00   2004-06-11  Gz/WM   ESCAN00008586    Support for ISO 15765-2 extension
 *  02.69.00   2004-07-16  Ls                       No changes
 *  02.70.00   2004-07-20  CB                       No changes
 *  02.71.00   2004-08-25  MHz     ESCAN00009326    Added function TpSetSourceAddress() for extendend Addressing
 *  02.72.00   2004-10-20  MHz                      Added default value for TP_USE_EXTENDED_ADDRESSING
 *  02.73.00   2004-11-12  Rr                       No changes
 *  02.74.00   2004-11-22  Et      ESCAN00010341    Support FBL_USE_OWN_MEMCPY
 *  02.75.00   2004-12-23  ACP                      Version check reworked
 *  02.76.00   2005-03-09  WM                       No changes
 *  02.77.00   2005-04-18  Rr      ESCAN00011941    TP_USE_WAIT_FOR_CORRECT_SN config
 *                                                  switch added
 *                                 ESCAN00011942    Added TP_USE_NO_STMIN_AFTER_FC config switch
 *  02.78.00   2005-05-17  Rr                       Add variable DLC support (TP_USE_VARIABLE_DLC)#
 *  02.79.00   2005-07-05  Mhz     ESCAN00012759    Changed setting of
 *                                                  gbRxSourceAddress for OEM Fiat07274
 *  02.80.00   2005-12-21  Ls                       No changes
 *  02.81.00   2006-08-04  Ls                       No changes
 *                         Hp      ESCAN00017186    Retry of transmission not handled correctly
 *  02.82.00   2006-09-15  Hp                       No changes here (see fbl_tp.c)
 *                         swk     ESCAN00017726    Added support for C_COMP_MTRWRKS_MCS08_MSCAN_COMMENT
 *  02.83.00   2006-09-26  Hp      ESCAN00017833    Changes after internal review
 *  02.84.00   2006-10-13  Hp      ESCAN00017988    FlowControl handling does only work for BS == 0
 *  02.85.00   2006-11-06  Hp                       No changes
 *  02.85.01   2007-01-23  Hp                       No changes (see fbl_tp.c)
 *  02.85.02   2007-02-27  MFR                      No changes
 *  02.85.03   2007-02-27  QPs                      No changes
 *  02.85.04   2007-06-21  Hp                       No changes (see fbl_tp.c)
 *  03.00.00   2008-02-08  Hp      ESCAN00024337    Adapt configuration switches to match naming rules.
 *                                                  FTP_CFG.H now generated by GENy.
 *  03.00.01   2008-02-28  Hp      ESCAN00024955    Add optionally support to map TP-functions into RAM using memmap
 *                                                  Indicate useless configuration combination with
 *                                                  ISO15765-2.2/Overrun bit support and TOO_LARGE_DATA indication.
 *                                                  Correcting prototype for FblTpPrecopy on some configurations.
 *  03.01.00   2008-03-12  Rr      ESCAN00025302    Removed pragma for V850 with NEC compiler
 *                                 ESCAN00027212    No changes
 *  03.02.00   2008-06-05  QPs     ESCAN00027374    No changes
 *                         Ci      ESCAN00027409    Added support for configurations with multiple CAN channels
 *  03.02.01   2008-06-24  MVi     ESCAN00027820    No changes
 *  03.03.00   2008-11-18  AWh     ESCAN00031393    Allow configurations with both FblTp and TPMC, allow
 *                                                  alternative txDataBuffer and rxDataBuffer configurations.
 *  03.04.00   2009-05-18  Ci      ESCAN00032418    No changes
 *                         Ls      ESCAN00035170    renamed FBL_TP_DISABLE_V2XX_COMPATIBILITY
 *                         QPs     ESCAN00035243    Support for variable block size and STmin
 *  03.05.00   2009-08-13  Hp      ESCAN00037079    Call Confirmation within FblTpTransmitSF after transmission request
 *                                                  Remove CAN calls if COM wrapper is used.
 *  03.05.01   2009-09-10  Hp      ESCAN00037122    No changes here (see fbl_tp.c)
 *  03.06.00   2009-12-09  TBe     ESCAN00039383    No changes
 *  03.07.00   2010-02-16  AWh     ESCAN00040879    Adaptations to reduce compiler warnings, changed include order
 *                                                  for string.h/stddef.h, readded kTpTxHandle compatibility macro
 *  03.08.00   2010-10-14  Ci      ESCAN00044300    Added definition of tpCanTxData
 *                                                  for Vector CAN driver configurations
 *                         Hp      ESCAN00045954    TxTimer isn't reset after successful send repetition
 *  03.09.00   2012-01-23  AWh     ESCAN00054601    Added configuration to allow dynamic switch normal/extended addressing
 *  03.10.00   2012-05-25  Ach     ESCAN00057426    No changes
 *                         CB      ESCAN00059071    No changes
 *  03.11.00   2012-07-06  CB      ESCAN00059951    No changes
 *  03.12.00   2013-04-19  ASe     ESCAN00064581    Adapted comments to use Doxygen
 *                         Hp      ESCAN00066786    TpRx- and TpTx states splitted and simplified, separate var. for rxSN
 *                                 ESCAN00066792    Add support for queued requests
 *  03.12.01   2013-07-25  Hp      ESCAN00068861    Remove compile check INTERRUPT_CONFIRMATION with CAN_CONFIRMATION 
 *                                                  for CBD-Driver usage
 *                                 ESCAN00069345    Separate FC-timer for FC.Wait transmission
 *                         
 *  NOTE: This version has been adapted and must be used with Flash-Bootloader ONLY!
 **********************************************************************************************************************/

#ifndef __FBL_TP_H__
#define __FBL_TP_H__

#include "v_cfg.h"
#include "v_def.h"

#include "ftp_cfg.h"                /* Tool generated header file for global and local usage */

/* Set this value according to the actual version number of this module. The
 * value is used to generate the Main- and Subversion number for diagnosis.*/

/* ##V_CFG_MANAGEMENT ##CQProject : FblTp_Iso CQComponent : Implementation */
#define FBLTP_ISO_VERSION           0x0312
#define FBLTP_ISO_RELEASE_VERSION   0x01

/***********************************************************************************************************************
 *  TRANSPORT PROTOCOL GLOBAL RESULT CODES
 **********************************************************************************************************************/

#define kTpSuccess                                  0    /**< Everything is fine */
#define kTpFailed                                   1    /**< Not used? */ /* TODO: Review by hp: Define is used when FblCanTransmit() failed, kTpCanTxFailed should be used instead? */
#define kTpCanTxFailed                              2    /**< Could not send message to CAN */
#define kTpBusy                                     3    /**< TpTransmit while TP is running */
#define kTpTxBufferUnderrun                         4    /**< Not enough data to send */
#define kTpTimeoutFC                                5    /**< Timeout waiting for FlowControl */
#define kTpErrTxFCNotExpected                       9    /**< Unexpected FC received */
#define kTpErrTxWaitFrLimitExeeded                 12    /**< Unexpected FC received */
#define kTpErrTxOverrun                            15    /**< TX overrun */
#define kTpErrTxFSInvalid                          16    /**< Invalid FS received */

/* Error notification codes */
#define kTpErrRxWrongSN                             6    /**< Wrong sequence number received in CF */
#define kTpErrRxTimeout                             7    /**< Receive (CF, FC) Timeout occurred */
#define kTpErrRxNotIdle                             8    /**< TP not idle */
#define kTpErrRxCFNotExpected                      10    /**< Unexpected CF received */
#define kTpErrRxSFDL                               11    /**< Single frame data length error (DL > possible SF_DL) */
#define kTpErrRxIllegalByteCountInBlockTransfer    13    /**< Illegal byte count in block transfer */
#define kTpErrRxSFInvalidFormat                    14    /**< Single frame format error */

extern MEMORY_ROM vuint8 kFblTpMainVersion;
extern MEMORY_ROM vuint8 kFblTpSubVersion;
extern MEMORY_ROM vuint8 kFblTpBugFixVersion;

#endif /* __FBL_TP_H__ */

#ifndef OSEKTP_H_IND
#define OSEKTP_H_IND

/***********************************************************************************************************************
 *  HEADER PART FOR TP CHANNEL
 **********************************************************************************************************************/

#if !defined( kTpOn )
# define kTpOn    1
#endif

#if !defined( kTpOff )
# define kTpOff   0
#endif

/***********************************************************************************************************************
 *  CHECK ALL SWITCHES TO ISO COMPLIANCE
 **********************************************************************************************************************/

/* !!! Non-ISO compliance with FBL_TP !!! (TP_USE_CONFIRMATION_INTERRUPT == kTpOn) */

#if defined( FBL_TP_ENABLE_ISO_COMPLIANCE )
# if defined( FBL_TP_ENABLE_FLOW_STATE ) && \
     defined( FBL_TP_ENABLE_ONLY_FIRST_FC )
   /* Everything is fine */
# else
#  error "Mismatch in configuration switches - no ISO compliance"
# endif
#endif

/***********************************************************************************************************************
 *  TRANSPORT PROTOCOL GLOBAL FUNCTIONS
 **********************************************************************************************************************/

void FblTpInitPowerOn(void);
vuint8 FblTpTransmit(tTpDataType count);
#if defined( MULTIPLE_RECEIVE_BUFFER )
vuint8 FblTpPrecopy(const pChipDataPtr data);
#endif
vuint8 FblTpTransmitSF(vuint8 count);
void FblTpTask(void);
void FblTpConfirmation(CanTransmitHandle txObject);

#if defined( FBL_TP_ENABLE_EXTENDED_ADDRESSING )
void FblTpSetTargetAddress(vuint8 address);
vuint8 FblTpGetSourceAddress(void);
void FblTpSetSourceAddress(vuint8 address);
#endif

/** Functions to manipulate the receive buffer blocking state */
void FblTpResetRxBlock(void);
void FblTpSetRxBlock(void);

#if defined( FBL_TP_DISABLE_V2XX_COMPATIBILITY )
#else
/* Backward compatibility naming convention */
# define TpResetRxBlock()           FblTpResetRxBlock()
# define TpSetRxBlock()             FblTpSetRxBlock()
#endif /* FBL_TP_DISABLE_V2XX_COMPATIBILITY */

/* Macros for block size handling with max. BS-value of 255 */
#define FblTpSetRxBS(newBS)         {bRxBlockSize = (newBS); bRxBSCounter = (newBS);}  /* Reset counter-byte also! */
#define FblTpGetRxBS()              (bRxBlockSize)

/* Macros for STmin handling */
#define FblTpSetRxSTmin(newSTmin)   (bRxSTmin = (newSTmin))
#define FblTpGetRxSTmin()           (bRxSTmin)

/***********************************************************************************************************************
 *  OSEKTP GLOBAL DATA STRUCTS
 **********************************************************************************************************************/

extern tTpDataType volatile gbTpRxDL;
extern tTpDataType volatile gbTpRxLength;
extern tTpDataType volatile gbTpTxDL;

extern MEMORY_NEAR volatile vuint8 bRxBlockSize;

extern MEMORY_NEAR volatile vuint8 bRxBSCounter;

extern vuint8 bRxSTmin;

#if defined( FBL_TP_ENABLE_ISO15765_2_2 ) || \
    defined( FBL_TP_ENABLE_OVERRUN_FLAG_IN_FC )
# if defined( FBL_TP_ENABLE_ACCEPT_TOO_LARGE_DATA )
#  error "TP configuration: Useless combination of options by accepting too large data together with overrun bit support in flow-control"
# endif
#endif

#  if defined( FBL_TP_ENABLE_CONFIRMATION_INTERRUPT )
#   error "TP configuration: Usage of TP confirmation without CAN confirmation is useless."
#  endif

#endif /*OSEKTP_H_IND*/

/***********************************************************************************************************************
 *  END OF FILE: FBL_TP.H
 **********************************************************************************************************************/
