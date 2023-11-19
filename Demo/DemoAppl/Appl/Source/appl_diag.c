/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file          fbl_diag.c
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
 *  05.02.00   2014-02-27  AWh     ESCAN00073571    Improvement: Additional Debug Status Information
 *                                 ESCAN00073658    Improvement: Encapsulate partId behind FblHdrGetParsedModPartId()
 *                                 ESCAN00073659    Justify Metrics, Rework of FblReadDataByIdentifier.
 *                                 ESCAN00073864    Pipelined verification feature will not shorten download time
 *                                 ESCAN00073975    Memdriver might get deinitialized while it is not present in memory
 *  05.03.00   2014-03-12  AWh     ESCAN00074221    Development feature "Stay In Boot": No answer is sent
 *  05.04.00   2014-08-21  VNa     ESCAN00078132    No Changes
 *  05.05.00   2014-09-08  HRf     ESCAN00078279    Replace Kb-tokens, remove Oem version check
 *             2014-09-11  HRf     ESCAN00078353    Add initialization values in case of SEC_ENABLE_WORKSPACE_EXTERNAL
 *                                 ESCAN00078354    Change function name FblDiagTask() to FblDiagTimerTask()
 *  05.06.00   2014-11-19  Dod     ESCAN00078505    No response to returnToNormalMode ($20) request
 *                         AWh     ESCAN00079190    No changes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "fbl_inc.h"    /* Common include files */
#if defined( VGEN_ENABLE_BRS )
# include "BrsCtrl.h"
# include "BrsAddOn.h"
#endif

#if ( FBLKBDIAG_14230_GM_VERSION != 0x0506u ) || \
    ( FBLKBDIAG_14230_GM_RELEASE_VERSION != 0x00u )
# error "Error in fbl_diag.c: Source and Header file are inconsistent!"
#endif

#if ( FBLKBDIAG_14230_GM_VERSION != _FBLKBDIAG_OEM_VERSION ) || \
    ( FBLKBDIAG_14230_GM_RELEASE_VERSION != _FBLKBDIAG_OEM_RELEASE_VERSION )
# error "Error in fbl_diag.c: Source and v_ver.h are inconsistent!"
#endif

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/* Defines, to access the AllNode request databuffer (extended-addressing)   */
/* CAN Hardware access support for diagnostic                                */
#define kDiagAllNodeAddr   ((vuint8)0xFEu)
#define kDiagGatewayAddr   ((vuint8)0xFDu)

#if defined( MULTIPLE_RECEIVE_BUFFER )
# define EXTADDR   (data[0])
# define TPCI      (data[1])
# define SRV_ID    (data[2])
# define PARAM     (data[3])
#endif

/* Defines for tranport layer data buffer access, *
 * same defines also in FBL_TP.C                  */

#if defined( FBL_TP_ENABLE_EXTENDED_ADDRESSING )
/* Extended addressing */
# define kTpciPos        ((vuint8)0x01u)
# define kSFDataPos      ((vuint8)0x02u)
#else
/* Normal addressing */
# define kTpciPos        ((vuint8)0x00u)
# define kSFDataPos      ((vuint8)0x01u)
#endif

/* Flag indicating that a diagnostics request has been received */
#define SetDiagIndication()            (fblStates |= kFblStateDiagIndication)
#define ClrDiagIndication()            (fblStates &= FblInvert16Bit(kFblStateDiagIndication))

/* Flag indicating that programming-mode has been enabled (Service A5 03) */
#define SetEnablePrgMode()             (fblStates |= kFblStateProgrammingMode)
#define ClrEnablePrgMode()             (fblStates &= FblInvert16Bit(kFblStateProgrammingMode))

/* Flag indicating that transfer-data service (36) is allowed */
#define SetTransferDataAllowed()       (fblStates |= kFblStateTransferDataAllowed)
#define ClrTransferDataAllowed()       (fblStates &= FblInvert16Bit(kFblStateTransferDataAllowed))

/* Flag indicating Security-Key request (27 02) is allowed */
#define SetSecurityKeyAllowed()        (fblStates |= kFblStateSecurityKey)
#define ClrSecurityKeyAllowed()        (fblStates &= FblInvert16Bit(kFblStateSecurityKey))

/* Flag indicating that a request is functionally addressed */
#define SetFunctionalRequest()         (fblStates |= kFblStateFunctionalRequest)
#define ClrFunctionalRequest()         (fblStates &= FblInvert16Bit(kFblStateFunctionalRequest))

/* Flag indicating a running response processing */
#define SetResponseProcessing()        (fblStates |= kFblStateResponseProcessing)
#define ClrResponseProcessing()        (fblStates &= FblInvert16Bit(kFblStateResponseProcessing))

/* Flag indicating that responses have been enabled */
#define SetEnableDiagResp()            (fblStates |= kFblStateEnableDiagResp)
#define ClrEnableDiagResp()            (fblStates &= FblInvert16Bit(kFblStateEnableDiagResp))

/* Flag indicating normal communication has been disabled (service 0x28) */
#define SetDisableComm()               (fblStates |= kFblStateDisableComm)
#define ClrDisableComm()               (fblStates &= FblInvert16Bit(kFblStateDisableComm))

/* Flag indicating high-speed mode has been requested (A5 02) */
#define SetHighspeedMode()             (fblStates |= kFblStateHighspeedMode)
#define ClrHighspeedMode()             (fblStates &= FblInvert16Bit(kFblStateHighspeedMode))

/* Flag indicating programming-mode has been requested (A5 01 or A5 02) */
#define SetRequestPrgMode()            (fblStates |= kFblStateRequestPrgMode)
#define ClrRequestPrgMode()            (fblStates &= FblInvert16Bit(kFblStateRequestPrgMode))

/* Flag indicating that a diagnostic request is being serviced */
#define SetDiagInProgress()            (fblStates |= kFblStateDiagInProgress)
#define ClrDiagInProgress()            (fblStates &= FblInvert16Bit(kFblStateDiagInProgress))

/* Response pending message indication */
#define SetRcrRpInProgress()           (fblStates |= kFblStateRcrRpInProgress)
#define ClrRcrRpInProgress()           (fblStates &= FblInvert16Bit(kFblStateRcrRpInProgress))

/* MemDriver init flag */
#define SetMemDriverInitialized()      (fblStates |= kFblStateMemDriverInit)
#define ClrMemDriverInitialized()      (fblStates &= FblInvert16Bit(kFblStateMemDriverInit))

#define kDiagPutResponse            ((vuint8)0x01u)
#define kSwmOutOfRange              ((vuint8)0xFFu)

/* Bit-masks for Transfer-Data state */
/* LSB reserved for persistent state flags,
 * MSB reserved for discrete state flags: only one can be set at a time */
#define DIAG_TRANSFERDATA_IDLE            ((vuint16)0x0000u)
#define DIAG_TRANSFERDATA_HDR_EXPECTED    ((vuint16)0x0400u)

/** Macro used for FblDirectMemAccess, in order to read chunks via MEMCPY */
#define MEMCPY_MAX_CHUNK_SIZE ((tFblLength)0x100u)

/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/

typedef struct
{
   vuint32  dummy;
   /** Buffer for on-the-fly signature calculation */
   vuint8   data[kDiagSigBufLength];
} tSigBuf;

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

/* Diagnostics Node Address */
V_MEMRAM0 V_MEMRAM1 vuint8 V_MEMRAM2 diagNodeAddr;

#if defined( FBL_ENABLE_MULTIPLE_NODES )
V_MEMRAM0 V_MEMRAM1 vuint8 V_MEMRAM2 fblEcuHandle; /**< Index into Node-Address and CAN-ID arrays to select the node */
V_MEMROM0 V_MEMROM1 vuint8 V_MEMROM2 kFblDiagNodeAddr[] = FBL_MULTIPLE_NM_ECUNR; /**< Array to hold multiple Node IDs */
#endif

V_MEMRAM0 V_MEMRAM1_NEAR vuint8 V_MEMRAM2_NEAR diagErrorCode; /**< Diagnostic error code */
V_MEMRAM0 V_MEMRAM1_NEAR tTpDataType V_MEMRAM2_NEAR DiagDataLength; /**< Stores number of received data */
V_MEMRAM0 V_MEMRAM1_NEAR vuint16 V_MEMRAM2_NEAR fblStates; /**< Internal state flags for FBL state machine */
V_MEMRAM0 V_MEMRAM1_NEAR vuint16 V_MEMRAM2_NEAR testerPresentTimeout; /**< Timer value for TesterPresent timeout */
V_MEMRAM0 V_MEMRAM1_NEAR vuint8 V_MEMRAM2_NEAR diagResponseFlag;  /**< Flag to enable/suppress diagnostic response */
V_MEMRAM0 V_MEMRAM1_NEAR vsint16 V_MEMRAM2_NEAR memSegment; /**< Idx to currently processed FlashBlock */
V_MEMRAM0 V_MEMRAM1_NEAR vuint8 V_MEMRAM2_NEAR diagServiceCurrent; /**< Contains the currently processed diag service ID */
V_MEMRAM0 V_MEMRAM1_NEAR vuint16 V_MEMRAM2_NEAR errPecErrorCode; /**< Current Program error code */

#if defined( FBL_ENABLE_DEBUG_STATUS )
/* Variables for error status reporting */
V_MEMRAM0 V_MEMRAM1 vuint16 V_MEMRAM2 errStatErrorCode; /**< Error status */
V_MEMRAM0 V_MEMRAM1 vuint16 V_MEMRAM2 errStatFblStates; /**< FBL state flag */
V_MEMRAM0 V_MEMRAM1 vuint8 V_MEMRAM2 errStatLastServiceId; /**< Last received service identifier */
V_MEMRAM0 V_MEMRAM1 vuint8 V_MEMRAM2 errStatFlashDrvVersion[3]; /**< Flash driver version information */
V_MEMRAM0 V_MEMRAM1 vuint16 V_MEMRAM2 errStatFlashDrvErrorCode; /**< Flash driver error code */
V_MEMRAM0 V_MEMRAM1 vuint16 V_MEMRAM2 errStatDescriptor; /**< Error status block descriptor */
V_MEMRAM0 V_MEMRAM1 vuint8 V_MEMRAM2 errStatTpError;
V_MEMRAM0 V_MEMRAM1 vuint8 V_MEMRAM2 errStatHaveDriver;
V_MEMRAM0 V_MEMRAM1 tFblAddress V_MEMRAM2 errStatAddress;
V_MEMRAM0 V_MEMRAM1 tDebugFileName V_MEMRAM2 errStatFileName;
V_MEMRAM0 V_MEMRAM1 tDebugLineNbr V_MEMRAM2 errStatLineNumber;
#endif

V_MEMRAM0 V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3* V_MEMRAM1_NEAR V_MEMRAM2_NEAR DiagBuffer; /**< Data buffer for diagnostic data */
V_MEMRAM0 V_MEMRAM1 vuint32 V_MEMRAM2 sleepCounter; /**< Inactivity counter: Determines when to go to sleep */
V_MEMRAM0 V_MEMRAM1 vuint8 V_MEMRAM1 fblProgrammedState; /**< Bit-flags containing results of memory-integrity checks and download under-run condition. */

V_MEMRAM0 V_MEMRAM1_NEAR tMsgObject V_MEMRAM2_NEAR fblCanTxObj[kNrOfTxObjects]; /**< CAN transmit buffers */

/***********************************************************************************************************************
 *  LOCAL DATA
 **********************************************************************************************************************/

