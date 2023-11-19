/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief         ID-based access for reading and writing non-volatile memory
 *
 *  --------------------------------------------------------------------------------------------------------------------
 *  COPYRIGHT
 *  --------------------------------------------------------------------------------------------------------------------
 *  \par Copyright
 *  \verbatim
 *  Copyright (c) 2012 by Vector Informatik GmbH.                                                  All rights reserved.
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
 *  Armin Happel                  Hp            Vector Informatik GmbH
 *  Joern Herwig                  JHg           Vector Informatik GmbH
 *  Christian Baeuerle            CB            Vector Informatik GmbH
 *  Achim Strobelt                Ach           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  01.00.00   2009-07-08  CB                       Initial implementation
 *  01.01.00   2012-01-26  Ach     ESCAN00056279    Adapt for usage with GENy DLL
 *  01.02.00   2012-11-29  Ach     ESCAN00062931    Adapted comments to use Doxygen
 **********************************************************************************************************************/

#ifndef __WRAPPERNV_H__
#define __WRAPPERNV_H__

/***********************************************************************************************************************
 *  VERSION
 **********************************************************************************************************************/

/* ##V_CFG_MANAGEMENT ##CQProject : SysService_WrapperNv CQComponent : Implementation */
#define SYSSERVICE_WRAPPERNV_VERSION              0x0102
#define SYSSERVICE_WRAPPERNV_RELEASE_VERSION      0x00

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "WrapNv_cfg.h"

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

#define NV_MK_STRUCT_ID(structure, id)             ((((vuint32)(structure))<<24u)|(id))
#define NV_GET_STRUCT_ID(id)                       ((id)&0x00FFFFFFul)
#define NV_GET_STRUCT_HANDLE(id)                   (((id)&0xFF000000ul)>>24u)

/*******************************************************************************
* TYPDEFS AND STRUCTURES FOR GLOBAL USE
*******************************************************************************/

/* ID and length specifier for a record */
typedef vuint32 tNvRecId;
typedef vuint32 tNvRecLen;

#if defined( FBL_ENABLE_EEPMGR )
#else
# if defined( NV_ENABLE_ADDRESS_LOOKUP )
/** Type for address lookup table */
typedef struct tNvRecordTbl {
   tNvRecId    recId;                   /**< Id for NV memory item      */
   vuint8      recAccess;               /**< Single, Listed, Structured */
   vuint32     recAddress;              /**< start address in memory    */
   vuint8      recSize;                 /**< Number of data bytes       */
} tNvRecordTbl;
# endif /* NV_ENABLE_ADDRESS_LOOKUP */
#endif /* FBL_ENABLE_EEPMGR */

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

#if defined( FBL_ENABLE_EEPMGR )
#else
# if defined( NV_ENABLE_ADDRESS_LOOKUP )
V_MEMROM0 extern V_MEMROM1_FAR vuint8 V_MEMROM2_FAR kNvNoOfNvRecords;
V_MEMROM0 extern V_MEMROM1_FAR tNvRecordTbl V_MEMROM2_FAR kNvRecordTbl[];
# endif /* NV_ENABLE_ADDRESS_LOOKUP */
#endif /* FBL_ENABLE_EEPMGR */

/* Configuration plausibility check */
#if defined( FBL_ENABLE_EEPMGR) && defined (NV_ENABLE_ADDRESS_LOOKUP )
# error "Error in WrapNv_cfg.h: Address lookup table not supported for EEPROM manager"
#endif
#if !defined( FBL_ENABLE_EEPMGR) && !defined (NV_ENABLE_ADDRESS_LOOKUP )
# error "Error in WrapNv_cfg.h: Address lookup table required"
#endif

#endif /* __WRAPPERNV_H__ */

/***********************************************************************************************************************
 *  END OF FILE: WRAPNV.H
 **********************************************************************************************************************/
