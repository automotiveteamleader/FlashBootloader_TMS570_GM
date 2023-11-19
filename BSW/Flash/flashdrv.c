/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        Flash algorithm for the TMS470
 *
 *  --------------------------------------------------------------------------------------------------------------------
 *  COPYRIGHT
 *  --------------------------------------------------------------------------------------------------------------------
 *  \par Copyright
 *  \verbatim
 *  Copyright (c) 2015 by Vector Informatik GmbH.                                                  All rights reserved.
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
 *  Moe Charara                   Mch           Vector CANtech, Inc.
 *  Marcel Viole                  MVi           Vector Informatik GmbH
 *  Luis Gonzalez                 LGO           Vector CANtech, Inc.
 *  Alexander Starke              ASe           Vector Informatik GmbH
 *  Johannes Krimmel              KJs           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id          Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  01.00.00   2011-05-05  Mch                        Creation of the TMS470 F021 flash driver algorithm to support
 *                                                    TMS570LS30336
 *  01.01.00   2011-08-11  Mch     ESCAN00052712      Added support for automatic device information retrieval
 *                                                    (GetFlashDeviceInfo)
 *  01.02.00   2011-10-24  MVi     ESCAN00054376      Added watchdog trigger in erase routine
 *  01.03.00   2012-03-30  LGO     ESCAN00058048      Allow erase of sector 0 (GetSectorNr())
 *  01.04.00   2013-07-03  ASe     ESCAN00067934      Adapted comments to use doxygen
 *  01.05.00   2013-08-19  ASe     ESCAN00069457      Adaptations for Flash API 2.00.00
 *  01.06.00   2013-08-21  ASe     ESCAN00069932      Modified verification handling to improve watchdog handling
 *  01.07.00   2015-02-12  KJs     ESCAN00080585      Added option to use statically cofigured Flash sector setup
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "flashdrv.h"
#include "flashdrv_cfg.h"

# include "F021.h"

/***********************************************************************************************************************
 *  VERSION
 **********************************************************************************************************************/

#if ( ( FBLDRVFLASH_TMS470F02XHIS_VERSION != 0x0107u ) || \
      ( FBLDRVFLASH_TMS470F02XHIS_RELEASE_VERSION != 0x00u ) )
# error "Error in FLASHDRV.C: Source and header file versions are inconsistent!"
#endif

/***********************************************************************************************************************
 *  CONFIGURATION CHECK
 **********************************************************************************************************************/

#define MASKTYPE 0x05u

#if ( ( FLASH_DRIVER_VERSION_MCUTYPE != 0x47u ) || \
      ( FLASH_DRIVER_VERSION_MASKTYPE != MASKTYPE ) )
# error "CPU type error! Wrong flash driver header file."
#endif

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/** Converts bytes to TMSx70 library API required word size */
#define FLASH_BYTES_TO_WORDS(bytes)    ((unsigned int)(bytes) / 4u)

/** Maximum number of sector informations */
#define FLASH_MAX_SECTOR_INFOS         64u

/** Maximum number of flash banks */
#define FLASH_MAX_BANKS                8u

/***********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/** Flash header structure */
typedef struct
{
   /* Definition of the 4-byte flash algorithm header */
   unsigned char CPUType;
   unsigned char maskType;
   unsigned char reserved;
   unsigned char version;
   tFlashFct flashInitFct;
   tFlashFct flashDeinitFct;
   tFlashFct flashEraseFct;
   tFlashFct flashWriteFct;
} tFlashHeader;

/** Flash sector information */
typedef struct
{
   unsigned int         startAddress;        /**< Sector start address */
   unsigned int         length;              /**< Sector length */
   Fapi_FlashBankType   flashBank;           /**< Sector core (flash bank) */
   Fapi_FlashSectorType flashSector;         /**< Sector number */
} tFlashSectorInfo;

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