/* Variables used to transfer data */
V_MEMRAM0 static V_MEMRAM1_NEAR FL_SegmentInfoType V_MEMRAM2_NEAR currDataSegInfo; /**< Actual address for flash programming */

V_MEMRAM0 static V_MEMRAM1 vuint8  V_MEMRAM2 currDataSegIdx; /**< Index to data segment currently programmed */
V_MEMRAM0 static V_MEMRAM1 tTpDataType V_MEMRAM2 dataInDiagBuffer; /**< Number of bytes in DiagBuffer to process */
V_MEMRAM0 static V_MEMRAM1 tFblMemSegmentInfo V_MEMRAM2 currMemLibSegment; /**< segment currently processed in MemLib */
V_MEMRAM0 static V_MEMRAM1 tFblMemBlockInfo V_MEMRAM2 blockInfo; /**< block currently processed in MemLib */
V_MEMRAM0 static V_MEMRAM1 tTpDataType V_MEMRAM2 diagBufferIndex; /**< Index into DiagBuffer of next segment to process */
V_MEMRAM0 static V_MEMRAM1 vuint16 V_MEMRAM2 transferState; /**< Current state of Transfer-Data service handler */
#if defined( FBL_ENABLE_SYSTEM_CHECK )
V_MEMRAM0 static V_MEMRAM1 tFblLength V_MEMRAM2 currDataSegInitialLen; /* Initial len of current data segment */
#endif

/* Pipelined Verification */
V_MEMRAM0 static V_MEMRAM1 tSigBuf V_MEMRAM2 sigBuf;   /* PRQA S 3218 */ /* MD_FblDiag_3218_3 */
V_MEMRAM0 static V_MEMRAM1 vuint32 V_MEMRAM2 currentDataLength; /* PRQA S 3218 */ /* MD_FblDiag_3218_3 */ /* PRQA S 0781 */ /* MD_FblDiag_0781 */

/***********************************************************************************************************************
 *  EXTERNAL DATA
 **********************************************************************************************************************/

V_MEMRAM0 extern V_MEMRAM1 tFblCanIdTable V_MEMRAM2 fblCanIdTable;
#if defined( FBL_USE_FBL_AS_DEMOAPPL )
extern void JumpToBootDemonstration (tFblLength pUncompressedMemSize, vuint8 pFormatId); /* only for demonstration how to jump from app to boot */
#endif
/***********************************************************************************************************************
 *  PROTOTYPES
 **********************************************************************************************************************/

static void FblEnterProgrammingMode( void );
static void FblEnterRequestDownload( vuint8 formatId, tFblLength uncompressedMemSize );

static void FblDiagTransferData( void );
static tFblResult FblDiagTransferDataProcess( void );
static tFblResult FblDiagTransferDataHeaderReceived( void );
static tFblResult FblDiagTransferDataEnd( void );

#if defined( FBL_ENABLE_INITIATE_DIAG_OP )
static void FblDiagInitiateDiagnosticOperation( void );
#endif

static void FblDiagReadDataByIdentifier( void );
static void FblDiagReturnToNormalMode( void );

#if defined( FBL_ENABLE_READ_MEMORY_BY_ADDRESS )
static void FblDiagReadMemoryByAddress( void );
#endif

#if defined( FBL_ENABLE_SECURITY_ACCESS )
static void FblDiagSecurityAccess( void );
#endif

static void FblDiagDisableNormalCommunication( void );
static void FblDiagRequestDownload( void );

#if defined( FBL_ENABLE_WRITE_DATA_BY_IDENTIFIER )
static void FblDiagWriteDataByIdentifier( void );
#endif

static void FblDiagTesterPresent( void );
static void FblDiagReportProgrammedState( void );
static void FblDiagProgrammingMode( void );

static void DiagDiscardReception( void );

static void FblDiagSetMemLibMemBlockInfo( void );
static void FblDiagSetMemLibSegmentAddrLengthInfo( void );

/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/* PRQA S 2006 EOF */ /* MD_CBD_14.7 */

/***********************************************************************************************************************
 *  FblDiagInit - GM specific
 **********************************************************************************************************************/
/*! \brief       Initialize module variables
 **********************************************************************************************************************/
void FblDiagInit( void )
{
   fblStates            = 0;
   transferState        = DIAG_TRANSFERDATA_IDLE;
   DiagClrError();
   FblClrPecErr(); /* PRQA S 3109 */ /* MD_CBD_14.3 */

   /* Get aligned diag buffer pointer from memory library */
   DiagBuffer = FblMemInitPowerOn();

#if defined( VGEN_ENABLE_BRS )
   BrsCtrlInitPowerOn();
   BrsAddOnInitPowerOn();
#endif

   /* Call Header Module Init */
   FblHdrInit();

   /* Stop tester present timer */
   StopTesterTimeout();

   /* Clear timer for response pending transmission */
   ClrP2Timer();

   /* Make sure to turn off the programming voltage */
   ApplFblResetVfp();

#if !defined( FBL_ENABLE_SECURITY_ACCESS )
   SetSecurityUnlock();
#endif

#  if defined( FBL_ENABLE_CAN_CONFIGURATION ) || defined( FBL_ENABLE_MULTIPLE_NODES )
   FblCanSetTxId(fblCanTxObj[0], MK_ID16(CanInitTable.TpTxIdHigh, CanInitTable.TpTxIdLow));
#  else
   /* Init CAN transmit message for diag response */
   if (0 != (GetFblMode() & START_FROM_APPL))
   {
      FblCanSetTxId(fblCanTxObj[0], MK_ID16(CanInitTable.TpTxIdHigh, CanInitTable.TpTxIdLow));
   }
   else
   {
      FblCanSetTxId(fblCanTxObj[kTpTxObj], MK_ID(DIAG_NODE_PHYSICAL_RESPONSE_ID));
   }
   diagNodeAddr = DIAG_NODE_ADDRESS;
# endif

   FblCanSetTxDLC(fblCanTxObj[kTpTxObj], 8);

#if defined( FBL_ENABLE_CAN_CONFIGURATION )
   SetEnableDiagResp();
#endif

   if (0 != (GetFblMode() & START_FROM_APPL))
   {
      /* Prepare sending out response pending early */
      SetDiagInProgress();
      SetEnablePrgMode();     /* Need this so that P2 timing is right for RCR-RP */

      DiagBuffer[kDiagFmtServiceId] = kDiagSidRequestDownload;
      diagServiceCurrent = kDiagSidRequestDownload;

      DiagExRCRResponsePending(kForceSendResponsePending);
   }

#if defined( FBL_ENABLE_DEBUG_STATUS )
   ApplFblInitErrStatus();
#endif

   (void) SecM_InitPowerOn(V_NULL);
} /* PRQA S 6050 */ /* MD_FblDiag_60xx */

/***********************************************************************************************************************
 *  FblGetBlockDescriptor
 **********************************************************************************************************************/
/*! \brief       Prepares a Dummy block descriptor that holds the module id required to satisfy the presence pattern API
 *  \param[in]   id id that sets current blockNr
 *  \return      tBlockDescriptor* dummy block descriptor prepared.
 **********************************************************************************************************************/
tBlockDescriptor* FblGetBlockDescriptor( const vuint8 partId )
{
   static  V_MEMRAM1 tBlockDescriptor V_MEMRAM2 dummyBlockDescriptor;
   dummyBlockDescriptor.blockNr = partId;
   return &dummyBlockDescriptor;
}

/***********************************************************************************************************************
 *  FblDiagInitStartFromAppl - GM specific
 **********************************************************************************************************************/
/*! \brief       This function is called when the FBL is started from application.
 *  \details     The function sets the FBL state assuming that the correct sequence
 *               of messages have been received by the Operating Software,
 *               and that the FBL is started upon reception of a Request-Download (0x34) service request.
 *  \pre         FblDiagInit must be called first.
 **********************************************************************************************************************/
void FblDiagInitStartFromAppl( void )
{
   ResetTesterTimeout();
   SetDisableComm();
   SetEnableDiagResp();
   DiagSetForceResponse();

#if defined( FBL_ENABLE_HIGHSPEED_MODE )
   if (CanInitTable.RequestProgrammingMode == REQUEST_DOWNLOAD_MODE_HIGHSPEED)
   {
      SetHighspeedMode();
      ApplTrcvrHighSpeedMode();
   }
#endif

#if defined( FBL_ENABLE_SECURITY_ACCESS )
   SetSecurityUnlock();
#endif

   DiagBuffer[kDiagFmtServiceId] = kDiagSidRequestDownload;
   diagServiceCurrent = kDiagSidRequestDownload;

   DiagBuffer[kDiagFmtSubparam] = CanInitTable.requestDownloadFormatID;

   DiagBuffer[kDiagFmtSubparam + 1] = (vuint8) (CanInitTable.requestDownloadMemorySize >> 24);
   DiagBuffer[kDiagFmtSubparam + 2] = (vuint8) (CanInitTable.requestDownloadMemorySize >> 16);
   DiagBuffer[kDiagFmtSubparam + 3] = (vuint8) (CanInitTable.requestDownloadMemorySize >> 8);
   DiagBuffer[kDiagFmtSubparam + 4] = (vuint8) (CanInitTable.requestDownloadMemorySize);

   if (kFblOk != ApplFblCheckProgConditions())
   {
      DiagNRCConditionsNotCorrectOrRequestSeqErr();
      ClrEnablePrgMode(); /* PRQA S 0277 */ /* MD_FblDiag_0277 */
   }
   else
   {
      FblEnterProgrammingMode();
      FblEnterRequestDownload(CanInitTable.requestDownloadFormatID, CanInitTable.requestDownloadMemorySize);
   }

   DiagProcessingDone(kDiagRslRequestDownload);
   ClrDiagInProgress(); /* PRQA S 0277 */ /* MD_FblDiag_0277 */
}

/***********************************************************************************************************************
 *  FblCalculateCsum16Bit
 **********************************************************************************************************************/
/*! \brief       Create 16-Bit checksum over one Ram buffer
 *  \details     Words are treated as 16-bit according to the native CPU-type (little/big endian)
 *  \param[in]   address Used as a 16-bit Pointer for memory access
 *  \param[in]   len Size of data for checksum calculation
 *  \param[in]   csum Start value of the checksum
 *  \return      calculated checksum
 **********************************************************************************************************************/
vuint16 FblCalculateCsum16Bit( const vuint16* buffer, vuint32 len, vuint16 csum )
{
   vuint32 i = 0;

   while (len > 0)
   {
      csum += buffer[i];
      i++;
      len -= 2;

      FblRealTimeSupportInLoop(len, 0x40);  /* PRQA S 3109 */ /* MD_FblDiag_3109_RealtimeInLoop */
   }
   return csum;
}

/***********************************************************************************************************************
 *  FblHandleRxMsg - GM specific
 **********************************************************************************************************************/
/*! \brief       Processing of filtered CAN receive messages
 *  \details     TpPrecopy is called for physically-addressed requests; FblDiagPrecopy is called for
 *               functionally-addressed request
 *  \pre         CAN hardware has to be initialized before call
 *  \param[in]   fblRxCanMsgHdl RxCanMsg handle
 **********************************************************************************************************************/
