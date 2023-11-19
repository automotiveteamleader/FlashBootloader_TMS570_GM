/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        Flash driver wrapper for TMS470
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
 *  Robert Schaeffner             Rr            Vector Informatik GmbH
 *  Marco Wierer                  WM            Vector Informatik GmbH
 *  Andreas Wenckebach            AWh           Vector Informatik GmbH
 *  Andre Caspari                 Ci            Vector Informatik GmbH
 *  Quetty Palacios               QPs           Vector Informatik GmbH
 *  Marcel Viole                  MVi           Vector Informatik GmbH
 *  Achim Strobelt                Ach           Vector Informatik GmbH
 *  Alexander Starke              ASe           Vector Informatik GmbH
 *  Vijay Natarajan               VNA           Vector CANtech Inc.
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  01.00.00   2003-03-26  Ls                       Release
 *  01.01.00   2003-04-11  Rr                       Added Titan-Support
 *  01.02.00   2003-04-14  Ls                       Minor changes; version control added
 *  01.03.00   2003-04-17  Ls                       Added FBL_ECU_FLASHSIZE for Bosch single
 *                                                  processor
 *  01.04.00   2004-01-26  CB                       _FLASHDRV_ROM_ support added
 *  01.05.00   2004-03-05  CB                       Watchdog support added
 *  01.06.00   2004-03-26  CB                       No changes
 *  01.07.00   2004-05-11  CB                       Minor changes
 *  01.08.00   2005-03-02  CB      ESCAN00011330    Fix in FlashDriver_InitSync
 *  01.09.00   2006-03-14  Rr                       Minor changes for FORD
 *  01.10.00   2006-04-20  WM      ESCAN00016093    Version scan support added
 *  01.11.00   2007-02-13  CB                       Added calls of FblLookForWatchdog in Init-
 *                                                  and DeinitSync
 *  01.12.00   2008-03-13  AWh     ESCAN00025330    Remove CAN-parameter-init of
 *                                                  flashParam if no RCRRP Retrigger is used
 *                         AWh     ESCAN00025327    Assign WDTriggerFct instead of
 *                                                  WDTriggerBuffer to flashParam.wdTriggerFct
 *  01.13.00   2008-08-15  Rr      ESCAN00029265    Remove RealTimeSupport
 *                                                  Init of P2ReloadValue only done for
 *                                                  FBL_ENABLE_RCRRP_RETRIGGER
 *  01.14.00   2009-09-22  Ci      ESCAN00036244    Added flashCode array
 *                                 ESCAN00036249    Adapted initialization of CAN
 *                                                  TX register
 *  01.15.00   2009-09-24  QPs     ESCAN00038012    FBL_ENABLE_COPY_CODE_TO_RAM_ON_STARTUP
 *                                                  support added
 *                         MVi     ESCAN00038014    Added support for TI TMS570
 *  01.16.00   2010-01-11  Pal     ESCAN00042050    Added support for C_COMP_ARM_TMS470_DCAN
 *  01.17.00   2011-10-24  AWh     ESCAN00049641    Adaptions to changed flashdrv-design
 *                         MVi     ESCAN00054405    Added support for Tms470F02xHis
 *  01.18.00   2012-09-04  CB      ESCAN00059374    Reduced number of FblLookForWatchdog calls
 *                         Ach     ESCAN00061133    Support calculation of parity data
 *  01.19.00   2013-03-26  QPs     ESCAN00066373    Remove checkBuf and checksumType initialization
 *                                                  when using F021 Flash
 *                                 ESCAN00066551    Converted macro FlashDriver_GetVersionOfDriver()
 *                                                  into a function
 *  01.20.00   2013-07-02  ASe     ESCAN00067929    Adapted comments to use doxygen
 *  01.21.00   2013-08-19  ASe     ESCAN00069664    Added option for user read function
 *  01.22.00   2014-04-30  ASe     ESCAN00074552    Added multi driver support
 *  01.23.00   2014-06-30  VNA     ESCAN00076586    FLASHDRV_ROM can be configured at compile time
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "fbl_inc.h"
#if defined( FBL_ENABLE_FLASHDRV_ROM )
# include "flashrom.h"
#endif
#include "appl_flio.h"
#include "flashdrv.h"

