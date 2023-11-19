/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        Configuration of the HIS security module
 *
 *  \description  Template for parameter values (e.g. cryptographic keys)
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
 *  02.03.00   2014-02-20  JHg     ESCAN00073562    No changes
 *                         Mss     ESCAN00073600    Added AES192 and AES256 keys
 *                         Mss     ESCAN00073811    No changes
 **********************************************************************************************************************/

#ifndef __SECMPAR_H__
#define __SECMPAR_H__

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "Sec.h"

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

#if defined( SEC_ENABLE_SECURITY_CLASS_CCC ) || \
    defined( SEC_ENABLE_SECURITY_CLASS_C )   || \
    defined( SEC_ENABLE_SECURITY_CLASS_AAA )
# define SECMPAR_START_SEC_DATA
# include "memmap.h"

# if defined( SEC_ENABLE_SECURITY_CLASS_CCC )
V_MEMROM0 extern V_MEMROM1 SecM_ByteType V_MEMROM2 SecM_RsaMod[SEC_SIZE_SIG_RSA];
V_MEMROM0 extern V_MEMROM1 SecM_ByteType V_MEMROM2 SecM_RsaExp[];
# endif /* SEC_ENABLE_SECURITY_CLASS_CCC */
# if defined( SEC_ENABLE_SECURITY_CLASS_C )
V_MEMROM0 extern V_MEMROM1 SecM_ByteType V_MEMROM2 kSecM_SecretKeyClassC[20];
# endif /* SEC_ENABLE_SECURITY_CLASS_C */
# if defined( SEC_ENABLE_SECURITY_CLASS_AAA )
#  if ( SEC_DECRYPTION_ALGORITHM == SEC_AES128 )
V_MEMROM0 extern V_MEMROM1 SecM_ByteType V_MEMROM2 SecM_AES128key[16];
#  elif( SEC_DECRYPTION_ALGORITHM == SEC_AES192 )
V_MEMROM0 extern V_MEMROM1 SecM_ByteType V_MEMROM2 SecM_AES192key[24];
#  elif( SEC_DECRYPTION_ALGORITHM == SEC_AES256 )
V_MEMROM0 extern V_MEMROM1 SecM_ByteType V_MEMROM2 SecM_AES256key[32];
#  endif /* SEC_DECRYPTION_ALGORITHM */
# endif /* SEC_ENABLE_SECURITY_CLASS_AAA */

# define SECMPAR_STOP_SEC_DATA
# include "memmap.h"
#endif /* SEC_ENABLE_SECURITY_CLASS_CCC || SEC_ENABLE_SECURITY_CLASS_C || SEC_ENABLE_SECURITY_CLASS_AAA */

#endif /* __SECMPAR_H__ */

/***********************************************************************************************************************
 *  END OF FILE: SECMPAR.H
 **********************************************************************************************************************/
