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
 *                                                   Renamed file name (was SecMVer.h)
 *                                                   Split into source and library parts
 *                                                   Added extended verification API (externally provided key and workspace)
 *                                                   Unified handling of Verification and VerifySignature API
 *  02.00.01   2013-12-19  JHg     ESCAN00072260    No changes
 *                         JHg     -                Fixed remapping of SEC_VER_DISABLE_LENGTH_AND_ADDRESS_INPUT
 *                         JHg     ESCAN00072561    No changes
 *  02.00.02   2014-01-20  JHg     ESCAN00073088    No changes
 *  02.01.00   2014-01-28  ASe     ESCAN00073047    No changes
 *  02.02.00   2014-02-10  JHg     ESCAN00073560    CRC algorithm selectable at compile time
 *                         JHg     ESCAN00073355    No changes
 *  02.02.01   2014-05-28  JHg     ESCAN00074792    Corrected memory qualifiers for configuration list parameters
 *                         JHg     ESCAN00073918    No changes
 **********************************************************************************************************************/

#ifndef __SEC_VERIFICATION_H__
#define __SEC_VERIFICATION_H__

/**********************************************************************************************************************
 *  VERSION
 *********************************************************************************************************************/

/* ##V_CFG_MANAGEMENT ##CQProject : SysService_SecModHis CQComponent : Impl_Verification */
#define SYSSERVICE_SECMODHIS_VERIFICATION_VERSION            0x0202u
#define SYSSERVICE_SECMODHIS_VERIFICATION_RELEASE_VERSION    0x01u

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "Sec_Inc.h"

/* Required for security class DDD with CRC and CRC type in verification parameter */
#include "Sec_Crc.h"

# include "Sec_Workspace.h"

/**********************************************************************************************************************
 *  DEFINES
 *********************************************************************************************************************/

/* Verification return codes */
#define SECM_VER_OK        0x00u /**< Verification successful              */
#define SECM_VER_ERROR     0x01u /**< Error occured during verification    */
#define SECM_VER_CRC       0x02u /**< CRC verification failed              */
#define SECM_VER_SIG       0x04u /**< HMAC/Signature verification failed   */

/* Verification states (as defined by SecM222 (V1.1)) */
#define kHashInit          0x01u /**< Init signature verification       */
#define kHashCompute       0x02u /**< Computation state                 */
#define kHashFinalize      0x03u /**< Finalize hash (not implemented)   */
#define kSigVerify         0x04u /**< Final tasks                       */

/* Remap verification states */
#define SEC_HASH_NONE      0x00u         /**< Marker for invalid verification state */
#define SEC_HASH_INIT      kHashInit     /**< Init signature verification           */
#define SEC_HASH_COMPUTE   kHashCompute  /**< Computation state                     */
#define SEC_HASH_FINALIZE  kHashFinalize /**< Finalize hash (not implemented)       */
#define SEC_SIG_VERIFY     kSigVerify    /**< Final tasks                           */

/* Masks for operations to be carried out by verification primitives */
#define SEC_UPDATE_OPERATION_DEFAULT         0x00u /**< Initialization and update with intra-segment data   */
#define SEC_UPDATE_OPERATION_ADDRESS_LENGTH  0x01u /**< Update with inter-segment data                      */
#define SEC_UPDATE_OPERATION_INTER_SEGMENT   0x02u /**< Update with address and length of segments          */
#define SEC_UPDATE_OPERATION_VERIFICATION    0x04u /**< Verification against given signature/checksum       */

/** Null pointer for data length (currentDataLength) */
#define SEC_DATA_LENGTH_NULL     ((V_MEMRAM1 SecM_SizeType V_MEMRAM2 V_MEMRAM3 *)V_NULL)
/** Null pointer for verification key */
#define SEC_VERIFY_KEY_NULL      ((SecM_VerifyKeyType)V_NULL)

/*********************************************************************************************************************/

/* Compatibility remapping of configuration defines */

