/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief         Diagnostic services supported in boot mode
 *                 Declaration of functions, variables, and constants
 *
 *  --------------------------------------------------------------------------------------------------------------------
 *  COPYRIGHT
 *  --------------------------------------------------------------------------------------------------------------------
 *  \par Copyright
 *  \verbatim
 *  Copyright (c) 2015 by Vector Informatik GmbH.                                                  All rights reserved.
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
 *  Vijay Natarajan               VNa           Vector CANtech, Inc.
 *  Ralf Haegenlaeuer             HRf           Vector Informatik GmbH
 *  Dennis O'Donnell              Dod           Vector CANtech, Inc.
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  05.00.00   2013-08-06  AWh                      Beta Release
 *  05.01.00   2014-01-31  AWh                      Final Release
 *  05.02.00   2014-02-27  AWh     ESCAN00073658    Improvement: Encapsulate partId behind FblHdrGetParsedModPartId()
 *                                 ESCAN00073659    Justify Metrics, Rework of FblReadDataByIdentifier.
 *                                 ESCAN00073864    Pipelined verification feature will not shorten download time
 *                                 ESCAN00073975    Memdriver might get deinitialized while it is not present in memory
 *  05.03.00   2014-03-12  AWh     ESCAN00074221    Development feature "Stay In Boot": No answer is sent
 *  05.04.00   2014-08-21  VNa     ESCAN00078132    Removed Assertion definitions that are defined in fbl_assert_oem
 *  05.05.00   2014-09-08  AWh     ESCAN00078279    Remove Kb-tokens, remove unused extern blockDescriptor
 *             2014-09-11  HRf     ESCAN00078353    Add initialization values in case of SEC_ENABLE_WORKSPACE_EXTERNAL
 *                                 ESCAN00078354    Change function name FblDiagTask() to FblDiagTimerTask()
 *  05.06.00   2014-11-19  Dod     ESCAN00078505    No changes
 *                         AWh     ESCAN00079190    FBL_WD_STATE_TASK is not called
 *  06.00.00   2015-01-20  AWh     ESCAN00080493    No changes
 *                                 ESCAN00080567    Support for compression
 *                                 ESCAN00078391    No changes
 **********************************************************************************************************************/
#ifndef __FBL_DIAG_H__
#define __FBL_DIAG_H__

/***********************************************************************************************************************
 *   VERSION
 **********************************************************************************************************************/

/* --- fbl_diag module version --- */
/* ##V_CFG_MANAGEMENT ##CQProject : FblKbDiag_14230_GM CQComponent : Implementation */
#define FBLKBDIAG_14230_GM_VERSION           0x0600u
#define FBLKBDIAG_14230_GM_RELEASE_VERSION   0x00u

/* --- FBL overall Version --- */
/* ##V_CFG_MANAGEMENT ##CQProject : FblOem_GM CQComponent : SipInformation */
#define FBLOEM_GM_VERSION         0x0503u
#define FBLOEM_GM_RELEASE_VERSION 0x00u

#define FBL_VERSION           FBLOEM_GM_VERSION
#define FBL_RELEASE_VERSION   FBLOEM_GM_RELEASE_VERSION

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "fbl_tp.h"
#include "fbl_hdr.h"

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/
#if !defined( START_MSG_ID )
#define START_MSG_ID   MK_ID(0x101u) /* Msg-Id for the FBL_ENABLE_STAY_IN_BOOT */
#endif

#if ( FBLDIAGHDR_GM_VERSION == 0x0100u )
/* Legacy define for older version of fbl_hdr.c */
# if !defined( FblHdrGetParsedModPartId )
#  define FblHdrGetParsedModPartId() (parsedModuleInfo.partId)
# endif
#endif

/* Macros for tester present timer */
#define DIAG_CALL_CYCLE                      TpCallCycle /* Same call cycle as tp */
#define TESTER_PRESENT_TIMEOUT               ((vuint32) (5000u / DIAG_CALL_CYCLE))

#define TimeoutTesterValue()     testerPresentTimeout
#define DecTimeoutTesterValue()  (testerPresentTimeout--)
#define ResetTesterTimeout()     (testerPresentTimeout = (vuint16)(TESTER_PRESENT_TIMEOUT / DIAG_CALL_CYCLE))
#define StopTesterTimeout()      (testerPresentTimeout = 0)

/* -- Defines of diag services -- */
#define kDiagSidInitiateDiagnosticOp                     ((vuint8) 0x10u)
#define kDiagSidReadDataByIdentifier                     ((vuint8) 0x1Au)
#define kDiagSidReturnToNormalMode                       ((vuint8) 0x20u)
#define kDiagSidReadMemoryByAddress                      ((vuint8) 0x23u)
#define kDiagSidSecurityAccess                           ((vuint8) 0x27u)
#define kDiagSidDisableNormalComm                        ((vuint8) 0x28u)
#define kDiagSidRequestDownload                          ((vuint8) 0x34u)
#define kDiagSidTransferData                             ((vuint8) 0x36u)
#define kDiagSidWriteDataByIdentifier                    ((vuint8) 0x3Bu)
#define kDiagSidTesterPresent                            ((vuint8) 0x3Eu)
#define kDiagSidReportProgrammedState                    ((vuint8) 0xA2u)
#define kDiagSidProgrammingMode                          ((vuint8) 0xA5u)

/* -- Response Identifier -- */
/** Negative response SID */
#define kDiagRidNegativeResponse                         ((vuint8) 0x7F)