#if defined( FBLDRVFLASH_TMS470FXHIS_VERSION )
# if ( FBLDRVFLASH_TMS470FXHIS_VERSION < 0x0128u )
#  error "This wrapper implementation is compliant to flash driver versions >= 0x0128 only."
# endif
#endif

/***********************************************************************************************************************
 *  VERSION
 **********************************************************************************************************************/

#if ( FBLWRAPPERFLASH_TMS470FXHIS_VERSION != 0x0123u ) || \
    ( FBLWRAPPERFLASH_TMS470FXHIS_RELEASE_VERSION != 0x00u )
# error "Error in FBL_FLIO.C: Source and header file are inconsistent!"
#endif

#if ( FBLWRAPPERFLASH_TMS470FXHIS_VERSION != _FBLWRAPPERFLASH_HW_VERSION ) || \
    ( FBLWRAPPERFLASH_TMS470FXHIS_RELEASE_VERSION != _FBLWRAPPERFLASH_HW_RELEASE_VERSION )
# error "Error in FBL_FLIO.C: Source and V_VER.H are inconsistent!"
#endif

/***********************************************************************************************************************
 *  CONFIGURATION CHECKS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

#define FBLWRAPPERFLASH_WD_TRIGGER_INTERVAL_MASK   ((IO_U16)0x1Fu)

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

#  pragma DATA_SECTION (flashCode, ".FLASHDRV")
IO_U8 flashCode[FLASH_SIZE];

/***********************************************************************************************************************
 *  LOCAL/EXTERNAL DATA
 **********************************************************************************************************************/

static tFlashParam flashParam;

/***********************************************************************************************************************
 *  INTERNAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

#if defined( FBL_ENABLE_FLASHDRV_ROM )
static IO_ErrorType FlashDriver_CopyDrvToRam( void );
#endif

/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  FlashDriver_InitSync
 **********************************************************************************************************************/
/*! \brief     Initialize the flash algorithm
 *  \pre       The flash algorithm has to be copied into RAM
 *  \param     address Not used; conformance to HIS interface
 *  \return    The return code shows the success of the initialization
 **********************************************************************************************************************/
IO_ErrorType FlashDriver_InitSync( void * address )
{
#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   (void)address;
#endif
   /* Initialize input parameters for init function */
   flashParam.wdTriggerFct = (IO_U8 (*)(void))FblLookForWatchdog;

#if defined( FBL_ENABLE_FLASHDRV_ROM )
   if(FlashDriver_CopyDrvToRam() != IO_E_OK)
   {
     return kFlashFailed;
   }
#endif /* FBL_ENABLE_FLASHDRV_ROM */

   /* Verify if flashCode signature is correct */
   if (   (FLASH_DRIVER_MCUTYPE(flashCode)   != FLASH_DRIVER_VERSION_MCUTYPE)
       || (FLASH_DRIVER_MASKTYPE(flashCode)  != FLASH_DRIVER_VERSION_MASKTYPE)
       || (FLASH_DRIVER_INTERFACE(flashCode) != FLASH_DRIVER_VERSION_INTERFACE))
   {
      /* Flash driver signature mismatch, wrong flash driver */
      return kFlashInitInvalidVersion;
   }

   flashParam.reserved1 = 0x00u;
   flashParam.majorVersion = FLASH_DRIVER_VERSION_MAJOR;
   flashParam.minorVersion = FLASH_DRIVER_VERSION_MINOR;
   flashParam.patchLevel = FLASH_DRIVER_VERSION_PATCH;
   flashParam.reserved2 = 0x00u;

   flashParam.hclkFrequency   = FBL_HCLK;                 /* System clock frequency in MHz */
   flashParam.programmingMode = Fapi_AutoEccGeneration;   /* Indicates the programming mode to use */

   /* Call initialization routine of the flash driver */
   FLASH_DRIVER_INIT(flashCode, &flashParam);

   return flashParam.errorCode;
}

