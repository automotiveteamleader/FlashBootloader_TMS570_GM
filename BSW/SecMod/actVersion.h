/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        Crypto library
 *
 *  \description  Main header file of actCLibrary by cryptovision GmbH.
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
 *  Joern Herwig                  JHg           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  01.00.00   2013-06-20  Al      -                Include Vector platform definitions
 *                         JHg     ESCAN00068214    Initial release based on original actCLibrary and
 *                                                   SysService_Crypto 01.04.01
 *                                                  File renamed from platform_t.h to actPlatformTypes.h
 *                                                  Moved from cv act library/ES
 *                                                  Declare base types defined by Vector platform definitions
 *                                                  Added ACT prefix to configuration settings
 *                                                  Use ULLONG_MAX from limits.h to detect availability of 64 bit type
 *                                                  Use compiler abstraction for ROM pointers from Vector platform definitions
 *                                                  Added additional watchdog triggers
 *  01.01.00   2013-09-26  JHg     ESCAN00070712    Added version information
 *                                                  Additional compiler abstraction
 *                                                  Added option to locate cryptographic keys in RAM instead of ROM
 *                                                  Include primitives for DES, T-DES and RC2
 *  01.02.00   2014-01-14  JHg     ESCAN00072933    Corrected ROM pointer cast in actECInit (actECTools.c)
 *  01.03.00   2014-01-20  JHg     ESCAN00073066    Corrected output length for symmetric stream decryption
 *  01.04.00   2014-02-11  JHg     ESCAN00073602    No changes
 *  01.04.01   2014-03-21  JHg     ESCAN00074421    Renamed certain single letter upper-case variables and members
 *  01.05.00   2014-04-23  JHg     ESCAN00074879    Added memcpy and memcmp variants for ROM to/with RAM
 *  01.06.00   2014-04-28  JHg     ESCAN00075252    Added dedicated file for version information
 *  01.06.01   2014-05-27  JHg     ESCAN00075506    Replaced down-cast in actGetByte by bit-mask
 **********************************************************************************************************************/

#ifndef ACTVERSION_H
#define ACTVERSION_H

/**********************************************************************************************************************
 *  VERSION
 *********************************************************************************************************************/

/* ##V_CFG_MANAGEMENT ##CQProject : SysService_CryptoCv CQComponent : Impl_actCLib */
#define SYSSERVICE_CRYPTOCV_ACTLIB_VERSION            0x0106u
#define SYSSERVICE_CRYPTOCV_ACTLIB_RELEASE_VERSION    0x01u

/*********************************************************************************************************************/

#endif /* ACTVERSION_H */

