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
 *  01.00.00   2004-07-26  WM                       Implementation
 *  01.01.00   2004-09-17  WM                       Adaptations for C_COMP_TASKING_XX
 *  01.02.00   2004-09-20  MHz                      Added extern global data eepData[]
 *  01.03.00   2004-09-22  WM                       eepcpy function added
 *  01.04.00   2004-11-19  WM                       Changes for C_COMP_FUJITSU_16LX
 *  01.05.00   2004-12-31  Ap                       EepromDriver_VerifySync added
 *                         WM                       Added support for MCS12
 *  01.06.00   2005-05-18  WM                       Support for M16C added
 *  01.07.00   2005-07-01  WM                       C_COMP_TASKING_TRICORE_MULTICAN added
 *                         Fz                       Support for C_COMP_NEC_V85X added
 *  01.08.00   2005-08-29  WM                       C_COMP_DIABDATA_MPC55XX added
 *                         QPs                      C_COMP_FUJITSU_FR60_CCAN added
 *  01.09.00   2005-09-12  WM                       No changes
 *  01.10.00   2005-10-24  WM                       C_COMP_IAR_M16/M32C added
 *                         Rr                       C_COMP_MITSUBISHI_M32R added
 *  01.11.00   2005-12-06  MHz     ESCAN00014549    Added C_COMP_IAR_MAC710X
 *  01.12.00   2006-05-24  CB                       C_COMP_IAR_78K0 added
 *  01.13.00   2006-07-14  WM                       C_COMP_DIABDATA_PPC added
 *                         JHG                      Added V_API_NEAR to functions
 *  01.14.00   2006-09-11  Ci                       C_COMP_IAR_V85X added
 *                         QPs                      C_COMP_FUJITSU_16FX_CCAN added
 *  01.15.00   2006-09-14  SWk     ESCAN00017712    Added support for KEIL and ST10F27x
 *  01.16.00   2006-11-15  JHg     ESCAN00017959    Remove V_MEMRAM2_NEAR from eepData
 *                                                  for HC08
 *                         WM                       Added C_COMP_TASKING_XC16X
 *                         JHg     ESCAN00018621    Added initialised state flag
 *  01.17.00   2007-02-02  SWk     ESCAN00019353    Added C_COMP_MTRWRKS_MCS08_MSCAN
 *  01.18.00   2007-11-22  JHg     ESCAN00023241    Added C_COMP_TASKING_XC2000_MULTICAN
 *  01.19.00   2007-11-23  Rr      ESCAN00023377    Added C_COMP_IAR_R32C
 *  01.20.00   2008-01-29  Ci      ESCAN00024281    Compiler warning due to duplicate
 *                                                  section definition
 *                         WM                       No changes
 *  01.21.00   2008-03-19  QPs     ESCAN00025402    C_COMP_COSMIC_MCS08 added
 *  01.22.00   2008-07-09  CB      ESCAN00025818    Renamed eepcpy to avoid name
 *                                                  collisions
 *                         WM                       C_COMP_GHS_MPC55XX added
 *  01.23.00   2008-07-22  SWk     ESCAN00028557    Added support for C_COMP_MICROCHIP_DSPIC33
 *  01.24.00   2008-11-11  MVi                      Added C_COMP_HEW_SH2_RCAN
 *  01.25.00   2008-12-11  MVi     ESCAN00024444    Added support for Tasking VX toolchain
 *  01.26.00   2009-02-27  JHg     ESCAN00033360    Added support for TI OMAP
 *  01.27.00   2009-04-15  QPs     ESCAN00034588    Added support for FlexRay-Bootloader
 *  01.28.00   2009-09-24  MVi     ESCAN00038014    Added support for TI TMS570
 *  01.29.00   2009-11-02  QPs     ESCAN00038902    v850, GHS Compiler error using
 *                                                  --strict=comperr
 *  01.30.00   2010-07-09  Ls      ESCAN00043706    Added C_COMP_GNU_IMX_FLEXCAN2
 *                         SWk     ESCAN00043695    Added support for C_COMP_IAR_78K0R_COMMENT
 *                         QPs     ESCAN00043961    Added support for LIN-Bootloader
 *  01.31.00   2011-05-30  ASe     ESCAN00051265    Added support for TI TMS320
 *                                                  Reworked module
 *  01.32.00   2011-09-29  MVi     ESCAN00051436    Added Fx4 Back-up RAM (BURAM) support
 *  01.33.00   2011-11-03  SWk     ESCAN00054646    Added support for C_COMP_ARM_STM32_BXCAN
 *  01.34.00   2012-05-03  ASe     ESCAN00056584    Fixed compiler warnings
 *  01.35.00   2012-10-08  MVi     ESCAN00058848    Added support for C_COMP_GHS_SJA2020
 *                         ASe     ESCAN00059022    Added support C_COMP_GHS_FCR4_CCAN_COMMENT
 *                         CB      ESCAN00061791    Added support C_COMP_METROWERKS_MPC55XX_COMMENT
 *                         DJO     ESCAN00061846    Added support C_COMP_FUJITSU_FR81_CCAN
 *  01.36.00   2012-11-29  ASe     ESCAN00062912    Adapted comments to use Doxygen
 *  01.37.00   2014-02-25  HRf     ESCAN00073917    Added support for RL78 with IAR and Renesas compiler
 *  01.38.00   2014-03-25  QPs     ESCAN00074568    Added support for C_COMP_GNU_STM32_BXCAN_COMMENT
 *  01.39.00   2014-04-30  ASe     ESCAN00075336    No changes
 *  01.40.00   2014-05-12  Svr     ESCAN00075517    Introduce FBL_ENABLE_MEMMAP_COMMENT
 *  01.41.00   2014-06-12  TBe     ESCAN00075967    Added section pragmas for 78K0R Renesas compiler
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "fbl_def.h"
#include "iotypes.h"
# if defined( VGEN_ENABLE_FBL ) || \
     defined( VGEN_ENABLE_CANFBL )
