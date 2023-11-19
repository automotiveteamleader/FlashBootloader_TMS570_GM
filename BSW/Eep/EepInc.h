/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief         EEPROM driver include file
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
 *  Konrad Lazarus                Ls            Vector Informatik GmbH
 *  Marco Wierer                  WM            Vector Informatik GmbH
 *  Andreas Pick                  Ap            Vector Informatik GmbH
 *  Quetty Palacios               Qps           Vector Informatik GmbH
 *  Florian Hees                  FHe           Vector Informatik GmbH
 *  Joern Herwig                  JHg           Vector Informatik GmbH
 *  Andre Caspari                 Ci            Vector Informatik GmbH
 *  Marcel Viole                  MVi           Vector Informatik GmbH
 *  Achim Strobelt                Ach           Vector Informatik GmbH
 *  Alexander Starke              ASe           Vector Informatik GmbH
 *  Sven Ruess                    SvR           Vector Informatik GmbH
 *  Thomas Bezold                 TBe           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  01.00.00   2004-07-26  WM                       Implementation
 *  01.01.00   2006-04-13  Ls                       FBL_CLIENT_PAG_SLP3 added
 *  01.02.00   2006-07-31  JHg                      Added kEepFblBaseAddress for combination
 *                                                  PAG SLP3, dummy EEPROM driver and Tricore
 *                         FHe                      VAG SLP2, VECTOR SLP3 Support
 *  01.03.00   2006-08-29  Ci      ESCAN00017468    Added include for TP configuration
 *  01.04.00   2007-05-30  FHe     ESCAN00020931    VAG SLP2 renamed to SLP3
 *  01.05.00   2008-02-11  Ci      ESCAN00024041    Moved inclusion of TP configuration to EepMap.h
 *  01.06.00   2008-03-07  JHg     ESCAN00025270    Added support for Volvo AB
 *                         Hp                       Include fbl_def.h and EepIO.h to use EepInc.h only.
 *  01.07.00   2008-07-25  JHg                      Remove special case for Volvo AB
 *                         AWh                      Added C_COMP_COSMIC_STM8A_BECAN3
 *  01.08.00   2008-10-21  JHg                      Changed base address for TriCore TC1797
 *  01.09.00   2009-03-03  FHe                      Improved support for VAG SLP1
 *  01.10.00   2010-11-08  JHg                      Added support for TC1767 and TC1768/TC1387
 *                         MVi     ESCAN00046619    Added support for TC1782
 *                         QPs     ESCAN00046630    FBL_CLIENT_GAC_SLP1 added
 *  01.11.00   2010-12-17  MVi                      Removed EepMap include for HMC
 *  01.12.00   2011-03-05  Ach     ESCAN00049121    Support Infineon TriCore
 *  01.13.00   2012-02-20  QPs     ESCAN00056265    Remove EepMap when using generated
 *                                                  NV-Wrapper interface
 *  01.14.00   2012-05-03  ASe     ESCAN00058661    Reworked to support WrapperNv
 *  01.15.00   2012-11-29  ASe     ESCAN00062912    Adapted comments to use Doxygen
 *  01.39.00   2014-04-30  ASe     ESCAN00075336    Invalid case for EepIO.h include
 *                                                  Harmonized version with EepIO.c / EepIO.h
 *  01.40.00   2014-05-12  Svr     ESCAN00075517    No changes
 *  01.41.00   2014-06-12  TBe     ESCAN00075967    No changes
 **********************************************************************************************************************/

#ifndef __EEP_INC_H__
#define __EEP_INC_H__

#include "fbl_def.h"
#include "fbl_wd.h"

# include "WrapNv_cfg.h"

#include "EepIO.h"

#endif /* __EEP_INC_H__ */

/***********************************************************************************************************************
 *  END OF FILE: EEPINC.H
 **********************************************************************************************************************/