/* -- Defines length of service request         -- */
/* -- (if applicable; excluding the Service-ID. -- */
#define kDiagRqlInitiateDiagnosticOp                     ((vuint8) 0x01u)
#define kDiagRqlReadDataByIdentifier                     ((vuint8) 0x01u)
#define kDiagRqlReturnToNormalMode                       ((vuint8) 0x00u)
#define kDiagRqlReadMemoryByAddress                      ((vuint8) 0x02u + kDiagRqlReadMemoryByAddressAddressParameter)
#define kDiagRqlSecurityAccessSeed                       ((vuint8) 0x01u)
#define kDiagRqlSecurityAccessKey                        ((vuint8) 0x03u)
#define kDiagRqlDisableNormalComm                        ((vuint8) 0x00u)
#define kDiagRqlRequestDownload                          ((vuint8) 0x01u + kDiagRqlRequestDownloadMemorySizeParameter)
#define kDiagRqlTransferData                             ((vuint8) 0x01u + kDiagRqlTransferDataStartingAddressParameter)
#define kDiagRqlWriteDataByIdentifier                    ((vuint8) 0x01u)
#define kDiagRqlTesterPresent                            ((vuint8) 0x00u)
#define kDiagRqlReportProgrammedState                    ((vuint8) 0x00u)
#define kDiagRqlProgrammingMode                          ((vuint8) 0x01u)

/* -- Defines for response length codes         -- */
/* -- (if applicable; excluding the Service-ID. -- */
#define kDiagRslInitiateDiagnosticOp                     ((vuint8) 0x00u)
#define kDiagRslReadDataByIdentifier                     ((vuint8) 0x01u)
#define kDiagRslReturnToNormalMode                       ((vuint8) 0x00u)
#define kDiagRslReadMemoryByAddress                      ((vuint8) 0x00u + kDiagRqlReadMemoryByAddressAddressParameter)
#define kDiagRslSecurityAccessSeed                       ((vuint8) 0x01u + kDiagSecSeedLength)
#define kDiagRslSecurityAccessKey                        ((vuint8) 0x01u)
#define kDiagRslDisableNormalComm                        ((vuint8) 0x00u)
#define kDiagRslRequestDownload                          ((vuint8) 0x00u)
#define kDiagRslTransferData                             ((vuint8) 0x00u)
#define kDiagRslWriteDataByIdentifier                    ((vuint8) 0x01u)
#define kDiagRslTesterPresent                            ((vuint8) 0x00u)
#define kDiagRslReportProgrammedState                    ((vuint8) 0x01u)
#define kDiagRslProgrammingMode                          ((vuint8) 0x00u)

/* -- Negative Response Codes -- */
#define kDiagErrorNone                                   ((vuint8) 0x00u)
#define kDiagNrcServiceNotSupported                      ((vuint8) 0x11u)
#define kDiagNrcSubFunctionNotSupportedInvalidFormat     ((vuint8) 0x12u)
#define kDiagNrcConditionsNotCorrectOrRequestSeqErr      ((vuint8) 0x22u)
#define kDiagNrcRequestOutOfRange                        ((vuint8) 0x31u)
#define kDiagNrcInvalidKey                               ((vuint8) 0x35u)
#define kDiagNrcExceedNumberOfAttempts                   ((vuint8) 0x36u)
#define kDiagNrcRequiredTimeDelayNotExpired              ((vuint8) 0x37u)
#define kDiagNrcRcrResponsePending                       ((vuint8) 0x78u)
#define kDiagNrcVoltageOutOfRange                        ((vuint8) 0x83u)
#define kDiagNrcGeneralProgFailure                       ((vuint8) 0x85u)
#define kDiagNrcDeviceTypeError                          ((vuint8) 0x89u)

/* -- Macros for diag exceptions -- */
#define DiagSetError(errorNo)                            (diagErrorCode = (errorNo)) /* PRQA S 3453 */ /* MD_CBD_19.7 */
#define DiagClrError()                                   (diagErrorCode = kDiagErrorNone)
#define DiagGetError()                                   (diagErrorCode)

#define DiagNRCServiceNotSupported()                     (DiagSetError(kDiagNrcServiceNotSupported))
#define DiagNRCSubFunctionNotSupportedInvalidFormat()    (DiagSetError(kDiagNrcSubFunctionNotSupportedInvalidFormat))
#define DiagNRCConditionsNotCorrectOrRequestSeqErr()     (DiagSetError(kDiagNrcConditionsNotCorrectOrRequestSeqErr))
#define DiagNRCRequestOutOfRange()                       (DiagSetError(kDiagNrcRequestOutOfRange))
#define DiagNRCInvalidKey()                              (DiagSetError(kDiagNrcInvalidKey))
#define DiagNRCExceedNumberOfAttempts()                  (DiagSetError(kDiagNrcExceedNumberOfAttempts))
#define DiagNRCRequiredTimeDelayNotExpired()             (DiagSetError(kDiagNrcRequiredTimeDelayNotExpired))
#define DiagNRCVoltageOutOfRange()                       (DiagSetError(kDiagNrcVoltageOutOfRange))
#define DiagNRCGeneralProgFailure()                      (DiagSetError(kDiagNrcGeneralProgFailure))
#define DiagNRCDeviceTypeError()                         (DiagSetError(kDiagNrcDeviceTypeError))

/* Defines for bootloader state flags */
#define kFblStateDiagIndication           ((vuint16) 0x0001u)
#define kFblStateLibMemProgrammingError   ((vuint16) 0x0002u)
/* State flags 1 and 2 reserved for OEM specific states */
#define kFblStateTransferDataAllowed      ((vuint16) 0x0008u)
#define kFblStateSecurityKey              ((vuint16) 0x0010u)
#define kFblStateSecurityAccess           ((vuint16) 0x0020u)
#define kFblStateFunctionalRequest        ((vuint16) 0x0040u)
#define kFblStateResponseProcessing       ((vuint16) 0x0080u)
#define kFblStateRcrRpInProgress          ((vuint16) 0x0800u)
#define kFblStateMemDriverInit            ((vuint16) 0x8000u)

