/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        Implementation of the verification component of the HIS security module - Signature verification
 *
 *  \description  Offers signature/checksum verification interface
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
 *  Joern Herwig                  JHg           Vector Informatik GmbH
 *  Alexander Starke              ASe           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  01.00.00   2013-06-20  JHg     ESCAN00068213    Initial release based on FblSecMod_Vector 02.02.00
 *  02.00.00   2013-11-08  JHg     ESCAN00071179    Major refactoring
 *                                                   Moved to separate sub-package
 *                                                   Renamed file name (was SecMVer.c)
 *                                                   Merged with SecM_VerSig.c
 *                                                   Split into source and library parts
 *                                                   Added extended verification API (externally provided key and workspace)
 *                                                   Unified handling of Verification and VerifySignature API
 *  02.00.01   2013-12-19  JHg     ESCAN00072260    Encapsulate prototype of SecM_UpdateSegmentAddress
 *                         JHg     ESCAN00072561    Corrected parameter used in computation only use-case
 *  02.00.02   2014-01-20  JHg     ESCAN00073088    Resolved compiler warnings
 *  02.01.00   2014-01-28  ASe     ESCAN00073047    Added support for word-addressed platforms
 *  02.02.00   2014-02-10  JHg     ESCAN00073560    No changes
 *                         JHg     ESCAN00073355    Resolved compiler warnings
 *  02.02.01   2014-05-28  JHg     ESCAN00074792    Corrected memory qualifiers for configuration list parameters
 *                         JHg     ESCAN00073918    No changes
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

/* Security module configuration settings */
#include "Sec_Inc.h"

/* Global definitions for security module */
#include "Sec_Types.h"

/* Verification interface */
#include "Sec_Verification.h"

# include "Sec_Workspace.h"

/**********************************************************************************************************************
 *  VERSION
 *********************************************************************************************************************/

#if ( SYSSERVICE_SECMODHIS_VERIFICATION_VERSION != 0x0202u ) || \
    ( SYSSERVICE_SECMODHIS_VERIFICATION_RELEASE_VERSION != 0x01u )
# error "Error in SEC_VERIFICATION.C: Source and header file are inconsistent!"
#endif

/**********************************************************************************************************************
 *  DEFINES
 *********************************************************************************************************************/

/* PRQA S 3453 TAG_SecVerification_3453_1 */ /* MD_CBD_19.7 */

/** Byte length of address and length information included in checksum calculation */
#define SEC_ADDRESS_LENGTH_INPUT_SIZE    4u

/* Configuration of security classes */
/* Security class DDD (checksum) */
# if ( SEC_CHECKSUM_TYPE == SEC_CHECKSUM_TYPE_HASH )
#  if ( SEC_HASH_ALGORITHM == SEC_SHA1 )
/** Verify signature compatible function (class DDD) */
#   define SEC_VERIFY_CLASS_DDD_FUNCTION        SecM_VerifyHashSha1
/** Workspace pointer (class DDD)*/
#   define SEC_VERIFY_CLASS_DDD_WORKSPACE       secWorkSpacePtrHashSha1
/** Workspace size (class DDD)*/
#   define SEC_VERIFY_CLASS_DDD_WORKSPACE_SIZE  secWorkSpaceSizeHashSha1
#  elif ( SEC_HASH_ALGORITHM == SEC_RIPEMD160 )
#   define SEC_VERIFY_CLASS_DDD_FUNCTION        SecM_VerifyHashRmd160
#   define SEC_VERIFY_CLASS_DDD_WORKSPACE       secWorkSpacePtrHashRmd160
#   define SEC_VERIFY_CLASS_DDD_WORKSPACE_SIZE  secWorkSpaceSizeHashRmd160
#  elif ( SEC_HASH_ALGORITHM == SEC_SHA256 )
#   define SEC_VERIFY_CLASS_DDD_FUNCTION        SecM_VerifyHashSha256
#   define SEC_VERIFY_CLASS_DDD_WORKSPACE       secWorkSpacePtrHashSha256
#   define SEC_VERIFY_CLASS_DDD_WORKSPACE_SIZE  secWorkSpaceSizeHashSha256
#  endif /* SEC_HASH_ALGORITHM */
# else /* SEC_CHECKSUM_TYPE == SEC_CHECKSUM_TYPE_CRC */
#  define SEC_VERIFY_CLASS_DDD_FUNCTION         SecM_VerifyChecksumCrc
#  define SEC_VERIFY_CLASS_DDD_WORKSPACE        &crcParam
#  define SEC_VERIFY_CLASS_DDD_WORKSPACE_SIZE   sizeof(crcParam)
# endif /* SEC_CHECKSUM_TYPE */


/* Security class CCC (signature) */
# if ( SEC_HASH_ALGORITHM == SEC_SHA1 )
/** Verify signature compatible function (class CCC) */
#  define SEC_VERIFY_CLASS_CCC_FUNCTION         SecM_VerifySigSha1
/** Workspace type (class CCC)*/
#  define SEC_VERIFY_CLASS_CCC_WORKSPACE_TYPE   SecM_BasicWorkspaceSigSha1Type
/** Workspace pointer (class CCC)*/
#  define SEC_VERIFY_CLASS_CCC_WORKSPACE        secWorkSpacePtrSigSha1
/** Workspace size (class CCC)*/
#  define SEC_VERIFY_CLASS_CCC_WORKSPACE_SIZE   secWorkSpaceSizeSigSha1
# elif ( SEC_HASH_ALGORITHM == SEC_RIPEMD160 )
#  define SEC_VERIFY_CLASS_CCC_FUNCTION         SecM_VerifySigRmd160
#  define SEC_VERIFY_CLASS_CCC_WORKSPACE_TYPE   SecM_BasicWorkspaceSigRmd160Type
#  define SEC_VERIFY_CLASS_CCC_WORKSPACE        secWorkSpacePtrSigRmd160
#  define SEC_VERIFY_CLASS_CCC_WORKSPACE_SIZE   secWorkSpaceSizeSigRmd160
# elif ( SEC_HASH_ALGORITHM == SEC_SHA256 )
#  define SEC_VERIFY_CLASS_CCC_FUNCTION         SecM_VerifySigSha256
#  define SEC_VERIFY_CLASS_CCC_WORKSPACE_TYPE   SecM_BasicWorkspaceSigSha256Type
#  define SEC_VERIFY_CLASS_CCC_WORKSPACE        secWorkSpacePtrSigSha256
#  define SEC_VERIFY_CLASS_CCC_WORKSPACE_SIZE   secWorkSpaceSizeSigSha256
# endif /* SEC_HASH_ALGORITHM */

/** Security class used for SecM_VerifySignature interface */
#if   ( SEC_SECURITY_CLASS_VERIFY == SEC_CLASS_DDD )
# define SEC_VERIFY_FUNCTION                    SecM_VerifyClassDDD
#elif ( SEC_SECURITY_CLASS_VERIFY == SEC_CLASS_C )
# define SEC_VERIFY_FUNCTION                    SecM_VerifyClassC
#elif ( SEC_SECURITY_CLASS_VERIFY == SEC_CLASS_CCC )
# define SEC_VERIFY_FUNCTION                    SecM_VerifyClassCCC
#elif ( SEC_SECURITY_CLASS_VERIFY == SEC_CLASS_VENDOR )
# define SEC_VERIFY_FUNCTION                    SecM_VerifyClassVendor
#endif /* SEC_SECURITY_CLASS_VERIFY */

/** Security class used for SecM_Verification interface */
#if   ( SEC_SECURITY_CLASS_VERIFICATION == SEC_CLASS_DDD )
# define SEC_VERIFICATION_FUNCTION              SecM_VerificationClassDDD
#elif ( SEC_SECURITY_CLASS_VERIFICATION == SEC_CLASS_C )
# define SEC_VERIFICATION_FUNCTION              SecM_VerificationClassC
#elif ( SEC_SECURITY_CLASS_VERIFICATION == SEC_CLASS_CCC )
# define SEC_VERIFICATION_FUNCTION              SecM_VerificationClassCCC
#elif ( SEC_SECURITY_CLASS_VERIFICATION == SEC_CLASS_VENDOR )
# define SEC_VERIFICATION_FUNCTION              SecM_VerificationClassVendor
#endif /* SEC_SECURITY_CLASS_VERIFY */

