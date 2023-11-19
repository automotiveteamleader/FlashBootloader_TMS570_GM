/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief         Include File
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
 *  Christian Baeuerle            Be            Vector Informatik GmbH
 *  Ralf Fritz                    Fz            Vector Informatik GmbH
 *  Armin Happel                  Hp            Vector Informatik GmbH
 *  Konrad Lazarus                Ls            Vector Informatik GmbH
 *  Marco Wierer                  WM            Vector Informatik GmbH
 *  Alexandre C. Plombin          ACP           Vector CANtech, Inc.
 *  Ahmad Nasser                  AMN           Vector CANtech, Inc.
 *  Markus Holzer                 MHz           Vector Informatik GmbH
 *  Andreas Pick                  Ap            Vector Informatik GmbH
 *  Ben Erickson                  BJE           Vector CANtech, Inc.
 *  Michael F. Radwick            MFR           Vector CANtech, Inc.
 *  Slawomir Wilk                 swk           Vector Informatik GmbH
 *  Florian Hees                  FHe           Vector Informatik GmbH
 *  Quetty Palacios               QPs           Vector Informatik GmbH
 *  Joern Herwig                  JHg           Vector Informatik GmbH
 *  Andre Caspari                 Ci            Vector Informatik GmbH
 *  Marcel Viole                  MVi           Vector Informatik GmbH
 *  Achim Strobelt                Ach           Vector Informatik GmbH
 *  Andreas Wenckebach            AWh           Vector Informatik GmbH
 *  Thomas Bezold                 TBe           Vector Informatik GmbH
 *  Sven Ruess                    SvR           Vector Informatik GmbH
 *  Alexander Starke              ASe           Vector Informatik GmbH
 *  Ralf Haegenlaeuer             HRf           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  02.00.00   2004-04-04  ACP                      New OEM
 *                         WM                       Minor changes for CLIENT_DC_UDS
 *  02.01.00   2004-05-03  WM                       Added include of iotypes.h
 *  02.02.00   2004-06-03  Fz                       Minor changes for CLIENT_RENAULT
 *  02.03.00   2004-07-01  WM                       Minor changes for CLIENT_DC_UDS
 *                         Ls                       Minor changes for CLIENT_PAG
 *  02.04.00   2004-07-14  MHz                      Added support for MB90340G
 *                         MHz                      Changed inclusion order of can_inc.h and fbl_def.h
 *                                                  to avoid double definition of memory qualifier
 *  02.05.00   2004-07-23  Ap                       Remove of fbl_sfr.h for NEC V850
 *  02.06.00   2004-08-19  Hp                       Changing include of FBL_APNV.H from CLIENT_FORD
 *                                                  to CLIENT_FORD_GGDS
 *  02.07.00   2004-08-23  WM                       Changed include order for CLIENT_CLAAS
 *  02.08.00   2004-09-08  Ap                       Encapsulation of fbl_mio.h removed
 *  02.09.00   2004-10-25  Ap                       Support for Vector standard components
 *  02.10.00   2004-11-12  MHz                      Support for new deriavte M32R 172/173 added
 *  02.11.00   2005-01-03  AMN                      Converted FBL_CLIENT_GGDS to FBL_CLIENT_FORD_GGDS
 *                         CB                       New OEM
 *  02.12.00   2005-01-25  WM                       Minor changes for CLIENT_DC
 *                                                  Added support for ST10F280
 *  02.13.00   2005-05-11  Hp                       Added support for V85x/IAR
 *                         Rr                       Added new OEM
 *  02.14.00   2005-05-22  BJE                      Added support for MB90F025
 *  02.15.00   2005-08-03  MFR                      Change FBL_CLIENT_GM support to split diagnostic callbacks
 *                         WM                       Support for C_COMP_DIABDATA_MPC55XX added
 *                         Mhz                      Minor changes for C_CLIENT_VECTOR
 *  02.16.00   2005-08-10  AMN                      Added include to v_ver.h
 *  02.17.00   2005-09-09  MFR                      Add FBL_CLIENT_GM_SLP1 and FBL_CLIENT_GM_SLP2
 *                         Ls                       FBL_ENABLE_MULTIPLE_MEM_DEVICES removed for PAG
 *  02.18.00   2005-09-26  Mhz                      Minor change due to renaming of dummy EEPROM
 *                                                  driver (only FORD GGDS)
 *  02.19.00   2005-11-09  MFR                      Minor changes to conditional compilation
 *                                                  switches used to include fbl_apdi.h in GM.
 *  02.20.00   2005-11-16  AMN                      Added include to EepInc.h to avoid compiler errors
 *  02.21.00   2005-11-28  swk                      Added FBL_CLIENT_VAG_COMMENT
 *  02.22.00   2006-01-11  Hp                       Add support for FJ16LX-420 and 420G in general (Geny-style define)
 *                         FHe                      Minor changes for FBL_CLIENT_VECTOR
 *  02.23.00   2006-03-29  Hp                       Add support for PAG_SLP3
 *                         Ls                       Minor changes for PAG_SLP3
 *                         QPs                      Added include of fbl_diagGW.h to support Gateway for Ford I3 Bootloader
 *  02.24.00   2006-04-06  QPs                      Added include of EepIO.h for FJ16LX and FBL_RENAULT
 *                         Ls                       Minor changes for PAG_SLP3
 *                         CB                       EEPROM driver includes added
 *  02.25.00   2006-06-30  FHe                      Added support for PAG_SLP3, VECTOR_SLP3, VAG_SLP2
 *  02.26.00   2006-07-04  FHe                      Changed position of SecM.h include for PAG
 *  02.27.00   2006-07-26  FHe                      Added SecM.h include for VAG SLP2 and VECTOR SLP3
 *  02.28.00   2006-08-18  Hp                       Add support for GM SLP3
 *                         FHe     ESCAN00017151    Adding support for Avccore
 *                         JHg                      Added support for new PPC hardware includes
 *  02.29.00   2006-09-12  QPs                      Added support for FJ16FX, OEM/Renault
 *  02.30.00   2006-11-07  QPs                      Added support for C_COMP_GAIO_MPC55XX
 *  02.31.00   2006-11-23  Ci                       Added MC9S12 EEPROM driver support for Ford SLP3
 *                         Cb      ESCAN00018080    New OEM
 *  02.32.00   2007-01-25  Hp      ESCAN00018814    Add support for HMC
 *                                                  Add optional support for CCP
 *                         Ci                       Added fbl_wd.h include for watchdog support
 *                         QPs                      Added support for FJ16LX-MB90920(G)
 *  02.33.00   2007-03-09  JHg     ESCAN00019721    Unconditionally include fbl_diag.h
 *                                                  ftp_cfg.h inclusion now before fbl_hw.h
 *                                                  Added version defines
 *  02.34.00   2007-05-30  FHe     ESCAN00020931    VAG SLP2 renamed to SLP3
 *  02.35.00   2007-07-24  Ci                       Added support for C_COMP_IAR_AVR_CANARY
 *                         Cb                       Minor changes
 *  02.36.00   2007-10-17  FHe                      Added EEPROM includes for VAG
 *  02.37.00   2007-11-22  JHg     ESCAN00023241    Added support for XC2000
 *  02.38.00   2008-02-26  CB      ESCAN00025000    Support for MB90DENSO
 *  02.39.00   2008-03-28  JHg     ESCAN00025270    Added support for Volvo AB
 *                         CB      ESCAN00025396    Support for MB90440GF52009
 *                         Ls                       removed special section for MPC -> moved to flashdrv.h
 *  02.40.00   2008-09-03  JHg                      Added support for variant parameter manager
 *                                                  Added support for EepMgr
 *                                                  Added support for RSA seed/key decryption
 *                                                  Added support for Vector decompression module
 *                         Ci                       Added support for Renesas R8C
 *  02.41.00   2008-09-17  Rr                       Added new OEM
 *                         MVi     ESCAN00030095    Added support for MB90950(G)
 *  02.42.00   2008-11-25  FHe     ESCAN00030834    New macro to prevent the file being included twice
 *                         Ci                       Added support for gateway component Gw_Route
 *  02.43.00   2009-01-12  CB      ESCAN00032719    Support for M16C5L (RCAN2)
 *  02.44.00   2009-03-02  QPs                      Added support for FlexRay-Bootloader
 *  02.45.00   2009-05-04  CB                       Support for GAIO compiler
 *  02.46.00   2009-06-16  Ach                      Add new OEM GAC (SLP1)
 *                         Rr                       Added new OEM
 *  02.47.00   2009-08-26  AWh                      Added new OEM Fisker (SLP1)
 *  02.48.00   2009-09-24  Ls                       minor changes for Volvo (usecase VPM/EEPMGR)
 *                         TBe     ESCAN00037946    Moved CAN specific includes from fbl_inc.h to fbl_cw.h
 *  02.49.00   2009-09-28  TBe     ESCAN00038082    Moved include for fbl_apnm.h back to fbl_inc.h
 *  02.50.00   2009-09-30  CB                       Include statement for NV-wrapper
 *  02.51.00   2009-10-27  AWH                      Adapted Claas includes to current bootloader design.
 *  02.52.00   2010-04-09  Ci      ESCAN00040853    Ford: Released decompression API
 *  02.53.00   2010-04-22  CB      ESCAN00042462    New OEM
 *  02.54.00   2010-05-06  MVi                      Added multiple segment support for VAG/PAG
 *  02.55.00   2010-12-17  Ach     ESCAN00047534    Support Chrysler security module in Fiat bootloaders
 *  02.56.00   2011-05-27  Rr                       Reworked OEM Renault
 *  02.57.00   2012-01-02  AWh                      Added new OEM Toyota (SLP1)
 *  02.58.00   2012-01-20  Ci                       Added support for XCP
 *  02.59.00   2012-03-19  Ach                      Common data support added
 *                         CB                       LibMem support added
 *  02.60.00   2012-04-20  Ci                       Added OEM VolvoCar
 *  02.61.00   2012-11-29  Ach     ESCAN00062912    Adapted comments to use Doxygen
 *                         CB                       Changed include sequence, WrapNv.h included prior to api-header
 *  02.62.00   2012-12-11  AWh                      Reorganized fbl_apdi.h includes for GM SLPs
 *  02.63.00   2013-02-26  CB      ESCAN00065402    Support for Fiat SLP4
 *  02.64.00   2013-04-08  SvR                      Added support for VolvoCar security module
 *  02.65.00   2013-04-09  CB                       Modifications for MMC SLP7
 *  02.66.00   2013-07-23  Ci                       Added SecMVer.h for VolvoCar
 *  02.67.00   2013-07-25  AWh                      Added SecM.h/Eep includes for Gm Slp5/Slp6, remove Gm Slp1
 *  02.68.00   2014-02-13  Ci                       Added support for Ford SLP7
 *                         Ach                      Added support for fbl_mtab.h in
 *                                                   Vector SLP3 / VAG SLP3 / Daimler UDS / Fiat SLP4 / HMC
 *  02.69.00   2014-04-30  ASe     ESCAN00075271    Some includes have an invalid case
 *                         HRf     ESCAN00075328    Add support for OEM PATAC
 *  02.70.00   2014-07-02  AWh                      Added support for fbl_mtab.h for Gm
 *  02.71.00   2014-08-07  Ach     ESCAN00077687    Change NV-Wrapper inclusion to compile time decision
 *  02.72.00   2014-08-21  Ach     ESCAN00077967    Change common data structure inclusion to compile time decision
 **********************************************************************************************************************/