# if defined( MULTIPLE_RECEIVE_BUFFER )
void FblHandleRxMsg( vuint8 fblRxCanMsgHdl, pChipDataPtr canDataPtr )
# endif
{
   switch (fblRxCanMsgHdl)
   {
     case FblRxCanMsg0Hdl:
        if ((GetResponseProcessing()) || (!GetEnableDiagResp()))
        {
           /* Diag response is currently processed, discard message */
        }
        else
        {
# if defined( MULTIPLE_RECEIVE_BUFFER )
            (void)FblTpPrecopy( canDataPtr );
# endif
        }
        break;

# if ( FBL_CAN_NUMBER_OF_RX_ID > 1 )
     case FblRxCanMsg1Hdl:
         FblDiagPrecopy( canDataPtr );
        break;
# endif

# if ( FBL_CAN_NUMBER_OF_RX_ID > 2 )
     case FblRxCanMsg2Hdl:
        break;
# endif

# if ( FBL_CAN_NUMBER_OF_RX_ID > 3 )
     case FblRxCanMsg3Hdl:
        break;
# endif

# if ( FBL_CAN_NUMBER_OF_RX_ID > 4 )
     case FblRxCanMsg4Hdl:
        break;
# endif

     default:
        /* Assertion: Wrong filter */
        break;
   }
}

#if defined( FBL_ENABLE_STAY_IN_BOOT )
/***********************************************************************************************************************
 *  FblCheckOwnStartMsg - GM Specific
 **********************************************************************************************************************/
/*! \brief       Returns kFblOk after a 0x1A request has been received
 *  \pre         CAN hardware has to be initialized before call
 *               May only be called after a special (0x101) message was received
 *  \param[in]   data Pointer to CAN message registers containing message
 *  \return      Return value says whether a 0x1A request was received
 **********************************************************************************************************************/
# if defined( MULTIPLE_RECEIVE_BUFFER )
   vuint8 FblCheckOwnStartMsg( pChipDataPtr data )
# endif   /* #if defined( MULTIPLE_RECEIVE_BUFFER ) */
{
   if (EXTADDR == diagNodeAddr)
   {
      /* Check service identifier */
      if ((kDiagSidReadDataByIdentifier == SRV_ID) && (kDiagDidECUOperatingStatus == PARAM))
      {
         return kFblOk;
      }
   }
   return kFblFailed;
}
#endif

/***********************************************************************************************************************
 *  DiagDiscardReception - GM specific
 **********************************************************************************************************************/
/*! \brief       Received diagnostic messages are discarded
 **********************************************************************************************************************/
static void DiagDiscardReception( void )
{
   SetResponseProcessing();   /* Set flag for running response procedure */
   while (0 != FblCanReceive())   /* Handle all rx messages */
   {
      FblCanProcessRxMsg();
   }
   ClrResponseProcessing(); /* PRQA S 0277 */ /* MD_FblDiag_0277 */
}

/***********************************************************************************************************************
 *  DiagResponseProcessor - 14230 specific
 **********************************************************************************************************************/
/*! \brief       Send diagnostic response message
 *  \pre         Current service ID must be in DiagBuffer[0], diagErrorCode must be initialized
 *  \param[in]   dataLength Data length to be transmitted
 **********************************************************************************************************************/
void DiagResponseProcessor( tTpDataType dataLength )
{
   ClrRcrRpInProgress(); /* PRQA S 0277 */ /* MD_FblDiag_0277 */
   ClrP2Timer();
   DiagDiscardReception();
   FblErrStatSetSId (diagServiceCurrent);

   if (kDiagErrorNone != DiagGetError())
   {
      /* Prepare response message */
      DiagBuffer[kDiagFmtSubparam]     = diagServiceCurrent;
      DiagBuffer[kDiagFmtServiceId]    = kDiagRidNegativeResponse;
      DiagBuffer[kDiagFmtNegResponse]  = DiagGetError();
      (void)FblTpTransmit(3);

      /* Clear 'pending' error */
      DiagClrError();
   }
   else if (kDiagPutResponse == diagResponseFlag)
   {
      /* Transmit positive response if not suppressed */
      DiagBuffer[kDiagFmtServiceId] = diagServiceCurrent+0x40u;
      (void)FblTpTransmit(dataLength + 1);
   }
   else
   {
      /* Transmit no response, unlock transport layer */
      TpResetRxBlock();  /* PRQA S 0277 */ /* MD_FblDiag_0277 */
   }
}

/***********************************************************************************************************************
 *  FblRealTimeSupport - 14230 specific
 **********************************************************************************************************************/
/*! \brief       Transmit busy messages and triggers the watchdog according to system timer
 *  \pre         P2Timer must be initialized
 **********************************************************************************************************************/
vuint8 FblRealTimeSupport( void )
{
   vuint8 returnCode;

   returnCode = FblLookForWatchdog();
   DiagExRCRResponsePending(kNotForceSendResponsePending);
   return returnCode;
}

/***********************************************************************************************************************
 *  DiagExRCRResponsePending - 14230 specific
 **********************************************************************************************************************/
/*! \brief       Transmit a busy message if timer expires (kNotForceSendResponsePending == forceSend)
 *               or if kForceSendResponsePending is passed.
 *  \param[in]   forceSend determines if a message is sent independently from timer state.
 **********************************************************************************************************************/
void DiagExRCRResponsePending( vuint8 forceSend )
{
   if (GetDiagInProgress())
   {
      if (     ( (!GetRcrRpInProgress()) && (GetP2Timer() > 0x00u)  && (GetP2Timer() < kP2TimerInitialRcRRPMargin))
            || (kForceSendResponsePending == forceSend)
         )
      {
         tpCanTxData[kTpciPos]      = 0x03u;
         tpCanTxData[kSFDataPos]    = kDiagRidNegativeResponse;
         tpCanTxData[kSFDataPos + 1]  = diagServiceCurrent;
         tpCanTxData[kSFDataPos + 2]  = kDiagNrcRcrResponsePending;
#if defined( FBL_TP_ENABLE_VARIABLE_TX_DLC )
#else
         tpCanTxData[kSFDataPos + 3]  = kFblTpFillPattern;
         tpCanTxData[kSFDataPos + 4]  = kFblTpFillPattern;
         tpCanTxData[kSFDataPos + 5]  = kFblTpFillPattern;
# if defined( FBL_TP_ENABLE_EXTENDED_ADDRESSING )
# else
         tpCanTxData[kSFDataPos + 6]  = kFblTpFillPattern;
# endif
#endif

# if defined( FBL_TP_ENABLE_VARIABLE_TX_DLC )
         FblCanSetTxDLC(fblCanTxObj[kTpTxObj], 4u);
# endif
         if (kCanTxOk == FblCanTransmit(kFblTpTxHandle))
         {
            SetP2Timer(kFblDiagTimeP2Star);
            SetRcrRpInProgress();
         }
      }
   }

}

#if defined( FBL_ENABLE_DEBUG_STATUS )
/***********************************************************************************************************************
 *  FblErrDebug
 **********************************************************************************************************************/
/*! \brief       This function saves the extended error status.
 *  \details     The status may be retrieved with a Read-Data-By-Identifier (0x1A) service request.
 *  \param[in]   error Extended error status, see macros kDiagErr<condition>
 *  \param[in]   fileName pointer to string containing name of file the error was detected in
 *  \param[in]   lineNumber source line where error was detected at
 **********************************************************************************************************************/
void FblErrDebug( vuint16 error, tDebugFileName fileName, tDebugLineNbr lineNumber )
{
   /* Do not overwrite already set error */
   if (0 == errStatErrorCode)
   {
      FblErrStatSetError(error);
      FblErrStatSetState(fblStates);
      errStatHaveDriver = 0;
      errStatFileName = fileName;
      errStatLineNumber = lineNumber;
   }
}

/***********************************************************************************************************************
 *  FblErrDebugDriver
 **********************************************************************************************************************/
/*! \brief       This routine is called when an memory-device related error is detected.
 *               The routine saves that error-code and address that the error occurred at.
 *  \details     The address may be retrieved with a Read-Data-By-Identifier (0x1A) service request.
 *  \param[in]   addr Address that an memory error occurred at
 *  \param[in]   error Error code returned from device-driver
 **********************************************************************************************************************/
void FblErrDebugDriver( tFblAddress addr, vuint16 error )
{
   FblErrStatSetFlashDrvError(error);
   errStatAddress=(addr);
   errStatHaveDriver = 1u;
}
#endif /* FBL_ENABLE_DEBUG_STATUS */

/***********************************************************************************************************************
 *  FblDiagSetMemLibMemBlockInfo
 **********************************************************************************************************************/
/*! \brief       Set LibMem segment address and length information
 **********************************************************************************************************************/
static void FblDiagSetMemLibMemBlockInfo( void )
{
   static SecM_VerifyParamType verifyParam;
   static SecM_SignatureParamType verifyParamPipelined;
   vuintx verifyBlockNr = GetBlockNbrFromPartId(FblHdrGetParsedModPartId());

   /* Init verifyParam data */
   verifyParam.segmentList = parsedModuleInfo.segmentList;

   verifyParam.wdTriggerFct = (FL_WDTriggerFctType)FblHdrRealTimeSupportSecMod;
   verifyParam.readMemory = (tFblMemVerifyReadFct)FblReadProm;

   /* init RoutinePipe Param */
   verifyParamPipelined.wdTriggerFct = (FL_WDTriggerFctType)FblHdrRealTimeSupportSecMod;
   verifyParamPipelined.currentDataLength = &currentDataLength;
#if defined( SEC_ENABLE_WORKSPACE_EXTERNAL )
   verifyParamPipelined.currentHash.length = 0u;
   verifyParamPipelined.currentHash.sigResultBuffer = 0u;
#else
   verifyParamPipelined.currentHash.length = kDiagSigBufLength;
   verifyParamPipelined.currentHash.sigResultBuffer = (vuint32) sigBuf.data; /* PRQA S 0306 */ /* MD_FblDiag_0306 */
#endif

   blockInfo.verifyRoutineOutput.function = (tFblMemVerifyFctOutput)(FblLogicalBlockTable.logicalBlock[verifyBlockNr].verifyROM); /* PRQA S 0313 */ /* MD_FblDiag_0313 */
   blockInfo.verifyRoutineOutput.param    = &verifyParam;

   assertFblGen(((tVerificationFct)FblLogicalBlockTable.logicalBlock[0].verifyROM == FblHdrVerifyIntegrity), kFblAssertCorrectGenyConfig); /* PRQA S 0313 */ /* MD_FblDiag_0313 */

   {
      blockInfo.verifyRoutineInput.function  = (tFblMemVerifyFctInput) 0;
      blockInfo.verifyRoutineInput.param     = V_NULL;
      blockInfo.verifyRoutinePipe.function   = (tFblMemVerifyFctInput)(FblLogicalBlockTable.logicalBlock[verifyBlockNr].verifyRAM); /* PRQA S 0313 */ /* MD_FblDiag_0313 */
      blockInfo.verifyRoutinePipe.param      = &verifyParamPipelined;
   }

   blockInfo.segmentList         = &verifyParam.segmentList;
   blockInfo.maxSegments         = SWM_DATA_MAX_NOAR;
   blockInfo.readFct             = verifyParam.readMemory;

   blockInfo.targetAddress = FblLogicalBlockTable.logicalBlock[verifyBlockNr].blockStartAddress;
   blockInfo.targetLength = FblLogicalBlockTable.logicalBlock[verifyBlockNr].blockLength;

   /* This is not used */
   blockInfo.logicalAddress = verifyBlockNr;
   blockInfo.logicalLength = 0;
}

/***********************************************************************************************************************
 *  FblDiagSetMemLibSegmentAddrLengthInfo
 **********************************************************************************************************************/