/* Additional FBL States (GM) */
#define kFblStateEnableDiagResp           ((vuint16) 0x0002u)
#define kFblStateDisableComm              ((vuint16) 0x0004u)
#define kFblStateProgrammingMode          ((vuint16) 0x0100u)
#define kFblStateHighspeedMode            ((vuint16) 0x0200u)
#define kFblStateRequestPrgMode           ((vuint16) 0x1000u)
#define kFblStateDiagInProgress           ((vuint16) 0x4000u)

/* Flag indicating that transfer-data service (36) is allowed */
#define SetTransferDataAllowed()       (fblStates |= kFblStateTransferDataAllowed)
#define ClrTransferDataAllowed()       (fblStates &= FblInvert16Bit(kFblStateTransferDataAllowed))

/* Macros for state flag access */
#define GetDiagIndication()         ((fblStates & kFblStateDiagIndication) != (vuint16)0u)
#define GetEnablePrgMode()          ((fblStates & kFblStateProgrammingMode) != (vuint16)0u)
#define GetTransferDataAllowed()    ((fblStates & kFblStateTransferDataAllowed) != (vuint16)0u)
#define GetSecurityKeyAllowed()     ((fblStates & kFblStateSecurityKey) != (vuint16)0u)
#define GetSecurityUnlock()         ((fblStates & kFblStateSecurityAccess) != (vuint16)0u)
#define GetFunctionalRequest()      ((fblStates & kFblStateFunctionalRequest) != (vuint16)0u)
#define GetResponseProcessing()     ((fblStates & kFblStateResponseProcessing) != (vuint16)0u)
#define GetEnableDiagResp()         ((fblStates & kFblStateEnableDiagResp) != (vuint16)0u)
#define GetDisableComm()            ((fblStates & kFblStateDisableComm) != (vuint16)0u)
#define GetHighspeedMode()          ((fblStates & kFblStateHighspeedMode) != (vuint16)0u)
#define GetRequestPrgMode()         ((fblStates & kFblStateRequestPrgMode) != (vuint16)0u)
#define GetDiagInProgress()         ((fblStates & kFblStateDiagInProgress) != (vuint16)0u)
#define GetRcrRpInProgress()        ((fblStates & kFblStateRcrRpInProgress) != (vuint16)0u)
#define GetMemDriverInitialized()   ((fblStates & kFblStateMemDriverInit) != (vuint16)0u)

/* Flag indicating ECU has been unlocked (Security requests were successful) */
#define SetSecurityUnlock()            (fblStates |= kFblStateSecurityAccess)
#define ClrSecurityUnlock()            (fblStates &= (vuint16)~kFblStateSecurityAccess)

/* Diagnostics specification timing */
#if !defined (FBL_DIAG_TIME_P2MAX_RAW)
# define FBL_DIAG_TIME_P2MAX_RAW  75u
#endif
#if !defined (FBL_DIAG_TIME_P2EXT_RAW)
# define FBL_DIAG_TIME_P2EXT_RAW  5000u
#endif
#if !defined (FBL_DIAG_TIME_P2STAR_RAW)
# define FBL_DIAG_TIME_P2STAR_RAW 30000u
#endif

/* Send out 10ms before timer passed */
#define kFblDiagAdjustP2Max  0u
#define kP2TimerInitialRcRRPMargin (10u/FBL_REPEAT_CALL_CYCLE)
/* Adjust values to allow 1,5% timer inaccuracy */
#define kFblDiagAdjustP2Ext  75u
#define kFblDiagAdjustP2Star 450u

/* Timer values for response pending handling */
#define FBL_DIAG_TIME_P2MAX  ((vuint16)((FBL_DIAG_TIME_P2MAX_RAW - kFblDiagAdjustP2Max) / FBL_REPEAT_CALL_CYCLE))
#define FBL_DIAG_TIME_P2EXT  ((vuint16)((FBL_DIAG_TIME_P2EXT_RAW - kFblDiagAdjustP2Ext) / FBL_REPEAT_CALL_CYCLE))
#define FBL_DIAG_TIME_P2STAR ((vuint16)((FBL_DIAG_TIME_P2STAR_RAW - kFblDiagAdjustP2Star) / FBL_REPEAT_CALL_CYCLE))

#define FBL_DIAG_TIME_P3MAX  (GetEnablePrgMode() ? FBL_DIAG_TIME_P2STAR : FBL_DIAG_TIME_P2EXT)

/* Diagnostic service format definitions */
#define kDiagFmtServiceId                                ((vuint8) 0x00u)                      /* Position of service id */
#define kDiagFmtSubparam                                 ((vuint8) (kDiagFmtServiceId + 1))     /* Position of DID */

/** Negative Response Service code */
#define kDiagFmtNegResponse                              ((vuint8) (kDiagFmtSubparam + 1))      /* Position of negative response code */

/** InitiateDiagnosticOperation */
#define kDiagSubWakeUpLinks                              ((vuint8)0x04u)

/* ReadMemoryByAddress */
#define kDiagReadMemoryByAddrAddr                        ((vuint8)(kDiagFmtServiceId + 1))
#define kDiagReadMemoryByAddrSize                        ((vuint8)(kDiagReadMemoryByAddrAddr + kDiagRqlReadMemoryByAddressAddressParameter))
#define kDiagRqlReadMemoryByAddressSizeParameter         ((vuint8)2u)
#define kDiagReadMemoryByAddrData                        ((vuint8)kDiagReadMemoryByAddrSize)  /* Attention: Data overwrites size parameter */

/* SecurityAccess */
#define kDiagFmtSeedKeyStart                             ((vuint8)(kDiagFmtSubparam + 0x01u))
#define kDiagSubSecTypeMask                              ((vuint8)0xFFu)
#define kDiagSubRequestSeed                              ((vuint8)0x01u)
#define kDiagSubSendKey                                  ((vuint8)0x02u)

#define kDiagSecSeedLength                               ((vuint8) kDiagRqlSeed)