#if defined( SEC_VER_ENABLE_CRC_TOTAL )
/** Enable CRC total calculation in SecM_Verification */
#  define SEC_ENABLE_CRC_TOTAL
#endif /* SEC_VER_ENABLE_CRC_TOTAL */
#if defined( SEC_VER_DISABLE_CRC_TOTAL )
# ifndef SEC_DISABLE_CRC_TOTAL
/** Disable CRC total calculation in SecM_Verification */
#  define SEC_DISABLE_CRC_TOTAL
# endif
#endif /* SEC_VER_DISABLE_CRC_TOTAL */

#if defined( SEC_VER_ENABLE_LENGTH_AND_ADDRESS_INPUT )
# ifndef SEC_ENABLE_VERIFICATION_ADDRESS_LENGTH
/** Include address and length information of segments in checksum calculation of SecM_Verification */
#  define SEC_ENABLE_VERIFICATION_ADDRESS_LENGTH
# endif
#endif /* SEC_VER_ENABLE_LENGTH_AND_ADDRESS_INPUT */
#if defined( SEC_VER_DISABLE_LENGTH_AND_ADDRESS_INPUT )
# ifndef SEC_DISABLE_VERIFICATION_ADDRESS_LENGTH
/** Do not include address and length information of segments in checksum calculation of SecM_Verification */
#  define SEC_DISABLE_VERIFICATION_ADDRESS_LENGTH
# endif
#endif /* SEC_VER_DISABLE_LENGTH_AND_ADDRESS_INPUT */

#if defined( SECM_VER_SIG_OFFSET )
# ifndef SEC_VER_SIG_OFFSET
/** Offset of signature value in verification data provided to SecM_Verification */
#  define SEC_VER_SIG_OFFSET  SECM_VER_SIG_OFFSET
# endif
#endif /* SECM_VER_SIG_OFFSET */
#if defined( SECM_VER_CRC_OFFSET )
# ifndef SEC_VER_CRC_OFFSET
/** Offset of CRC value in verification data provided to SecM_Verification */
#  define SEC_VER_CRC_OFFSET  SECM_VER_CRC_OFFSET
# endif
#endif /* SECM_VER_CRC_OFFSET */

/*********************************************************************************************************************/

/* Defaults for configuration defines */

#if defined( SEC_ENABLE_VERIFICATION_KEY_EXTERNAL )
#else
/** Use internal default keys for signature/checksum verification */
# define SEC_ENABLE_VERIFICATION_KEY_INTERNAL
#endif /* SEC_ENABLE_VERIFICATION_KEY_INTERNAL || SEC_ENABLE_VERIFICATION_KEY_EXTERNAL */

/* Remap security classes to default when not explicitly set */
/** Security class of SecM_VerifySignature interface */
#if defined( SEC_SECURITY_CLASS_VERIFY )
#else
# define SEC_SECURITY_CLASS_VERIFY         SEC_SECURITY_CLASS
#endif

/** Security class of SecM_Verification interface */
#if defined( SEC_SECURITY_CLASS_VERIFICATION )
#else
# define SEC_SECURITY_CLASS_VERIFICATION   SEC_SECURITY_CLASS
#endif

#  if defined( SEC_CHECKSUM_TYPE )
#  else
/* CRC not available for checksum verification, use hash */
#   define SEC_CHECKSUM_TYPE        SEC_CHECKSUM_TYPE_HASH
#  endif /* SEC_CHECKSUM_TYPE */

#if defined( SEC_CHECKSUM_TYPE )
#else
/** Algorithm used for checksum verification (security class DDD)*/
# define SEC_CHECKSUM_TYPE          SEC_CHECKSUM_TYPE_CRC
#endif /* SEC_CHECKSUM_TYPE */

