/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        Implementation of the HIS security module - Type definitions
 *
 *  \description  Defines types, constants and configuration switches used by the security module
 *  -------------------------------------------------------------------------------------------------------------------
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \par Copyright
 *  \verbatim
 *  Copyright (c) 2014 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 */
/*********************************************************************************************************************/

/***********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  --------------------------------------------------------------------------------------------------------------------
 *  Joern Herwig                  JHg           Vector Informatik GmbH
 *  Markus Schneider              Mss           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  01.00.00   2013-06-20  JHg     ESCAN00068213    Initial release based on FblSecMod_Vector 02.02.00
 *  02.00.00   2013-10-01  JHg     ESCAN00070689    Major refactoring
 *                                                   Renamed file name (was SecM_def.h)
 *                                                   Introduction of supporting types for easier handling
 *                                                   Added types for extended functionality (workspace and key handling)
 *                                                   Defaults and remapping of configuration switches
 *  02.01.00   2013-11-08  JHg     ESCAN00071173    Added support for CRC, seed/key and verification APIs
 *  02.01.01   2013-11-08  JHg     ESCAN00071733    No changes
 *  02.01.02   2013-11-11  JHg     ESCAN00071785    No changes
 *  02.02.00   2013-12-19  Mss     ESCAN00072743    No changes
 *  02.03.00   2014-02-20  JHg     ESCAN00073562    CRC algorithm selectable at compile time
 *                         Mss     ESCAN00073600    Decryption algorithm selectable at compile time
 *                         Mss     ESCAN00073811    No changes
 **********************************************************************************************************************/

#ifndef __SECM_TYPES_H__
#define __SECM_TYPES_H__

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "Sec_Inc.h"

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/* Defaults for configuration defines */


#if defined( SEC_WATCHDOG_CYCLE_COUNT )
#else
/** Watchdog triggered every n-th cycle (has to 2^m - 1) */
# define SEC_WATCHDOG_CYCLE_COUNT   (0x1Fu)
#endif

#if defined( SEC_ENABLE_WORKSPACE_INTERNAL ) || \
    defined( SEC_ENABLE_WORKSPACE_EXTERNAL )
#else
/** Use internal default workspaces */
# define SEC_ENABLE_WORKSPACE_INTERNAL
#endif /* SEC_ENABLE_WORKSPACE_INTERNAL || SEC_ENABLE_WORKSPACE_EXTERNAL */

#if defined( SEC_CHECKSUM_TYPE_CRC )
#else
/** Use CRC algorithm for checksum verification (security class DDD) */
# define SEC_CHECKSUM_TYPE_CRC      0u
#endif /* SEC_CHECKSUM_TYPE_CRC */
#if defined( SEC_CHECKSUM_TYPE_HASH )
#else
/** Use hash algorithms for checksum verification (security class DDD) */
# define SEC_CHECKSUM_TYPE_HASH     (SEC_CHECKSUM_TYPE_CRC + 1u)
#endif /* SEC_CHECKSUM_TYPE_HASH */

/** Algorithm identifier for SHA-256 */
#if defined( SEC_SHA256 )
#else
# if (SEC_SHA1 > SEC_RIPEMD160)
#  define SEC_SHA256    (SEC_SHA1 + 1u)
# else
#  define SEC_SHA256    (SEC_RIPEMD160 + 1u)
# endif
#endif /* SEC_SHA256 */

#if defined( SEC_CRC32 )
#else
/** Use CRC-32 algorithm for checksum calculation */
# define SEC_CRC32         0u
#endif /* SEC_CRC32 */
#if defined( SEC_CRC16_CCITT )
#else
/** Use CRC-16 CCITT algorithm for checksum calculation */
# define SEC_CRC16_CCITT   (SEC_CRC32 + 1u)
#endif /* SEC_CRC16_CCITT */

