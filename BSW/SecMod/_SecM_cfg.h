/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        Configuration of the HIS security module
 *
 *  \description  Template for configuration settings
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
 *  02.01.02   2013-11-11  JHg     ESCAN00071785    Added file
 *  02.02.00   2013-12-19  Mss     ESCAN00072743    No changes
 *  02.03.00   2014-02-20  JHg     ESCAN00073562    CRC algorithm selectable at compile time
 *                         Mss     ESCAN00073600    Decryption algorithm selectable at compile time
 *                         Mss     ESCAN00073811    No changes
 **********************************************************************************************************************/

#ifndef __SECM_CFG_H__
#define __SECM_CFG_H__

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

#define SEC_CLASS_DDD                        0u
#define SEC_CLASS_C                          1u
#define SEC_CLASS_CCC                        2u
#define SEC_CLASS_VENDOR                     3u
#define SEC_DEVELOPMENT                      1u
#define SEC_PRODUCTION                       2u
#define SEC_ADDRESS                          1u
#define SEC_FILE                             2u
#define SEC_CRC_SPEED_OPTIMIZED              0u
#define SEC_CRC_SIZE_OPTIMIZED               1u
#define SEC_SECURITY_CLASS                   SEC_CLASS_DDD
#define SEC_ECU_KEY                          0x00u
#define SEC_KEY_TIMEOUT                      100u
#define SEC_CALL_CYCLE                       10u
#define SEC_MEMORY_ACCESS                    SEC_ADDRESS
#define SEC_MODE                             SEC_PRODUCTION
#define SEC_CRC_OPT                          SEC_CRC_SPEED_OPTIMIZED
#define SEC_DISABLE_CRC_TOTAL
#define SEC_RIPEMD160                        0u
#define SEC_SHA1                             1u
#define SEC_SHA256                           2u
#define SEC_HASH_ALGORITHM                   SEC_SHA1
#define SEC_AES128                           0u
#define SEC_AES192                           1u
#define SEC_AES256                           2u
#define SEC_DECRYPTION_ALGORITHM             SEC_AES128
#define SEC_CRC32                            0u
#define SEC_CRC16_CCITT                      1u
#define SEC_CRC_TYPE                         SEC_CRC32
#define SEC_VERIFY_BYTES                     64u
#define SEC_DISABLE_DECRYPTION
#define SEC_DISABLE_ENCRYPTION
#define SEC_DISABLE_VERIFICATION_ADDRESS_LENGTH
#define SEC_VER_SIG_OFFSET                   0u
#define SEC_VER_CRC_OFFSET                   0u

#endif /* __SECM_CFG_H__ */

/***********************************************************************************************************************
 *  END OF FILE: SECM_CFG.H
 **********************************************************************************************************************/
