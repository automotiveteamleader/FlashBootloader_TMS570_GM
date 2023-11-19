/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file          fbl_hdr.c
 *  \brief         Secure Fbl header parsing module
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
 *  Dennis O'Donnell              Dod           Vector CANtech, Inc.
 *  Ralf Haegenlaeuer             HRf           Vector Informatik GmbH
 *  Vijay Natarajan               Vna           Vector CANtech, Inc.
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id       Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  01.00.00   2014-01-31  AWh     -               First release
 *  01.01.00   2014-03-04  AWh     ESCAN00073546   Add Non readable device configuration
 *                                 ESCAN00073567   Change to LibMem 2.xx interface; Minor Improvements
 *                                 ESCAN00073883   Cal module may be considered invalid, when it actually is valid
 *                                 ESCAN00073910   Enable Multi processor use case
 *                         Dod     ESCAN00074067   Compile error when cal disabled and verify integrity word enabled
 *  01.02.00   2014-07-14  Dod     ESCAN00075058   Update latest version to be compatible with GM SLP6
 *                         AWh     ESCAN00075272   GapFill need to use the at least FLASH_SEGMENT_SIZE buffer size
 *                                 ESCAN00075353   Track Cal Part. Erase Segments for Gap Fill on Cal Part. update
 *                                 ESCAN00075678   SLP6: Add support for Pec Error Err_Undefined
 *                                 ESCAN00075685   SLP6: Do not clear Pec Error on retries
 *                                 ESCAN00076569   SLP6: Download with more than one calibration in partition fails
 *                                 ESCAN00076911   Sba ticket is stored and read inclusive the data type
 *                                 ESCAN00077055   Late initialization of pecErrorCode
 *  01.03.00   2014-09-11  HRf     ESCAN00078350   Add cast to second parameter when FblMemGetInteger is called in
 *                                                 the context of FblHdrInitFblRomCheck
 *  01.04.00   2014-11-05  AWh     ESCAN00078423   No changes
 *                         Vna     ESCAN00078603   No changes
 *                         AWh     ESCAN00078852   Compile error: FblMemGetInteger 2nd par need to be pointer type
 *                                 ESCAN00078855   No changes
 *                                 ESCAN00079183   SLP6: incorrect handling of calibration modules in wrong order
 *                                 ESCAN00079328   SLP5: Order of calibration partition members is not checked
 *  02.00.00   2015-01-15  AWh     ESCAN00079553   Add Compression Interface
 *  02.00.01   2015-01-16  AWh     ESCAN00080628   Allow overwriting Data compression type
 *  02.01.00   2015-02-09  AWh     ESCAN00081143   Programming failure on transfer data
 **********************************************************************************************************************/

#include "fbl_inc.h"
#include "Secmpar.h"

#if ( FBLDIAGHDR_GM_VERSION != 0x0201u ) || \
    ( FBLDIAGHDR_GM_RELEASE_VERSION != 0x00u )
# error "Error in fbl_hdr.c: Source and Header file are inconsistent!"
#endif

#if ( FBLDIAGHDR_GM_VERSION != _FBLDIAGHDR_OEM_VERSION ) || \
    ( FBLDIAGHDR_GM_RELEASE_VERSION != _FBLDIAGHDR_OEM_RELEASE_VERSION )
# error "Error in fbl_hdr.c: Source and v_ver.h are inconsistent!"
#endif

#if (FBLLIB_MEM_VERSION < 0x0200)
# error "Implementation requires LibMem in version 2.00.00 or higher."
#endif

# if( FBLKBDIAG_14230_GM_VERSION < 0x600u )
#  error "Error in fbl_hdr.c: Interface to fbl_diag.c does not match."
# endif

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/
/* Sbi-flag */
#define SBI_FLAG_A_VALID ((vuint32)0xA5163829u)
#define SBI_FLAG_B_VALID (FblInvert32Bit(SBI_FLAG_A_VALID))

#define FblHdrClrSBITicketValid()  {\
                                      sbiFlag.flagA = 0;\
                                      sbiFlag.flagB = 0;\
                                   } /* PRQA S 3458 */ /* MD_CBD_19.4 */
#define FblHdrSetSBITicketValid()  {\
                                      sbiFlag.flagA = SBI_FLAG_A_VALID;\
                                      sbiFlag.flagB = SBI_FLAG_B_VALID;\
                                   } /* PRQA S 3458 */ /* MD_CBD_19.4 */
#define FblHdrGetSBITicketValid()  ( (SBI_FLAG_A_VALID == sbiFlag.flagA) && (SBI_FLAG_B_VALID == sbiFlag.flagB) )

/* Compatibility define */
# define FblDiagSetError DiagSetError

#if defined ( FBL_ENABLE_COMPRESSION_MODE )
# if !defined ( FBL_MAX_COMPR_PRODUCED_CHUNK )
#  error "Compression module needs provide FBL_MAX_COMPR_PRODUCED_CHUNK definition."
   /* FBL_MAX_COMPR_PRODUCED_CHUNK shall be overwritten by compression module, depending on algo.
    * The worst case is that only 1 byte is required from the following compressed package/chunk to complete the
    * signed header info, but a lot more bytes need to be decompressed for that.
    * If you want to keep it small: Implement the decompression the way that you decompress chunks partly.  */
   /* The default is ARLE, the max produced chunk is a 4 byte repeated pattern, repeating 63 times */
# endif
# define RAM_HEADER_LENGTH (HDR_MODULE_MAX_RAW_LEN + FBL_MAX_COMPR_PRODUCED_CHUNK) /* This is the maximum buffer needed to decompress the signed header */
# if ( FBL_MEM_PROC_BUFFER_SIZE < RAM_HEADER_LENGTH )
#  error "GENy parameter to small: Data processing buffer size. Please enlarge this parameter."
# endif
#endif /* defined ( FBL_ENABLE_COMPRESSION_MODE ) */

/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/
V_MEMRAM0 V_MEMRAM1 tDownloadModuleInfo V_MEMRAM2 parsedModuleInfo; /**< Externally accessible Header info of currently parsed module */
V_MEMRAM0 V_MEMRAM1 tBootInitStatus V_MEMRAM2 fblHdrBis; /**< Externally accessible Boot Initialization Status */
#if defined( FBL_ENABLE_CALIBRATION_MODULES )
V_MEMRAM0 V_MEMRAM1 vuint8 V_MEMRAM2 calPartActive; /**< Cal partition is currently updated */
#endif
V_MEMRAM0 V_MEMRAM1 tHdrRcvBuffHdl V_MEMRAM2 rcvBuffHandle; /**< store current buffer processing state */
V_MEMRAM0 V_MEMRAM1_NEAR FL_SegmentInfoType V_MEMRAM2_NEAR currDataSegInfo; /**< Actual region for flash programming */
V_MEMRAM0 V_MEMRAM1_NEAR vuint16 V_MEMRAM2_NEAR errPecErrorCode; /**< Current Program error code */

/***********************************************************************************************************************
 *  LOCAL DATA
 **********************************************************************************************************************/
V_MEMRAM0 static V_MEMRAM1 tSBIFlag V_MEMRAM2 sbiFlag; /**< Signature Bypass active? */
V_MEMRAM0 static V_MEMRAM1 tNBIDInfo V_MEMRAM2 appNBIDInfo; /**< Application Not Before ID Info Data */
V_MEMRAM0 static V_MEMRAM1 tNBIDInfo V_MEMRAM2 keyNBIDInfo; /**< Key Not Before ID Info Data */

#if defined( FBL_ENABLE_CALIBRATION_MODULES )
V_MEMRAM0 static V_MEMRAM1 tCalPartitionInfo V_MEMRAM2 calPartitionInfo; /**< Externally accessible partition info of currently parsed module */
V_MEMRAM0 static V_MEMRAM1 tCalInfo V_MEMRAM2 calInfo; /**< Cal module Info */
#endif
#if defined ( FBL_ENABLE_COMPRESSION_MODE )
V_MEMRAM0 static V_MEMRAM1 tFblLength V_MEMRAM2 compressedLength;  /* Length of compressed data content */
#endif
V_MEMRAM0 static V_MEMRAM1 vuint8 V_MEMRAM2 msgDigest[HDR_MSGDIGEST_LEN]; /**< Message Digest received in signed header and stored during module download */
V_MEMRAM0 static V_MEMRAM1 tPartitionBlockList V_MEMRAM2 partBlockList; /**< List of Flashblocks used by partitions */

/* Variables used to parse reception buffer */
V_MEMRAM0 static V_MEMRAM1 vuint8  V_MEMRAM2 currDataSegIdx; /**< Index to data segment currently programmed */

V_MEMRAM0 static V_MEMRAM1 tFblMemSegmentInfo V_MEMRAM2 currMemLibSegment; /**< Segment currently processed in MemLib */
V_MEMRAM0 static V_MEMRAM1 tFblMemBlockInfo V_MEMRAM2 blockInfo; /**< block currently processed in MemLib */
#if defined( FBL_ENABLE_SYSTEM_CHECK )
V_MEMRAM0 static V_MEMRAM1 tFblLength V_MEMRAM2 currDataSegInitialLen;    /* Initial len of current data segment */
#endif

/***********************************************************************************************************************
 *  LOCAL PROTOTYPES
 **********************************************************************************************************************/
static tFblResult FblHdrCheckMtabIncludesSegment( vuint8 partId, const V_MEMRAM1 FL_SegmentInfoType V_MEMRAM2 V_MEMRAM3 * segment );
static tFblResult FblHdrValidateSignature( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * const valRegStart, SecM_LengthType valRegLen,
                                           const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * const signatureStart,
                                           const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * const publicKey );
static tFblResult FblHdrChkIdPresent(vuint8 mid);
static vuint32 FblHdrGetIntegerRom( tFblResult* const result, vuint8 count, tFblAddress addr );
static vuintx FblHdrPartSegmentListAddEntry( vuint8 partId, const FL_SegmentInfoType * segment );
#if defined( FBL_ENABLE_GAP_FILL )
static void FblHdrGetSortedApplRegions( FL_SegmentInfoType* retSeg, vuint8 regionNbr );
static tFblResult FblHdrFillRegion( tFblAddress inBeginAddr, tFblLength inLength );
static tFblBlockPartUsage FblHdrPartSegmentListGetBlockStatus( vsint16 blockNbr, vuint8 partId );
#endif

static tTpDataType FblHdrTransferDataHeaderReceived( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * const signedHeaderAddr);
#if defined ( FBL_ENABLE_CALIBRATION_MODULES )
static tFblResult FblHdrEraseCalPartition( vuint8 calPartId );
#endif
static tFblResult FblHdrTransferDataEnd( void );
static tFblResult FblHdrEraseRoutine( vuint8 partId );

static tTpDataType FblHdrProcessHeaderInfo( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * const signedHeaderAddr );
static void FblHdrSetMemLibMemBlockInfo( void );
static void FblHdrSetMemLibSegmentAddrLengthInfo( void );

/* Header Parsing */
static tFblResult FblHdrMemCompare( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * a, const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * b, vuint16 len );
static tFblResult FblHdrCheckAndParseDownloadMid( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3* const moduleAddr );
static tFblResult FblHdrCheckAndInitAppNBID( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * const locInfoAddr );
static tFblResult FblHdrCheckAndParseSwLocInfo( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * const locInfoAddr );
static vuint8 const * FblHdrValidateSignerInfo( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * const signInfoAddr, vuint16 keyNbToCheck );
static tFblResult FblHdrCheckDownloadDcid( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * const dcidAddr );
static tFblResult FblHdrCheckEcuNameAndId( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * const ecuNameAddr, tEcuIdZeroCheck zeroCheck );
static tFblResult FblHdrInitBaseModInfo( vuint8 partId, V_MEMRAM1 tBaseModInfo V_MEMRAM2 V_MEMRAM3 * const pBaseModInfo );
static tFblResult FblHdrValidatePlainHeader( void );
static tFblResult FblHdrCheckAndInitDownloadModuleInfo( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * const signedHdrAddr );
#if defined( FBL_ENABLE_CALIBRATION_MODULES )
static tFblResult FblHdrInitCalPartInfo( vuint8 calPartId, V_MEMRAM1 tCalPartitionInfo V_MEMRAM2 V_MEMRAM3 * pCalPartInfo );
static tFblResult FblHdrInitNextCalPartInfo( V_MEMRAM1 tCalPartitionInfo V_MEMRAM2 V_MEMRAM3 * const pCalPartInfo );
static tFblResult FblHdrValidateCalPartInfo( void );
static tFblResult FblHdrValidateCalPartitionInfo_CalRegions( vuintx* const incCals, const FL_SegmentInfoType* const erReg, const tCalPartitionInfo* const inPartInfo );
static tFblResult FblHdrSetCalModuleInfo( void );
static tFblResult FblHdrPrepareDownloadCalPartionData( vuint8 calModId );
#endif
#if defined( FBL_ENABLE_CALIBRATION_MODULES ) || defined( FBL_ENABLE_VERIFY_INTEGRITY_WORD )
static tFblResult FblHdrGetRegionRomAndInc( FL_SegmentInfoType* segInfo, tFblAddress baseAddr, tFblAddress* relAddr );
#endif

/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/* PRQA S 2006 EOF */ /* MD_CBD_14.7 */

/***********************************************************************************************************************
 *  FblHdrInitPowerOn
 **********************************************************************************************************************/
/*! \brief       Init Power on for Header module
 **********************************************************************************************************************/
void FblHdrInitPowerOn(void)
{
   static FL_SegmentInfoType downloadSegments[SWM_DATA_MAX_NOAR];

   /* Initialize parsedModuleInfo segmentInfo pointer */
   parsedModuleInfo.segmentList.segmentInfo = downloadSegments;

   /* Init as this value is compared previous to being written */
   keyNBIDInfo.received = 0;

#if defined( FBL_ENABLE_CALIBRATION_MODULES )
   calPartitionInfo.partID = 0;
#endif

#if defined( FBL_USER_OPSW1_ID )
   assertFblUser( (  (FBL_USER_OPSW1_ID == MODID_OPSW_2)
                   ||(FBL_USER_OPSW1_ID == MODID_OPSW_3)
                   ||(FBL_USER_OPSW1_ID == MODID_OPSW_4)), kFblAssertVariableInValidState); /* PRQA S 2214 */ /* MD_FblHdr_2214 */
#endif
}

/***********************************************************************************************************************
 *  FblHdrInitRetry
 **********************************************************************************************************************/
/*! \brief     Init when receiving Erase Mem Routine
 *             (possible scenario apart from standard flow: after error; retry programming sequence)
 **********************************************************************************************************************/
void FblHdrInitRetry(void)
{
   vuintx i;
   /* Init partBlockList */
   for(i = 0; i<HDR_MAX_PARTITION_TRACKED_REGIONS ;i++)
   {
      (void)FblRealTimeSupport();

      partBlockList.segList[i].startBlk = kFblDiagMemSegmNotFound;
   }

   FblClrPecErr(); /* PRQA S 3109 */ /* MD_CBD_14.3 */
}

/***********************************************************************************************************************
 *  FblHdrInit
 **********************************************************************************************************************/
/*! \brief       Init Header module, check SBA ticket
 *  \pre         DiagBuffer needs to be initialized
 **********************************************************************************************************************/
void FblHdrInit(void)
{
   const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * signerPublicKeyAddr;

   FblHdrClrSBITicketValid(); /* PRQA S 3109 */ /* MD_CBD_14.3 */
   FblClrBisBytes(); /* PRQA S 3109 */ /* MD_CBD_14.3 */
   FblClrPecErr(); /* PRQA S 3109 */ /* MD_CBD_14.3 */

# if defined( FBL_ENABLE_CALIBRATION_MODULES )
   /* Initialize Cal Partition state (start with not active state) */
   calPartActive = NO_CALPART_ACTIVE;
# endif
   /********************************************************************/
   /* Read and test SBA ticket                                         */
   /********************************************************************/

   /* Use Diagbuffer during init to read SBA ticket. 820 bytes are to be read */
   if (kFblOk != ApplFblNVMReadSBATicket(DiagBuffer))
   {
      /* Do nothing */
   }
   else if (MODID_SBA != FilterMid(FblMemGetInteger(HDR_MID_LEN, &DiagBuffer[S1_SBA_MID_IDX])))
   {
     /* Do nothing */
   }
   else if (kFblOk != FblHdrCheckEcuNameAndId(&DiagBuffer[S1_SBA_ECUNAME_IDX], ECUID_SKIP_ZEROCHECK))
   {
      /* Read error from  FblHdrCheckEcuNameAndId */
      assertFblInternal( (Err_EcuId == FblGetPecErr()) || (Err_EcuName == FblGetPecErr()), kFblAssertInternalIlegalState); /* PRQA S 2214 */ /* MD_FblHdr_2214 */
      FblSetBisMsbByte( (vuint8)((Err_EcuId == FblGetPecErr())? SBIMSB_ERR_ECUID : SBIMSB_ERR_ECUNAME) ); /* PRQA S 3109 */ /* MD_CBD_14.3 */
   }
   else
   {
      vuint16 locKeyNbidToCheck = (vuint16) FblMemGetInteger(HDR_NBID_LEN, &DiagBuffer[S1_SBA_SIGNINFO_IDX+SIGNINFO_KEYNBID_IDX]);
      signerPublicKeyAddr = FblHdrValidateSignerInfo(&DiagBuffer[S1_SBA_SIGNINFO_IDX], locKeyNbidToCheck);
      if (V_NULL == signerPublicKeyAddr)
      {
         /* Overwrite error info ( for SBA error values are not correctly set ) */
         FblSetBisMsbByte( SBIMSB_ERR_CERT ); /* PRQA S 3109 */ /* MD_CBD_14.3 */
      }
      else
      {
         if (kFblOk != FblHdrValidateSignature(&DiagBuffer[S1_SBA_MID_IDX], S1_SBA_SIGNATURE_IDX-S1_SBA_MID_IDX, &DiagBuffer[S1_SBA_SIGNATURE_IDX], signerPublicKeyAddr))
         {
            FblSetBisMsbByte(SBIMSB_ERR_SIGNATURE); /* PRQA S 3109 */ /* MD_CBD_14.3 */
         }
         else
         {
            FblHdrSetSBITicketValid(); /* PRQA S 3109 */ /* MD_CBD_14.3 */
            FblSetBisMsbByte(SBIMSB_SBI_VALID); /* PRQA S 3109 */ /* MD_CBD_14.3 */
         }
      }
   }
} /* PRQA S 6080 */ /* MD_FblHdr_60xx */

