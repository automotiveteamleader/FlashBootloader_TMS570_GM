/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        Crypto library
 *
 *  \description  Main header file of cv act library/ES by cryptovision GmbH.
 *                Additional header used to document changes implemented by Vector Informatik GmbH.
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
 *  Thorsten Albers               Al            Vector Informatik GmbH
 *  Armin Happel                  Hp            Vector Informatik GmbH
 *  Joern Herwig                  JHg           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  01.00.00   2013-06-20  Al      -                Renamed asn_1.h to ESLib_ASN_1.h
 *                                                   Protection against multiple inclusion added
 *                                                  Renamed types.h to ESLib_types.h
 *                         Hp      -                Added SHA256 DigestInfo to ESLib_ASN_1.h
 *                         JHg     ESCAN00068215    Initial release based on original cv act library/ES and
 *                                                   SysService_Crypto 01.04.01
 *                                                  Use base types from actPlatformTypes.h
 *                                                  Use compiler abstraction for ROM pointers from Vector platform definitions
 *  01.01.00   2013-09-26  JHg     ESCAN00070713    Added version information
 *                                                  Added API for separate hash finalization and signature verification
 *                                                  Added option to locate cryptographic keys in RAM instead of ROM
 *                                                  Include primitives for DES, T-DES and RC2
 *                                                  Maximum size of RSA keys configurable at integration-time
 *  01.01.01   2014-01-14  JHg     ESCAN00072935    No changes
 *  01.02.00   2014-02-11  JHg     ESCAN00073601    No changes
 *  01.03.00   2014-04-23  JHg     ESCAN00074877    Place ASN.1 digest info in global constants
 *  01.04.00   2014-04-28  JHg     ESCAN00075253    Added dedicated file for version information
 *  01.05.00   2014-05-26  JHg     ESCAN00075892    Added RSA decryption with public key (esl_decryptPubRSA_V15)
 *                                                  Refactored handling of RSA padding
 *  01.05.01   2014-09-04  JHg     ESCAN00077752    Corrected handling of public key padding
 **********************************************************************************************************************/

#ifndef __ESLib_version_H__
#define __ESLib_version_H__

/**********************************************************************************************************************
 *  VERSION
 *********************************************************************************************************************/

/* ##V_CFG_MANAGEMENT ##CQProject : SysService_CryptoCv CQComponent : Impl_ESLib */
#define SYSSERVICE_CRYPTOCV_ESLIB_VERSION             0x0105u
#define SYSSERVICE_CRYPTOCV_ESLIB_RELEASE_VERSION     0x01u

#endif /* __ESLib_version_H__ */