/* Locate this at the beginning of the code */
#pragma DATA_SECTION (flashHeader, ".SIGNATURE")
const tFlashHeader flashHeader=
{
   FLASH_DRIVER_VERSION_INTERFACE,
   0x00u,
   FLASH_DRIVER_VERSION_MASKTYPE,
   FLASH_DRIVER_VERSION_MCUTYPE,
   &ExpFlashInit,
   &ExpFlashDeinit,
   &ExpFlashErase,
   &ExpFlashWrite
};

/***********************************************************************************************************************
 *  LOCAL DATA
 **********************************************************************************************************************/

/** Local flash parameter (required for watchdog triggering) */
static tFlashParam * localFlashParam;

#if ! defined( FLASH_ENABLE_CUSTOM_SECTOR_SETUP )
/** Maximum number of sector informations that can be retrieved from Flash API / OTP */
static tFlashSectorInfo flashSectorInfo[FLASH_MAX_SECTOR_INFOS];

/** Actual number of flash sectors on device */
static unsigned char nrOfFlashSectors;
#else
static const tFlashSectorInfo flashSectorInfo[] = ERASE_SECTOR_MAP_INITIALIZER;

/** Actual number of flash sectors on device */
static const unsigned char nrOfFlashSectors = (sizeof(flashSectorInfo)/sizeof(flashSectorInfo[0]));
#endif

/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#if ! defined( FLASH_ENABLE_CUSTOM_SECTOR_SETUP )
static tFlashErrorCode GetFlashDeviceInfo( void );
#endif
static unsigned char GetSectorNr( unsigned int );
static Fapi_StatusType ProgramFlashModeSelect( tFlashAddress * pStartAddress, unsigned char * pData, tFlashLength length );
static Fapi_StatusType ProgramFlash( tFlashAddress startAddress, unsigned char * pData, unsigned char firstSector );
static Fapi_StatusType VerifyFlash( tFlashAddress startAddress, unsigned char * pData, tFlashLength length );

/***********************************************************************************************************************
 *  ExpFlashInit
 **********************************************************************************************************************/
/*! \brief           Do initialization before flash programming
 *  \param[in,out]   fp Flash parameter handle
 **********************************************************************************************************************/
void ExpFlashInit( tFlashParam * fp )
{
   Fapi_StatusType result;

   fp->majorVersion = FLASH_DRIVER_VERSION_MAJOR;
   fp->minorVersion = FLASH_DRIVER_VERSION_MINOR;
   fp->patchLevel   = FLASH_DRIVER_VERSION_PATCH;

   fp->errorCode = kFlashOk;
   localFlashParam = fp;

   (void)Fapi_serviceWatchdogTimer();

   if (FBLDRVFLASH_TMS470F02XHIS_VERSION <
       ((unsigned short)(((unsigned short)(fp->majorVersion) << 8) | (unsigned short)(fp->minorVersion))))
   {
      /* If we are an older version: Do not accept it */
      fp->errorCode = kFlashInitInvalidVersion;
   }
   else
   {
      /* Initialize the flash API and HCLK frequency in MHz */
      result = Fapi_initializeFlashBanks(fp->hclkFrequency);

      /* Fail if initialization was not successful*/
      if (Fapi_Status_Success != result)
      {
         fp->fsmResult = result;
         fp->errorCode = kFlashInitFailed;
      }
      else
      {
         /* Clear FSM status register */
         (void)Fapi_issueAsyncCommand(Fapi_ClearStatus);

         (void)Fapi_issueAsyncCommand(Fapi_ClearMore);

         Fapi_flushPipeline();

#if ! defined( FLASH_ENABLE_CUSTOM_SECTOR_SETUP )
         /* Must be called after Fapi_initializeAPI to initialize flashSectorInfo array for the currently used
          * controller. This is intended to dynamically retrieve MCU flash sector information with any bank and sector
          * sizes using F021 flash technology */
         fp->errorCode = GetFlashDeviceInfo();
#endif
      }
   }
}

/***********************************************************************************************************************
 *  ExpFlashDeinit
 **********************************************************************************************************************/
