/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief         EEPROM driver wrapper functions
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
 *  Quetty Palacios               QPs           Vector Informatik GmbH
 *  Joern Herwig                  JHG           Vector Informatik GmbH
 *  Andre Caspari                 Ci            Vector Informatik GmbH
 *  Slawomir Wilk                 SWk           Vector Informatik GmbH
 *  Marcel Viole                  MVi           Vector Informatik GmbH
 *  Alexander Starke              ASe           Vector Informatik GmbH
 *  Dennis O'Donnell              DJO           Vector CANtech, Inc.
 *  Ralf Haegenlaeuer             HRf           Vector Informatik GmbH
 *  Sven Ruess                    SvR           Vector Informatik GmbH
 *  Thomas Bezold                 TBe           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  01.00.00   2004-06-27  WM                       Implementation
 *  01.01.00   2004-09-17  WM                       Minor changes
 *  01.02.00   2004-09-20  MHz                      Added extern global data eepData[]
 *  01.03.00   2004-09-22  WM                       eepmemcpy added
 *  01.04.00   2004-11-19  WM                       Changes for C_COMP_FUJITSU_16LX
 *  01.05.00   2004-12-13  Ap                       EepromDriver_VerifySync added
 *  01.06.00   2005-05-18  WM                       Support for M16C added
 *  01.07.00   2005-07-01  WM                       C_COMP_TASKING_TRICORE_MULTICAN added
 *                         Fz                       Support for C_COMP_NEC_V85X added
 *  01.08.00   2005-08-29  WM                       C_COMP_DIABDATA_MPC55XX added
 *                         QPs                      C_COMP_FUJITSU_FR60_CCAN added
 *  01.09.00   2005-09-12  WM                       Minor changes for C_COMP_HIWARE
 *  01.10.00   2005-10-24  WM                       C_COMP_IAR_M16/M32C added
 *  01.11.00   2005-12-06  MHz                      No changes
 *  01.12.00   2006-05-24  CB                       C_COMP_IAR_78K0 added
 *  01.13.00   2006-07-14  WM                       C_COMP_DIABDATA_PPC added
 *                         JHG                      No changes
 *  01.14.00   2006-09-11  Ci                       C_COMP_IAR_V85X added
 *                         QPs                      No changes
 *  01.15.00   2006-09-14  SWk     ESCAN00017712    Added support for KEIL and ST10F27x
 *  01.16.00   2006-11-15  JHg     ESCAN00017959    Remove V_MEMRAM2_NEAR from eepData for HC08
 *                         WM                       Added C_COMP_TASKING_XC16X
 *                         JHg                      No changes
 *  01.17.00   2007-02-02  SWk     ESCAN00019353    No changes
 *  01.18.00   2007-11-22  JHg     ESCAN00023241    Added C_COMP_TASKING_XC2000_MULTICAN
 *  01.19.00   2007-11-23  Rr      ESCAN00023377    Added C_COMP_IAR_R32C
 *  01.20.00   2008-01-29  Ci      ESCAN00024281    Compiler warning due to duplicate section definition
 *                         WM                       Added C_COMP_MTRWRKS_MCS08_MSCAN
 *  01.21.00   2008-03-19  QPs     ESCAN00025402    C_COMP_COSMIC_MCS08 added
 *  01.22.00   2008-04-04  CB      ESCAN00025818    No changes
 *                         WM                       C_COMP_GHS_MPC55XX added
 *  01.23.00   2008-07-22  SWk     ESCAN00028557    Added support for C_COMP_MICROCHIP_DSPIC33
 *  01.24.00   2008-11-11  MVi                      Added C_COMP_HEW_SH2_RCAN
 *  01.25.00   2008-12-11  MVi     ESCAN00024444    Added support for Tasking VX toolchain
 *  01.26.00   2009-02-27  JHg     ESCAN00033360    Added support for TI OMAP
 *  01.27.00   2009-04-15  QPs     ESCAN00034588    No changes
 *  01.28.00   2009-09-24  MVi     ESCAN00038014    No changes
 *  01.29.00   2009-11-02  QPs     ESCAN00038902    No changes
 *  01.30.00   2010-07-09  Ls      ESCAN00043706    No changes
 *                         SWk     ESCAN00043695    Add Support for C_COMP_IAR_78K0R_COMMENT
 *                         QPs     ESCAN00043961    No changes
 *  01.31.00   2011-05-30  ASe     ESCAN00051265    No changes
 *                                                  Reworked module
 *  01.32.00   2011-09-29  MVi     ESCAN00051436    No changes
 *  01.33.00   2011-11-03  SWk     ESCAN00054646    No changes
 *  01.34.00   2012-05-03  ASe     ESCAN00056584    No changes
 *  01.35.00   2012-10-08  MVi     ESCAN00058848    Add support for C_COMP_GHS_SJA2020
 *                         ASe     ESCAN00059022    Added support C_COMP_GHS_FCR4_CCAN_COMMENT
 *                         CB      ESCAN00061791    Added support C_COMP_METROWERKS_MPC55XX_COMMENT
 *                         DJO     ESCAN00061846    Added support C_COMP_FUJITSU_FR81_CCAN
 *  01.36.00   2012-11-29  ASe     ESCAN00062912    Adapted comments to use Doxygen
 *  01.37.00   2014-02-25  HRf     ESCAN00073917    Add support for RL78 with IAR and Renesas compiler
 *  01.38.00   2014-03-25  QPs     ESCAN00074568    No changes
 *  01.39.00   2014-04-30  ASe     ESCAN00075336    No changes
 *  01.40.00   2014-05-12  Svr     ESCAN00075517    Introduce FBL_ENABLE_MEMMAP_COMMENT
 *  01.41.00   2014-06-12  TBe     ESCAN00075967    No changes
 **********************************************************************************************************************/