# else
#  include "can_cfg.h"
# endif
#include "EepInc.h"
#include "EepIO.h"

/***********************************************************************************************************************
 *  VERSION
 **********************************************************************************************************************/

#if ( EEP_IO_VERSION != 0x0141u )
# error "Error in EEP_IO.C: Source and header file are inconsistent!"
#endif

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

#define EEP_INIT_FLAG       0x55

#define EEP_UNINITIALISED   0x00
#define EEP_INITIALISED     0x01


/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

static MEMORY_NEAR vuint8 eepInitialised;

# define EEPDUMMY_START_SEC_VAR
# include "MemMap.h"   /* PRQA S 5087 *//* MD_MSR_19.1 */

#if defined( V_COMP_RENESAS_78K0R )
# pragma section @@DATA EEPDATA
V_MEMRAM0 V_MEMRAM1_NEAR IO_U8 V_MEMRAM2_NEAR eepData[EEP_BUFFER_SIZE];
# pragma section @@DATA @@DATA
#else
V_MEMRAM0 V_MEMRAM1_NEAR IO_U8 V_MEMRAM2_NEAR eepData[EEP_BUFFER_SIZE];
#endif

# define EEPDUMMY_STOP_SEC_VAR
# include "MemMap.h"   /* PRQA S 5087 *//* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

static void MEMORY_HUGE * EepCpy(void MEMORY_HUGE * dest, void MEMORY_HUGE * source, IO_SizeType size);

/***********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  EepCpy
 **********************************************************************************************************************/
/*! \brief       Local memcpy function
 *  \param[in]   dest Pointer to destination
 *  \param[in]   source Pointer to source
 *  \param[in]   size Size of data to be copied in bytes
 **********************************************************************************************************************/
static void MEMORY_HUGE * EepCpy( void MEMORY_HUGE * dest, void MEMORY_HUGE * source, IO_SizeType size )
{
   IO_SizeType i;

   for (i = 0; i < size; i++)
   {
      ((vuint8 MEMORY_HUGE *)dest)[i] = ((vuint8 MEMORY_HUGE *)source)[i];
   }

   return dest;
}

/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  EepromDriver_InitSync
 **********************************************************************************************************************/
/*! \brief       Initialize the EEPROM algorithm
 *  \param[in]   address Unused parameter for HIS interface compliance
 *  \return      The return code shows the success of the initialization
 **********************************************************************************************************************/
IO_ErrorType V_API_NEAR EepromDriver_InitSync( void * address )
{
   IO_SizeType i;

#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   (void)address;
#endif

   /* Check if buffer already initialized */
   if (eepData[EEP_BUFFER_SIZE-1] != EEP_INIT_FLAG)
   {
      /* Initialize buffer */
      for (i = 0; i < EEP_BUFFER_SIZE; i++)
      {
         eepData[i] = 0xFFu;
      }

      /* Set initialized flag */
      eepData[EEP_BUFFER_SIZE - 1] = EEP_INIT_FLAG;
   }

   eepInitialised = EEP_INITIALISED;

   return IO_E_OK;
}