/* Configuration of addons of verification API */
# define SEC_OPERATION_SIZE_CRC_TOTAL           0u
# define SEC_OPERATION_SIZE_CRC_WRITTEN         0u
#if defined( SEC_ENABLE_VENDOR_CHECKSUM )
# define SEC_OPERATION_SIZE_VENDOR_CHECKSUM     1u
#else
# define SEC_OPERATION_SIZE_VENDOR_CHECKSUM     0u
#endif /* SEC_ENABLE_CRC_WRITTEN */
/** Total size of operation table */
#define SEC_OPERATION_SIZE                      (SEC_OPERATION_SIZE_CRC_TOTAL + SEC_OPERATION_SIZE_CRC_WRITTEN + SEC_OPERATION_SIZE_VENDOR_CHECKSUM + 1u)

/** Maximum number of operation primitives which can be passed to SecM_VerificationWrapper */
#if defined( SEC_VER_MAX_CONFIG_COUNT )
#else
# define SEC_VER_MAX_CONFIG_COUNT               SEC_OPERATION_SIZE
#endif /* SEC_VER_MAX_CONFIG_COUNT */

/** Number of elements in array */
#define SEC_VER_ARRAY_SIZE(arr)                 (sizeof(arr) / sizeof((arr)[0]))

/* Handling of resulting length for memory reads */

/** Report no error in case no data was read at all */
#define SEC_READ_COUNT_RESULT_IGNORE            SECM_VER_OK
/** Report error in case of mismatch between read count and requested length */
#define SEC_READ_COUNT_RESULT_ASSERT            SECM_VER_ERROR

/** Read count can differ from requested length */
#define SEC_READ_COUNT_MASK_IGNORE              0x00u
/** Read count has to match requested length */
/* Remark: Conversion of negative one to unsigned type with all bits set is guaranteed by C standard (6.3.1.3/2) */
#define SEC_READ_COUNT_MASK_ASSERT              (SecM_SizeType)(-1)

/** Depending on the underlying memory (byte-/word-addressed) the behavior of the readMemory() function deviates:
 *  - On byte-addressed platforms the function usually reads exactly the requested number of bytes
 *  - On word-addressed platforms twice the number of requested bytes is read */
# define SEC_MEMORY_READ_ACCESS_WIDTH           1u

/* PRQA L:TAG_SecVerification_3453_1 */

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/** Context of SecM_Verification operation */
typedef struct
{
   FL_ReadMemoryFctType       readMemory;       /**< Pointer to memory read function */
   SecM_AddrType              verifyAddress;    /**< Current verification address */
   SecM_SizeType              remainingBytes;   /**< Number of remaining bytes in segment */
   SecM_SignatureParamType    sigParam;         /**< Pointer to global signature parameter */
} SecM_VerifyContextType;

/** List of verification primitive configurations */
typedef struct
{
   SecM_VerifyConfigType   pPrimitives[SEC_VER_MAX_CONFIG_COUNT]; /**< List of primitive operation and context pairs */
   SecM_ByteFastType       count;                                 /**< Number of list entries */
} SecM_VerifyConfigPairType;

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#if defined( SEC_ENABLE_VERIFICATION_ADDRESS_LENGTH )
static void SecM_SetInteger( SecM_ByteFastType count, SecM_WordType input, SecM_RamDataType buffer );
#endif /* (SEC_ENABLE_VERIFY_CHECKSUM_CRC && SEC_ENABLE_COMPUTATION_ONLY) || SEC_ENABLE_VERIFICATION_ADDRESS_LENGTH */


#if defined( SEC_ENABLE_VERIFICATION_RANGE_CHECK )
static SecM_StatusType SecM_CheckRangeOverflow( SecM_AddrType address, SecM_SizeType length );
#endif /* SEC_ENABLE_VERIFICATION_RANGE_CHECK */

static SecM_StatusType SecM_VerifyWrapper( V_MEMRAM1 SecM_SignatureParamType V_MEMRAM2 V_MEMRAM3 * pVerifyParam,
   V_MEMRAM1 SecM_VerifyConfigPairType V_MEMRAM2 V_MEMRAM3 * pCfgList );
static SecM_StatusType SecM_UpdateSegment( V_MEMRAM1 SecM_VerifyContextType V_MEMRAM2 V_MEMRAM3 * pContext,
   V_MEMRAM1 SecM_VerifyConfigPairType V_MEMRAM2 V_MEMRAM3 * pCfgList );

#if defined( SEC_ENABLE_VERIFICATION_ADDRESS_LENGTH )
static SecM_StatusType SecM_UpdateSegmentAddress( V_MEMRAM1 SecM_VerifyContextType V_MEMRAM2 V_MEMRAM3 * pContext,
   V_MEMRAM1 SecM_VerifyConfigPairType V_MEMRAM2 V_MEMRAM3 * pCfgList,
   const V_MEMRAM1 FL_SegmentInfoType V_MEMRAM2 V_MEMRAM3 * pSegmentInfo );
#endif /* SEC_ENABLE_VERIFICATION_ADDRESS_LENGTH */

static void SecM_PopulateCfgList( V_MEMRAM1 SecM_VerifyConfigListType V_MEMRAM2 V_MEMRAM3 * pSourceList,
   V_MEMRAM1 SecM_VerifyConfigPairType V_MEMRAM2 V_MEMRAM3 * pTargetList, SecM_ByteType mask );

#if defined( SEC_ENABLE_VERIFICATION_WRAPPER )
#else
static SecM_StatusType SecM_VerificationWrapper ( V_MEMRAM1 SecM_VerifyParamType V_MEMRAM2 V_MEMRAM3 * pVerifyParam,
   V_MEMRAM1 SecM_VerifyConfigListType V_MEMRAM2 V_MEMRAM3 * pCfgList );
#endif /* SEC_ENABLE_VERIFICATION_WRAPPER */

static SecM_StatusType SecM_VerificationBase ( V_MEMRAM1 SecM_VerifyParamType V_MEMRAM2 V_MEMRAM3 * pVerifyParam,
   V_MEMRAM1 SecM_VerifyConfigListType V_MEMRAM2 V_MEMRAM3 * pCfgList );

/**********************************************************************************************************************
 *  LOCAL DATA
 *********************************************************************************************************************/

#if defined( SEC_ENABLE_SECURITY_CLASS_DDD )

/** Verification primitive configuration for security class DDD */
V_MEMROM0 static V_MEMROM1 SecM_VerifyOperationType V_MEMROM2 verifyConfigClassDDD[] = /* PRQA S 3218 */ /* MD_SecVerification_3218 */
{
   {
      /* Use configured primitive */
      SecM_VerifyClassDDD,
      /* Size and offset of checksum/signature */
      SEC_VERIFY_CLASS_DDD_VERIFY_SIZE,
      SEC_VERIFY_CLASS_DDD_VERIFY_OFFSET,
      /* Contributes to verification result */
      SEC_UPDATE_OPERATION_VERIFICATION
   }
# if defined( SEC_ENABLE_VENDOR_CHECKSUM )
  ,{
      /* Use vendor specific primitive (callback) */
      SecM_VerifyClassVendor,
      SEC_SIZE_CHECKSUM_VENDOR,
      SEC_VERIFY_CLASS_DDD_CHECKSUM_OFFSET,
      /* Contributes to verification result */
      SEC_UPDATE_OPERATION_VERIFICATION,
   }
# endif /* SEC_ENABLE_CRC_TOTAL */
};
#endif /* SEC_ENABLE_SECURITY_CLASS_DDD */


