/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        OEM dependent configuration for FblLib_Mem
 *
 *  --------------------------------------------------------------------------------------------------------------------
 *  COPYRIGHT
 *  --------------------------------------------------------------------------------------------------------------------
 *  \par Copyright
 *  \verbatim
 *  Copyright (c) 2014 by Vector Informatik GmbH.                                             All rights reserved.
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
 *  Joern Herwig                  JHg           Vector Informatik GmbH
 *  Achim Strobelt                Ach           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  TEMPLATE REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  01.00.00   2012-03-23  JHg     -                Initial release
 *  01.00.01   2012-03-26  JHg     -                Additional comments
 *  01.01.00   2012-04-05  JHg     -                Changes after code review
 *  01.02.00   2012-04-27  JHg     -                Updated type casts in macros using ternary operator
 *  01.04.00   2013-02-01  Ach     ESCAN00062919    Adapted comments to use Doxygen
 *                         Ach     -                Correct macro __ApplFblMemDriverReady
 *                         JHg     ESCAN00064292    Added pre and post handlers for erase routine
 *                         JHg     ESCAN00064296    Renamed default preamble length define
 *                         JHg     ESCAN00064301    Split verify functions for RAM and ROM verification
 *                         JHg     ESCAN00064333    Differentiate watchdog trigger w/ and w/o status
 *                         JHg     ESCAN00064334    Added interface to remap error codes to OEM dependent value
 *                         JHg     -                Added additional error codes for new functionality
 *                         JHg     ESCAN00064720    Replaced __ApplFblMemCheckDfi by __ApplFblMemIsDataProcessingRequired
 *  01.05.00   2013-04-04  JHg     ESCAN00066379    Added interface version compatibility check
 *                         JHg     ESCAN00066377    Added additional error codes for new functionality
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  05.00.00   2014-01-31  AWh     -               Release
 *  05.01.00   2014-01-31  AWh     -               Added configuration checks, update Misra justifications.
 **********************************************************************************************************************/

#ifndef __FBL_MEM_OEM_H__
#define __FBL_MEM_OEM_H__

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/* PRQA S 3458 TAG_FblMem_3458_1 */ /* MD_CBD_19.4 */
/* PRQA S 3453 TAG_FblMem_3453_1 */ /* MD_CBD_19.7 */

/**-- Version information ----------------------------------------------------*/

/* --- Version --- */
/* ##V_CFG_MANAGEMENT ##CQProject : FblLib_Mem_Gm CQComponent : Implementation */
#define FBLLIB_MEM_GM_VERSION                0x0501u
#define FBLLIB_MEM_GM_RELEASE_VERSION        0x00u

/* Reference interface version */
/** Major interface version identifies incompatible changes */
#define FBL_MEM_API_REFERENCE_VERSION_MAJOR     0x02u
/** Minor interface version identifies backward compatible changes */
#define FBL_MEM_API_REFERENCE_VERSION_MINOR     0x01u
/** Release interface version identifies cosmetic changes */
#define FBL_MEM_API_REFERENCE_VERSION_RELEASE   0x00u

/**-- Buffer configuration ---------------------------------------------------*/

/** Input buffer size */
#define FBL_MEM_BUFFER_SIZE               FBL_DIAG_BUFFER_LENGTH
/** Length of default preamble */
#define FBL_MEM_DEFAULT_PREAMBLE_LENGTH   6u
/** Maximum preamble length */
#define FBL_MEM_MAX_PREAMBLE_LENGTH       6u

/** Check whether specific data format identifier implies the need for data processing */
#define __ApplFblMemIsDataProcessingRequired(dataFormat)    ((tFblResult)((kDiagSubNoDataProcessing == (dataFormat)) ? kFblFailed : kFblOk))

/**-- On-the-fly verification ------------------------------------------------*/
#if defined( SEC_VER_DISABLE_LENGTH_AND_ADDRESS_INPUT )
#else
/** Include address and length info in on-the-fly calculation */
# define FBL_MEM_ENABLE_ADDRESS_LENGTH_CHECKSUM
#endif
#if !defined( FBL_MEM_DISABLE_VERIFY_PIPELINED )
  /* The Gm specifications default pipelined verification */
# define FBL_MEM_ENABLE_VERIFY_PIPELINED
#endif

/** Size of temporary verification result buffer */
#define FBL_MEM_SIG_RESULT_BUFFER_SIZE                kDiagSigBufLength

