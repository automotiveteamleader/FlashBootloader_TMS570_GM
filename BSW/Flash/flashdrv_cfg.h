/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief         Flash Driver Configuration for the TMS470
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
 *  Johannes Krimmel              KJs           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  01.07.00   2015-02-12  KJs                      Creation
 **********************************************************************************************************************/

#ifndef _FBLDRVFLASH_TMS470F02XHIS_CFG_H_
#define _FBLDRVFLASH_TMS470F02XHIS_CFG_H_

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/
 
/* Define only if  Fapi_getBankSectors() API call is not working for your derivative
 * 
 * #define FLASH_ENABLE_CUSTOM_SECTOR_SETUP
 */ 
 
#if defined( FLASH_ENABLE_CUSTOM_SECTOR_SETUP )
/* Use this define to specify the flash sector layout of your derivative.
 * Specifying a static layout is only necessary if the Fapi_getBankSectors() API call returns incorrect information.
 * The layout below is only an example and needs to be adapted for your derivative.
 *
 * Every flash sector entry consists of:
 *  <start address of flash sector>, <length>, <Flash bank number>, <Sector number>                */

# define ERASE_SECTOR_MAP_INITIALIZER                                               \
      {                                                                             \
      { 0x00000000, 0x00004000, (Fapi_FlashBankType)0, (Fapi_FlashSectorType)0 },   \
      { 0x00004000, 0x00004000, (Fapi_FlashBankType)0, (Fapi_FlashSectorType)1 },   \
      { 0x00008000, 0x00008000, (Fapi_FlashBankType)0, (Fapi_FlashSectorType)2 },   \
      { 0x00010000, 0x00008000, (Fapi_FlashBankType)0, (Fapi_FlashSectorType)3 },   \
      { 0x00018000, 0x00008000, (Fapi_FlashBankType)0, (Fapi_FlashSectorType)4 },   \
      { 0x00020000, 0x00020000, (Fapi_FlashBankType)0, (Fapi_FlashSectorType)5 },   \
      { 0x00040000, 0x00020000, (Fapi_FlashBankType)0, (Fapi_FlashSectorType)6 },   \
      { 0x00060000, 0x00020000, (Fapi_FlashBankType)0, (Fapi_FlashSectorType)7 },   \
      { 0x00080000, 0x00040000, (Fapi_FlashBankType)1, (Fapi_FlashSectorType)0 },   \
      { 0x000C0000, 0x00040000, (Fapi_FlashBankType)1, (Fapi_FlashSectorType)1 },   \
      { 0x00100000, 0x00040000, (Fapi_FlashBankType)1, (Fapi_FlashSectorType)2 },   \
      { 0x00140000, 0x00040000, (Fapi_FlashBankType)1, (Fapi_FlashSectorType)3 },   \
      { 0x00180000, 0x00040000, (Fapi_FlashBankType)1, (Fapi_FlashSectorType)4 },   \
      { 0x001C0000, 0x00040000, (Fapi_FlashBankType)1, (Fapi_FlashSectorType)5 },   \
      { 0xF0200000, 0x00004000, (Fapi_FlashBankType)7, (Fapi_FlashSectorType)0 },   \
      { 0xF0204000, 0x00004000, (Fapi_FlashBankType)7, (Fapi_FlashSectorType)1 }    \
      }
#endif

#endif /* _FBLDRVFLASH_TMS470F02XHIS_CFG_H_ */

/***********************************************************************************************************************
 *  END OF FILE: FLASHDRV_CFG.H
 **********************************************************************************************************************/
 