#if (FBL_MTAB_NO_OF_BLOCKS > 1)
/***********************************************************************************************************************
 *  GetBlockNbrFromPartId
 **********************************************************************************************************************/
/*! \brief       Returns BlockNumber from partId
 *  \param[in]   partId some partition ID for which logical block table mapping shall be queried
 *  \return      tBlockDescriptor* dummy block descriptor prepared.
 **********************************************************************************************************************/
vuint8 GetBlockNbrFromPartId( const vuint8 partId )
{
   vuint8 retVal = UNDEFINED_LOG_BLOCK;
   vuint8 i;

   (void)FblRealTimeSupport();

   for(i = 0; i < FBL_MTAB_NO_OF_BLOCKS; i++)
   {
      if (FblLogicalBlockTable.logicalBlock[i].blockIndex == FblHdrGetBasePartId(partId))
      {
         /* Module ID for Application1 has to be first entry in Logical Block Table  */
         assertFblUser( ((0x00 == FblLogicalBlockTable.logicalBlock[i].blockNr) || /* PRQA S 2214 */ /* MD_FblHdr_2214 */
               (PARTID_OPSW_1 != FblLogicalBlockTable.logicalBlock[i].blockIndex)), kFblAssertAppl1NotFirstBlock);
         assertFblUser( ((0x00 != FblLogicalBlockTable.logicalBlock[i].blockNr) || /* PRQA S 2214 */ /* MD_FblHdr_2214 */
               (PARTID_OPSW_1 == FblLogicalBlockTable.logicalBlock[i].blockIndex)) , kFblAssertAppl1NotFirstBlock);
         retVal = FblLogicalBlockTable.logicalBlock[i].blockNr;
         break;
      }
   }

   return retVal;
}
#endif

/***********************************************************************************************************************
 *  FblHdrRealTimeSupportSecMod
 **********************************************************************************************************************/
/*! \brief       Wrap FblRealTimeSupport with function not returning a value
 **********************************************************************************************************************/
void FblHdrRealTimeSupportSecMod( void )
{
   (void) FblRealTimeSupport();
}

/***********************************************************************************************************************
 *  FblGetBlockDescriptor
 **********************************************************************************************************************/
/*! \brief       Prepares a Dummy block descriptor that holds the module id required to satisfy the presence pattern API
 *  \param[in]   id id that sets current blockNr
 *  \return      tBlockDescriptor* dummy block descriptor prepared.
 **********************************************************************************************************************/
V_MEMRAM1 tBlockDescriptor V_MEMRAM2 V_MEMRAM3 * FblGetBlockDescriptor( vuint8 partId )
{
   static  V_MEMRAM1 tBlockDescriptor V_MEMRAM2 dummyBlockDescriptor;
   dummyBlockDescriptor.blockNr = partId;
   return &dummyBlockDescriptor;
}

#if defined( FBL_ENABLE_CALIBRATION_MODULES )
/***********************************************************************************************************************
 *  FblHdrValidateCalPartInfo_CalRegions
 **********************************************************************************************************************/
/*! \brief       Validate application partition info section Cal Regions
 *  \detail      This is validated:
 *                - Check all calibration segments inside erase regions
 *  \pre         These preconditions need to apply (else kFblFailed is returned)
 *                - Any Cal Region is included in only one Partition Region
 *  \param[in]   incCals Number of Cals included in Erase Segments (receive current counter and update with current Region)
 *  \param[in]   erReg Erase Region to be checked
 *  \param[in]   inPartInfo partition information pointer
 *  \return      kFblOk / kFblFailed
 **********************************************************************************************************************/
static tFblResult FblHdrValidateCalPartitionInfo_CalRegions( vuintx* const incCals, const FL_SegmentInfoType* const erReg,
                                                             const tCalPartitionInfo* const inPartInfo )
{
   tFblResult retVal = kFblOk;
   FL_SegmentInfoType calProgSegment;
   vuintx locNbrCals = inPartInfo->nbrOfCals;
   /* Start with offset to Cal Module Info 1 */
   tFblAddress calModInfoOffset = HDR_NBROF_LEN + (inPartInfo->nbrOfRgns * HDR_REGINFO_LEN) + HDR_NBROF_LEN;

   /* Check which calibration regions are included in this block */
   while ( (kFblOk == retVal) && (locNbrCals > 0) )
   {
      {
         (void)FblRealTimeSupport();

         calModInfoOffset+= HDR_MID_LEN + HDR_DCID_LEN;
         retVal = FblHdrGetRegionRomAndInc(&calProgSegment, inPartInfo->partInfoAddr, &calModInfoOffset);

         /* Determine if Cal Segment is included in Erase Segment */
         if (FBL_UTIL_CHECK_RANGE_CONTAINED_LEN(calProgSegment.targetAddress, calProgSegment.length, erReg->targetAddress, erReg->length))
         {
            (*incCals)++;
         }

         locNbrCals--;
      }
   }
   return retVal;
}

/***********************************************************************************************************************
 *  FblHdrValidateCalPartInfo
 **********************************************************************************************************************/
/*! \brief       Validate application partition info section
 *  \detail      This is validated:
 *                - Check all partition (erase) regions valid
 *                  + check inside mtab
 *                  + check overlapping appl blocks
 *                  + check in order and not overlapping with previous region
 *                - Check all calibration segments inside erase regions
 *                - Check PSI address not overlapping with segments
 *  \pre         These preconditions need to apply (else kFblFailed is returned)
 *                - Cal Space outside of Application Space (no calibration segments interleaving application segments)
 *                - Cal Region information sections are in order
 *                - Any Cal Region is included in only one Partition Region
 *  \return      kFblOk / kFblFailed
 **********************************************************************************************************************/
static tFblResult FblHdrValidateCalPartInfo( void )
{
   tFblResult retVal;
   /* It is not allowed to have calibration regions between Application regions */
   tCalPartitionInfo locPartInfo;
   vuint8 partCounter;
   vuint8 opSwPartId = FblHdrGetBasePartId(parsedModuleInfo.partID);

   locPartInfo.partID = 0;
   /* Initialize to min #partitions to allow enter loop. Gets overwritten after call to FblHdrInitCalPartInfo */
   retVal = FblHdrInitCalPartInfo(FblHdrGetFirstCalPartNbr(opSwPartId), &locPartInfo);

   for (partCounter = 0; (partCounter < locPartInfo.nbrOfPrts) && (kFblOk == retVal); partCounter++)
   {
      /* Start with offset to Region Info 1 */
      tFblAddress regInfoOffset = HDR_NBROF_LEN;
      vuintx nBrOfRegions;
      vuintx calsInPartition = 0;
      vsint16 previosRegLastCalBlock = kFblDiagMemSegmNotFound;

      retVal = FblHdrInitCalPartInfo(FblHdrGetFirstCalPartNbr(opSwPartId) + partCounter, &locPartInfo);
      nBrOfRegions = locPartInfo.nbrOfRgns;

      while ( (kFblOk == retVal) && (nBrOfRegions > 0) )
      {
         FL_SegmentInfoType partErReg; /* Partition erase region */

         (void)FblRealTimeSupport();

         retVal = FblHdrGetRegionRomAndInc(&partErReg, locPartInfo.partInfoAddr, &regInfoOffset);
         if (kFblOk == retVal)
         {
            /* Check blocks valid, in order and not overlapping with other partitions */
            vuintx regionIdx = FblHdrPartSegmentListAddEntry(locPartInfo.partID, &partErReg);

            if ( (PARTITION_REGION_ERROR == regionIdx)
                 || (partBlockList.segList[regionIdx].startBlk <= previosRegLastCalBlock)
               )
            {
               /* Pec error set externally */
               retVal = kFblFailed;
            }
            else
            {
               if (1 == nBrOfRegions)
               {
                  /* Reduce length by Presence Pattern, to automatically check no overlapping */
                  partErReg.length -= (2 * FBL_PP_SEGMENT_SIZE);
               }

               retVal = FblHdrValidateCalPartitionInfo_CalRegions(&calsInPartition, &partErReg, &locPartInfo);
               previosRegLastCalBlock = partBlockList.segList[regionIdx].endBlk;
               nBrOfRegions--;
            }
         }
      } /* while (nBrOfRegions > 0) */

      /* Check all calibration segments inside erase regions. */
      if (calsInPartition != locPartInfo.nbrOfCals)
      {
         /* Pec error set externally */
         retVal = kFblFailed;
      }
   } /* for (partCounter = 0 ..*/

   return retVal;
} /* PRQA S 6080 */ /* MD_FblHdr_60xx */
#endif

/***********************************************************************************************************************
 *  FblHdrValidatePlainHeader
 **********************************************************************************************************************/
/*! \brief       Check plain/programmed header to contain valid data
 *  \detail      Checks Cal partition info and data segments
 *  \param[in]   verifyParam verify parameter structure
 *  \return      kFblOk / kFblFailed
 **********************************************************************************************************************/
static tFblResult FblHdrValidatePlainHeader( void )
{
   tFblResult retVal;

#if defined( FBL_ENABLE_CALIBRATION_MODULES )
   if (MOD_TYPE_CAL == parsedModuleInfo.modType)
   {
      retVal = kFblOk;
   }
   else
#endif
   {
      vuint16 plainAppNBID;
      tBaseModInfo locBaseModInfo;

      retVal = FblHdrInitBaseModInfo(parsedModuleInfo.partID, &locBaseModInfo);

      if (kFblOk == retVal)
      {
         plainAppNBID = (vuint16) FblHdrGetIntegerRom(&retVal, HDR_NBID_LEN, locBaseModInfo.startAddr + P1_APP_NBID_IDX);

         if ((kFblOk != retVal) || (appNBIDInfo.received != plainAppNBID))
         {
            FblErrDebugStatus(kDiagErrP1AppNBIDInconsistent); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
            retVal = kFblFailed;
         }
         else
         {
#if defined( FBL_ENABLE_VERIFY_INTEGRITY_WORD )
            FL_SegmentInfoType progRegion;

            /* Are the Regions programmed equal to the regions used? Read regions and compare segmentlist */
            if (parsedModuleInfo.segmentList.nrOfSegments != locBaseModInfo.nbrOfRgns)
            {
               FblErrDebugStatus(kDiagErrP1SegNbrInconsistent); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
               retVal = kFblFailed;
            }
            else
            {
               vuintx i = 0;
               locBaseModInfo.swLocInfoAddr+= HDR_NBROF_LEN;

               while ( (locBaseModInfo.nbrOfRgns > 0) && (kFblOk == retVal) )
               {
                  (void)FblRealTimeSupport();

                  retVal = FblHdrGetRegionRomAndInc(&progRegion,(tFblAddress)0, &locBaseModInfo.swLocInfoAddr);
                  if(kFblOk == retVal)
                  {
                     if ((  parsedModuleInfo.segmentList.segmentInfo[i].targetAddress != progRegion.targetAddress)
                           || (parsedModuleInfo.segmentList.segmentInfo[i].length != progRegion.length))
                     {
                        FblErrDebugStatus(kDiagErrP1SegRangeInconsistent); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
                        retVal = kFblFailed;
                     }
                  }
                  i++;
                  locBaseModInfo.nbrOfRgns--;
               }
            } /* if (parsedModuleInfo.segmentList.nrOfSegments != locBaseModInfo.nbrOfRgns) */

            if (kFblOk == retVal)
#endif
            {
               if ( 0 != FblHdrGetIntegerRom(&retVal, HDR_NBROF_LEN, locBaseModInfo.nbPartAddr))
               {
                  assertFblInternal(kFblOk == retVal, kFblAssertInternalIlegalState); /* PRQA S 2214 */ /* MD_FblHdr_2214 */
#if defined( FBL_ENABLE_CALIBRATION_MODULES )
                  retVal = FblHdrValidateCalPartInfo();
                  if (kFblOk != retVal)
                  {
                     FblSetPecErr(Err_CalRegion); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
                  }
#else
                  /* No Partition Information expected If calibration Modules are not supported. */
                  retVal = kFblFailed;
#endif
               } /* in else case result may be kFblFailed. This will set Err_GetAppInfo */
            }
         }
      }

      if ((kFblOk != retVal) && (Err_NoError == FblGetPecErr()))
      {
         /* Set Default Error */
         FblSetPecErr(Err_GetAppInfo); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
      }
   } /* (MOD_TYPE_CAL != parsedModuleInfo.modType) */

   return retVal;
} /* PRQA S 6080 */ /* MD_FblHdr_60xx */

/***********************************************************************************************************************
 *  FblHdrUpdatePSI
 **********************************************************************************************************************/
/*! \brief       Update PSI information in memory. Also write App/Key-NBID data
 *  \return      kFblOk / kFblFailed
 **********************************************************************************************************************/
tFblResult FblHdrUpdatePSI( void )
{
   tFblResult retVal = kFblOk;

#if defined( FBL_ENABLE_CALIBRATION_MODULES )
   if (MOD_TYPE_CAL != parsedModuleInfo.modType)
#endif
   {
      /* Update NBID information. */
      if (appNBIDInfo.received > appNBIDInfo.stored)
      {
         if (kFblOk != ApplFblNVMWriteAppNBID(&appNBIDInfo))
         {
            FblSetPecErr(Err_FlashWrite); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
            retVal = kFblFailed;
         }
      }
   }
   if (keyNBIDInfo.received > keyNBIDInfo.stored)
   {
      if (kFblOk != ApplFblNVMWriteKeyNBID(&keyNBIDInfo))
      {
          FblErrDebugStatus(kDiagErrNbidFailed); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
          FblSetPecErr(Err_FlashWrite); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
          retVal = kFblFailed;
      }
   }
# if defined( FBL_ENABLE_CALIBRATION_MODULES )
   if ((MOD_TYPE_CAL != parsedModuleInfo.modType) || IsCalLastModInPartition())
# endif
   {
# if defined( FBL_ENABLE_CALIBRATION_MODULES )
      calPartActive = NO_CALPART_ACTIVE;
# endif

      if (kFblOk != ApplFblValidateBlock(*FblGetBlockDescriptor(parsedModuleInfo.partID)))
      {
         FblSetPecErr(Err_UpdatePSI); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
         retVal = kFblFailed;
      }
   } /* if ((MOD_TYPE_CAL != parsedModuleInfo.modType) || IsCalLastModInPartition())  */
   return retVal;
}

/***********************************************************************************************************************
 *  FblHdrPipelinedVerifyIntegrity
 **********************************************************************************************************************/
/*! \brief       Perform Application / Calibration verify actions (depending on configuration)
 *               on Rom data after single transfer reception (FBL_MEM_ENABLE_VERIFY_INPUT/non readable device:
 *               on Ram data if ApplFblNrdGetDeviceReadSupport returns failed, check FblHdrPostFlashValidation )
 *  \param[in]   verifyParam verify parameter structure
 *  \return      SECM_VER_OK / SECM_VER_ERROR
 **********************************************************************************************************************/
SecM_StatusType FblHdrPipelinedVerifyIntegrity( V_MEMRAM1 SecM_SignatureParamType V_MEMRAM2 V_MEMRAM3 * verifyParam ) /* PRQA S 3112 */ /* MD_FblHdr_3112 */
{
   SecM_StatusType retVal;

#if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED ) 
   /* Verify Hash / Digest */
   retVal = SecM_VerifyClassDDD(verifyParam);
#else
   retVal = SECM_VER_OK;
#endif
   return retVal;
}

/***********************************************************************************************************************
 *  FblHdrVerifyIntegrity
 **********************************************************************************************************************/
/*! \brief       Perform Application / Calibration verify actions (depending on configuration)
 *               on Rom data after whole module transfer. (FBL_MEM_ENABLE_VERIFY_INPUT/non readable device:
 *               call user callback ApplFblNrdVerify for device specific verification )
 *  \param[in]   verifyParam verify parameter structure
 *  \return      SECM_VER_OK / SECM_VER_ERROR
 **********************************************************************************************************************/