/***********************************************************************************************************************
 *  EepromDriver_DeinitSync
 **********************************************************************************************************************/
/*! \brief       De-initialize the EEPROM algorithm, is called on ReturnToNormalMode
 *  \param[in]   address Unused parameter for HIS interface compliance
 *  \return      The return code shows the success of the de-initialization
 **********************************************************************************************************************/
IO_ErrorType V_API_NEAR EepromDriver_DeinitSync( void * address )
{
#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   (void)address;
#endif

   eepInitialised = EEP_UNINITIALISED;

   return IO_E_OK;
}

/***********************************************************************************************************************
 *  EepromDriver_RWriteSync
 **********************************************************************************************************************/
/*! \brief       Program EEPROM by calling EEPROM driver routine stored in RAM memory.
 *  \pre         EEPROM driver has to be initialized
 *  \param[in]   writeBuffer Write data buffer
 *  \param[in]   writeLength Number of bytes to be written
 *  \param[in]   writeAddress The write address
 *  \return      Status of EEPROM programming
 **********************************************************************************************************************/
IO_ErrorType V_API_NEAR EepromDriver_RWriteSync( IO_MemPtrType writeBuffer, IO_SizeType writeLength, IO_PositionType writeAddress )
{
   IO_ErrorType errorState;

   errorState = IO_E_NOT_OK;

   if (eepInitialised == EEP_INITIALISED)
   {
      (void)EepCpy((IO_MemPtrType)writeAddress, writeBuffer, writeLength);

      errorState = IO_E_OK;
   }

   return errorState;
}

/***********************************************************************************************************************
 *  EepromDriver_REraseSync
 **********************************************************************************************************************/
/*! \brief       Erase EEPROM memory
 *  \pre         EEPROM driver has to be initialized
 *  \param[in]   eraseLength Number of bytes to be erased
 *  \param[in]   eraseAddress The erase address
 *  \return      Status of EEPROM erase
 **********************************************************************************************************************/
IO_ErrorType V_API_NEAR EepromDriver_REraseSync( IO_SizeType eraseLength, IO_PositionType eraseAddress )
{
   IO_ErrorType errorState;

#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   (void)eraseLength;
   (void)eraseAddress;
#endif

   errorState = IO_E_NOT_OK;

   if (eepInitialised == EEP_INITIALISED)
   {
      errorState = IO_E_OK;
   }

   return errorState;
}

/***********************************************************************************************************************
 *  EepromDriver_RReadSync
 **********************************************************************************************************************/
/*! \brief          Read EEPROM memory
 *  \pre            EEPROM driver has to be initialized
 *  \param[out]     readBuffer Read data buffer
 *  \param[in]      readLength Number of bytes to be read
 *  \param[in]      readAddress The read address
 *  \return         Status of EEPROM read
 **********************************************************************************************************************/
IO_ErrorType V_API_NEAR EepromDriver_RReadSync( IO_MemPtrType readBuffer, IO_SizeType readLength, IO_PositionType readAddress )
{
   IO_ErrorType errorState;

   errorState = IO_E_NOT_OK;

   if (eepInitialised == EEP_INITIALISED)
   {
      (void)EepCpy(readBuffer, (IO_MemPtrType)readAddress, readLength);

      errorState = IO_E_OK;
   }

   return errorState;
}

/***********************************************************************************************************************
 *  EepromDriver_VerifySync
 **********************************************************************************************************************/
/*! \brief       Verify EEPROM memory
 *  \pre         EEPROM driver has to be initialized
 *  \param[in]   address Unused parameter for HIS interface compliance
 *  \return      Status of EEPROM verification
 **********************************************************************************************************************/
IO_ErrorType V_API_NEAR EepromDriver_VerifySync( void * address )
{
   IO_ErrorType errorState;

#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   (void)address;
#endif

   errorState = IO_E_NOT_OK;

   if (eepInitialised == EEP_INITIALISED)
   {
      errorState = IO_E_OK;
   }

   return errorState;
}

/***********************************************************************************************************************
 *  END OF FILE: EEPIO.C
 **********************************************************************************************************************/