#ifndef __EEP_IO_H__
#define __EEP_IO_H__

/***********************************************************************************************************************
 *  VERSION
 **********************************************************************************************************************/

#define EEP_IO_VERSION    0x0141u

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

#define IO_DRIVERMODUS    SYNCRON
#define IO_DEVICETYPE     RANDOM

#ifndef V_API_NEAR
# define V_API_NEAR
#endif

# if ! defined( EEP_BUFFER_SIZE )
#  define EEP_BUFFER_SIZE 0x100
# endif

#define EEPROM_DRIVER_VERSION_MAJOR (EEP_IO_VERSION >> 8)
#define EEPROM_DRIVER_VERSION_MINOR (EEP_IO_VERSION & 0xFF)
#define EEPROM_DRIVER_VERSION_PATCH 0x00

#define EepromDriver_GetVersionOfDriver()  ((IO_U32)(EEPROM_DRIVER_VERSION_MAJOR << 16) | \
                                            (IO_U32)(EEPROM_DRIVER_VERSION_MINOR << 8)  | \
                                                     EEPROM_DRIVER_VERSION_PATCH)

/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

IO_ErrorType V_API_NEAR EepromDriver_InitSync( void * );
IO_ErrorType V_API_NEAR EepromDriver_DeinitSync( void * );
IO_ErrorType V_API_NEAR EepromDriver_VerifySync( void * );
IO_ErrorType V_API_NEAR EepromDriver_RReadSync( IO_MemPtrType, IO_SizeType, IO_PositionType );
IO_ErrorType V_API_NEAR EepromDriver_RWriteSync( IO_MemPtrType, IO_SizeType, IO_PositionType );
IO_ErrorType V_API_NEAR EepromDriver_REraseSync( IO_SizeType, IO_PositionType );

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

# define EEPDUMMY_START_SEC_VAR_EXPORT
# include "MemMap.h"   /* PRQA S 5087 *//* MD_MSR_19.1 */

extern V_MEMRAM0 V_MEMRAM1_NEAR IO_U8 V_MEMRAM2_NEAR eepData[EEP_BUFFER_SIZE];

# define EEPDUMMY_STOP_SEC_VAR_EXPORT
# include "MemMap.h"   /* PRQA S 5087 *//* MD_MSR_19.1 */

#endif /* __EEP_IO_H__ */

/***********************************************************************************************************************
 *  END OF FILE: EEPIO.H
 **********************************************************************************************************************/