SecM_StatusType FblHdrVerifyIntegrity( V_MEMRAM1 SecM_VerifyParamType V_MEMRAM2 V_MEMRAM3* verifyParam ) /* PRQA S 3673 */ /* MD_FblHdr_3673 */
{
   SecM_StatusType retVal;
#if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED )
# if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   (void) verifyParam; /* Not used in this configuration */ /* PRQA S 3112 */ /* MD_FblHdr_3112 */
# endif
#endif

#if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED ) 
   {
      retVal = SECM_VER_OK;
   }

# if defined( FBL_ENABLE_VERIFY_INTEGRITY_WORD )
   /* Verify Integrity word */
   (void) SecM_InitVerification(V_NULL);
   retVal = SecM_VerificationClassVendor(verifyParam);
   (void) SecM_DeinitVerification(V_NULL);
# endif
#else
   /* Verify Hash / Digest (and optionally Integrity word, if FBL_ENABLE_VERIFY_INTEGRITY_WORD is set) */
   (void) SecM_InitVerification(V_NULL);
   retVal = SecM_Verification(verifyParam);
   (void) SecM_DeinitVerification(V_NULL);
#endif /* defined( FBL_MEM_ENABLE_VERIFY_PIPELINED ) || defined( FBL_MEM_ENABLE_VERIFY_INPUT ) */

   return retVal;
}

/***********************************************************************************************************************
 *  FblHdrPostFlashValidation
 **********************************************************************************************************************/
/*! \brief       Perform Application / Calibration Post Flash Validation Actions
 *  \return      kFblOk / kFblFailed
 **********************************************************************************************************************/
tFblResult FblHdrPostFlashValidation( void )
{
   tFblResult retVal;
   tFblMemStatus memLibResult;

   tFblMemBlockVerifyData  verifyData;
   tFblMemVerifyStatus verifyErrorCode = SECM_VER_ERROR;

   if (kFblOk != FblHdrValidatePlainHeader())
   {
      retVal = kFblFailed;
   }
   else
   {
#if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED ) 
      {
         verifyData.verifyDataInput.length  = 0;
         verifyData.verifyDataInput.data    = V_NULL;
         verifyData.verifyDataPipe.length   = HDR_MSGDIGEST_LEN;
         verifyData.verifyDataPipe.data     = msgDigest;
      }
      verifyData.verifyDataOutput.length = HDR_MSGDIGEST_LEN;
      verifyData.verifyDataOutput.data   = msgDigest;
#else
      verifyData.inputSigData    = V_NULL;
      verifyData.inputSigLength  = 0;
      verifyData.outputSigData   = msgDigest;
      verifyData.outputSigLength = HDR_MSGDIGEST_LEN;
#endif

      memLibResult = FblMemBlockVerify(&verifyData, &verifyErrorCode);
      if ((kFblMemStatus_Ok != memLibResult) && (!((FblHdrGetSBITicketValid()) && (SECM_VER_SIG == verifyErrorCode))))
      {
         FblDiagSetError((vuint8) memLibResult);
         if (SECM_VER_SIG == verifyErrorCode)
         {
            FblSetPecErr(Err_MD); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
         }
#if defined( FBL_ENABLE_VERIFY_INTEGRITY_WORD )
         else if (SECM_VER_CRC == verifyErrorCode)
         {
            /* Overwrite Pec */
            FblSetPecErr(Err_GetInfo); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
         }
#endif
         else
         {
            FblErrDebugStatus(kHdrDebugErrBlockVerify);  /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
         }
         retVal = kFblFailed;
      }
      else
      {
         /* LibmemRes == kFblOk || Flag overwhelming Digest ((SbaFlag == true) && (verifyError ==SECM_VER_SIG)) */
         retVal = ApplFblFillGaps();
         if (kFblOk != retVal)
         {
            FblErrDebugStatus(kDiagErrFillFailed); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
            /* this is used as Pec error */
            FblSetPecErr(Err_FlashWrite); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
         }
      }
   } /* if (kFblOk != FblHdrValidatePlainHeader()()) */

   return retVal;
}

/***********************************************************************************************************************
 *  FblHdrValidateSignature
 **********************************************************************************************************************/
/*! \brief       validate Signature inside Application/Calibration/SBA Signed Header
 *  \param[in]   valRegStart Start of region to validate Signature on
 *  \param[in]   valRegLen Length of region to validate Signature on
 *  \param[in]   signatureStart Start of Signature (Validation region ends here)
 *  \param[in]   publicKey The public key to be used to validate signature
 *  \return      kFblOk / kFblFailed
 **********************************************************************************************************************/
static tFblResult FblHdrValidateSignature( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * const valRegStart, SecM_LengthType valRegLen,
                                           const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * const signatureStart,
                                           const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * const publicKey )
{
   SecM_SignatureParamType sigParam;
   tFblResult retVal = kFblFailed;
   SecM_AsymKeyType asymKey;

   asymKey.shared.data = publicKey;
   asymKey.shared.size = sizeof(SecM_RsaMod);
   /* Macro APPL_FBL_READ_SEC_DATA can be used to copy SecM_Production_RsaExp to Ram if required */
   asymKey.individual.data = APPL_FBL_READ_SEC_DATA(SecM_RsaExp);
   asymKey.individual.size = sizeof(SecM_RsaExp);

   sigParam.key = &asymKey;

   sigParam.currentDataLength = V_NULL;
   sigParam.currentHash.length = 0;
   sigParam.currentHash.sigResultBuffer = 0;
   sigParam.wdTriggerFct = (FL_WDTriggerFctType) FblHdrRealTimeSupportSecMod;
   sigParam.sigState = kHashInit;
   sigParam.sigSourceBuffer = V_NULL;
   sigParam.sigByteCount = 0;

   if (SECM_OK == SecM_VerifySignature(&sigParam))
   {
      sigParam.sigState = kHashCompute;
      sigParam.sigSourceBuffer = valRegStart;
      sigParam.sigByteCount = valRegLen;

      if (SECM_OK == SecM_VerifySignature(&sigParam))
      {
         sigParam.sigState = kHashFinalize;
         sigParam.sigSourceBuffer = V_NULL;
         sigParam.sigByteCount = 0;

         if (SECM_OK == SecM_VerifySignature(&sigParam))
         {
            sigParam.sigState = kSigVerify;
            sigParam.sigSourceBuffer = signatureStart;
            sigParam.sigByteCount = HDR_SIGNATURE_LEN;

            if (SECM_OK == SecM_VerifySignature(&sigParam))
            {
               /* pec error is set externally */
               retVal = kFblOk;
            }
         }
      }
   }

   return retVal;
}

/***********************************************************************************************************************
 *  FblHdrMemCompare
 **********************************************************************************************************************/
/*! \brief       Memcmp implementation for comparing content of pointers to vuint8
 *  \param[in]   a pointer to array one for comparison
 *  \param[in]   b pointer to array two for comparison
 *  \param[in]   len length of data for comparison
 *  \return      kFblOk / kFblFailed
 **********************************************************************************************************************/
static tFblResult FblHdrMemCompare( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * a, const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * b, vuint16 len )
{
   tFblResult retVal = kFblOk;

   while ( len > 0 )
   {
      FblRealTimeSupportInLoop(len, 0x20); /* PRQA S 3109 */ /* MD_CBD_14.3 */

      len--;
      if (a[len] != b[len])
      {
         retVal = kFblFailed;
         break;
      }
   }

   return retVal;
}

/***********************************************************************************************************************
 *  FblHdrCheckMtabIncludesSegment
 **********************************************************************************************************************/
/*! \brief       Returns kFblOk if the Modules Logical Block table entry includes the FlashBlock, kFblFailed if not.
 *  \param[in]   partId partition Id of the module requesting for (any partition Id, including calibration)
 *  \param[in]   segment the Segment to check to be included
 *  \return      kFblOk / kFblFailed
 **********************************************************************************************************************/
static tFblResult FblHdrCheckMtabIncludesSegment( vuint8 partId, const V_MEMRAM1 FL_SegmentInfoType V_MEMRAM2 V_MEMRAM3 * segment )
{
   tFblResult retVal = kFblFailed;
   vuint8 blockNbr = GetBlockNbrFromPartId(partId);

#if (FBL_MTAB_NO_OF_BLOCKS > 1)
   if (UNDEFINED_LOG_BLOCK == blockNbr)
   {
      /* kFblFailed, do nothing */
   }
   else
#else
# if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   (void) partId; /* Not used in this configuration */ /* PRQA S 3112 */ /* MD_FblHdr_3112 */
# endif
#endif
   {
      tFblAddress segmentStart = segment->targetAddress;
      tFblLength segmentLen = segment->length;
      tFblAddress blockStart =  FblLogicalBlockTable.logicalBlock[blockNbr].blockStartAddress;
      tFblLength blockLen =  FblLogicalBlockTable.logicalBlock[blockNbr].blockLength;

      if ( FBL_UTIL_CHECK_RANGE_CONTAINED_LEN(segmentStart, segmentLen, blockStart, blockLen) )
      {
         retVal = kFblOk;
      }
   }

   return retVal;
}

/***********************************************************************************************************************
 *  FblHdrGetBaseModuleHdrAddr
 **********************************************************************************************************************/
/*! \brief       Retrieve the header address for all base modules stored in memory.
 *  \pre         NO_HEADER_ADDR_DEFINED == 0xFFFFFFFF is not allowed for header address
 *  \param[in]   partId Module Identifier of the base module address to retrieve.
 *  \return      Base module address
 **********************************************************************************************************************/
tFblAddress FblHdrGetBaseModuleHdrAddr(const vuint8 partId)
{
   tFblAddress retVal = NO_HEADER_ADDR_DEFINED;

   if ( PartIdOpSw(partId) )
   {
      vuint8 blockNbr = GetBlockNbrFromPartId(partId);
#if (FBL_MTAB_NO_OF_BLOCKS > 1)
      if (UNDEFINED_LOG_BLOCK != blockNbr )
#endif
      {
         retVal = ApplFblGetModuleHeaderAddress(blockNbr);
      }
   }
   else
   {
      /* Nothing to do */
   }
   return retVal;
}

#if defined( FBL_ENABLE_MULTI_PROCESSOR )
/***********************************************************************************************************************
 *  FblHdrGetBasePartIdFromModuleId
 **********************************************************************************************************************/
/*! \brief       Retrieves the base partition Id for all types of modules.
 *  \details     This is the application partition ID for all calibration files
    \pre         mid is valid
 *  \param[in]   mid Module ID ( including calibration IDs ) for which base Partition ID shall be returned
 *  \return      Base module partition ID
 **********************************************************************************************************************/
vuint8 FblHdrGetBasePartIdFromModuleId(const vuint8 mid)
{
   vuint8 partId;
   vuint8 baseMid;

#if defined( FBL_ENABLE_CALIBRATION_MODULES )
   if (IsIdOfCalForOpsw1(mid))
   {
      baseMid = MODID_OPSW_1;
   }
   else if (IsIdOfCalForOpsw2(mid))
   {
      baseMid = MODID_OPSW_2;
   }
   else if (IsIdOfCalForOpsw3(mid) )
   {
      baseMid = MODID_OPSW_3;
   }
   else if (IsIdOfCalForOpsw4(mid) )
   {
      baseMid = MODID_OPSW_4;
   }
   else
#endif
   {
      baseMid = mid;
   }

   if (IsIdOfOpsw1(baseMid))
   {
      partId = PARTID_OPSW_1;
   }
   else if (IsIdOfOpsw2(baseMid))
   {
      partId = PARTID_OPSW_2;
   }
   else if (IsIdOfOpsw3(baseMid) )
   {
      partId = PARTID_OPSW_3;
   }
   else if (IsIdOfOpsw4(baseMid) )
   {
      partId = PARTID_OPSW_4;
   }
   else
   {
      partId = NO_PART_ID_DEFINED;
   }
   return partId;
}
#endif

/***********************************************************************************************************************
 *  FblHdrChkIdPresent
 **********************************************************************************************************************/
/*! \brief       Check on given module ID the presence of the module.
    \detail      Helper function to get present state for given module
    \pre         mid is valid
 *  \param[in]   mid Module ID of any module
 *  \return      kFblOk / kFblFailed
 **********************************************************************************************************************/
static tFblResult FblHdrChkIdPresent(vuint8 mid)
{
   tPartPresState psiState;
# if !defined( FBL_ENABLE_CALIBRATION_MODULES ) && !defined( FBL_ENABLE_ASSERTION )
  (void) mid;
# endif

   assertFblInternal((MOD_TYPE_NOTSUPPORTED != FblGetModType(mid)), kFblAssertParameterValid); /* PRQA S 2214 */ /* MD_FblHdr_2214 */

   /* Parameter of FblHdrGetBasePartIdFromModuleId() is only used in case calibration modules are supported */
   psiState = ApplFblChkPSIState(FblHdrGetBasePartIdFromModuleId(mid));

#if defined( FBL_ENABLE_CALIBRATION_MODULES )
   if (MOD_TYPE_CAL == FblGetModType(mid))
   {
      tCalPartitionInfo calPinfo;
      tCalInfo dummyCalInfo;

      calPinfo.partID = 0;
      /* Only if base module is valid, partition info can be parsed */
      if (PSI_PART_PRESENT == psiState)
      {
         if (kFblOk == FblHdrInitCalPartInfoAndCalInfo(mid, &calPinfo, &dummyCalInfo))
         {
            psiState = ApplFblChkPSIState(calPinfo.partID);
         }
         else
         {
            psiState = PSI_PART_INVALID;
         }
      }
   }
#endif

   return (tFblResult)((PSI_PART_PRESENT != psiState)? kFblFailed : kFblOk);
}

/***********************************************************************************************************************
 *  FblHdrCheckModuleValidAndGetAddr
 **********************************************************************************************************************/
/*! \brief       Return address of where downloaded header is stored in memory, if module is valid;
 *               return NO_HEADER_ADDR_DEFINED else.
 *  \detail      This function is also called on mid input from external. The mid therefore has to be checked to be
                 of supported module type first.
 *  \param[in]   mid Module ID (any ID, including calibration IDs)
 *  \return      tFblAddress address where header is stored.
 **********************************************************************************************************************/
tFblAddress FblHdrCheckModuleValidAndGetAddr(const vuint8 mid)
{
   tFblAddress retVal = NO_HEADER_ADDR_DEFINED;

   if (MOD_TYPE_NOTSUPPORTED != FblGetModType(mid))
   {
      if (kFblOk == FblHdrChkIdPresent(mid))
      {
#if defined( FBL_ENABLE_CALIBRATION_MODULES )
         if (MOD_TYPE_CAL == FblGetModType(mid))
         {
            tCalInfo locCalInfo;
            tCalPartitionInfo locPartInfo;
            tFblResult readRetVal = kFblOk;

            locPartInfo.partID = 0;
            {
               if (kFblOk == FblHdrInitCalPartInfoAndCalInfo(mid, &locPartInfo, &locCalInfo))
               {
                  /* Get the start address of the calibration module */
                  retVal = FblHdrGetIntegerRom(&readRetVal, HDR_REGADDR_LEN, (locCalInfo.startAddr+HDR_MID_LEN+HDR_DCID_LEN));

                  if (kFblOk != readRetVal)
                  {
                     retVal = NO_HEADER_ADDR_DEFINED;
                  }
               }
            }
         }
         else
#endif
         {
            retVal = FblHdrGetBaseModuleHdrAddr(FblHdrGetBasePartIdFromModuleId(mid));
         }
      }
   }

   return retVal;
} /* PRQA S 6080 */ /* MD_FblHdr_60xx */

/***********************************************************************************************************************
 *  FblGetModType
 **********************************************************************************************************************/
/*! \brief       Translates ID to Module Type
 *  \param[in]   mid Module ID (any ID, including calibration IDs)
 *  \return      Module Type
 **********************************************************************************************************************/
vuint8 FblGetModType(const vuint8 mid)
{
   vuint8 locModType;

   if (ModIdOpSw(mid))
   {
      locModType = MOD_TYPE_APPL;
   }
#if defined( FBL_ENABLE_CALIBRATION_MODULES )
   /* not supported in case of non multiple modules configuration */
   else if (ModIdCalibration(mid))
   {
      locModType = MOD_TYPE_CAL;
   }
#endif
   else
   {
      locModType = MOD_TYPE_NOTSUPPORTED;
   }

   return locModType;
}

/***********************************************************************************************************************
 *  FblHdrCheckEnvelopesExtractSignedHeader
 **********************************************************************************************************************/
/*! \brief       Returns pointer to buffer containing Signed header information required during download.
 *  \details     Checks envelope information and extracts Signed header information in case it is
 *               received compressed / encrypted.
 *  \param[in]   rcvBuffer
 *  \return      pointer to Signed header information
 **********************************************************************************************************************/