/*! \brief           Do deinitialization after flash programming
 *  \param[in,out]   fp Flash parameter handle
 **********************************************************************************************************************/
void ExpFlashDeinit( tFlashParam * fp )
{
   fp->errorCode = kFlashOk;
}

/***********************************************************************************************************************
 *  ExpFlashErase
 **********************************************************************************************************************/
/*! \brief           Erases one flash sector
 *  \param[in,out]   fp Flash parameter handle
 **********************************************************************************************************************/
void ExpFlashErase( tFlashParam * fp )
{
   unsigned char sectorNumber;
   unsigned char firstSector;
   unsigned char lastSector;
   Fapi_StatusType result;
   Fapi_FlashStatusType fsmStat;
   tFlashAddress * pstartAddress;
   unsigned int sectorsEnables_31_0;
   unsigned int sectorsEnables_63_32;

   localFlashParam = fp;

   (void)Fapi_serviceWatchdogTimer();

   firstSector = GetSectorNr(fp->address);

   lastSector = GetSectorNr(fp->address + (fp->length - 1u));

   if ((kFlashSectorNotFound == firstSector) || (kFlashSectorNotFound == lastSector))
   {
      fp->errorCode = kFlashEraseInvalidAddr;
      return;
   }

   /* Until now, there is no erase needed */

   for (sectorNumber = firstSector; sectorNumber <= lastSector; sectorNumber++)
   {
      /* Activate current bank for erase */
      result = Fapi_setActiveFlashBank(flashSectorInfo[sectorNumber].flashBank);

      if (Fapi_Status_Success != result)
      {
         fp->fsmResult = result;
         return;
      }
      else
      {
         if (Fapi_FlashBank7 == flashSectorInfo[firstSector].flashBank)
         {
            if (flashSectorInfo[firstSector].flashSector > 31u)
            {
               sectorsEnables_31_0 = 0u;
               sectorsEnables_63_32 = (1u << (flashSectorInfo[firstSector].flashSector - 32u));
            }
            else
            {
               sectorsEnables_31_0 = (1u << flashSectorInfo[firstSector].flashSector);
               sectorsEnables_63_32 = 0u;
            }

            (void)Fapi_enableEepromBankSectors(sectorsEnables_31_0, sectorsEnables_63_32);
         }
         else /* Fapi_FlashBank0 - Fapi_FlashBank6 */
         {
            (void)Fapi_enableMainBankSectors(flashSectorInfo[sectorNumber].flashSector);
         }

         do
         {
            (void)Fapi_serviceWatchdogTimer();
         }
         while (Fapi_Status_FsmReady != Fapi_checkFsmForReady());

         pstartAddress = (unsigned int *)flashSectorInfo[sectorNumber].startAddress;

         result= Fapi_issueAsyncCommandWithAddress(Fapi_EraseSector, pstartAddress) ;

         do
         {
            (void)Fapi_serviceWatchdogTimer();
         }
         while (Fapi_Status_FsmReady != Fapi_checkFsmForReady());

         fsmStat =  Fapi_getFsmStatus();

         if ((0u != fsmStat) || (Fapi_Status_Success != result))
         {
            fp->fsmResult = result;
            fp->errorCode = kFlashFctErase;
#if defined( FLASH_DRIVER_DEBUG )
            fp->fsmStat   = fsmStat;
            fp->errorAddress = (tFlashAddress) flashSectorInfo[sectorNumber].startAddress;
#endif /* FLASH_DRIVER_DEBUG */
            return;
         }
      }
   }

   /* Flush pipeline after erase operation */
   Fapi_flushPipeline();
}

/***********************************************************************************************************************
 *  ProgramFlashModeSelect
 **********************************************************************************************************************/
/*! \brief       Select programming mode to transfer data flash memory
 *  \param[in]   pStartAddress The start address
 *  \param[in]   pData Pointer to the data
 *  \param[in]   length Number of bytes to be programmed
 *  \return      Fapi_Status_Success is succeeded, otherwise an error code
 **********************************************************************************************************************/