/* ReadDataByIdentifier / WriteDataByIdentifier */
#define kDiagFmtDid                                      ((vuint8) (kDiagFmtServiceId + 1))

#define kDiagDidECUOperatingStatus                       ((vuint8)0x79u)
#define kDiagDidReadLastError                            ((vuint8)0x7Fu)

/* Standard GM Corporate Data-Identifiers (DIDs) */
/* The FBL will typically use only a subset of these */
#define kDiagDidVehicleIdentification                    ((vuint8)0x90u)
#define kDiagDidSystemSupplierID                         ((vuint8)0x92u)
#define kDiagDidSystemNameEngineType                     ((vuint8)0x97u)
#define kDiagDidRepairShopCode                           ((vuint8)0x98u)
#define kDiagDidProgrammingDate                          ((vuint8)0x99u)
#define kDiagDiagnosticDataId                            ((vuint8)0x9Au)
#define kDiagDidManufEnableCounter                       ((vuint8)0xA0u)
#define kDiagECUConfig1                                  ((vuint8)0xA1u)
#define kDiagECUConfig2                                  ((vuint8)0xA2u)
#define kDiagECUConfig3                                  ((vuint8)0xA3u)
#define kDiagECUConfig4                                  ((vuint8)0xA4u)
#define kDiagECUConfig5                                  ((vuint8)0xA5u)
#define kDiagECUConfig6                                  ((vuint8)0xA6u)
#define kDiagECUConfig7                                  ((vuint8)0xA7u)
#define kDiagECUConfig8                                  ((vuint8)0xA8u)
#define kDiagDidECUDiagnosticAddr                        ((vuint8)0xB0u)
#define kDiagDidECUFuncSystem                            ((vuint8)0xB1u)
#define kDiagDidManufData                                ((vuint8)0xB2u)
#define kDiagDidManufTraceabilityChars                   ((vuint8)0xB4u)
#define kDiagDidGmBroadcastCode                          ((vuint8)0xB5u)
#define kDiagDidTargetVehicle                            ((vuint8)0xB6u)
#define kDiagDidSoftwareUsage                            ((vuint8)0xB7u)
#define kDiagDidBenchVerification                        ((vuint8)0xB8u)
#define kDiagDidSubnetConfigListHS                       ((vuint8)0xB9u)
#define kDiagDidSubnetConfigListLS                       ((vuint8)0xBAu)
#define kDiagDidSubnetConfigListMS                       ((vuint8)0xBBu)
#define kDiagDidSubnetConfigListNC1                      ((vuint8)0xBCu)
#define kDiagDidSubnetConfigListNC2                      ((vuint8)0xBDu)
#define kDiagDidSubnetConfigListNC3                      ((vuint8)0xBEu)
#define kDiagDidBootSoftwarePartNr                       ((vuint8)0xC0u)
#define kDiagDidSwmi01                                   ((vuint8)0xC1u)
#define kDiagDidSwmi02                                   ((vuint8)0xC2u)
#define kDiagDidSwmi03                                   ((vuint8)0xC3u)
#define kDiagDidSwmi04                                   ((vuint8)0xC4u)
#define kDiagDidSwmi05                                   ((vuint8)0xC5u)
#define kDiagDidSwmi06                                   ((vuint8)0xC6u)
#define kDiagDidSwmi07                                   ((vuint8)0xC7u)
#define kDiagDidSwmi08                                   ((vuint8)0xC8u)
#define kDiagDidSwmi09                                   ((vuint8)0xC9u)
#define kDiagDidSwmi10                                   ((vuint8)0xCAu)
#define kDiagDidEndModelPartNr                           ((vuint8)0xCBu)
#define kDiagDidBaseModelPartNr                          ((vuint8)0xCCu)
#define kDiagDidBootSoftwareAlpha                        ((vuint8)0xD0u)
#define kDiagDidAlpha01                                  ((vuint8)0xD1u)
#define kDiagDidAlpha02                                  ((vuint8)0xD2u)
#define kDiagDidAlpha03                                  ((vuint8)0xD3u)
#define kDiagDidAlpha04                                  ((vuint8)0xD4u)
#define kDiagDidAlpha05                                  ((vuint8)0xD5u)
#define kDiagDidAlpha06                                  ((vuint8)0xD6u)
#define kDiagDidAlpha07                                  ((vuint8)0xD7u)
#define kDiagDidAlpha08                                  ((vuint8)0xD8u)
#define kDiagDidAlpha09                                  ((vuint8)0xD9u)
#define kDiagDidAlpha10                                  ((vuint8)0xDAu)
#define kDiagDidEndModelPartNrAlpha                      ((vuint8)0xDBu)
#define kDiagDidBaseModelPartNrAlpha                     ((vuint8)0xDCu)
#define kDiagDidSoftwareModuleDataIds                    ((vuint8)0xDDu)
#define kDiagDidSerialDataDatabaseRev                    ((vuint8)0xDEu)
#define kDiagDidVehicleLevelData1                        ((vuint8)0xE0u)
#define kDiagDidVehicleLevelData2                        ((vuint8)0xE1u)
#define kDiagDidVehicleLevelData3                        ((vuint8)0xE2u)
#define kDiagDidVehicleLevelData4                        ((vuint8)0xE3u)
#define kDiagDidVehicleLevelData5                        ((vuint8)0xE4u)
#define kDiagDidVehicleLevelData6                        ((vuint8)0xE5u)
#define kDiagDidVehicleLevelData7                        ((vuint8)0xE6u)
#define kDiagDidVehicleLevelData8                        ((vuint8)0xE7u)

/* New DIDs as per GMW3110 1.6 Addendum */
#define kDiagDidProgrammedStateIndicator                 ((vuint8)0xF0u)
#define kDiagDidProgramingErrorCode                      ((vuint8)0xF1u)
#define kDiagDidBootInitializationStatus                 ((vuint8)0xF2u)
#define kDiagDidEcuId                                    ((vuint8)0xF3u)
#define kDiagDidBInfoSubjandECUName                      ((vuint8)0xF6u)

