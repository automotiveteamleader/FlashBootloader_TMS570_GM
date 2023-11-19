/* Kernbauer Version: 1.12 Konfiguration: FBL Erzeugungsgangnummer: 1 */

/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file        fbl_apdi.h
 *  \brief       Diagnostic user callbacks/hooks
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
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY (KbFbl_apdiFrameOem.c, GM SLP5 and SLP6)
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  05.00.00   2014-01-31  AWh                      First release
 *  05.01.00   2014-07-14  AWh                      No changes
 *  05.02.00   2015-01-15  AWH     ESCAN00078960    Gm SLP6: Make allowed Tester addresses configurable
 *                                 ESCAN00080504    No changes
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
 *  INCLUDES
 **********************************************************************************************************************/

#include "fbl_inc.h"

/***********************************************************************************************************************
 *   VERSION
 **********************************************************************************************************************/

#if ( FBLKBAPI_FRAMEDIAG_GM_VERSION != 0x0502u )
# error "Error in fbl_apdi.c: Source and Header file are inconsistent!"
#endif
#if ( FBLKBAPI_FRAMEDIAG_GM_RELEASE_VERSION != 0x00u )
# error "Error in fbl_apdi.c: Source and Header file are inconsistent!"
#endif

#if ( FBLKBAPI_FRAMEDIAG_GM_VERSION != _FBLKBAPI_FRAMEDIAG_OEM_VERSION ) || \
    ( FBLKBAPI_FRAMEDIAG_GM_RELEASE_VERSION != _FBLKBAPI_FRAMEDIAG_OEM_RELEASE_VERSION )
# error "Error in fbl_apdi.c: Source and v_ver.h are inconsistent!"
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

static tTpDataType ApplFblRdbidProgrammedStateInd(vuint8 * pbDiagData);

/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/

# if defined( FBL_ENABLE_INITIATE_DIAG_OP )
/***********************************************************************************************************************
 *  ApplFblInitiateDiagnosticOperation
 **********************************************************************************************************************/
/*! \brief       This function is called upon receipt of the Initiate-Diagnostic-Operation (0x10) service request.
 *  \details     The routine must perform that task indicated by the sub-function parameter, or indicate error by
 *               invoking one of the error macros (DiagNRC<condition>).
 *  \param[in]   subfunction Value of subfunction parameter supplied in diagnostic request
 **********************************************************************************************************************/
void ApplFblInitiateDiagnosticOperation( vuint8 subfunction )
{
#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   subfunction = subfunction;
#endif

   DiagNRCSubFunctionNotSupportedInvalidFormat();
}
# endif /* FBL_ENABLE_INITIATE_DIAG_OP */

# if defined( FBL_ENABLE_READ_MEMORY_BY_ADDRESS )
/***********************************************************************************************************************
 *  FblReadMemoryByAddress
 **********************************************************************************************************************/
/*! \brief       This function is called when a Read-Memory-By-Address (0x23) service request is received.
 *  \details     The function obtains a specified number of bytes starting at a specified address.
 *               The data to be returned must be placed in DiagBuffer[].
 *               If the data cannot be read, one of the error macros (DiagNRC<condition>) must be called.
 *               This routine is responsible for checking that the appropriate
 *               security access level is set before retrieving the data.
 *  \param[in]   address Starting address of data to be obtained.
 *  \param[in]   len Number of bytes of data to be obtained.
 **********************************************************************************************************************/
void ApplFblReadMemoryByAddress( tFblAddress address, FBL_MEMSIZE_TYPE len )
{
   /* If there are security concerns, implement them here */

   if (FblReadProm(address, &DiagBuffer[kDiagReadMemoryByAddrData], len) != len )
   {
      DiagNRCRequestOutOfRange();
   }
}
# endif /* FBL_ENABLE_READ_MEMORY_BY_ADDRESS */

#if defined( FBL_ENABLE_SECURITY_ACCESS )
/***********************************************************************************************************************
 *  ApplFblSecurityAccess
 **********************************************************************************************************************/
/*! \brief       This function is called when a Security-Access (0x27) request is received with any subfunction other than
 *               seed (1) or key (2).
 *  \details     If the subfunction cannot be completed, the function must indicate an error using one of the error
 *               macros (DiagNRC<condition>).
 *  \param[in]   subfunction Value of subfunction parameter supplied in diagnostic request
 **********************************************************************************************************************/