#if defined( SEC_AES128 )
#else
/** Use AES128 algorithm for decryption / encryption */
# define SEC_AES128        0u
#endif /* SEC_AES128 */
#if defined( SEC_AES192 )
#else
/** Use AES192 algorithm for decryption / encryption */
# define SEC_AES192        (SEC_AES128 + 1u)
#endif /* SEC_AES192 */
#if defined( SEC_AES256 )
#else
/** Use AES256 algorithm for decryption / encryption */
# define SEC_AES256        (SEC_AES192 + 1u)
#endif /* SEC_AES256 */

/*********************************************************************************************************************/

/* Return codes */

/** Positive return code */
#define SECM_OK      0x00u
/** Negative return code */
#define SECM_NOT_OK  0xFFu

#if defined( SEC_CALL_TYPE )
#else
/** Qualifier for function pointer declarations */
# define SEC_CALL_TYPE
#endif /* SEC_CALL_TYPE */

/** Null pointer for RAM data */
#define SEC_RAM_DATA_NULL              ((SecM_RamDataType)V_NULL)
/** Pointer to indicate use of internal default workspace */
#define SEC_DEFAULT_WORKSPACE          ((SecM_WorkspacePtrType)V_NULL)
/** Size to indicate use of internal default workspace */
#define SEC_DEFAULT_WORKSPACE_SIZE     0u
/** Size to indicate use of default key (e.g. for decryption) */
#define SEC_DEFAULT_KEY_SIZE           0u

/*********************************************************************************************************************/

/* Watchdog handling */

/** Empty watchdog trigger function pointer */
#define SEC_WATCHDOG_NONE              ((FL_WDTriggerFctType)0u)

/** Trigger watchdog
 *  Watchdog is disabled in case null pointer is passed */
#define SEC_WATCHDOG_TRIGGER(function)       \
   do                                        \
   {                                         \
      if ((SEC_WATCHDOG_NONE) != (function)) \
      {                                      \
         function();                         \
      }                                      \
   } while(0) /* Prevents empty statement (";" on its own) */

/** Trigger watchdog every n-th cycle */
#define SEC_WATCHDOG_CYCLE_TRIGGER(function, cycle)         \
   do                                                       \
   {                                                        \
      if (((cycle) & (SEC_WATCHDOG_CYCLE_COUNT)) == 0x00u)  \
      {                                                     \
         SEC_WATCHDOG_TRIGGER(function);                    \
      }                                                     \
   } while(0) /* Prevents empty statement (";" on its own) */

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/* Supporting data types for easier handling */

/** Data type for byte values (8 bit)*/
typedef vuint8  SecM_ByteType;
/** Data type for short values (16 bit) */
typedef vuint16 SecM_ShortType;

/** Pointer type to byte data in RAM */
typedef V_MEMRAM1 SecM_ByteType V_MEMRAM2 V_MEMRAM3 *       SecM_RamDataType;
/** Pointer type to constant byte data in RAM */
typedef const V_MEMRAM1 SecM_ByteType V_MEMRAM2 V_MEMRAM3 * SecM_ConstRamDataType;
/** Pointer type to byte data in ROM */
typedef V_MEMROM1 SecM_ByteType V_MEMROM2 V_MEMROM3 *       SecM_RomDataType;
/** Void pointer type */
typedef V_MEMRAM1 void V_MEMRAM2 V_MEMRAM3 *                SecM_VoidPtrType;
/** Void pointer type (const) */
typedef const V_MEMRAM1 void V_MEMRAM2 V_MEMRAM3 *          SecM_ConstVoidPtrType;

/*********************************************************************************************************************/

/* Types specified by HIS specification */

/** Data type for status codes */
typedef SecM_ByteType SecM_StatusType;
/** Data type for word values (32 bit) */
typedef vuint32 SecM_WordType;
/** Data type for length information */
typedef SecM_ShortType SecM_LengthType;

/** Data type for address information */
typedef SecM_WordType SecM_AddrType;
/** Data type for size information */
typedef SecM_WordType SecM_SizeType;