/* RequestDownload */
#define kDiagFmtReqDlDataFormatId                        ((vuint8)(kDiagFmtServiceId + 1))
#define kDiagFmtReqDlMemSizeOffset                       ((vuint8)(kDiagFmtReqDlDataFormatId + 1))

/* TransferData */
#define kDiagFmtAddrOffset                               ((vuint8)(kDiagFmtSubparam + 1))
#define kDiagFmtDataOffset                               ((vuint8)(kDiagFmtAddrOffset + kDiagRqlTransferDataStartingAddressParameter))

#define kDiagSubDownload                                 ((vuint8)0x00u)
#define kDiagSubDownloadAndExecute                       ((vuint8)0x80u)

/* ReportProgrammedState */
#define kDiagProgStateFullyProgrammed                    ((vuint8)0x00u)
#define kDiagProgStateNoSoftwareOrCal                    ((vuint8)0x01u)
#define kDiagProgStateNoCalibration                      ((vuint8)0x02u)
#define kDiagProgStateDefOrNoStartCal                    ((vuint8)0x03u)
#define kDiagProgStateGeneralMemFault                    ((vuint8)0x50u)
#define kDiagProgStateRamFault                           ((vuint8)0x51u)
#define kDiagProgStateNvramFault                         ((vuint8)0x52u)
#define kDiagProgStateBootFault                          ((vuint8)0x53u)
#define kDiagProgStateFlashFault                         ((vuint8)0x54u)
#define kDiagProgStateEepromFault                        ((vuint8)0x55u)

/* ProgrammingMode */
#define kDiagSubRequestProgMode                          ((vuint8)0x01u)
#define kDiagSubRequestProgModeHSpeed                    ((vuint8)0x02u)
#define kDiagSubEnableProgrammingMode                    ((vuint8)0x03u)

/* Error codes for error status reporting */
#define kDiagErrNone                                     ((vuint8)0x00u)

/***********************************************************************************************************************
 *  FblHdr Callbacks
 **********************************************************************************************************************/
#define  FblDiagSetErrLengthExceeded()                   DiagNRCRequestOutOfRange()
#define  FblDiagSetErrEndIndication()                    DiagNRCRequestOutOfRange()
#define  FblDiagSetErrSwRegion()                         DiagNRCRequestOutOfRange()
#define  FblDiagSetErrModuleLength()                     DiagNRCRequestOutOfRange()
#define  FblDiagSetFirstTransferTooSmall()               DiagNRCSubFunctionNotSupportedInvalidFormat()

#define  FblDiagDoCompleteTransfer() ClrTransferDataAllowed()

#if defined( FBL_ENABLE_DEBUG_STATUS )
#define kDiagErrDataEncryption                           ((vuint16)0x0D00u)
#define kDiagErrCodeNotAligned                           ((vuint16)0x0F00u)
#define kDiagErrNoMemoryRegionFound                      ((vuint16)0x1000u)
#define kDiagErrFlashcodeInitFailed                      ((vuint16)0x1100u)
#define kDiagErrDownloadInProgress                       ((vuint16)0x1500u)
#define kDiagErrFillFailed                               ((vuint16)0x1900u)
#define kDiagErrValueOutOfRange                          ((vuint16)0x1B00u)
#define kDiagErrTPFailed                                 ((vuint16)0x1D00u)

/* incompatible to previous SLPs */
#define kDiagErrP1DataTypeNotPresent                     ((vuint16)0x4000u)
#define kDiagErrP1AppNBIDInconsistent                    ((vuint16)0x4100u)
#define kDiagErrP1SegNbrInconsistent                     ((vuint16)0x4200u)
#define kDiagErrP1SegRangeInconsistent                   ((vuint16)0x4300u)
#define kDiagErrP1PartitionInfoFailure                   ((vuint16)0x4400u)
#define kDiagErrNbidFailed                               ((vuint16)0x4500u)
#define kDiagErrTransferTooSmall                         ((vuint16)0x4600u)
#define kDiagErrSecurityLocked                           ((vuint16)0x4700u)
#define kDiagErrProgrammingModeDisabled                  ((vuint16)0x4800u)

/* Macros for error status reporting */
# define FblErrStatSetError(error)           (errStatErrorCode = (error)) /* PRQA S 3453 */ /* MD_CBD_19.7 */
# define FblErrStatSetSId(id)                (errStatLastServiceId = (id)) /* PRQA S 3453 */ /* MD_CBD_19.7 */
# define FblErrStatSetState(state)           (errStatFblStates = (state)) /* PRQA S 3453 */ /* MD_CBD_19.7 */
# define FblErrStatSetFlashDrvError(error)   (errStatFlashDrvErrorCode = (error)) /* PRQA S 3453 */ /* MD_CBD_19.7 */

# if defined( __FILE__ ) && defined( __LINE__ )
#  define FblErrDebugStatus(error)            {\
                                                 FblErrDebug((error), (tDebugFileName) __FILE__, __LINE__);\
                                              } /* PRQA S 3458 */ /* MD_CBD_19.4 */
# else
#  define FblErrDebugStatus(error)            {\
	                                             FblErrDebug((error), 0, 0);\
                                              } /* PRQA S 3458 */ /* MD_CBD_19.4 */
# endif

#else
# define FblErrStatSetError(error)
# define FblErrStatSetSId(id)
# define FblErrStatSetState(state)
# define FblErrStatSetFlashDrvError(error)
# define FblErrDebugStatus(error)
# define FblErrDebugDriver(addr, error)
#endif

#define kFblSleepDelayTime       ((vuint32)(60000u / DIAG_CALL_CYCLE)) /* Wait 60s before entering sleep mode */

