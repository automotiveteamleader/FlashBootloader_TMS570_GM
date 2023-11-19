/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        OEM specific assertion defines (Gm)
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
 *  Andreas Wenckebach            AWh            Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  05.00.00   2014-06-03  AWh                      Creation
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  DEFINITION OF ASSERTION CODES (OEM)
 **********************************************************************************************************************/

/* Note: OEM assertions are only allowed in the range from 0x80-0xFF. Others are reserved. */

/* 0x80-0x9F: General FBL assertions */
#define kFblAssertVariableInValidState             0x80u
#define kFblAssertParameterValid                   0x81u
#define kFblAssertConditionCorrect                 0x82u

/* 0xA0-0xBF: Internal assertions */
#define kFblAssertInternalIlegalState              0xA0u
#define kFblAssertInternalApplFblCorrectErrSetting 0xA1u

/* 0xC0-0xDF: Generated data assertions */
#define kFblAssertAppl1NotFirstBlock               0xC0u

/* 0xE0-0xFF: User parameter assertions */
#define kFblAssertApplFblGetMdlHdrAddrCfgrd        0xE0u

/***********************************************************************************************************************
 *  END OF FILE: FBL_ASSERT_OEM.H
 **********************************************************************************************************************/