#if defined( SEC_ENABLE_SECURITY_CLASS_CCC )
/** Verification primitive configuration for security class CCC */
V_MEMROM0 static V_MEMROM1 SecM_VerifyOperationType V_MEMROM2 verifyConfigClassCCC[] = /* PRQA S 3218 */ /* MD_SecVerification_3218 */
{
   {
      /* Use configured primitive */
      SecM_VerifyClassCCC,
      /* Size and offset of checksum/signature */
      SEC_VERIFY_CLASS_CCC_VERIFY_SIZE,
      SEC_VERIFY_CLASS_CCC_VERIFY_OFFSET,
      /* Include address and length of segment, contributes to verification result */
      SEC_UPDATE_OPERATION_ADDRESS_LENGTH | SEC_UPDATE_OPERATION_VERIFICATION
   }
# if defined( SEC_ENABLE_VENDOR_CHECKSUM )
  ,{
      /* Use vendor specific primitive (callback) */
      SecM_VerifyClassVendor,
      SEC_SIZE_CHECKSUM_VENDOR,
      SEC_VERIFY_CLASS_CCC_CHECKSUM_OFFSET,
      /* Contributes to verification result */
      SEC_UPDATE_OPERATION_VERIFICATION,
   }
# endif /* SEC_ENABLE_CRC_TOTAL */
};
#endif /* SEC_ENABLE_SECURITY_CLASS_CCC */

#if defined( SEC_ENABLE_SECURITY_CLASS_VENDOR )
/** Verification primitive configuration for security class vendor */
V_MEMROM0 static V_MEMROM1 SecM_VerifyOperationType V_MEMROM2 verifyConfigClassVendor[] = /* PRQA S 3218 */ /* MD_SecVerification_3218 */
{
   {
      /* Use vendor specific primitive (callback) */
      SecM_VerifyClassVendor,
      /* Size and offset of checksum/signature */
      SEC_SIZE_CHECKSUM_VENDOR,
      SEC_VERIFY_CLASS_VENDOR_CHECKSUM_OFFSET,
      /* Contributes to verification result */
      SEC_UPDATE_OPERATION_VERIFICATION,
   }
};
#endif /* SEC_ENABLE_SECURITY_CLASS_VENDOR */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

#if defined( SEC_ENABLE_VERIFICATION_ADDRESS_LENGTH )
/***********************************************************************************************************************
 *  SecM_SetInteger
 **********************************************************************************************************************/
/*! \brief      Convert given integer value to big-endian byte array
 *  \param[in]  count Number of relevant bytes
 *  \param[in]  input Input value
 *  \param[out] buffer Pointer to output buffer
 **********************************************************************************************************************/
static void SecM_SetInteger( SecM_ByteFastType count, SecM_WordType input, SecM_RamDataType buffer )
{
   SecM_ByteFastType index;

   /* Loop relevant bytes */
   index = count;
   while (index > 0u)
   {
      index--;
      /* Store most significant byte first */
      buffer[index] = (SecM_ByteType)(input & 0xFFu);
      /* Shift in next byte */
      input >>= 8u;
   }
}
#endif /* (SEC_ENABLE_VERIFY_CHECKSUM_CRC && SEC_ENABLE_COMPUTATION_ONLY) || SEC_ENABLE_VERIFICATION_ADDRESS_LENGTH */


#if defined( SEC_ENABLE_VERIFICATION_RANGE_CHECK )
/***********************************************************************************************************************
 *  SecM_CheckRangeOverflow
 **********************************************************************************************************************/
/*! \brief      Check whether memory range defined by address and length create an address range overflow
 *  \param[in]  address Start address of memory range
 *  \param[in]  length Length of memory range
 *  \return     SECM_OK if no range overflow detected
 *              SECM_NOT_OK otherwise
 **********************************************************************************************************************/
static SecM_StatusType SecM_CheckRangeOverflow( SecM_AddrType address, SecM_SizeType length )
{
   SecM_StatusType result;

   result = SECM_OK;

   /* No range overflow possible with zero length */
   if (length > 0u)
   {
      /* Last address of memory range is not allowed to be greater than the maximum representable value
         Following expression mathematically equals: (address + (length - 1u)) > 0xFF...FFu
         Conversion of negative one to unsigned type with all bits set is guaranteed by C standard (6.3.1.3/2) */
      if (address > (((SecM_AddrType)(-1)) - (length - 1u))) /* PRQA S 0277 */ /* MD_SecVerification_0277 */
      {
         result = SECM_NOT_OK;
      }
   }

   return result;
}
#endif /* SEC_ENABLE_VERIFICATION_RANGE_CHECK */

/**********************************************************************************************************************
 *  SecM_VerifyWrapper
 *********************************************************************************************************************/
/*! \brief         Performs a verify operation for a given set of verification primitives
 *  \details       Interates over all primitives given in list, passing them the information from the verification
 *                 parameter.
 *                 During initialization operation (SEC_HASH_INIT) the following additional information is assigned
 *                 to the primary verification primitive:
 *                 - External key data (SEC_ENABLE_VERIFICATION_KEY_EXTERNAL)
 *                 - External workspace (SEC_ENABLE_WORKSPACE_EXTERNAL)
 *                 During verification operation (SEC_SIG_VERIFY) the following additional information is passed
 *                 to all verification primitives:
 *                 - Length of signature/checksum taken from primitive configuration instead of verification parameter
 *                 - Pointer to verification data moved by offset taken from primitive configuration
 *  \param[in]     pVerifyParam Pointer to parameter structure for signature verification
 *                   When SEC_ENABLE_WORKSPACE_INTERNAL is set member currentHash may contain reference to buffer
 *                   used as workspace for primary verification primitive
 *  \param[in]     pCfgList Pointer to list of verification primitives including contexts
 *  \return        SECM_VER_OK if verification operation successful
 *                 SECM_VER_ERROR if error occured during verification
 *                 SECM_VER_CRC if CRC verification failed
 *                 SECM_VER_SIG if HMAC/Signature verification failed
 *********************************************************************************************************************/