/*! \brief       Set LibMem segment address and length information
 **********************************************************************************************************************/
static void FblDiagSetMemLibSegmentAddrLengthInfo( void )
{
   currMemLibSegment.targetAddress  = currDataSegInfo.targetAddress;
   currMemLibSegment.targetLength   = currDataSegInfo.length;
   /* Logical address and length not used */
   currMemLibSegment.logicalAddress = 0;
   currMemLibSegment.logicalLength  = 0;
}

/***********************************************************************************************************************
 *  FblMemSegmentNrGet - GM specific
 **********************************************************************************************************************/
/*! \brief       Get the number of the corresponding block for the given address
 *  \param[in]   address address to find
 *  \return      Index of flash block; kFblDiagMemSegmNotFound, if not found
 **********************************************************************************************************************/
vsint16 FblMemSegmentNrGet( tFblAddress address )
{
   vsint16 locMemSeg;

   locMemSeg = (vsint16) (kNrOfFlashBlock - 1u);

   while ((locMemSeg >= 0) && ((address < FlashBlock[locMemSeg].begin) || (address > FlashBlock[locMemSeg].end)))
   {
      FblRealTimeSupportInLoop((vuint16)locMemSeg, 0x10); /* PRQA S 3109 */ /* MD_FblDiag_3109_RealtimeInLoop */
      locMemSeg--;
   }

   return locMemSeg;
}

/***********************************************************************************************************************
 *  FblDirectMemAccess
 **********************************************************************************************************************/
/*! \brief       This function uses memcpy to read some Ram content
 *  \param[in]   address Memory address to read out
 *  \param[out]   buffer target buffer
 *  \param[in]   length number of bytes to read
 *  \return      Number of bytes requested to read (unchanged)
 **********************************************************************************************************************/
tFblLength FblDirectMemAccess( tFblAddress address, vuint8* buffer, tFblLength length )
{
   /* bytesToRead may be 0-(MEMCPY_MAX_CHUNK_SIZE-1) */
   vuintx byteToAlignedStart =  GetBytesToNextAlignedAddress(address, MEMCPY_MAX_CHUNK_SIZE);
   vuintx bytesToRead = ((length < byteToAlignedStart) ? length : byteToAlignedStart);
   tFblLength tmpLength = length;

   while ( tmpLength > 0 )
   {
      (void)FblRealTimeSupport();
      (void)MEMCPY(buffer, address, bytesToRead); /* PRQA S 0306 */ /* MD_FblDiag_0306 */

      tmpLength -= bytesToRead;
      address += bytesToRead;
      buffer = &buffer[bytesToRead];

      bytesToRead = ((MEMCPY_MAX_CHUNK_SIZE > tmpLength)? tmpLength : MEMCPY_MAX_CHUNK_SIZE);
   }

   return length;
}

#if defined( FBL_ENABLE_DIRECT_MEMORY_ACCESS )
#else
/***********************************************************************************************************************
 *  FblReadProm
 **********************************************************************************************************************/
/*! \brief       This function checks the FlashBlock table and copies a given amount of bytes from the Flash memory.
 *  \details     If no entry in FlashBlock table is found, no device driver can be selected and
 *               the function will retrun the failed result 0.
 *               Address argument must be passed in logical format.
 *  \param[in]   address Memory address to read out
 *  \param[out]  buffer target buffer
 *  \param[in]   length number of bytes to read
 *  \return      Number of actually copied bytes
 **********************************************************************************************************************/
tFblLength FblReadProm( tFblAddress address, vuint8* buffer, tFblLength length )
{
   tFblLength  actualReadCount = 0;
   tFblLength  currentReadCount;
   tFblLength  lenInFlashBlock;

   memSegment = FblMemSegmentNrGet(address);
   if (kFblDiagMemSegmNotFound == memSegment)
   {
      return 0;
   }

   /* Because the device drive can change on boundaries of FlashBlock[] we need to read all data from each
    * individual FlashBlock */
   while (length > 0)
   {
      (void) FblRealTimeSupport();

      memSegment = FblMemSegmentNrGet(address);
      if (kFblDiagMemSegmNotFound != memSegment)
      {
         lenInFlashBlock = (FlashBlock[memSegment].end - address) + 1;
         /* If number of data exceeds current block, we are only allowed to read to the end of the current block. */
         currentReadCount = (( length > lenInFlashBlock ) ? lenInFlashBlock : length);

         /* Get the memory data through the device driver's read function */
         if (IO_E_OK != MemDriver_RReadSync((IO_MemPtrType)buffer,(IO_SizeType)currentReadCount,(IO_PositionType)address))
         {
            /* Read failure. End of read operation */
            length = 0;
         }
         else
         {
            /* Read operation successful. Prepare for end or next loop */
            actualReadCount += currentReadCount;
            buffer           = &buffer[currentReadCount];
            address         += currentReadCount;
            length          -= currentReadCount;
         }
      }
      else
      {
         length = 0;
      }
   }
   return actualReadCount;
}
#endif /* FBL_ENABLE_DIRECT_MEMORY_ACCESS */

/***********************************************************************************************************************
 *  FblEraseRoutine
 **********************************************************************************************************************/
/*! \brief       Invalidates the module and erases all blocks defined by the FlashBlock
 *               table that are occupied by the module being downloaded.
 *  \details     This routine will only do its job on programmable Modules.
 *               It checks the parsedModuleInfo addresses against FlashBlock
 *               table, and also checks if the corresponding module is allowed to erase a block.
 *
 *  \return      kFblOk / kFblFailed.
 **********************************************************************************************************************/
tFblResult FblEraseRoutine( void )
{
   tFblResult retVal = kFblOk;

#if defined( FBL_ENABLE_CALIBRATION_MODULES )
   if (     ( IsCalDownLd() ) && ( !IsCalFirstModInPartition() )  )
   {
      /* Erasure only on first calibration module in partition */
      return kFblOk;
   }
#endif

   /* In case of application, only application PSI information is revoked */
   if (kFblOk != ApplFblInvalidateBlock(*FblGetBlockDescriptor(FblHdrGetParsedModPartId())))
   {
      FblSetPecErr(Err_Revoke); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblDiag_Debug_0310 */
   }
   else
   {
#if defined( FBL_ENABLE_CALIBRATION_MODULES )
      if ( IsCalDownLd() )
      {
         /* Pec error set internally */
         retVal = FblHdrEraseCalPartition(FblHdrGetParsedModPartId());
      }
      else
#endif
      {
         /* This is the erasure for Application  */
         vuint8 blockNbr = GetBlockNbrFromPartId(FblHdrGetParsedModPartId());

#if (FBL_MTAB_NO_OF_BLOCKS > 1)
         if (UNDEFINED_LOG_BLOCK == blockNbr)
         {
            FblSetPecErr(Err_PartitionId); /* PRQA S 3109 */ /* MD_CBD_14.3 */
            retVal = kFblFailed;
         }
         else
#endif
         {
            /* Erase entire logical block for corresponding application */
            blockInfo.targetAddress = FblLogicalBlockTable.logicalBlock[blockNbr].blockStartAddress;
            blockInfo.targetLength = FblLogicalBlockTable.logicalBlock[blockNbr].blockLength;

            if (kFblMemStatus_Ok != FblMemBlockEraseIndication(&blockInfo))
            {
               FblSetPecErr(Err_EraseSW); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblDiag_Debug_0310 */
               retVal = kFblFailed;
            }
         }
      }
   }

   return retVal;
}

#if defined( FBL_ENABLE_VERIFY_INTEGRITY_WORD )
/***********************************************************************************************************************
 *  FblDiagCheckDldIntegrityWord
 **********************************************************************************************************************/
/*! \brief       Verify download data against Plain Header Integrity word.
 *  \details     This is done additionally to the hash verification inside FblHdrPostFlashValidation.
 *               Plain Header Integrity word might be used later to verify the application correctness.
 *  \return      kFblOk / kFblFailed
 **********************************************************************************************************************/
SecM_StatusType FblDiagCheckDldIntegrityWord( V_MEMRAM1 SecM_SignatureParamType V_MEMRAM2 V_MEMRAM3 * pVerifyParam ) /* PRQA S 3673 */ /* MD_FblDiag_3673 */
{
   SecM_StatusType retVal = SECM_VER_CRC;
   static vuint16 checksum;

      switch (pVerifyParam->sigState)
      {
         case kHashInit:
         {
            /* Init workSpace */
            checksum = 0x0000;
            retVal = SECM_VER_OK;
            break;
         }
         case kHashCompute:
         {
            /* callback */
            /* segmentlist from downlaod can be reused, it is checked to be identical to programmed */
            if ( kFblOk == ApplFblUpdateChecksum(&checksum, pVerifyParam->sigByteCount, pVerifyParam->sigSourceBuffer) )
            {
               retVal = SECM_VER_OK;
            }
            break;
         }
         case kHashFinalize:
         {
            /* Do nothing */
            retVal = SECM_VER_OK;
            break;
         }
         case kSigVerify:
         {
            if (kFblOk == ApplFblFinalizeChecksum(&checksum))
            {
               retVal = SECM_VER_OK;
            }
            break;
         }
         default:
         {
            break;
         }
      }

   return retVal;
}
#endif

/* Diagnostic service functions  **************************************************************************************/
/***********************************************************************************************************************
 *  FblEnterProgrammingMode
 **********************************************************************************************************************/
/*! \brief       This function sets the FBL states to indicate that programming-mode has been started.
 **********************************************************************************************************************/
static void FblEnterProgrammingMode( void )
{
   FblSleepCounterClear();
   SetEnablePrgMode();
   ApplFblSetVfp();
}

/***********************************************************************************************************************
 *  FblEnterRequestDownload
 **********************************************************************************************************************/
/*! \brief       This function is called when a Request-Download (0x34) service request is received, and when starting
 *               the FBL from the Operating Software.
 *  \details     The routine initializes decryption/decompression (if requested), and sets the FBL states to allow
 *               Transfer-Data requests.
 *  \param[in]   formatId Format Identifier from Request-Download service request
 *  \param[in]   uncompressedMemSize Number of Bytes to program from Request-Download service request
 **********************************************************************************************************************/
static void FblEnterRequestDownload( vuint8 formatId, tFblLength uncompressedMemSize )
{
   if (formatId != 0)
   {
#if defined( FBL_ENABLE_DATA_PROCESSING )
      if (kFblOk != ApplFblCheckDataFormatIdentifier(formatId))
      {
         FblErrDebugStatus(kDiagErrDataEncryption); /* PRQA S 3109 */ /* MD_CBD_14.3 */
      }
#else
      DiagNRCSubFunctionNotSupportedInvalidFormat();
      return;
#endif
   }

   /* Prepare LibMem for new module Download */
   DiagBuffer = FblMemInit();

   currMemLibSegment.dataFormat = formatId;

   if (kFblOk != ApplFblRequestDownload())
   {
      assertFblInternal(kDiagErrorNone != diagErrorCode, kFblAssertInternalApplFblCorrectErrSetting); /* PRQA S 2214 */ /* MD_FblDiag_2214 */
      return;
   }

#if defined( FBL_USE_FBL_AS_DEMOAPPL )
   /* This call was added in this demonstration bootloader (which acts as an application here) only
    * to show you what to do in your applications transition to the boot. */
   JumpToBootDemonstration(uncompressedMemSize,formatId);
#endif

   if (IO_E_OK != MemDriver_InitSync(NULL))
   {
      DiagNRCConditionsNotCorrectOrRequestSeqErr();
      FblErrDebugStatus(kDiagErrFlashcodeInitFailed); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblDiag_Debug_0310 */
      return;
   }

   SetMemDriverInitialized();
   /* Allow now reception of TransferData */
   SetTransferDataAllowed();
   transferState = DIAG_TRANSFERDATA_HDR_EXPECTED;

   CanInitTable.requestDownloadMemorySize = uncompressedMemSize;

   dataInDiagBuffer = 0;
   FblHdrInitRetry();
}