static Fapi_StatusType ProgramFlashModeSelect( tFlashAddress * pStartAddress, unsigned char * pData, tFlashLength length )
{
   Fapi_StatusType result;

   switch (localFlashParam->programmingMode)
   {
      case Fapi_AutoEccGeneration: /* Intentional fall-through */
      case Fapi_DataOnly:
      {
         result = Fapi_issueProgrammingCommand(pStartAddress,
                                               pData,
                                               length,
                                               0u,
                                               0u,
                                               localFlashParam->programmingMode);
         break;
      }
      case Fapi_EccOnly:    /* Intentional fall-through */
      case Fapi_DataAndEcc: /* Intentional fall-through */
      default:
      {
         result = Fapi_Error_Fail;
         localFlashParam->errorCode = kFlashProgrammingModeNotSupported;
         return result;
      }
   }

   return result;
}

/***********************************************************************************************************************
 *  ProgramFlash
 **********************************************************************************************************************/
/*! \brief       Align and prepare data to be transfered to flash memory
 *  \param[in]   startAddress The start address
 *  \param[in]   pData Pointer to the data
 *  \param[in]   firstSector The first sector
 *  \return      Fapi_Status_Success is succeeded, otherwise an error code
 **********************************************************************************************************************/
static Fapi_StatusType ProgramFlash( tFlashAddress startAddress, unsigned char * pData, unsigned char firstSector )
{
   tFlashAddress i;
   tFlashAddress * pStartAddress;
   Fapi_StatusType result;
   Fapi_FlashStatusType fsmStat;
   unsigned char bankNr;
   unsigned int sectorsEnables_31_0;
   unsigned int sectorsEnables_63_32;

   /* Find the start byte and align if necessary */
   (void)Fapi_serviceWatchdogTimer();

   /* Initialize variables for flash program*/
   bankNr = flashSectorInfo[firstSector].flashBank;
   fsmStat = 0u;

   /* Set active flash bank to perform write, only one bank is activated at a time */
   result = Fapi_setActiveFlashBank((Fapi_FlashBankType)bankNr);

   if (Fapi_Status_Success == result)
   {
      if (Fapi_FlashBank7 == flashSectorInfo[firstSector].flashBank)
      {
         if (flashSectorInfo[firstSector].flashSector > 31u)
         {
            sectorsEnables_31_0 = 0u;
            sectorsEnables_63_32 = (1u << (flashSectorInfo[firstSector].flashSector - 32u));
         }
         else
         {
            sectorsEnables_31_0 = (1u << flashSectorInfo[firstSector].flashSector);
            sectorsEnables_63_32 = 0u;
         }

         (void)Fapi_enableEepromBankSectors(sectorsEnables_31_0, sectorsEnables_63_32);
      }
      else /* Fapi_FlashBank0 - Fapi_FlashBank6 */
      {
         (void)Fapi_enableMainBankSectors(flashSectorInfo[firstSector].flashSector);
      }

      do
      {
         (void)Fapi_serviceWatchdogTimer();
      }
      while (Fapi_Status_FsmReady != Fapi_checkFsmForReady());

      for (i = 0u; (i < localFlashParam->length) && (Fapi_Status_Success == result) && (0u == fsmStat); i += FLASH_SEGMENT_SIZE)
      {
         (void)Fapi_serviceWatchdogTimer();

         pStartAddress = (unsigned int *)((localFlashParam->address) + i);
         /* Program data to Flash memory based on programming mode*/
         result = ProgramFlashModeSelect(pStartAddress, pData, FLASH_SEGMENT_SIZE);
         /* Increment data pointer to the next write segment*/
         pData += FLASH_SEGMENT_SIZE;

         do
         {
            (void)Fapi_serviceWatchdogTimer();
         }
         while (Fapi_Status_FsmReady != Fapi_checkFsmForReady());

         fsmStat = Fapi_getFsmStatus();
      }
   }

#if defined( FLASH_DRIVER_DEBUG )
   localFlashParam->fsmStat = fsmStat;
#endif /* FLASH_DRIVER_DEBUG */

   return result;
}