const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * FblHdrCheckEnvelopesExtractSignedHeader( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * const rcvBuffer, tTpDataType rcvDataSize)
{
   const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * retBuffer = V_NULL;
   vuint16 locDataType;
#if defined ( FBL_ENABLE_COMPRESSION_MODE )
   static  vuint8 FblRamHeader[RAM_HEADER_LENGTH];
#endif

   locDataType = (vuint16) FblMemGetInteger(HDR_DATATYPE_LEN, &rcvBuffer[HDR_DATYPE_IDX]);

   switch (locDataType)
   {
      case DATA_TYPE_SIGNED_V1:
         {
            if (rcvDataSize < HDR_MODULE_MAX_RAW_LEN)
            {
               /* Very small cal files may hit here. These have to be filled to have at minimum (SWM_MAX_NOAR * HDR_REGINFO_LEN)
                * content length */
               FblErrDebugStatus(kHdrDebugErrTransferTooSmall);  /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
               FblDiagSetFirstTransferTooSmall();
            }
            else
            {
               currMemLibSegment.dataFormat = MODULE_DF_RAW;
               retBuffer = rcvBuffer;
            }
            break;
         }
#if defined ( FBL_ENABLE_COMPRESSION_MODE )
      case DATA_TYPE_COMPRESSED:
      {
         tProcParam envProcParam;

         /* Initialize compressedLength to either 0 (if value is n/a) or compressed length, read start of compressed data in header. */
         if(kFblOk != ApplFblCmprReadHeader(&compressedLength, rcvBuffer, &rcvBuffHandle.buffIdx))
         {
            /* retBuffer already set to V_NULL */
            FblSetPecErr(Err_Compression); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
         }
         else
         {
            envProcParam.dataBuffer = (V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3* ) &rcvBuffer[rcvBuffHandle.buffIdx]; /* PRQA S 0311 */ /* MD_FblHdr_0311 */
            envProcParam.dataLength = rcvDataSize; /* The complete signed header part need always to get in within first transferData */
            envProcParam.dataOutBuffer = FblRamHeader;
            envProcParam.dataOutLength = 0;
            envProcParam.dataOutMaxLength = RAM_HEADER_LENGTH;
            envProcParam.wdTriggerFct = FblRealTimeSupport;
            envProcParam.mode = MODULE_DF_COMPR_HDR;

            if (    (kFblOk != ApplFblInitDataProcessing(&envProcParam)) /* PRQA S 3415 */ /* MD_FblHdr_3415_2 */
                 || (kFblOk != ApplFblDataProcessing(&envProcParam))
               )
            {
               FblSetPecErr(Err_Compression); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
            }

            if (envProcParam.dataOutLength < HDR_MODULE_MAX_RAW_LEN)
            {
               /* Very small cal files may hit here. These have to be filled to have at minimum (SWM_MAX_NOAR * HDR_REGINFO_LEN)
                * content length */
               FblErrDebugStatus(kHdrDebugErrTransferTooSmall);  /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
               FblDiagSetFirstTransferTooSmall();
            }
            else
            {
               /* Deinit is not allowed to produce any data, all data wanted is already decompressed. */
               envProcParam.dataOutMaxLength = 0u;
               if ( kFblOk != ApplFblDeinitDataProcessing(&envProcParam))
               {
                  FblSetPecErr(Err_Compression); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
               }

               locDataType = (vuint16) FblMemGetInteger(HDR_DATATYPE_LEN, &FblRamHeader[HDR_DATYPE_IDX]);

               if (DATA_TYPE_SIGNED_V1 != locDataType)
               {
                  FblSetPecErr(Err_DataType); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
               }
               else
               {
                  /* Compression flag shall be set here. */
                  currMemLibSegment.dataFormat = MODULE_DF_COMPR;
                  retBuffer = FblRamHeader;
               }
            }
         }
         break;
      }
#endif
      default:
      {
# if defined( FBLHDR_USER_DATA_TYPE )
         /* Input rcvBuffer, get retBuffer and dataFormat */
         ApplFblCheckEnvelopesExtractSignedHeader(retBuffer, rcvBuffer, &currMemLibSegment.dataFormat);
         if (V_NULL == retBuffer)
# endif
         {
            FblSetPecErr(Err_DataType); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
         }
         break;
      }
   }

   return retBuffer;
}

/***********************************************************************************************************************
 *  FblHdrGetIntegerRom
 **********************************************************************************************************************/
/*! \brief       This function extracts 'count' bytes from addr and converts it into an integer
 *  \param[in/out]  result Allows to indicate an error to the caller. The value is unchanged in the case of
 *                  positive result. It is set kFblFailed in case of error. This allows for checking result only after
 *                  a sequence of calls and distinguish "all Ok" from "one failed".
 *  \param[in]   count Number of bytes to extract from array
 *  \param[in]   addr Address in memory where data is located
 *  \return      Extracted integer number
 **********************************************************************************************************************/
static vuint32 FblHdrGetIntegerRom( tFblResult* const result, vuint8 count, tFblAddress addr )
{
   vuint8 buffer[4];
   vuint32 retVal = 0;

   /* Read from memory */
   if (4 != FblReadProm(addr, buffer, 4))
   {
     *result = kFblFailed;
     /* keep "0" for retVal */
   }
   else
   {
      retVal = FblMemGetInteger(count, buffer);
   }

   return retVal;
}

#if defined( FBL_ENABLE_CALIBRATION_MODULES ) || defined( FBL_ENABLE_VERIFY_INTEGRITY_WORD )
/***********************************************************************************************************************
 *  FblHdrGetRegionRomAndInc
 **********************************************************************************************************************/
/*! \brief       This functions extracts a full segment information from Rom.
 *  \detail      It is used in calibration file configuration to read Information from Application Plain Header.
 *  \param[out]  segInfo read Segment info will be written to here
 *  \param[in]   baseAddr constant offset to be added to relAddr
 *  \param[in/out] relAddr variable addr part that is incremented after having read segment information parts
 *  \return      kFblOk/kFblFailed read result
 **********************************************************************************************************************/
static tFblResult FblHdrGetRegionRomAndInc( FL_SegmentInfoType* segInfo, tFblAddress baseAddr, tFblAddress* relAddr)
{
   tFblResult retVal = kFblOk;

   segInfo->targetAddress = FblHdrGetIntegerRom(&retVal, HDR_REGADDR_LEN, baseAddr + *relAddr);
   *relAddr+= HDR_REGADDR_LEN;
   segInfo->length = FblHdrGetIntegerRom(&retVal, HDR_REGLEN_LEN, baseAddr + *relAddr);
   *relAddr+= HDR_REGLEN_LEN;
   return retVal;
}
#endif

#if defined( FBL_ENABLE_GAP_FILL )
/***********************************************************************************************************************
 *  FblHdrPartSegmentListGetBlockStatus
 **********************************************************************************************************************/
/*! \brief       Read from partBlockList FlashBlock usage information.
 *               The information might be:
 *               - BLOCK_USED_BY_PARTITION
 *               - BLOCK_UNUSED_AND_IN_OTHER_PARTITION
 *               - BLOCK_UNUSED_AND_IN_BASEPARTITION
 *               - BLOCK_UNUSED_NOT_IN_BASEPARTITION
 *  \param[in]   blockNr FlashBlock Nbr for which usage information is required
 *  \param[in]   partId Partition for which usage information is required
 *  \return      tFblBlockPartUsage Block usage information
 **********************************************************************************************************************/
static tFblBlockPartUsage FblHdrPartSegmentListGetBlockStatus( vsint16 blockNbr, vuint8 partId )
{
   vuintx i;
   FL_SegmentInfoType segment;

   for (i=0; i < HDR_MAX_PARTITION_TRACKED_REGIONS; i++)
   {
      (void)FblRealTimeSupport();

      if (    (kFblDiagMemSegmNotFound != partBlockList.segList[i].startBlk)
           && (blockNbr >= partBlockList.segList[i].startBlk) && ( blockNbr <= (partBlockList.segList[i].endBlk)) )
      {
#if defined( FBL_ENABLE_CALIBRATION_MODULES) || defined( FBL_ENABLE_MULTI_PROCESSOR )
         if (partId != partBlockList.segList[i].partID)
         {
             return BLOCK_UNUSED_AND_IN_OTHER_PARTITION;
         }
         else
#endif
         {
             return BLOCK_USED_BY_PARTITION;
         }
      }
   }

   segment.targetAddress = FlashBlock[blockNbr].begin;
   segment.length = FBL_UTIL_GET_RANGE_LEN(FlashBlock[blockNbr].begin, FlashBlock[blockNbr].end);

   if (kFblOk == FblHdrCheckMtabIncludesSegment(partId, &segment))
   {
      return BLOCK_UNUSED_AND_IN_BASEPARTITION;
   }
   else
   {
      return BLOCK_UNUSED_NOT_IN_BASEPARTITION;
   }
}
#endif

/***********************************************************************************************************************
 *  FblHdrPartSegmentListAddEntry
 **********************************************************************************************************************/
/*! \brief       This functions checks the Segment information and updates partBlockList list, if the region is accepted.
 *               Checks:
 *               - covered by FlashBlock table
 *               - covered by Logical Block Table
 *               - not overlapping with already read in partition information (cal region <-> appl region)
 *  \param[in]   partId Partition ID part to add to list *
 *  \param[in]   segment Segment in partition to add to list
 *  \return      kFblOk Adding entry successful
 *               kFblFailed Adding entry failed, or overlapping of partition regions
 **********************************************************************************************************************/
static vuintx FblHdrPartSegmentListAddEntry( vuint8 partId, FL_SegmentInfoType const* segment)
{
   vuintx i;
   vuintx retVal = PARTITION_REGION_UNDEF;
   tPartitionSegment newSeg;

#if defined( FBL_ENABLE_CALIBRATION_MODULES) || defined( FBL_ENABLE_MULTI_PROCESSOR )
   newSeg.partID = partId;
#endif
   newSeg.startBlk = FblMemSegmentNrGet(segment->targetAddress);
   newSeg.endBlk = FblMemSegmentNrGet(FBL_UTIL_GET_RANGE_END(segment->targetAddress, segment->length));

   if (     (kFblOk != FblHdrCheckMtabIncludesSegment(partId, segment))
         || (kFblDiagMemSegmNotFound == newSeg.startBlk)
         || (kFblDiagMemSegmNotFound == newSeg.endBlk) )
   {
      retVal = PARTITION_REGION_ERROR;
   }

   for (i=0; (i<HDR_MAX_PARTITION_TRACKED_REGIONS) && (PARTITION_REGION_UNDEF == retVal); i++)
   {
      (void)FblRealTimeSupport();

      if(kFblDiagMemSegmNotFound == partBlockList.segList[i].startBlk) /* "kFblDiagMemSegmNotFound" marks unused elements */
      {
         /* Add entry and leave loop*/
         partBlockList.segList[i] = newSeg;
         retVal = i;
      }
      else
      {
#if defined( FBL_ENABLE_ASSERTION ) && (defined( FBL_ENABLE_CALIBRATION_MODULES) || defined( FBL_ENABLE_MULTI_PROCESSOR ))
         if (!FBL_UTIL_CHECK_RANGES_EXCLUSIVE_ADDR(newSeg.startBlk, newSeg.endBlk,
                                                   partBlockList.segList[i].startBlk, partBlockList.segList[i].endBlk))
         {
            if ( newSeg.partID != partBlockList.segList[i].partID )
            {
               /* Different partitions need to use different physical Flash blocks */
               retVal = PARTITION_REGION_ERROR;
            }
         } /* if !(FBL_UTIL .. */
#else
          /* loop further */
#endif /* FBL_ENABLE_ASSERTION */
      } /* i == HDR_MAX_PARTITION_TRACKED_REGIONS */
   } /* for ..*/

   if (i == HDR_MAX_PARTITION_TRACKED_REGIONS)
   {
      retVal = PARTITION_REGION_ERROR;
   }

   return retVal;
}

/***********************************************************************************************************************
 *  FblHdrValidateSignerInfo
 **********************************************************************************************************************/
/*! \brief       Validate Signer Info
 *  \pre         This function is called after module signer info is loaded to signInfoAddr buffer location
 *  \param[in]   signInfoAddr address to signer Info information in download buffer
 *  \return      Address of public key stored in signer info.  In case of error, V_NULL is returned
 **********************************************************************************************************************/
static vuint8 const * FblHdrValidateSignerInfo( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * const signInfoAddr, vuint16 keyNbToCheck )
{
   tFblResult cmpResult;
   vuint8 const * retVal = V_NULL;

   /* Subject name = Stored? */
   cmpResult = FblHdrMemCompare( &signInfoAddr[SIGNINFO_SUBJNAME_IDX], FblHeader.SUBJNAME, HDR_SUBJNAME_LEN );
   if (kFblOk != cmpResult)
   {
      FblSetPecErr(Err_SubjectName); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
   }
   /* KeyNBID needs to be read before Signature Verification */
   else if (     (kFblOk != ApplFblNVMReadKeyNBID(&keyNBIDInfo))
              || (keyNbToCheck < keyNBIDInfo.stored) )
   {
      FblSetPecErr(Err_KeyNBID); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
   }
   /* Validate signer Info using Root Public key from Boot Info Block and Root Signature from Signer Info downloaded */
   /* Macro APPL_FBL_READ_SEC_DATA can be used to copy SecM_Production_RsaExp to Ram if required */
   else if (kFblOk != FblHdrValidateSignature(&signInfoAddr[0], SIGNINFO_ROOTSIGN_IDX,
                                              &signInfoAddr[SIGNINFO_ROOTSIGN_IDX], APPL_FBL_READ_SEC_DATA(SecM_RsaMod)))
   {
      FblSetPecErr(Err_RootSignature); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
   }
   else
   {
      retVal = &signInfoAddr[SIGNINFO_SIGNPKEY_IDX];
   }

   return retVal;
}

/***********************************************************************************************************************
 *  FblHdrCheckAndInitAppNBID
 **********************************************************************************************************************/
/*! \brief       Check Applicaiton NBID
 *  \pre         This function is called only after module download header information was received.
 *  \param[in]   locInfoAddr address to Location Info information in download buffer
 *  \return      kFblOk=Success; kFblFailed=Failure
 **********************************************************************************************************************/
static tFblResult FblHdrCheckAndInitAppNBID( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * const locInfoAddr )
{
   tFblResult retVal = kFblFailed;

#if defined( FBL_ENABLE_MULTI_PROCESSOR )
   appNBIDInfo.MID = parsedModuleInfo.MID;
#endif
   appNBIDInfo.stored = 0;
   appNBIDInfo.received = (vuint16) FblMemGetInteger(HDR_NBID_LEN, locInfoAddr);

   if (kFblOk == ApplFblNVMReadAppNBID(&appNBIDInfo))
   {
      if (appNBIDInfo.received >= appNBIDInfo.stored)
      {
         retVal = kFblOk;
      }
   }
   else
   {
      /* After discussion with GM: security is more important than continuation of download */
      /* Make sure ApplFblNVMReadAppNBID is as reliable as possible! */
   }

   /* pec error is set externally */
   return retVal;
}

/***********************************************************************************************************************
 *  FblHdrCheckAndParseSwLocInfo
 **********************************************************************************************************************/
/*! \brief       Initialize and check Applicaiton Location Info
 *  \detail      Check this:
 *                 - SW address ranges are compared to boot Info block.
 *                 - Not overlapping PSI information
 *                 - Check FblHdrGetBaseModuleHdrAddr() implementation return the same value than the address
 *                   downloaded (to ensure data can be accessed later on)
 *  \pre         This function is called only after module download header information was received.
 *  \param[in]   locInfoAddr address to Location Info information in download buffer
 *  \return      kFblOk=Success; kFblFailed=Failure
 **********************************************************************************************************************/
static tFblResult FblHdrCheckAndParseSwLocInfo( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * const locInfoAddr )
{
   vuintx i;
   vuintx parseIdx;
   FL_SegmentInfoType prPtn;
   tFblResult retVal = kFblOk;

   parsedModuleInfo.segmentList.nrOfSegments = (vuint8) FblMemGetInteger(HDR_NBROF_LEN, locInfoAddr);
   parseIdx = HDR_NBROF_LEN;

   if (parsedModuleInfo.segmentList.nrOfSegments > SWM_MAX_NOAR)
   {
      /* pec error is set externally */
      FblErrDebugStatus(kDiagErrValueOutOfRange);  /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
      return kFblFailed;
   }

   /* Prepare PSI data structures: transform address and length information to block information */
   FblHdrGetPsiRegion(parsedModuleInfo.partID, &prPtn.targetAddress, &prPtn.length);

   /* Extract the Address-Region table */
   for (i=0; (kFblOk == retVal) && (i < parsedModuleInfo.segmentList.nrOfSegments); ++i)
   {
      FL_SegmentInfoType currSeg;

      (void)FblRealTimeSupport();

      currSeg.targetAddress = (tFblAddress) FblMemGetInteger(HDR_REGADDR_LEN, &locInfoAddr[parseIdx]);
      parseIdx += HDR_REGADDR_LEN;
      currSeg.length = (tFblLength) FblMemGetInteger(HDR_REGLEN_LEN, &locInfoAddr[parseIdx]);
      parseIdx += HDR_REGLEN_LEN;

      /* Check region is not overlapping with PSI region and included in Boot Info space */
      if (    (PARTITION_REGION_ERROR == FblHdrPartSegmentListAddEntry(parsedModuleInfo.partID, &currSeg))
           || (! FBL_UTIL_CHECK_RANGES_EXCLUSIVE_LEN(prPtn.targetAddress, prPtn.length, /* PRQA S 3321 */ /* MD_FblHdr_3321 */
                  currSeg.targetAddress, currSeg.length))
         )
      {
         /* pec error is set externally */
         FblErrDebugStatus(kHdrDebugErrIlegalSwRegion);  /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
         retVal = kFblFailed;
      }
      else
      {
         parsedModuleInfo.segmentList.segmentInfo[i] = currSeg;
      }
   }
   parsedModuleInfo.segmentList.nrOfSegments = (vuint8)i;

   /* Add Pressence Pattern Region to take into account for FlashBlock partition usage check */
   if (PARTITION_REGION_ERROR == FblHdrPartSegmentListAddEntry(parsedModuleInfo.partID, &prPtn))
   {
      /* pec error is set externally. Note that kFblFailed  can already be set here. */
      FblErrDebugStatus(kHdrDebugErrConflictingPSILocation);  /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
      retVal = kFblFailed;
   }

   /* if FblHdrGetBaseModuleHdrAddr returns NO_HEADER_ADDR_DEFINED, it will never match parsedModuleInfo.segmentList.segmentInfo[0].targetAddress */
   if (parsedModuleInfo.segmentList.segmentInfo[0].targetAddress != FblHdrGetBaseModuleHdrAddr(parsedModuleInfo.partID) )
   {
      /* pec error is set externally. Note that kFblFailed can already be set here. */
      FblErrDebugStatus(kHdrDebugErrInvalidApplHeaderAddress);  /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
      retVal = kFblFailed;
   }

   return retVal;
}