#if defined( FBL_ENABLE_INITIATE_DIAG_OP )
/***********************************************************************************************************************
 *  FblDiagInitiateDiagnosticOperation
 **********************************************************************************************************************/
/*! \brief       This function is called when a Initiate-Diagnostic-Operation 0x10) service request is received.
 *  \details     The function verifies that the request may be processed, then calls ApplFblInitiateDiagnosticOperation
 *               to process the request.
 **********************************************************************************************************************/
static void FblDiagInitiateDiagnosticOperation( void )
{
   if (!GetEnableDiagResp())
   {
      DiagProcessingDone(0);
      return;
   }

   if ((kDiagRqlInitiateDiagnosticOp != DiagDataLength) )
   {
      DiagNRCSubFunctionNotSupportedInvalidFormat();
      return;
   }

   ApplFblInitiateDiagnosticOperation(DiagBuffer[kDiagFmtSubparam]);
   DiagProcessingDone(kDiagRslInitiateDiagnosticOp);
}
#endif /* FBL_ENABLE_INITIATE_DIAG_OP */

/***********************************************************************************************************************
 *  FblDiagReadDataByIdentifier
 **********************************************************************************************************************/
/*! \brief       This function is called when a Read-Data-By-Identifier (0x1A) service request is received.
 *  \details     DIDs related to Vector-specific requests (stay-in-boot and retrieve extended status) are handled
 *               directly. All other DIDs are passed to ApplFblReadDataByIdentifier for processing.
 *               These IDs are already implemented in Core Module:
 *               - kDiagDidProgramingErrorCode
 *               - kDiagDidBootInitializationStatus
 *               - kDiagDidEcuId
 *               - kDiagDidBInfoSubjandECUName
 *               Only available in Integration Project State, not in Production state (for debugging/testing)
 *               - kDiagDidReadLastError (FBL_ENABLE_DEBUG_STATUS - Debug Info in case of Project State Integration)
 *               - kDiagDidECUOperatingStatus (FBL_ENABLE_STAY_IN_BOOT - Test/Debug option)
 *               - kBrsDiagLocalId (VGEN_ENABLE_BRS - should be never set in delivery, this is for testing purpose only)
 *               Note:
 *               Further DIDs are that are optional,
 *               or that may require modification are found in ApplFblReadDataByIdentifier
 **********************************************************************************************************************/
static void FblDiagReadDataByIdentifier( void )
{
   if ((kDiagRqlReadDataByIdentifier != DiagDataLength)
#if defined( VGEN_ENABLE_BRS )
       && ((0 == DiagDataLength) || (kBrsDiagLocalId != DiagBuffer[kDiagFmtDid]))
#endif
      )
   {
      if (GetEnableDiagResp())
      {
         DiagNRCSubFunctionNotSupportedInvalidFormat();
      }
      else
      {
         DiagProcessingDone(0);
      }
      return;
   }

#if defined( FBL_ENABLE_STAY_IN_BOOT )
   if (kDiagDidECUOperatingStatus == DiagBuffer[kDiagFmtDid])
   {
      /* A response message will be transmitted if either responses are allowed or the ECU is in the Stay-in-boot
       * phase after reset. */
      if (0 != (fblMode & FBL_START_WITH_PING))
      {
         DiagSetForceResponse();
      }

      DiagBuffer[kDiagFmtDid+1] = (vuint8)1;  /* FBL in boot-mode */
      DiagProcessingDone(kDiagRslReadDataByIdentifier + 1u);
      return;
   }
#endif

   if (!GetEnableDiagResp())
   {
      DiagProcessingDone(0);
      return;
   }

   if (kDiagDidProgramingErrorCode == DiagBuffer[kDiagFmtDid])
   {
      DiagBuffer[kDiagFmtDid + 1u] = (vuint8)(errPecErrorCode >>  8u);
      DiagBuffer[kDiagFmtDid + 2u] = (vuint8)(errPecErrorCode);
      DiagDataLength = sizeof(errPecErrorCode);
   }
#if defined( VGEN_ENABLE_BRS )
   else if (kBrsDiagLocalId == DiagBuffer[kDiagFmtDid])
   {
      (void)BrsCtrlPrecopy(&DiagBuffer[kDiagFmtDid + 1u], &DiagBuffer[kDiagFmtDid + 1u]);
      DiagSetForceResponse();
      DiagDataLength = 8u;
   }
#endif
   else if (kDiagDidBootInitializationStatus == DiagBuffer[kDiagFmtDid])
   {
      DiagBuffer[kDiagFmtDid + 1u] = FblGetBisLsbByte();
      DiagBuffer[kDiagFmtDid + 2u] = FblGetBisMsbByte();
      DiagDataLength = 2;
   }
   else if (kDiagDidEcuId == DiagBuffer[kDiagFmtDid])
   {
      (void) ApplFblNVMReadECUID(&DiagBuffer[kDiagFmtDid + 1u]);  /* PRQA S 0306 */ /* MD_FblDiag_0306 */ /* PRQA S 3425 */ /* MD_FblDiag_3425 */
      DiagDataLength = HDR_ECUID_LEN;
   }
   else if (kDiagDidBInfoSubjandECUName == DiagBuffer[kDiagFmtDid])
   {
      (void) MEMCPY(&DiagBuffer[kDiagFmtDid + 1u], FblHeader.SUBJNAME, HDR_SUBJNAME_LEN);
      (void) MEMCPY(&DiagBuffer[kDiagFmtDid + HDR_SUBJNAME_LEN + 1u], FblHeader.ECUNAME, HDR_ECUNAME_LEN);
      DiagDataLength = HDR_SUBJNAME_LEN + HDR_ECUNAME_LEN;
   }
#if defined( FBL_ENABLE_DEBUG_STATUS )
   else if (kDiagDidReadLastError == DiagBuffer[kDiagFmtDid])
   {
      tTpDataType i, j;

      DiagBuffer[kDiagFmtDid + 1u] = errStatLastServiceId;
      DiagBuffer[kDiagFmtDid + 2u] = (vuint8)(errStatErrorCode >>  8u);
      DiagBuffer[kDiagFmtDid + 3u] = (vuint8)(errStatErrorCode);
      if (errStatHaveDriver != 0)
      {
         DiagBuffer[kDiagFmtDid + 4u] = (vuint8)(errStatAddress >> 16u);
         DiagBuffer[kDiagFmtDid + 5u] = (vuint8)(errStatAddress >>  8u);
         DiagBuffer[kDiagFmtDid + 6u] = (vuint8)(errStatAddress);

         i = (tTpDataType) (kDiagFmtDid + 7u);
      }
      else if (kDiagErrTPFailed == errStatErrorCode)
      {
         DiagBuffer[kDiagFmtDid + 4u] = errStatTpError;
         i = (tTpDataType) (kDiagFmtDid + 5u);
      }
      else
      {
         i = (tTpDataType) (kDiagFmtDid + 4u);
      }

      if (errStatFileName != 0)
      {
         for (j = (tTpDataType)0; (errStatFileName[j] != (vuint8)0) && (i < (tTpDataType)FBL_DIAG_BUFFER_LENGTH); j++)
         {
            (void)FblRealTimeSupport();

            DiagBuffer[i] = (vuint8) errStatFileName[j];
            i++;
         }

         DiagBuffer[i] = (vuint8)0;
         i++;

         for (j = (tTpDataType)sizeof(vuint16); (j > (tTpDataType)0) && (i < (tTpDataType)FBL_DIAG_BUFFER_LENGTH); j--)
         {
            DiagBuffer[i] = (vuint8)(((vuint16)errStatLineNumber) >> (vuint8)( ( (vuint8)((vuint8)j-(vuint8)1) << (vuint8)3 ) ) );
            i++;
         }
      }
      DiagDataLength = i-2;
      ApplFblInitErrStatus();
   }
#endif
   else
   {
      /* Prepare identification data by callback function */
      ApplFblReadDataByIdentifier(&DiagBuffer[1], DiagDataLength);
   }

   DiagProcessingDone(kDiagRslReadDataByIdentifier + DiagDataLength);
}  /* PRQA S 6010 */ /* PRQA S 6030 */ /* PRQA S 6080 */ /* MD_FblDiag_60xx */

/***********************************************************************************************************************
 *  FblDiagReturnToNormalMode
 **********************************************************************************************************************/
/*! \brief       This function is called when a Return-To-Normal-Mode (0x20) service request is received.
 *  \details     The function resets the CAN controller & transceiver, resets the FBL state, and restarts the ECU.
 *
 **********************************************************************************************************************/
static void FblDiagReturnToNormalMode( void )
{
   if (!GetEnableDiagResp())
   {
      DiagProcessingDone(0);
      return;
   }

   if (kDiagRqlReturnToNormalMode != DiagDataLength)
   {
      DiagNRCSubFunctionNotSupportedInvalidFormat();
      return;
   }

   /* Disable tester present timeout monitoring */
   StopTesterTimeout();

   /* Turn off programming voltage  */
   ApplFblResetVfp();

   /* Transmit response if required */
   if (!GetEnablePrgMode())
   {

      DiagProcessingDone(kDiagRslReturnToNormalMode);

      while (kCanTxInProcess == FblCanMsgTransmitted())
      {
         (void)FblLookForWatchdog();
      }

   }
   else
   {
      /* avoid response pending is sent in case of no response required. */
      ClrP2Timer();
   }

#if defined( FBL_ENABLE_HIGHSPEED_MODE )
   if (GetHighspeedMode())
   {
      ApplTrcvrNormalMode();

      /* Reset CAN controller to low-speed mode */
      ApplFblDiagLowSpeed();

      /* Reinitialize CAN interface */
      FblCanInit();
   }
#endif

   /* De-initialize device-driver(s) */
   if ( GetMemDriverInitialized() )
   {
      (void)MemDriver_DeinitSync(NULL);
      ClrMemDriverInitialized();
   }

   if (GetEnablePrgMode())
   {
#if defined( FBL_ENABLE_HIGHSPEED_MODE )
      /* Delay reset for fast ECUs, FBL_RESET_DELAY parameter can be reduced from max 30 (ms) by "reset to communication" time:
       * Example "reset to communication in 10ms" -> FBL_RESET_DELAY may be reduced to 20
       * Example "reset to communication in 40ms" -> FBL_RESET_DELAY may be reduced to 0
       */
      vuint16 resetDelayCounter = FBL_RESET_MS_DELAY;
      while ( resetDelayCounter > 0 )
      {
         if (( FblRealTimeSupport() & FBL_TM_TRIGGERED) == FBL_TM_TRIGGERED)
         {
            resetDelayCounter--;
         }
      }
#endif
      fblStates = 0u;

#if defined( FBL_WATCHDOG_ON )
      /* Synchronize watchdog before transition */
      ApplFblWDLong();
#endif

      /* Stop timer to avoid a timer interrupt after application start */
      FblTimerStopp();

      /* Call reset application call-back */
      APPL_FBL_RESET();
      /* Code that is placed here will be never reached! */
   }
   else
   {
      fblStates            = 0;
      transferState        = DIAG_TRANSFERDATA_IDLE;

      DiagClrError();
      ClrP2Timer();
#if !defined( FBL_ENABLE_SECURITY_ACCESS )
      SetSecurityUnlock();
#endif
#if defined( FBL_ENABLE_CAN_CONFIGURATION )
      SetEnableDiagResp();
#endif
   }
} /* PRQA S 6010 */ /* PRQA S 6050 */ /* MD_FblDiag_60xx */

