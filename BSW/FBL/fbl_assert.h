/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        System specific assertion defines
 *
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
 *  Christian Baeuerle            CB            Vector Informatik GmbH
 *  Armin Happel                  Hp            Vector Informatik GmbH
 *  Andre Caspari                 Ci            Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  01.00.00   2007-08-02  Hp                       Creation
 *  01.01.00   2011-01-13  CB                       Added kFblSysAssertServiceMainHandlerInvalid and
 *                                                  kFblSysAssertRepeatedCallOnConcludedService
 *  01.02.00   2014-05-28  Ci                       Converted to normal include file
 *                                                  Added include for OEM specific assertion defines
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  DEFINITION OF ASSERTION CODES (SYSTEM)
 **********************************************************************************************************************/

/* Note: System assertions are only allowed in the range from 0x00-0x7F. Others are reserved. */

/* 0x00-0x1F: General FBL assertions */
#define kFblSysAssertMissingForcedRCRRP               0x00u

/* 0x20-0x3F: Internal assertions */
#define kFblSysAssertServiceReqBeforeResp             0x20u
#define kFblSysAssertServiceMainHandlerInvalid        0x21u

/* 0x40-0x5F: Generated data assertions */
#define kFblSysAssertWdBufferOverflow                 0x40u

/* 0x50-0x7F: User parameter assertions */
#define kFblSysAssertParameterOutOfRange              0x50u

#define kFblSysAssertRepeatedCallOnConcludedService   0x60u

/* 0x80-0xFF: OEM specific */
#include "fbl_assert_oem.h"

/***********************************************************************************************************************
 *  END OF FILE: FBL_ASSERT.H
 **********************************************************************************************************************/