#define FblSleepCounterClear()   (sleepCounter = 0x00u)
#define FblSleepCounterReload()  (sleepCounter = kFblSleepDelayTime)

/* Parameter to the function FblDiagEcuReset() */
#define kDiagResetPutResponse                ((vuint8)0x00u)
#define kDiagResetNoResponse                 ((vuint8)0x01u)

/* Parameters for busy response handling function */
#define kNotForceSendResponsePending         ((vuint8)0x00u)
#define kForceSendResponsePending            ((vuint8)0x01u)

#define DiagProcessingDone(len)              DiagResponseProcessor(len) /* PRQA S 3453 */ /* MD_CBD_19.7 */
#define DiagSetNoResponse()                  (diagResponseFlag = 0u)
#define DiagSetForceResponse()               (diagResponseFlag = kDiagPutResponse)

/* Bit-masks for fblProgrammedState */
#define kProgrammedStateRomError          ((vuint8)0x01u)
#define kProgrammedStateRamError          ((vuint8)0x02u)
#define kProgrammedStateUnderrunError     ((vuint8)0x04u)
#define kProgrammedStateAppPresent        ((vuint8)0x08u)

#define kFblDiagMemSegmNotFound             ((vsint16) -1)

/* Assertion error codes  */
#define kFblAssertCorrectGenyConfig                ((vuint8)0x08u)

#define kDiagSubNoDataProcessing             ((vuint8) 0x00)

#define FblIsAddrAlignedToBinVal(address, segSize) (( (address) & ( (segSize) - 1)) == 0) /* PRQA S 3453 */ /* MD_CBD_19.7 */
#define GetBytesToNextAlignedAddress(address, segSize)  ((!FblIsAddrAlignedToBinVal(address, segSize)) ? ((segSize) - (((address) & ((segSize) - 1)))) : 0) /* PRQA S 3453 */ /* MD_CBD_19.7 */

/* compatibility define / remapping */
#define FblGetInteger  FblMemGetInteger

#if !defined( kDiagSigBufLength )
# define kDiagSigBufLength 0x01u
#endif

/* Polling of CAN receive messages */
#define FBL_WD_STATE_TASK()\
                                /* Check for fblState mask kFblStateTransferDataAllowed && !kFblStateDiagInProgress */\
                                if ((fblStates & (kFblStateTransferDataAllowed|kFblStateDiagInProgress)) == kFblStateTransferDataAllowed)\
                                {\
                                       if (0 != FblCanReceive())\
                                       {\
                                          FblCanProcessRxMsg();\
                                       }\
                                }

/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/

/* The function pointer type memory qualifiers have to be similar to tExportFct */
typedef MEMORY_FAR SecM_StatusType  (*tVerificationFct) ( SecM_VerifyParamType *verifyParam );
#if defined (FBL_ENABLE_DEBUG_STATUS)
typedef int tDebugLineNbr;
typedef V_MEMROM1 vuint8 V_MEMROM2* tDebugFileName;
#endif

/***********************************************************************************************************************
 *  PROTOTYPES
 **********************************************************************************************************************/

#if defined (FBL_ENABLE_DEBUG_STATUS)
void FblErrDebug( vuint16 error, tDebugFileName fileName, tDebugLineNbr lineNumber );
void FblErrDebugDriver( tFblAddress addr, vuint16 error );
#endif

#if defined( MULTIPLE_RECEIVE_BUFFER )
void FblDiagPrecopy( pChipDataPtr data );
#endif
void FblDiagInit( void );
void FblDiagInitKey (void);
void FblDiagInitStartFromAppl( void );
void FblDiagTimerTask(void);
void FblDataInd( tTpDataType rxDataLen );
# if defined( MULTIPLE_RECEIVE_BUFFER )
void FblHandleRxMsg( vuint8 fblRxCanMsgHdl, pChipDataPtr canDataPtr );
# endif
#if defined( MULTIPLE_RECEIVE_BUFFER )
vuint8 FblCheckOwnStartMsg( pChipDataPtr data );
#endif
void DiagExRCRResponsePending( vuint8 forceSend );
void DiagResponseProcessor( tTpDataType dataLength );
vuint8 FblRealTimeSupport( void );

tFblLength FblReadProm( tFblAddress address, vuint8* buffer, tFblLength length );

vsint16 FblMemSegmentNrGet( tFblAddress address );

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

/* Near globals */
V_MEMRAM0 extern V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3* V_MEMRAM1_NEAR V_MEMRAM2_NEAR DiagBuffer;
V_MEMRAM0 extern V_MEMRAM1_NEAR vuint8 V_MEMRAM2_NEAR diagErrorCode;
V_MEMRAM0 extern V_MEMRAM1_NEAR tTpDataType V_MEMRAM2_NEAR DiagDataLength;
V_MEMRAM0 extern V_MEMRAM1_NEAR vuint16 V_MEMRAM2_NEAR fblStates;
V_MEMRAM0 extern V_MEMRAM1_NEAR vuint8 V_MEMRAM2_NEAR flashState;
V_MEMRAM0 extern V_MEMRAM1_NEAR vuint16 V_MEMRAM2_NEAR testerPresentTimeout;
V_MEMRAM0 extern V_MEMRAM1_NEAR vuint8 V_MEMRAM2_NEAR diagResponseFlag;
V_MEMRAM0 extern V_MEMRAM1_NEAR vsint16 V_MEMRAM2_NEAR memSegment;
V_MEMRAM0 extern V_MEMRAM1_NEAR vuint8 V_MEMRAM2_NEAR diagServiceCurrent;