#if defined( FBL_ENABLE_READ_MEMORY_BY_ADDRESS )
/***********************************************************************************************************************
 *  FblDiagReadMemoryByAddress
 **********************************************************************************************************************/
/*! \brief       This function is called when a Read-Memory-By-Address (0x23) service request is received.
 *  \details     The function extracts the address and length parameters from the request (in DiagBuffer) and then calls
 *               ApplFblReadMemoryByAddress to process the request.
 **********************************************************************************************************************/
static void FblDiagReadMemoryByAddress( void )
{
   tFblAddress memoryAddress;
   tFblLength memorySize;

   if (!GetEnableDiagResp())
   {
      DiagProcessingDone(0);
      return;
   }

   if (kDiagRqlReadMemoryByAddress != DiagDataLength)
   {
      DiagNRCSubFunctionNotSupportedInvalidFormat();
      return;
   }

   memoryAddress = FblMemGetInteger(kDiagRqlReadMemoryByAddressAddressParameter, &DiagBuffer[kDiagFmtSubparam]);
   memorySize = FblMemGetInteger(kDiagRqlReadMemoryByAddressSizeParameter, &DiagBuffer[kDiagFmtSubparam + kDiagRqlReadMemoryByAddressAddressParameter]);

   if (memorySize > (FBL_DIAG_BUFFER_LENGTH - kDiagRslReadMemoryByAddress))
   {                                   /* Cannot allow request of more data than will fit in buffer */
      DiagNRCRequestOutOfRange();
   }
   else
   {
      ApplFblReadMemoryByAddress(memoryAddress, memorySize);
      DiagProcessingDone(kDiagRslReadMemoryByAddress + (tTpDataType)memorySize);
   }
}
#endif /* FBL_ENABLE_READ_MEMORY_BY_ADDRESS */

#if defined( FBL_ENABLE_SECURITY_ACCESS )
/***********************************************************************************************************************
 *  FblDiagSecurityAccess - GM specific
 **********************************************************************************************************************/
/*! \brief       This function is called when a Security-Access (0x27) service request is received.
 *  \details     The function checks that the correct conditions exist for handling the subfunction, and invokes the
 *               appropriate callback (ApplFblSecuritySeed, ApplFblSecurityKey, or ApplFblSecurityAccess).
 **********************************************************************************************************************/
static void FblDiagSecurityAccess( void )
{
   if (!GetEnableDiagResp())
   {
      DiagProcessingDone(0);
      return;
   }

   /* Check request length */
   if (DiagDataLength < kDiagRqlSecurityAccessSeed)
   {
      DiagNRCSubFunctionNotSupportedInvalidFormat();
      return;
   }

   switch (DiagBuffer[kDiagFmtSubparam] & kDiagSubSecTypeMask)
   {
      case kDiagSubRequestSeed:
      {
         vuint8 i;

         /* Check for correct request length. */
         if (kDiagRqlSecurityAccessSeed != DiagDataLength)
         {
            DiagNRCSubFunctionNotSupportedInvalidFormat();
            return;
         }

         /* Init response length for seed */
         DiagDataLength = kDiagRslSecurityAccessSeed;

         /* ECU already unlocked, send zero-seed */
         for(i = 0; i < kDiagSecSeedLength; i++)
         {
            DiagBuffer[kDiagFmtSeedKeyStart + i] = 0x00;
         }

         SetSecurityUnlock();
         break;
     }
     default:
     {
        ApplFblSecurityAccess(DiagBuffer[kDiagFmtSubparam]);
     }
   }
   DiagProcessingDone(DiagDataLength);
}
#endif /* FBL_ENABLE_SECURITY_ACCESS */

/***********************************************************************************************************************
 *  FblDiagDisableNormalCommunication
 **********************************************************************************************************************/
/*! \brief       This function is called when a Disable-Normal-Communication (0x28) service request is received.
 *  \details     The function sets the FBL state to indicate that normal communications are disabled and starts the
 *               tester-present timer.
 **********************************************************************************************************************/
static void FblDiagDisableNormalCommunication( void )
{
   if (kDiagRqlDisableNormalComm != DiagDataLength )   /* Only service id */
   {
      if (!GetEnableDiagResp())
      {
         DiagProcessingDone(0);
      }
      else
      {
         DiagNRCSubFunctionNotSupportedInvalidFormat();
      }
      return;
   }
   /* Set flag for disable normal communication */
   SetDisableComm();

   /* TesterPresent timeout active */
   if (TimeoutTesterValue() == 0)
   {
      ResetTesterTimeout();
   }

   DiagProcessingDone(kDiagRslDisableNormalComm);
}

/***********************************************************************************************************************
 *  FblDiagRequestDownload - GM specific
 **********************************************************************************************************************/
/*! \brief       This function is called when a Request-Download (0x34) service request is received.
 *  \details     The function verifies that the FBL is in the proper state to received data, and if so sets the FBL
 *               state to allow Transfer-Data (0x36) requests.
 **********************************************************************************************************************/
static void FblDiagRequestDownload( void )
{
   tFblLength uncompressedMemSize;DiagClrError();

   if (!GetEnableDiagResp())
   {
      DiagProcessingDone(0);
      return;
   }

   /* Check frame length */
   if (kDiagRqlRequestDownload != DiagDataLength)
   {
      DiagNRCSubFunctionNotSupportedInvalidFormat();
      return;
   }

   if ((!GetEnablePrgMode()) || (!GetSecurityUnlock()) )
   {
      FblErrDebugStatus(((!GetEnablePrgMode())? kDiagErrProgrammingModeDisabled: kDiagErrSecurityLocked)); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblDiag_Debug_0310 */
      DiagNRCConditionsNotCorrectOrRequestSeqErr();
      return;
   }

   if (GetTransferDataAllowed() )
   {
      FblErrDebugStatus(kDiagErrDownloadInProgress); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblDiag_Debug_0310 */
      DiagNRCConditionsNotCorrectOrRequestSeqErr();
      ClrTransferDataAllowed(); /* PRQA S 0277 */ /* MD_FblDiag_0277 */
      transferState = DIAG_TRANSFERDATA_IDLE;
      return;
   }

   uncompressedMemSize = FblMemGetInteger(kDiagRqlRequestDownloadMemorySizeParameter, &DiagBuffer[kDiagFmtReqDlMemSizeOffset]);
   FblEnterRequestDownload(DiagBuffer[kDiagFmtSubparam], uncompressedMemSize);

   /* Send response message */
   DiagProcessingDone(kDiagRslRequestDownload);
}

/***********************************************************************************************************************
 *  FblDiagTransferDataHeaderReceived
 **********************************************************************************************************************/
/*! \brief       This function is called on every first transfer data received after request download.
 *  \details     It parses the header inside the Diagnostic buffer and checks for the received module type.
 *               Depending on the module type it additionally does
 *               - a key update (Key replacement module)
 *               - memory erasure (application and calibration modules)
 *               - Routine init (on routine module)
 **********************************************************************************************************************/
static tFblResult FblDiagTransferDataHeaderReceived( void )
{
   tFblMemStatus memLibResult;
   vuint16 tmpHeaderLength;

   (void)FblRealTimeSupport();

   tmpHeaderLength = FblHdrProcessHeaderInfo(&DiagBuffer[kDiagFmtDataOffset]);

   {
      if (DOWNLOAD_MODULE_NOT_ACCEPTED == tmpHeaderLength)
      {
         /* Pec already set inside FblHdrProcessHeaderInfo, if applicable */
         return kFblFailed;
      }

#if defined( FBL_ENABLE_DATA_PROCESSING )
      if (MODULE_DF_RAW == parsedModuleInfo.dataFormat)
#endif
      {
         /* no data processing interface is being called, add offset directly, not via Dfi */
         diagBufferIndex += tmpHeaderLength;
         dataInDiagBuffer -= tmpHeaderLength;
      }
#if defined( FBL_ENABLE_SYSTEM_CHECK )
      {
         /* tmpHeaderLength is not included in segmentList */
         tFblLength  len = tmpHeaderLength;

         for (currDataSegIdx = 0; currDataSegIdx < parsedModuleInfo.segmentList.nrOfSegments; currDataSegIdx++)
         {
            len += parsedModuleInfo.segmentList.segmentInfo[currDataSegIdx].length;
         }

         if (len != CanInitTable.requestDownloadMemorySize)
         {
            FblSetPecErr((vuint16)((CanInitTable.requestDownloadMemorySize < len)? Err_MoreDataExpected : Err_LengthExceeded)); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblDiag_Debug_0310 */
            DiagNRCRequestOutOfRange();
            return kFblFailed;
         }
      }
#endif /* FBL_ENABLE_SYSTEM_CHECK */

      /* modules applicable for erasure are checked inside function,
         for non applicable modules function returns kFblOk. */
      if (kFblOk != FblEraseRoutine())
      {
         return kFblFailed;
      }

      currDataSegIdx = 0;
      currDataSegInfo = parsedModuleInfo.segmentList.segmentInfo[currDataSegIdx];
      currMemLibSegment.type = kFblMemType_ROM;
   }

#if defined( FBL_ENABLE_SYSTEM_CHECK )
   currDataSegInitialLen = currDataSegInfo.length;
#endif

   {
      (void)FblRealTimeSupport();

      /* Indicate the start of a module download */
      FblDiagSetMemLibMemBlockInfo();
      memLibResult = FblMemBlockStartIndication(&blockInfo);
      if (kFblMemStatus_Ok != memLibResult)
      {
         DiagSetError(memLibResult);
         return kFblFailed;
      }

      /* Indicate start of new segment */
      FblDiagSetMemLibSegmentAddrLengthInfo();
      memLibResult = FblMemSegmentStartIndication(&currMemLibSegment);
      if (kFblMemStatus_Ok != memLibResult)
      {
         DiagSetError(memLibResult);
         return kFblFailed;
      }
   }

   return kFblOk;
} /* PRQA S 6050 */ /* MD_FblDiag_60xx */

/***********************************************************************************************************************
 *  FblDiagTransferDataEnd
 **********************************************************************************************************************/
/*! \brief       When the last address-region of a module is downloaded, this function is called to handle the
 *               post-transfer tasks, such as checksum verification, signature verification, validation and device
 *               driver initialization.
 *  \return      kFblOk if the transferred data was successfully verified, kFblFailed if the verification failed.
 **********************************************************************************************************************/
static tFblResult FblDiagTransferDataEnd( void )
{
   tFblMemStatus memLibResult;

   memLibResult = FblMemBlockEndIndication();
   if (kFblMemStatus_Ok != memLibResult)
   {
      DiagSetError(memLibResult);
      return kFblFailed;
   }

   if (kFblOk != FblHdrPostFlashValidation())
   {
     /* Set NRC externally */
     return kFblFailed;
   }

   return FblHdrUpdatePSI();
}

/***********************************************************************************************************************
 *  FblDiagTransferDataProcess
 **********************************************************************************************************************/