void ApplFblSecurityAccess( vuint8 subfunction )
{
#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   subfunction = subfunction;
#endif

   if (kDiagSubSendKey == subfunction)
   {
      /* Force 'key' request to be discarded */
      DiagSetNoResponse();
   }
   else
   {
      DiagNRCSubFunctionNotSupportedInvalidFormat();
   }
}
#endif /* FBL_ENABLE_SECURITY_ACCESS */

/***********************************************************************************************************************
 *  ApplFblRequestDownload
 **********************************************************************************************************************/
/*! \brief       This function is called when the FBL is started by the Operating Software, and when a Request-Download (0x34) service request is received.
 *  \details     The function should determine if download should be allowed to proceed.
 *               If not, then the appropriate error should be indicated using one of the error macros
 *               (DiagNRC<condition>).
 *  \return      kFblOk or kFblFailed.
 **********************************************************************************************************************/
tFblResult ApplFblRequestDownload( void )
{
   return kFblOk;
}

/***********************************************************************************************************************
 *  ApplFblTransferData
 **********************************************************************************************************************/
/*! \brief       This function is called upon receipt of the Transfer-Data (0x36) service request.
 *  \details     The function should determine if download should be allowed to proceed.
 *               If not, then the appropriate error should be indicated using one of the error macros
 *               (DiagNRC<condition>).
 *  \return      kFblOk or kFblFailed.
 **********************************************************************************************************************/
tFblResult ApplFblTransferData( void )
{
   /*
    * You may add checks to ensure that the download may continue.
    * (such as programming-voltage in range).
    */
   return kFblOk;
}

/***********************************************************************************************************************
 *  ApplFblReadDataByIdentifier
 **********************************************************************************************************************/
/*! \brief         ReadDataByIdentifier service function.
 *  \param[in,out] pbDiagData Pointer to diag service data (after SID!)
 *  \param[in]     diagReqDataLen Service data length (without SID!)
 **********************************************************************************************************************/
void ApplFblReadDataByIdentifier( vuint8 * pbDiagData, tTpDataType diagReqDataLen )
{
   vuint8 mid; /* module Id*/
   vuint8 did; /* Data Identifier */
   tFblAddress moduleHdrAddr; /* start address of module's header in Rom */
   vuint8 hdrIdx; /* index to plain header data */

#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   pbDiagData = pbDiagData;
   diagReqDataLen = diagReqDataLen;
#endif

   did = pbDiagData[0];

   switch (did)
   {
      case kDiagDidProgrammedStateIndicator:
         DiagDataLength = ApplFblRdbidProgrammedStateInd(&pbDiagData[1]);
         break;

      /* Application specific DIDs */
      /* Here are the Corporate Standard Data Identifiers per GMW3110.  */
      /* You should implement these as required by your CTS. */
      case kDiagDidSystemSupplierID:         /* DID 0x92 */
      case kDiagDidManufEnableCounter:       /* DID 0xA0 */
      case kDiagDidManufTraceabilityChars:   /* DID 0xB4 */
      case kDiagDidGmBroadcastCode:          /* DID 0xB5 */
      case kDiagDidEndModelPartNr:           /* DID 0xCB */
      case kDiagDidBaseModelPartNr:          /* DID 0xCC */
      case kDiagDidEndModelPartNrAlpha:      /* DID 0xDB */
      case kDiagDidBaseModelPartNrAlpha:     /* DID 0xDC */
      case kDiagDidSoftwareModuleDataIds:    /* DID 0xDD */
         DiagNRCRequestOutOfRange();
         break;
      /* DID 0xB0: Diagnostics Node Address */
      case kDiagDidECUDiagnosticAddr:
         pbDiagData[1] = diagNodeAddr;   /* ECUaddress       */
         DiagDataLength = 1;
         break;

      /* DID 0xC0: Boot Software Part Number (BSPN) */
      case kDiagDidBootSoftwarePartNr:
         (void) FblReadBlock( (tFblAddress) FblHeader.SWMI, &pbDiagData[1], HDR_HEXPN_LEN );
         DiagDataLength = HDR_HEXPN_LEN;
         break;

      /* DID 0xC1 - 0xCA: Software Module Identifier (SWMI) */
      case kDiagDidSwmi01:
      case kDiagDidSwmi02:
      case kDiagDidSwmi03:
      case kDiagDidSwmi04:
      case kDiagDidSwmi05:
      case kDiagDidSwmi06:
      case kDiagDidSwmi07:
      case kDiagDidSwmi08:
      case kDiagDidSwmi09:
      case kDiagDidSwmi10:
         mid = MODID_OPSW_1 + (pbDiagData[(vuint8)0] - kDiagDidSwmi01);
         moduleHdrAddr = FblHdrCheckModuleValidAndGetAddr(mid);

         if (NO_HEADER_ADDR_DEFINED == moduleHdrAddr)
         {
            DiagNRCConditionsNotCorrectOrRequestSeqErr();
         }
         else
         {
            hdrIdx = (kDiagDidSwmi01 == did)? P1_APP_HEXPN_IDX : P1_CAL_HEXPN_IDX;
            FblReadProm( (moduleHdrAddr+hdrIdx), &pbDiagData[1], HDR_HEXPN_LEN );
            DiagDataLength = HDR_HEXPN_LEN;
         }
         break;

      /* DID 0xD0: Boot Software Part Number Alpha Code */
      case kDiagDidBootSoftwareAlpha:
         (void) FblReadBlock( (tFblAddress) FblHeader.DLS, &pbDiagData[1], HDR_DLS_LEN );
         DiagDataLength = HDR_DLS_LEN;
         break;

      /* DID 0xD1 - 0xDA: Software Module Identifier Alpha Code (SWMIAC/DLS) */
      case kDiagDidAlpha01:
      case kDiagDidAlpha02:
      case kDiagDidAlpha03:
      case kDiagDidAlpha04:
      case kDiagDidAlpha05:
      case kDiagDidAlpha06:
      case kDiagDidAlpha07:
      case kDiagDidAlpha08:
      case kDiagDidAlpha09:
      case kDiagDidAlpha10:
         mid = MODID_OPSW_1 + (pbDiagData[(vuint8)0] - kDiagDidAlpha01);
         moduleHdrAddr = FblHdrCheckModuleValidAndGetAddr(mid);

         if (NO_HEADER_ADDR_DEFINED == moduleHdrAddr)
         {
            DiagNRCConditionsNotCorrectOrRequestSeqErr();
         }
         else
         {
            hdrIdx = (kDiagDidAlpha01 == did)? P1_APP_DLS_IDX : P1_CAL_DLS_IDX;
            FblReadProm( (moduleHdrAddr+hdrIdx), &pbDiagData[1], HDR_DLS_LEN );
            DiagDataLength = HDR_DLS_LEN;
         }
         break;

      default:
         DiagNRCRequestOutOfRange();
         break;
   }
}  /* PRQA S 2006 */ /* MD_CBD_14.7 */