/* Remap verification data offsets to default when not explicitly set */
/** Offset of signature value in verification data (used by SecM_Verification) */
#if defined( SEC_VER_SIG_OFFSET )
#else
# define SEC_VER_SIG_OFFSET                    0u
#endif /* SEC_VER_SIG_OFFSET */
/** Offset of CRC value in verification data (used by SecM_Verification) */
#if defined( SEC_VER_CRC_OFFSET )
#else
# define SEC_VER_CRC_OFFSET                    0u
#endif /* SEC_VER_CRC_OFFSET */

/** Overwrite default verification data offsets in case explicitly set */
#if ( SEC_VER_SIG_OFFSET != 0u ) && \
    ( SEC_VER_CRC_OFFSET != 0u )
# define SEC_VERIFY_OVERWRITE_OFFSET
#endif

#if defined( SEC_ENABLE_VERIFICATION_ASSERT_READ_COUNT ) || \
    defined( SEC_DISABLE_VERIFICATION_ASSERT_READ_COUNT )
#else
/** Assert equality of read count and requested length for memory reads */
# define SEC_ENABLE_VERIFICATION_ASSERT_READ_COUNT
#endif


/*********************************************************************************************************************/

/* Remap compile-time switches */

# if ( SEC_SECURITY_CLASS_VERIFY == SEC_CLASS_DDD ) || \
     ( SEC_SECURITY_CLASS_VERIFICATION == SEC_CLASS_DDD )
#  define SEC_ENABLE_SECURITY_CLASS_DDD
# endif /* SEC_CLASS_DDD */


# if ( SEC_SECURITY_CLASS_VERIFY == SEC_CLASS_CCC ) || \
     ( SEC_SECURITY_CLASS_VERIFICATION == SEC_CLASS_CCC )
#  define SEC_ENABLE_SECURITY_CLASS_CCC
# endif /* SEC_CLASS_CCC */

# if ( SEC_SECURITY_CLASS_VERIFY == SEC_CLASS_VENDOR ) || \
     ( SEC_SECURITY_CLASS_VERIFICATION == SEC_CLASS_VENDOR )
#  define SEC_ENABLE_SECURITY_CLASS_VENDOR
# endif /* SEC_CLASS_VENDOR */

# if ( SEC_CHECKSUM_TYPE == SEC_CHECKSUM_TYPE_HASH )
#  define SEC_ENABLE_CHECKSUM_TYPE_HASH
# endif /* SEC_CHECKSUM_TYPE_HASH */




# if ( SEC_HASH_ALGORITHM == SEC_SHA256 )
#  define SEC_ENABLE_HASH_ALGORITHM_SHA256
# endif /* SEC_SHA256 */


/*********************************************************************************************************************/

/* Size definitions for checksum/signature values */
/** Size of CRC-16 checksum */
#define SEC_SIZE_CHECKSUM_CRC_16    2u
/** Size of CRC-32 checksum */
#define SEC_SIZE_CHECKSUM_CRC_32    4u
/** Size of SHA-1 hash */
#define SEC_SIZE_HASH_SHA1          20u
/** Size of SHA-256 hash */
#define SEC_SIZE_HASH_SHA256        32u
/** Size of RIPEMD-160 hash */
#define SEC_SIZE_HASH_RMD160        20u
/** Size of RSA-512 signature */
#define SEC_SIZE_SIG_RSA512         64u
/** Size of RSA-1024 signature */
#define SEC_SIZE_SIG_RSA1024        128u
/** Size of RSA-1536 signature */
#define SEC_SIZE_SIG_RSA1536        192u
/** Size of RSA-2048 signature */
#define SEC_SIZE_SIG_RSA2048        256u
/** Default value for vendor specific checksum */
#if defined( SEC_SIZE_CHECKSUM_VENDOR )
#else
# define SEC_SIZE_CHECKSUM_VENDOR   1u
#endif /* SEC_SIZE_CHECKSUM_VENDOR */

/** Size of CRC checksum (configuration dependent) */
#if defined( SEC_ENABLE_CRC_TYPE_CRC32 )
#define SEC_SIZE_CHECKSUM_CRC       SEC_SIZE_CHECKSUM_CRC_32
#endif /* SEC_ENABLE_CRC_TYPE_CRC32 */