/*! \brief       This function is called on every TransferData-request that passes checked preconditions inside
 *               FblDiagTransferData().
 *  \details     It checks current transferState and decides if the currently processed transfer is the first, an
 *               intermediate or the last transfer expected. FblDiagTransferDataHeaderReceived() is called for first
 *               transfer and FblDiagTransferDataEnd() is called for last transfer. The function interfaces with the
 *               LibMem module to program the received data.
 **********************************************************************************************************************/
static tFblResult FblDiagTransferDataProcess( void )
{
   tFblLength addrRegionLength;
   tFblMemStatus memLibResult;
   tTpDataType bytesToWrite;

   /* Put RCRRP message in CAN transmit buffer so that FblCanRetransmit()can be called from WD during device-driver
    * erase/write operations */
   DiagExRCRResponsePending(kForceSendResponsePending);

   if (transferState == DIAG_TRANSFERDATA_HDR_EXPECTED)
   {
      transferState = DIAG_TRANSFERDATA_IDLE;
      if (kFblOk != FblDiagTransferDataHeaderReceived())
      {
         return kFblFailed;
      }
   }

   while (dataInDiagBuffer > 0)
   {
      (void)FblRealTimeSupport();

      bytesToWrite = (tTpDataType) ((((tFblLength)dataInDiagBuffer > currDataSegInfo.length))? currDataSegInfo.length : dataInDiagBuffer);

      memLibResult = FblMemDataIndication(DiagBuffer, diagBufferIndex, bytesToWrite);
      if (kFblMemStatus_Ok != memLibResult)
      {
         DiagSetError(memLibResult);
         return kFblFailed;
      }

      currDataSegInfo.targetAddress += bytesToWrite;
      currDataSegInfo.length -= bytesToWrite;
      diagBufferIndex += bytesToWrite;
      dataInDiagBuffer -= bytesToWrite;

      if (currDataSegInfo.length == (tFblLength)0)
      {
         memLibResult = FblMemSegmentEndIndication(&addrRegionLength);
         if (kFblMemStatus_Ok != memLibResult)
         {
            DiagSetError(memLibResult);
            return kFblFailed;
         }
#if defined( FBL_ENABLE_SYSTEM_CHECK )
         if (addrRegionLength != currDataSegInitialLen)
         {
            FblErrDebugStatus(kDiagErrValueOutOfRange); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblDiag_Debug_0310 */
            DiagNRCRequestOutOfRange();
            return kFblFailed;
         }
#endif

         ++currDataSegIdx;
         if (currDataSegIdx < parsedModuleInfo.segmentList.nrOfSegments)
         {
            /* Start of new segment */
            currDataSegInfo = parsedModuleInfo.segmentList.segmentInfo[currDataSegIdx];
#if defined( FBL_ENABLE_SYSTEM_CHECK )
            currDataSegInitialLen = currDataSegInfo.length;
#endif

            FblDiagSetMemLibSegmentAddrLengthInfo();
            memLibResult = FblMemSegmentStartIndication(&currMemLibSegment);
            if (kFblMemStatus_Ok != memLibResult)
            {
               DiagSetError(memLibResult);
               return kFblFailed;
            }
         }
         else
         {
            /* transfer complete */
            if (dataInDiagBuffer != 0)
            {
               FblSetPecErr(Err_LengthExceeded); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblDiag_Debug_0310 */
               DiagNRCRequestOutOfRange();
               return kFblFailed;
            }
            else if (kFblOk != FblDiagTransferDataEnd())
            {
               return kFblFailed;
            }
            else
            {
               ClrTransferDataAllowed(); /* PRQA S 0277 */ /* MD_FblDiag_0277 */
               return kFblOk;
            }
         }
      }
   }
   return kFblOk;
}  /* PRQA S 6010 */ /* PRQA S 6030 */ /* PRQA S 6050 */ /* PRQA S 6080 */ /* MD_FblDiag_60xx */

/***********************************************************************************************************************
 *  FblDiagTransferData
 **********************************************************************************************************************/
/*! \brief       Checks Service format and current Bootloader state to allow transfer data service.
 *  \details     Initialize global data used for this function and for help functions FblDiagTransferDataKeyUpdate,
 *               FblDiagTransferDataProcess, FblDiagTransferDataHeaderReceived and FblDiagTransferDataEnd. Call
 *               ApplFblTransferData for user checks and FblDiagTransferDataProcess to process service request.
 **********************************************************************************************************************/
static void FblDiagTransferData( void )
{
   if (!GetEnableDiagResp())
   {
      DiagProcessingDone(0);
      return;
   }

   if (  (kDiagSubDownload != DiagBuffer[kDiagFmtSubparam])
         || ( (DiagDataLength <(HDR_MODULE_MAX_LEN+6)) && (transferState == DIAG_TRANSFERDATA_HDR_EXPECTED) )
      )
   {
#if defined( FBL_ENABLE_DEBUG_STATUS )
      /* HDR_MODULE_MAX_LEN is the minimum allowed Diagbuffer size */
      if ( (DiagDataLength <(HDR_MODULE_MAX_LEN+6))  && (transferState == DIAG_TRANSFERDATA_HDR_EXPECTED) )
      {
          FblErrDebugStatus(kDiagErrTransferTooSmall); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblDiag_Debug_0310 */
      }
#endif
      DiagNRCSubFunctionNotSupportedInvalidFormat();
      ClrTransferDataAllowed(); /* PRQA S 0277 */ /* MD_FblDiag_0277 */
      transferState = DIAG_TRANSFERDATA_IDLE;
      return;
   }

   if (!GetTransferDataAllowed())
   {
      DiagNRCConditionsNotCorrectOrRequestSeqErr();
      return;
   }

   dataInDiagBuffer = DiagDataLength - kDiagRqlTransferData;
   diagBufferIndex = kDiagFmtDataOffset;

   if ((kFblOk != ApplFblTransferData()) || (kFblOk != FblDiagTransferDataProcess())) /* PRQA S 3415 */ /* MD_FblDiag_3415 */
   {
      if (kDiagErrorNone == DiagGetError())
      {
        /* This is the default NRC, set it globally */
        DiagNRCGeneralProgFailure();
      }
      FblHdrClrCalPartDldActive(); /* PRQA S 3109 */ /* MD_CBD_14.3 */
      ClrTransferDataAllowed(); /* PRQA S 0277 */ /* MD_FblDiag_0277 */
      transferState = DIAG_TRANSFERDATA_IDLE;
   }

   DiagBuffer = FblMemGetActiveBuffer();
   DiagProcessingDone(kDiagRslTransferData);
}

#if defined( FBL_ENABLE_WRITE_DATA_BY_IDENTIFIER )
/***********************************************************************************************************************
 *  FblDiagWriteDataByIdentifier
 **********************************************************************************************************************/
/*! \brief       This function is called when a Write-Data-By-Identifier (0x3B) service request is received.
 *  \details     The function validates the request, and calls ApplFblWriteDataByIdentifer for processing.
 **********************************************************************************************************************/
static void FblDiagWriteDataByIdentifier( void )
{
   if (!GetEnableDiagResp())
   {
      DiagProcessingDone(0);
      return;
   }

   if (DiagDataLength < kDiagRqlWriteDataByIdentifier)
   {
      DiagNRCSubFunctionNotSupportedInvalidFormat();
      return;
   }

   (void)ApplFblWriteDataByIdentifier(&DiagBuffer[1], DiagDataLength);
   DiagProcessingDone(kDiagRslWriteDataByIdentifier);
}
#endif

/***********************************************************************************************************************
 *  FblDiagTesterPresent
 **********************************************************************************************************************/
/*! \brief       This function is called when a Tester-Present (0x3E) service request is received.
 *  \details     The function validates the request, and resets the tester-present timer (if it is running).
 *               Note: This function is called only if the service request was physically-addressed.
 *                     Functionally-Addressed requests are handled directly in FblDiagPrecopy so that this service
 *                     may be interleaved between segmented physically addressed messages (such as Transfer-Data)
 *                     without corrupting DiagBuffer[].
 **********************************************************************************************************************/
static void FblDiagTesterPresent( void )
{
   if (kDiagRqlTesterPresent != DiagDataLength)
   {
      if (!GetEnableDiagResp())
      {
         DiagProcessingDone(0);
      }
      else
      {
         DiagNRCSubFunctionNotSupportedInvalidFormat();
      }
   }
   else
   {
      /* Reinitialize tester present timer */
      if (TimeoutTesterValue() != (vuint16)0)
      {
         ResetTesterTimeout();
      }
      DiagProcessingDone(kDiagRslTesterPresent);
   }
}

/***********************************************************************************************************************
 *  FblDiagReportProgrammedState
 **********************************************************************************************************************/
/*! \brief       This function is called when a Report-Programmed-State (0xA2) service request is received.
 *  \details     The function validates the request, then calls ApplFblReportProgrammedState to retrieve the state
 *               response. Also, the function may enable responses to requests if normal communications have been
 *               disabled.
 **********************************************************************************************************************/
static void FblDiagReportProgrammedState( void )
{
   if ((kDiagRqlReportProgrammedState != DiagDataLength))
   {
      if (!GetEnableDiagResp())
      {
         DiagProcessingDone(0);
      }
      else
      {
         DiagNRCSubFunctionNotSupportedInvalidFormat();
      }
      return;
   }

   if ((!GetEnableDiagResp()) && GetDisableComm())
   {
      SetEnableDiagResp();
      DiagSetForceResponse();
   }

   if (GetTransferDataAllowed())
   {
      FblErrDebugStatus(kDiagErrDownloadInProgress); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblDiag_Debug_0310 */
      DiagNRCConditionsNotCorrectOrRequestSeqErr();
      ClrTransferDataAllowed(); /* PRQA S 0277 */ /* MD_FblDiag_0277 */
      transferState = DIAG_TRANSFERDATA_IDLE;
      return;
   }

   DiagBuffer[kDiagFmtSubparam] = ApplFblReportProgrammedState();
   DiagProcessingDone(kDiagRslReportProgrammedState);
}

/***********************************************************************************************************************
 *  FblDiagProgrammingMode
 **********************************************************************************************************************/
/*! \brief       This function is called when a Programming-Mode (0xA5) service request is received.
 *  \details     The function validates the request, and responds per the subfunction parameter contained in the request.
 **********************************************************************************************************************/