/***********************************************************************************************************************
 *  VerifyFlash
 **********************************************************************************************************************/
/*! \brief       Verify flashed memory
 *  \param[in]   startAddress The start address
 *  \param[in]   pData Pointer to the data
 *  \param[in]   length The number of data bytes
 *  \return      Fapi_Status_Success is succeeded, otherwise an error code
 **********************************************************************************************************************/
static Fapi_StatusType VerifyFlash( tFlashAddress startAddress, unsigned char * pData, tFlashLength length )
{
   Fapi_StatusType result;
   Fapi_FlashStatusWordType flashStatus;
   unsigned int * pStartAddress;
   unsigned int * pCheckValueBuffer;
   unsigned int index;

   for (index = 0u; index < length; index += FLASH_SEGMENT_SIZE)
   {
      pStartAddress = (unsigned int *)(startAddress + index);
      pCheckValueBuffer = (unsigned int *)&pData[index];

      (void)Fapi_serviceWatchdogTimer();

      result = Fapi_doVerify(pStartAddress, FLASH_BYTES_TO_WORDS(FLASH_SEGMENT_SIZE), pCheckValueBuffer, &flashStatus);

      if (Fapi_Status_Success != result)
      {
         break;
      }
   }

#if defined( FLASH_DRIVER_DEBUG )
   if (Fapi_Status_Success != result)
   {
      /* Flash verify error*/
      localFlashParam->errorAddress = flashStatus.au32StatusWord[0u];
      localFlashParam->actualData = flashStatus.au32StatusWord[1u];
      localFlashParam->intendedData = flashStatus.au32StatusWord[2u];
   }
#endif /* FLASH_DRIVER_DEBUG */

   return result;
}

/***********************************************************************************************************************
 *  ExpFlashWrite
 **********************************************************************************************************************/
/*! \brief           Program the flash at the given address
 *  \param[in,out]   fp Flash parameter handle
 **********************************************************************************************************************/
void ExpFlashWrite( tFlashParam * fp )
{
   Fapi_StatusType result;
   tFlashAddress startAddress;
   unsigned char firstSector;
   tFlashData * pData;

   localFlashParam = fp;

   (void)Fapi_serviceWatchdogTimer();

   /* Initialize error code */
   fp->errorCode = kFlashOk;

   if (0u == fp->length)   /* GAP_FILL makes calls with length 0 */
   {
      return;
   }

   firstSector = GetSectorNr(fp->address);

   if (kFlashSectorNotFound == firstSector)
   {
      fp->errorCode = kFlashWriteInvalidAddr;
      return;
   }

   /* Save registers for verification after write */
   startAddress = fp->address;
   pData = fp->data;

   result = ProgramFlash(startAddress, pData, firstSector);

   /* Check the final result of program routine */
   if (   (Fapi_Status_Success != result)
#if defined( FLASH_DRIVER_DEBUG )
       || (0u != localFlashParam->fsmStat)
#endif /* FLASH_DRIVER_DEBUG */
      )
   {
#if defined( FLASH_DRIVER_DEBUG )
      fp->fsmStat   = localFlashParam->fsmStat;
#endif /* FLASH_DRIVER_DEBUG */
      fp->fsmResult = result;
      fp->errorCode = kFlashWriteFailed;
      return;
   }

   (void)Fapi_serviceWatchdogTimer();

   /* Flush pipeline after write operation */
   Fapi_flushPipeline();

   result = VerifyFlash(startAddress, pData, fp->length);

   if (Fapi_Status_Success != result)
   {
      /* Flash verify error*/
      fp->fsmResult = result;
      fp->errorCode = kFlashWriteVerify;
      return;
   }
}

/***********************************************************************************************************************
 *  GetSectorNr
 **********************************************************************************************************************/
/*! \brief       Finds the sector of the given address
 *  \param[in]   address The memory address
 *  \return      Sector number of sector to erase
 **********************************************************************************************************************/