/*-- Watchdog ---------------------------------------------------------------*/
/* Default return code */
#define FBL_MEM_WD_TRIGGER_DEFAULT                 (FBL_NO_TRIGGER)
/*
   Default watchdog trigger
   Remove macro if not applicable
*/
#define __ApplFblMemWdTrigger()                    FblLookForWatchdog()
/*
   Watchdog trigger including adaptive generation of RCR-RP
   Remove macro if not applicable
*/
#define __ApplFblMemAdaptiveRcrRp()                FblRealTimeSupport()
/*
   Check whether RCR-RP is already active
   Remove macro if not applicable or required (e.g. if kForceSendRpIfNotInProgress is available)
*/
#define __ApplFblMemIsRcrRpActive()                ((tFblResult)(GetRcrRpInProgress() ? kFblOk : kFblFailed))
/*
   Forced generation of RCR-RP (hint: use kForceSendRpIfNotInProgress if available)
   Remove macro if not applicable
*/
#define __ApplFblMemForcedRcrRp()                  DiagExRCRResponsePending(kForceSendResponsePending)

/* Pass watchdog trigger with extended status result to data processing */
#define FBL_MEM_ENABLE_EXT_TRIGGER_DATA_PROC
/* Pass watchdog trigger with extended status result to signature verification over input data */
#define FBL_MEM_DISABLE_EXT_TRIGGER_INPUT_VERIFY
/* Pass watchdog trigger with extended status result to signature verification over output data */
#define FBL_MEM_DISABLE_EXT_TRIGGER_OUTPUT_VERIFY

/* Allow for multi segments in Diagbuffer */
#define FBL_MEM_ENABLE_SEGMENTED_INPUT_BUFFER
#define FBL_ENABLE_TRANSFER_VERIFICATION_CHECK

/*-- Error handling ---------------------------------------------------------*/
/** Overwrite error codes by OEM dependent value */
#define FBL_MEM_ENABLE_STATUS_OVERWRITE
/* Enable interface to remap error codes to OEM dependent value */
#define FBL_MEM_DISABLE_STATUS_REMAPPING
/* OEM dependent status type */
#define FBL_MEM_STATUS_TYPE                           vuint8

#if defined( FBL_MEM_ENABLE_STATUS_OVERWRITE ) 
/* Default */
# define FBL_MEM_STATUS_Ok                            kDiagErrorNone
# define FBL_MEM_STATUS_Failed                        kDiagNrcConditionsNotCorrectOrRequestSeqErr
/* BlockErase indication */
# define FBL_MEM_STATUS_BlockEraseSequence            kDiagNrcConditionsNotCorrectOrRequestSeqErr
/* BlockStart indication */
# define FBL_MEM_STATUS_BlockStartSequence            kDiagNrcConditionsNotCorrectOrRequestSeqErr
# define FBL_MEM_STATUS_BlockStartParam               kDiagNrcConditionsNotCorrectOrRequestSeqErr
# define FBL_MEM_STATUS_BlockStartVerifyInit          kDiagNrcConditionsNotCorrectOrRequestSeqErr
/* BlockEnd indication */
# define FBL_MEM_STATUS_BlockEndSequence              kDiagNrcConditionsNotCorrectOrRequestSeqErr
# define FBL_MEM_STATUS_BlockEndVerifyFinalize        kDiagNrcGeneralProgFailure
/* BlockVerify */
# define FBL_MEM_STATUS_BlockVerifySequence           kDiagNrcConditionsNotCorrectOrRequestSeqErr
# define FBL_MEM_STATUS_BlockVerifyInputVerify        kDiagNrcGeneralProgFailure
# define FBL_MEM_STATUS_BlockVerifyPipeVerify         kDiagNrcGeneralProgFailure
# define FBL_MEM_STATUS_BlockVerifyOutputVerify       kDiagNrcGeneralProgFailure
/* SegmentStart indication */
# define FBL_MEM_STATUS_SegmentStartSequence          kDiagNrcConditionsNotCorrectOrRequestSeqErr
# define FBL_MEM_STATUS_SegmentStartDataProcInit      kDiagNrcGeneralProgFailure
# define FBL_MEM_STATUS_SegmentStartVerifyCompute     kDiagNrcGeneralProgFailure
# define FBL_MEM_STATUS_SegmentStartSegmentCount      kDiagNrcConditionsNotCorrectOrRequestSeqErr
/* SegmentEnd indication */
# define FBL_MEM_STATUS_SegmentEndSequence            kDiagNrcConditionsNotCorrectOrRequestSeqErr
# define FBL_MEM_STATUS_SegmentEndInsufficientData    kDiagNrcConditionsNotCorrectOrRequestSeqErr
# define FBL_MEM_STATUS_SegmentEndPost                kDiagNrcConditionsNotCorrectOrRequestSeqErr
/* Data indication */
# define FBL_MEM_STATUS_DataIndSequence               kDiagNrcRequestOutOfRange
# define FBL_MEM_STATUS_DataIndParam                  kDiagNrcGeneralProgFailure
# define FBL_MEM_STATUS_DataIndOverflow               kDiagNrcGeneralProgFailure
/* Data processing */
# define FBL_MEM_STATUS_DataProc                      kDiagNrcGeneralProgFailure
# define FBL_MEM_STATUS_DataProcConsume               kDiagNrcGeneralProgFailure
# define FBL_MEM_STATUS_DataProcDeinit                kDiagNrcConditionsNotCorrectOrRequestSeqErr
/* Memory driver */
# define FBL_MEM_STATUS_DriverResumeWrite             kDiagNrcGeneralProgFailure
# define FBL_MEM_STATUS_DriverWrite                   kDiagNrcGeneralProgFailure
# define FBL_MEM_STATUS_DriverErase                   kDiagNrcGeneralProgFailure
# define FBL_MEM_STATUS_DriverRemainder               kDiagNrcGeneralProgFailure
# define FBL_MEM_STATUS_DriverSuspendWrite            kDiagNrcGeneralProgFailure
/* Programming operation */
# define FBL_MEM_STATUS_ProgramOverflow               kDiagNrcGeneralProgFailure
# define FBL_MEM_STATUS_ProgramOutsideFbt             kDiagNrcRequestOutOfRange
# define FBL_MEM_STATUS_ProgramUnalignedAddress       kDiagNrcGeneralProgFailure
# define FBL_MEM_STATUS_ProgramDriverNotReady         kDiagNrcGeneralProgFailure
# define FBL_MEM_STATUS_ProgramPreWrite               kDiagNrcGeneralProgFailure
# define FBL_MEM_STATUS_ProgramPostWrite              kDiagNrcGeneralProgFailure
/* Erase operation */
# define FBL_MEM_STATUS_EraseOutsideFbt               kDiagNrcRequestOutOfRange
# define FBL_MEM_STATUS_EraseDriverNotReady           kDiagNrcGeneralProgFailure
# define FBL_MEM_STATUS_ErasePreErase                 kDiagNrcGeneralProgFailure
# define FBL_MEM_STATUS_ErasePostErase                kDiagNrcGeneralProgFailure
/* On-the-fly verification */
# define FBL_MEM_STATUS_VerifyCompute                 kDiagNrcGeneralProgFailure
# define FBL_MEM_STATUS_VerifyFinalize                kDiagNrcGeneralProgFailure
/* Pass-through */
# define FBL_MEM_STATUS_PassThroughLocal              kDiagNrcConditionsNotCorrectOrRequestSeqErr
# define FBL_MEM_STATUS_PassThroughRemote             kDiagNrcConditionsNotCorrectOrRequestSeqErr
#endif /* FBL_MEM_ENABLE_STATUS_OVERWRITE || FBL_MEM_ENABLE_STATUS_REMAPPING */