#if defined( FBL_ENABLE_WRITE_DATA_BY_IDENTIFIER )
/***********************************************************************************************************************
 *  ApplDiagWriteDataByIdentifier
 **********************************************************************************************************************/
/*! \brief         WriteDataByIdentifier service function.
 *  \param[in,out] pbDiagData Pointer to diag service data (after SID!)
 *  \param[in]     diagReqDataLen Service data length (without SID!)
 *  \return        kFblOk: WriteDataByIdentifer was successful, KFblFailed: WriteDataByIdentifier failed.
 **********************************************************************************************************************/
vuint8 ApplFblWriteDataByIdentifier( vuint8 * pbDiagData, tTpDataType diagReqDataLen )
{
#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   pbDiagData = pbDiagData;
   diagReqDataLen = diagReqDataLen;
#endif

   switch (pbDiagData[(vuint8)0])
   {
      /*
       * Add cases for each supported data-identifier.
       * See ApplFblReadDataByIdentifier() for names of standard identifiers.
       */

      default:
         DiagNRCRequestOutOfRange();
         break;
   }

   return kFblOk;
}
#endif /* FBL_ENABLE_WRITE_DATA_BY_IDENTIFIER */

/***********************************************************************************************************************
 *  ApplFblReportProgrammedState
 **********************************************************************************************************************/
/*! \brief       This function is called upon receipt of the Report-Programmed-State (0xA2) service request.
 *               The function must return the current programmed state of the ECU.
 *  \return      ECU state See values of kDiagProgState<state> macros.
 **********************************************************************************************************************/