/***********************************************************************************************************************
 *  FblHdrCheckEcuNameAndId
 **********************************************************************************************************************/
/*! \brief       Check for Ecu name and Id
 *  \pre         This function is called only after module download header information was received.
 *  \param[in]   ecuNameAddr address to Ecu Name information in download buffer
 *  \param[in]   zeroCheck perform zero-check or not
 *  \return      kFblOk=Success; kFblFailed=Failure
 **********************************************************************************************************************/
static tFblResult FblHdrCheckEcuNameAndId( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * const ecuNameAddr, tEcuIdZeroCheck zeroCheck )
{
   tFblResult retVal = kFblFailed;
   const vuint8 zeroesId[HDR_ECUID_LEN] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
   vuint8 ecuIdVal[HDR_ECUID_LEN];

   if (kFblOk != FblHdrMemCompare(ecuNameAddr, FblHeader.ECUNAME, HDR_ECUNAME_LEN ))
   {
      FblSetPecErr(Err_EcuName); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
   }
   else
   {
      if (zeroCheck == ECUID_DO_ZEROCHECK)
      {
         if (kFblOk == FblHdrMemCompare(&ecuNameAddr[HDR_ECUNAME_LEN], &zeroesId[0], HDR_ECUID_LEN ))
         {
            retVal = kFblOk;
         }
      }

      if (kFblOk != retVal)
      {
         if (kFblOk == ApplFblNVMReadECUID(ecuIdVal))
         {
            if (kFblOk == FblHdrMemCompare(&ecuNameAddr[HDR_ECUNAME_LEN], &ecuIdVal[0], HDR_ECUID_LEN))
            {
               retVal = kFblOk;
            }
         }
      }

      if (kFblOk != retVal)
      {
         FblSetPecErr(Err_EcuId); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
      }
   }
   return retVal;
}

/***********************************************************************************************************************
 *  FblHdrCheckDownloadDcid
 **********************************************************************************************************************/
/*! \brief       Check for compatibility of downloaded module
 *  \pre         This function is called only after module download header information was received.
 *  \param[in]   dcidAddr address to Dcid information in download buffer
 *  \return      kFblOk=Success; kFblFailed=Failure
 **********************************************************************************************************************/
static tFblResult FblHdrCheckDownloadDcid( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * const dcidAddr )
{
   tFblResult retVal = kFblOk;
   vuint16 locDcidNewMod;
   vuint16 locDcidRefMod;

#if defined( FBL_ENABLE_CALIBRATION_MODULES )
   if( MOD_TYPE_CAL == parsedModuleInfo.modType )
   {
      /* if FblHdrGetIntegerRom fails, locDcidRefMod gets 0, this will lead to an error in the checks below */
      locDcidRefMod = (vuint16) FblHdrGetIntegerRom(&retVal, HDR_DCID_LEN, calInfo.startAddr + HDR_MID_LEN);
   }
   else
#endif
   {
      locDcidRefMod = (vuint16) FblMemGetInteger(HDR_DCID_LEN, FblHeader.DCID);
   }

   locDcidNewMod = (vuint16) FblMemGetInteger(HDR_DCID_LEN, dcidAddr);

   /* If DCID is available, verify the data-compatibility ID */
   if (
#if defined( FBL_ENABLE_CALIBRATION_MODULES )
         (kFblOk == retVal) && /* FblHdrGetIntegerRom may have set retVal to kFblFailed */
#endif
         (locDcidNewMod != (vuint16)0xFFFFu) && (locDcidRefMod != (vuint16)0xFFFFu)
     )
   {
      if (locDcidNewMod != locDcidRefMod)
      {
         retVal = kFblFailed;
      }
   }

   return retVal;
}

/***********************************************************************************************************************
 *  FblHdrInitBaseModInfo
 **********************************************************************************************************************/
/*! \brief       Initialize pBaseModInfo with all relevant information for Base Sw Module
 *  \detail      Parse Application programmed header Information not related to Cal partitions
 *  \pre         application is programmed
 *  \param[in]   partId partition ID (possible values: 0x01-0x0F, 0x11-0x19, 0x21-0x29, 0x31-0x39)
 *  \param[in/out]  pBaseModInfo pointer to Application Info related information
 *  \return      kFblOk=Success; kFblFailed=Failure
 **********************************************************************************************************************/
static tFblResult FblHdrInitBaseModInfo( vuint8 partId, V_MEMRAM1 tBaseModInfo V_MEMRAM2 V_MEMRAM3 * const pBaseModInfo )
{
   tFblResult retVal = kFblOk;
   vuint16 locParseIdx = P1_APP_ASCIIPN_IDX;
   tFblAddress modHdrAddr;

   /* Get header address of op/sw */
   modHdrAddr = FblHdrGetBaseModuleHdrAddr(FblHdrGetBasePartId(partId));

   if (NO_HEADER_ADDR_DEFINED == modHdrAddr)
   {
      retVal = kFblFailed;
   }
   else
   {
      if (ASCII_PN_PRESENT == (ASCII_PN_PRESENT & FblHdrGetIntegerRom(&retVal, HDR_CFGOPT_LEN, modHdrAddr + P1_CFGOPT_IDX)))
      {
         locParseIdx += HDR_ASCIIPN_LEN;
      }
   }

   if (kFblOk != retVal)
   {
      /* Error reading the application SW Info */
      FblSetPecErr(Err_GetAppInfo); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
   }

   /* pBaseModInfo->swLocInfoAddr now points to loc info section->Num of SW Regions */
   pBaseModInfo->startAddr = modHdrAddr;
   pBaseModInfo->swLocInfoAddr = modHdrAddr + locParseIdx; /* PRQA S 3382 */ /* MD_FblHdr_3382*/
   pBaseModInfo->nbrOfRgns = (vuint8) FblHdrGetIntegerRom(&retVal, HDR_NBROF_LEN, modHdrAddr + locParseIdx); /* PRQA S 3382 */ /* MD_FblHdr_3382.3 */
   pBaseModInfo->nbPartAddr = pBaseModInfo->swLocInfoAddr + HDR_NBROF_LEN +
                             (pBaseModInfo->nbrOfRgns * (HDR_REGADDR_LEN + HDR_REGLEN_LEN));
   return retVal;
}

#if defined( FBL_ENABLE_CALIBRATION_MODULES )
/***********************************************************************************************************************
 *  FblHdrInitNextCalPartInfo
 **********************************************************************************************************************/
/*! \brief       Read partition info data from partition Number
 *  \detail      Initialize calPartInfo based on knowing the address of the partition info in memory.
 *  \pre         pCalPartInfo is initialized with partId, partInfoAddr and lengthPart
 *  \param[in/out]  pCalPartInfo pointer to "Partition Info" related information
 *  \return      kFblOk=Success; kFblFailed=Failure
 **********************************************************************************************************************/
static tFblResult FblHdrInitNextCalPartInfo(V_MEMRAM1 tCalPartitionInfo V_MEMRAM2 V_MEMRAM3 * const pCalPartInfo)
{
   tFblResult retVal = kFblOk;

   if (FblHdrGetCalPartNbrFromId(pCalPartInfo->partID) > pCalPartInfo->nbrOfPrts)
   {
      /* There is no further partition Info */
      retVal = kFblFailed;
   }
   else
   {
      /* Set address to start of next partition info block */
      pCalPartInfo->partInfoAddr += pCalPartInfo->lengthPart;

      /* partInfoAddr now points to "partition Info x->Num of Regions" */
      pCalPartInfo->nbrOfRgns = (vuint8) FblHdrGetIntegerRom(&retVal, HDR_NBROF_LEN, (pCalPartInfo->partInfoAddr));

      /* lengthPart points to region information for the last partition region */
      pCalPartInfo->lengthPart = HDR_NBROF_LEN + ((pCalPartInfo->nbrOfRgns-1) * (HDR_REGINFO_LEN));

      {
         /* set psiStart address */
         pCalPartInfo->psiStart = (tFblAddress) FblHdrGetIntegerRom(&retVal, HDR_REGADDR_LEN, pCalPartInfo->partInfoAddr +
               pCalPartInfo->lengthPart);
         pCalPartInfo->psiStart += (tFblAddress) FblHdrGetIntegerRom(&retVal, HDR_REGLEN_LEN, pCalPartInfo->partInfoAddr +
               pCalPartInfo->lengthPart + HDR_REGADDR_LEN );
         pCalPartInfo->psiStart -= (2 * FBL_PP_SEGMENT_SIZE);
      }

      /* lengthPart points to "partition Info x->Num of Calibrations" */
      pCalPartInfo->lengthPart += HDR_REGINFO_LEN;

      /* set number of calibrations */
      pCalPartInfo->nbrOfCals = (vuint8) FblHdrGetIntegerRom(&retVal, HDR_NBROF_LEN, (pCalPartInfo->partInfoAddr +
                                                              pCalPartInfo->lengthPart));

      /* lengthPart is the total length of this partition info block */
      pCalPartInfo->lengthPart += HDR_NBROF_LEN + (pCalPartInfo->nbrOfCals * HDR_CALINFO_LEN);
   }

   return retVal;
}

/***********************************************************************************************************************
 *  FblHdrInitCalPartInfo
 **********************************************************************************************************************/
/*! \brief       Initialize calPartInfo with all relevant information for calPartId
 *  \detail      Parse Application programmed header from and sequentially all partitions that are located before the
 *               partition, which is intended to read up to the requested partition. Once the requested partition info
 *               location is determined, initialize calPartInfo based on partition information.
 *  \pre         application is programmed, pCalPartInfo.partID is initialized
 *  \param[in]   calPartId partition ID of a calibration partition (possible values: 0x02-0x0F, 0x12-0x19, 0x22-0x29, 0x32-0x39)
 *  \param[in/out]  pCalPartInfo pointer to "Partition Info" related information
 *  \return      kFblOk=Success; kFblFailed=Failure
 **********************************************************************************************************************/
static tFblResult FblHdrInitCalPartInfo( vuint8 calPartId, V_MEMRAM1 tCalPartitionInfo V_MEMRAM2 V_MEMRAM3 * pCalPartInfo )
{
   tFblResult retVal = kFblOk;

   /* Calibration partition ID cannot be 0 or 1 */
   assertFblInternal(PartIdCal(calPartId), kFblAssertParameterValid); /* PRQA S 2214 */ /* MD_FblHdr_2214 */

   /* Check if we already have partition information for the current partition */
   if (calPartId == pCalPartInfo->partID)
   {
      /* Partition information already initialized, nothing to do  */
   }
   /* Check if we already have partition information for the previous calibration partition */
   else if ((calPartId - 1) == pCalPartInfo->partID)
   {
      /* Get partition information for current partition */
      pCalPartInfo->partID = calPartId;
      retVal = FblHdrInitNextCalPartInfo(pCalPartInfo);
   }
   else
   {
      /* Get Application Module Info to retrieve partition info start */
      tBaseModInfo locApplModInfo;
      retVal = FblHdrInitBaseModInfo(calPartId, &locApplModInfo);
      pCalPartInfo->nbrOfPrts = (vuint8) FblHdrGetIntegerRom(&retVal, HDR_NBROF_LEN, locApplModInfo.nbPartAddr);

      /* Check if any failures occured, any partitions exist, current partition is within bounds */
      if ((kFblOk != retVal) || (FblHdrGetCalPartNbrFromId(calPartId) > pCalPartInfo->nbrOfPrts))
      {
         retVal = kFblFailed;
      }
      else
      {
         assertFblInternal(0 != pCalPartInfo->nbrOfPrts, kFblAssertInternalIlegalState);  /* PRQA S 2214 */ /* MD_FblHdr_2214 */

         /* Setting of partition Id validates the information */
         pCalPartInfo->partID = calPartId;
         /* pCalPartInfo->partInfoAddr now points to "partition Info 1->Num of Regions" */
         pCalPartInfo->partInfoAddr = locApplModInfo.nbPartAddr+HDR_NBROF_LEN;
         pCalPartInfo->lengthPart = 0;

         while ( ( FblHdrGetCalPartNbrFromId(calPartId) > 0u) && (kFblOk == retVal) )
         {
            (void)FblRealTimeSupport();

            retVal = FblHdrInitNextCalPartInfo(pCalPartInfo);
            calPartId--;
         }
      }
   }

   if (kFblOk != retVal)
   {
      /* Invalidate partition information */
      pCalPartInfo->partID = 0;
   }

   return retVal;
}

/***********************************************************************************************************************
 *  FblHdrInitCalPartInfoAndCalInfo
 **********************************************************************************************************************/
/*! \brief       Init cal ID related "Partion Info" and "Cal Info" data from programmed application plain header.
 *  \detail      Calls FblHdrInitCalPartInfo in order to initialize "Partition Info" to pCalPartInfo. Use that information
 *               to init pCalInfo for requested calMid.
 *  \pre         application is programmed
 *  \param[in]   calMid Calibration Module ID for which data shall be initialized
 *  \param[in/out]  pCalPartInfo pointer to "Partition Info" related information
 *  \param[out]  pCalInfo pointer to "Calibration Info" related information
 *  \return      kFblOk=Success; kFblFailed=Failure
 **********************************************************************************************************************/
tFblResult FblHdrInitCalPartInfoAndCalInfo( vuint8 calMid, V_MEMRAM1 tCalPartitionInfo V_MEMRAM2 V_MEMRAM3 * const pCalPartInfo,
                                            V_MEMRAM1 tCalInfo V_MEMRAM2 V_MEMRAM3 * const pCalInfo )
{
   tFblResult retVal = kFblOk;
   vuint8 calMidFound = 0;
   tFblAddress currCalInfoAddress;
   vuint8 currCalMid;
   vuint8 currCalModNbr;
   vuint8 partCounter; /* start counting partitions with "1..n", will increase before get */
   vuint8 opSwPartId;

   assertFblInternal((MOD_TYPE_CAL == FblGetModType(calMid)), kFblAssertInternalIlegalState); /* PRQA S 2214 */ /* MD_FblHdr_2214 */

   /* Parameter of FblHdrGetBasePartIdFromModuleId is only used in case of Multi Processor configuration */
   opSwPartId = FblHdrGetBasePartIdFromModuleId(calMid);

   /* for nbr part, check nbr calibrations for mid */
   for (partCounter = 0; (calMidFound == 0) && (kFblOk == retVal); partCounter++)
   {
      (void)FblRealTimeSupport();

      retVal = FblHdrInitCalPartInfo(FblHdrGetFirstCalPartNbr(opSwPartId) + partCounter, pCalPartInfo);
      if (kFblOk == retVal)
      {
         currCalInfoAddress = pCalPartInfo->partInfoAddr + (pCalPartInfo->nbrOfRgns * HDR_REGINFO_LEN) + (2*HDR_NBROF_LEN);
         for(currCalModNbr = FIRST_CAL_NBR; (calMidFound == 0) && (currCalModNbr <= pCalPartInfo->nbrOfCals); currCalModNbr++)
         {
            (void)FblRealTimeSupport();

            currCalMid = (vuint8) FilterMid(FblHdrGetIntegerRom(&retVal, HDR_MID_LEN, currCalInfoAddress));
            if (kFblOk == retVal)
            {
               if (calMid == currCalMid)
               {
                  pCalInfo->startAddr = currCalInfoAddress;
                  pCalInfo->calNbr = currCalModNbr;

                  /* retVal may be failed, in case FblHdrGetIntegerRom did not read correctly */
                  calMidFound = 1;
               }
               else
               {
                  currCalInfoAddress += HDR_CALINFO_LEN;
               }
            }
         }
      }
   }

   if (calMidFound != 1)
   {
      retVal = kFblFailed;
   }

   return retVal;
} /* PRQA S 6080 */ /* MD_FblHdr_60xx */

/***********************************************************************************************************************
 *  FblHdrSetCalModuleInfo
 **********************************************************************************************************************/
/*! \brief       Set parsedModuleInfo fields calPos, segmentList and partId from application partition information.
 *  \pre         application is programmed
 *  \return      kFblOk=Success; kFblFailed=Failure
 **********************************************************************************************************************/
static tFblResult FblHdrSetCalModuleInfo( void )
{
   tFblResult retVal = kFblOk;
   tFblAddress currReadAddr;
   vuint8 expModId;

   parsedModuleInfo.segmentList.nrOfSegments = 1;

   if (calInfo.calNbr > calPartitionInfo.nbrOfCals)
   {
      FblSetPecErr(Err_ModuleID); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
      retVal = kFblFailed;
   }
   else
   {
      expModId = (vuint8) FilterMid(FblHdrGetIntegerRom(&retVal, HDR_MID_LEN, calInfo.startAddr));
      if (parsedModuleInfo.MID != expModId)
      {
         FblSetPecErr(Err_ModuleID); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
         retVal = kFblFailed;
      }
      else
      {
         /* Init Cal Position */
         parsedModuleInfo.calPos = CALPOS_IRRELEVANT;
         /* If both flags are set position is CALPOS_SINGLE */
         if ( calInfo.calNbr == calPartitionInfo.nbrOfCals)
         {
            parsedModuleInfo.calPos |= CALPOS_LAST;
         }
         if (FIRST_CAL_NBR == calInfo.calNbr )
         {
            parsedModuleInfo.calPos |= CALPOS_FIRST;
         }

         currReadAddr = calInfo.startAddr + HDR_MID_LEN + HDR_DCID_LEN;
         retVal = FblHdrGetRegionRomAndInc(&parsedModuleInfo.segmentList.segmentInfo[0], (tFblAddress)0, &currReadAddr);

         parsedModuleInfo.partID = calPartitionInfo.partID;
      }
   }

   return retVal;
}
#endif /* #if defined( FBL_ENABLE_CALIBRATION_MODULES ) */

