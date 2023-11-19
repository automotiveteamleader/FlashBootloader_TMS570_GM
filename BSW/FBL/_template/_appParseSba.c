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
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  01.00.00   2014-05-05  AWh                      creation
 *  01.01.00   2014-05-09  AWh                      Update External workspace use case
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#if defined( VGEN_ENABLE_CANFBL )
/* Bootloader types and definitions (e.g. SWM_DATA_MAX_NOAR) */
#define FBL_ENABLE_KEY_EXPORT
#endif /* VGEN_ENABLE_CANFBL */
#include "Sec.h"
#include "fbl_def.h"

/* #include "fbl_hdr.h" */
#include "appParseSba.h"

/***********************************************************************************************************************
 *   VERSION
 **********************************************************************************************************************/

#if ( APP_PARSE_SBA_VERSION != 0x0101u )
# error "Error in appParseSba.c: Source and Header file are inconsistent!"
#endif

#if ( APP_PARSE_SBA_RELEASE_VERSION != 0x00u )
# error "Error in appParseSba.c: Source and Header file are inconsistent!"
#endif

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/
#define MODID_SBA ((vuint16) 91u)

/* Sba Header length definitions */
#define HDR_DATATYPE_LEN 2u /**< Data type field length */
#define HDR_NBROF_LEN 2u /**< Length for fields depicting the number of entries, regions, partitions, cal modules */
#define HDR_MID_LEN 2u /**< Module Id field length, used in several headers */
#define HDR_NBID_LEN 2u /**<  Not Before Id field length, used in several headers (key, application) */
#define HDR_ECUNAME_LEN 8u /**< Ecu name field length */
#define HDR_ECUID_LEN 16u /**< Ecu Id field length */

#define HDR_SIGNATURE_LEN 256u /**< Signature field length (both root/signer signature) */
#define HDR_SUBJNAME_LEN 16u /**< Subject name field length */
#define HDR_CERTID_LEN 8u /**< Certificate Id field length ( the field itself is not used in the bootloader ) */
#define HDR_SIGNPKEY_LEN 256u /**< Public key field length */

/* For SBA-ticket different Indices have to be used (no starting HDR_DATYPE_IDX ) */
#define S1_SBA_MID_IDX 0u /**< SBA ticket Module-ID field index */
#define S1_SBA_ECUNAME_IDX (S1_SBA_MID_IDX + HDR_MID_LEN) /**< SBA ticket Ecu name index */
#define S1_SBA_SIGNINFO_IDX (S1_SBA_ECUNAME_IDX + (HDR_ECUNAME_LEN+HDR_ECUID_LEN)) /**< SBA ticket Signer Info index */
#define S1_SBA_SIGNATURE_IDX (S1_SBA_SIGNINFO_IDX + HDR_SIGINFO_LEN) /**< SBA ticket Signature index */

/***** Field static indexes in Signer Info ******/
#define SIGNINFO_SUBJNAME_IDX  0u /**< Signer Info Subject Name index */
#define SIGNINFO_CERTID_IDX (SIGNINFO_SUBJNAME_IDX + HDR_SUBJNAME_LEN) /**< Signer Info Certificate ID index */
#define SIGNINFO_KEYNBID_IDX (SIGNINFO_CERTID_IDX + HDR_CERTID_LEN) /**< Signer Info Key-NBID index */
#define SIGNINFO_SIGNPKEY_IDX (SIGNINFO_KEYNBID_IDX + HDR_NBID_LEN) /**< Signer Info Signer Public Key index */
#define SIGNINFO_ROOTSIGN_IDX (SIGNINFO_SIGNPKEY_IDX + HDR_SIGNPKEY_LEN) /**< Signer Info Root Signature index */

/* Parse results */
#define SBIMSB_ERR_MID ((vuint8) 0x00u) /* This is no BIS error */
#define SBIMSB_ERR_ECUNAME ((vuint8) 0x01u)
#define SBIMSB_ERR_ECUID ((vuint8) 0x02u)
#define SBIMSB_ERR_SIGNATURE ((vuint8) 0x04u)
#define SBIMSB_ERR_CERT ((vuint8) 0x08u)
#define SBIMSB_SBI_VALID ((vuint8) 0x80u)