static SecM_StatusType SecM_VerifyWrapper( V_MEMRAM1 SecM_SignatureParamType V_MEMRAM2 V_MEMRAM3 * pVerifyParam,  /* PRQA S 3673 */ /* MD_SecVerification_3673 */
                                           V_MEMRAM1 SecM_VerifyConfigPairType V_MEMRAM2 V_MEMRAM3 * pCfgList )   /* PRQA S 3673 */ /* MD_SecVerification_3673 */
{
   SecM_StatusType            result;
   SecM_ByteFastType          index;
   SecM_ByteFastType          cfgCount;
   SecM_ByteType              action;
   SecM_LengthType            byteCount;
   SecM_LengthFastType        sourceOffset;
   V_MEMRAM1 SecM_VerifyConfigType V_MEMRAM2 V_MEMRAM3 *    pConfig;
   V_MEMROM1 SecM_VerifyOperationType V_MEMROM2 V_MEMROM3 * pPrimitive;
   V_MEMRAM1 SecM_SignatureParamType V_MEMRAM2 V_MEMRAM3 *  pParam;

   result = SECM_VER_OK;

   /* Action and byte count taken from verification parameter per default */
   action         = pVerifyParam->sigState;
   byteCount      = pVerifyParam->sigByteCount;
   /* Offset into source buffer defaults to zero: all primitives process the full input data */
   sourceOffset   = 0u;

#if defined( SEC_ENABLE_VERIFICATION_DATA_LENGTH )
   /* Update member currentDataLength of global verification parameter */
   SecM_UpdateDataLength(pVerifyParam);
#endif /* SEC_ENABLE_VERIFICATION_DATA_LENGTH */

#if defined( SEC_ENABLE_VERIFICATION_KEY_EXTERNAL ) || \
    defined( SEC_ENABLE_WORKSPACE_EXTERNAL )
   /* Perform some special actions during intialization */
   if (SEC_HASH_INIT == action)
   {
      /* Get pointer to primary verification primitive
         Located in first entry of */
      pConfig  = &pCfgList->pPrimitives[0];
      pParam   = pConfig->pContext;

# if defined( SEC_ENABLE_VERIFICATION_KEY_EXTERNAL )
      /* Pass external key to primary verification primitive
         This may be a null pointer (SEC_VERIFY_KEY_NULL) */
      pParam->key = pVerifyParam->key;
# endif /* SEC_ENABLE_VERIFICATION_KEY_EXTERNAL */

# if defined( SEC_ENABLE_WORKSPACE_EXTERNAL )
      /* External workspace given? */
      if (SEC_DEFAULT_WORKSPACE_SIZE != pVerifyParam->currentHash.length)
      {
         /* Pass external workspace to primary verification primitive */
         pParam->currentHash = pVerifyParam->currentHash;
      }
# endif /* SEC_ENABLE_WORKSPACE_EXTERNAL */
   }
#endif /* SEC_ENABLE_VERIFICATION_KEY_EXTERNAL || SEC_ENABLE_WORKSPACE_EXTERNAL */

   /* Loop all verification primitives */
   cfgCount = pCfgList->count;

   for (index = 0u; index < cfgCount; index++)
   {
      /* Get pointer to verification primitive for easier access */
      pConfig     = &pCfgList->pPrimitives[index];
      pPrimitive  = pConfig->pOperation;
      pParam      = pConfig->pContext;

      /* Special handling for finalization operation */
      if (SEC_SIG_VERIFY == action)
      {
         /* Length of signature/checksum and offset into verification data taken from primitve configuration */
         sourceOffset   = pPrimitive->offset;
         byteCount      = pPrimitive->length;
      }

      /* Pass settings to dedicated parameters */
      pParam->sigState        = action;
      pParam->sigSourceBuffer = &pVerifyParam->sigSourceBuffer[sourceOffset];
      pParam->sigByteCount    = byteCount;
      pParam->wdTriggerFct    = pVerifyParam->wdTriggerFct;

      /* Perform operation for current primitive */
      result = pPrimitive->pFunction(pParam);

      if (SECM_VER_OK != result)
      {
         /* Operation failed, abort further processing */
         break;
      }
   }

   return result;
}

/**********************************************************************************************************************
 *  SecM_UpdateSegment
 *********************************************************************************************************************/
/*! \brief         Read memory contents of one segment and pass to update operation of given primitives
 *  \param[in,out] pContext Pointer to verification context containing segment information
 *  \param[in]     pCfgList Pointer to list of verification primitives
 *  \return        SECM_VER_OK if update operation successful
 *                 SECM_VER_ERROR if error occured during update
 *********************************************************************************************************************/
static SecM_StatusType SecM_UpdateSegment( V_MEMRAM1 SecM_VerifyContextType V_MEMRAM2 V_MEMRAM3 * pContext,
   V_MEMRAM1 SecM_VerifyConfigPairType V_MEMRAM2 V_MEMRAM3 * pCfgList )
{
   SecM_StatusType   result;
   SecM_AddrType     currentAddress;
   SecM_SizeType     remainder;
   SecM_SizeType     verifyCount;
   SecM_SizeType     readCount;
   SecM_ByteType     verifyBuffer[SEC_VERIFY_BYTES];
   V_MEMRAM1 SecM_SignatureParamType V_MEMRAM2 V_MEMRAM3 * pVerifyParam;

   result = SECM_VER_OK;

   if (pCfgList->count > 0u)
   {
      /* Move context parameters to local variables for easier access */
      currentAddress = pContext->verifyAddress;
      remainder      = pContext->remainingBytes;


#if defined( SEC_ENABLE_VERIFICATION_RANGE_CHECK )
      /* Check for range overflow */
      if (SECM_OK != SecM_CheckRangeOverflow(currentAddress, remainder))
      {
         /* Invalid memory range */
         result = SECM_VER_ERROR;
      }
      else
#endif /* SEC_ENABLE_VERIFICATION_RANGE_CHECK */
      {
         pVerifyParam = &pContext->sigParam;

         pVerifyParam->sigSourceBuffer  = verifyBuffer;      /* PRQA S 3225 */ /* MD_SecVerification_3225 */
         pVerifyParam->sigState         = SEC_HASH_COMPUTE;

         while (remainder > 0u)
         {
            /* Serve watchdog every loop cycle */
            SEC_WATCHDOG_TRIGGER(pVerifyParam->wdTriggerFct);

            /* Number of bytes to handle in this loop */
            verifyCount = SEC_VERIFY_BYTES / SEC_MEMORY_READ_ACCESS_WIDTH;
            if (remainder < (SEC_VERIFY_BYTES / SEC_MEMORY_READ_ACCESS_WIDTH))
            {
               /* Limit to remainder */
               verifyCount = remainder;
            }

            /* Copy data from memory to RAM buffer */
            readCount = pContext->readMemory(currentAddress, verifyBuffer, verifyCount);

#if defined( SEC_ENABLE_VERIFICATION_ASSERT_READ_COUNT )
            /* Verification only operates on inter-segment data
               So read count has to match the requested length, otherwise something went wrong */
            if (readCount == (SEC_MEMORY_READ_ACCESS_WIDTH * verifyCount))
#else
            /* Update operation only necessary in case any data was read */
            if (readCount > 0u)
#endif /* SEC_ENABLE_VERIFICATION_ASSERT_READ_COUNT */
            {
               /* Pass read data to all verification primitives */
               pVerifyParam->sigByteCount = (SecM_LengthType)readCount;
               result = SecM_VerifyWrapper(pVerifyParam, pCfgList);
            }
            else
            {
#if defined( SEC_ENABLE_VERIFICATION_ASSERT_READ_COUNT )
               /* Read operation did not return expeced length, report error */
               result = SECM_VER_ERROR;
#else
               /* Continue operation */
#endif /* SEC_ENABLE_VERIFICATION_ASSERT_READ_COUNT */
            }

            /* Update address information */
            currentAddress += verifyCount;
            remainder      -= verifyCount;   /* PRQA S 3382 */ /* MD_SecVerification_3382 */

            if (SECM_VER_OK != result)
            {
               /* Operation failed, abort further processing */
               break;
            }
         }
      }
   }
   else
   {
      /* No matching configurations given (e.g. CRC total calculation disabled at run-time)
         Continue without performing any actual operation */
   }

   return result;
}

#if defined( SEC_ENABLE_VERIFICATION_ADDRESS_LENGTH )
/**********************************************************************************************************************
 *  SecM_UpdateSegmentAddress
 *********************************************************************************************************************/
/*! \brief         Pass serialized address and length information of one segment to update operation of given primitives
 *  \param[in,out] pContext Pointer to verification context containing segment information
 *  \param[in]     pCfgList Pointer to list of verification primitives
 *  \param[in]     pSegmentInfo Pointer to segment information (address and length)
 *  \return        SECM_VER_OK if update operation successful
 *                 SECM_VER_ERROR if error occured during update
 *********************************************************************************************************************/
static SecM_StatusType SecM_UpdateSegmentAddress( V_MEMRAM1 SecM_VerifyContextType V_MEMRAM2 V_MEMRAM3 * pContext,
   V_MEMRAM1 SecM_VerifyConfigPairType V_MEMRAM2 V_MEMRAM3 * pCfgList,
   const V_MEMRAM1 FL_SegmentInfoType V_MEMRAM2 V_MEMRAM3 * pSegmentInfo )
{
   SecM_StatusType   result;
   SecM_ByteType     addressBuffer[2 * SEC_ADDRESS_LENGTH_INPUT_SIZE];
   V_MEMRAM1 SecM_SignatureParamType V_MEMRAM2 V_MEMRAM3 * pVerifyParam;

   result = SECM_VER_OK;

   if (pCfgList->count > 0u)
   {
      /* Convert address and length of current segment to big-endian byte array */
      SecM_SetInteger(SEC_ADDRESS_LENGTH_INPUT_SIZE, pSegmentInfo->transferredAddress, &addressBuffer[0]);
      SecM_SetInteger(SEC_ADDRESS_LENGTH_INPUT_SIZE, pSegmentInfo->length, &addressBuffer[SEC_ADDRESS_LENGTH_INPUT_SIZE]);

      pVerifyParam = &pContext->sigParam;

      pVerifyParam->sigSourceBuffer = addressBuffer;                     /* PRQA S 3225 */ /* MD_SecVerification_3225 */
      pVerifyParam->sigByteCount    = 2 * SEC_ADDRESS_LENGTH_INPUT_SIZE;
      pVerifyParam->sigState        = SEC_HASH_COMPUTE;

      /* Update verification using address and length information */
      result = SecM_VerifyWrapper(pVerifyParam, pCfgList);
   }
   else
   {
      /* No matching configurations given, continue without performing any actual operation */
   }

   return result;
}
#endif /* SEC_ENABLE_VERIFICATION_ADDRESS_LENGTH */