static void FblDiagProgrammingMode( void )
{
   if (!GetEnableDiagResp())
   {
      /* No response allowed at this time. This is an invalid action here, means sequence error */
      DiagProcessingDone(0);
      return;
   }

   if (kDiagRqlProgrammingMode != DiagDataLength)
   {
      DiagNRCSubFunctionNotSupportedInvalidFormat();
      return;
   }

   if (GetEnablePrgMode())
   {
      DiagNRCConditionsNotCorrectOrRequestSeqErr();
      return;
   }

   if (kDiagSubRequestProgMode == DiagBuffer[kDiagFmtSubparam])
   {
      if (!GetDisableComm())
      {
         DiagNRCConditionsNotCorrectOrRequestSeqErr();
      }
      else if (kFblOk != ApplFblRequestPrgMode(kDiagSubRequestProgMode))
      {
         DiagNRCConditionsNotCorrectOrRequestSeqErr();
      }
      else
      {
         ClrHighspeedMode(); /* No high speed programming mode */ /* PRQA S 0277 */ /* MD_FblDiag_0277 */
         SetRequestPrgMode(); /* Set request programming mode flag */
      }
   }

#if defined( FBL_ENABLE_HIGHSPEED_MODE )
   else if (kDiagSubRequestProgModeHSpeed == DiagBuffer[kDiagFmtSubparam])
   {
      if (!GetDisableComm())
      {
         DiagNRCConditionsNotCorrectOrRequestSeqErr();
      }
      else if (kFblOk != ApplFblRequestPrgMode(kDiagSubRequestProgModeHSpeed))
      {
         DiagNRCConditionsNotCorrectOrRequestSeqErr();
      }
      else
      {
         SetHighspeedMode(); /* High speed programming mode */
         SetRequestPrgMode(); /* Set request programming mode flag */
      }
   }
#endif

   else if (kDiagSubEnableProgrammingMode == DiagBuffer[kDiagFmtSubparam])
   {
      /* Was programming mode already requested ?*/
      if (!GetRequestPrgMode())
      {
         /* Answer with negative response */
         DiagNRCConditionsNotCorrectOrRequestSeqErr();
      }
      else
      {
#if defined( FBL_ENABLE_HIGHSPEED_MODE )
         /* Enter high speed programming mode if requested */
         if (GetHighspeedMode())
         {
            /* Copy high speed baudrate */
            ApplFblDiagHighSpeed();

            /* Reinitialize CAN interface */
            FblCanInit();
            ApplTrcvrHighSpeedMode();
         }
#endif

         FblEnterProgrammingMode();
         ApplFblEnablePrgMode();

         DiagSetNoResponse();
      }
   }

   else
   {
      DiagNRCSubFunctionNotSupportedInvalidFormat();
      if (GetRequestPrgMode() && (!GetEnablePrgMode()) )
      {
         ClrRequestPrgMode(); /* PRQA S 0277 */ /* MD_FblDiag_0277 */
         ClrHighspeedMode(); /* PRQA S 0277 */ /* MD_FblDiag_0277 */
      }
   }

   DiagProcessingDone(kDiagRslProgrammingMode);
}  /* PRQA S 6010 */ /* PRQA S 6030 */ /* PRQA S 6080 */ /* MD_FblDiag_60xx */

/***********************************************************************************************************************
 *  FblDiagPrecopy - GM specific
 **********************************************************************************************************************/
/*! \brief       Function for support of functionally addressed requests
 *  \param[in]   data Receive data value (not used in each case)
 **********************************************************************************************************************/
# if defined( MULTIPLE_RECEIVE_BUFFER )
void FblDiagPrecopy( pChipDataPtr data )
{
# endif

   /* -- Check Address -- */
   if ((kDiagAllNodeAddr != EXTADDR)
# if defined( FBL_ENABLE_INITIATE_DIAG_OP )
       && (kDiagGatewayAddr != EXTADDR)
# endif
# if defined( FBL_ENABLE_STAY_IN_BOOT )
       /* Process this only if waiting for PING message */
# if defined( MULTIPLE_RECEIVE_BUFFER ) 
       && (kFblOk != FblCheckOwnStartMsg(data))
# endif
# endif
      )
   {
      return;                 /* ECU is not addressed */
   }

   /* Handle TesterPresent request here */
   if (kDiagSidTesterPresent == SRV_ID)
   {
      if ((TPCI & 0x0Fu) == (kDiagRqlTesterPresent + 1u))
      {
         if (TimeoutTesterValue() != 0)
         {
            ResetTesterTimeout();
         }
         FblSleepCounterReload();
      }
   }
   else if (!GetResponseProcessing())
   {
      /* Copy message into diag buffer */
      DiagBuffer[0] = (&TPCI)[1];
      DiagBuffer[1] = (&TPCI)[2];
      DiagBuffer[2] = (&TPCI)[3];
      DiagBuffer[3] = (&TPCI)[4];
      DiagBuffer[4] = (&TPCI)[5];
      DiagBuffer[5] = (&TPCI)[6];
      DiagBuffer[6] = (&TPCI)[7];

      /* Set flag for functional addressing */
      SetFunctionalRequest();

      /* Handle functional requests like physical ones */
      FblDataInd((vuint8)(TPCI & 0x0Fu));
   }
   else
   {
      /* Diag response is currently processed, discard message */
   }

}

/***********************************************************************************************************************
 *  FblDiagDataInd - GM specific
 **********************************************************************************************************************/
/*! \brief       Indication function for a transport layer message (diag request)
 *  \param[in]   rxDataLen Number of received bytes
 **********************************************************************************************************************/
void FblDataInd( tTpDataType rxDataLen )
{
   DiagClrError();
   diagResponseFlag = kDiagPutResponse;

   FblSleepCounterReload();

   SetP2Timer(kFblDiagTimeP2);   /* Reset P2 timer (initial timing) */
   DiagDataLength = rxDataLen - 1u;
   diagServiceCurrent = DiagBuffer[kDiagFmtServiceId];

   SetDiagIndication();

#if defined( FBL_MEM_ENABLE_PIPELINING )
   if (FblMemTaskIsPending())
   {
      FblMemRxNotification();
   }
#endif /* FBL_MEM_ENABLE_PIPELINING */

   if (!GetEnableDiagResp())
   {
      DiagSetNoResponse();
   }
}
/***********************************************************************************************************************
 *  FblDiagTimerTask - GM specific
 **********************************************************************************************************************/
/*! \brief       This function dispatches all services requests to the appropriate service handler.
 *  \details     The function also handles the surveillance of the tester-present timer. When a timeout occurs the
 *               startup code of the FBL is executed. The function also manages the security-access delay timer.
 **********************************************************************************************************************/
void FblDiagTimerTask(void)
{
   /* Tester present timing */
   if (TimeoutTesterValue()!=0)
   {
      DecTimeoutTesterValue();
      if (TimeoutTesterValue() == 0)
      {
         /* Tester present timer expired, exit bootloader via Reset  */
         DiagDataLength = kDiagRqlReturnToNormalMode;
         DiagBuffer[kDiagFmtServiceId] = kDiagSidReturnToNormalMode;
         diagServiceCurrent = kDiagSidReturnToNormalMode;
         FblDiagReturnToNormalMode();
      }
   }

   SetDiagInProgress();

   if (GetDiagIndication())
   {
      ClrDiagIndication();  /* PRQA S 0277 *//* MD_FblDiag_0277 */
      DiagClrError();

      /* Parse Service IDs supported in default session */
      switch (diagServiceCurrent)
      {
#if defined( FBL_ENABLE_INITIATE_DIAG_OP )
         case kDiagSidInitiateDiagnosticOp:
            FblDiagInitiateDiagnosticOperation();
            break;
#endif

         case kDiagSidReadDataByIdentifier:
            FblDiagReadDataByIdentifier();
            break;

         case kDiagSidReturnToNormalMode:
            FblDiagReturnToNormalMode();
            break;

#if defined( FBL_ENABLE_READ_MEMORY_BY_ADDRESS )
         case kDiagSidReadMemoryByAddress:
            FblDiagReadMemoryByAddress();
            break;
#endif

#if defined( FBL_ENABLE_SECURITY_ACCESS )
         case kDiagSidSecurityAccess:
            FblDiagSecurityAccess();
            break;
#endif

         case kDiagSidDisableNormalComm:
            FblDiagDisableNormalCommunication();
            break;

         case kDiagSidRequestDownload:
            FblDiagRequestDownload();
            break;

#if defined( FBL_USE_FBL_AS_DEMOAPPL )
#else
         case kDiagSidTransferData:
            FblDiagTransferData();
            break;
#endif

#if defined( FBL_ENABLE_WRITE_DATA_BY_IDENTIFIER )
         case kDiagSidWriteDataByIdentifier:
            FblDiagWriteDataByIdentifier();
            break;
#endif

         case kDiagSidTesterPresent:
            FblDiagTesterPresent();
            break;

         case kDiagSidReportProgrammedState:
            FblDiagReportProgrammedState();
            break;

         case kDiagSidProgrammingMode:
            FblDiagProgrammingMode();
            break;

         default:
            if (!GetFunctionalRequest())
            {
               DiagNRCServiceNotSupported();
            }
            break;
      }
      /* -- 14230 specific -- */
      if (kDiagErrorNone != DiagGetError())
      {
         DiagResponseProcessor(0);
      }

      /* Reset flag for functional request after processing */
      ClrFunctionalRequest(); /* PRQA S 0277 *//* MD_FblDiag_0277 */
   }

   ClrDiagInProgress();   /* PRQA S 0277 *//* MD_FblDiag_0277 */

#if defined( FBL_ENABLE_APPL_TASK )
   /* Call cyclic function of fbl application module (if configured) */
   ApplFblTask();
#endif
}   /* PRQA S 6030 */ /* PRQA S 6050 */ /* MD_FblDiag_60xx */

/* module specific MISRA deviations:

   MD_FblDiag_0277:
     Reason : Usage of FblInvert16Bit() macro implies usage of ~ operator and appropriate casts. This includes a
      cast from negative integer to unsigned value. The integer is only an intermediate type that occurs because of
      integer promotion while using ~ operator, though only a bit mask is operated on.
     Risk : No risk.
     Prevention : No prevention defined.

   MD_FblDiag_Debug_0310
     Reason: The compiler macro __FILE__ is casted to (vuint8 *) object type. The definition of __FILE__ is compiler
             platform specific.
     Risk: The file name information is only used for analysis purpose. Moreover the code is only available
           if Debug Status configuration (FBL_ENABLE_DEBUG_STATUS is set), which is only the case in GENy project
           state "Integration". For GENy project state "Production" the code is no more executed.
     Prevention: Change to Project State "Production" in GENy to disable Debug Status.

   MD_FblDiag_3415:
     Reason: Both calls have a uniform reaction, the call to FblDiagTransferDataProcess() need not be executed if the
             call to ApplFblTransferData() returns kFblFailed. No further transfer is allowed in this situation, a Request
             Download has to be send first again.
     Risk: No risk
     Prevention: No prevention

   MD_FblDiag_3673:
     Reason: Standard Api that cannot easily be changed.
     Risk: Decreased readability, function may be compiled with less efficient code.
     Prevention: No prevention defined.

   MD_FblDiag_0306:
     Reason: Address conversion between integer values and pointers is required to allow for hardware independent configuration and address range checks.
     Risk: The size of integer required to hold the result of a pointer cast is implementation-defined.
     Prevention: The size of the respective integer data type which holds the address value is adapted on a hardware specific basis.

   MD_FblDiag_0851:
     Reason: The used parameter is a macro that may be empty or not depending on the given use case.
     Risk: Non matching use cases may be choosen for Diag / hw-layer.
     Prevention: Macro Parameter is defined in HW-layer.

   MD_FblDiag_2214:
     Reason: This is an assertion macro used only for debugging in development (FBL_ENABLE_ASSERTION).
     Risk: No risk.
     Prevention: No prevention defined.

   MD_FblDiag_3109_RealtimeInLoop:
     Reason: This happens only in Misra Test Environment (Canoe Emu), where FblRealTimeSupport is just an empty macro.
     Risk: No risk.
     Prevention: No prevention defined.

   MD_FblDiag_3425
     Reason: This message does not make sense in this context. both sides of the : expression are simple constants without side effect.
     Risk: No risk.
     Prevention: No prevention defined.

   MD_FblDiag_60xx
     Reason: Monolytic function for several sepparable features choosen in order to save code size
     Risk: Complexity decreases readability and maintainability
     Prevention: Catch complexity through (regression) testing
*/

/***********************************************************************************************************************
 *  END OF FILE: FBL_DIAG.C
 **********************************************************************************************************************/