#define AppParseSba_FilterMid(mid)  ((mid) & 0x007F)

/* Length of the entire signed header */
#define HDR_SIGINFO_LEN (HDR_SUBJNAME_LEN + HDR_CERTID_LEN + HDR_NBROF_LEN + HDR_SIGNPKEY_LEN + HDR_SIGNATURE_LEN)

/* Dummy Pec Error definitions */
#define Err_EcuName 0x01u
#define Err_EcuId 0x02u

/***********************************************************************************************************************
 *  LOCAL DATA
 **********************************************************************************************************************/
V_MEMRAM0 static V_MEMRAM1 vuint16 V_MEMRAM2 dummyPec; /**< Pec just used to follow same interface than in fbl_hdr.c */
V_MEMRAM0 static V_MEMRAM1 vuint16 V_MEMRAM2 storedKeyNBID; /**< Key Not Before ID stored value */

#if ( APP_SBA_CRYPTO_LIB_WORKSPACE == WORKSPACE_USE_OWN )   
struct {
   vuint32 alignHelper; /* this guarantees 4 byte alignment */
   unsigned char buffer[APP_SBA_WS_SIZE];
} applWorkSpace;
#endif

/***********************************************************************************************************************
 *  PROTOTYPES
 **********************************************************************************************************************/
static void AppParseSba_PollingRoutine( void );
static tFblResult AppParseSba_NVMReadKeyNBID( V_MEMRAM1 vuint16 V_MEMRAM2 V_MEMRAM3 * const storedKeyNBID );
static vuint32 AppParseSba_GetInteger( vuintx count, const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * const buffer );
static tFblResult AppParseSba_ValidateSignature( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * const valRegStart, SecM_LengthType valRegLen,
                                                 const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * const signatureStart,
                                                 const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3* const publicKey );
static tFblResult AppParseSba_CheckEcuNameAndId( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * const ecuNameAddr);
/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  AppParseSba_PollingRoutine
 **********************************************************************************************************************/
/*! \brief       validate Signature inside Application/Calibration/SBA Signed Header
 **********************************************************************************************************************/
static void AppParseSba_PollingRoutine( void )
{
   /* TODO (optional):
    *
    * This function is executed from within the Security module loops and can be used to call code that is
    * required to be cyclically executed. A standard use cases would be watchdog triggering.
    * Note that the function will be called at least every 1ms, but can be called considerably more often. So if you want
    * to adhere a certain timing you will have to poll for a hardware timer yourself.
    */
}

/***********************************************************************************************************************
 *  AppParseSba_NVMReadKeyNBID
 **********************************************************************************************************************/
/*! \brief       Read keyNBID to keyNBID->stored out of NVM
 *  \param[out]   Info structure holding buffer to read KeyNBID to
 *  \return      kFblOk / kFblFailed
 **********************************************************************************************************************/
static tFblResult AppParseSba_NVMReadKeyNBID( V_MEMRAM1 vuint16 V_MEMRAM2 V_MEMRAM3 * const inStoredKeyNBID )
{
   /* TODO (mandatory):
    *
    * Read the KeyNBID from its stored NVM location
    */

#if defined( APP_SBA_TEST_WITH_DUMMY )
   /* This is not to be used for final implementation */
   *inStoredKeyNBID = 0;
   return kFblOk;
#else
   return kFblFailed;
#endif
}

/***********************************************************************************************************************
 *  AppParseSba_NVMReadECUID
 **********************************************************************************************************************/
/*! \brief       Fills buffer with ECU ID from ECUID storage location.
 *  \param[out]  buffer to read ECUID to
 *  \return      kFblOk / kFblFailed result
 **********************************************************************************************************************/