#if defined( FBL_ENABLE_DEBUG_STATUS )
/* Variables for error status reporting */
V_MEMRAM0 extern V_MEMRAM1 vuint16 V_MEMRAM2 errStatFblStates;
V_MEMRAM0 extern V_MEMRAM1 vuint8 V_MEMRAM2 errStatLastServiceId;
V_MEMRAM0 extern V_MEMRAM1 vuint8 V_MEMRAM2 errStatFlashDrvVersion[3];
V_MEMRAM0 extern V_MEMRAM1 vuint16 V_MEMRAM2 errStatFlashDrvErrorCode;
V_MEMRAM0 extern V_MEMRAM1 vuint16 V_MEMRAM2 errStatDescriptor;
V_MEMRAM0 extern V_MEMRAM1 vuint16 V_MEMRAM2 errStatErrorCode;

V_MEMRAM0 extern V_MEMRAM1 vuint8 V_MEMRAM2 errStatTpError;
V_MEMRAM0 extern V_MEMRAM1 vuint8 V_MEMRAM2 errStatHaveDriver;
V_MEMRAM0 extern V_MEMRAM1 tFblAddress V_MEMRAM2 errStatAddress;
V_MEMRAM0 extern V_MEMRAM1 tDebugFileName V_MEMRAM2 errStatFileName;
V_MEMRAM0 extern V_MEMRAM1 tDebugLineNbr V_MEMRAM2 errStatLineNumber;
#endif

V_MEMRAM0 extern V_MEMRAM1_NEAR tMsgObject V_MEMRAM2_NEAR fblCanTxObj[kNrOfTxObjects];

V_MEMRAM0 extern V_MEMRAM1 vuint8 V_MEMRAM2 diagNodeAddr;
V_MEMRAM0 extern V_MEMRAM1 vuint32 V_MEMRAM2 sleepCounter;
V_MEMRAM0 extern V_MEMRAM1 vuint8 V_MEMRAM2 fblProgrammedState;
#if defined( FBL_ENABLE_MULTIPLE_NODES )
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2 kFblDiagNodeAddr[];
#endif
V_MEMRAM0 extern V_MEMRAM1 tFblAddress V_MEMRAM2 progErrorAddr;
V_MEMRAM0 extern V_MEMRAM1 vuint16 V_MEMRAM2 idxToSignature; /* used for setting presence pattern */

/* Plausibility checks of configuration settings *****************************/
#if ((!defined( FBL_WATCHDOG_OFF ) && !defined( FBL_WATCHDOG_ON )) || \
     (defined( FBL_WATCHDOG_OFF ) && defined( FBL_WATCHDOG_ON )))
# error "Error in fbl_cfg.h: Missing or incorrect usage of watchdog"
#endif

#if ((!defined( FBL_ENABLE_DEBUG_STATUS ) && !defined( FBL_DISABLE_DEBUG_STATUS )) || \
     (defined( FBL_ENABLE_DEBUG_STATUS ) && defined( FBL_DISABLE_DEBUG_STATUS )))
# error "Error in fbl_cfg.h: Missing or incorrect usage of debug-status"
#endif

#if ((!defined( FBL_ENABLE_SYSTEM_CHECK ) && !defined( FBL_DISABLE_SYSTEM_CHECK )) || \
     (defined( FBL_ENABLE_SYSTEM_CHECK ) && defined( FBL_DISABLE_SYSTEM_CHECK )))
# error "Error in fbl_cfg.h: Missing or incorrect usage of system-check"
#endif

#if ((!defined( FBL_ENABLE_HIGHSPEED_MODE ) && !defined( FBL_DISABLE_HIGHSPEED_MODE )) || \
     (defined( FBL_ENABLE_HIGHSPEED_MODE ) && defined( FBL_DISABLE_HIGHSPEED_MODE )))
# error "Error in fbl_cfg.h: Missing or incorrect usage of highspeed-mode"
#endif

#if ((!defined( FBL_ENABLE_APPL_TASK ) && !defined( FBL_DISABLE_APPL_TASK ))||\
     (defined( FBL_ENABLE_APPL_TASK ) && defined( FBL_DISABLE_APPL_TASK )))
# error "Error in fbl_cfg.h: Missing or incorrect usage of appl-task"
#endif

#if ((!defined( FBL_ENABLE_INITIATE_DIAG_OP ) && !defined( FBL_DISABLE_INITIATE_DIAG_OP )) || \
     (defined( FBL_ENABLE_INITIATE_DIAG_OP ) && defined( FBL_DISABLE_INITIATE_DIAG_OP )))
# error "Error in fbl_cfg.h: Missing or incorrect usage of initiate-diagnostic-operation"
#endif

#if ( (!defined( FBL_ENABLE_WRITE_DATA_BY_IDENTIFIER ) && !defined( FBL_DISABLE_WRITE_DATA_BY_IDENTIFIER )) || \
      (defined( FBL_ENABLE_WRITE_DATA_BY_IDENTIFIER ) && defined( FBL_DISABLE_WRITE_DATA_BY_IDENTIFIER )))
# error "Error in fbl_cfg.h: Missing or incorrect usage of write-data-by-identifier"
#endif

#if ((!defined( FBL_ENABLE_READ_MEMORY_BY_ADDRESS ) && !defined( FBL_DISABLE_READ_MEMORY_BY_ADDRESS )) || \
     (defined( FBL_ENABLE_READ_MEMORY_BY_ADDRESS ) && defined( FBL_DISABLE_READ_MEMORY_BY_ADDRESS )))
# error "Error in fbl_cfg.h: Missing or incorrect usage of read-memory-by-address"
#endif

#if ((!defined( FBL_ENABLE_STAY_IN_BOOT ) && !defined( FBL_DISABLE_STAY_IN_BOOT )) || \
     (defined( FBL_ENABLE_STAY_IN_BOOT ) && defined( FBL_DISABLE_STAY_IN_BOOT )))
# error "Error in fbl_cfg.h: Missing or incorrect usage of stay-in-boot"
#endif

#if ((!defined( FBL_ENABLE_SLEEPMODE ) && !defined( FBL_DISABLE_SLEEPMODE )) || \
     (defined( FBL_ENABLE_SLEEPMODE ) && defined( FBL_DISABLE_SLEEPMODE )))