#ifndef __FBL_INC_H__
#define __FBL_INC_H__

/***********************************************************************************************************************
 *  VERSION
 **********************************************************************************************************************/

/* ##V_CFG_MANAGEMENT ##CQProject : FblInc CQComponent : Implementation */
#define FBLINC_VERSION          0x0272u
#define FBLINC_RELEASE_VERSION  0x00u

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

/* Version check file */
#include "v_ver.h"
#include "v_cfg.h"
#if defined( VGEN_GENY )
# include "v_inc.h"
#else
# ifdef C_ENABLE_CCP
#  include "ccppar.h"
# endif /* C_ENABLE_CCP */
#endif /* VGEN_GENY */

/* Security module */
# include "SecM.h"

#include "fbl_def.h"

/* Logical block table types and definitions */
# include "fbl_mtab.h"

/* Basic types and definitions */
/* Set up CAN-IDs for the HW-File */
#  include "fbl_can.h"

/* Configuration of the transport protocol */
#   include "ftp_cfg.h"
/* Hardware dependent header file */

#include "flashdrv.h"
# include "fbl_hw.h"

#include "fbl_wd.h"
# include "iotypes.h"
#  include "fbl_mio.h"
# include "fbl_flio.h"
#  include "EepInc.h"
#  include "EepIO.h"
/* Application vector table */
# include "applvect.h"

#if defined( FBL_ENABLE_WRAPPER_NV )
# include "WrapNv.h"
#endif

/* Application call-back functions */
#include "fbl_ap.h"
#include "fbl_apwd.h"
#include "fbl_apfb.h"
# include "fbl_apdi.h"
#  include "fbl_apnv.h"

/* Diagnostic layer */
#include "fbl_diag.h"

# include "fbl_mem.h"

#endif /* __FBL_INC_H__ */

/***********************************************************************************************************************
 *  END OF FILE: FBL_INC.H
 **********************************************************************************************************************/
