/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        Implementation of the HIS security module - CRC calculation
 *
 *  \description  Offers CRC calculation according to CRC-16 CCITT or CRC-32 (IEEE 802.3)
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
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  01.00.00   2013-06-20  JHg     ESCAN00068213    Initial release based on FblSecMod_Vector 02.02.00
 *  02.00.00   2013-11-08  JHg     ESCAN00071176    Major refactoring
 *                                                   Moved to separate sub-package
 *                                                   Renamed file name (was SecMCrc.h)
 *                                                   Added CRC-16 CCITT implementation
 *                                                   Remove exported function for CRC table generation
 *  02.00.01   2014-01-20  JHg     ESCAN00073016    No changes
 *  02.01.00   2014-02-10  JHg     ESCAN00073559    CRC algorithm selectable at compile time
 **********************************************************************************************************************/

#ifndef __SEC_CRC_H__
#define __SEC_CRC_H__

/***********************************************************************************************************************
 *   VERSION
 **********************************************************************************************************************/

/* ##V_CFG_MANAGEMENT ##CQProject : SysService_SecModHis CQComponent : Impl_Crc */
#define SYSSERVICE_SECMODHIS_CRC_VERSION            0x0201u
#define SYSSERVICE_SECMODHIS_CRC_RELEASE_VERSION    0x00u

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "Sec_Inc.h"

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/* Defines from SecM214 (V1.1) */
/** Initialize CRC calculation */
#define kCRCInit           0x01u
/** Update CRC value */
#define kCRCCompute        0x02u
/** Finalize CRC calculation */
#define kCRCFinalize       0x03u

/* Remap internal states */
#define SEC_CRC_NONE       0x00u
#define SEC_CRC_INIT       kCRCInit
#define SEC_CRC_COMPUTE    kCRCCompute
#define SEC_CRC_FINALIZE   kCRCFinalize

/*********************************************************************************************************************/

/* Defaults for configuration defines */

#if defined( SEC_CRC_TYPE )
#else
#  define SEC_CRC_TYPE     SEC_CRC32
#endif /* SEC_CRC_TYPE */

# if ( SEC_CRC_TYPE == SEC_CRC32 )
#  define SEC_ENABLE_CRC_TYPE_CRC32
# endif /* SEC_CRC32 */


/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/** Type for CRC value */
typedef SecM_WordType   SecM_CRCType;

/** Parameter structure for CRC computation */
typedef struct
{
   SecM_CRCType            currentCRC;       /**< Current CRC-value */
   SecM_ByteType           crcState;         /**< Operation state to be carried out */
   SecM_ConstRamDataType   crcSourceBuffer;  /**< Pointer to source data */
   SecM_LengthType         crcByteCount;     /**< Number of bytes in source buffer */
   FL_WDTriggerFctType     wdTriggerFct;     /**< Watchdog trigger function */
} SecM_CRCParamType;

/** Pointer type for API functions */
typedef SecM_StatusType SEC_CALL_TYPE (* pSecComputeCRCFct)( V_MEMRAM1 SecM_CRCParamType V_MEMRAM2 V_MEMRAM3 * crcParam );

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#if defined( __cplusplus )
extern "C" {
#endif


#if defined( __cplusplus )
} /* extern "C" */
#endif

#endif /* __SEC_CRC_H__ */

/***********************************************************************************************************************
 *  END OF FILE: SEC_CRC.H
 **********************************************************************************************************************/