/**********************************************************************************************************************
 *  SecM_PopulateCfgList
 *********************************************************************************************************************/
/*! \brief         Populate configuration list with all verification primitives matching given criteria
 *  \param[in]     pSourceList Pointer to list containing all verification primitives
 *  \param[out]    pTargetList Pointer to list filled with verification primitive pairs matching criteria
 *  \param[in]     mask Criteria mask (0x00u if all primitives shall be included)
 *********************************************************************************************************************/
static void SecM_PopulateCfgList( V_MEMRAM1 SecM_VerifyConfigListType V_MEMRAM2 V_MEMRAM3 * pSourceList,
   V_MEMRAM1 SecM_VerifyConfigPairType V_MEMRAM2 V_MEMRAM3 * pTargetList, SecM_ByteType mask )
{
   SecM_ByteFastType inIndex;
   SecM_ByteFastType outIndex;
   V_MEMROM1 SecM_VerifyOperationType V_MEMROM2 V_MEMROM3 * pSourceOperation;
   V_MEMRAM1 SecM_SignatureParamType V_MEMRAM2 V_MEMRAM3 *  pSourceParam;
   V_MEMRAM1 SecM_VerifyConfigType V_MEMRAM2 V_MEMRAM3 *    pTarget;

   /* Index for output list */
   outIndex = 0u;
   /* Loop all input primitives */
   for (inIndex = 0u; inIndex < pSourceList->count; inIndex++)
   {
      /* Take matching operation and context from the individual lists */
      pSourceOperation  = &pSourceList->pPrimitives.pOperation[inIndex];
      pSourceParam      = &pSourceList->pPrimitives.pContext[inIndex];

      /* Check whether primitive matches criteria
         In case mask == 0x00u all primitives will be included */
      if (mask == (pSourceOperation->mask & mask))
      {
         /* Pair associated operation and context pointers to configuration */
         pTarget = &pTargetList->pPrimitives[outIndex];

         pTarget->pOperation  = pSourceOperation;
         pTarget->pContext    = pSourceParam;

         outIndex++;
      }
   }

   /* Number of verification primitives matching criteria */
   pTargetList->count = outIndex;
}

/**********************************************************************************************************************
 *  SecM_VerificationWrapper
 *********************************************************************************************************************/
/*! \brief         Performs a verification operation for a given set of verification primitives
 *  \details       Loops all segments present in the verification parameter and passes the information and data to the
 *                 verification primitives given in list.
 *                 Depending on the criteria given in the mask member of the primitive the following operations are
 *                 carried out:
 *                 - Initialization and update with intra-segment data (SEC_UPDATE_OPERATION_DEFAULT - all primitives)
 *                 - Update with inter-segment data (SEC_UPDATE_OPERATION_INTER_SEGMENT)
 *                 - Update with address and length of segments (SEC_UPDATE_OPERATION_ADDRESS_LENGTH)
 *                 - Verification against given signature/checksum (SEC_UPDATE_OPERATION_VERIFICATION)
 *  \pre           Segments in member segmentList of pVerifyParam have to be sorted in ascending order
 *  \param[in]     pVerifyParam Pointer to parameter structure for signature verification
 *  \param[in]     pCfgList Pointer to list of verification primitives
 *  \return        SECM_VER_OK if signature verification successful
 *                 SECM_VER_ERROR if error occured during verification
 *                 SECM_VER_CRC if CRC verification failed
 *                 SECM_VER_SIG if HMAC/Signature verification failed
 *********************************************************************************************************************/
#if defined( SEC_ENABLE_VERIFICATION_WRAPPER )
SecM_StatusType SecM_VerificationWrapper( V_MEMRAM1 SecM_VerifyParamType V_MEMRAM2 V_MEMRAM3 * pVerifyParam,
   V_MEMRAM1 SecM_VerifyConfigListType V_MEMRAM2 V_MEMRAM3 * pCfgList )
#else
static SecM_StatusType SecM_VerificationWrapper( V_MEMRAM1 SecM_VerifyParamType V_MEMRAM2 V_MEMRAM3 * pVerifyParam,
   V_MEMRAM1 SecM_VerifyConfigListType V_MEMRAM2 V_MEMRAM3 * pCfgList )
#endif /* SEC_ENABLE_VERIFICATION_WRAPPER */
{
   SecM_StatusType         result;
   /* Context of verification passed to helper function */
   SecM_VerifyContextType  context;
   SecM_ByteFastType       index;
   /* Local pointer to segments */
   V_MEMRAM1 FL_SegmentListType V_MEMRAM2 V_MEMRAM3 * pSegmentList;
   /* Pointer to current segment */
   V_MEMRAM1 FL_SegmentInfoType V_MEMRAM2 V_MEMRAM3 * pSegmentInfo;
   /* Pointers to all primitives */
   SecM_VerifyConfigPairType   cfgListAll;
   /* Pointers to primitives executing final verification */
   SecM_VerifyConfigPairType   cfgListVerify;
#if defined( SEC_ENABLE_VERIFICATION_ADDRESS_LENGTH )
   /* Pointers to primitives updated with address and length information */
   SecM_VerifyConfigPairType   cfgListAddr;
#endif /* SEC_ENABLE_VERIFICATION_ADDRESS_LENGTH */

#if defined( SEC_ENABLE_VERIFICATION_WRAPPER )
   result = SECM_VER_ERROR;

   /* Verify list contains any primitives and does not exceed maximum configured count */
   if ((pCfgList->count > 0u) && (pCfgList->count <= SEC_VER_MAX_CONFIG_COUNT))
#endif /* SEC_ENABLE_VERIFICATION_WRAPPER */
   {
      /* Convert list of all primitives into matching format (structure of arrays -> array of structures) */
      SecM_PopulateCfgList(pCfgList, &cfgListAll,     SEC_UPDATE_OPERATION_DEFAULT);

      /* Get all primitives contributing to final verification */
      SecM_PopulateCfgList(pCfgList, &cfgListVerify,  SEC_UPDATE_OPERATION_VERIFICATION);
#if defined( SEC_ENABLE_VERIFICATION_ADDRESS_LENGTH )
      /* Get all primitives updated with address and lenght information of segment */
      SecM_PopulateCfgList(pCfgList, &cfgListAddr,    SEC_UPDATE_OPERATION_ADDRESS_LENGTH);
#endif /* SEC_ENABLE_VERIFICATION_ADDRESS_LENGTH */

      /* Ease the access of segment data by local variable */
      /* According to HIS specification */
      pSegmentList = &pVerifyParam->segmentList;

      /* Setup verification parameter in context
         Uninitialized/unchanged values not relevant for initialization */
      context.sigParam.sigState           = SEC_HASH_INIT;
      context.sigParam.wdTriggerFct       = pVerifyParam->wdTriggerFct;
      context.readMemory                  = pVerifyParam->readMemory;
#if defined( SEC_ENABLE_VERIFICATION_DATA_LENGTH )
      context.sigParam.currentDataLength  = SEC_DATA_LENGTH_NULL;
#endif /* SEC_ENABLE_VERIFICATION_DATA_LENGTH */
#if defined( SEC_ENABLE_VERIFICATION_KEY_EXTERNAL )
      /* Pass external key to verify wrapper */
      context.sigParam.key = pVerifyParam->key;
#endif /* SEC_ENABLE_VERIFICATION_KEY_EXTERNAL */
#if defined( SEC_ENABLE_WORKSPACE_EXTERNAL )
      /* Pass external workspace to verify wrapper */
      context.sigParam.currentHash.sigResultBuffer  = (SecM_ResultBufferType)pVerifyParam->workspace.data; /* PRQA S 0306 */ /* MD_SecVerification_0306 */
      context.sigParam.currentHash.length           = pVerifyParam->workspace.size;
#endif /* SEC_ENABLE_WORKSPACE_EXTERNAL */

      /* Initialize all verification primitives */
      result = SecM_VerifyWrapper(&context.sigParam, &cfgListAll);

      if (SECM_VER_OK == result)
      {
         /* CRC total option disabled
            Use optimized implementation which only handles inter-segment data
            Loop all segments */
         for (index = 0u; index < pSegmentList->nrOfSegments; index++)
         {
            /* Get current segment information */
            pSegmentInfo = &pSegmentList->segmentInfo[index];

            /* Pass segment information in context */
            context.verifyAddress  = pSegmentInfo->targetAddress;
            context.remainingBytes = pSegmentInfo->length;

# if defined( SEC_ENABLE_VERIFICATION_ADDRESS_LENGTH )
            /* Update verification with address and length of segment */
            result = SecM_UpdateSegmentAddress(&context, &cfgListAddr, pSegmentInfo);
# endif /* SEC_ENABLE_VERIFICATION_ADDRESS_LENGTH */

            if (SECM_VER_OK == result)
            {
               /* Update verification with inter-segment data */
               result = SecM_UpdateSegment(&context, &cfgListAll);
            }

            /* This is not the else of the if above */
            if (SECM_VER_OK != result)
            {
               /* Operation failed, abort processing */
               break;
            }
         }
      }

      if (SECM_VER_OK == result)
      {
         /* Finalize calculation */
         context.sigParam.sigState        = SEC_HASH_FINALIZE;

         result = SecM_VerifyWrapper(&context.sigParam, &cfgListAll);

         if (SECM_VER_OK == result)
         {
            /* Finalize verification */
            context.sigParam.sigSourceBuffer = pVerifyParam->verificationData;
            context.sigParam.sigState        = SEC_SIG_VERIFY;

            result = SecM_VerifyWrapper(&context.sigParam, &cfgListVerify);
         }
      }
   }

   return result;
}