static unsigned char GetSectorNr( unsigned int address )
{
   unsigned char sectorNumber;
   unsigned int startAddress;
   unsigned int endAddress;

   for (sectorNumber = 0u; sectorNumber < nrOfFlashSectors; sectorNumber++)
   {
      (void)Fapi_serviceWatchdogTimer();

      startAddress = flashSectorInfo[sectorNumber].startAddress;
      endAddress = flashSectorInfo[sectorNumber].startAddress + flashSectorInfo[sectorNumber].length - 1u;

      if ((address >= startAddress) && (address <= endAddress))
      {
         /* Found sector matching input address */
         break;
      }
   }

   if (nrOfFlashSectors == sectorNumber)
   {
      sectorNumber = kFlashSectorNotFound;
   }

   return sectorNumber;
}

/***********************************************************************************************************************
 *  main
 **********************************************************************************************************************/
/*! \brief       The linker needs this entry point to generate code.
 **********************************************************************************************************************/
int main( void )
{
   return 0;
}

#if ! defined( FLASH_ENABLE_CUSTOM_SECTOR_SETUP )
/***********************************************************************************************************************
 *  GetFlashDeviceInfo
 **********************************************************************************************************************/
/*! \brief       Initializes flash driver with necessary device bank and sector information that is used for program
 *               and erase operation.
 *  \return      kFlashOk if succeeded, otherwise an error code
 **********************************************************************************************************************/
static tFlashErrorCode GetFlashDeviceInfo( void )
{
   unsigned int sectorNr;
   unsigned int readAllSectors;
   unsigned int bankNr;
   Fapi_FlashBankSectorsType flashBankSectors;
   Fapi_StatusType status;
   tFlashErrorCode result;

   nrOfFlashSectors = 0u;
   result = kFlashOk;

   for (bankNr = 0u; bankNr < FLASH_MAX_BANKS; bankNr++)
   {
      status = Fapi_getBankSectors((Fapi_FlashBankType)bankNr, &flashBankSectors);

      /* Check if flash bank exists, there might be gaps in between */
      if (Fapi_Status_Success == status)
      {
         /*Populate flashSectorInfo for the current device (StartAddress, EndAddress, bankNr, SectorNr)*/
         sectorNr = 0u;
         readAllSectors = flashBankSectors.u32NumberOfSectors;

         if ((nrOfFlashSectors + readAllSectors) > FLASH_MAX_SECTOR_INFOS)
         {
            /* Abort in case the derivative indicates to have more sectors than our statically configured limit */
            result = kFlashInvalidSize;
            break;
         }

         while (sectorNr < readAllSectors)
         {
            (void)Fapi_serviceWatchdogTimer();

            if (0u == sectorNr)
            {
               flashSectorInfo[nrOfFlashSectors].startAddress = flashBankSectors.u32BankStartAddress;
            }
            else
            {
               flashSectorInfo[nrOfFlashSectors].startAddress =   flashSectorInfo[nrOfFlashSectors - 1u].startAddress
                                                                + flashSectorInfo[nrOfFlashSectors - 1u].length;
            }

            /* Find the actual sector length by multiplying the logical sector length */
            flashSectorInfo[nrOfFlashSectors].length = (flashBankSectors.au16SectorSizes[sectorNr]) << 10u;
            flashSectorInfo[nrOfFlashSectors].flashBank = (Fapi_FlashBankType)bankNr;
            flashSectorInfo[nrOfFlashSectors].flashSector = (Fapi_FlashSectorType)sectorNr;
            nrOfFlashSectors++;
            sectorNr++;
         }
      }
      else if (Fapi_Error_InvalidBank == status) 
      {
         /* Since TI Flash API does not provide useful information for Bank 7 sectors:
          * We abort asking for new sector information after the code flash banks (compatible with earlier version of this driver). 
          * This way, the sector information of the Bank7 sectors won't be included in the flashSectorInfo Table */
          break;
      }
      else
      {
         /* In all other cases, the API has encountered an error or the return value is unexpected, break and return error */
         result = kFlashFailed;
         break;
      }
   }

   return result;
}
#endif

