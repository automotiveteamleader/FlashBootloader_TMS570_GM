/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        Implementation of the HIS security module - Common includes
 *
 *  \description  Contains all includes used by all security module sources
 *                Not to be included in other sources directly, include Sec.h instead
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
 *                                                   Renamed file name (was SecM_inc.h)
 *                                                   Conditional inclusion of FBL configuration
 *                                                   Removed API includes
 *  02.01.00   2013-11-08  JHg     ESCAN00071173    Only include configuration and parameters for CRC or verification API
 *  02.01.01   2013-11-08  JHg     ESCAN00071733    No changes
 *  02.01.02   2013-11-11  JHg     ESCAN00071785    Unconditionally include configuration and parameter settings
 *  02.02.00   2013-12-19  Mss     ESCAN00072743    No changes
 *  02.03.00   2014-02-20  JHg     ESCAN00073562    No changes
 *                         Mss     ESCAN00073600    No changes
 *                         Mss     ESCAN00073811    No changes
 **********************************************************************************************************************/

#ifndef __SEC_INC_H__
#define __SEC_INC_H__

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

/* Global configuration */
#include "v_cfg.h"

/* Global definitions */
#include "v_def.h"

#if defined( VGEN_ENABLE_CANFBL )
/* Bootloader types and definitions (e.g. SWM_DATA_MAX_NOAR) */
# include "fbl_cfg.h"
#endif /* VGEN_ENABLE_CANFBL */

/* Security module configuration */
# include "SecM_cfg.h"

/* Additional header files */
#include "Sec_Types.h"

/* Security module parameters */
# include "SecMPar.h"

#endif /* __SEC_INC_H__ */

/***********************************************************************************************************************
 *  END OF FILE: SEC_INC.H
 **********************************************************************************************************************/