# error "Error in fbl_cfg.h: Missing or incorrect usage of sleep-mode"
#endif

#if ((!defined( FBL_ENABLE_ROM_INTEGRITY_CHECK ) && !defined( FBL_DISABLE_ROM_INTEGRITY_CHECK )) || \
      (defined( FBL_ENABLE_ROM_INTEGRITY_CHECK ) && defined( FBL_DISABLE_ROM_INTEGRITY_CHECK )))
# error "Error in fbl_cfg.h: Missing or incorrect usage of rom-integrity-check"
#endif

#if ((!defined( FBL_ENABLE_RAM_INTEGRITY_CHECK ) && !defined( FBL_DISABLE_RAM_INTEGRITY_CHECK )) || \
      (defined( FBL_ENABLE_RAM_INTEGRITY_CHECK ) && defined( FBL_DISABLE_RAM_INTEGRITY_CHECK )))
# error "Error in fbl_cfg.h: Missing or incorrect usage of ram-integrity-check"
#endif

#if ((!defined( FBL_ENABLE_SECURITY_ACCESS ) && !defined( FBL_DISABLE_SECURITY_ACCESS )) || \
     (defined( FBL_ENABLE_SECURITY_ACCESS ) && defined( FBL_DISABLE_SECURITY_ACCESS )))
# error "Error in fbl_cfg.h: Missing or incorrect usage of security-access"
#endif

#if ((!defined( FBL_ENABLE_MULTIPLE_NODES ) && !defined( FBL_DISABLE_MULTIPLE_NODES )) || \
     (defined( FBL_ENABLE_MULTIPLE_NODES ) && defined( FBL_DISABLE_MULTIPLE_NODES )))
# error "Error in fbl_cfg.h: Missing or incorrect usage of multiple-nodes"
#endif

#if ((!defined( FBL_ENABLE_CAN_CONFIGURATION ) && !defined( FBL_DISABLE_CAN_CONFIGURATION )) || \
     (defined( FBL_ENABLE_CAN_CONFIGURATION ) && defined( FBL_DISABLE_CAN_CONFIGURATION )))
# error "Error in fbl_cfg.h: Missing or incorrect usage of can-configuration"
#endif

#if ((!defined( FBL_ENABLE_MULTIPLE_MEM_DEVICES ) && !defined( FBL_DISABLE_MULTIPLE_MEM_DEVICES )) || \
      (defined( FBL_ENABLE_MULTIPLE_MEM_DEVICES ) && defined( FBL_DISABLE_MULTIPLE_MEM_DEVICES )))
# error "Error in fbl_cfg.h: Missing or incorrect usage of multiple-memory-devices"
#endif

#if ((!defined( FBL_ENABLE_GAP_FILL ) && !defined( FBL_DISABLE_GAP_FILL )) || \
     (defined( FBL_ENABLE_GAP_FILL ) && defined( FBL_DISABLE_GAP_FILL )))
# error "Error in fbl_cfg.h: Missing or incorrect usage of gap-fill"
#endif

#if !defined( FBL_ENABLE_FBL_START ) && !defined( FBL_DISABLE_FBL_START )
# error "Error in fbl_cfg.h: FBL-Start must be specified"
#endif

#if !defined( FBL_WATCHDOG_TIME )
# error "Error in fbl_cfg.h: Usage of fbl-watchdog-time not specified"
#endif

#if !defined( SWM_DATA_MAX_NOAR )
# error "Error in fbl_cfg.h: Usage of swm-data-max-noar not specified"
#endif

#if (SWM_DATA_MAX_NOAR < 2u) || (SWM_DATA_MAX_NOAR > 255u)
# error "Error in fbl_cfg.h: Unsupported value of SWM_DATA_MAX_NOAR"
#endif

#if !defined( FBL_DIAG_BUFFER_LENGTH )
# error "Error in fbl_cfg.h: Usage of fbl-diag-buffer-length not specified"
#endif

#if !defined( FLASH_SIZE )
# error "Error in fbl_cfg.h: Usage of flash-size not specified"
#endif

#if (FLASH_SIZE < 1u)
# error "Error in fbl_cfg.h: Unsupported value of FLASH_SIZE"
#endif

#if (FBL_DIAG_BUFFER_LENGTH > 4095u)
# error "Error in fbl_cfg.h: Unsupported value of FBL_DIAG_BUFFER_LENGTH"
#endif

#if (FBL_PROJECT_STATE == FBL_PRODUCTION)
# if defined( FBL_ENABLE_STAY_IN_BOOT )
#  error "Error in fbl_cfg.h: Stay-in-boot feature needs to be disabled for production."
# endif
#endif /* (FBL_PROJECT_STATE == FBL_PRODUCTION) */

/* This size is required in order to hold complete application header in first transfer data. */
# if (FBL_DIAG_BUFFER_LENGTH < (HDR_MODULE_MAX_LEN) )
 /* If this error occurs, you may decrease the SWM_MAX_NOAR value in GENy or you can increase the DiagBuffer size configuration. */
#  error "Diagnostic buffer size is not sufficient. Change your GENy configuration."
# endif

/* Diagnostic buffer size configuration is required to be equal to Tp buffer size configuration. */
#if (FBL_DIAG_BUFFER_LENGTH != kFblTpBufferSize)
# error "Diagnostic and Tp buffer size configuration do not match."
#endif

#if !( defined( FBL_ENABLE_MULTI_PROCESSOR )) && ( FBL_MTAB_NO_OF_BLOCKS > 1u)
# error "Multiple logical block table entries only allowed in multiple application support configuration.\
         Please do not configure calibration modules in the Logical Block Table."
#endif

#endif /* __FBL_DIAG_H__ */
/***********************************************************************************************************************
 *  END OF FILE: FBL_DIAG.H
 **********************************************************************************************************************/