/** Description of a download segment */
typedef struct
{
   SecM_AddrType transferredAddress;
   SecM_AddrType targetAddress;
   SecM_SizeType length;
} FL_SegmentInfoType;

/** List of downloaded segments */
typedef struct
{
   /** Number of segments in list */
   SecM_ByteType        nrOfSegments;
   /** List of segments */
   /* According to HIS specification */
   V_MEMRAM1 FL_SegmentInfoType V_MEMRAM2 V_MEMRAM3 * segmentInfo;
} FL_SegmentListType;

/** Pointer to watchdog trigger function */
typedef void (* FL_WDTriggerFctType)( void );
/** Pointer to memory read function */
typedef SecM_SizeType (* FL_ReadMemoryFctType)( SecM_AddrType, SecM_RamDataType, SecM_SizeType );

/*********************************************************************************************************************/

/* Fast types (used for loops) */

#if defined( C_CPUTYPE_8BIT )
/** Fast type which can hold at least a byte */
typedef SecM_ByteType   SecM_ByteFastType;
/** Fast type which can hold at least a short */
typedef SecM_ShortType  SecM_ShortFastType;
#elif defined( C_CPUTYPE_16BIT )
typedef SecM_ShortType  SecM_ByteFastType;
typedef SecM_ShortType  SecM_ShortFastType;
#elif defined( C_CPUTYPE_32BIT )
typedef SecM_WordType   SecM_ByteFastType;
typedef SecM_WordType   SecM_ShortFastType;
#endif /* C_CPUTYPE */

/** Fast type which can hold length information */
typedef SecM_ShortFastType SecM_LengthFastType;

/*********************************************************************************************************************/

/* Buffer types */

/** Information of buffer located in RAM */
typedef struct
{
   /** Pointer to data */
   SecM_ConstRamDataType   data;
   /** Buffer size given in byte */
   SecM_LengthType         size;
} SecM_RamBufferType;

/** Information of buffer located in ROM */
typedef struct
{
   /** Pointer to data */
   SecM_RomDataType  data;
   /** Buffer size given in byte */
   SecM_LengthType   size;
} SecM_RomBufferType;

/*********************************************************************************************************************/

/* Cryptographic key types */

/** Reference to cryptographic key */
#if defined( SEC_ENABLE_KEY_LOCATION_RAM )
typedef SecM_RamBufferType SecM_CryptKeyType;
#else
typedef SecM_RomBufferType SecM_CryptKeyType;
#endif /* SEC_ENABLE_KEY_LOCATION_RAM */

/** Reference to symmetric cryptographic key (always located in RAM) */
typedef SecM_RamBufferType SecM_SymKeyType;

/** Reference to asymmetric cryptographic key pair */
typedef struct
{
   /** Shared key */
   SecM_CryptKeyType shared;
   /** Individual (public) key */
   SecM_CryptKeyType individual;
} SecM_AsymKeyType;

/*********************************************************************************************************************/

/* Workspace types */

/** Basic workspace structure used for aligned pointer type */
typedef struct
{
   SecM_ByteType  digest[1];  /**< Placeholder for message digest located at beginning of every workspace */
   SecM_WordType  dummy;      /**< Dummy member to enforce alignment, not actually used */
} SecM_BasicWorkspaceType;

/** Pointer to aligned workspace */
typedef V_MEMRAM1 SecM_BasicWorkspaceType V_MEMRAM2 V_MEMRAM3 * SecM_WorkspacePtrType;

/** Reference to workspace */
typedef struct
{
   /** Pointer to data */
   SecM_WorkspacePtrType   data;
   /** Buffer size given in byte */
   SecM_LengthType         size;
} SecM_WorkspaceType;

#endif /* __SECM_TYPES_H__ */

/***********************************************************************************************************************
 *  END OF FILE: SEC_TYPES.H
 **********************************************************************************************************************/