/** Size of RSA signature (configuration dependent) */
#define SEC_SIZE_SIG_RSA            SEC_SIZE_SIG_RSA2048

# if (SEC_CHECKSUM_TYPE == SEC_CHECKSUM_TYPE_HASH)
#  if (SEC_HASH_ALGORITHM == SEC_SHA1)
/** Size of message digest (class DDD) */
#   define SEC_VERIFY_CLASS_DDD_DIGEST_SIZE     SEC_SIZE_HASH_SHA1
/** Size of verification data (class DDD) */
#   define SEC_VERIFY_CLASS_DDD_VERIFY_SIZE     SEC_SIZE_HASH_SHA1
#  elif (SEC_HASH_ALGORITHM == SEC_RIPEMD160)
#   define SEC_VERIFY_CLASS_DDD_DIGEST_SIZE     SEC_SIZE_HASH_RMD160
#   define SEC_VERIFY_CLASS_DDD_VERIFY_SIZE     SEC_SIZE_HASH_RMD160
#  elif (SEC_HASH_ALGORITHM == SEC_SHA256)
#   define SEC_VERIFY_CLASS_DDD_DIGEST_SIZE     SEC_SIZE_HASH_SHA256
#   define SEC_VERIFY_CLASS_DDD_VERIFY_SIZE     SEC_SIZE_HASH_SHA256
#  endif /* SEC_HASH_ALGORITHM */
# else /* SEC_CHECKSUM_TYPE == SEC_CHECKSUM_TYPE_CRC */
#  define SEC_VERIFY_CLASS_DDD_DIGEST_SIZE      SEC_SIZE_CHECKSUM_CRC
#  define SEC_VERIFY_CLASS_DDD_VERIFY_SIZE      SEC_SIZE_CHECKSUM_CRC
# endif /* SEC_CHECKSUM_TYPE */

/** Offset of checksum in verification data (class DDD) */
# if defined( SEC_VERIFY_OVERWRITE_OFFSET )
#  define SEC_VERIFY_CLASS_DDD_VERIFY_OFFSET    SEC_VER_CRC_OFFSET
#  define SEC_VERIFY_CLASS_DDD_CHECKSUM_OFFSET  (SEC_VER_CRC_OFFSET + SEC_VERIFY_CLASS_DDD_VERIFY_SIZE)
# else
#  define SEC_VERIFY_CLASS_DDD_VERIFY_OFFSET    0u
#  define SEC_VERIFY_CLASS_DDD_CHECKSUM_OFFSET  SEC_VERIFY_CLASS_DDD_VERIFY_SIZE
# endif /* SEC_VERIFY_OVERWRITE_OFFSET */


# if (SEC_HASH_ALGORITHM == SEC_SHA1)
/** Size of message digest (class CCC) */
#  define SEC_VERIFY_CLASS_CCC_DIGEST_SIZE      SEC_SIZE_HASH_SHA1
# elif (SEC_HASH_ALGORITHM == SEC_RIPEMD160)
#  define SEC_VERIFY_CLASS_CCC_DIGEST_SIZE      SEC_SIZE_HASH_RMD160
# elif (SEC_HASH_ALGORITHM == SEC_SHA256)
#  define SEC_VERIFY_CLASS_CCC_DIGEST_SIZE      SEC_SIZE_HASH_SHA256
# endif /* SEC_HASH_ALGORITHM */

/** Size of verification data (class CCC) */
# define SEC_VERIFY_CLASS_CCC_VERIFY_SIZE       SEC_SIZE_SIG_RSA