/***********************************************************************************************************************
 *  FblHdrCheckAndParseDownloadMid
 **********************************************************************************************************************/
/*! \brief       Check and parse Mid and Mid related information
 *  \pre         This function is called only after SWM header information was received.
 *  \param[in]   moduleAddr Address where Module is in Memory
 *  \return      kFblOk=Success; kFblFailed=Failure
 **********************************************************************************************************************/
static tFblResult FblHdrCheckAndParseDownloadMid( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * const moduleAddr )
{
   tFblResult retVal = kFblOk;
   parsedModuleInfo.MID = (vuint8) FilterMid(FblMemGetInteger(HDR_MID_LEN, moduleAddr));
   parsedModuleInfo.modType = FblGetModType(parsedModuleInfo.MID);

   /* Unexpected App module ID will lead to MOD_TYPE_NOTSUPPORTED */
   if (MOD_TYPE_NOTSUPPORTED == parsedModuleInfo.modType)
   {
      FblSetPecErr(Err_ModuleID); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
      retVal = kFblFailed;
   }
   else
   {
      /* parsedModuleInfo.partID is going to be overwritten for calibration modules */
      parsedModuleInfo.partID = FblHdrGetBasePartIdFromModuleId(parsedModuleInfo.MID);
#if defined( FBL_ENABLE_CALIBRATION_MODULES )
      if ( MOD_TYPE_APPL == parsedModuleInfo.modType )
      {
# if defined( FBL_ENABLE_MULTI_PROCESSOR )
         if (UNDEFINED_LOG_BLOCK == GetBlockNbrFromPartId(parsedModuleInfo.partID))
         {
            FblSetPecErr(Err_PartitionId); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
            retVal = kFblFailed;
         }
         else
# endif
         {
            /* Reset Cal states, prepare change of partition context (erase will still take place)  */
            calPartActive = NO_CALPART_ACTIVE;
         }
      }
#endif
#if defined( FBL_ENABLE_CALIBRATION_MODULES )
      else if ( MOD_TYPE_CAL == parsedModuleInfo.modType )
      {
         if (PSI_PART_PRESENT != ApplFblChkPSIState(FblHdrGetBasePartIdFromModuleId(parsedModuleInfo.MID)))
         {
            DiagNRCConditionsNotCorrectOrRequestSeqErr();
            FblSetPecErr(Err_SwNotPresent); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
            retVal = kFblFailed;
         }
         else
         {
            /* Pec error set internally in case of failure */
            retVal = FblHdrPrepareDownloadCalPartionData(parsedModuleInfo.MID);
            if( kFblOk == retVal )
            {
               if (NO_CALPART_ACTIVE == calPartActive )
               {
                  /*  If the Cal ID received is not the first to be programmed for that partition, the bootloader
                   *  shall return an error indicating a non-supported partition ID.
                   */
                  FblSetPecErr(Err_PartitionId); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
                  retVal = kFblFailed;
               }
               else
               {
                  if (CALPART_START_SEQU == calPartActive )
                  {
                     /* Proceed to cal partition active state  */
                     calPartActive = CALPART_ACTIVE;
                  }
                  else
                  {
                     calInfo.calNbr++;
                     calInfo.startAddr +=HDR_CALINFO_LEN;
                  }
                  retVal = FblHdrSetCalModuleInfo();
               }
            }
         }
      }
      else
      {
         /* Nothing to do */
      }
#endif /* FBL_ENABLE_CALIBRATION_MODULES */
   }

   return retVal;
} /* PRQA S 6080 */ /* MD_FblHdr_60xx */

/***********************************************************************************************************************
 *  FblHdrCheckAndInitDownloadModuleInfo
 **********************************************************************************************************************/
/*! \brief       Check Signed header elements and Init parsedModuleInfo structure.
 *  \detail      Checks: Mid, BCID/CCID, ECU name, ECU ID, Sw location Info
 *  \pre         This function is called only after SWM header information was received.
 *  \param[in]   rcvBuffer Address where Signed Header Information is in memory
 *  \return      kFblOk=Success; kFblFailed=Failure
 **********************************************************************************************************************/
static tFblResult FblHdrCheckAndInitDownloadModuleInfo( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * const signedHdrAddr )
{
   tFblResult retVal = kFblFailed;

   if (kFblOk != FblHdrCheckAndParseDownloadMid( &signedHdrAddr[S1_MID_IDX] ))
   {
      /* Do nothing, pec error set internally */
   }
   else if (kFblOk != FblHdrCheckDownloadDcid( &signedHdrAddr[S1_DCID_IDX] ))
   {
      FblSetPecErr(Err_CID); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
   }
   else if (kFblOk != FblHdrCheckEcuNameAndId( &signedHdrAddr[S1_ECUNAME_IDX], ECUID_DO_ZEROCHECK ))
   {
      /* Do nothing, pec error set internally */
   }
   else
   {

#if defined( FBL_ENABLE_CALIBRATION_MODULES )
      if (MOD_TYPE_CAL == parsedModuleInfo.modType)
      {
         retVal = kFblOk;
      }
      else
#endif
      {
         if (kFblOk != FblHdrCheckAndParseSwLocInfo( &signedHdrAddr[S1_APP_SWLINFO_IDX]))
         {
            FblDiagSetErrSwRegion();
            FblSetPecErr(Err_SwRegion); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
         }
         else
         {
            retVal = kFblOk;
         }
      }
   }

   return retVal;
} /* PRQA S 6080 */ /* MD_FblHdr_60xx */

/***********************************************************************************************************************
 *  FblHdrSetMemLibMemBlockInfo
 **********************************************************************************************************************/
/*! \brief       Set LibMem segment address and length information
 **********************************************************************************************************************/