static tFblResult AppParseSba_NVMReadECUID( V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * const buffer )
{
   /* TODO (mandatory):
    *
    * Read unique ECU_ID from stored NVM location
    */

#if defined( APP_SBA_TEST_WITH_DUMMY )
   /* This may be ONLY used for final implementation, if you store a UNIQUE EcuId in FblHeader (e.g. through 
    * writing it there via post process
    */
   {
      vuint8 i;
      for (i = 0; i<0x10; i++)
      {
         buffer[i] = GetFblEcuIdAddr()[i];
      }
   }
   return kFblOk;
#else
   return kFblFailed;
#endif
}

/***********************************************************************************************************************
 *  AppParseSba_GetInteger
 **********************************************************************************************************************/
/*! \brief      Convert given big-endian byte array to integer value
 *               (This is a copy of FblMemGetInteger inside fbl_mem.c in the Bootloader).
 *  \param[in]  count Number of relevant bytes
 *  \param[in]  buffer Pointer to input buffer
 *  \return     Integer value
 **********************************************************************************************************************/
static vuint32 AppParseSba_GetInteger( vuintx count, const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * const buffer )
{
   vuint32 output;
   vuintx idx;

   output = 0u;
   idx    = 0u;

   /* Loop relevant bytes */
   while (count > 0u)
   {
      /* Most significant byte first */
      output <<= 8u;
      /* Add current byte */
      output |= (vuint32)buffer[idx];

      idx++;
      count--;
   }

   return output;
}

/***********************************************************************************************************************
 *  AppParseSba_MemCompare
 **********************************************************************************************************************/
/*! \brief       Memcmp implementation for comparing content of pointers to vuint8.
 *               (This is a copy of FblHdrMemCompare inside fbl_hdr.c in the Bootloader).
 *  \param[in]   a pointer to array one for comparison
 *  \param[in]   b pointer to array two for comparison
 *  \param[in]   len length of data for comparison
 *  \return      kFblOk / kFblFailed
 **********************************************************************************************************************/