# if defined( SEC_VERIFY_OVERWRITE_OFFSET )
/** Offset of signature in verification data (class CCC) */
#  define SEC_VERIFY_CLASS_CCC_VERIFY_OFFSET    SEC_VER_SIG_OFFSET
/** Offset of checksum in verification data (class CCC) */
#  define SEC_VERIFY_CLASS_CCC_CHECKSUM_OFFSET  SEC_VER_CRC_OFFSET
# else
#  define SEC_VERIFY_CLASS_CCC_VERIFY_OFFSET    0u
#  define SEC_VERIFY_CLASS_CCC_CHECKSUM_OFFSET  SEC_VERIFY_CLASS_CCC_VERIFY_SIZE
# endif /* SEC_VERIFY_OVERWRITE_OFFSET */

/** Offset of checksum in verification data (class vendor) */
# if defined( SEC_VERIFY_OVERWRITE_OFFSET )
#  define SEC_VERIFY_CLASS_VENDOR_CHECKSUM_OFFSET  SEC_VER_CRC_OFFSET
# else
#  define SEC_VERIFY_CLASS_VENDOR_CHECKSUM_OFFSET  0u
# endif /* SEC_VERIFY_OVERWRITE_OFFSET */

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/* Supporting data types for easier handling of configuration dependent settings */

/** Pointer to verification data */
/* Verification data is read-only */
typedef SecM_ConstRamDataType SecM_VerifyDataType;

/** Type to reference current hash value (also used to pass workspace) */
/* According to HIS specification: store address as unsigned 32 bit integer */
typedef SecM_SizeType         SecM_ResultBufferType;

/** Anonymous pointer to verification key (actual type depending on security class) */
typedef SecM_VoidPtrType      SecM_VerifyKeyType;

/*********************************************************************************************************************/

/* Types specified by HIS specification */

/** Type of parameter passed to SecM_InitVerification */
typedef SecM_ConstVoidPtrType   SecM_VerifyInitType;
/** Type of parameter passed to SecM_DeinitVerification */
typedef SecM_ConstVoidPtrType   SecM_VerifyDeinitType;

/** Parameter structure for SecM_Verification interface */
typedef struct
{
   /** Address and length information of download segments */
   /* According to HIS specification */
   FL_SegmentListType   segmentList;
   /** Start address of logical block */
   SecM_AddrType        blockStartAddress;
   /** Length of logical block */
   SecM_SizeType        blockLength;
   /** Pointer to verification data (e.g. checksum and/or signature) */
   SecM_VerifyDataType  verificationData;
   /** CRC total calculated over full logical block */
   SecM_CRCType         crcTotal;
   /** Pointer to watchdog trigger function */
   FL_WDTriggerFctType  wdTriggerFct;
   /** Pointer to memory read function */
   FL_ReadMemoryFctType readMemory;
   /* The following members are an extension of the HIS specification */
   /** Reference to workspace */
   SecM_WorkspaceType   workspace;
   /** Pointer to verification key */
   SecM_VerifyKeyType   key;
} SecM_VerifyParamType;

/** Structure to describe current hash value (also used to pass workspace) */
typedef struct
{
   SecM_ResultBufferType sigResultBuffer;
   SecM_SizeType         length;
} SecM_SignatureType;

/* Parameter structure for signature verification */
typedef struct
{
   /** Reference to current hash value (also used to pass workspace) */
   SecM_SignatureType   currentHash;
   /** Pointer to the current length of the hashed data */
   V_MEMRAM1 SecM_SizeType V_MEMRAM2 V_MEMRAM3 * currentDataLength;
   /** Signature state / operation to be executed */
   SecM_StatusType      sigState;
   /** Pointer to input or verification data */
   SecM_VerifyDataType  sigSourceBuffer;
   /** Size of input or verification data */
   SecM_LengthType      sigByteCount;
   /** Pointer to watchdog trigger function */
   FL_WDTriggerFctType  wdTriggerFct;
   /* The following members are an extension of the HIS specification */
   /** Pointer to verification key */
   SecM_VerifyKeyType   key;
} SecM_SignatureParamType;

