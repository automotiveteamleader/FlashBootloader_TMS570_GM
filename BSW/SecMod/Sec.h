/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        Implementation of the HIS security module
 *
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
 *                                                   Split into separate sub-packages
 *                                                   Renamed file name (was SecM.h)
 *                                                   Select API sub-set at integration time
 *  02.01.00   2013-11-08  JHg     ESCAN00071173    Added support for CRC, seed/key and verification APIs
 *  02.01.01   2013-11-08  JHg     ESCAN00071733    No changes
 *  02.01.02   2013-11-11  JHg     ESCAN00071785    No changes
 *  02.02.00   2013-12-19  Mss     ESCAN00072743    Adapted switch to support encryption API
 *  02.03.00   2014-02-20  JHg     ESCAN00073562    No changes
 *                         Mss     ESCAN00073600    No changes
 *                         Mss     ESCAN00073811    Fixed typo
 **********************************************************************************************************************/

#ifndef __SEC_H__
#define __SEC_H__

/**********************************************************************************************************************
 *  VERSION
 *********************************************************************************************************************/

/* ##V_CFG_MANAGEMENT ##CQProject : SysService_SecModHis CQComponent : Implementation */
# define SYSSERVICE_SECMODHIS_VERSION              0x0203u
# define SYSSERVICE_SECMODHIS_RELEASE_VERSION      0x00u

/** API compatible to HIS security module specification 1.1 */
# define SECM_HIS_SECURITY_MODULE_VERSION          0x010100ul

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

/* Security module configuration settings */
#include "Sec_Inc.h"

/* API dependent includes */
# include "Sec_Verification.h"

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/* Key timer not used, seed validity never expires */
# define  SecM_StartKeyTimer()
# define  SecM_StopKeyTimer()
# define  SecM_GetKeyTimer()     (1u)
# define  SecM_DecrKeyTimer()

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/** Parameter type passed to SecM_InitPowerOn */
typedef SecM_ConstVoidPtrType   SecM_InitType;

/** Pointer type for function calls from external application */
typedef void SEC_CALL_TYPE (* pSecTaskFct)( void );

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#if defined( __cplusplus )
extern "C" {
#endif

extern SecM_StatusType SecM_InitPowerOn( SecM_InitType initParam );
extern void SecM_Task( void );

#if defined( __cplusplus )
} /* extern "C" */
#endif

#endif /* __SEC_H__ */

/***********************************************************************************************************************
 *  END OF FILE: SEC.H
 **********************************************************************************************************************/