/***********************************************************************************************************************
 *  Fapi_serviceWatchdogTimer
 **********************************************************************************************************************/
/*! \brief       Service the watchdog
 *  \note        This function is NOT called by the Flash API when writing to the flash memory
 *  \return      Fapi_Status_Success is succeeded, otherwise an error code
 **********************************************************************************************************************/
Fapi_StatusType Fapi_serviceWatchdogTimer( void )
{
   (void)(*localFlashParam->wdTriggerFct)();
   return Fapi_Status_Success;
}

/***********************************************************************************************************************
 *  Fapi_enableMainBankSectors
 **********************************************************************************************************************/
/*! \brief       This function sets up the sectors in the non Flash banks that are available for erase and programming
 *               operations.
 *  \param[in]   sectorsEnables Specifies which sectors shall be enabled
 *  \return      Fapi_Status_Success is succeeded, otherwise an error code
 **********************************************************************************************************************/
Fapi_StatusType Fapi_enableMainBankSectors( unsigned short sectorsEnables )
{
   (void)sectorsEnables;

   /* FMAC has been previously set by call to Fapi_setActiveFlashBank()
    * We ignore the argument and always enable access to all sectors of the active flash bank */

   FLASH_CONTROL_REGISTER->Fbprot.FBPROT_BITS.PROTL1DIS = 0x01u;
   FAPI_WRITE_LOCKED_FSM_REGISTER(FLASH_CONTROL_REGISTER->FsmSector.u32Register, 0u);  /* Enables sectors for bank and sector erase */
   FAPI_WRITE_LOCKED_FSM_REGISTER(FLASH_CONTROL_REGISTER->Fbse.u32Register, 0xFFFFu);  /* Enable sectors for erase and programming */
   FLASH_CONTROL_REGISTER->Fbprot.FBPROT_BITS.PROTL1DIS = 0x00u;

   return Fapi_Status_Success;
}

/***********************************************************************************************************************
 *  Fapi_enableEepromBankSectors
 **********************************************************************************************************************/
/*! \brief       This function sets up the sectors in the EEPROM bank that are available for erase and programming
 *               operations.
 *  \param[in]   sectorsEnables_31_0 Specifies which sectors shall be enabled
 *  \param[in]   sectorsEnables_63_32 Specifies which sectors shall be enabled
 *  \return      Fapi_Status_Success is succeeded, otherwise an error code
 **********************************************************************************************************************/
Fapi_StatusType Fapi_enableEepromBankSectors( unsigned int sectorsEnables_31_0, unsigned int sectorsEnables_63_32 )
{
   (void)sectorsEnables_31_0;
   (void)sectorsEnables_63_32;

   /* FMAC has been previously set by call to Fapi_setActiveFlashBank()
    * We ignore the argument and always enable access to all sectors of the active flash bank */

   FLASH_CONTROL_REGISTER->Fbprot.FBPROT_BITS.PROTL1DIS = 0x01u;
   FAPI_WRITE_LOCKED_FSM_REGISTER(FLASH_CONTROL_REGISTER->Fbse.u32Register, 0xFFFFu);   /* Enable sectors for erase and programming */
   FAPI_WRITE_LOCKED_FSM_REGISTER(FLASH_CONTROL_REGISTER->FsmSector1.u32Register, 0u);  /* Enables sectors for bank and sector erase */
   FAPI_WRITE_LOCKED_FSM_REGISTER(FLASH_CONTROL_REGISTER->FsmSector2.u32Register, 0u);  /* Enables sectors for bank and sector erase */
   FLASH_CONTROL_REGISTER->Fbprot.FBPROT_BITS.PROTL1DIS = 0x00u;

   return Fapi_Status_Success;
}

/***********************************************************************************************************************
 *  END OF FILE: FLASHDRV.C
 **********************************************************************************************************************/