/* Pointer types for function calls from external application */
typedef SecM_StatusType SEC_CALL_TYPE (*pSecInitVerificationFct)     (SecM_VerifyInitType init);
typedef SecM_StatusType SEC_CALL_TYPE (*pSecDeinitVerificationFct)   (SecM_VerifyDeinitType deinit);
typedef SecM_StatusType SEC_CALL_TYPE (*pSecVerificationFct)         (V_MEMRAM1 SecM_VerifyParamType V_MEMRAM2 V_MEMRAM3 * pVerifyParam);
typedef SecM_StatusType SEC_CALL_TYPE (*pSecVerifySignatureFct)      (V_MEMRAM1 SecM_SignatureParamType V_MEMRAM2 V_MEMRAM3 * pVerifyParam);

/*********************************************************************************************************************/

/* Configuration of verification primitives */

/** Operation to be carried out during verification */
typedef struct
{
   pSecVerifySignatureFct  pFunction;     /**< Pointer to verification function (SecM_VerifySignature compatible) */
   SecM_LengthType         length;        /**< Length of digest (e.g. checksum or signature) */
   SecM_LengthFastType     offset;        /**< Offset of digest in verification data */
   SecM_ByteType           mask;          /**< Masks for operations to be carried out (see SEC_UPDATE_OPERATION_* for details) */
} SecM_VerifyOperationType;

/** Config for verification primitive */
typedef struct
{
   V_MEMROM1 SecM_VerifyOperationType V_MEMROM2 V_MEMROM3 * pOperation; /**< Verification operation */
   V_MEMRAM1 SecM_SignatureParamType V_MEMRAM2 V_MEMRAM3 *  pContext;   /**< Verification parameter passed to function, used to hold context
                                                                         *   Remark: Members currentHash and currentDataLength have to be set by caller
                                                                         *   either to provide
                                                                         *   - An explicit workspace or use the default ((SecM_ResultBufferType)SEC_DEFAULT_WORKSPACE, SEC_DEFAULT_WORKSPACE_SIZE)
                                                                         *   - An explicit data length variable or ignore it as not required (SEC_DATA_LENGTH_NULL) */
} SecM_VerifyConfigType;

/** Configuration list of verification primitives including contexts */
typedef struct
{
   SecM_VerifyConfigType   pPrimitives;   /**< Pointers to list of primitive operations and contexts */
   SecM_ByteFastType       count;         /**< Number of list entries */
} SecM_VerifyConfigListType;

/*********************************************************************************************************************/

/* Workspace types used by verification primitives
   Remark: Hash values are located at very beginning of workspace */

/** Basic workspace for SHA-256 hash primitive */
typedef struct
{
   SecM_ByteType     digest[SEC_SIZE_HASH_SHA256]; /**< Location for calculated hash value */
} SecM_BasicWorkspaceHashSha256Type;