/**********************************************************************************************************************
 *  SecM_VerificationBase
 *********************************************************************************************************************/
/*! \brief         Calculate and verify checksum or signature using passed primitive
 *  \details       Beside the passed base operation, auxiliary operations are included (if configured) in the
 *                 following order:
 *                 - CRC total
 *                 - Vendor specific checksum
 *                 - CRC written
 *                 - ...
 *                 Additional details see SecM_Verification
 *  \param[in]     pVerifyParam Pointer to parameter structure for signature verification
 *  \param[in]     pCfgList Pointer to list of verification primitives
 *  \return        SECM_VER_OK if signature verification successful
 *                 SECM_VER_ERROR if error occured during verification
 *                 SECM_VER_CRC if CRC verification failed
 *                 SECM_VER_SIG if HMAC/Signature verification failed
 *********************************************************************************************************************/
static SecM_StatusType SecM_VerificationBase( V_MEMRAM1 SecM_VerifyParamType V_MEMRAM2 V_MEMRAM3 * pVerifyParam,
   V_MEMRAM1 SecM_VerifyConfigListType V_MEMRAM2 V_MEMRAM3 * pCfgList )
{
   SecM_StatusType   result;
#if (defined( SEC_ENABLE_WORKSPACE_INTERNAL ) && defined( SEC_ENABLE_WORKSPACE_EXTERNAL )) || \
    defined( SEC_ENABLE_VERIFICATION_DATA_LENGTH ) 
   V_MEMRAM1 SecM_SignatureParamType V_MEMRAM2 V_MEMRAM3 * pVerifyContext;
#endif /* (SEC_ENABLE_WORKSPACE_INTERNAL && SEC_ENABLE_WORKSPACE_EXTERNAL) || SEC_ENABLE_VERIFICATION_DATA_LENGTH || SEC_ENABLE_CRC_TOTAL || SEC_ENABLE_CRC_WRITTEN */

#if (defined( SEC_ENABLE_WORKSPACE_INTERNAL ) && defined( SEC_ENABLE_WORKSPACE_EXTERNAL )) || \
    defined( SEC_ENABLE_VERIFICATION_DATA_LENGTH ) 
   pVerifyContext = pCfgList->pPrimitives.pContext;
#endif /* (SEC_ENABLE_WORKSPACE_INTERNAL && SEC_ENABLE_WORKSPACE_EXTERNAL) || SEC_ENABLE_VERIFICATION_DATA_LENGTH || SEC_ENABLE_CRC_TOTAL || SEC_ENABLE_CRC_WRITTEN */

#if defined( SEC_ENABLE_WORKSPACE_INTERNAL ) && \
    defined( SEC_ENABLE_WORKSPACE_EXTERNAL )
   /* Use default workspace, data length not relevant */
   pVerifyContext[0].currentHash.sigResultBuffer   = (SecM_ResultBufferType)SEC_DEFAULT_WORKSPACE; /* PRQA S 0306 */ /* MD_SecVerification_0306 */
   pVerifyContext[0].currentHash.length            = SEC_DEFAULT_WORKSPACE_SIZE;
#endif /* SEC_ENABLE_WORKSPACE_INTERNAL && SEC_ENABLE_WORKSPACE_EXTERNAL */
#if defined( SEC_ENABLE_VERIFICATION_DATA_LENGTH )
   pVerifyContext[0].currentDataLength             = SEC_DATA_LENGTH_NULL;
#endif /* SEC_ENABLE_VERIFICATION_DATA_LENGTH */




   /* Perform verification with configuration list */
   result = SecM_VerificationWrapper(pVerifyParam, pCfgList);

   pVerifyParam->crcTotal = 0u;

   return result;
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

#if defined( SEC_ENABLE_VERIFICATION_DATA_LENGTH )
/**********************************************************************************************************************
 *  SecM_UpdateDataLength
 *********************************************************************************************************************/
/*! \brief         Update data length of signature parameter
 *  \param[in]     pVerifyParam Pointer to parameter structure for signature verification
 *********************************************************************************************************************/
void SecM_UpdateDataLength( V_MEMRAM1 SecM_SignatureParamType V_MEMRAM2 V_MEMRAM3 * pVerifyParam )
{
   SecM_SizeType dataLength;

   /* Disable update if not available (NULL pointer) */
   if (SEC_DATA_LENGTH_NULL != pVerifyParam->currentDataLength)
   {
      /* Load current value into local variable */
      dataLength = *(pVerifyParam->currentDataLength);

      switch (pVerifyParam->sigState)
      {
         case SEC_HASH_INIT:
         {
            /* Reset processed data length */
            dataLength = 0u;

            break;
         }
         case SEC_HASH_COMPUTE:
         {
            /* Update processed data length (temporarily holding byte count) */
            dataLength += pVerifyParam->sigByteCount;

            break;
         }
         case SEC_HASH_FINALIZE:
         {
            /* Data length expected in multiples of 64 byte */
            dataLength >>= 6u;

            break;
         }
         case SEC_SIG_VERIFY:
         default:
         {
            break;
         }
      }

      /* Store possibly modified value */
      *(pVerifyParam->currentDataLength) = dataLength;
   }
}
#endif /* SEC_ENABLE_VERIFICATION_DATA_LENGTH */


#if defined( SEC_ENABLE_SECURITY_CLASS_DDD )
/**********************************************************************************************************************
 *  SecM_VerifyClassDDD
 *********************************************************************************************************************/
/*! \brief         Calculate and verify checksum or hash using primitive configured for security class DDD
 *  \details       See SecM_VerifySignature for details
 *  \param[in]     pVerifyParam Pointer to parameter structure for signature verification
 *                   Member currentHash must (SEC_DISABLE_DEFAULT_WORKSPACE) or may (SEC_ENABLE_WORKSPACE_INTERNAL)
 *                   contain reference to buffer used as workspace
 *  \return        SECM_VER_OK if verification operation successful
 *                 SECM_VER_ERROR if error occured during verification
 *                 SECM_VER_CRC if CRC verification failed
 *                 SECM_VER_SIG if HMAC/Signature verification failed
 *********************************************************************************************************************/
SecM_StatusType SecM_VerifyClassDDD( V_MEMRAM1 SecM_SignatureParamType V_MEMRAM2 V_MEMRAM3 * pVerifyParam )
{
# if defined( SEC_ENABLE_WORKSPACE_INTERNAL )
   /* Perform initializations */
   if (SEC_HASH_INIT == pVerifyParam->sigState)
   {
#  if defined( SEC_ENABLE_WORKSPACE_EXTERNAL )
      /* External workspace given? */
      if (SEC_DEFAULT_WORKSPACE_SIZE == pVerifyParam->currentHash.length)
#  endif /* SEC_ENABLE_WORKSPACE_EXTERNAL */
      {
         /* Set internal default workspace */
         pVerifyParam->currentHash.sigResultBuffer = (SecM_ResultBufferType)SEC_VERIFY_CLASS_DDD_WORKSPACE; /* PRQA S 0306 */ /* MD_SecVerification_0306 */
         pVerifyParam->currentHash.length          = SEC_VERIFY_CLASS_DDD_WORKSPACE_SIZE;
      }
   }
# endif /* SEC_ENABLE_WORKSPACE_INTERNAL */

   return SEC_VERIFY_CLASS_DDD_FUNCTION(pVerifyParam);
}
#endif /* SEC_ENABLE_SECURITY_CLASS_DDD */


#if defined( SEC_ENABLE_SECURITY_CLASS_CCC )
/**********************************************************************************************************************
 *  SecM_VerifyClassCCC
 *********************************************************************************************************************/
/*! \brief         Calculate and verify signature using primitive configured for security class CCC
 *  \details       See SecM_VerifySignature for details
 *  \param[in]     pVerifyParam Pointer to parameter structure for signature verification
 *                   Member currentHash must (SEC_DISABLE_DEFAULT_WORKSPACE) or may (SEC_ENABLE_WORKSPACE_INTERNAL)
 *                   contain reference to buffer used as workspace
 *  \return        SECM_VER_OK if verification operation successful
 *                 SECM_VER_ERROR if error occured during verification
 *                 SECM_VER_CRC if CRC verification failed
 *                 SECM_VER_SIG if HMAC/Signature verification failed
 *********************************************************************************************************************/
SecM_StatusType SecM_VerifyClassCCC( V_MEMRAM1 SecM_SignatureParamType V_MEMRAM2 V_MEMRAM3 * pVerifyParam )
{
   SecM_StatusType result;

   /* Perform initializations */
   if (SEC_HASH_INIT == pVerifyParam->sigState)
   {
# if defined( SEC_ENABLE_WORKSPACE_INTERNAL )
#  if defined( SEC_ENABLE_WORKSPACE_EXTERNAL )
      /* External workspace given? */
      if (SEC_DEFAULT_WORKSPACE_SIZE == pVerifyParam->currentHash.length)
#  endif /* SEC_ENABLE_WORKSPACE_EXTERNAL */
      {
         /* Set internal default workspace */
         pVerifyParam->currentHash.sigResultBuffer = (SecM_ResultBufferType)SEC_VERIFY_CLASS_CCC_WORKSPACE; /* PRQA S 0306 */ /* MD_SecVerification_0306 */
         pVerifyParam->currentHash.length          = SEC_VERIFY_CLASS_CCC_WORKSPACE_SIZE;
      }
# endif /* SEC_ENABLE_WORKSPACE_INTERNAL */

   }

   /* Perform verification operation */
   result = SEC_VERIFY_CLASS_CCC_FUNCTION(pVerifyParam);

   return result;
}
#endif /* SEC_ENABLE_SECURITY_CLASS_CCC */

/**********************************************************************************************************************
 *  SecM_VerifySignature
 *********************************************************************************************************************/
/*! \brief         Calculate and verify checksum or signature using primitive of configured security class
 *  \details       Perform one of the following operations, defined by pVerifyParam->sigState:
 *                 - Initialization (kHashInit)
 *                 - Update checksum (kHashCompute)
 *                 - Finalize checksum (kHashFinalize)
 *                 - Verify signature/checksum (kSigVerify)
 *  \param[in]     pVerifyParam Pointer to parameter structure for signature verification
 *                   Member currentHash must (SEC_DISABLE_DEFAULT_WORKSPACE) or may (SEC_ENABLE_WORKSPACE_INTERNAL)
 *                   contain reference to buffer used as workspace
 *  \return        SECM_VER_OK if verification operation successful
 *                 SECM_VER_ERROR if error occured during verification
 *                 SECM_VER_CRC if CRC verification failed
 *                 SECM_VER_SIG if HMAC/Signature verification failed
 *********************************************************************************************************************/
SecM_StatusType SecM_VerifySignature( V_MEMRAM1 SecM_SignatureParamType V_MEMRAM2 V_MEMRAM3 * pVerifyParam )
{
   /* Perform verification operation */
   return SEC_VERIFY_FUNCTION(pVerifyParam);
}

/**********************************************************************************************************************
 *  SecM_InitVerification
 *********************************************************************************************************************/
/*! \brief         Intializes the verification
 *  \param[in]     init Dummy pointer (currently not used)
 *  \return        SECM_OK if initalization successful
 *                 SECM_NOT_OK if error occured during initalization
 *********************************************************************************************************************/
SecM_StatusType SecM_InitVerification( SecM_VerifyInitType init )
{
# if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   /* Avoid compiler warnings */
   (void)init; /* PRQA S 3112 */ /* MD_SecVerification_3112 */
# endif /* V_ENABLE_USE_DUMMY_STATEMENT */

   return SECM_OK;
}

/**********************************************************************************************************************
 *  SecM_DeinitVerification
 *********************************************************************************************************************/
/*! \brief         Deintializes the verification
 *  \param[in]     deinit Dummy pointer (currently not used)
 *  \return        SECM_OK if deinitalization successful
 *                 SECM_NOT_OK if error occured during deinitalization
 *********************************************************************************************************************/
SecM_StatusType SecM_DeinitVerification( SecM_VerifyDeinitType deinit )
{
# if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   /* Avoid compiler warnings */
   (void)deinit; /* PRQA S 3112 */ /* MD_SecVerification_3112 */
# endif /* V_ENABLE_USE_DUMMY_STATEMENT */

   return SECM_OK;
}


#if defined( SEC_ENABLE_SECURITY_CLASS_DDD )
/**********************************************************************************************************************
 *  SecM_VerificationClassDDD
 *********************************************************************************************************************/
/*! \brief         Calculate and verify checksum or signature using primitive configured for security class DDD
 *  \details       See SecM_Verification
 *  \param[in]     pVerifyParam Pointer to parameter structure for signature verification
 *  \return        SECM_VER_OK if signature verification successful
 *                 SECM_VER_ERROR if error occured during verification
 *                 SECM_VER_CRC if CRC verification failed
 *                 SECM_VER_SIG if HMAC/Signature verification failed
 *********************************************************************************************************************/
SecM_StatusType SecM_VerificationClassDDD( V_MEMRAM1 SecM_VerifyParamType V_MEMRAM2 V_MEMRAM3 * pVerifyParam )
{
   SecM_SignatureParamType    verifyContext[SEC_VER_ARRAY_SIZE(verifyConfigClassDDD)];
   SecM_VerifyConfigListType  config;

   config.pPrimitives.pOperation = verifyConfigClassDDD;
   config.pPrimitives.pContext   = verifyContext;
   config.count                  = SEC_VER_ARRAY_SIZE(verifyConfigClassDDD);

   /* Include CRC total and additional vendor checksum, leave out CRC written (already covered by base) */
   return SecM_VerificationBase(pVerifyParam, &config);
}
#endif /* SEC_ENABLE_SECURITY_CLASS_DDD */


#if defined( SEC_ENABLE_SECURITY_CLASS_CCC )
/**********************************************************************************************************************
 *  SecM_VerificationClassCCC
 *********************************************************************************************************************/
/*! \brief         Calculate and verify checksum or signature using primitive configured for security class CCC
 *  \details       See SecM_Verification
 *  \param[in]     pVerifyParam Pointer to parameter structure for signature verification
 *  \return        SECM_VER_OK if signature verification successful
 *                 SECM_VER_ERROR if error occured during verification
 *                 SECM_VER_CRC if CRC verification failed
 *                 SECM_VER_SIG if HMAC/Signature verification failed
 *********************************************************************************************************************/
SecM_StatusType SecM_VerificationClassCCC( V_MEMRAM1 SecM_VerifyParamType V_MEMRAM2 V_MEMRAM3 * pVerifyParam )
{
   SecM_SignatureParamType    verifyContext[SEC_VER_ARRAY_SIZE(verifyConfigClassCCC)];
   SecM_VerifyConfigListType  config;

   config.pPrimitives.pOperation = verifyConfigClassCCC;
   config.pPrimitives.pContext   = verifyContext;
   config.count                  = SEC_VER_ARRAY_SIZE(verifyConfigClassCCC);

   /* Include CRC total, CRC written and additional vendor checksum */
   return SecM_VerificationBase(pVerifyParam, &config);
}
#endif /* SEC_ENABLE_SECURITY_CLASS_CCC */

#if defined( SEC_ENABLE_SECURITY_CLASS_VENDOR )
/**********************************************************************************************************************
 *  SecM_VerificationClassVendor
 *********************************************************************************************************************/
/*! \brief         Calculate and verify checksum or signature using primitive configured for security class Vendor
 *  \details       See SecM_Verification
 *  \param[in]     pVerifyParam Pointer to parameter structure for signature verification
 *  \return        SECM_VER_OK if signature verification successful
 *                 SECM_VER_ERROR if error occured during verification
 *                 SECM_VER_CRC if CRC verification failed
 *                 SECM_VER_SIG if HMAC/Signature verification failed
 *********************************************************************************************************************/
SecM_StatusType SecM_VerificationClassVendor( V_MEMRAM1 SecM_VerifyParamType V_MEMRAM2 V_MEMRAM3 * pVerifyParam )
{
   SecM_SignatureParamType    verifyContext[SEC_VER_ARRAY_SIZE(verifyConfigClassVendor)];
   SecM_VerifyConfigListType  config;

   config.pPrimitives.pOperation = verifyConfigClassVendor;
   config.pPrimitives.pContext   = verifyContext;
   config.count                  = SEC_VER_ARRAY_SIZE(verifyConfigClassVendor);

   /* Include CRC total, leave out CRC written and additional vendor checksum (already covered by base) */
   return SecM_VerificationBase(pVerifyParam, &config);
}
#endif /* SEC_ENABLE_SECURITY_CLASS_VENDOR */

/**********************************************************************************************************************
 *  SecM_Verification
 *********************************************************************************************************************/
/*! \brief         Calculate and verify checksum or signature using primitive of configured security class
 *  \details       Calculates the checksum or signature ov all segments present in the verification parameter
 *                 Function passed in readMemory is used to access memory.
 *                 Afterwards the calculated value is used to verify the checksum/signature provided in verificationData
 *                 If configured a CRC over the complete block, defined by blockStartAddress and blockLength, including
 *                 all readable gaps between the actual segments is calculated too and returned in parameter
 *  \param[in]     pVerifyParam Pointer to parameter structure for signature verification crcTotal.
 *  \return        SECM_VER_OK if signature verification successful
 *                 SECM_VER_ERROR if error occured during verification
 *                 SECM_VER_CRC if CRC verification failed
 *                 SECM_VER_SIG if HMAC/Signature verification failed
 *********************************************************************************************************************/
SecM_StatusType SecM_Verification( V_MEMRAM1 SecM_VerifyParamType V_MEMRAM2 V_MEMRAM3 * pVerifyParam )
{
   /* Perform verification using configured security class */
   return SEC_VERIFICATION_FUNCTION(pVerifyParam);
}

/**********************************************************************************************************************
 *  MISRA
 *********************************************************************************************************************/

/* Module specific MISRA deviations:

   MD_SecVerification_0703:
      Reason: Not all of the declared struct members are initialized. Signature parameter member of verification
       primitive serves two functions:
       1) Pass configuration information to the primitive instance. This includes the workspace (specific or default)
          and data length (usually mapped to null pointer as irrelevant)
       2) Provide a context for the primitive instance. The signature parameter is exclusively used by this instance.
          All relevant parameters passed to the verify wrapper are copied to the individual signature parameters.
          This includes all members not set during initialization.
      Risk: Configuration of verification primitive is initialized with fewer members than expected.
      Prevention: Document initialization requirements in description of structure.

   MD_SecVerification_3196:
      Reason: Variable used as context to remove the need for global variables. Initialization takes place in called
       function.
      Risk: Callee does not perform complete initializaton, but accesses those un-initialized parts of the variable anyway.
       Implies same risk as use of global variable.
      Prevention: Perform code analysis of callee to make sure no un-initialized parts are accessed.

   MD_SecVerification_0277:
      Reason:Bit-width of a non-basic type may not be trivial as it could be configuration dependent.
       Cast of negative one to unsigned type is used to get maximum representable value (all bits set).
      Risk: Although this conversion is guaranteed by the C standard (6.3.1.3/2) it may not be obvious.
      Prevention: Document intend and explanation in code.

   MD_SecVerification_0306:
      Reason: HIS security module specification defines reference to buffer to be stored as unsigned 32-bit value instead
       of pointer type.
      Risk: The size of integer required to hold the result of a pointer cast is implementation defined.
      Prevention: 32 bit handles all current use-cases. Pay special attention when 64 bit ECUs are introduced.

   MD_SecVerification_3225:
      Reason: Input buffer placed on stack to remove the need for global variables. Reference only used in SecM_Verify*
       functions executed in context of declaring function.
      Risk: Buffer reference used outside of context of declaring function.
      Prevention: Ensure correct use of buffer in called functions through code inspection.

   MD_SecVerification_3673:
     Reason: Parameter or parts of it are actually modified inside the calling hierarchy.
     Risk: No identifiable risk.
     Prevention: No prevention required.

   MD_SecVerification_3112:
     Reason: Dummy statements have been added to avoid compiler warnings because of unused variables.
     Risk: Unused statements add to program complexity just by being there - adding lines to the program and causing
      readers to puzzle over its presence.
     Prevention: Dummy statements can be turned off externally (V_DISABLE_USE_DUMMY_STATEMENT).

   MD_SecVerification_3218:
     Reason: The local variables of this module are kept at a central location for a better overview and maintenance.
     Risk: Scope is larger than required (whole file instead of one function). Some other function could access
      the variable.
     Prevention: Restrict the functionality in this module to the intended purpose. Don't add functions which shall not
      be able to access the local data buffers.

   MD_SecVerification_3382:
     Reason: Subtraction from unsigned value without explicit range check directly before operation.
      Instead limitation of subtracted value is located above as it is required before.
     Risk: Distance between limitation and subtraction makes correlation less obvious.
     Prevention: Ensure correct limitation of subtracted value through code inspection..
*/

/***********************************************************************************************************************
 *  END OF FILE: SEC_VERIFICATION.C
 **********************************************************************************************************************/