vuint8 ApplFblReportProgrammedState( void )
{
#if defined( FBL_ENABLE_ROM_INTEGRITY_CHECK ) || \
    defined( FBL_ENABLE_RAM_INTEGRITY_CHECK )
   /*
    * Note: fblProgrammedState also contains a bit for under-run
    *       handling, but the Diagnostics Layer handles this.
    *       So, there is no need to check for under-run from here.
    */
   if (fblProgrammedState != 0)
   {
# if defined( FBL_ENABLE_ROM_INTEGRITY_CHECK )
      if ((fblProgrammedState & kProgrammedStateRomError) != 0)
      {
         fblProgrammedState &= (vuint8)~(vuint32)kProgrammedStateRomError;
         return kDiagProgStateBootFault;
      }
# endif
# if defined( FBL_ENABLE_RAM_INTEGRITY_CHECK )
      if ((fblProgrammedState & kProgrammedStateRamError) != 0)
      {
         fblProgrammedState &= (vuint8)~(vuint32)kProgrammedStateRamError;
         return kDiagProgStateRamFault;
      }
# endif
   }
#endif
   /* Determine application and calibration programmed state */
   return ApplFblGetProgrammedState();
}

/***********************************************************************************************************************
 *  ApplFblRequestPrgMode
 **********************************************************************************************************************/
/*! \brief       This function is called when a Programming-Mode (0xA5) service request has been received requesting
 *               a programming mode.
 *  \details     Note that this is _not_ called when the FBL is started by the Operating Software.
 *               See also ApplFblEnablePrgMode. This routine should verify that conditions are correct to
 *               proceed with programming the device.
 *  \param[in]   requestMode
 *               kDiagSubRequestProgMode
 *               kDiagSubRequestProgModeHSpeed
 *               (only applicable if FBL_ENABLE_HIGHSPEED_MODE is set in FBL_CFG.H)
 *  \return      kFblFailed: Programming mode not allowed,
 *               kFblOk: Programming mode allowed
 **********************************************************************************************************************/
tFblResult ApplFblRequestPrgMode( vuint8 requestMode )
{
#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   requestMode = requestMode;
#endif
   return ApplFblCheckProgConditions();
}

/***********************************************************************************************************************
 *  ApplFblEnablePrgMode
 **********************************************************************************************************************/
/*! \brief       This function is called upon receipt of the Programming-Mode (0xA5) service request
 *               (with enable-programming-mode subfunction).
 *  \details     Note that this is _not_ called when the FBL is started by the Operating Software.
 *               The application is informed about reception of service 'EnableProgramming mode'.
 **********************************************************************************************************************/
void ApplFblEnablePrgMode( void )
{
   /* Normally, nothing to do */
}
/***********************************************************************************************************************
 *  ApplFblRdbidProgrammedStateInd
 **********************************************************************************************************************/
/*! \brief       This function is called when a ReadDataByIdentifier (0x1A) service is requested with data identifier
 *               ProgrammedStateIndicator (0xF0)
 *  \details     This functions retrieves the programmed state of the software partitions.
 *  \param[out]  pbDiagData   buffer to store partition data
 *  \return      length of data stored in buffer
 **********************************************************************************************************************/
static tTpDataType ApplFblRdbidProgrammedStateInd(vuint8 * pbDiagData)
{
   vuint8 index = 0;
   vuint8 opSwPartId = PARTID_OPSW_1;

#if defined( FBL_ENABLE_MULTI_PROCESSOR )
   vuint8 i;
   for(i = 0; i < FBL_MTAB_NO_OF_BLOCKS; i++)
   {
#endif
      pbDiagData[index++] = opSwPartId;
      pbDiagData[index++] = ApplFblChkPSIState(opSwPartId);

#if defined( FBL_ENABLE_CALIBRATION_MODULES )
      if (pbDiagData[index-1] == PSI_PART_PRESENT)
      {
         vuint8 numberOfPartitions;
         vuint8 currCalPartNb = 1;

         numberOfPartitions = FblHdrGetNoOfCalPartitions(opSwPartId);

         while (currCalPartNb <= numberOfPartitions)
         {
            (void)FblRealTimeSupport();

            pbDiagData[index++] = opSwPartId + currCalPartNb;
            pbDiagData[index++] = ApplFblChkPSIState(pbDiagData[index-1]);

            currCalPartNb++;
         }
      }
#endif /* FBL_ENABLE_CALIBRATION_MODULES */

#if defined( FBL_ENABLE_MULTI_PROCESSOR )
      /* Go to next operating software partition ID (IDs: 0x11/0x21/0x31/0x41) */
      opSwPartId += (PARTID_OPSW_2-PARTID_OPSW_1);
   }
#endif

   return (tTpDataType)index;
}

/***********************************************************************************************************************
 *  END OF FILE: KBFBL_APDIFRAMEOEM.C
 **********************************************************************************************************************/