/** Basic workspace for SHA-256 RSA signature primitive */
typedef struct
{
   SecM_ByteType     digest[SEC_SIZE_HASH_SHA256]; /**< Location for calculated hash value */
   SecM_AsymKeyType  defaultKey;                   /**< Reserve space for key structure referencing default key */
} SecM_BasicWorkspaceSigSha256Type;

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#if defined( __cplusplus )
extern "C" {
#endif

/* Functions specified by HIS specification */

extern SecM_StatusType SecM_InitVerification       ( SecM_VerifyInitType init );
extern SecM_StatusType SecM_DeinitVerification     ( SecM_VerifyDeinitType deinit );
extern SecM_StatusType SecM_Verification           ( V_MEMRAM1 SecM_VerifyParamType V_MEMRAM2 V_MEMRAM3 * pVerifyParam );
extern SecM_StatusType SecM_VerifySignature        ( V_MEMRAM1 SecM_SignatureParamType V_MEMRAM2 V_MEMRAM3 * pVerifyParam );

/*********************************************************************************************************************/

/* Helper functions */

#if defined( SEC_ENABLE_VERIFICATION_DATA_LENGTH )
extern void SecM_UpdateDataLength                  ( V_MEMRAM1 SecM_SignatureParamType V_MEMRAM2 V_MEMRAM3 * pVerifyParam );
#endif /* SEC_ENABLE_VERIFICATION_DATA_LENGTH */
#if defined( SEC_ENABLE_VERIFICATION_WRAPPER )
extern SecM_StatusType SecM_VerificationWrapper ( V_MEMRAM1 SecM_VerifyParamType V_MEMRAM2 V_MEMRAM3 * pVerifyParam,
   V_MEMRAM1 SecM_VerifyConfigListType V_MEMRAM2 V_MEMRAM3 * pCfgList );
#endif /* SEC_ENABLE_VERIFICATION_WRAPPER */

/*********************************************************************************************************************/

/* Access individual security classes and verification primitives */


#if defined( SEC_ENABLE_SECURITY_CLASS_DDD )
extern SecM_StatusType SecM_VerificationClassDDD   ( V_MEMRAM1 SecM_VerifyParamType V_MEMRAM2 V_MEMRAM3 * pVerifyParam );
extern SecM_StatusType SecM_VerifyClassDDD         ( V_MEMRAM1 SecM_SignatureParamType V_MEMRAM2 V_MEMRAM3 * pVerifyParam );

extern SecM_StatusType SecM_VerifyHashSha256       ( V_MEMRAM1 SecM_SignatureParamType V_MEMRAM2 V_MEMRAM3 * pVerifyParam );
#endif /* SEC_ENABLE_SECURITY_CLASS_DDD */


#if defined( SEC_ENABLE_SECURITY_CLASS_CCC )
extern SecM_StatusType SecM_VerificationClassCCC   ( V_MEMRAM1 SecM_VerifyParamType V_MEMRAM2 V_MEMRAM3 * pVerifyParam );
extern SecM_StatusType SecM_VerifyClassCCC         ( V_MEMRAM1 SecM_SignatureParamType V_MEMRAM2 V_MEMRAM3 * pVerifyParam );

extern SecM_StatusType SecM_VerifySigSha256        ( V_MEMRAM1 SecM_SignatureParamType V_MEMRAM2 V_MEMRAM3 * pVerifyParam );
#endif /* SEC_ENABLE_SECURITY_CLASS_CCC */

#if defined( SEC_ENABLE_SECURITY_CLASS_VENDOR )
extern SecM_StatusType SecM_VerificationClassVendor( V_MEMRAM1 SecM_VerifyParamType V_MEMRAM2 V_MEMRAM3 * pVerifyParam );
#endif /* SEC_ENABLE_SECURITY_CLASS_VENDOR */
#if defined( SEC_ENABLE_SECURITY_CLASS_VENDOR ) || \
    defined( SEC_ENABLE_VENDOR_CHECKSUM )
/* User callback */
extern SecM_StatusType SecM_VerifyClassVendor      ( V_MEMRAM1 SecM_SignatureParamType V_MEMRAM2 V_MEMRAM3 * pVerifyParam );
#endif /* SEC_ENABLE_SECURITY_CLASS_VENDOR || SEC_ENABLE_VENDOR_CHECKSUM */

#if defined( __cplusplus )
} /* extern "C" */
#endif

/***********************************************************************************************************************
 *  CONFIGURATION CHECKS
 **********************************************************************************************************************/



#if ( SEC_VERIFY_BYTES > 0xFFFFu )
# error "Error in configuration: SEC_VERIFY_BYTES exceeds valid range"
#endif /* SEC_VERIFY_BYTES */

#if defined( SEC_ENABLE_WORKSPACE_INTERNAL )  || \
    defined( SEC_ENABLE_WORKSPACE_EXTERNAL )
#else
# error "Error in configuration: Neither SEC_ENABLE_WORKSPACE_INTERNAL nor SEC_ENABLE_WORKSPACE_EXTERNAL set"
#endif

#endif /* __SEC_VERIFICATION_H__ */

/***********************************************************************************************************************
 *  END OF FILE: SEC_VERIFICATION.H
 **********************************************************************************************************************/