/** Enable extended error status handling */
# define FBL_MEM_ENABLE_EXT_STATUS

/* Default */
# define FBL_MEM_EXT_STATUS_Ok(status)
# define FBL_MEM_EXT_STATUS_Failed(status)
/* BlockErase indication */
# define FBL_MEM_EXT_STATUS_BlockEraseSequence(status)
/* BlockStart indication */
# define FBL_MEM_EXT_STATUS_BlockStartSequence(status)
# define FBL_MEM_EXT_STATUS_BlockStartParam(status)
# define FBL_MEM_EXT_STATUS_BlockStartVerifyInit(status)
/* BlockEnd indication */
# define FBL_MEM_EXT_STATUS_BlockEndSequence(status)
# define FBL_MEM_EXT_STATUS_BlockEndVerifyFinalize(status)
/* BlockVerify */
# define FBL_MEM_EXT_STATUS_BlockVerifySequence(status)
# define FBL_MEM_EXT_STATUS_BlockVerifySigVerify(status)
# define FBL_MEM_EXT_STATUS_BlockVerifyInputVerify(status)
# define FBL_MEM_EXT_STATUS_BlockVerifyPipeVerify(status)
# define FBL_MEM_EXT_STATUS_BlockVerifyOutputVerify(status)
/* SegmentStart indication */
# define FBL_MEM_EXT_STATUS_SegmentStartSequence(status)
# define FBL_MEM_EXT_STATUS_SegmentStartDataProcInit(status)
# define FBL_MEM_EXT_STATUS_SegmentStartVerifyCompute(status)
# define FBL_MEM_EXT_STATUS_SegmentStartSegmentCount(status)
/* SegmentEnd indication */
# define FBL_MEM_EXT_STATUS_SegmentEndSequence(status)
# define FBL_MEM_EXT_STATUS_SegmentEndInsufficientData(status)
# define FBL_MEM_EXT_STATUS_SegmentEndPost(status)
/* Data indication */
# define FBL_MEM_EXT_STATUS_DataIndSequence(status)
# define FBL_MEM_EXT_STATUS_DataIndParam(status)
# define FBL_MEM_EXT_STATUS_DataIndOverflow(status)            {FblSetPecErr(Err_LengthExceeded); /* PRQA S 3109 */ /* MD_FblHdr_Debug_3109 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */ }
/* Data processing */
# define FBL_MEM_EXT_STATUS_DataProc(status)
# define FBL_MEM_EXT_STATUS_DataProcConsume(status)
# define FBL_MEM_EXT_STATUS_DataProcDeinit(status)
/* Memory driver */
# define FBL_MEM_EXT_STATUS_DriverResumeWrite(status)
/* Status contains flash driver error code */
# define FBL_MEM_EXT_STATUS_DriverWrite(status)                {FblErrDebugStatus(Err_FlashWrite|status); FblSetPecErr(Err_FlashWrite); /* PRQA S 3109 */ /* MD_FblHdr_Debug_3109 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */}
# define FBL_MEM_EXT_STATUS_DriverErase(status)                {FblSetPecErr(Err_Erase); FblErrDebugStatus(Err_Erase|status); /* PRQA S 3109 */ /* MD_FblHdr_Debug_3109 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */}
# define FBL_MEM_EXT_STATUS_DriverRemainder(status)
# define FBL_MEM_EXT_STATUS_DriverSuspendWrite(status)
/* Programming operation */
# define FBL_MEM_EXT_STATUS_ProgramOverflow(status)            FblSetPecErr(Err_LengthExceeded)
# define FBL_MEM_EXT_STATUS_ProgramOutsideFbt(status)          {FblErrDebugStatus(kDiagErrNoMemoryRegionFound|status); FblSetPecErr(Err_FlashWrite); /* PRQA S 3109 */ /* MD_FblHdr_Debug_3109 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */}
# define FBL_MEM_EXT_STATUS_ProgramUnalignedAddress(status)    {FblErrDebugStatus(kDiagErrCodeNotAligned|status); FblSetPecErr(Err_FlashWrite); /* PRQA S 3109 */ /* MD_FblHdr_Debug_3109 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */}
# define FBL_MEM_EXT_STATUS_ProgramDriverNotReady(status)      {FblErrDebugStatus(kDiagErrFlashcodeInitFailed|status); FblSetPecErr(Err_FlashWrite); /* PRQA S 3109 */ /* MD_FblHdr_Debug_3109 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */}
# define FBL_MEM_EXT_STATUS_ProgramPreWrite(status)
# define FBL_MEM_EXT_STATUS_ProgramPostWrite(status)
/* Erase operation */
# define FBL_MEM_EXT_STATUS_EraseOutsideFbt(status)            {FblErrDebugStatus(kDiagErrNoMemoryRegionFound|status); FblSetPecErr(Err_Erase); /* PRQA S 3109 */ /* MD_FblHdr_Debug_3109 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */}
# define FBL_MEM_EXT_STATUS_EraseDriverNotReady(status)        {FblErrDebugStatus(kDiagErrFlashcodeInitFailed|status); FblSetPecErr(Err_Erase); /* PRQA S 3109 */ /* MD_FblHdr_Debug_3109 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */}
# define FBL_MEM_EXT_STATUS_ErasePreErase(status)
# define FBL_MEM_EXT_STATUS_ErasePostErase(status)
/* On-the-fly verification */
# define FBL_MEM_EXT_STATUS_VerifyCompute(status)
# define FBL_MEM_EXT_STATUS_VerifyFinalize(status)
/* Pass-through */
# define FBL_MEM_EXT_STATUS_PassThroughLocal(status)
# define FBL_MEM_EXT_STATUS_PassThroughRemote(status)

/* Extended info: status contains current address */
# define FBL_MEM_EXT_STATUS_ProgramAddress(status)
# define FBL_MEM_EXT_STATUS_EraseAddress(status)

/* PRQA L:TAG_FblMem_3458_1 */
/* PRQA L:TAG_FblMem_3453_1 */

#if defined( FBL_ENABLE_VERIFY_INTEGRITY_WORD ) /* Alm Secmodule*/
# if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED ) 
   /* Integrity word can be verified in Fbl by using vendor verify mechanism */
#  if !defined ( SEC_ENABLE_SECURITY_CLASS_VENDOR )
#   error "SEC_ENABLE_SECURITY_CLASS_VENDOR needs to be defined."
#  endif
# else
#  error"This use case is not available in this delivery."
   /* Integrity word can be verified in Fbl by using vendor checksum mechanism */
#  if !defined ( SEC_ENABLE_VENDOR_CHECKSUM )
#   error "SEC_ENABLE_VENDOR_CHECKSUM needs to be defined."
#  endif
# endif
#endif

#endif /* __FBL_MEM_OEM_H__ */

/***********************************************************************************************************************
 *  END OF FILE: FBL_MEM_OEM.H
 **********************************************************************************************************************/