/***********************************************************************************************************************
 *  FlashDriver_DeinitSync
 **********************************************************************************************************************/
/*! \brief     De-initialize the flash algorithm
 *  \pre       The flash algorithm has to be copied into RAM
 *  \param     address Not used; conformance to HIS interface
 *  \return    The return code shows the success of the de-initialization
 **********************************************************************************************************************/
IO_ErrorType FlashDriver_DeinitSync( void * address )
{
   IO_U16 i;

#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   (void)address;
#endif

   /* Call deinitialization function */
   FLASH_DRIVER_DEINIT(flashCode, &flashParam);

   /* Remove the flash code from RAM */
   for (i = 0u; i < FLASH_SIZE; i++)
   {
      if ((i & FBLWRAPPERFLASH_WD_TRIGGER_INTERVAL_MASK) == 0x00u)
      {
         (void)FblLookForWatchdog();
      }

      flashCode[i] = 0u;
   }

   return flashParam.errorCode;
}

/***********************************************************************************************************************
 *  FlashDriver_RWriteSync
 **********************************************************************************************************************/
/*! \brief        Program flash by calling flash driver routine stored in RAM memory
 *  \pre          Flash driver has to be initialized
 *  \param[in]    writeBuffer Data buffer
 *  \param[in]    writeLength Size of data to be written in bytes
 *  \param[in]    writeAddress Start address of data to be written
 *  \return       Status of flash programming
 **********************************************************************************************************************/
IO_ErrorType FlashDriver_RWriteSync( IO_MemPtrType writeBuffer, IO_SizeType writeLength, IO_PositionType writeAddress )
{
   /* Check for alignment of destination to FLASH_SEGMENT_SIZE */
   if ((writeAddress & (FLASH_SEGMENT_SIZE - 1u)) != 0u)
   {
      return kFlashWriteInvalidAddr;
   }

   /* Check for alignment of length to FLASH_SEGMENT_SIZE */
   if ((writeLength & (FLASH_SEGMENT_SIZE - 1u)) != 0u)
   {
      return kFlashWriteInvalidSize;
   }

   /* Initialize parameter field */
   flashParam.address = writeAddress;                /* Flash memory target address */
   flashParam.length = writeLength;                  /* Number of bytes to program */
   flashParam.data = (tFlashData *)writeBuffer; /* Pointer to source data buffer */

   /* Call flash write function in RAM */
   FLASH_DRIVER_WRITE(flashCode, &flashParam);

   return flashParam.errorCode;
}

/***********************************************************************************************************************
 *  FlashDriver_REraseSync
 **********************************************************************************************************************/
/*! \brief        Erase flash memory
 *  \pre          Flash driver has to be initialized
 *  \param[in]    eraseLength Size of data to be erased in bytes
 *  \param[in]    eraseAddress Start address of data to be erased
 *  \return       Status of flash erase
 **********************************************************************************************************************/
IO_ErrorType FlashDriver_REraseSync( IO_SizeType eraseLength, IO_PositionType eraseAddress )
{
   flashParam.address = eraseAddress;  /* Flash erase address */
   flashParam.length = eraseLength;    /* Number of bytes to be erased */

   /* Call the erase function in RAM */
   FLASH_DRIVER_ERASE(flashCode, &flashParam);

   return flashParam.errorCode;
}

#if defined( FBL_ENABLE_USR_RREAD_SYNC )
#else
/***********************************************************************************************************************
 *  FlashDriver_RReadSync
 **********************************************************************************************************************/
/*! \brief        Read flash memory
 *  \pre          Flash driver has to be initialized
 *  \param[out]   readBuffer Data buffer
 *  \param[in]    readLength Size of data to be read in bytes
 *  \param[in]    readAddress Start address of data to be read
 *  \return       Status of flash read
 **********************************************************************************************************************/
