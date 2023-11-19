/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        Implementation of the verification component of the HIS security module - Default workspaces
 *
 *  \description  Defines the default workspaces used by the library parts for verification, seed/key and decryption
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
 *  02.00.00   2013-10-01  JHg     ESCAN00070691    Major refactoring
 *                                                   Moved to separate sub-package
 *                                                   Select available workspaces at integration-time
 *                                                   Export pointer and size of workspaces instead workspaces directly
 *  02.01.00   2013-11-08  JHg     ESCAN00071174    Added support for seed/key and verification (filter settings)
 *  02.02.00   2013-12-19  Mss     ESCAN00072741    Added support for AES encryption
 *  02.03.00   2014-02-12  Mss     ESCAN00073596    Added support for AES192 and AES256
 *  02.03.01   2014-05-26  JHg     ESCAN00075254    Fixed case of MemMap.h
 **********************************************************************************************************************/

#ifndef __SEC_WORKSPACE_H__
#define __SEC_WORKSPACE_H__

/**********************************************************************************************************************
 *  VERSION
 *********************************************************************************************************************/

/* ##V_CFG_MANAGEMENT ##CQProject : SysService_SecModHis CQComponent : Impl_Workspace */
#define SYSSERVICE_SECMODHIS_WORKSPACE_VERSION            0x0203u
#define SYSSERVICE_SECMODHIS_WORKSPACE_RELEASE_VERSION    0x01u

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "Sec_Inc.h"

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

#define SECMWS_START_SEC_DATA_EXPORT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


V_MEMROM0 extern V_MEMROM1 SecM_WorkspacePtrType   V_MEMROM2 secWorkSpacePtrHashSha256;
V_MEMROM0 extern V_MEMROM1 SecM_LengthType         V_MEMROM2 secWorkSpaceSizeHashSha256;


V_MEMROM0 extern V_MEMROM1 SecM_WorkspacePtrType   V_MEMROM2 secWorkSpacePtrSigSha256;
V_MEMROM0 extern V_MEMROM1 SecM_LengthType         V_MEMROM2 secWorkSpaceSizeSigSha256;


#define SECMWS_STOP_SEC_DATA_EXPORT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* __SEC_WORKSPACE_H__ */

/***********************************************************************************************************************
 *  END OF FILE: SEC_WORKSPACE.H
 **********************************************************************************************************************/