static void FblHdrSetMemLibMemBlockInfo( void )
{
   static SecM_VerifyParamType verifyParam;
   static SecM_SignatureParamType verifyParamPipelined;
   static vuint32 dataLengthVar;
   vuintx verifyBlockNr = GetBlockNbrFromPartId(parsedModuleInfo.partID);

   /* Init verifyParam data */
   verifyParam.segmentList = parsedModuleInfo.segmentList;

   verifyParam.wdTriggerFct = (FL_WDTriggerFctType)FblHdrRealTimeSupportSecMod;
   verifyParam.readMemory = (tFblMemVerifyReadFct)FblReadProm;

   /* Init RoutinePipe Param */
   verifyParamPipelined.wdTriggerFct = (FL_WDTriggerFctType)FblHdrRealTimeSupportSecMod;
   verifyParamPipelined.currentDataLength = &dataLengthVar;
   verifyParamPipelined.currentHash.length = 0;
   verifyParamPipelined.currentHash.sigResultBuffer = 0;

   blockInfo.verifyRoutineOutput.function = (tFblMemVerifyFctOutput)(FblLogicalBlockTable.logicalBlock[verifyBlockNr].verifyROM); /* PRQA S 0313 */ /* MD_FblHdr_0313 */
   blockInfo.verifyRoutineOutput.param    = &verifyParam;

   assertFblGen(((tVerificationFct)FblLogicalBlockTable.logicalBlock[0].verifyROM == FblHdrVerifyIntegrity), kFblAssertCorrectGenyConfig); /* PRQA S 0313 */ /* MD_FblHdr_0313 */ /* PRQA S 2214 */ /* MD_FblHdr_2214 */

   {
      blockInfo.verifyRoutineInput.function  = (tFblMemVerifyFctInput) 0;
      blockInfo.verifyRoutineInput.param     = V_NULL;
      blockInfo.verifyRoutinePipe.function   = (tFblMemVerifyFctInput)(FblLogicalBlockTable.logicalBlock[verifyBlockNr].verifyRAM); /* PRQA S 0313 */ /* MD_FblHdr_0313 */
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
 *  FblHdrSetMemLibSegmentAddrLengthInfo
 **********************************************************************************************************************/
/*! \brief       Set LibMem segment address and length information
 **********************************************************************************************************************/
static void FblHdrSetMemLibSegmentAddrLengthInfo( void )
{
   currMemLibSegment.targetAddress  = currDataSegInfo.targetAddress;
   currMemLibSegment.targetLength   = currDataSegInfo.length;
   /* Logical address and length not used */
   currMemLibSegment.logicalAddress = 0;
   currMemLibSegment.logicalLength  = 0;
}

/***********************************************************************************************************************
 *  FblHdrProcessHeaderInfo
 **********************************************************************************************************************/
/*! \brief       Init basic header information for currently downloaded module to parsedModuleInfo structure,
                 compare to compatibilty check to referrence module.
 *  \pre         This function is called only after SWM header information was received.
 *  \param[in]   rcvBuffer Address where Signed Header Information is in memory
 *  \return      vuint16 Index of Plain Header inside rcvBuffer
 **********************************************************************************************************************/
static tTpDataType FblHdrProcessHeaderInfo( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * const signedHeaderAddr )
{
   tTpDataType retVal;
   tFblResult valResult;
   vuint8 const * signerPublicKeyAddr;
   vuint16 idxMsgDigest;
   vuint16 idxSignerInfo;
   vuint16 idxSignature;
   vuint16 locKeyNbidToCheck;

   if ( kFblOk != FblHdrCheckAndInitDownloadModuleInfo(signedHeaderAddr))
   {
      retVal = DOWNLOAD_MODULE_NOT_ACCEPTED;
   }
   else
   {
      /* Validate 1.Signer Info and 2. Signed Header Signature */
#if defined( FBL_ENABLE_CALIBRATION_MODULES )
      if (MOD_TYPE_CAL == parsedModuleInfo.modType)
      {
         idxMsgDigest = S1_CAL_MSGDIGEST_IDX;
      }
      else
#endif
      {
         idxMsgDigest = S1_APP_SWLINFO_IDX + 2 + (8 * parsedModuleInfo.segmentList.nrOfSegments);
      }

      idxSignerInfo = idxMsgDigest+HDR_MSGDIGEST_LEN;
      idxSignature = idxSignerInfo+HDR_SIGINFO_LEN;

      locKeyNbidToCheck = (vuint16) FblMemGetInteger(HDR_NBID_LEN, &signedHeaderAddr[idxSignerInfo+SIGNINFO_KEYNBID_IDX]);
      signerPublicKeyAddr = FblHdrValidateSignerInfo( &signedHeaderAddr[idxSignerInfo], locKeyNbidToCheck );

      if (V_NULL == signerPublicKeyAddr)
      {
         /* Pec error code is set internally */
         retVal = DOWNLOAD_MODULE_NOT_ACCEPTED;
      }
      else
      {
         /* Keep largest value during update */
         if (locKeyNbidToCheck > keyNBIDInfo.received)
         {
            keyNBIDInfo.received = locKeyNbidToCheck;
         }

         valResult = FblHdrValidateSignature(&signedHeaderAddr[S1_MID_IDX], idxSignature-S1_MID_IDX,
               &signedHeaderAddr[idxSignature], signerPublicKeyAddr);

         if (FblHdrGetSBITicketValid() || (kFblOk == valResult)  )
         {
            if (
#if defined( FBL_ENABLE_CALIBRATION_MODULES )
                  (MOD_TYPE_APPL == parsedModuleInfo.modType) && /* PRQA S 3415 */ /* MD_FblHdr_3415 */
#endif
                  (kFblOk != FblHdrCheckAndInitAppNBID( &signedHeaderAddr[S1_APP_NBID_IDX]))
               )
            {
               FblSetPecErr(Err_AppNBID); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
               retVal = DOWNLOAD_MODULE_NOT_ACCEPTED;
            }
            else
            {
               retVal = idxSignature+HDR_SIGNATURE_LEN;
            }
         }
         else
         {
            FblSetPecErr(Err_Signature); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
            retVal = DOWNLOAD_MODULE_NOT_ACCEPTED;
         }
      }

      /* Keep message digest for later verification */
      (void) MEMCPY(msgDigest, &signedHeaderAddr[idxMsgDigest], sizeof(msgDigest));

      /* Parse until DATA_TYPE_PLAIN_V1 signed Header end marker */
      if ( DOWNLOAD_MODULE_NOT_ACCEPTED != retVal )
      {
         /* Initialize to value different from DATA_TYPE_PLAIN_V1*/
         vuint16 dataTypeParse = DATA_TYPE_SIGNED_V1;

         /* Parse until DATA_TYPE_PLAIN_V1 information */
         while ( (retVal <= ((idxSignature + HDR_SIGNATURE_LEN) + FBL_HDR_MAX_S1_FILL)) && (DATA_TYPE_PLAIN_V1 != dataTypeParse))
         {
            (void)FblRealTimeSupport();

            dataTypeParse = (vuint16) FblMemGetInteger(HDR_DATATYPE_LEN, &signedHeaderAddr[retVal]);
            retVal+=2; /* when the value is read, still increment by two */
         }

         if (dataTypeParse != DATA_TYPE_PLAIN_V1)
         {
            FblErrDebugStatus(kDiagErrP1DataTypeNotPresent); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
            FblSetPecErr(Err_DataType); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
            retVal = DOWNLOAD_MODULE_NOT_ACCEPTED;
         }
      }
   }
   return retVal;
} /* PRQA S 6050 */ /* MD_FblHdr_60xx */

/***********************************************************************************************************************
 *  FblHdrTransferDataHeaderReceived
 **********************************************************************************************************************/
/*! \brief       This function is called on every first transfer data received for a new module.
 *  \details     It parses the header information inside the buffer data and checks for the received module type.
 *  \param[in]   rcvBuffer received data bytes
 *  \return      vuint16 Index of Plain Header inside rcvBuffer
 **********************************************************************************************************************/
static tTpDataType FblHdrTransferDataHeaderReceived( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * const signedHeaderAddr)
{
   tTpDataType retVal;
   tFblMemStatus memLibResult;

   (void)FblRealTimeSupport();

   retVal = FblHdrProcessHeaderInfo(signedHeaderAddr);

   if (DOWNLOAD_MODULE_NOT_ACCEPTED == retVal)
   {
      /* Pec already set inside FblHdrProcessHeaderInfo, if applicable */
      return DOWNLOAD_MODULE_NOT_ACCEPTED;
   }

   /* modules applicable for erasure are checked inside function,
      for non applicable modules function returns kFblOk. */
# if defined( FBL_ENABLE_CALIBRATION_MODULES )
   if ( (( !IsCalDownLd() ) || ( IsCalFirstModInPartition() )) )
# endif
   {
      if (kFblOk != FblHdrEraseRoutine(parsedModuleInfo.partID))
      {
         return DOWNLOAD_MODULE_NOT_ACCEPTED;
      }
   }

   currDataSegIdx = 0;
   currDataSegInfo = parsedModuleInfo.segmentList.segmentInfo[currDataSegIdx];
   currMemLibSegment.type = kFblMemType_ROM;

#if defined( FBL_ENABLE_SYSTEM_CHECK )
   currDataSegInitialLen = currDataSegInfo.length;
#endif

   (void)FblRealTimeSupport();

   /* Indicate the start of a module download */
   FblHdrSetMemLibMemBlockInfo();
   memLibResult = FblMemBlockStartIndication(&blockInfo);
   if (kFblMemStatus_Ok != memLibResult)
   {
      FblErrDebugStatus(kHdrDebugErrBlockStart);  /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
      FblDiagSetError((vuint8)memLibResult);
      retVal = DOWNLOAD_MODULE_NOT_ACCEPTED;
   }
   else
   {
      /* Indicate start of new segment */
      FblHdrSetMemLibSegmentAddrLengthInfo();
      memLibResult = FblMemSegmentStartIndication(&currMemLibSegment);
      if (kFblMemStatus_Ok != memLibResult)
      {
         FblErrDebugStatus(kHdrDebugErrSegStart);  /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
         FblDiagSetError((vuint8)memLibResult);
         retVal = DOWNLOAD_MODULE_NOT_ACCEPTED;
      }
   }

   return retVal;
} /* PRQA S 6050 */ /* MD_FblHdr_60xx */

/***********************************************************************************************************************
 *  FblHdrTransferDataProcess
 **********************************************************************************************************************/
/*! \brief       This function is to be called for newly received data.
 *  \details     It checks current rcvBuffHandle.transferState and decides if the currently processed transfer is the
 *               first, or any later transfer. FblHdrTransferDataHeaderReceived() is called for first
 *               transfer. TransferData Exit is allowed when all expected byte have been received.
 *               The function interfaces with the LibMem module to program the received data.
 *  \param[in]   rcvBuffer received data bytes
 *  \param[in]   rcvDataSize received data bytes
 **********************************************************************************************************************/
tFblResult FblHdrTransferDataProcess( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * const rcvBuffer, tTpDataType rcvDataSize)
{
   tFblResult retVal = kFblOk;
   tFblMemStatus memLibResult;
   tTpDataType bytesForDataInd;
   tTpDataType leftDataSize;

   /* Put RCRRP message in CAN transmit buffer so that FblCanRetransmit() can be called from WD during device-driver
    * erase/write operations */
   DiagExRCRResponsePending(kForceSendResponsePending);

   /* Init data processing state */
   rcvBuffHandle.buffIdx = 0;
   rcvBuffHandle.progDataOffset = 0;

   if (rcvBuffHandle.transferState == DIAG_TRANSFERDATA_HDR_EXPECTED)
   {
      const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * pSignedHeader;
# if defined ( FBL_ENABLE_COMPRESSION_MODE )
      /* compressedLength will read out from compression header and set in FblHdrCheckEnvelopesExtractSignedHeader */
      compressedLength = 0;
# endif

      /* Check for compression type/ signed type, forward rcvBuffer to Signed header structure */
      pSignedHeader = FblHdrCheckEnvelopesExtractSignedHeader(rcvBuffer, rcvDataSize);

      if (V_NULL == pSignedHeader)
      {
         /* Pec Error set in FblHdrCheckEnvelopesExtractSignedHeader */
         retVal = kFblFailed;
      }
      else
      {
         rcvBuffHandle.progDataOffset = FblHdrTransferDataHeaderReceived(pSignedHeader);
         if (DOWNLOAD_MODULE_NOT_ACCEPTED == rcvBuffHandle.progDataOffset)
         {
            retVal = kFblFailed;
         }
         else
         {
#if defined( FBL_ENABLE_DATA_PROCESSING )
            if (FblHdrModIsRawData())
#endif
            {
               /* no data processing interface is being called, add offset directly, not via Dfi */
               rcvBuffHandle.buffIdx += rcvBuffHandle.progDataOffset;
            }
         }
      }
      rcvBuffHandle.transferState = DIAG_TRANSFERDATA_IDLE;
   }

   leftDataSize = rcvDataSize-rcvBuffHandle.buffIdx;

# if defined ( FBL_ENABLE_COMPRESSION_MODE )
   /* For Raw data and/or compression not using compressedLength it will be 0 and thus never larger. */
   if (compressedLength >= (tFblLength) leftDataSize)
   {
      compressedLength -= (tFblLength) leftDataSize;
   }
# endif

   /* Only count consumed bytes == received bytes in case of no data processing */
   while ((leftDataSize > 0) && (kFblOk == retVal))
   {
      (void)FblRealTimeSupport();

      bytesForDataInd = (tTpDataType) FblHdrGetDataIndBytes(leftDataSize);

      memLibResult = FblMemDataIndication(rcvBuffer, rcvBuffHandle.buffIdx, bytesForDataInd);
      if (kFblMemStatus_Ok != memLibResult)
      {
         FblErrDebugStatus(kHdrDebugErrDataInd);  /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
         FblDiagSetError((vuint8)memLibResult);
         retVal = kFblFailed;
         break;
      }

#if defined( FBL_ENABLE_DATA_PROCESSING )
      if (FblHdrModIsRawData())
#endif
      {
         /* Update segment info.
          * Note:This is done behind data processing interface in case received data is not raw. */
         FblHdrConsumeCurrSegmentBytes(bytesForDataInd); /* PRQA S 3109 */ /* MD_CBD_14.3 */
         rcvBuffHandle.buffIdx += bytesForDataInd;
         /* Always reset progDataOffset after first programmed data chunk ( which includes the header ) */
         rcvBuffHandle.progDataOffset = 0;
      }

      /* rcvBuffHandle.buffIdx has been updated (inside FblMemDataIndication in case of compression ), also update leftDataSize */
      leftDataSize = rcvDataSize-rcvBuffHandle.buffIdx;

      if (currDataSegInfo.length == (tFblLength)0)
      {
         tFblLength addrRegionLength;
         memLibResult = FblMemSegmentEndIndication(&addrRegionLength);
         currDataSegIdx++;

         if (kFblMemStatus_Ok != memLibResult)
         {
            FblErrDebugStatus(kHdrDebugErrSegEnd);  /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
            FblDiagSetError((vuint8)memLibResult);
            retVal = kFblFailed;
         }
#if defined( FBL_ENABLE_SYSTEM_CHECK )
         else if (addrRegionLength != currDataSegInitialLen)
         {
            FblErrDebugStatus(kDiagErrValueOutOfRange);  /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
            FblDiagSetErrEndIndication();
            retVal = kFblFailed;
         }
#endif
         else if (currDataSegIdx < parsedModuleInfo.segmentList.nrOfSegments)
         {
            /* Start of new segment */
            currDataSegInfo = parsedModuleInfo.segmentList.segmentInfo[currDataSegIdx];
#if defined( FBL_ENABLE_SYSTEM_CHECK )
            currDataSegInitialLen = currDataSegInfo.length;
#endif

            FblHdrSetMemLibSegmentAddrLengthInfo();
            memLibResult = FblMemSegmentStartIndication(&currMemLibSegment);
            if (kFblMemStatus_Ok != memLibResult)
            {
               FblErrDebugStatus(kHdrDebugErrSegStart);  /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
               /* Pec Error set internally */
               FblDiagSetError((vuint8)memLibResult);
               retVal = kFblFailed;
            }
         }
         else
         {
            /* no further segments; Still data? */
            if (     (leftDataSize != 0)
#if defined ( FBL_ENABLE_COMPRESSION_MODE )
                  || (compressedLength != 0)
#endif
               )
            {
               FblDiagSetErrLengthExceeded();
               FblSetPecErr(Err_LengthExceeded); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
               retVal = kFblFailed;
            }
            else if (kFblOk != FblHdrTransferDataEnd())
            {
               /* Pec Error set internally */
               retVal = kFblFailed;
            }
            else
            {
               /* Module is completely downloaded */
               FblDiagDoCompleteTransfer(); /* PRQA S 0277 */ /* MD_FblHdr_0277 */
            }
         }
      }
   }

#if defined( FBL_ENABLE_CALIBRATION_MODULES )
   if (kFblOk != retVal)
   {
      calPartActive = NO_CALPART_ACTIVE;
   }
#endif

   return retVal;
} /* PRQA S 6010 */ /* PRQA S 6030 */ /* PRQA S 6050 */ /* PRQA S 6080 */ /* MD_FblHdr_FillGaps_60xx */

/***********************************************************************************************************************
 *  FblDiagTransferDataEnd
 **********************************************************************************************************************/
/*! \brief       When the last address-region of a module is downloaded, this function is called to handle the
 *               post-transfer tasks, such as checksum verification, signature verification, validation and device
 *               driver initialization.
 *  \return      kFblOk if the transferred data was successfully verified, kFblFailed if the verification failed.
 **********************************************************************************************************************/
static tFblResult FblHdrTransferDataEnd( void )
{
   tFblResult retVal = kFblFailed;
   tFblMemStatus memLibResult;

   memLibResult = FblMemBlockEndIndication();
   if (kFblMemStatus_Ok != memLibResult)
   {
      FblErrDebugStatus(kHdrDebugErrBlockEnd);  /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
      FblDiagSetError((vuint8)memLibResult);
   }
   else
   {
      retVal = FblHdrPostFlashValidation();
      if (kFblOk == retVal)
      {
         retVal = FblHdrUpdatePSI();
      }
   }

   return retVal;
}

/***********************************************************************************************************************
 *  FblHdrGetPsiRegion
 **********************************************************************************************************************/
/*! \brief       This function will return Psi region information
 *  \param[in]   partId Partition ID (any, including calibration)
 *  \param[out]   pPsiAddr PSI Region start address
 *  \param[out]   pPsiLen PSI Region length
 **********************************************************************************************************************/
void FblHdrGetPsiRegion (vuint8 partId, V_MEMRAM1 IO_PositionType V_MEMRAM2 V_MEMRAM3 * const pPsiAddr,
                         V_MEMRAM1 IO_SizeType V_MEMRAM2 V_MEMRAM3 * const pPsiLen)
{
   *pPsiAddr = NO_PP_ADDRESS_DEFINED;
   *pPsiLen = 0;

   if (PartIdOpSw(partId))
   {
      /* Partition is application */
      ApplFblGetBaseModulePPRegion(partId, pPsiAddr, pPsiLen);
   }
#if defined( FBL_ENABLE_CALIBRATION_MODULES )
   /* Partition is calibration */
   else if (PartIdCal(partId))
   {
       if ( kFblOk == FblHdrInitCalPartInfo(partId, &calPartitionInfo) )
       {
          *pPsiAddr = calPartitionInfo.psiStart;
          *pPsiLen  = 2*FBL_PP_SEGMENT_SIZE;
       }
   }
   else
   {
      /* Do nothing */
   }
#endif
}

#if defined( FBL_ENABLE_ROM_INTEGRITY_CHECK )
/***********************************************************************************************************************
 *  FblHdrInitFblRomCheck
 **********************************************************************************************************************/
/*! \brief       This function will read the Fbl header segment information to parsedModuleInfo.
 *  \pre         Only allowed to be called during init, not during programming
 *               (parsedModuleInfo structure is reserved then)
 *  \param[out]   segList put the FblHeader configured Region here.
 **********************************************************************************************************************/
void FblHdrInitFblRomCheck( V_MEMRAM1 FL_SegmentListType V_MEMRAM2 V_MEMRAM3 * const segList )
{
   vuintx i;
   /* Extract number of address regions */
   segList->nrOfSegments = FBL_GMHEADER_NOAR_FBL;
   for(i=0; i<segList->nrOfSegments; i++)
   {
      segList->segmentInfo[i].targetAddress = FblMemGetInteger(HDR_REGADDR_LEN,
                                               &FblHeader.AddressRegions[i * (HDR_REGADDR_LEN + HDR_REGLEN_LEN)]);
      segList->segmentInfo[i].length = FblMemGetInteger(HDR_REGLEN_LEN,
                                               &FblHeader.AddressRegions[(i * (HDR_REGADDR_LEN + HDR_REGLEN_LEN)) + HDR_REGADDR_LEN]);
   }
}
#endif /* FBL_ENABLE_ROM_INTEGRITY_CHECK */

#if defined( FBL_ENABLE_GAP_FILL )
/***********************************************************************************************************************
 *  FblHdrFillRegion
 **********************************************************************************************************************/
/*! \brief       This function writes fill bytes to a region within one FlashBlock.
 *  \param[in]   inBeginAddr start address of region to fill
 *  \param[in]   inLength length of region to fill
 *  \return      kFblOk / kFblFailed
 **********************************************************************************************************************/
static tFblResult FblHdrFillRegion( tFblAddress inBeginAddr, tFblLength inLength )
{
   vuint16 i;
   tFblResult retVal = kFblOk;
   tFblAddress fillChunkLen;
   vuint8 fillBuffer[HDR_GAPFILL_BUFF_SIZE];

   /* This assertion gets hit when some external memory device is used with higher Segment size than internal flash
    * (Writing fails through alignment issues, if assertion is disabled).
    * Please manually define HDR_GAPFILL_BUFF_SIZE as multiple of minimum write segment size for this device
    * to resolve this.
    */
#if defined( FBL_ENABLE_MULTIPLE_MEM_DEVICES )
   assertFblInternal( (((HDR_GAPFILL_BUFF_SIZE / MemDriver_SegmentSize) * MemDriver_SegmentSize) == HDR_GAPFILL_BUFF_SIZE), kFblAssertInternalIlegalState); /* PRQA S 2214 */ /* MD_FblHdr_2214 */
#endif

   /* call FblMemSegmentNrGet() to init MemDriver_SegmentSize */
   memSegment = FblMemSegmentNrGet(inBeginAddr);

   if (inLength == 0)
   {
      /* nothing to do */
   }
   else if (FblMemSegmentNrGet(FBL_UTIL_GET_RANGE_END(inBeginAddr, inLength)) != memSegment)
   {
      /* FblFillRegion() allows only to write within one Flashblock */
      retVal = kFblFailed;
   }
   else
   {
#if ( HDR_GAPFILL_BUFF_SIZE <= 0x40)
      (void)FblRealTimeSupport();
#endif
      for (i=0; i < HDR_GAPFILL_BUFF_SIZE; ++i)
      {
#if ( HDR_GAPFILL_BUFF_SIZE > 0x40)
         FblRealTimeSupportInLoop(i, HDR_GAPFILL_BUFF_SIZE)
#endif
         fillBuffer[i] = kFillChar;
      }

      while (inLength > 0)
      {
         (void)FblRealTimeSupport();

         fillChunkLen = ((inLength > HDR_GAPFILL_BUFF_SIZE) ? HDR_GAPFILL_BUFF_SIZE : inLength);

         if (kFblMemStatus_Ok != FblMemProgramBuffer(inBeginAddr, &fillChunkLen, fillBuffer))
         {
            retVal = kFblFailed;
            break;
         }
         inLength -= fillChunkLen;
         inBeginAddr += fillChunkLen;
      }
   }

   return retVal;
}

/***********************************************************************************************************************
 *  FblHdrGetSortedApplRegions
 **********************************************************************************************************************/
/*! \brief       Return requested Appl-Region in sorted order (0-first, 1-2nd, 2 ..)
 *               Appl Regions may be unsorted because of "Header segment must always lead" requirement.
 *  \param[out]   retSeg reference to requested sorted Region number
 *  \param[in]   regionNbr the (sorted) region number to read
 *  \return      kFblOk if regions were sucessfully saved.
 *               kFblFailed otherwise
 **********************************************************************************************************************/
static void FblHdrGetSortedApplRegions(FL_SegmentInfoType* retSeg, vuint8 regionNbr)
{
   vuintx hdrElemtRealNbr = 0;
   vuintx idxToRequiredRegion;
   vuintx i;

   for (i=0; i < (vuintx)(parsedModuleInfo.segmentList.nrOfSegments - 1); i++)
   {
      if (parsedModuleInfo.segmentList.segmentInfo[i].targetAddress > parsedModuleInfo.segmentList.segmentInfo[i+1].targetAddress)
      {
         (void)FblRealTimeSupport();
         /* There is just one region that is potentially not sorted (Header Segment)
          * If a segment start address > successor start address, then according to A) the successor is  the header address
          */
         hdrElemtRealNbr = i+1;
         break;
      }
   }

   /* In case the elements are sorted, hdrElemtRealNbr stays 0 */
   if (regionNbr == hdrElemtRealNbr)
   {
      /* The header element is the (hdrElemtRealNbr)th element in sorted order, but the first element in the list */
      idxToRequiredRegion = 0;
   }
   else if (regionNbr < hdrElemtRealNbr)
   {
      /* Elements below the (hdrElemtRealNbr) element are to be found with offset 1 (header segment is placed in front) */
      idxToRequiredRegion = regionNbr+1;
   }
   else
   {
      /* Elements after the (hdrElemtRealNbr) element are sorted */
      idxToRequiredRegion = regionNbr;
   }

   *retSeg = parsedModuleInfo.segmentList.segmentInfo[idxToRequiredRegion];
}

/***********************************************************************************************************************
 *  FblHdrFillGaps
 **********************************************************************************************************************/
/*! \brief       This function guarantees that all memory associated to a module is being filled with
 *               the configured fill pattern.
 *  \return      kFblOk if regions were sucessfully saved.
 *               kFblFailed otherwise
 **********************************************************************************************************************/
tFblResult FblHdrFillGaps( void )
{
   vsint16 i;
   IO_PositionType prPtnAddr;
   IO_SizeType prPtnLen;
   tFblResult retVal = kFblOk;

   tFblBlockPartUsage location;
   tFlashBlock currFlashBlock;
   vuint8 currRegion;
   vuint8 regionNumber;
   FL_SegmentInfoType currSeg;
#if defined( FBL_ENABLE_CALIBRATION_MODULES )
   tFblAddress calModInfoOffset;
   tFblAddress calModInfoBase = calPartitionInfo.partInfoAddr +
                                (calPartitionInfo.nbrOfRgns*HDR_REGINFO_LEN) + (2 * HDR_NBROF_LEN);

   /* Unused bytes for all calibration files are filled on download of calibration file with lowest ID. */
   if ( (MOD_TYPE_CAL == parsedModuleInfo.modType) && (!(IsCalFirstModInPartition())) )
   {
      /* Do nothing */
   }
   else
# endif
   {
      for (i=0; i < kNrOfFlashBlock; ++i)
      {
         currFlashBlock = FlashBlock[i];
         location = FblHdrPartSegmentListGetBlockStatus(i, parsedModuleInfo.partID);

         if ( (BLOCK_UNUSED_NOT_IN_BASEPARTITION == location) || (BLOCK_UNUSED_AND_IN_OTHER_PARTITION == location))
         {
            /* Prevent fill */
            currFlashBlock.begin = currFlashBlock.end;
         }
         else if( BLOCK_UNUSED_AND_IN_BASEPARTITION == location)
         {
#if defined( FBL_ENABLE_CALIBRATION_MODULES )
            if(MOD_TYPE_CAL == parsedModuleInfo.modType)
            {
               /* Prevent fill */
               currFlashBlock.begin = currFlashBlock.end;
            }
            else
#endif
            {
               /* Fill the whole Block. */
            }
         }
         else
         {

            /********************************************************************/
            /* Get the number of regions written within the partition. */
            /********************************************************************/
#if defined( FBL_ENABLE_CALIBRATION_MODULES )
            if (parsedModuleInfo.modType == MOD_TYPE_CAL)
            {
               regionNumber = calPartitionInfo.nbrOfCals;
               calModInfoOffset = 0;
            }
            else
#endif
            {
               regionNumber = parsedModuleInfo.segmentList.nrOfSegments;
            }

            for ( currRegion = 0; currRegion < regionNumber; currRegion++ )
            {
               (void)FblRealTimeSupport();

               /********************************************************************/
               /* Init currSeg to either next sorted cal module/Appl region        */
               /********************************************************************/
#if defined( FBL_ENABLE_CALIBRATION_MODULES )
               if (parsedModuleInfo.modType == MOD_TYPE_CAL)
               {
                  /* Get current cal file region */
                  calModInfoOffset+=HDR_MID_LEN + HDR_DCID_LEN; /* PRQA S 3353 */ /* MD_FblHdr_3353 */
                  /* Result does not have to be evaluated, it has been previously verified. */
                  (void) FblHdrGetRegionRomAndInc(&currSeg, calModInfoBase, &calModInfoOffset);
               }
               else
#endif
               {
                  FblHdrGetSortedApplRegions(&currSeg, currRegion);
               }

               /********************************************************************/
               /* Check currSeg position relative to Flashblock                    */
               /********************************************************************/
               if(!FBL_UTIL_CHECK_RANGES_EXCLUSIVE_ADDR(currFlashBlock.begin, currFlashBlock.end, currSeg.targetAddress,
                        FBL_UTIL_GET_RANGE_END(currSeg.targetAddress, currSeg.length)))
               {
                  tFblLength currFlashBlockLen = FBL_UTIL_GET_RANGE_LEN(currFlashBlock.begin, currFlashBlock.end);
                  if(1 == FBL_UTIL_CHECK_ADDRESS_IN_RANGE_LEN(currSeg.targetAddress, currFlashBlock.begin, currFlashBlockLen))
                  {
                     /* fill block start to segment Start */
                     if (kFblOk != FblHdrFillRegion(currFlashBlock.begin,
                              FblHdrGetAlignedAddr(currSeg.targetAddress, MemDriver_SegmentSize)-currFlashBlock.begin))
                     {
                        retVal = kFblFailed;
                        break;
                     }
                  }
                  currFlashBlock.begin = FblHdrGetAlignedAddr((currSeg.targetAddress + currSeg.length), MemDriver_SegmentSize);
               }
            } /* for( currRegion ..*/
         } /* (location == BLOCK_UNUSED_NOT_IN_BASEPARTITION) */

         FblHdrGetPsiRegion(parsedModuleInfo.partID, &prPtnAddr, &prPtnLen);
         if (!FBL_UTIL_CHECK_RANGES_EXCLUSIVE_ADDR(currFlashBlock.begin, currFlashBlock.end, prPtnAddr,
                  FBL_UTIL_GET_RANGE_END(prPtnAddr, prPtnLen)))
         {
            /* Only fill up to Presence Pattern Region (expected to be at end of Flashblock).
             * If there is memory behind pattern, it wont get filled (ilegal configuration).
             */
            assertFblGen((FBL_UTIL_GET_RANGE_END(prPtnAddr, prPtnLen) == currFlashBlock.end), kFblAssertConditionCorrect); /* PRQA S 2214 */ /* MD_FblHdr_2214 */
            currFlashBlock.end = prPtnAddr-1;
         }

         /* Fill remaining block to end */
         if (currFlashBlock.begin < currFlashBlock.end)
         {
            /* Fill last remaining gap in block ( if applicable, currFlashBlock.begin may be equal to currFlashBlock.end ) */
            if (kFblOk != FblHdrFillRegion(currFlashBlock.begin, FBL_UTIL_GET_RANGE_LEN(currFlashBlock.begin, currFlashBlock.end)))
            {
               retVal = kFblFailed;
               break;
            }
         }
      } /* for (i =0; i < kNrOfFlashBlock .. */
   }
   return retVal;
} /* PRQA S 6010 */ /* PRQA S 6030 */ /* PRQA S 6080 */ /* MD_FblHdr_FillGaps_60xx */
#endif

#if defined( FBL_ENABLE_CALIBRATION_MODULES )
/***********************************************************************************************************************
 *  FblHdrGetCalPartitionValid
 **********************************************************************************************************************/
/*! \brief       This function will check if all calibration partitions of an operating software are valid
 *  \pre         Operating software must be valid
 *  \param[in]   opSwPartId Operating software partition ID
 *  \return      kFblOk = all calibration partitions are valid; kFblFailed = at least 1 calibration partition is invalid
 **********************************************************************************************************************/
tFblResult FblHdrGetCalPartitionValid(vuint8 opSwPartId)
{
   vuint8 partCounter = 0;
   vuint8 retVal = kFblOk;
   vuint8 numbPartitions;

#if defined( FBL_ENABLE_CALIBRATION_MODULES )
   assertFblInternal((opSwPartId == FblHdrGetBasePartId(opSwPartId)), kFblAssertInternalIlegalState); /* PRQA S 2214 */ /* MD_FblHdr_2214 */
#endif

   /* Initialize first partition and get the number of partitions */
   numbPartitions = FblHdrGetNoOfCalPartitions(opSwPartId);

   /* Loop through all partitions */
   while ( (partCounter < numbPartitions) && (kFblOk == retVal) )
   {
      (void)FblRealTimeSupport();

     /* Check if current partition is valid */
      if (PSI_PART_PRESENT != ApplFblChkPSIState(FblHdrGetFirstCalPartNbr(opSwPartId) + partCounter))
      {
         retVal = kFblFailed;
      }

      partCounter++;
   }

   /* Reset calPartitionInfo.partID; This is usefull if Invalid Application Presence pattern is found
    * in memory and calPartion Info is ilegally initialized. This would lead to errorness state during development.
    */
   calPartitionInfo.partID = 0;

   return retVal;
}

/***********************************************************************************************************************
 *  FblHdrGetNoOfCalPartitions
 **********************************************************************************************************************/
/*! \brief       This function will return the number of calibration partitions available for a specific operating
 *               software
 *  \pre         Operating software must be valid
 *  \param[in]   inOpSwPartId Operating software partition ID
 *  \return      Number of calibration partitions
 **********************************************************************************************************************/
vuint8 FblHdrGetNoOfCalPartitions(vuint8 opSwPartId)
{
   vuint8 numbPartitions;
   vuint8 calPart1 = opSwPartId+1;

   if (kFblOk == FblHdrInitCalPartInfo(calPart1, &calPartitionInfo))
   {
      numbPartitions = calPartitionInfo.nbrOfPrts;
   }
   else
   {
      numbPartitions = 0;
   }

   return numbPartitions;
}

/***********************************************************************************************************************
 *  FblHdrPrepareDownloadCalPartionData
 **********************************************************************************************************************/
/*! \brief       Set Calibration Partition Data
 *  \detail      This function is called upon reception of a new calibration file.
 *               Partition properties will be loaded to calPartitionInfo. It is checked if the download is the first
 *               module of a partition or not. If it is the first module, calInfo and calPartActive are initialized
 *               additionally. Note: The first cal file received will trigger erasure of the partition in
 *               FblHdrTransferDataHeaderReceived.
 *  \return      kFblOk = data successfully set; kFblFailed = else
 *  \param[in]   calModId Calibration module ID
 **********************************************************************************************************************/
static tFblResult FblHdrPrepareDownloadCalPartionData( vuint8 calModId )
{
   tFblResult retVal;

   tCalInfo V_MEMRAM2 readReceivedModCalInfo;
   retVal = FblHdrInitCalPartInfoAndCalInfo(calModId, &calPartitionInfo, &readReceivedModCalInfo);
   if (kFblOk != retVal)
   {
      FblSetPecErr(Err_GetCalInfo); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
      retVal = kFblFailed;
   }
   else if (FIRST_CAL_NBR != readReceivedModCalInfo.calNbr)
   {
      /* calinfo already initialized. Increment calInfo within FblHdrCheckAndParseDownloadMid. */
   }
   else
   {
      calInfo.calNbr = FIRST_CAL_NBR;
      calInfo.startAddr = calPartitionInfo.partInfoAddr + (calPartitionInfo.nbrOfRgns * HDR_REGINFO_LEN) + (2*HDR_NBROF_LEN);
      calPartActive = CALPART_START_SEQU;
   }

   return retVal;
}

/***********************************************************************************************************************
 *  FblHdrEraseCalPartition
 **********************************************************************************************************************/
/*! \brief       This function will erase a single calibration partition
 *  \pre         Operating software must be valid, SLP5: calPartitionInfo needs to be initialized.
 *  \param[in]   Calibration partition ID
 *  \return      kFblOk = erase succeeded; kFblFailed = erase failed
 **********************************************************************************************************************/
static tFblResult FblHdrEraseCalPartition(vuint8 calPartId)
{
   tFblResult retVal = kFblOk;
   tFblMemBlockInfo blockInfoErase;
   vuint16 locNbOfReg;
   tFblAddress locAddrIdx;

# if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   (void) calPartId; /* Not used in this configuration */ /* PRQA S 3112 */ /* MD_FblHdr_3112 */
# endif
   {
      locAddrIdx = calPartitionInfo.partInfoAddr + HDR_NBROF_LEN;
      locNbOfReg = calPartitionInfo.nbrOfRgns;

      while ( (locNbOfReg > 0) && (kFblOk == retVal) )
      {
         FL_SegmentInfoType tmpSeg;

         (void)FblRealTimeSupport();

         /* Get the start address and length of the calibration partition */
         retVal = FblHdrGetRegionRomAndInc(&tmpSeg, (tFblAddress)0, &locAddrIdx);

         if (kFblOk == retVal)
         {
            blockInfoErase.targetAddress = tmpSeg.targetAddress;
            blockInfoErase.targetLength = tmpSeg.length;

            /* Erase partition */
            if (kFblMemStatus_Ok != FblMemBlockEraseIndication(&blockInfoErase))
            {
               FblSetPecErr(Err_EraseCal); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
               retVal = kFblFailed;
            }
#if defined( FBL_ENABLE_GAP_FILL )
            else
            {
               /* Track Partition Erase Segments for allowing Gap Fill */
               (void) FblHdrPartSegmentListAddEntry(calPartId, &tmpSeg);
            }
#endif
         }
         locNbOfReg--;
      }
   }
   return retVal;
} /* PRQA S 6050 */ /* MD_FblHdr_60xx */ /* PRQA S 6080 */ /* MD_FblHdr_60xx */
#endif /* defined( FBL_ENABLE_CALIBRATION_MODULES ) */

/***********************************************************************************************************************
 *  FblHdrEraseRoutine
 **********************************************************************************************************************/
/*! \brief       Invalidates the module and erases all blocks defined by the FlashBlock
 *               table that are occupied by the module being downloaded.
 *  \details     This routine will only do its job on programmable Modules.
 *               It checks the parsedModuleInfo addresses against FlashBlock
 *               table, and also checks if the corresponding module is allowed to erase a block.
 *  \param[in]   partition ID
 *  \return      kFblOk / kFblFailed.
 **********************************************************************************************************************/
static tFblResult FblHdrEraseRoutine( vuint8 partId )
{
   tFblResult retVal = kFblFailed;
   parsedModuleInfo.partID = partId;

   if ( PartIdOpSw(partId) )
   {
      vuint8 blockNbr = GetBlockNbrFromPartId(partId);
#if (FBL_MTAB_NO_OF_BLOCKS > 1)
      if (UNDEFINED_LOG_BLOCK == blockNbr)
      {
         FblSetPecErr(Err_PartitionId); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
      }
      else
#endif
      {
         /* Erase entire logical block for corresponding application */
         blockInfo.targetAddress = FblLogicalBlockTable.logicalBlock[blockNbr].blockStartAddress;
         blockInfo.targetLength = FblLogicalBlockTable.logicalBlock[blockNbr].blockLength;

         /* In case of application, only application PSI information is revoked */
         if (kFblOk != ApplFblInvalidateBlock(*FblGetBlockDescriptor(partId)))
         {
            FblSetPecErr(Err_Revoke); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
         }
         else if (kFblMemStatus_Ok != FblMemBlockEraseIndication(&blockInfo))
         {
            FblSetPecErr(Err_EraseSW); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
         }
         else
         {
            retVal = kFblOk;
         }
      }
   }
#if defined( FBL_ENABLE_CALIBRATION_MODULES )
   else if ( PartIdCal(partId) )
   {
      /* Pec error set internally */
      retVal = FblHdrEraseCalPartition(partId);
   }
#endif
   else
   {
      FblSetPecErr(Err_PartitionId); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
   }

   return retVal;
}

/***********************************************************************************************************************
 *  FblCalculateCsum16Bit
 **********************************************************************************************************************/
/*! \brief       Create 16-Bit checksum over one Ram buffer
 *  \details     Words are treated as 16-bit according to the native CPU-type (little/big endian)
 *  \param[in]   address Used as a 16-bit Pointer for memory access
 *  \param[in]   len Size of data for checksum calculation in bytes
 *  \param[in]   csum Start value of the checksum
 *  \return      calculated checksum
 **********************************************************************************************************************/
vuint16 FblCalculateCsum16Bit( const V_MEMRAM1 vuint16 V_MEMRAM2 V_MEMRAM3 * buffer, vuint32 len, vuint16 csum )
{
   vuint32 i = 0u;

   if (0x01u == ((vuintx)buffer & 0x01u)) /* PRQA S 0306 */ /* MD_FblHdr_0306 */
   {
      /* Avoid unaligned data access */
      csum = 0u;
   }
   else
   {
      /* Prevent endless loop when length is odd */
      while (len > 1u)
      {
         csum += buffer[i];
         i++;
         len -= 2u;

         FblRealTimeSupportInLoop(len, 0x40u);  /* PRQA S 3109 */ /* MD_FblDiag_3109_RealtimeInLoop */
      }
   }
   return csum;
}

#if defined( FBL_ENABLE_VERIFY_INTEGRITY_WORD )
/***********************************************************************************************************************
 *  FblHdrCheckDldIntegrityWord
 **********************************************************************************************************************/
/*! \brief       Verify download data against Plain Header Integrity word.
 *  \details     This is done additionally to the hash verification inside FblHdrPostFlashValidation.
 *               Plain Header Integrity word might be used later to verify the application correctness.
 *  \return      kFblOk / kFblFailed
 **********************************************************************************************************************/
SecM_StatusType FblHdrCheckDldIntegrityWord( V_MEMRAM1 SecM_SignatureParamType V_MEMRAM2 V_MEMRAM3 * pVerifyParam ) /* PRQA S 3673 */ /* MD_FblDiag_3673 */
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
            /* Segment list from download can be reused, it is checked to be identical to programmed */
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

/* module specific MISRA deviations:

   MD_FblHdr_3382
     Reason: Check for overflow of modHdrAddr + locParseIdx is not necessary, a configuration using such a critical value would be catched early
             when trying to download such an application.
     Risk: No risk.
     Prevention: No prvention.

   MD_FblHdr_2214:
     Reason: This is an assertion macro used only for debugging in development (FBL_ENABLE_ASSERTION).
     Risk: No risk.
     Prevention: Set Integration state other than Development.

    MD_FblHdr_3112:
     Reason: Dummy statements have been added to avoid compiler warnings because of unused parameters. Unused parameters may exist due to its usage in
             a different configured use case.
     Risk: Unused parameters add to program complexity just by being there.
     Prevention: Dummy statements can be turned off externally (V_DISABLE_USE_DUMMY_STATEMENT).

   MD_FblHdr_3321:
     Reason: prPtn.length is not unset. It is set in the call to FblHdrGetPsiRegion above.
     Risk: No risk
     Prevention: No prevention

    MD_FblHdr_3353:
     Reason: calModInfoOffset is not unset. It is set and used only if (parsedModuleInfo.modType == MOD_TYPE_CAL). In the case (parsedModuleInfo.modType != MOD_TYPE_CAL)
             it is neither set nor used.
     Risk: No risk
     Prevention: No prevention

    MD_FblHdr_3673:
     Reason: Standard Api that cannot be changed.
     Risk: No risk
     Prevention: No prevention

    MD_FblHdr_3415:
     Reason: The intended behavior is: Call the function only for Application modules.
     Risk: No risk
     Prevention: No prevention

    MD_FblHdr_3415_2:
     Reason: The intended behavior is: Call the Data processing function after Init function.
             Leave if any of these fail.
     Risk: No risk
     Prevention: No prevention

    MD_FblHdr_0310:
     Reason: This cast is used intentionally.
     Risk: No risk, only used in Development feature
     Prevention: No prevention

    MD_FblHdr_0311
     Reason: Data processing interface does not change pointer, though its data type does allow changes.
     Risk: No risk
     Prevention: No prevention

    MD_FblHdr_0306:
     Reason: Address conversion between integer values and pointers is required to allow for hardware independent configuration and address range checks.
     Risk: The size of integer required to hold the result of a pointer cast is implementation-defined.
     Prevention: The size of the respective integer data type which holds the address value is adapted on a hardware specific basis.

    MD_FblHdr_0277:
     Reason: This is hit due to the FblInvert16bit macro, which uses the ~ operator which results in a negative integer casted again to unsigned.
             This behavior is intended.
     Risk: No risk
     Prevention: No prevention

   MD_FblHdr_3201:
     Reason: This cast is used intentionally.
     Risk: No risk, only used in Development feature
     Prevention: No prevention

    MD_FblHdr_60xx:
     Reason: Monolytic functions chosen in order to save code size
     Risk: Complexity decreases readability and maintainability
     Prevention: Catch complexity through (regression) testing
*/

/***********************************************************************************************************************
 *  END OF FILE: FBL_HDR.C
 **********************************************************************************************************************/