IO_ErrorType FlashDriver_RReadSync( IO_MemPtrType readBuffer, IO_SizeType readLength, IO_PositionType readAddress )
{
   (void)MEMCPY(readBuffer, (IO_U8 *)readAddress, readLength);

   return IO_E_OK;
}
#endif /* FBL_ENABLE_USR_RREAD_SYNC */

/***********************************************************************************************************************
 *  FlashDriver_GetVersionOfDriver
 **********************************************************************************************************************/
/*! \brief        Get actual version of flash driver
 *  \pre          Flash driver has to be initialized
 *  \return       BCD coded version of flash driver
 **********************************************************************************************************************/
IO_U32 FlashDriver_GetVersionOfDriver( void )
{
   IO_U32 version;

   /* Check if flash code is present */
   if (   (FLASH_DRIVER_MCUTYPE(flashCode)   != FLASH_DRIVER_VERSION_MCUTYPE)
       || (FLASH_DRIVER_MASKTYPE(flashCode)  != FLASH_DRIVER_VERSION_MASKTYPE)
       || (FLASH_DRIVER_INTERFACE(flashCode) != FLASH_DRIVER_VERSION_INTERFACE))
   {
      /* Flash driver not present or signature mismatch: return built-in version */
      version = (IO_U32)(((IO_U32)FLASH_DRIVER_VERSION_MAJOR << 16u) | \
                         ((IO_U32)FLASH_DRIVER_VERSION_MINOR <<  8u) | \
                         ((IO_U32)FLASH_DRIVER_VERSION_PATCH <<  0u));
   }
   else
   {
      /* Flash driver is present, return actual version */
      version = (IO_U32)(((IO_U32)flashParam.majorVersion << 16u) | \
                         ((IO_U32)flashParam.minorVersion <<  8u) | \
                         ((IO_U32)flashParam.patchLevel   <<  0u));
   }

   return version;
}
#if defined( FBL_ENABLE_FLASHDRV_ROM )
/***********************************************************************************************************************
 *  FlashDriver_CopyDrvToRam
 **********************************************************************************************************************/
/*! \brief        Copies the flash driver from ROM/Flash to RAM
 *  \return       If driver copied successfully, retun IO_E_OK. else returns IO_E_NOT_OK
 **********************************************************************************************************************/
static IO_ErrorType FlashDriver_CopyDrvToRam( void ) 
{
   /* Copy flash driver into RAM buffer */
   IO_U16 i;
# if defined( FBL_ENABLE_SYSTEM_CHECK )
#  if defined( FLASH_DRIVER_RELOCATABLE )
   if(FLASH_SIZE < FLASHDRV_BLOCK0_LENGTH)
#  else
   /* Check if addresses match for non-relocatable driver */
   if (((FBL_ADDR_TYPE)flashCode != FLASHDRV_BLOCK0_ADDRESS) ||
       (FLASH_SIZE < FLASHDRV_BLOCK0_LENGTH))
#  endif /* FLASH_DRIVER_RELOCATABLE */
   {
      return IO_E_NOT_OK;
   }
# endif /* FBL_ENABLE_SYSTEM_CHECK */
   /* Copy flash driver into RAM buffer */
   for (i = 0u; i < FLASHDRV_BLOCK0_LENGTH; i++)
   {
      if ((i & FBLWRAPPERFLASH_WD_TRIGGER_INTERVAL_MASK) == 0x00u)
      {
         (void)FblLookForWatchdog();
      }
      //flashCode[i] = FLASHDRV_DECRYPTDATA(flashDrvBlk0[i]);
   }
   return IO_E_OK;
}
#endif /* FBL_ENABLE_FLASHDRV_ROM */
/***********************************************************************************************************************
 *  END OF FILE: FBL_FLIO.C
 **********************************************************************************************************************/