static tFblResult AppParseSba_MemCompare( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * a, const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * b, vuint16 len )
{
   tFblResult retVal = kFblOk;

   while ( len > 0 )
   {
      AppParseSba_PollingRoutine();

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
 *  AppParseSba_ValidateSignature
 **********************************************************************************************************************/
/*! \brief       Validate Signature inside SBA Signed Header
 *               (This is a copy of FblHdrValidateSignature inside fbl_hdr.c in the Bootloader).
 *  \param[in]   valRegStart Start of region to validate Signature on
 *  \param[in]   valRegLen Length of region to validate Signature on
 *  \param[in]   signatureStart Start of Signature (Validation region ends here)
 *  \param[in]   publicKey The public key to be used to validate signature
 *  \return      kFblOk / kFblFailed
 **********************************************************************************************************************/
static tFblResult AppParseSba_ValidateSignature( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * const valRegStart, SecM_LengthType valRegLen,
                                                 const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * const signatureStart,
                                                 const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3* const publicKey )
{
   SecM_SignatureParamType sigParam;
   tFblResult retVal = kFblFailed;
   SecM_AsymKeyType asymKey;

   asymKey.shared.data = publicKey;
   asymKey.shared.size = GetFblSigRsaModLen();
   /* Macro APP_PARSE_SBA_READ_SEC_DATA can be used to copy SecM_Production_RsaExp to Ram if required */
   asymKey.individual.data = APP_PARSE_SBA_READ_SEC_DATA(GetFblSigRsaExp());
   asymKey.individual.size = GetFblSigRsaExpLen();

   sigParam.key = &asymKey;

   sigParam.currentDataLength = V_NULL;
#if ( APP_SBA_CRYPTO_LIB_WORKSPACE == WORKSPACE_SHARE_WITH_FBL )
   sigParam.currentHash.length = 0;
   sigParam.currentHash.sigResultBuffer = 0;
#elif ( APP_SBA_CRYPTO_LIB_WORKSPACE == WORKSPACE_USE_OWN )   
   sigParam.currentHash.length = sizeof(applWorkSpace.buffer);
   sigParam.currentHash.sigResultBuffer = (SecM_ResultBufferType) &(applWorkSpace.buffer);
#else
   #error "Ilegal option."   
#endif
   sigParam.wdTriggerFct = (FL_WDTriggerFctType) AppParseSba_PollingRoutine;
   sigParam.sigState = kHashInit;
   sigParam.sigSourceBuffer = V_NULL;
   sigParam.sigByteCount = 0;

   if (SECM_OK == ApplSecVerifySignatureFct(&sigParam))
   {
      sigParam.sigState = kHashCompute;
      sigParam.sigSourceBuffer = valRegStart;
      sigParam.sigByteCount = valRegLen;

      if (SECM_OK == ApplSecVerifySignatureFct(&sigParam))
      {
         sigParam.sigState = kHashFinalize;
         sigParam.sigSourceBuffer = V_NULL;
         sigParam.sigByteCount = 0;

         if (SECM_OK == ApplSecVerifySignatureFct(&sigParam))
         {
            sigParam.sigState = kSigVerify;
            sigParam.sigSourceBuffer = signatureStart;
            sigParam.sigByteCount = HDR_SIGNATURE_LEN;

            if (SECM_OK == ApplSecVerifySignatureFct(&sigParam))
            {
               retVal = kFblOk;
            }
         }
      }
   }

   return retVal;
}


/***********************************************************************************************************************
 *  AppParseSba_ValidateSignerInfo
 **********************************************************************************************************************/
/*! \brief       Validate Signer Info
 *               (This is a copy of FblHdrValidateSignerInfo inside fbl_hdr.c in the Bootloader).
 *  \pre         This function is called after module signer info is loaded to signInfoAddr buffer location
 *  \param[in]   signInfoAddr address to signer Info information in download buffer
 *  \return      Address of public key stored in signer info.  In case of error, V_NULL is returned
 **********************************************************************************************************************/
static vuint8 const * AppParseSba_ValidateSignerInfo( const V_MEMRAM1 vuint8 V_MEMRAM2 * const signInfoAddr, vuint16 keyNbToCheck )
{
   tFblResult cmpResult;
   vuint8 const * retVal = V_NULL;

   /* Subject name = Stored? */
   cmpResult = AppParseSba_MemCompare( &signInfoAddr[SIGNINFO_SUBJNAME_IDX], GetFblSubjNameAddr(), HDR_SUBJNAME_LEN );
   if (kFblOk != cmpResult)
   {
      /* Do nothing */
   }
   /* KeyNBID needs to be read before Signature Verification */
   else if (     (kFblOk != AppParseSba_NVMReadKeyNBID(&storedKeyNBID))
              || (keyNbToCheck < storedKeyNBID) )
   {
      /* Do nothing */      
   }
   /* Validate signer Info using Root Public key from Boot Info Block and Root Signature from Signer Info downloaded */
   /* Macro APP_PARSE_SBA_READ_SEC_DATA can be used to copy SecM_Production_RsaExp to Ram if required */
   else if (kFblOk != AppParseSba_ValidateSignature(&signInfoAddr[0], SIGNINFO_ROOTSIGN_IDX,
                                              &signInfoAddr[SIGNINFO_ROOTSIGN_IDX], APP_PARSE_SBA_READ_SEC_DATA(GetFblSigRsaMod())))
   {
      /* Do nothing */      
   }
   else
   {
      retVal = &signInfoAddr[SIGNINFO_SIGNPKEY_IDX];
   }

   return retVal;
}

/***********************************************************************************************************************
 *   AppParseSba_CheckEcuNameAndId
 **********************************************************************************************************************/
/*! \brief       Check for Ecu name and Id
 *               (This is a copy of FblHdrCheckEcuNameAndId inside fbl_hdr.c in the Bootloader).
 *  \pre         This function is called only after module download header information was received.
 *  \param[in]   ecuNameAddr address to Ecu Name information in download buffer
 *  \param[in]   zeroCheck perform zero-check or not
 *  \return      kFblOk=Success; kFblFailed=Failure
 **********************************************************************************************************************/
static tFblResult  AppParseSba_CheckEcuNameAndId( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * const ecuNameAddr )
{
   tFblResult retVal = kFblFailed;
   vuint8 ecuIdVal[HDR_ECUID_LEN];

   if (kFblOk != AppParseSba_MemCompare(ecuNameAddr, GetFblEcuNameAddr(), HDR_ECUNAME_LEN ))
   {
     dummyPec = Err_EcuName;
   }
   else
   {
      if (kFblOk != retVal)
      {
         if (kFblOk == AppParseSba_NVMReadECUID(ecuIdVal))
         {
            if (kFblOk == AppParseSba_MemCompare(&ecuNameAddr[HDR_ECUNAME_LEN], &ecuIdVal[0], HDR_ECUID_LEN))
            {
               retVal = kFblOk;
            }
         }
      }

      if (kFblOk != retVal)
      {
         dummyPec = Err_EcuId;
      }
   }
   return retVal;
}


/***********************************************************************************************************************
 *  AppParseSba_ParseSba
 **********************************************************************************************************************/
/*! \brief       Initialize error status information.
 *  \param[in]   sbaTicket - Pointer to SBA-ticket, starting with module ID (data Type excluded)
 *  \return      The return value is in the format required to set the Most significant byte of the Boot Initialization
 *               Status (BIS).
 *
 *               The following values may be returned:
 *
 *               SBA "not valid" states:
 *               0                      - Sba module ID not present
 *               SBIMSB_ERR_ECUID       - ECUID incorrect
 *               SBIMSB_ERR_ECUNAME     - ECUNAME incorrect
 *               SBIMSB_ERR_CERT        - Signer Info incorrect
 *               SBIMSB_ERR_SIGNATURE   - Signature incorrect
 *
 *               Sba valid stat:
 *               SBIMSB_SBI_VALID       - Valid Sba ticket is present
 ***********************************************************************************************************************/
vuint8 AppParseSba_ParseSba( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * sbaTicket )
{
   /* Clear Sba result */
   vuint8 retVal = 0u;

   if (MODID_SBA != AppParseSba_FilterMid(AppParseSba_GetInteger(HDR_MID_LEN, &sbaTicket[S1_SBA_MID_IDX])))
   {
     /* Do nothing */
   }
   else if (kFblOk != AppParseSba_CheckEcuNameAndId(&sbaTicket[S1_SBA_ECUNAME_IDX]))
   {
      retVal = (vuint8)((Err_EcuId == dummyPec)? SBIMSB_ERR_ECUID : SBIMSB_ERR_ECUNAME);
   }
   else
   {
      vuint16 locKeyNbidToCheck = (vuint16) AppParseSba_GetInteger(HDR_NBID_LEN, &sbaTicket[S1_SBA_SIGNINFO_IDX+SIGNINFO_KEYNBID_IDX]);
      vuint8 const * signerPublicKeyAddr = AppParseSba_ValidateSignerInfo(&sbaTicket[S1_SBA_SIGNINFO_IDX], locKeyNbidToCheck);
      if (V_NULL == signerPublicKeyAddr)
      {
         retVal = SBIMSB_ERR_CERT;
      }
      else
      {
         if (kFblOk != AppParseSba_ValidateSignature(&sbaTicket[S1_SBA_MID_IDX], S1_SBA_SIGNATURE_IDX-S1_SBA_MID_IDX, &sbaTicket[S1_SBA_SIGNATURE_IDX], signerPublicKeyAddr))
         {
             retVal = SBIMSB_ERR_SIGNATURE;
         }
         else
         {
            retVal = SBIMSB_SBI_VALID;
         }
      }
   }

   return retVal;
}

/***********************************************************************************************************************
 *  END OF FILE: appParseSba.C
 **********************************************************************************************************************/
